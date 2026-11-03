/*++

Copyright (C) 2026 3MF Consortium

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

ToolpathPointOverrides.cpp: Defines unit tests for linear and non-linear profile
modification factors on polyline and loop points (override support).

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

#include <cmath>

namespace Lib3MF
{
	class ToolpathPointOverrides : public Lib3MFTest {
	protected:
		PModel model;
		PToolpath toolpath;
		PToolpathLayerData layer;
		PWriter writer;
		PPersistentReaderSource readSource;
		Lib3MF_uint32 profileID = 0;
		Lib3MF_uint32 partID = 0;

		const double dUnits = 0.001;

		virtual void SetUp() {
			model = wrapper->CreateModel();
			writer = model->QueryWriter("3mf");

			// Minimal build item so that a part can be registered.
			std::vector<sPosition> vertices;
			std::vector<sTriangle> triangles;
			fnCreateBox(vertices, triangles);
			auto mesh = model->AddMeshObject();
			mesh->SetGeometry(vertices, triangles);
			auto buildItem = model->AddBuildItem(mesh.get(), wrapper->GetIdentityTransform());

			toolpath = model->AddToolpath(dUnits);
			auto profile = toolpath->AddProfile("default");
			layer = toolpath->AddLayer(1000, "/Metadata/toolpath_1.xml", writer.get());
			profileID = layer->RegisterProfile(profile.get());
			partID = layer->RegisterBuildItem(buildItem.get());
		}

		// Writes the model to a buffer, re-reads it and returns the layer reader of the first toolpath.
		// Note: a persistent reader source is used (and kept alive as a fixture member) so that the
		// data source stays valid for lazily-loaded toolpath layer attachments.
		PToolpathLayerReader roundtrip(PModel & readModelOut) {
			std::vector<Lib3MF_uint8> buffer;
			writer->WriteToBuffer(buffer);

			readModelOut = wrapper->CreateModel();
			readSource = readModelOut->CreatePersistentSourceFromBuffer(buffer);
			auto reader = readModelOut->QueryReader("3mf");
			reader->ReadFromPersistentSource(readSource.get());

			auto iterator = readModelOut->GetToolpaths();
			EXPECT_TRUE(iterator->MoveNext());
			auto readToolpath = iterator->GetCurrentToolpath();
			EXPECT_EQ(readToolpath->GetLayerCount(), 1u);
			return readToolpath->ReadLayerData(0);
		}

		std::vector<sPosition2D> makePoints() {
			std::vector<sPosition2D> points(3);
			points[0].m_Coordinates[0] = 0.0f; points[0].m_Coordinates[1] = 0.0f;
			points[1].m_Coordinates[0] = 10.0f; points[1].m_Coordinates[1] = 0.0f;
			points[2].m_Coordinates[0] = 10.0f; points[2].m_Coordinates[1] = 10.0f;
			return points;
		}
	};

	// Round-trips a polyline whose points carry a linearly ramped F factor.
	TEST_F(ToolpathPointOverrides, PolylineLinearFactorF)
	{
		auto points = makePoints();
		std::vector<double> f1 = { 0.1, 0.2, 0.3 };
		std::vector<double> f2 = { 0.15, 0.25, 0.35 };

		layer->WritePolylineInModelUnitsWithLinearFactors(profileID, partID, points, eToolpathProfileModificationFactor::FactorF, f1, f2);
		layer->Finish();

		PModel readModel;
		auto reader = roundtrip(readModel);

		ASSERT_EQ(reader->GetSegmentCount(), 1u);
		eToolpathSegmentType eType;
		Lib3MF_uint32 nPointCount = 0;
		reader->GetSegmentInfo(0, eType, nPointCount);
		ASSERT_EQ(eType, eToolpathSegmentType::Polyline);
		ASSERT_EQ(nPointCount, 3u);

		ASSERT_TRUE(reader->SegmentHasLinearPointModificationFactors(0, eToolpathProfileModificationFactor::FactorF));

		std::vector<sHatch2DFactors> factors;
		reader->GetLinearSegmentPointModificationFactors(0, eToolpathProfileModificationFactor::FactorF, factors);
		ASSERT_EQ(factors.size(), 3u);
		for (size_t i = 0; i < 3; i++) {
			EXPECT_NEAR(factors[i].m_Point1Factor, f1[i], 1e-6);
			EXPECT_NEAR(factors[i].m_Point2Factor, f2[i], 1e-6);
		}

		// There is no linear factor for G on this segment.
		EXPECT_FALSE(reader->SegmentHasLinearPointModificationFactors(0, eToolpathProfileModificationFactor::FactorG));
	}

	// Round-trips a loop whose points carry a linearly ramped G factor (selected via the enum).
	TEST_F(ToolpathPointOverrides, LoopLinearFactorG)
	{
		auto points = makePoints();
		std::vector<double> g1 = { 0.4, 0.5, 0.6 };
		std::vector<double> g2 = { 0.45, 0.55, 0.65 };

		layer->WriteLoopInModelUnitsWithLinearFactors(profileID, partID, points, eToolpathProfileModificationFactor::FactorG, g1, g2);
		layer->Finish();

		PModel readModel;
		auto reader = roundtrip(readModel);

		eToolpathSegmentType eType;
		Lib3MF_uint32 nPointCount = 0;
		reader->GetSegmentInfo(0, eType, nPointCount);
		ASSERT_EQ(eType, eToolpathSegmentType::Loop);

		ASSERT_TRUE(reader->SegmentHasLinearPointModificationFactors(0, eToolpathProfileModificationFactor::FactorG));
		std::vector<sHatch2DFactors> factors;
		reader->GetLinearSegmentPointModificationFactors(0, eToolpathProfileModificationFactor::FactorG, factors);
		ASSERT_EQ(factors.size(), 3u);
		for (size_t i = 0; i < 3; i++) {
			EXPECT_NEAR(factors[i].m_Point1Factor, g1[i], 1e-6);
			EXPECT_NEAR(factors[i].m_Point2Factor, g2[i], 1e-6);
		}
	}

	// Round-trips a polyline with non-linear (sub) interpolation points for F.
	TEST_F(ToolpathPointOverrides, PolylineNonlinearFactorF)
	{
		auto points = makePoints();
		std::vector<double> f1 = { 0.0, 0.2, 0.4 };
		std::vector<double> f2 = { 0.0, 0.6, 0.8 };
		// Point 0 has no subs, point 1 has two subs, point 2 has one sub.
		std::vector<Lib3MF_uint32> subCounts = { 0, 2, 1 };
		std::vector<sHatchModificationInterpolationData> subs(3);
		subs[0].m_Parameter = 0.3; subs[0].m_Factor = 0.35; // point 1, sub 0
		subs[1].m_Parameter = 0.7; subs[1].m_Factor = 0.55; // point 1, sub 1
		subs[2].m_Parameter = 0.5; subs[2].m_Factor = 0.65; // point 2, sub 0

		layer->WritePolylineInModelUnitsWithNonlinearFactors(profileID, partID, points, eToolpathProfileModificationFactor::FactorF, f1, f2, subCounts, subs);
		layer->Finish();

		PModel readModel;
		auto reader = roundtrip(readModel);

		ASSERT_TRUE(reader->SegmentHasNonlinearPointModificationInterpolation(0));

		std::vector<Lib3MF_uint32> counts;
		std::vector<sHatchModificationInterpolationData> allSubs;
		reader->GetSegmentAllNonlinearPointsModificationInterpolation(0, eToolpathProfileModificationFactor::FactorF, counts, allSubs);
		ASSERT_EQ(counts.size(), 3u);
		EXPECT_EQ(counts[0], 0u);
		EXPECT_EQ(counts[1], 2u);
		EXPECT_EQ(counts[2], 1u);
		ASSERT_EQ(allSubs.size(), 3u);
		EXPECT_NEAR(allSubs[0].m_Parameter, 0.3, 1e-6);
		EXPECT_NEAR(allSubs[1].m_Parameter, 0.7, 1e-6);
		EXPECT_NEAR(allSubs[2].m_Parameter, 0.5, 1e-6);

		// Query the single point's subs directly.
		std::vector<sHatchModificationInterpolationData> point1Subs;
		reader->GetSegmentNonlinearPointModificationInterpolation(0, 1, eToolpathProfileModificationFactor::FactorF, point1Subs);
		ASSERT_EQ(point1Subs.size(), 2u);
		EXPECT_NEAR(point1Subs[0].m_Factor, 0.35, 1e-6);
		EXPECT_NEAR(point1Subs[1].m_Factor, 0.55, 1e-6);
	}

	// A <sub> list with non-increasing parameters must be rejected at write time.
	TEST_F(ToolpathPointOverrides, NonincreasingSubParametersThrows)
	{
		auto points = makePoints();
		std::vector<double> f1 = { 0.0, 0.2, 0.4 };
		std::vector<double> f2 = { 0.0, 0.6, 0.8 };
		std::vector<Lib3MF_uint32> subCounts = { 0, 2, 0 };
		std::vector<sHatchModificationInterpolationData> subs(2);
		subs[0].m_Parameter = 0.7; subs[0].m_Factor = 0.35;
		subs[1].m_Parameter = 0.3; subs[1].m_Factor = 0.55; // not increasing

		ASSERT_SPECIFIC_THROW(
			layer->WritePolylineInModelUnitsWithNonlinearFactors(profileID, partID, points, eToolpathProfileModificationFactor::FactorF, f1, f2, subCounts, subs),
			ELib3MFException);
	}

	// A <sub> parameter outside the open interval (0,1) must be rejected at write time.
	TEST_F(ToolpathPointOverrides, SubParameterOutOfRangeThrows)
	{
		auto points = makePoints();
		std::vector<double> f1 = { 0.0, 0.2, 0.4 };
		std::vector<double> f2 = { 0.0, 0.6, 0.8 };
		std::vector<Lib3MF_uint32> subCounts = { 0, 1, 0 };
		std::vector<sHatchModificationInterpolationData> subs(1);
		subs[0].m_Parameter = 1.0; subs[0].m_Factor = 0.35; // must be strictly < 1

		ASSERT_SPECIFIC_THROW(
			layer->WritePolylineInModelUnitsWithNonlinearFactors(profileID, partID, points, eToolpathProfileModificationFactor::FactorF, f1, f2, subCounts, subs),
			ELib3MFException);
	}

	// Querying point modification factors on a hatch segment must fail.
	TEST_F(ToolpathPointOverrides, PointFactorsOnHatchSegmentThrows)
	{
		std::vector<sHatch2D> hatches(1);
		hatches[0].m_Point1Coordinates[0] = 0.0; hatches[0].m_Point1Coordinates[1] = 0.0;
		hatches[0].m_Point2Coordinates[0] = 10.0; hatches[0].m_Point2Coordinates[1] = 0.0;

		layer->WriteHatchDataInModelUnits(profileID, partID, hatches);
		layer->Finish();

		PModel readModel;
		auto reader = roundtrip(readModel);

		eToolpathSegmentType eType;
		Lib3MF_uint32 nPointCount = 0;
		reader->GetSegmentInfo(0, eType, nPointCount);
		ASSERT_EQ(eType, eToolpathSegmentType::Hatch);

		std::vector<sHatch2DFactors> factors;
		ASSERT_SPECIFIC_THROW(
			reader->GetLinearSegmentPointModificationFactors(0, eToolpathProfileModificationFactor::FactorF, factors),
			ELib3MFException);
	}
}
