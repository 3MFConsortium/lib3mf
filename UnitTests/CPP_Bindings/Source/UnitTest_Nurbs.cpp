/*++

Copyright (C) 2018 3MF Consortium

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

UnitTest_Nurbs.cpp: Defines Unittests for the Nurbs Class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class Nurbs : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			
		}
		virtual void TearDown() {
			model.reset();
		}
	
		static PLib3MFModel model;

		static std::string OutFolder;
	};

	PLib3MFModel Nurbs::model;

	std::string Nurbs::OutFolder(sOutFilesPath + "/Nurbs/");
	
	TEST_F(Nurbs, Export)
	{

		auto surface = Nurbs::model->AddNurbsSurface(3, 3, 4, 4);
		surface->AddKnotU(4, 0.0);
		surface->AddKnotU(4, 1.0);
		surface->AddKnotV(4, 0.0);
		surface->AddKnotV(4, 1.0);

		auto uv1id = surface->AddUVCoordinate(0.5, 0.5);
		auto uv2id = surface->AddUVCoordinate(0.75, 0.5);
		auto uv3id = surface->AddUVCoordinate(0.5, 0.75);
		auto edge1id = surface->AddEdgeMapping(2, 0.5, 0.75);

		surface->AddEdgeMappingUVCoordinate(edge1id, 0.213, 0.237);
		surface->AddEdgeMappingUVCoordinate(edge1id, 0.213, 0.294);
		surface->AddEdgeMappingUVCoordinate(edge1id, 0.213, 0.437);

		surface->SetControlPoint(0, 0, 0.0, 0.0, 1.0, 1.0);
		surface->SetControlPoint(1, 0, 0.0, 0.0, 1.0, 1.0 / 3.0);
		surface->SetControlPoint(2, 0, 0.0, 0.0, 1.0, 1.0 / 3.0);
		surface->SetControlPoint(3, 0, 0.0, 0.0, 1.0, 1.0);

		surface->SetControlPoint(0, 1, 2.0, 0.0, 1.0, 1.0 / 3.0);
		surface->SetControlPoint(1, 1, 2.0, 4.0, 1.0, 1.0 / 9.0);
		surface->SetControlPoint(2, 1, -2.0, 4.0, 1.0, 1.0 / 9.0);
		surface->SetControlPoint(3, 1, -2.0, 0.0, 1.0, 1.0 / 3.0);

		surface->SetControlPoint(0, 2, 2.0, 0.0, -1.0, 1.0 / 3.0);
		surface->SetControlPoint(1, 2, 2.0, 4.0, -1.0, 1.0 / 9.0);
		surface->SetControlPoint(2, 2, -2.0, 4.0, -1.0, 1.0 / 9.0);
		surface->SetControlPoint(3, 2, -2.0, 0.0, -1.0, 1.0 / 3.0);

		surface->SetControlPoint(0, 3, 0.0, 0.0, -1.0, 1.0);
		surface->SetControlPoint(1, 3, 0.0, 0.0, -1.0, 1.0 / 3.0);
		surface->SetControlPoint(2, 3, 0.0, 0.0, -1.0, 1.0 / 3.0);
		surface->SetControlPoint(3, 3, 0.0, 0.0, -1.0, 1.0);

		auto mesh = model->AddMeshObject();
		sLib3MFPosition Position;
		Position.m_coordinates[0] = 0;
		Position.m_coordinates[1] = 0;
		Position.m_coordinates[2] = 0;
		mesh->AddVertex(Position);
		mesh->AddVertex(Position);
		mesh->AddVertex(Position);

		sLib3MFTriangle Triangle;
		Triangle.m_indices[0] = 0;
		Triangle.m_indices[1] = 1;
		Triangle.m_indices[2] = 2;
		mesh->AddTriangle(Triangle);

		sLib3MFTriangleNurbsProperties Properties;

		Properties.m_ResourceID = surface->GetResourceID();
		Properties.m_UVIDs[0] = uv1id;
		Properties.m_UVIDs[1] = uv2id;
		Properties.m_UVIDs[2] = uv3id;
		Properties.m_EdgeIDs[0] = edge1id;
		Properties.m_EdgeIDs[1] = 0;
		Properties.m_EdgeIDs[2] = 0;
		mesh->SetTriangleNurbsProperties(0, Properties);

		auto writer = Nurbs::model->QueryWriter("3mf");
		writer->WriteToFile(sOutFilesPath + "nurbstest1.3mf");
	}

	TEST_F(Nurbs, 3MFReadNurbsFromFile)
	{
		std::vector<sLib3MFNURBSKnot> KnotBuffer;
		double dX, dY, dZ, dW;

		auto pReader = Nurbs::model->QueryReader ("3mf");

		pReader->ReadFromFile(sTestFilesPath + "/Nurbs/" + "nurbscylinder.3mf");

		auto CurveIterator = Nurbs::model->GetNurbsCurves();
		ASSERT_TRUE(CurveIterator->MoveNext());

		auto NurbsCurve1 = CurveIterator->GetCurrentNurbsCurve();
		ASSERT_EQ(NurbsCurve1->GetResourceID(), 1);
		ASSERT_EQ(NurbsCurve1->GetDegree (), 2);

		NurbsCurve1->GetKnots(KnotBuffer);
		ASSERT_EQ(KnotBuffer.size (), 3);
		ASSERT_EQ(KnotBuffer[0].m_Multiplicity, 3);
		ASSERT_EQ(KnotBuffer[1].m_Multiplicity, 2);
		ASSERT_EQ(KnotBuffer[2].m_Multiplicity, 3);
		ASSERT_EQ(KnotBuffer[0].m_Value, -3.14159);
		ASSERT_EQ(KnotBuffer[1].m_Value, -1.5708);
		ASSERT_EQ(KnotBuffer[2].m_Value, 0);

		NurbsCurve1->GetControlPoint(0, dX, dY, dZ, dW);
		ASSERT_EQ(dX, -2.95265);
		ASSERT_EQ(dY, 0);
		ASSERT_EQ(dZ, 20);
		ASSERT_EQ(dW, 1);
		NurbsCurve1->GetControlPoint(4, dX, dY, dZ, dW);
		ASSERT_EQ(dX, 42.9527);
		ASSERT_EQ(dY, 0);
		ASSERT_EQ(dZ, 20);
		ASSERT_EQ(dW, 1);


		auto SurfaceIterator = Nurbs::model->GetNurbsSurfaces();
		ASSERT_TRUE(SurfaceIterator->MoveNext());
		
		auto NurbsSurface1 = SurfaceIterator->GetCurrentNurbsSurface();
		ASSERT_EQ(NurbsSurface1->GetResourceID(), 20);
		ASSERT_EQ(NurbsSurface1->GetDegreeU(), 1);
		ASSERT_EQ(NurbsSurface1->GetDegreeV(), 2);

		NurbsSurface1->GetKnotsU(KnotBuffer);
		ASSERT_EQ(KnotBuffer.size(), 2);
		ASSERT_EQ(KnotBuffer[0].m_Multiplicity, 2);
		ASSERT_EQ(KnotBuffer[1].m_Multiplicity, 2);
		ASSERT_EQ(KnotBuffer[0].m_Value, 0);
		ASSERT_EQ(KnotBuffer[1].m_Value, 1.0892);

		NurbsSurface1->GetKnotsV(KnotBuffer);
		ASSERT_EQ(KnotBuffer.size(), 3);
		ASSERT_EQ(KnotBuffer[0].m_Multiplicity, 3);
		ASSERT_EQ(KnotBuffer[1].m_Multiplicity, 2);
		ASSERT_EQ(KnotBuffer[2].m_Multiplicity, 3);
		ASSERT_EQ(KnotBuffer[0].m_Value, -3.14159);
		ASSERT_EQ(KnotBuffer[1].m_Value, -1.5708);
		ASSERT_EQ(KnotBuffer[2].m_Value, 0);

		NurbsSurface1->GetControlPoint(0, 0, dX, dY, dZ, dW);
		ASSERT_EQ(dX, -2.95265);
		ASSERT_EQ(dY, 0);
		ASSERT_EQ(dZ, 20);
		ASSERT_EQ(dW, 1);

	}


}
