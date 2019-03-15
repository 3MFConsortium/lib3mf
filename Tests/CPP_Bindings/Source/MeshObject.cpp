/*++

Copyright (C) 2019 3MF Consortium

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

UnitTest_MeshObject.cpp: Defines Unittests for the mesh object class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class MeshObject : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
			float fSizeX = 100.0f;
			float fSizeY = 200.0f;
			float fSizeZ = 300.0f;

			// Manually create vertices
			pVertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
			pVertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
			pVertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
			pVertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
			pVertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
			pVertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
			pVertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
			pVertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);

			// Manually create triangles
			pTriangles[0] = fnCreateTriangle(2, 1, 0);
			pTriangles[1] = fnCreateTriangle(0, 3, 2);
			pTriangles[2] = fnCreateTriangle(4, 5, 6);
			pTriangles[3] = fnCreateTriangle(6, 7, 4);
			pTriangles[4] = fnCreateTriangle(0, 1, 5);
			pTriangles[5] = fnCreateTriangle(5, 4, 0);
			pTriangles[6] = fnCreateTriangle(2, 3, 7);
			pTriangles[7] = fnCreateTriangle(7, 6, 2);
			pTriangles[8] = fnCreateTriangle(1, 2, 6);
			pTriangles[9] = fnCreateTriangle(6, 5, 1);
			pTriangles[10] = fnCreateTriangle(3, 0, 4);
			pTriangles[11] = fnCreateTriangle(4, 7, 3);
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			mesh = model->AddMeshObject();
		}
		virtual void TearDown() {
			model.reset();
		}
	
		static PLib3MFModel model;
		static PLib3MFMeshObject mesh;
		static sLib3MFPosition pVertices[8];
		static sLib3MFTriangle pTriangles[12];
	};
	PLib3MFModel MeshObject::model;
	PLib3MFMeshObject MeshObject::mesh;
	sLib3MFPosition MeshObject::pVertices[8];
	sLib3MFTriangle MeshObject::pTriangles[12];

	TEST_F(MeshObject, CountFunctions)
	{
		ASSERT_EQ(0, mesh->GetVertexCount());
		ASSERT_EQ(0, mesh->GetTriangleCount());
	}

	TEST_F(MeshObject, SingleVertexOperations)
	{
		sLib3MFPosition pos, posOut;
		pos.m_Coordinates[0] = 0.0;
		pos.m_Coordinates[1] = 0.0;
		pos.m_Coordinates[2] = 0.0;

		mesh->AddVertex(pos);
		ASSERT_EQ(1, mesh->GetVertexCount());

		pos.m_Coordinates[0] = 1.0;
		pos.m_Coordinates[1] = 1.0;
		pos.m_Coordinates[2] = 0.0;
		Lib3MF_uint32 index = mesh->AddVertex(pos);
		ASSERT_EQ(2, mesh->GetVertexCount());
		ASSERT_EQ(index, 1);

		posOut = mesh->GetVertex(1);
		ASSERT_EQ(pos.m_Coordinates[0], posOut.m_Coordinates[0]);
		ASSERT_EQ(pos.m_Coordinates[1], posOut.m_Coordinates[1]);
		ASSERT_EQ(pos.m_Coordinates[2], posOut.m_Coordinates[2]);

		pos.m_Coordinates[0] = 2.0;
		pos.m_Coordinates[1] = 3.0;
		pos.m_Coordinates[2] = 4.0;
		mesh->SetVertex(0, pos);
		posOut = mesh->GetVertex(0);
		ASSERT_EQ(pos.m_Coordinates[0], posOut.m_Coordinates[0]);
		ASSERT_EQ(pos.m_Coordinates[1], posOut.m_Coordinates[1]);
		ASSERT_EQ(pos.m_Coordinates[2], posOut.m_Coordinates[2]);
	}


	TEST_F(MeshObject, SingleTriangleOperations)
	{
		mesh->AddVertex(pVertices[0]);
		mesh->AddVertex(pVertices[1]);
		mesh->AddVertex(pVertices[2]);
		mesh->AddVertex(pVertices[3]);

		sLib3MFTriangle t0 = fnCreateTriangle(2, 1, 0);
		mesh->AddTriangle(t0);
		ASSERT_EQ(1, mesh->GetTriangleCount());

		sLib3MFTriangle t1 = fnCreateTriangle(0, 3, 2);
		Lib3MF_uint32 index = mesh->AddTriangle(t1);
		ASSERT_EQ(1, index);
		ASSERT_EQ(2, mesh->GetTriangleCount());

		sLib3MFTriangle t = mesh->GetTriangle(1);
		ASSERT_EQ(t.m_Indices[0], t1.m_Indices[0]);
		ASSERT_EQ(t.m_Indices[1], t1.m_Indices[1]);
		ASSERT_EQ(t.m_Indices[2], t1.m_Indices[2]);

		mesh->SetTriangle(1, t0);
		t = mesh->GetTriangle(1);
		ASSERT_EQ(t.m_Indices[0], t0.m_Indices[0]);
		ASSERT_EQ(t.m_Indices[1], t0.m_Indices[1]);
		ASSERT_EQ(t.m_Indices[2], t0.m_Indices[2]);
	}

	TEST_F(MeshObject, EmptyGeometryOperations)
	{
		mesh->SetGeometry(CLib3MFInputVector<sLib3MFPosition>(pVertices, 8), CLib3MFInputVector<sLib3MFTriangle>(nullptr, 0));
		mesh->SetGeometry(CLib3MFInputVector<sLib3MFPosition>(nullptr, 0), CLib3MFInputVector<sLib3MFTriangle>(nullptr, 0));
		ASSERT_SPECIFIC_THROW(mesh->SetGeometry(CLib3MFInputVector<sLib3MFPosition>(pVertices, 8), CLib3MFInputVector<sLib3MFTriangle>(nullptr, 1)), ELib3MFException);
		ASSERT_SPECIFIC_THROW(mesh->SetGeometry(CLib3MFInputVector<sLib3MFPosition>(nullptr, 0), CLib3MFInputVector<sLib3MFTriangle>(pTriangles, 12)), ELib3MFException);
	}

	TEST_F(MeshObject, GeometryOperations)
	{
		mesh->SetGeometry(CLib3MFInputVector<sLib3MFPosition>(pVertices, 8), CLib3MFInputVector<sLib3MFTriangle>(pTriangles, 12));
		ASSERT_EQ(mesh->GetVertexCount(), 8);
		ASSERT_EQ(mesh->GetTriangleCount(), 12);
		
		std::vector<sLib3MFPosition> vctPositions;
		mesh->GetVertices(vctPositions);
		ASSERT_EQ(vctPositions.size(), mesh->GetVertexCount());
		for (Lib3MF_uint32 i = 0; i < mesh->GetVertexCount(); i++)
		{
			for (int j = 0; j < 3; j++)
				ASSERT_EQ(pVertices[i].m_Coordinates[j], vctPositions[i].m_Coordinates[j]);
		}

		std::vector<sLib3MFTriangle> vctTriangles;
		mesh->GetTriangleIndices(vctTriangles);
		ASSERT_EQ(vctTriangles.size(), mesh->GetTriangleCount());
		for (Lib3MF_uint32 i = 0; i < mesh->GetTriangleCount(); i++)
		{
			for (int j = 0; j < 3; j++)
				ASSERT_EQ(pTriangles[i].m_Indices[j], vctTriangles[i].m_Indices[j]);
		}
		
	}

	TEST_F(MeshObject, IsManifoldAndOriented)
	{
		ASSERT_FALSE(mesh->IsManifoldAndOriented());
		mesh->SetGeometry(CLib3MFInputVector<sLib3MFPosition>(pVertices, 8), CLib3MFInputVector<sLib3MFTriangle>(pTriangles, 12));
		ASSERT_TRUE(mesh->IsManifoldAndOriented());
	}

	TEST_F(MeshObject, IsValid)
	{
		ASSERT_FALSE(mesh->IsValid());
		mesh->SetType(eLib3MFObjectType::eObjectTypeOther);
		ASSERT_FALSE(mesh->IsValid());
		mesh->SetType(eLib3MFObjectType::eObjectTypeSolidSupport);
		ASSERT_FALSE(mesh->IsValid());
		mesh->SetType(eLib3MFObjectType::eObjectTypeSupport);
		ASSERT_TRUE(mesh->IsValid());
		mesh->SetType(eLib3MFObjectType::eObjectTypeModel);
		ASSERT_FALSE(mesh->IsValid());
	}


	TEST_F(MeshObject, BeamLattice)
	{
		auto beamLattice = mesh->BeamLattice();
	}
	
}


