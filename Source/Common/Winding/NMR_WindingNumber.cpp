/*++

Copyright (C) 2026 tensorGrad Consulting LLP

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

Internal generalized winding number helper used by boolean processing.
Copyright remains with tensorGrad Consulting LLP; this file is licensed under
the BSD terms above for inclusion in lib3mf.

--*/

#include "Common/Winding/NMR_WindingNumber.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <thread>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #define TG_X86 1
#else
    #define TG_X86 0
#endif

#if TG_X86 && (defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER))
    #include <immintrin.h>
#endif

#if defined(TGWN_DISABLE_AVX2)
    #define TG_HAS_AVX2 0
#elif TG_X86 && (defined(__GNUC__) || defined(__clang__))
    #define TG_HAS_AVX2 1
#elif TG_X86 && defined(_MSC_VER) && defined(__AVX2__)
    #define TG_HAS_AVX2 1
#else
    #define TG_HAS_AVX2 0
#endif

#define TINYBVH_IMPLEMENTATION
#include "tiny_bvh.h"
#include "nanoflann.hpp"

namespace tg {
    // Implementation details:
    // - Builds a BVH over triangles (mesh) or points (point cloud).
    // - Stores Taylor expansion moments per BVH node.
    // - Evaluates using approximation for far nodes and exact contributions for leaves.
    struct windingNumber::Impl {
        using Point = windingNumber::Point;
        using Triangle = std::array<unsigned int, 3>;

        struct vec3 {
            double x, y, z;

            vec3() : x(0), y(0), z(0) {
            }

            vec3(double X, double Y, double Z) : x(X), y(Y), z(Z) {
            }

            vec3 operator+(const vec3 &o) const { return {x + o.x, y + o.y, z + o.z}; }
            vec3 operator-(const vec3 &o) const { return {x - o.x, y - o.y, z - o.z}; }
            vec3 operator*(double s) const { return {x * s, y * s, z * s}; }
        };

        struct aabb {
            vec3 mn, mx;
        };

        // Per-node aggregates for Taylor expansion about averageP.
        // N = area-weighted normal sum (mesh) or dipole sum (points).
        // Nij/Nijk terms match the "Fast Winding Numbers for Soups and Clouds" expansion.
        struct nodeAgg {
            bool isLeaf = false;
            uint64_t left = 0, right = 0;
            uint64_t firstPrim = 0, primCount = 0;

            vec3 averageP{}, N{};
            double maxPDist2 = 0.0;

            vec3 NijDiag{};
            double Nxy_Nyx = 0, Nyz_Nzy = 0, Nzx_Nxz = 0;

            vec3 NijkDiag{};
            double sumPermuteNxyz = 0;
            double twoNxxy_Nyxx = 0, twoNxxz_Nzxx = 0;
            double twoNyyz_Nzyy = 0, twoNyyx_Nxyy = 0;
            double twoNzzx_Nxzz = 0, twoNzzy_Nyzz = 0;
        };

        // Temporary build data used while constructing aggregates bottom-up.
        struct buildData {
            aabb box{};
            vec3 averageP{}, areaP{}, N{};
            double area = 0, maxPDist2 = 0;

            vec3 NijDiag{};
            double Nxy = 0, Nyx = 0, Nyz = 0, Nzy = 0, Nzx = 0, Nxz = 0;

            vec3 NijkDiag{};
            double sumPermuteNxyz = 0;
            double twoNxxy_Nyxx = 0, twoNxxz_Nzxx = 0;
            double twoNyyz_Nzyy = 0, twoNyyx_Nxyy = 0;
            double twoNzzx_Nxzz = 0, twoNzzy_Nyzz = 0;
        };

        // KD-tree adaptor for nanoflann (point cloud normal/area estimation).
        struct PointCloudAdaptor {
            const std::vector<Point> *pts;

            PointCloudAdaptor(const std::vector<Point> &p) : pts(&p) {
            }

            size_t kdtree_get_point_count() const { return pts->size(); }
            double kdtree_get_pt(size_t idx, size_t dim) const { return (*pts)[idx][dim]; }

            template<class BBOX>
            bool kdtree_get_bbox(BBOX &) const { return false; }
        };

        using KDTree = nanoflann::KDTreeSingleIndexAdaptor<
            nanoflann::L2_Simple_Adaptor<double, PointCloudAdaptor>,
            PointCloudAdaptor, 3, size_t>;

        enum class Mode { Mesh, PointCloud };

        Mode mode = Mode::Mesh;

        Settings settings;

        // Mesh data (triangle soup).
        std::vector<Point> vertices;
        std::vector<Triangle> triangles;
        std::vector<vec3> triVecArea, triCentroid;
        std::vector<aabb> triBounds;

        // Point cloud data (oriented points).
        std::vector<Point> cloudPoints;
        std::vector<vec3> cloudNormals; // Unit normals.
        std::vector<double> cloudAreas; // Area per point.
        std::vector<vec3> cloudDipoles; // Area-weighted normal (A * n).
        std::vector<aabb> cloudBounds; // Degenerate AABB per point.

        // BVH (shared for mesh and point cloud modes).
        tinybvh::BVH_Double bvh;
        std::vector<tinybvh::bvhdbl3> triVertsPacked; // For mesh mode
        std::vector<nodeAgg> nodes;

        static constexpr double pi() { return 3.141592653589793238462643383279502884; }
        static constexpr int kPacketSize = 4;

        static vec3 toVec3(const Point &v) { return {v[0], v[1], v[2]}; }
        static double dot(const vec3 &a, const vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

        static vec3 cross(const vec3 &a, const vec3 &b) {
            return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
        }

        static double norm(const vec3 &a) { return std::sqrt(dot(a, a)); }

        static aabb triAabb(const vec3 &a, const vec3 &b, const vec3 &c) {
            return {
                {std::min({a.x, b.x, c.x}), std::min({a.y, b.y, c.y}), std::min({a.z, b.z, c.z})},
                {std::max({a.x, b.x, c.x}), std::max({a.y, b.y, c.y}), std::max({a.z, b.z, c.z})}
            };
        }

        static vec3 aabbCenter(const aabb &bb) { return (bb.mn + bb.mx) * 0.5; }

        static aabb aabbUnion(const aabb &a, const aabb &b) {
            return {
                {std::min(a.mn.x, b.mn.x), std::min(a.mn.y, b.mn.y), std::min(a.mn.z, b.mn.z)},
                {std::max(a.mx.x, b.mx.x), std::max(a.mx.y, b.mx.y), std::max(a.mx.z, b.mx.z)}
            };
        }

        static double maxPDist2FromBox(const aabb &bb, const vec3 &p) {
            double dx = std::max(std::abs(p.x - bb.mn.x), std::abs(bb.mx.x - p.x));
            double dy = std::max(std::abs(p.y - bb.mn.y), std::abs(bb.mx.y - p.y));
            double dz = std::max(std::abs(p.z - bb.mn.z), std::abs(bb.mx.z - p.z));
            return dx * dx + dy * dy + dz * dz;
        }

        uint64_t primIndex(uint64_t i) const { return bvh.primIdx[i]; }


        // Estimate per-point normals using PCA on k nearest neighbors.
        void estimateNormals(int k) {
            size_t N = cloudPoints.size();
            cloudNormals.resize(N);

            PointCloudAdaptor adaptor(cloudPoints);
            KDTree kdtree(3, adaptor, nanoflann::KDTreeSingleIndexAdaptorParams(10));
            kdtree.buildIndex();

            // Compute centroid for orientation
            vec3 centroid{};
            for (const auto &p: cloudPoints) {
                centroid = centroid + toVec3(p);
            }
            centroid = centroid * (1.0 / N);

            std::vector<size_t> indices(k + 1);
            std::vector<double> dists(k + 1);

            for (size_t i = 0; i < N; ++i) {
                size_t found = kdtree.knnSearch(cloudPoints[i].data(), k + 1, indices.data(), dists.data());

                // Compute local centroid of neighbors
                vec3 localCentroid{};
                size_t count = 0;
                for (size_t j = 0; j < found; ++j) {
                    localCentroid = localCentroid + toVec3(cloudPoints[indices[j]]);
                    ++count;
                }
                if (count > 0) localCentroid = localCentroid * (1.0 / count);

                // Build covariance matrix
                double cov[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
                for (size_t j = 0; j < found; ++j) {
                    vec3 d = toVec3(cloudPoints[indices[j]]) - localCentroid;
                    cov[0][0] += d.x * d.x;
                    cov[0][1] += d.x * d.y;
                    cov[0][2] += d.x * d.z;
                    cov[1][0] += d.y * d.x;
                    cov[1][1] += d.y * d.y;
                    cov[1][2] += d.y * d.z;
                    cov[2][0] += d.z * d.x;
                    cov[2][1] += d.z * d.y;
                    cov[2][2] += d.z * d.z;
                }

                // Find smallest eigenvector using power iteration on inverse
                // (or just use the cross product of two largest eigenvectors)
                // Simplified: use iterative method to find normal
                vec3 normal = computeSmallestEigenvector(cov);

                // Orient normal to point away from global centroid
                vec3 toPoint = toVec3(cloudPoints[i]) - centroid;
                if (dot(normal, toPoint) < 0) {
                    normal = normal * -1.0;
                }

                cloudNormals[i] = normal;
            }
        }

        // Compute smallest eigenvector of 3x3 symmetric matrix using Jacobi iteration.
        static vec3 computeSmallestEigenvector(double cov[3][3]) {
            // Copy matrix
            double a[3][3];
            double v[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // Eigenvectors

            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    a[i][j] = cov[i][j];

            // Jacobi iteration
            for (int iter = 0; iter < 50; ++iter) {
                // Find largest off-diagonal element
                int p = 0, q = 1;
                double maxVal = std::abs(a[0][1]);
                if (std::abs(a[0][2]) > maxVal) {
                    maxVal = std::abs(a[0][2]);
                    p = 0;
                    q = 2;
                }
                if (std::abs(a[1][2]) > maxVal) {
                    maxVal = std::abs(a[1][2]);
                    p = 1;
                    q = 2;
                }

                if (maxVal < 1e-15) break;

                // Compute rotation
                double theta = 0.5 * std::atan2(2.0 * a[p][q], a[q][q] - a[p][p]);
                double c = std::cos(theta), s = std::sin(theta);

                // Apply rotation to a
                double app = a[p][p], aqq = a[q][q], apq = a[p][q];
                a[p][p] = c * c * app - 2 * c * s * apq + s * s * aqq;
                a[q][q] = s * s * app + 2 * c * s * apq + c * c * aqq;
                a[p][q] = a[q][p] = 0;

                for (int i = 0; i < 3; ++i) {
                    if (i != p && i != q) {
                        double aip = a[i][p], aiq = a[i][q];
                        a[i][p] = a[p][i] = c * aip - s * aiq;
                        a[i][q] = a[q][i] = s * aip + c * aiq;
                    }
                }

                // Apply rotation to eigenvectors
                for (int i = 0; i < 3; ++i) {
                    double vip = v[i][p], viq = v[i][q];
                    v[i][p] = c * vip - s * viq;
                    v[i][q] = s * vip + c * viq;
                }
            }

            // Find index of smallest eigenvalue
            int minIdx = 0;
            if (a[1][1] < a[minIdx][minIdx]) minIdx = 1;
            if (a[2][2] < a[minIdx][minIdx]) minIdx = 2;

            vec3 normal{v[0][minIdx], v[1][minIdx], v[2][minIdx]};
            double len = norm(normal);
            if (len > 0) normal = normal * (1.0 / len);
            return normal;
        }

        // Build mesh precomputes + BVH + per-node aggregates.
        void build() {
            size_t T = triangles.size();
            triVecArea.resize(T);
            triCentroid.resize(T);
            triBounds.resize(T);
            triVertsPacked.clear();
            triVertsPacked.reserve(T * 3);

            for (size_t t = 0; t < T; ++t) {
                const auto &idx = triangles[t];
                vec3 a = toVec3(vertices[idx[0]]);
                vec3 b = toVec3(vertices[idx[1]]);
                vec3 c = toVec3(vertices[idx[2]]);
                triVecArea[t] = cross(b - a, c - a) * 0.5;
                triCentroid[t] = (a + b + c) * (1.0 / 3.0);
                triBounds[t] = triAabb(a, b, c);
                triVertsPacked.emplace_back(a.x, a.y, a.z);
                triVertsPacked.emplace_back(b.x, b.y, b.z);
                triVertsPacked.emplace_back(c.x, c.y, c.z);
            }

            bvh.Build(triVertsPacked.data(), (uint64_t) T);
            buildAggregates();
        }

        // Estimate per-point areas using k-nearest neighbors.
        void estimateAreas(int k) {
            size_t N = cloudPoints.size();
            cloudAreas.resize(N);

            PointCloudAdaptor adaptor(cloudPoints);
            KDTree kdtree(3, adaptor, nanoflann::KDTreeSingleIndexAdaptorParams(10));
            kdtree.buildIndex();

            std::vector<size_t> indices(k + 1);
            std::vector<double> dists(k + 1);

            for (size_t i = 0; i < N; ++i) {
                // Query k+1 neighbors (includes self)
                size_t found = kdtree.knnSearch(cloudPoints[i].data(), k + 1, indices.data(), dists.data());

                // Average distance to neighbors (excluding self at index 0)
                double avgDist = 0;
                size_t count = 0;
                for (size_t j = 1; j < found; ++j) {
                    avgDist += std::sqrt(dists[j]);
                    ++count;
                }
                if (count > 0) avgDist /= count;

                // Area ~ π * r² where r is average neighbor distance
                // This gives area of a disk with radius = avgDist
                cloudAreas[i] = pi() * avgDist * avgDist;
            }
        }

        // Build point cloud BVH + aggregates (dipole model).
        void buildPointCloud() {
            size_t N = cloudPoints.size();
            cloudNormals.resize(N);
            cloudDipoles.resize(N);
            cloudBounds.resize(N);
            triVertsPacked.clear();
            triVertsPacked.reserve(N * 3);

            // Compute dipoles and create degenerate triangles for BVH
            for (size_t i = 0; i < N; ++i) {
                vec3 p = toVec3(cloudPoints[i]);

                // Dipole = area * normal
                cloudDipoles[i] = cloudNormals[i] * cloudAreas[i];

                // Degenerate AABB (point)
                cloudBounds[i] = {p, p};

                // Create a tiny degenerate triangle for BVH building
                // All three vertices are the same point
                triVertsPacked.emplace_back(p.x, p.y, p.z);
                triVertsPacked.emplace_back(p.x, p.y, p.z);
                triVertsPacked.emplace_back(p.x, p.y, p.z);
            }

            bvh.Build(triVertsPacked.data(), (uint64_t) N);
            buildAggregatesPointCloud();
        }

        // Build aggregates for point cloud mode (dipole moments).
        void buildAggregatesPointCloud() {
            uint64_t nodeCount = bvh.usedNodes;
            nodes.assign(nodeCount, nodeAgg{});
            std::vector<buildData> bd(nodeCount);

            for (uint64_t i = 0; i < nodeCount; ++i) {
                const auto &n = bvh.bvhNode[i];
                auto &nd = nodes[i];
                nd.isLeaf = n.triCount > 0;
                if (nd.isLeaf) {
                    nd.firstPrim = n.leftFirst;
                    nd.primCount = n.triCount;
                } else {
                    nd.left = n.leftFirst;
                    nd.right = n.leftFirst + 1;
                }
            }

            int order = std::clamp(settings.taylorOrder, 0, 2);

            for (uint64_t i = nodeCount; i-- > 0;) {
                auto &nd = nodes[i];
                auto &out = bd[i];

                if (nd.isLeaf) {
                    bool hasBox = false;
                    vec3 areaP{}, Nsum{};
                    double area = 0;

                    for (uint64_t t = 0; t < nd.primCount; ++t) {
                        uint64_t ptId = primIndex(nd.firstPrim + t);
                        vec3 p = toVec3(cloudPoints[ptId]);
                        vec3 dipole = cloudDipoles[ptId];
                        double ptArea = cloudAreas[ptId];

                        if (!hasBox) {
                            out.box = {p, p};
                            hasBox = true;
                        } else {
                            out.box.mn.x = std::min(out.box.mn.x, p.x);
                            out.box.mn.y = std::min(out.box.mn.y, p.y);
                            out.box.mn.z = std::min(out.box.mn.z, p.z);
                            out.box.mx.x = std::max(out.box.mx.x, p.x);
                            out.box.mx.y = std::max(out.box.mx.y, p.y);
                            out.box.mx.z = std::max(out.box.mx.z, p.z);
                        }

                        areaP = areaP + p * ptArea;
                        area += ptArea;
                        Nsum = Nsum + dipole;
                    }

                    out.areaP = areaP;
                    out.area = area;
                    out.N = Nsum;
                    out.averageP = area > 0 ? areaP * (1 / area) : aabbCenter(out.box);
                    out.maxPDist2 = maxPDist2FromBox(out.box, out.averageP);

                    // For points, Nijk terms about the point itself are zero
                    // We only need to apply the shift
                    for (uint64_t t = 0; t < nd.primCount; ++t) {
                        uint64_t ptId = primIndex(nd.firstPrim + t);
                        buildData child{};
                        child.averageP = toVec3(cloudPoints[ptId]);
                        child.N = cloudDipoles[ptId];
                        // Nij, Nijk are all zero for a single point about its own center
                        applyShift(out, child, child.averageP - out.averageP, order);
                    }
                } else {
                    const auto &L = bd[nd.left];
                    const auto &R = bd[nd.right];
                    out.box = aabbUnion(L.box, R.box);
                    out.areaP = L.areaP + R.areaP;
                    out.area = L.area + R.area;
                    out.N = L.N + R.N;
                    out.averageP = out.area > 0 ? out.areaP * (1 / out.area) : aabbCenter(out.box);
                    out.maxPDist2 = maxPDist2FromBox(out.box, out.averageP);

                    if (order >= 1) out.NijDiag = L.NijDiag + R.NijDiag;
                    if (order >= 2) {
                        out.NijkDiag = L.NijkDiag + R.NijkDiag;
                        out.sumPermuteNxyz = L.sumPermuteNxyz + R.sumPermuteNxyz;
                        out.twoNxxy_Nyxx = L.twoNxxy_Nyxx + R.twoNxxy_Nyxx;
                        out.twoNxxz_Nzxx = L.twoNxxz_Nzxx + R.twoNxxz_Nzxx;
                        out.twoNyyz_Nzyy = L.twoNyyz_Nzyy + R.twoNyyz_Nzyy;
                        out.twoNyyx_Nxyy = L.twoNyyx_Nxyy + R.twoNyyx_Nxyy;
                        out.twoNzzx_Nxzz = L.twoNzzx_Nxzz + R.twoNzzx_Nxzz;
                        out.twoNzzy_Nyzz = L.twoNzzy_Nyzz + R.twoNzzy_Nyzz;
                    }
                    applyShift(out, L, L.averageP - out.averageP, order);
                    applyShift(out, R, R.averageP - out.averageP, order);
                }

                nd.averageP = out.averageP;
                nd.N = out.N;
                nd.maxPDist2 = out.maxPDist2;
                nd.NijDiag = out.NijDiag;
                nd.Nxy_Nyx = out.Nxy + out.Nyx;
                nd.Nyz_Nzy = out.Nyz + out.Nzy;
                nd.Nzx_Nxz = out.Nzx + out.Nxz;
                nd.NijkDiag = out.NijkDiag;
                nd.sumPermuteNxyz = out.sumPermuteNxyz;
                nd.twoNxxy_Nyxx = out.twoNxxy_Nyxx;
                nd.twoNxxz_Nzxx = out.twoNxxz_Nzxx;
                nd.twoNyyz_Nzyy = out.twoNyyz_Nzyy;
                nd.twoNyyx_Nxyy = out.twoNyyx_Nxyy;
                nd.twoNzzx_Nxzz = out.twoNzzx_Nxzz;
                nd.twoNzzy_Nyzz = out.twoNzzy_Nyzz;
            }
        }

        // Dipole contribution for a single oriented point.
        static double dipole(const vec3 &dipoleVec, const vec3 &ptPos, const vec3 &queryPos) {
            vec3 r = ptPos - queryPos;
            double r2 = dot(r, r);
            if (r2 == 0) return 0;
            double r3 = r2 * std::sqrt(r2);
            return dot(dipoleVec, r) / r3;
        }

        // Exact solid angle for a triangle at query point p.
        static double solidAngle(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &p) {
            vec3 qa = a - p, qb = b - p, qc = c - p;
            double al = norm(qa), bl = norm(qb), cl = norm(qc);
            if (al == 0 || bl == 0 || cl == 0) return 0;
            vec3 qan = qa * (1 / al), qbn = qb * (1 / bl), qcn = qc * (1 / cl);
            double num = dot(qan, cross(qbn - qan, qcn - qan));
            if (num == 0) return 0;
            double den = 1 + dot(qan, qbn) + dot(qan, qcn) + dot(qbn, qcn);
            return 2 * std::atan2(num, den);
        }

        // Compute second-order triangle integrals for Taylor terms (order 2).
        void computeNijkTerms(const vec3 &a, const vec3 &b, const vec3 &c,
                              const vec3 &P, const vec3 &N, buildData &out) {
            double area = norm(N);
            if (area == 0) return;
            vec3 n = N * (1 / area);

            vec3 vals[3] = {a, b, c};
            int ox[3] = {0, 1, 2}, oy[3] = {0, 1, 2}, oz[3] = {0, 1, 2};

            auto sort3 = [&](int *o, auto get) {
                if (get(vals[0]) > get(vals[1])) std::swap(o[0], o[1]);
                if (get(vals[o[0]]) > get(vals[2])) std::swap(o[0], o[2]);
                if (get(vals[o[1]]) > get(vals[o[2]])) std::swap(o[1], o[2]);
            };
            sort3(ox, [](const vec3 &v) { return v.x; });
            sort3(oy, [](const vec3 &v) { return v.y; });
            sort3(oz, [](const vec3 &v) { return v.z; });

            double dx = vals[ox[2]].x - vals[ox[0]].x;
            double dy = vals[oy[2]].y - vals[oy[0]].y;
            double dz = vals[oz[2]].z - vals[oz[0]].z;

            auto getComp = [](const vec3 &v, int i) { return i == 0 ? v.x : (i == 1 ? v.y : v.z); };

            auto integrate = [&](const vec3 &aa, const vec3 &bb, const vec3 &cc,
                                 double *ii, double *ij, double *ik, int i) {
                vec3 oab = bb - aa, oac = cc - aa, ocb = bb - cc;
                double oac_i = getComp(oac, i), oab_i = getComp(oab, i);
                if (oac_i == 0) return;
                double t = oab_i / oac_i;
                int j = (i + 1) % 3, k = (i + 2) % 3;
                double jdiff = t * getComp(oac, j) - getComp(oab, j);
                double kdiff = t * getComp(oac, k) - getComp(oab, k);

                vec3 ca{jdiff * getComp(oab, k) - kdiff * getComp(oab, j), kdiff * oab_i, jdiff * oab_i};
                vec3 cc_{
                    jdiff * getComp(ocb, k) - kdiff * getComp(ocb, j), kdiff * getComp(ocb, i), jdiff * getComp(ocb, i)
                };
                double sa = norm(ca), sc = norm(cc_);
                double Pai = getComp(aa, i) - getComp(P, i), Pci = getComp(cc, i) - getComp(P, i);
                double ocb_i = getComp(ocb, i);

                *ii = sa * (0.5 * Pai * Pai + (2.0 / 3) * Pai * oab_i + 0.25 * oab_i * oab_i)
                      + sc * (0.5 * Pci * Pci + (2.0 / 3) * Pci * ocb_i + 0.25 * ocb_i * ocb_i);

                for (int jk: {j, k}) {
                    double *out_ptr = (jk == j) ? ij : ik;
                    double diff = (jk == j) ? jdiff : kdiff;
                    if (!out_ptr) continue;
                    double bmid = getComp(bb, jk) + 0.5 * diff;
                    double Paj = getComp(aa, jk) - getComp(P, jk), Pcj = getComp(cc, jk) - getComp(P, jk);
                    *out_ptr = sa * (0.5 * Pai * Paj + (1.0 / 3) * Pai * (bmid - getComp(aa, jk)) + (1.0 / 3) * Paj *
                                     oab_i + 0.25 * oab_i * (bmid - getComp(aa, jk)))
                               + sc * (0.5 * Pci * Pcj + (1.0 / 3) * Pci * (bmid - getComp(cc, jk)) + (1.0 / 3) * Pcj *
                                       ocb_i + 0.25 * ocb_i * (bmid - getComp(cc, jk)));
                }
            };

            double ixx = 0, ixy = 0, iyy = 0, iyz = 0, izz = 0, izx = 0;
            if (dx > 0)
                integrate(vals[ox[0]], vals[ox[1]], vals[ox[2]], &ixx,
                          (dx >= dy && dy > 0) ? &ixy : nullptr, (dx >= dz && dz > 0) ? &izx : nullptr, 0);
            if (dy > 0)
                integrate(vals[oy[0]], vals[oy[1]], vals[oy[2]], &iyy,
                          (dy >= dz && dz > 0) ? &iyz : nullptr, (dx < dy && dx > 0) ? &ixy : nullptr, 1);
            if (dz > 0)
                integrate(vals[oz[0]], vals[oz[1]], vals[oz[2]], &izz,
                          (dx < dz && dx > 0) ? &izx : nullptr, (dy < dz && dy > 0) ? &iyz : nullptr, 2);

            out.NijkDiag.x += ixx * n.x;
            out.NijkDiag.y += iyy * n.y;
            out.NijkDiag.z += izz * n.z;
            out.sumPermuteNxyz += 2 * (n.x * iyz + n.y * izx + n.z * ixy);
            out.twoNxxy_Nyxx += 2 * n.x * ixy + n.y * ixx;
            out.twoNxxz_Nzxx += 2 * n.x * izx + n.z * ixx;
            out.twoNyyz_Nzyy += 2 * n.y * iyz + n.z * iyy;
            out.twoNyyx_Nxyy += 2 * n.y * ixy + n.x * iyy;
            out.twoNzzx_Nxzz += 2 * n.z * izx + n.x * izz;
            out.twoNzzy_Nyzz += 2 * n.z * iyz + n.y * izz;
        }

        // Shift child moments to parent's expansion center.
        void applyShift(buildData &parent, const buildData &child, const vec3 &d, int order) {
            const vec3 &N = child.N;
            parent.NijDiag.x += N.x * d.x;
            parent.NijDiag.y += N.y * d.y;
            parent.NijDiag.z += N.z * d.z;

            double Nxy = child.Nxy + N.x * d.y, Nyx = child.Nyx + N.y * d.x;
            double Nyz = child.Nyz + N.y * d.z, Nzy = child.Nzy + N.z * d.y;
            double Nzx = child.Nzx + N.z * d.x, Nxz = child.Nxz + N.x * d.z;
            parent.Nxy += Nxy;
            parent.Nyx += Nyx;
            parent.Nyz += Nyz;
            parent.Nzy += Nzy;
            parent.Nzx += Nzx;
            parent.Nxz += Nxz;

            if (order < 2) return;

            parent.NijkDiag.x += 2 * d.x * child.NijDiag.x + d.x * d.x * N.x;
            parent.NijkDiag.y += 2 * d.y * child.NijDiag.y + d.y * d.y * N.y;
            parent.NijkDiag.z += 2 * d.z * child.NijDiag.z + d.z * d.z * N.z;
            parent.sumPermuteNxyz += d.x * (Nyz + Nzy) + d.y * (Nzx + Nxz) + d.z * (Nxy + Nyx);
            parent.twoNxxy_Nyxx += 2 * (d.y * child.NijDiag.x + d.x * child.Nxy + N.x * d.x * d.y) + 2 * child.Nyx * d.x
                    + N.y * d.x * d.x;
            parent.twoNxxz_Nzxx += 2 * (d.z * child.NijDiag.x + d.x * child.Nxz + N.x * d.x * d.z) + 2 * child.Nzx * d.x
                    + N.z * d.x * d.x;
            parent.twoNyyz_Nzyy += 2 * (d.z * child.NijDiag.y + d.y * child.Nyz + N.y * d.y * d.z) + 2 * child.Nzy * d.y
                    + N.z * d.y * d.y;
            parent.twoNyyx_Nxyy += 2 * (d.x * child.NijDiag.y + d.y * child.Nyx + N.y * d.y * d.x) + 2 * child.Nxy * d.y
                    + N.x * d.y * d.y;
            parent.twoNzzx_Nxzz += 2 * (d.x * child.NijDiag.z + d.z * child.Nzx + N.z * d.z * d.x) + 2 * child.Nxz * d.z
                    + N.x * d.z * d.z;
            parent.twoNzzy_Nyzz += 2 * (d.y * child.NijDiag.z + d.z * child.Nzy + N.z * d.z * d.y) + 2 * child.Nyz * d.z
                    + N.y * d.z * d.z;
        }

        // Build aggregates for mesh mode.
        void buildAggregates() {
            uint64_t nodeCount = bvh.usedNodes;
            nodes.assign(nodeCount, nodeAgg{});
            std::vector<buildData> bd(nodeCount);

            for (uint64_t i = 0; i < nodeCount; ++i) {
                const auto &n = bvh.bvhNode[i];
                auto &nd = nodes[i];
                nd.isLeaf = n.triCount > 0;
                if (nd.isLeaf) {
                    nd.firstPrim = n.leftFirst;
                    nd.primCount = n.triCount;
                } else {
                    nd.left = n.leftFirst;
                    nd.right = n.leftFirst + 1;
                }
            }

            int order = std::clamp(settings.taylorOrder, 0, 2);

            for (uint64_t i = nodeCount; i-- > 0;) {
                auto &nd = nodes[i];
                auto &out = bd[i];

                if (nd.isLeaf) {
                    bool hasBox = false;
                    vec3 areaP{}, Nsum{};
                    double area = 0;

                    for (uint64_t t = 0; t < nd.primCount; ++t) {
                        uint64_t triId = primIndex(nd.firstPrim + t);
                        if (!hasBox) {
                            out.box = triBounds[triId];
                            hasBox = true;
                        } else out.box = aabbUnion(out.box, triBounds[triId]);
                        vec3 N = triVecArea[triId];
                        double ta = norm(N);
                        areaP = areaP + triCentroid[triId] * ta;
                        area += ta;
                        Nsum = Nsum + N;
                    }

                    out.areaP = areaP;
                    out.area = area;
                    out.N = Nsum;
                    out.averageP = area > 0 ? areaP * (1 / area) : aabbCenter(out.box);
                    out.maxPDist2 = maxPDist2FromBox(out.box, out.averageP);

                    for (uint64_t t = 0; t < nd.primCount; ++t) {
                        uint64_t triId = primIndex(nd.firstPrim + t);
                        buildData child{};
                        child.averageP = triCentroid[triId];
                        child.N = triVecArea[triId];

                        if (order >= 2) {
                            const auto &idx = triangles[triId];
                            computeNijkTerms(toVec3(vertices[idx[0]]), toVec3(vertices[idx[1]]),
                                             toVec3(vertices[idx[2]]), child.averageP, child.N, child);
                            out.NijkDiag = out.NijkDiag + child.NijkDiag;
                            out.sumPermuteNxyz += child.sumPermuteNxyz;
                            out.twoNxxy_Nyxx += child.twoNxxy_Nyxx;
                            out.twoNxxz_Nzxx += child.twoNxxz_Nzxx;
                            out.twoNyyz_Nzyy += child.twoNyyz_Nzyy;
                            out.twoNyyx_Nxyy += child.twoNyyx_Nxyy;
                            out.twoNzzx_Nxzz += child.twoNzzx_Nxzz;
                            out.twoNzzy_Nyzz += child.twoNzzy_Nyzz;
                        }
                        applyShift(out, child, child.averageP - out.averageP, order);
                    }
                } else {
                    const auto &L = bd[nd.left];
                    const auto &R = bd[nd.right];
                    out.box = aabbUnion(L.box, R.box);
                    out.areaP = L.areaP + R.areaP;
                    out.area = L.area + R.area;
                    out.N = L.N + R.N;
                    out.averageP = out.area > 0 ? out.areaP * (1 / out.area) : aabbCenter(out.box);
                    out.maxPDist2 = maxPDist2FromBox(out.box, out.averageP);

                    if (order >= 1) out.NijDiag = L.NijDiag + R.NijDiag;
                    if (order >= 2) {
                        out.NijkDiag = L.NijkDiag + R.NijkDiag;
                        out.sumPermuteNxyz = L.sumPermuteNxyz + R.sumPermuteNxyz;
                        out.twoNxxy_Nyxx = L.twoNxxy_Nyxx + R.twoNxxy_Nyxx;
                        out.twoNxxz_Nzxx = L.twoNxxz_Nzxx + R.twoNxxz_Nzxx;
                        out.twoNyyz_Nzyy = L.twoNyyz_Nzyy + R.twoNyyz_Nzyy;
                        out.twoNyyx_Nxyy = L.twoNyyx_Nxyy + R.twoNyyx_Nxyy;
                        out.twoNzzx_Nxzz = L.twoNzzx_Nxzz + R.twoNzzx_Nxzz;
                        out.twoNzzy_Nyzz = L.twoNzzy_Nyzz + R.twoNzzy_Nyzz;
                    }
                    applyShift(out, L, L.averageP - out.averageP, order);
                    applyShift(out, R, R.averageP - out.averageP, order);
                }

                nd.averageP = out.averageP;
                nd.N = out.N;
                nd.maxPDist2 = out.maxPDist2;
                nd.NijDiag = out.NijDiag;
                nd.Nxy_Nyx = out.Nxy + out.Nyx;
                nd.Nyz_Nzy = out.Nyz + out.Nzy;
                nd.Nzx_Nxz = out.Nzx + out.Nxz;
                nd.NijkDiag = out.NijkDiag;
                nd.sumPermuteNxyz = out.sumPermuteNxyz;
                nd.twoNxxy_Nyxx = out.twoNxxy_Nyxx;
                nd.twoNxxz_Nzxx = out.twoNxxz_Nzxx;
                nd.twoNyyz_Nzyy = out.twoNyyz_Nzyy;
                nd.twoNyyx_Nxyy = out.twoNyyx_Nxyy;
                nd.twoNzzx_Nxzz = out.twoNzzx_Nxzz;
                nd.twoNzzy_Nyzz = out.twoNzzy_Nyzz;
            }
        }

        static std::vector<uint64_t> &getStack() {
            thread_local std::vector<uint64_t> stack;
            stack.clear();
            stack.reserve(128);
            return stack;
        }

#if TG_HAS_AVX2 && (defined(__GNUC__) || defined(__clang__))
        #define TG_AVX2_TARGET __attribute__((target("avx2")))
#elif TG_HAS_AVX2 && defined(_MSC_VER)
        #define TG_AVX2_TARGET
#else
        #define TG_AVX2_TARGET
#endif

#if TG_HAS_AVX2
        struct alignas(32) Packet {
            double x[kPacketSize];
            double y[kPacketSize];
            double z[kPacketSize];
        };

        struct PacketItem {
            uint64_t node;
            uint8_t mask;
        };

        static std::vector<PacketItem> &getPacketStack() {
            thread_local std::vector<PacketItem> stack;
            stack.clear();
            stack.reserve(128);
            return stack;
        }

        TG_AVX2_TARGET static inline __m256d avxDot3(__m256d ax, __m256d ay, __m256d az,
                                                     __m256d bx, __m256d by, __m256d bz) {
            return _mm256_add_pd(_mm256_add_pd(_mm256_mul_pd(ax, bx), _mm256_mul_pd(ay, by)),
                                 _mm256_mul_pd(az, bz));
        }

        TG_AVX2_TARGET static inline void avxCross3(__m256d ax, __m256d ay, __m256d az,
                                                    __m256d bx, __m256d by, __m256d bz,
                                                    __m256d &cx, __m256d &cy, __m256d &cz) {
            cx = _mm256_sub_pd(_mm256_mul_pd(ay, bz), _mm256_mul_pd(az, by));
            cy = _mm256_sub_pd(_mm256_mul_pd(az, bx), _mm256_mul_pd(ax, bz));
            cz = _mm256_sub_pd(_mm256_mul_pd(ax, by), _mm256_mul_pd(ay, bx));
        }

        TG_AVX2_TARGET void approxPacketFull(const nodeAgg &nd, const Packet &p, double *out) const {
            __m256d px = _mm256_load_pd(p.x);
            __m256d py = _mm256_load_pd(p.y);
            __m256d pz = _mm256_load_pd(p.z);
            __m256d cx = _mm256_set1_pd(nd.averageP.x);
            __m256d cy = _mm256_set1_pd(nd.averageP.y);
            __m256d cz = _mm256_set1_pd(nd.averageP.z);

            __m256d qx = _mm256_sub_pd(px, cx);
            __m256d qy = _mm256_sub_pd(py, cy);
            __m256d qz = _mm256_sub_pd(pz, cz);

            __m256d ql2 = avxDot3(qx, qy, qz, qx, qy, qz);
            __m256d ql_m2 = _mm256_div_pd(_mm256_set1_pd(1.0), ql2);
            __m256d ql_m1 = _mm256_sqrt_pd(ql_m2);

            __m256d qnx = _mm256_mul_pd(qx, ql_m1);
            __m256d qny = _mm256_mul_pd(qy, ql_m1);
            __m256d qnz = _mm256_mul_pd(qz, ql_m1);

            __m256d Nx = _mm256_set1_pd(nd.N.x);
            __m256d Ny = _mm256_set1_pd(nd.N.y);
            __m256d Nz = _mm256_set1_pd(nd.N.z);
            __m256d omega = _mm256_mul_pd(_mm256_set1_pd(-1.0),
                                          _mm256_mul_pd(ql_m2, avxDot3(qnx, qny, qnz, Nx, Ny, Nz)));

            int order = std::clamp(settings.taylorOrder, 0, 2);
            if (order >= 1) {
                __m256d q2x = _mm256_mul_pd(qnx, qnx);
                __m256d q2y = _mm256_mul_pd(qny, qny);
                __m256d q2z = _mm256_mul_pd(qnz, qnz);
                __m256d ql_m3 = _mm256_mul_pd(ql_m2, ql_m1);

                __m256d Nijx = _mm256_set1_pd(nd.NijDiag.x);
                __m256d Nijy = _mm256_set1_pd(nd.NijDiag.y);
                __m256d Nijz = _mm256_set1_pd(nd.NijDiag.z);
                __m256d dq2Nij = _mm256_add_pd(_mm256_add_pd(_mm256_mul_pd(q2x, Nijx), _mm256_mul_pd(q2y, Nijy)),
                                               _mm256_mul_pd(q2z, Nijz));
                __m256d cross = _mm256_add_pd(
                    _mm256_add_pd(_mm256_mul_pd(_mm256_mul_pd(qnx, qny), _mm256_set1_pd(nd.Nxy_Nyx)),
                                  _mm256_mul_pd(_mm256_mul_pd(qny, qnz), _mm256_set1_pd(nd.Nyz_Nzy))),
                    _mm256_mul_pd(_mm256_mul_pd(qnz, qnx), _mm256_set1_pd(nd.Nzx_Nxz)));

                __m256d NijSum = _mm256_set1_pd(nd.NijDiag.x + nd.NijDiag.y + nd.NijDiag.z);
                __m256d term1 = _mm256_mul_pd(ql_m3,
                                              _mm256_sub_pd(NijSum, _mm256_mul_pd(_mm256_set1_pd(3.0),
                                                                                  _mm256_add_pd(dq2Nij, cross))));
                omega = _mm256_add_pd(omega, term1);

                if (order >= 2) {
                    __m256d q3x = _mm256_mul_pd(q2x, qnx);
                    __m256d q3y = _mm256_mul_pd(q2y, qny);
                    __m256d q3z = _mm256_mul_pd(q2z, qnz);
                    __m256d ql_m4 = _mm256_mul_pd(ql_m2, ql_m2);

                    __m256d t0x = _mm256_set1_pd(nd.twoNyyx_Nxyy + nd.twoNzzx_Nxzz);
                    __m256d t0y = _mm256_set1_pd(nd.twoNzzy_Nyzz + nd.twoNxxy_Nyxx);
                    __m256d t0z = _mm256_set1_pd(nd.twoNxxz_Nzxx + nd.twoNyyz_Nzyy);

                    __m256d t1x = _mm256_add_pd(_mm256_mul_pd(qny, _mm256_set1_pd(nd.twoNxxy_Nyxx)),
                                                _mm256_mul_pd(qnz, _mm256_set1_pd(nd.twoNxxz_Nzxx)));
                    __m256d t1y = _mm256_add_pd(_mm256_mul_pd(qnz, _mm256_set1_pd(nd.twoNyyz_Nzyy)),
                                                _mm256_mul_pd(qnx, _mm256_set1_pd(nd.twoNyyx_Nxyy)));
                    __m256d t1z = _mm256_add_pd(_mm256_mul_pd(qnx, _mm256_set1_pd(nd.twoNzzx_Nxzz)),
                                                _mm256_mul_pd(qny, _mm256_set1_pd(nd.twoNzzy_Nyzz)));

                    __m256d tnX = _mm256_set1_pd(3.0 * nd.NijkDiag.x);
                    __m256d tnY = _mm256_set1_pd(3.0 * nd.NijkDiag.y);
                    __m256d tnZ = _mm256_set1_pd(3.0 * nd.NijkDiag.z);
                    __m256d s0x = _mm256_add_pd(tnX, t0x);
                    __m256d s0y = _mm256_add_pd(tnY, t0y);
                    __m256d s0z = _mm256_add_pd(tnZ, t0z);

                    __m256d dot_q_s0 = avxDot3(qnx, qny, qnz, s0x, s0y, s0z);
                    __m256d dot_q3_nijk = _mm256_add_pd(_mm256_add_pd(_mm256_mul_pd(q3x, _mm256_set1_pd(nd.NijkDiag.x)),
                                                                      _mm256_mul_pd(q3y, _mm256_set1_pd(nd.NijkDiag.y))),
                                                        _mm256_mul_pd(q3z, _mm256_set1_pd(nd.NijkDiag.z)));
                    __m256d dot_q2_t1 = _mm256_add_pd(_mm256_add_pd(_mm256_mul_pd(q2x, t1x), _mm256_mul_pd(q2y, t1y)),
                                                      _mm256_mul_pd(q2z, t1z));
                    __m256d qxyz = _mm256_mul_pd(_mm256_mul_pd(qnx, qny), qnz);

                    __m256d term2 = _mm256_mul_pd(ql_m4,
                                                  _mm256_sub_pd(_mm256_mul_pd(_mm256_set1_pd(1.5), dot_q_s0),
                                                                _mm256_mul_pd(_mm256_set1_pd(7.5),
                                                                              _mm256_add_pd(_mm256_add_pd(dot_q3_nijk,
                                                                                                          _mm256_mul_pd(qxyz, _mm256_set1_pd(nd.sumPermuteNxyz))),
                                                                                            dot_q2_t1))));
                    omega = _mm256_add_pd(omega, term2);
                }
            }

            _mm256_store_pd(out, omega);
        }

        TG_AVX2_TARGET void solidAnglePacketFull(const vec3 &a, const vec3 &b, const vec3 &c, const Packet &p, double *out) const {
            __m256d ax = _mm256_set1_pd(a.x), ay = _mm256_set1_pd(a.y), az = _mm256_set1_pd(a.z);
            __m256d bx = _mm256_set1_pd(b.x), by = _mm256_set1_pd(b.y), bz = _mm256_set1_pd(b.z);
            __m256d cx = _mm256_set1_pd(c.x), cy = _mm256_set1_pd(c.y), cz = _mm256_set1_pd(c.z);
            __m256d px = _mm256_load_pd(p.x);
            __m256d py = _mm256_load_pd(p.y);
            __m256d pz = _mm256_load_pd(p.z);

            __m256d Ax = _mm256_sub_pd(ax, px), Ay = _mm256_sub_pd(ay, py), Az = _mm256_sub_pd(az, pz);
            __m256d Bx = _mm256_sub_pd(bx, px), By = _mm256_sub_pd(by, py), Bz = _mm256_sub_pd(bz, pz);
            __m256d Cx = _mm256_sub_pd(cx, px), Cy = _mm256_sub_pd(cy, py), Cz = _mm256_sub_pd(cz, pz);

            __m256d aN = _mm256_sqrt_pd(avxDot3(Ax, Ay, Az, Ax, Ay, Az));
            __m256d bN = _mm256_sqrt_pd(avxDot3(Bx, By, Bz, Bx, By, Bz));
            __m256d cN = _mm256_sqrt_pd(avxDot3(Cx, Cy, Cz, Cx, Cy, Cz));

            __m256d invA = _mm256_div_pd(_mm256_set1_pd(1.0), aN);
            __m256d invB = _mm256_div_pd(_mm256_set1_pd(1.0), bN);
            __m256d invC = _mm256_div_pd(_mm256_set1_pd(1.0), cN);

            __m256d Anx = _mm256_mul_pd(Ax, invA), Any = _mm256_mul_pd(Ay, invA), Anz = _mm256_mul_pd(Az, invA);
            __m256d Bnx = _mm256_mul_pd(Bx, invB), Bny = _mm256_mul_pd(By, invB), Bnz = _mm256_mul_pd(Bz, invB);
            __m256d Cnx = _mm256_mul_pd(Cx, invC), Cny = _mm256_mul_pd(Cy, invC), Cnz = _mm256_mul_pd(Cz, invC);

            __m256d crossX, crossY, crossZ;
            avxCross3(Bnx, Bny, Bnz, Cnx, Cny, Cnz, crossX, crossY, crossZ);
            __m256d num = avxDot3(Anx, Any, Anz, crossX, crossY, crossZ);

            __m256d cosAB = avxDot3(Anx, Any, Anz, Bnx, Bny, Bnz);
            __m256d cosBC = avxDot3(Bnx, Bny, Bnz, Cnx, Cny, Cnz);
            __m256d cosCA = avxDot3(Cnx, Cny, Cnz, Anx, Any, Anz);
            __m256d denom = _mm256_add_pd(_mm256_add_pd(_mm256_add_pd(_mm256_set1_pd(1.0), cosAB), cosBC), cosCA);

            alignas(32) double numArr[kPacketSize];
            alignas(32) double denomArr[kPacketSize];
            _mm256_store_pd(numArr, num);
            _mm256_store_pd(denomArr, denom);
            for (int i = 0; i < kPacketSize; ++i) {
                out[i] += 2.0 * std::atan2(numArr[i], denomArr[i]);
            }
        }

        TG_AVX2_TARGET void dipolePacketFull(const vec3 &dipoleVec, const vec3 &ptPos, const Packet &p, double *out) const {
            __m256d px = _mm256_load_pd(p.x);
            __m256d py = _mm256_load_pd(p.y);
            __m256d pz = _mm256_load_pd(p.z);
            __m256d ox = _mm256_set1_pd(ptPos.x);
            __m256d oy = _mm256_set1_pd(ptPos.y);
            __m256d oz = _mm256_set1_pd(ptPos.z);

            __m256d rx = _mm256_sub_pd(ox, px);
            __m256d ry = _mm256_sub_pd(oy, py);
            __m256d rz = _mm256_sub_pd(oz, pz);
            __m256d r2 = avxDot3(rx, ry, rz, rx, ry, rz);
            __m256d r = _mm256_sqrt_pd(r2);
            __m256d r3 = _mm256_mul_pd(r2, r);

            __m256d dx = _mm256_set1_pd(dipoleVec.x);
            __m256d dy = _mm256_set1_pd(dipoleVec.y);
            __m256d dz = _mm256_set1_pd(dipoleVec.z);
            __m256d dotd = avxDot3(dx, dy, dz, rx, ry, rz);
            __m256d contrib = _mm256_div_pd(dotd, r3);

            alignas(32) double vals[kPacketSize];
            _mm256_store_pd(vals, contrib);
            for (int i = 0; i < kPacketSize; ++i) {
                out[i] += vals[i];
            }
        }
#endif

        // Evaluate Taylor approximation of a node at point p.
        // Approximation uses expansion in q = (p - averageP) with terms up to taylorOrder.
        double approx(const nodeAgg &nd, const vec3 &p) const {
            vec3 qraw = p - nd.averageP;
            double ql2 = dot(qraw, qraw);
            if (ql2 == 0) return 0;

            double ql_m2 = 1 / ql2, ql_m1 = std::sqrt(ql_m2);
            vec3 q = qraw * ql_m1;
            double omega = -ql_m2 * (q.x * nd.N.x + q.y * nd.N.y + q.z * nd.N.z);

            int order = std::clamp(settings.taylorOrder, 0, 2);
            if (order >= 1) {
                vec3 q2{q.x * q.x, q.y * q.y, q.z * q.z};
                double ql_m3 = ql_m2 * ql_m1;
                double dq2Nij = q2.x * nd.NijDiag.x + q2.y * nd.NijDiag.y + q2.z * nd.NijDiag.z;
                double cross = q.x * q.y * nd.Nxy_Nyx + q.y * q.z * nd.Nyz_Nzy + q.z * q.x * nd.Nzx_Nxz;
                omega += ql_m3 * ((nd.NijDiag.x + nd.NijDiag.y + nd.NijDiag.z) - 3 * (dq2Nij + cross));

                if (order >= 2) {
                    vec3 q3{q2.x * q.x, q2.y * q.y, q2.z * q.z};
                    double ql_m4 = ql_m2 * ql_m2;
                    vec3 t0{
                        nd.twoNyyx_Nxyy + nd.twoNzzx_Nxzz, nd.twoNzzy_Nyzz + nd.twoNxxy_Nyxx,
                        nd.twoNxxz_Nzxx + nd.twoNyyz_Nzyy
                    };
                    vec3 t1{
                        q.y * nd.twoNxxy_Nyxx + q.z * nd.twoNxxz_Nzxx, q.z * nd.twoNyyz_Nzyy + q.x * nd.twoNyyx_Nxyy,
                        q.x * nd.twoNzzx_Nxzz + q.y * nd.twoNzzy_Nyzz
                    };
                    vec3 tn{3 * nd.NijkDiag.x, 3 * nd.NijkDiag.y, 3 * nd.NijkDiag.z};
                    vec3 s0{tn.x + t0.x, tn.y + t0.y, tn.z + t0.z};
                    omega += ql_m4 * (1.5 * (q.x * s0.x + q.y * s0.y + q.z * s0.z)
                                      - 7.5 * (q3.x * nd.NijkDiag.x + q3.y * nd.NijkDiag.y + q3.z * nd.NijkDiag.z
                                               + q.x * q.y * q.z * nd.sumPermuteNxyz + q2.x * t1.x + q2.y * t1.y + q2.z
                                               * t1.z));
                }
            }
            return omega;
        }

        // Query winding number at a single point.
        // Traversal: approximate nodes if (distance^2 > maxPDist2 * accuracy^2),
        // otherwise descend to leaves and accumulate exact contributions.
        double queryOne(const Point &pt) const {
            vec3 p = toVec3(pt);
            double sum = 0;
            double acc2 = settings.accuracy * settings.accuracy;

            auto &stack = getStack();
            stack.push_back(0);

            while (!stack.empty()) {
                uint64_t ni = stack.back();
                stack.pop_back();
                const auto &nd = nodes[ni];

                vec3 q = p - nd.averageP;
                if (dot(q, q) > nd.maxPDist2 * acc2) {
                    sum += approx(nd, p);
                    continue;
                }

                if (!nd.isLeaf) {
                    stack.push_back(nd.right);
                    stack.push_back(nd.left);
                    continue;
                }

                if (mode == Mode::Mesh) {
                    for (uint64_t t = 0; t < nd.primCount; ++t) {
                        uint64_t triId = primIndex(nd.firstPrim + t);
                        const auto &idx = triangles[triId];
                        sum += solidAngle(toVec3(vertices[idx[0]]), toVec3(vertices[idx[1]]),
                                          toVec3(vertices[idx[2]]), p);
                    }
                } else {
                    // Point cloud mode - use dipole
                    for (uint64_t t = 0; t < nd.primCount; ++t) {
                        uint64_t ptId = primIndex(nd.firstPrim + t);
                        sum += dipole(cloudDipoles[ptId], toVec3(cloudPoints[ptId]), p);
                    }
                }
            }
            return sum / (4 * pi());
        }

        // Query many points in parallel.
        std::vector<double> queryMany(const std::vector<Point> &points) const {
            std::vector<double> out(points.size());
            if (points.empty()) return out;

            unsigned int tc = settings.threads;
            if (tc == 0) tc = std::thread::hardware_concurrency();
            tc = std::max(1u, std::min(tc, (unsigned int) points.size()));

            std::atomic<size_t> next{0};
            const size_t chunk = 2048;

            auto worker = [&]() {
                while (true) {
                    size_t start = next.fetch_add(chunk, std::memory_order_relaxed);
                    if (start >= points.size()) break;
                    size_t end = std::min(points.size(), start + chunk);
#if TG_HAS_AVX2
                    bool useAvx2 = false;
                    #if defined(__GNUC__) || defined(__clang__)
                    useAvx2 = __builtin_cpu_supports("avx2");
                    #elif defined(_MSC_VER)
                    int info[4] = {0};
                    __cpuidex(info, 0, 0);
                    if (info[0] >= 7) {
                        __cpuidex(info, 7, 0);
                        useAvx2 = (info[1] & (1 << 5)) != 0;
                    }
                    #endif

                    if (useAvx2) {
                        size_t i = start;
                        for (; i + kPacketSize <= end; i += kPacketSize) {
                            Packet pack{};
                            for (int k = 0; k < kPacketSize; ++k) {
                                pack.x[k] = points[i + k][0];
                                pack.y[k] = points[i + k][1];
                                pack.z[k] = points[i + k][2];
                            }

                            alignas(32) double sums[kPacketSize] = {0, 0, 0, 0};
                            double acc2 = settings.accuracy * settings.accuracy;
                            auto &stack = getPacketStack();
                            stack.push_back({0, 0x0F});

                            while (!stack.empty()) {
                                PacketItem item = stack.back();
                                stack.pop_back();
                                const auto &nd = nodes[item.node];
                                uint8_t mask = item.mask;

                                if (!nd.isLeaf) {
                                    uint8_t approxMask = 0;
                                    uint8_t descendMask = 0;
                                    for (int lane = 0; lane < kPacketSize; ++lane) {
                                        if (!((mask >> lane) & 1)) continue;
                                        vec3 p = {pack.x[lane], pack.y[lane], pack.z[lane]};
                                        vec3 q = p - nd.averageP;
                                        if (dot(q, q) > nd.maxPDist2 * acc2) approxMask |= (1u << lane);
                                        else descendMask |= (1u << lane);
                                    }

                                    if (approxMask) {
                                        if (approxMask == 0x0F) {
                                            alignas(32) double vals[kPacketSize];
                                            approxPacketFull(nd, pack, vals);
                                            for (int lane = 0; lane < kPacketSize; ++lane) sums[lane] += vals[lane];
                                        } else {
                                            for (int lane = 0; lane < kPacketSize; ++lane) {
                                                if (!((approxMask >> lane) & 1)) continue;
                                                vec3 p = {pack.x[lane], pack.y[lane], pack.z[lane]};
                                                sums[lane] += approx(nd, p);
                                            }
                                        }
                                    }

                                    if (descendMask) {
                                        stack.push_back({nd.left, descendMask});
                                        stack.push_back({nd.right, descendMask});
                                    }
                                    continue;
                                }

                                if (mode == Mode::Mesh) {
                                    for (uint64_t t = 0; t < nd.primCount; ++t) {
                                        uint64_t triId = primIndex(nd.firstPrim + t);
                                        const auto &idx = triangles[triId];
                                        vec3 a = toVec3(vertices[idx[0]]);
                                        vec3 b = toVec3(vertices[idx[1]]);
                                        vec3 c = toVec3(vertices[idx[2]]);

                                        if (mask == 0x0F) {
                                            solidAnglePacketFull(a, b, c, pack, sums);
                                        } else {
                                            for (int lane = 0; lane < kPacketSize; ++lane) {
                                                if (!((mask >> lane) & 1)) continue;
                                                vec3 p = {pack.x[lane], pack.y[lane], pack.z[lane]};
                                                sums[lane] += solidAngle(a, b, c, p);
                                            }
                                        }
                                    }
                                } else {
                                    for (uint64_t t = 0; t < nd.primCount; ++t) {
                                        uint64_t ptId = primIndex(nd.firstPrim + t);
                                        vec3 dip = cloudDipoles[ptId];
                                        vec3 pos = toVec3(cloudPoints[ptId]);

                                        if (mask == 0x0F) {
                                            dipolePacketFull(dip, pos, pack, sums);
                                        } else {
                                            for (int lane = 0; lane < kPacketSize; ++lane) {
                                                if (!((mask >> lane) & 1)) continue;
                                                vec3 p = {pack.x[lane], pack.y[lane], pack.z[lane]};
                                                sums[lane] += dipole(dip, pos, p);
                                            }
                                        }
                                    }
                                }
                            }

                            for (int lane = 0; lane < kPacketSize; ++lane) {
                                out[i + lane] = sums[lane] / (4 * pi());
                            }
                        }
                        for (; i < end; ++i) {
                            out[i] = queryOne(points[i]);
                        }
                    } else {
                        for (size_t i = start; i < end; ++i) {
                            out[i] = queryOne(points[i]);
                        }
                    }
#else
                    for (size_t i = start; i < end; ++i) {
                        out[i] = queryOne(points[i]);
                    }
#endif
                }
            };

            std::vector<std::thread> threads;
            threads.reserve(tc);
            for (unsigned int i = 0; i < tc; ++i) threads.emplace_back(worker);
            for (auto &t: threads) t.join();

            return out;
        }
    };

    // Public API
    windingNumber::windingNumber(const std::vector<Point> &vertices,
                                 const std::vector<Triangle> &triangles,
                                 Settings s)
        : impl_(std::make_unique<Impl>()) {
        if (vertices.empty() || triangles.empty()) {
            throw std::runtime_error("Mesh cannot be empty");
        }
        for (const auto &tri: triangles) {
            if (tri[0] >= vertices.size() || tri[1] >= vertices.size() || tri[2] >= vertices.size()) {
                throw std::runtime_error("Triangle index out of range");
            }
        }
        impl_->settings = s;
        impl_->mode = Impl::Mode::Mesh;
        impl_->vertices = vertices;
        impl_->triangles = triangles;
        impl_->build();
    }

    windingNumber::windingNumber(const std::vector<Point> &points, Settings s)
        : impl_(std::make_unique<Impl>()) {
        if (points.empty()) {
            throw std::runtime_error("Point cloud cannot be empty");
        }

        impl_->settings = s;
        impl_->mode = Impl::Mode::PointCloud;
        impl_->cloudPoints = points;

        // Estimate normals using PCA
        impl_->estimateNormals(s.kNeighbors);

        // Estimate areas using k-NN
        impl_->estimateAreas(s.kNeighbors);
        impl_->buildPointCloud();
    }

    windingNumber::windingNumber(const std::vector<Point> &points,
                                 const std::vector<Point> &normals,
                                 Settings s)
        : impl_(std::make_unique<Impl>()) {
        if (points.size() != normals.size()) {
            throw std::runtime_error("Points and normals must have same size");
        }
        if (points.empty()) {
            throw std::runtime_error("Point cloud cannot be empty");
        }

        impl_->settings = s;
        impl_->mode = Impl::Mode::PointCloud;

        // Copy points
        impl_->cloudPoints = points;

        // Normalize and store normals
        impl_->cloudNormals.resize(points.size());
        for (size_t i = 0; i < points.size(); ++i) {
            Impl::vec3 n = Impl::toVec3(normals[i]);
            double len = Impl::norm(n);
            if (len > 0) n = n * (1.0 / len);
            impl_->cloudNormals[i] = n;
        }

        // Estimate areas using k-NN
        impl_->estimateAreas(s.kNeighbors);
        impl_->buildPointCloud();
    }

    windingNumber::windingNumber(const std::vector<Point> &points,
                                 const std::vector<Point> &normals,
                                 const std::vector<double> &areas,
                                 Settings s)
        : impl_(std::make_unique<Impl>()) {
        if (points.size() != normals.size() || points.size() != areas.size()) {
            throw std::runtime_error("Points, normals, and areas must have same size");
        }
        if (points.empty()) {
            throw std::runtime_error("Point cloud cannot be empty");
        }

        impl_->settings = s;
        impl_->mode = Impl::Mode::PointCloud;

        // Copy points and areas
        impl_->cloudPoints = points;
        impl_->cloudAreas = areas;

        // Normalize and store normals
        impl_->cloudNormals.resize(points.size());
        for (size_t i = 0; i < points.size(); ++i) {
            Impl::vec3 n = Impl::toVec3(normals[i]);
            double len = Impl::norm(n);
            if (len > 0) n = n * (1.0 / len);
            impl_->cloudNormals[i] = n;
        }

        impl_->buildPointCloud();
    }

    windingNumber::~windingNumber() = default;

    windingNumber::windingNumber(windingNumber &&) noexcept = default;

    windingNumber &windingNumber::operator=(windingNumber &&) noexcept = default;

    double windingNumber::query(const Point &p) const {
        return impl_->queryOne(p);
    }

    std::vector<double> windingNumber::query(const std::vector<Point> &points) const {
        return impl_->queryMany(points);
    }
} // namespace tg
