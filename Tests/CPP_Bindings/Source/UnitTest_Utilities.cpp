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

void fnCreateBox(std::vector<Lib3MF::sPosition> &vctVertices, std::vector<Lib3MF::sTriangle> &vctTriangles) {
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

void CompareColors(Lib3MF::sColor c1, Lib3MF::sColor c2)
{
	EXPECT_EQ(c1.m_Alpha, c2.m_Alpha);
	EXPECT_EQ(c1.m_Red, c2.m_Red);
	EXPECT_EQ(c1.m_Green, c2.m_Green);
	EXPECT_EQ(c1.m_Blue, c2.m_Blue);
}

void CompareTransforms(Lib3MF::sTransform t1, Lib3MF::sTransform t2)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			ASSERT_EQ(t1.m_Fields[i][j], t2.m_Fields[i][j]);
}

void CompareFieldReferences(Lib3MF::PModel modelA, Lib3MF::PFieldReference A, Lib3MF::PModel modelB, Lib3MF::PFieldReference B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr) {
		CompareTransforms(A->GetTransform(), B->GetTransform());

		auto scalarFieldA = modelA->GetScalarFieldByID(A->GetFieldResourceID());
		auto scalarFieldB = modelB->GetScalarFieldByID(B->GetFieldResourceID());
		ASSERT_EQ(scalarFieldA == nullptr, scalarFieldB == nullptr);
		if (scalarFieldA != nullptr && scalarFieldB != nullptr) {
			CompareScalarFields(modelA, scalarFieldA, modelB, scalarFieldB);
		}
		auto vectorField3DA = modelA->GetVector3DFieldByID(A->GetFieldResourceID());
		auto vectorField3DB = modelB->GetVector3DFieldByID(B->GetFieldResourceID());
		ASSERT_EQ(vectorField3DA == nullptr, vectorField3DB == nullptr);
		if (scalarFieldA != nullptr && scalarFieldB != nullptr) {
			CompareVector3DFields(modelA, vectorField3DA, modelB, vectorField3DB);
		}
	}
}

void CompareScalarFieldReferences(Lib3MF::PModel modelA, Lib3MF::PScalarFieldReference A, Lib3MF::PModel modelB, Lib3MF::PScalarFieldReference B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr) {
		CompareTransforms(A->GetTransform(), B->GetTransform());

		auto scalarFieldA = modelA->GetScalarFieldByID(A->GetFieldResourceID());
		auto scalarFieldB = modelB->GetScalarFieldByID(B->GetFieldResourceID());
		ASSERT_EQ(scalarFieldA == nullptr, scalarFieldB == nullptr);
		if (scalarFieldA != nullptr && scalarFieldB != nullptr) {
			CompareScalarFields(modelA, scalarFieldA, modelB, scalarFieldB);
		}
	}
}

void CompareVector3DFieldReferences(Lib3MF::PModel modelA, Lib3MF::PVector3DFieldReference A, Lib3MF::PModel modelB, Lib3MF::PVector3DFieldReference B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr)
	{
		CompareTransforms(A->GetTransform(), B->GetTransform());

		auto vectorField3DA = modelA->GetVector3DFieldByID(A->GetFieldResourceID());
		auto vectorField3DB = modelB->GetVector3DFieldByID(B->GetFieldResourceID());
		ASSERT_EQ(vectorField3DA == nullptr, vectorField3DB == nullptr);
		if (vectorField3DA != nullptr && vectorField3DB != nullptr) {
			CompareVector3DFields(modelA, vectorField3DA, modelB, vectorField3DB);
		}
	}
}

void CompareImage3Ds(Lib3MF::PModel modelA, Lib3MF::PImage3D i1, Lib3MF::PModel modelB, Lib3MF::PImage3D i2)
{
	ASSERT_EQ(i1->GetName(), i2->GetName());
	ASSERT_EQ(i1->IsImageStack(), i2->IsImageStack());
	if (i1->IsImageStack())
	{
		Lib3MF::PImageStack stack1 = modelA->GetImageStackByID(i1->GetUniqueResourceID());
		Lib3MF::PImageStack stack2 = modelB->GetImageStackByID(i2->GetUniqueResourceID());
		
		CompareImageStacks(stack1, stack2);
	}
}

void CompareImageStacks(Lib3MF::PImageStack i1, Lib3MF::PImageStack i2)
{
	ASSERT_EQ(i1->GetColumnCount(), i2->GetColumnCount());
	ASSERT_EQ(i1->GetRowCount(), i2->GetRowCount());
	ASSERT_EQ(i1->GetSheetCount(), i2->GetSheetCount());
	for (Lib3MF_uint32 i = 0; i < i1->GetSheetCount(); i++)
	{
		Lib3MF::PAttachment attachment1 = i1->GetSheet(i);
		Lib3MF::PAttachment attachment2 = i2->GetSheet(i);

		ASSERT_EQ(attachment1->GetPath(), attachment2->GetPath());
		ASSERT_EQ(attachment1->GetStreamSize(), attachment2->GetStreamSize());
	}
}

void CompareScalarFields(Lib3MF::PModel modelA, Lib3MF::PScalarField A, Lib3MF::PModel modelB, Lib3MF::PScalarField B)
{
	if (A->IsFromImage3D() && B->IsFromImage3D())
	{
		CompareScalarFieldFromImage3D(modelA, modelA->GetScalarFieldFromImage3DByID(A->GetUniqueResourceID()),
			modelB, modelB->GetScalarFieldFromImage3DByID(B->GetUniqueResourceID()));
	}

	if (A->IsConstant() && B->IsConstant())
	{
		CompareScalarFieldConstant(modelA, modelA->GetScalarFieldConstantByID(A->GetUniqueResourceID()),
			modelB, modelB->GetScalarFieldConstantByID(B->GetUniqueResourceID()));
	}

	if (A->IsComposed() && B->IsComposed())
	{
		CompareScalarFieldComposed(modelA, modelA->GetScalarFieldComposedByID(A->GetUniqueResourceID()),
			modelB, modelB->GetScalarFieldComposedByID(B->GetUniqueResourceID()));
	}
}

void CompareScalarFieldConstant(Lib3MF::PModel modelA, Lib3MF::PScalarFieldConstant A, Lib3MF::PModel modelB, Lib3MF::PScalarFieldConstant B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr)
	{
		ASSERT_EQ(A->GetName(), B->GetName());
		ASSERT_EQ(A->GetValue(), B->GetValue());
	}
}

void CompareScalarFieldFromImage3D(Lib3MF::PModel modelA, Lib3MF::PScalarFieldFromImage3D A, Lib3MF::PModel modelB, Lib3MF::PScalarFieldFromImage3D B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr)
	{
		ASSERT_EQ(A->GetName(), B->GetName());
		CompareImage3Ds(modelA, A->GetImage(), modelB, B->GetImage());
		ASSERT_EQ(A->GetOffset(), B->GetOffset());
		ASSERT_EQ(A->GetScale(), B->GetScale());
		ASSERT_EQ(A->GetFilter(), B->GetFilter());
	}
}

void CompareScalarFieldComposed(Lib3MF::PModel modelA, Lib3MF::PScalarFieldComposed A, Lib3MF::PModel modelB, Lib3MF::PScalarFieldComposed B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr)
	{
		ASSERT_EQ(A->GetName(), B->GetName());
		ASSERT_EQ(A->GetFactor1(), B->GetFactor1());
		ASSERT_EQ(A->GetFactor2(), B->GetFactor2());
		ASSERT_EQ(A->GetMethod(), B->GetMethod());
		CompareScalarFieldReferences(modelA, A->ScalarFieldReference1(), modelB, B->ScalarFieldReference1());
		CompareScalarFieldReferences(modelA, A->ScalarFieldReference2(), modelB, B->ScalarFieldReference2());
	}
}

void CompareVector3DFields(Lib3MF::PModel modelA, Lib3MF::PVector3DField A, Lib3MF::PModel modelB, Lib3MF::PVector3DField B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr)
	{
		if (A->IsFromImage3D() && B->IsFromImage3D())
		{
			CompareVector3DFieldFromImage3D(modelA, modelA->GetVector3DFieldFromImage3DByID(A->GetUniqueResourceID()),
				modelB, modelB->GetVector3DFieldFromImage3DByID(B->GetUniqueResourceID()));
		}
		if (A->IsConstant() && B->IsConstant())
		{
			CompareVector3DFieldConstant(modelA, modelA->GetVector3DFieldConstantByID(A->GetUniqueResourceID()),
				modelB, modelB->GetVector3DFieldConstantByID(B->GetUniqueResourceID()));
		}
		if (A->IsComposed() && B->IsComposed())
		{
			CompareVector3DFieldComposed(modelA, modelA->GetVector3DFieldComposedByID(A->GetUniqueResourceID()),
				modelB, modelB->GetVector3DFieldComposedByID(B->GetUniqueResourceID()));
		}
	}
}

void CompareVector3DFieldConstant(Lib3MF::PModel modelA, Lib3MF::PVector3DFieldConstant A, Lib3MF::PModel modelB, Lib3MF::PVector3DFieldConstant B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr)
	{
		ASSERT_EQ(A->GetName(), B->GetName());
		ASSERT_EQ(A->GetValueX(), B->GetValueX());
		ASSERT_EQ(A->GetValueY(), B->GetValueY());
		ASSERT_EQ(A->GetValueZ(), B->GetValueZ());
	}
}

void CompareVector3DFieldFromImage3D(Lib3MF::PModel modelA, Lib3MF::PVector3DFieldFromImage3D A, Lib3MF::PModel modelB, Lib3MF::PVector3DFieldFromImage3D B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr)
	{
		ASSERT_EQ(A->GetName(), B->GetName());
		CompareImage3Ds(modelA, A->GetImage(), modelB, B->GetImage());
		ASSERT_EQ(A->GetOffset(), B->GetOffset());
		ASSERT_EQ(A->GetScale(), B->GetScale());
		ASSERT_EQ(A->GetFilter(), B->GetFilter());
	}
}

void CompareVector3DFieldComposed(Lib3MF::PModel modelA, Lib3MF::PVector3DFieldComposed A, Lib3MF::PModel modelB, Lib3MF::PVector3DFieldComposed B)
{
	ASSERT_EQ(A == nullptr, B == nullptr);
	if (A != nullptr && B != nullptr)
	{
		ASSERT_EQ(A->GetName(), B->GetName());
		ASSERT_EQ(A->GetFactor1(), B->GetFactor1());
		ASSERT_EQ(A->GetFactor2(), B->GetFactor2());
		ASSERT_EQ(A->GetMethod(), B->GetMethod());
		CompareVector3DFieldReferences(modelA, A->Vector3DFieldReference1(), modelB, B->Vector3DFieldReference1());
		CompareVector3DFieldReferences(modelA, A->Vector3DFieldReference2(), modelB, B->Vector3DFieldReference2());
	}
}

void CompareVolumeData(Lib3MF::PModel modelA, Lib3MF::PVolumeData A, Lib3MF::PModel modelB, Lib3MF::PVolumeData B)
{
	ASSERT_EQ(A->GetBoundary() == nullptr, B->GetBoundary() == nullptr);
	if (A->GetBoundary())
	{
		CompareScalarFieldReferences(modelA, A->GetBoundary(), modelB, B->GetBoundary());
		ASSERT_EQ(A->GetBoundary()->GetSolidThreshold(), B->GetBoundary()->GetSolidThreshold());
	}

	ASSERT_EQ(A->GetColor() == nullptr, B->GetColor() == nullptr);
	if (A->GetColor())
	{
		CompareVector3DFieldReferences(modelA, A->GetColor(), modelB, B->GetColor());
	}

	ASSERT_EQ(A->GetPropertyCount(), B->GetPropertyCount());
	for (Lib3MF_uint32 i=0; i<A->GetPropertyCount(); i++)
	{
		auto propertyA = A->GetProperty(i);
		auto propertyB = B->GetProperty(i);
		ASSERT_EQ(propertyA->GetName(), propertyB->GetName());
		CompareFieldReferences(modelA, propertyA, modelB, propertyB);
	}
	// TODO
	return;
	//ASSERT_EQ(A->GetComposite() == nullptr, B->GetComposite() == nullptr);
	//if (A->GetComposite() != nullptr)
	//{
		//auto compositeA = A->GetComposite();
		//auto compositeB = B->GetComposite();
		////CompareBaseMaterialGroups(compositeA->GetBaseMaterialGroup(), compositeB->GetBaseMaterialGroup());
		//ASSERT_EQ(compositeA->GetMaterialMappingCount(), compositeB->GetMaterialMappingCount());
		//for (Lib3MF_uint32 i = 0; i < compositeA->GetMaterialMappingCount(); i++)
		//{
		//	auto materialMappingA = compositeA->GetMaterialMapping(i);
		//	auto materialMappingB = compositeB->GetMaterialMapping(i);
		//	CompareScalarFieldReferences(modelA, materialMappingA, modelB, materialMappingB);
		//}
	//}
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
