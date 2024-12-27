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
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
    class TriangleSet : public Lib3MFTest {
    protected:
        virtual void SetUp() {
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

            model = wrapper->CreateModel();
            mesh = model->AddMeshObject();
        }
        virtual void TearDown() {
            model.reset();
        }

        static PModel model;
        static PMeshObject mesh;
        static PTriangleSet triangleSet;
        static sPosition pVertices[8];
        static sTriangle pTriangles[12];
        static std::string InFolder;
        static std::string OutFolder;
        static PWrapper wrapper;

        static void SetUpTestCase() {
            wrapper = CWrapper::loadLibrary();
        }
    };

    PWrapper TriangleSet::wrapper;
    PModel TriangleSet::model;
    PMeshObject TriangleSet::mesh;
    sPosition TriangleSet::pVertices[8];
    sTriangle TriangleSet::pTriangles[12];
    std::string TriangleSet::InFolder(sTestFilesPath + "/TriangleSets/");
    std::string TriangleSet::OutFolder(sOutFilesPath + "/TriangleSets/");

    TEST_F(TriangleSet, meshTriangleSet)
    {
        mesh->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
        ASSERT_EQ(mesh->GetVertexCount(), 8);
        ASSERT_EQ(mesh->GetTriangleCount(), 12);

        auto numTriangles = mesh->GetTriangleCount();
        mesh->AddTriangleSet("test_id", "test_name");
        PTriangleSet triSet = mesh->GetTriangleSet(0);
        for (Lib3MF_uint32 i = 0; i < numTriangles; i += 2)
        {
            triSet->AddTriangle(i);
        }
        std::vector<Lib3MF_uint32> triangleIndices;
        triSet->GetTriangleList(triangleIndices);
        ASSERT_EQ(triSet->GetName(), "test_name");
        ASSERT_EQ(triSet->GetIdentifier(), "test_id");
        ASSERT_EQ(triangleIndices.size(), 6);

        // check adding duplicate
        triSet->AddTriangle(0);
        std::vector<Lib3MF_uint32> triangleIndices2;
        triSet->GetTriangleList(triangleIndices2);
        ASSERT_EQ(triangleIndices2.size(), 6);

        mesh->AddTriangleSet("test_id_1", "test_name");
        auto triSet1 = mesh->FindTriangleSet("test_id_1");
        triSet1->AddTriangle(1);
        triSet1->AddTriangle(3);
        std::vector<Lib3MF_uint32> triangleIndices3;
        triSet1->GetTriangleList(triangleIndices3);
        ASSERT_EQ(triangleIndices3.size(), 2);

        ASSERT_EQ(mesh->GetTriangleSetCount(), 2);

        ASSERT_FALSE(mesh->HasTriangleSet("test"));

        triSet1->DeleteSet();

        ASSERT_EQ(mesh->GetTriangleSetCount(), 1);

        ASSERT_FALSE(mesh->HasTriangleSet("test_id_1"));

        mesh->AddTriangleSet("test_id_2", "test_name_2");
        auto triSet2 = mesh->FindTriangleSet("test_id_2");
        triSet2->AddTriangle(1);
        triSet2->AddTriangle(3);

        triSet2->Merge(triSet, false);

        std::vector<Lib3MF_uint32> triangleIndices4;
        triSet2->GetTriangleList(triangleIndices4);
        ASSERT_EQ(triangleIndices4.size(), 8);

    }

    TEST_F(TriangleSet, meshTriangleSetReaderWriter)
    {
        mesh->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
        auto numTriangles = mesh->GetTriangleCount();
        mesh->AddTriangleSet("test_id", "test_name");
        PTriangleSet triSet = mesh->GetTriangleSet(0);
        for (Lib3MF_uint32 i = 0; i < numTriangles; i++)
        {
            triSet->AddTriangle(i);
        }
        PWriter writer = model->QueryWriter("3mf");
        std::vector<Lib3MF_uint8> buffer;
        writer->WriteToBuffer(buffer);

        PModel model2 = wrapper->CreateModel();
        PReader reader3 = model2->QueryReader("3mf");
        reader3->ReadFromBuffer(buffer);

        ASSERT_EQ(model2->GetMeshObjects()->Count(), 1);

        PMeshObject mesh2 = model2->GetMeshObjectByID(1);
        ASSERT_TRUE(mesh2->HasTriangleSet("test_id"));
        ASSERT_EQ(mesh2->GetTriangleSetCount(), 1);

        PTriangleSet triSet2 = mesh2->GetTriangleSet(0);
        ASSERT_EQ(triSet2->GetName(), "test_name");

        std::vector<Lib3MF_uint32> triangleIndices;
        triSet2->GetTriangleList(triangleIndices);
        ASSERT_EQ(triangleIndices.size(), 12);
    }

    

}


