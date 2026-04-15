#pragma once

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

#include <array>
#include <memory>
#include <vector>

namespace tg {
    struct windingSettings {
        double accuracy = 2.0;
        int taylorOrder = 2;
        unsigned int threads = 0;
        int kNeighbors = 12;
    };

    class windingNumber {
    public:
        using Point = std::array<double, 3>;
        using Triangle = std::array<unsigned int, 3>;
        using Settings = windingSettings;

        windingNumber(const std::vector<Point> &vertices,
                      const std::vector<Triangle> &triangles,
                      Settings s = {});

        explicit windingNumber(const std::vector<Point> &points, Settings s = {});

        windingNumber(const std::vector<Point> &points,
                      const std::vector<Point> &normals,
                      Settings s = {});

        windingNumber(const std::vector<Point> &points,
                      const std::vector<Point> &normals,
                      const std::vector<double> &areas,
                      Settings s = {});

        // For closed, consistently oriented shapes:
        // query(...) ~ 1 => inside, ~ 0 => outside.
        // Practical thresholds:
        //   w > 0.5  => inside
        //   w < 0.5  => outside
        // Optional conservative bands often used in practice:
        //   w > 0.75 => confidently inside
        //   w < 0.25 => confidently outside
        double query(const Point &p) const;

        std::vector<double> query(const std::vector<Point> &points) const;

        ~windingNumber();

        windingNumber(windingNumber &&) noexcept;

        windingNumber &operator=(windingNumber &&) noexcept;

        windingNumber(const windingNumber &) = delete;

        windingNumber &operator=(const windingNumber &) = delete;

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
} // namespace tg
