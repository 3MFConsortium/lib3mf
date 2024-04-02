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

UnitTest_Utilities.cpp: Implementtion of Utilities for UnitTests

--*/


#include "UnitTest_Utilities.h"

Lib3MF::PWrapper Lib3MFTest::wrapper;

namespace Lib3MF
{
    namespace helper
    {
        bool directoryExists(std::string& path)
        {
            struct stat info;
            if(stat(path.c_str(), &info) != 0)
                return false;
            else if(info.st_mode & S_IFDIR)
                return true;
            else
                return false;
        }

        void comparePorts(Lib3MF::PImplicitPortIterator const& portIterator1,
                          Lib3MF::PImplicitPortIterator const& portIterator2,
                          bool ignoreReference)
        {
            ASSERT_EQ(portIterator1->Count(), portIterator2->Count());
            while(portIterator1->MoveNext())
            {
                EXPECT_TRUE(portIterator2->MoveNext());

                auto port1 = portIterator1->GetCurrent();
                auto port2 = portIterator2->GetCurrent();

                EXPECT_EQ(port1->GetIdentifier(), port2->GetIdentifier());
                EXPECT_EQ(port1->GetType(), port2->GetType());
                EXPECT_EQ(port1->GetDisplayName(), port2->GetDisplayName());
                if(!ignoreReference)
                {
                    EXPECT_EQ(port1->GetReference(), port2->GetReference());
                }
            }
            EXPECT_FALSE(portIterator2->MoveNext());
        }

        void compareImplicitFunctions(PImplicitFunction const& function1,
                                      PImplicitFunction const& function2)
        {
            EXPECT_EQ(function1->GetDisplayName(), function2->GetDisplayName());
            EXPECT_EQ(function1->GetModelResourceID(),
                      function2->GetModelResourceID());

            auto nodeIterator1 = function1->GetNodes();
            auto nodeIterator2 = function2->GetNodes();
            ASSERT_EQ(nodeIterator1->Count(), nodeIterator2->Count());
            while(nodeIterator1->MoveNext())
            {
                EXPECT_TRUE(nodeIterator2->MoveNext());

                auto node1 = nodeIterator1->GetCurrent();
                auto node2 = nodeIterator2->GetCurrent();

                EXPECT_TRUE(node1->AreTypesValid());
                EXPECT_TRUE(node2->AreTypesValid());

                EXPECT_EQ(node1->GetIdentifier(), node2->GetIdentifier());
                EXPECT_EQ(node1->GetNodeType(), node2->GetNodeType());
                EXPECT_EQ(node1->GetDisplayName(), node2->GetDisplayName());

                
                // if(constantNode1 != nullptr && constantNode2 != nullptr)
                if (node1->GetNodeType() == Lib3MF::eImplicitNodeType::Constant)
                {
                    auto constantNode1 = std::dynamic_pointer_cast<CConstantNode>(node1);
                    auto constantNode2 = std::dynamic_pointer_cast<CConstantNode>(node2);
                    ASSERT_NE(constantNode1, nullptr);
                    EXPECT_EQ(constantNode1->GetConstant(), constantNode2->GetConstant());
                }

                comparePorts(node1->GetInputs(), node2->GetInputs(), false);
                comparePorts(node1->GetOutputs(), node2->GetOutputs(),
                             true);  // ignore reference
            }
            EXPECT_FALSE(nodeIterator2->MoveNext());

            auto inputs1 = function1->GetInputs();
            auto inputs2 = function2->GetInputs();
            ASSERT_EQ(inputs1->Count(), inputs2->Count());
            while(inputs1->MoveNext())
            {
                EXPECT_TRUE(inputs2->MoveNext());
                EXPECT_EQ(inputs1->GetCurrent()->GetDisplayName(),
                          inputs2->GetCurrent()->GetDisplayName());
                EXPECT_EQ(inputs1->GetCurrent()->GetIdentifier(),
                          inputs2->GetCurrent()->GetIdentifier());
                EXPECT_EQ(inputs1->GetCurrent()->GetType(),
                          inputs2->GetCurrent()->GetType());
            }
            EXPECT_FALSE(inputs2->MoveNext());

            auto outputs1 = function1->GetOutputs();
            auto outputs2 = function2->GetOutputs();
            ASSERT_EQ(outputs1->Count(), outputs2->Count());
            while(outputs1->MoveNext())
            {
                EXPECT_TRUE(outputs2->MoveNext());
                EXPECT_EQ(outputs1->GetCurrent()->GetDisplayName(),
                          outputs2->GetCurrent()->GetDisplayName());
                EXPECT_EQ(outputs1->GetCurrent()->GetIdentifier(),
                          outputs2->GetCurrent()->GetIdentifier());
                EXPECT_EQ(outputs1->GetCurrent()->GetType(),
                          outputs2->GetCurrent()->GetType());
            }
            EXPECT_FALSE(outputs2->MoveNext());
        }

        void compareFunctionsFromImage3D(Lib3MF::PModel model1,
                                         PFunctionFromImage3D const& function1,
                                         Lib3MF::PModel model2,
                                         PFunctionFromImage3D const& function2)
        {
            EXPECT_EQ(function1->GetDisplayName(), function2->GetDisplayName());
            EXPECT_EQ(function1->GetModelResourceID(),
                      function2->GetModelResourceID());

            EXPECT_EQ(function1->GetFilter(), function2->GetFilter());

            eTextureTileStyle eTileStyleU1, eTileStyleV1, eTileStyleW1;
            eTextureTileStyle eTileStyleU2, eTileStyleV2, eTileStyleW2;
            function1->GetTileStyles(eTileStyleU1, eTileStyleV1, eTileStyleW1);
            function2->GetTileStyles(eTileStyleU2, eTileStyleV2, eTileStyleW2);
            EXPECT_EQ(eTileStyleU1, eTileStyleU2);
            EXPECT_EQ(eTileStyleV1, eTileStyleV2);
            EXPECT_EQ(eTileStyleW1, eTileStyleW2);

            EXPECT_EQ(function1->GetOffset(), function2->GetOffset());
            EXPECT_EQ(function1->GetScale(), function2->GetScale());

            CompareImage3Ds(model1, function1->GetImage3D(), model2, function2->GetImage3D());
        }

        void compareFunctions(Lib3MF::PModel model1, PFunction const& function1,
                              Lib3MF::PModel model2, PFunction const& function2)
        {
            PImplicitFunction implicitFunction1 =
                std::dynamic_pointer_cast<CImplicitFunction>(function1);

            PImplicitFunction implicitFunction2 =
                std::dynamic_pointer_cast<CImplicitFunction>(function2);

            if(implicitFunction1 != nullptr && implicitFunction2 != nullptr)
            {
                compareImplicitFunctions(implicitFunction1, implicitFunction2);
            }
            else
            {
                PFunctionFromImage3D functionFromImage3D1 =
                    std::dynamic_pointer_cast<CFunctionFromImage3D>(function1);

                PFunctionFromImage3D functionFromImage3D2 =
                    std::dynamic_pointer_cast<CFunctionFromImage3D>(function2);

                compareFunctionsFromImage3D(model1, functionFromImage3D1,
                                            model2, functionFromImage3D2);
            }
        }

        void CompareColors(Lib3MF::sColor c1, Lib3MF::sColor c2)
        {
            EXPECT_EQ(c1.m_Alpha, c2.m_Alpha);
            EXPECT_EQ(c1.m_Red, c2.m_Red);
            EXPECT_EQ(c1.m_Green, c2.m_Green);
            EXPECT_EQ(c1.m_Blue, c2.m_Blue);
        }

        void CompareTransforms(Lib3MF::sTransform t1, Lib3MF::sTransform t2)
        {
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 3; j++)
                    ASSERT_EQ(t1.m_Fields[i][j], t2.m_Fields[i][j]);
        }

        void CompareFunctionReferences(Lib3MF::PModel modelA,
                                       Lib3MF::PFunctionReference A,
                                       Lib3MF::PModel modelB,
                                       Lib3MF::PFunctionReference B)
        {
            ASSERT_EQ(A == nullptr, B == nullptr);
            if(A != nullptr && B != nullptr)
            {
                Lib3MF::PResource fieldResourceA =
                    modelA->GetResourceByID(A->GetFunctionResourceID());
                Lib3MF::PResource fieldResourceB =
                    modelB->GetResourceByID(B->GetFunctionResourceID());

                auto functionA = std::dynamic_pointer_cast<Lib3MF::CFunction>(
                    fieldResourceA);
                auto functionB = std::dynamic_pointer_cast<Lib3MF::CFunction>(
                    fieldResourceB);

                CompareTransforms(A->GetTransform(), B->GetTransform());
                
                ASSERT_EQ(functionA == nullptr, functionB == nullptr);
                if(functionA != nullptr && functionB != nullptr)
                {
                    Lib3MF::helper::compareFunctions(modelA, functionA, modelB, functionB);
                }
            }
        }

        void CompareImage3Ds(Lib3MF::PModel modelA, Lib3MF::PImage3D imgA,
                             Lib3MF::PModel modelB, Lib3MF::PImage3D imbB)
        {
            ASSERT_EQ(imgA->GetName(), imbB->GetName());
            ASSERT_EQ(imgA->IsImageStack(), imbB->IsImageStack());
            if(imgA->IsImageStack())
            {
                Lib3MF::PImageStack stackA =
                    modelA->GetImageStackByID(imgA->GetUniqueResourceID());
                Lib3MF::PImageStack stackB =
                    modelB->GetImageStackByID(imbB->GetUniqueResourceID());

                CompareImageStacks(stackA, stackB);
            }
        }

        void CompareImageStacks(Lib3MF::PImageStack i1, Lib3MF::PImageStack i2)
        {
            ASSERT_EQ(i1->GetColumnCount(), i2->GetColumnCount());
            ASSERT_EQ(i1->GetRowCount(), i2->GetRowCount());
            ASSERT_EQ(i1->GetSheetCount(), i2->GetSheetCount());
            for(Lib3MF_uint32 i = 0; i < i1->GetSheetCount(); i++)
            {
                Lib3MF::PAttachment attachment1 = i1->GetSheet(i);
                Lib3MF::PAttachment attachment2 = i2->GetSheet(i);

                ASSERT_EQ(attachment1->GetPath(), attachment2->GetPath());
                ASSERT_EQ(attachment1->GetStreamSize(),
                          attachment2->GetStreamSize());
            }
        }

        void CompareVolumeData(Lib3MF::PModel modelA, Lib3MF::PVolumeData A,
                               Lib3MF::PModel modelB, Lib3MF::PVolumeData B)
        {
            ASSERT_EQ(A->GetColor() == nullptr, B->GetColor() == nullptr);
            if(A->GetColor())
            {
                CompareFunctionReferences(modelA, A->GetColor(), modelB,
                                          B->GetColor());
            }

            ASSERT_EQ(A->GetPropertyCount(), B->GetPropertyCount());
            for(Lib3MF_uint32 i = 0; i < A->GetPropertyCount(); i++)
            {
                auto propertyA = A->GetProperty(i);
                auto propertyB = B->GetProperty(i);
                ASSERT_EQ(propertyA->GetName(), propertyB->GetName());
                ASSERT_EQ(propertyA->IsRequired(), propertyB->IsRequired());
                CompareFunctionReferences(modelA, propertyA, modelB, propertyB);
            }
            // TODO
            return;
            // ASSERT_EQ(A->GetComposite() == nullptr, B->GetComposite() ==
            // nullptr); if (A->GetComposite() != nullptr)
            //{
            // auto compositeA = A->GetComposite();
            // auto compositeB = B->GetComposite();
            ////CompareBaseMaterialGroups(compositeA->GetBaseMaterialGroup(),
            ///compositeB->GetBaseMaterialGroup());
            // ASSERT_EQ(compositeA->GetMaterialMappingCount(),
            // compositeB->GetMaterialMappingCount()); for (Lib3MF_uint32 i = 0;
            // i < compositeA->GetMaterialMappingCount(); i++)
            //{
            //	auto materialMappingA = compositeA->GetMaterialMapping(i);
            //	auto materialMappingB = compositeB->GetMaterialMapping(i);
            //	CompareScalarFieldReferences(modelA, materialMappingA, modelB,
            //materialMappingB);
            // }
            //}
        }

        Lib3MF::sTransform ComputeTransformFromMeshCoordinatesToUVW(
            Lib3MF::PMeshObject mesh)
        {
            // 1. Get the bounding box of the mesh
            Lib3MF::sBox box = mesh->GetOutbox();

           
            EXPECT_GT(box.m_MaxCoordinate[0] - box.m_MinCoordinate[0], 0.f);
            EXPECT_GT(box.m_MaxCoordinate[1] - box.m_MinCoordinate[1], 0.f);
            EXPECT_GT(box.m_MaxCoordinate[2] - box.m_MinCoordinate[2], 0.f);

            // 2. Calculate the transform
            Lib3MF::sTransform transform =  getIdentityTransform();


            // scaling
            transform.m_Fields[0][0] = 1.0f / (box.m_MaxCoordinate[0] -
                                               box.m_MinCoordinate[0]);
            transform.m_Fields[1][1] = 1.0f / (box.m_MaxCoordinate[1] -
                                               box.m_MinCoordinate[1]);
            transform.m_Fields[2][2] = 1.0f / (box.m_MaxCoordinate[2] -
                                                  box.m_MinCoordinate[2]); 

            // translation
            transform.m_Fields[3][0] = -box.m_MinCoordinate[0] * transform.m_Fields[0][0];
            transform.m_Fields[3][1] = -box.m_MinCoordinate[1] * transform.m_Fields[1][1];
            transform.m_Fields[3][2] = -box.m_MinCoordinate[2] * transform.m_Fields[2][2];

            return transform;
        }
    }  // namespace helper
}  // namespace Lib3MF

void fnCreateBox(std::vector<Lib3MF::sPosition>& vctVertices,
                     std::vector<Lib3MF::sTriangle>& vctTriangles)
    {
	float fSizeX = 100.0f;
	float fSizeY = 100.0f;
	float fSizeZ = 100.0f;

	vctVertices.resize(8);
	vctTriangles.resize(12);

	// Manually create vertices
	vctVertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
	vctVertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
	vctVertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
	vctVertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
	vctVertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
	vctVertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
	vctVertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
	vctVertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);


	// Manually create triangles
	vctTriangles[0] = fnCreateTriangle(2, 1, 0);
	vctTriangles[1] = fnCreateTriangle(0, 3, 2);
	vctTriangles[2] = fnCreateTriangle(4, 5, 6);
	vctTriangles[3] = fnCreateTriangle(6, 7, 4);
	vctTriangles[4] = fnCreateTriangle(0, 1, 5);
	vctTriangles[5] = fnCreateTriangle(5, 4, 0);
	vctTriangles[6] = fnCreateTriangle(2, 3, 7);
	vctTriangles[7] = fnCreateTriangle(7, 6, 2);
	vctTriangles[8] = fnCreateTriangle(1, 2, 6);
	vctTriangles[9] = fnCreateTriangle(6, 5, 1);
	vctTriangles[10] = fnCreateTriangle(3, 0, 4);
	vctTriangles[11] = fnCreateTriangle(4, 7, 3);
}

std::vector<Lib3MF_uint8> ReadFileIntoBuffer(std::string sFileName)
{
	// Read the file fully into a memory buffer
	std::ifstream file(sFileName, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	// Memory buffer
	std::vector<Lib3MF_uint8> buffer(static_cast<size_t>(size));
	file.read((char*)buffer.data(), size);
	return buffer;
}

void WriteBufferToFile(std::vector<Lib3MF_uint8> const& buffer, std::string sFileName)
{
	std::ofstream file(sFileName, std::ios::binary);
	for (Lib3MF_uint8 cByte : buffer) {
		file.put(cByte);
	}
	file.close();
}

sLib3MFTransform getIdentityTransform()
{
	sLib3MFTransform t;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
			t.m_Fields[i][j] = 0 + 1.0f * (i == j);
	}
	return t;
}



sLib3MFPosition fnCreateVertex(float x, float y, float z)
{
	sLib3MFPosition result;
	result.m_Coordinates[0] = x;
	result.m_Coordinates[1] = y;
	result.m_Coordinates[2] = z;
	return result;
}

sLib3MFTriangle fnCreateTriangle(int v0, int v1, int v2)
{
	sLib3MFTriangle result;
	result.m_Indices[0] = v0;
	result.m_Indices[1] = v1;
	result.m_Indices[2] = v2;
	return result;
}

void CheckReaderWarnings(Lib3MF::PReader reader, Lib3MF_uint32 nWarnings)
{
	EXPECT_EQ(reader->GetWarningCount(), nWarnings);
	for (Lib3MF_uint32 iWarning = 0; iWarning < reader->GetWarningCount(); iWarning++)
	{
		Lib3MF_uint32 nErrorCode;
		std::string sWarning = reader->GetWarning(iWarning, nErrorCode);
		EXPECT_TRUE(true) << iWarning << ": " << nErrorCode << ", " << sWarning;
	}
}
