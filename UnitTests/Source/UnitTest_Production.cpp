/*++

Copyright (C) 2017 Autodesk, Inc.
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

UnitTest_Production.cpp:
Defines Unittests for reading and writing the production extenstion to 3MF.

--*/

#include "UnitTests/UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"

#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"


void investigateMeshObject(NMR::CustomLib3MFBase& pModel, NMR::CustomLib3MFBase& pObjectResource) {
	using namespace NMR;
	std::string sNameBuffer = "";
	ULONG cbNeededChars = 0;

	ASSERT_EQ(lib3mf_object_getpartnumberutf8(pObjectResource.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of part number string";
	sNameBuffer.resize(cbNeededChars);
	ASSERT_EQ(lib3mf_object_getpartnumberutf8(pObjectResource.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get part number";

	ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of part name string";
	sNameBuffer.resize(cbNeededChars);
	ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get part name";

	std::string uuid; uuid.resize(37);
	BOOL bHasUUID;
	EXPECT_EQ(lib3mf_object_getuuidutf8(pObjectResource.get(), &bHasUUID, &uuid[0]), S_OK) << L"Could not get mesh object's uuid.";
	EXPECT_TRUE(bHasUUID) << "Mesh object does not have a UUID";

	DWORD nSliceStackId;
	ASSERT_EQ(lib3mf_meshobject_getslicestackid(pObjectResource.get(), &nSliceStackId), S_OK);
	if (nSliceStackId > 0) {
		NMR::CustomLib3MFBase sliceStack;
		ASSERT_EQ(lib3mf_model_getslicestackById(pModel.get(), nSliceStackId, &sliceStack.get()), S_OK) << L"Could not get slicestack.";
		DWORD nSliceCount;
		ASSERT_EQ(lib3mf_slicestack_getslicecount(sliceStack.get(), &nSliceCount), S_OK) << L"Could not get slice count.";
		EXPECT_TRUE(nSliceCount > 0) << L"Empty slice stack.";

		if (nSliceCount > 0) {
			DWORD nVertCount;
			DWORD nPolyCount;
			NMR::CustomLib3MFBase slice;
			ASSERT_EQ(lib3mf_slicestack_getslice(sliceStack.get(), 0, &slice.get()), S_OK) << L"Could not get slice.";
			ASSERT_EQ(lib3mf_slice_getvertexcount(slice.get(), &nVertCount), S_OK) << L"Could not get slice vertex count.";
			if (nVertCount > 0) {
				std::vector<NMR::MODELSLICEVERTEX> vctVertices(nVertCount);
				ASSERT_EQ(lib3mf_slice_getvertices(slice.get(), &vctVertices[0], nVertCount), S_OK) << L"Could not get slice vertex count.";
			}
			ASSERT_EQ(lib3mf_slice_getpolygoncount(slice.get(), &nPolyCount), S_OK) << L"Could not get slice polygon count.";
			for (DWORD iPolyIndex = 0; iPolyIndex < nPolyCount; iPolyIndex++) {
				DWORD nIndexCount;
				ASSERT_EQ(lib3mf_slice_getpolygonindexcount(slice.get(), iPolyIndex, &nIndexCount), S_OK) << L"Could not get polygon index count.";
				if (nIndexCount > 0) {
					std::vector<DWORD> vctPolyIndices(nIndexCount);
					ASSERT_EQ(lib3mf_slice_getpolygonindices(slice.get(), iPolyIndex, &vctPolyIndices[0], nIndexCount), S_OK) << L"Could not get polygon indices.";
				}
			}
			
		}
	}
}

void investigateComponentsObject(NMR::CustomLib3MFBase& pModel, NMR::CustomLib3MFBase& pObjectResource) {
	using namespace NMR;
	// Retrieve Component
	DWORD nComponentCount;
	DWORD nIndex;
	EXPECT_EQ(lib3mf_componentsobject_getcomponentcount(pObjectResource.get(), &nComponentCount), S_OK) << L"Could not get components count.";
	
	std::string uuid; uuid.resize(37);
	BOOL bHasUUID;
	EXPECT_EQ(lib3mf_object_getuuidutf8(pObjectResource.get(), &bHasUUID, &uuid[0]), S_OK) << L"Could not get componentsobject's uuid.";
	EXPECT_TRUE(bHasUUID) << "Componentsobject does not have a UUID";

	for (nIndex = 0; nIndex < nComponentCount; nIndex++) {
		NMR::CustomLib3MFBase pComponent;
		EXPECT_EQ(lib3mf_componentsobject_getcomponent(pObjectResource.get(), nIndex, &pComponent.get()), S_OK) << L"Could not get component.";

		NMR::CustomLib3MFBase pResource;
		EXPECT_EQ(lib3mf_component_getobjectresource(pComponent.get(), &pResource.get()), S_OK) << L"Could not get component resource.";

		BOOL bIsMeshObject;
		EXPECT_EQ(NMR::lib3mf_object_ismeshobject(pResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether component object is mesh object";
		if (bIsMeshObject) {
			investigateMeshObject(pModel, pResource);
		}
		BOOL bIsComponentObject;
		EXPECT_EQ(NMR::lib3mf_object_iscomponentsobject(pResource.get(), &bIsComponentObject), S_OK) << L"Could not determine whether component object is components object";
		if (bIsComponentObject) {
			investigateComponentsObject(pModel, pResource);
		}
	}
}


bool LoadFile(const char* fileName) {
	using namespace NMR;
	CustomLib3MFBase pModel;
	EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
	CustomLib3MFBase p3MFReader;
	EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
	HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
		(sTestFilesPath + separator() + "Production" + separator() + fileName).c_str());
	DWORD nWarningCount;
	lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount);
	EXPECT_EQ(nWarningCount, 0) << L"Warnings in the 3MF file : " << nWarningCount;
	for (DWORD iWarning=0; iWarning<nWarningCount; iWarning++) {
		DWORD nErrorCode;
		wchar_t sWarning[4096];
		EXPECT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, sWarning, 4096, NULL), S_OK) << L"Could not get warning " << iWarning;
		EXPECT_TRUE(true) << iWarning << L": " << nErrorCode << ", " << sWarning;
	}

	DWORD nErrorCode;
	LPCSTR sErrorMessage;
	lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
	EXPECT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << hResult << ", " << nErrorCode << ", " << sErrorMessage;
	
	char sUUID[37];
	BOOL bHasUUID;
	EXPECT_EQ(lib3mf_model_getbuilduuidutf8(pModel.get(), &bHasUUID, sUUID), S_OK) << L"Could not get build uuid.";
	EXPECT_TRUE(bHasUUID) << L"Expected the build to have a UUID";

	// iterate build items
	CustomLib3MFBase pBuildItemIterator;
	EXPECT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

	BOOL bHasNext;
	EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
	while (bHasNext) {
		CustomLib3MFBase pBuildItem;
		EXPECT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

		CustomLib3MFBase pObjectResource;
		EXPECT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Ccould not get build item resource";

		BOOL bIsMeshObject;
		EXPECT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";
		if (bIsMeshObject) {
			investigateMeshObject(pModel, pObjectResource);
		}
		BOOL bIsComponentObject;
		EXPECT_EQ(NMR::lib3mf_object_iscomponentsobject(pObjectResource.get(), &bIsComponentObject), S_OK) << L"Could not determine whether build item is components object";
		if (bIsComponentObject) {
			investigateComponentsObject(pModel, pObjectResource);
		}

		EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
	}

	// Re-Output file
	CustomLib3MFBase p3MFWriter;
	std::string sFileName = fileName;
	EXPECT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
	EXPECT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + +"ReOut_" + fileName).c_str()), S_OK) << "Cannot write 3mf";

	return true;
}

// A new one of these is create for each test
class Production_LoadFileTest : public testing::TestWithParam<const char*>
{
public:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(InstantiationName,
	Production_LoadFileTest,
	::testing::Values("Replaces_PP_701_04.3mf", "2ProductionBoxes_OneSliceFile.3mf"));

TEST_P(Production_LoadFileTest, ReadFiles)
{
	ASSERT_TRUE(LoadFile(GetParam()));
}



void investigateMeshObject(NMR::CustomLib3MFBase& pModel, NMR::CustomLib3MFBase& pObjectResource, int nExpVerts, int nExpTriangles, int nExpSlices, int bExpUsesSliceRef) {
	using namespace NMR;
	DWORD nVerts;
	ASSERT_EQ(lib3mf_meshobject_getvertexcount(pObjectResource.get(), &nVerts), S_OK) << L"Could not get number of vertices.";
	ASSERT_EQ(nVerts, nExpVerts) << "Incorrect number of vertices";

	DWORD nTriangles;
	ASSERT_EQ(lib3mf_meshobject_gettrianglecount(pObjectResource.get(), &nTriangles), S_OK) << L"Could not get number of triangles.";
	ASSERT_EQ(nTriangles, nExpTriangles) << "Incorrect number of triangles";

	DWORD nSliceStackId;
	ASSERT_EQ(lib3mf_meshobject_getslicestackid(pObjectResource.get(), &nSliceStackId), S_OK) << L"Could not get slicestack id.";
	if (nExpSlices > 0) {
		ASSERT_NE(nSliceStackId, 0) << L"expected slices in this mesh";
		NMR::CustomLib3MFBase sliceStack;
		ASSERT_EQ(lib3mf_model_getslicestackById(pModel.get(), nSliceStackId, &sliceStack.get()), S_OK) << L"Could not get slicestack.";
		DWORD nSliceCount;
		ASSERT_EQ(lib3mf_slicestack_getslicecount(sliceStack.get(), &nSliceCount), S_OK) << L"Could not get slice count.";
		ASSERT_EQ(nExpSlices, nSliceCount) << L"Incorrect number of slices.";

		BOOL bUsesSliceRef;
		ASSERT_EQ(lib3mf_slicestack_getusessliceref(sliceStack.get(), &bUsesSliceRef), S_OK) << L"Could not get bUsesSliceRef.";
		ASSERT_EQ(bUsesSliceRef, bExpUsesSliceRef) << L"Incorrect bUsesSliceRef.";
	}
	else {
		ASSERT_EQ(nSliceStackId, 0) << L"did not expected slices in this mesh";
	}
}

TEST(Production, Read_2ProductionBoxes_OneSliceFile)
{
	std::string fileName = "2ProductionBoxes_OneSliceFile.3mf";
	using namespace NMR;

	auto VerifyFile = [](const std::string& fileName, CustomLib3MFBase& pModel) {
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		CustomLib3MFBase p3MFReader;
		EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
			fileName.c_str());
		DWORD nWarningCount;
		lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount);
		EXPECT_EQ(nWarningCount, 0) << L"Warnings in the 3MF file : " << nWarningCount;
		for (DWORD iWarning = 0; iWarning<nWarningCount; iWarning++) {
			DWORD nErrorCode;
			wchar_t sWarning[4096];
			EXPECT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, sWarning, 4096, NULL), S_OK) << L"Could not get warning " << iWarning;
			EXPECT_TRUE(false) << iWarning << L": " << nErrorCode << ", " << sWarning;
		}

		DWORD nErrorCode;
		LPCSTR sErrorMessage;
		lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
		EXPECT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << hResult << ", " << nErrorCode << ", " << sErrorMessage;

		// iterate build items
		CustomLib3MFBase pBuildItemIterator;
		EXPECT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

		int nMeshes = 2;
		int iMesh = 0;
		int vctNodes[] = { 8,9 };
		int vctTriangles[] = { 12, 13 };
		int vctSlices[] = { 1250, 625 };
		int vctUsesSliceRef[] = { 0, 1 };

		BOOL bHasNext;
		EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		while (bHasNext) {
			if (iMesh >= nMeshes)
				EXPECT_FALSE(true) << L"too many meshes in file";
			CustomLib3MFBase pBuildItem;
			EXPECT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

			CustomLib3MFBase pObjectResource;
			EXPECT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Ccould not get build item resource";

			BOOL bIsMeshObject;
			EXPECT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";
			if (bIsMeshObject) {
				investigateMeshObject(pModel, pObjectResource, vctNodes[iMesh], vctTriangles[iMesh], vctSlices[iMesh], vctUsesSliceRef[iMesh]);
			}

			BOOL bIsComponentObject;
			EXPECT_EQ(NMR::lib3mf_object_iscomponentsobject(pObjectResource.get(), &bIsComponentObject), S_OK) << L"Could not determine whether build item is components object";
			if (bIsComponentObject) {
				investigateComponentsObject(pModel, pObjectResource);
			}


			EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
			iMesh++;
		}
	};

	{
		CustomLib3MFBase pModel;
		VerifyFile(sTestFilesPath + separator() + "Production" + separator() + fileName, pModel);
		{
			CustomLib3MFBase p3MFWriter;
			ASSERT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
			ASSERT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + fileName).c_str()), S_OK) << "Cannot write 3mf";
		}
	}

	{
		CustomLib3MFBase pModel;
		VerifyFile(std::string("TestOutput") + separator() + fileName, pModel);
		{
			CustomLib3MFBase p3MFWriter;
			ASSERT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
			ASSERT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + +"ReOut_" + fileName).c_str()), S_OK) << "Cannot write 3mf";
		}
	}
	

}


TEST(Production, Read_Positive_StillBroken)
{
	//std::string fileName = std::string("D:/CodeArchive/netfabb-testing/Data/FileFormats/3D Files/3MF/3MFReference_Microsoft/") +
	//	"MUSTPASS_Chapter5.1c_MaterialResources_sRGB_RGB_Colors.3mf";
	std::string fileName = sTestFilesPath + separator()+ "Production" + separator() + "2ProductionBoxes_OneSliceFile.3mf";
	using namespace NMR;

	auto VerifyFile = [](const std::string& fileName, CustomLib3MFBase& pModel) {
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		CustomLib3MFBase p3MFReader;
		EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
			fileName.c_str());
		DWORD nWarningCount;
		lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount);
		EXPECT_EQ(nWarningCount, 0) << L"Warnings in the 3MF file : " << nWarningCount;
		for (DWORD iWarning = 0; iWarning<nWarningCount; iWarning++) {
			DWORD nErrorCode;
			wchar_t sWarning[4096];
			EXPECT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, sWarning, 4096, NULL), S_OK) << L"Could not get warning " << iWarning;
			EXPECT_TRUE(false) << iWarning << L": " << nErrorCode << ", " << sWarning;
		}

		DWORD nErrorCode;
		LPCSTR sErrorMessage;
		lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
		EXPECT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << hResult << ", " << nErrorCode << ", " << sErrorMessage;

		// iterate build items
		CustomLib3MFBase pBuildItemIterator;
		EXPECT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

		BOOL bHasNext;
		EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		while (bHasNext) {
			CustomLib3MFBase pBuildItem;
			EXPECT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

			CustomLib3MFBase pObjectResource;
			EXPECT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Ccould not get build item resource";

			BOOL bIsMeshObject;
			EXPECT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";

			if (bIsMeshObject) {
				investigateMeshObject(pModel, pObjectResource);
			}

			EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		}

	};
	{
		CustomLib3MFBase pModel;
		VerifyFile(fileName, pModel);
	}
}

TEST(Production, Read_Negative_StillWorking)
{
	//std::string fileName = std::string("D:/CodeArchive/lib3MF_integration/Data/Negatives/Anakin_HP/") +
	//	"NC_416_01.3mf";
	std::string fileName = sTestFilesPath + separator()+ "Production" + separator() + "2ProductionBoxes_OneSliceFile.3mf";

	using namespace NMR;

	auto VerifyFile = [](const std::string& fileName, CustomLib3MFBase& pModel) {
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		CustomLib3MFBase p3MFReader;
		EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
			fileName.c_str());
		DWORD nWarningCount;
		DWORD nErrorCode;
		if (hResult != LIB3MF_OK) {
			LPCSTR sErrorMessage;
			lib3mf_getlasterror(p3MFReader.get(), &nErrorCode, &sErrorMessage);
			EXPECT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << hResult << ", " << nErrorCode << ", " << sErrorMessage;
		}

		lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount);
		EXPECT_EQ(nWarningCount, 0) << L"Warnings in the 3MF file : " << nWarningCount;
		for (DWORD iWarning = 0; iWarning<nWarningCount; iWarning++) {
			DWORD nErrorCode;
			wchar_t sWarning[4096];
			EXPECT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, sWarning, 4096, NULL), S_OK) << L"Could not get warning " << iWarning;
			EXPECT_TRUE(false) << iWarning << L": " << nErrorCode << ", " << sWarning;
		}

		// iterate build items
		CustomLib3MFBase pBuildItemIterator;
		EXPECT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

		BOOL bHasNext;
		EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		while (bHasNext) {
			CustomLib3MFBase pBuildItem;
			EXPECT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

			CustomLib3MFBase pObjectResource;
			EXPECT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Ccould not get build item resource";

			BOOL bIsMeshObject;
			EXPECT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";

			if (bIsMeshObject) {
				investigateMeshObject(pModel, pObjectResource);
			}

			EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		}

	};
	{
		CustomLib3MFBase pModel;
		VerifyFile(fileName, pModel);
		{
			CustomLib3MFBase p3MFWriter;
			ASSERT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
			ASSERT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + "tmp.3mf").c_str()), S_OK) << "Cannot write 3mf";
		}
	}
}

TEST(Production, ProductionUUIDs)
{
	using namespace NMR;
	CustomLib3MFBase pModel;
	ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
	std::string buildUUID = "125bd689-aadc-478c-8ab8-c15d43294992";
	EXPECT_EQ(lib3mf_model_setbuilduuidutf8(pModel.get(), buildUUID.c_str()), S_OK) << L"Could not set build uuid.";

	BOOL bHasUUID;
	std::string testUUID; testUUID.resize(36);
	EXPECT_EQ(lib3mf_model_getbuilduuidutf8(pModel.get(), &bHasUUID, &testUUID[0]), S_OK) << L"Could not get build uuid.";
	EXPECT_TRUE(bHasUUID) << L"Expected to get a UUID";
	EXPECT_TRUE(testUUID == buildUUID) << L"Expected to get equal UUID";

	CustomLib3MFBase pMeshObject;
	ASSERT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << L"Could not add mesh object";

	CustomLib3MFBase pComponentsObject;
	ASSERT_EQ(lib3mf_model_addcomponentsobject(pModel.get(), &pComponentsObject.get()), S_OK) << L"Could not add components object";
	std::string componentsUUID = "e647d36f-7c46-4d45-adbd-7a9aa2e83b0c";
	EXPECT_EQ(lib3mf_object_setuuidutf8(pComponentsObject.get(), componentsUUID.c_str()), S_OK) << L"Could not set components-object uuid.";
	EXPECT_EQ(lib3mf_object_getuuidutf8(pComponentsObject.get(), &bHasUUID, &testUUID[0]), S_OK) << L"Could not get components-object uuid.";
	EXPECT_TRUE(bHasUUID) << L"Expected to get a UUID";
	EXPECT_TRUE(testUUID == componentsUUID) << L"Expected to get equal UUID";

	CustomLib3MFBase pComponent;
	ASSERT_EQ(lib3mf_componentsobject_addcomponent(pComponentsObject.get(), pMeshObject.get(), nullptr, &pComponent.get()), S_OK) << L"Could not add component";
	std::string componentUUID = "a9a92712-3b0b-4dea-8fcd-51e3cbd6182b";
	EXPECT_EQ(lib3mf_component_setuuidutf8(pComponent.get(), componentUUID.c_str()), S_OK) << L"Could not set component uuid.";
	EXPECT_EQ(lib3mf_component_getuuidutf8(pComponent.get(), &bHasUUID, &testUUID[0]), S_OK) << L"Could not get component uuid.";
	EXPECT_TRUE(bHasUUID) << L"Expected to get a UUID";
	EXPECT_TRUE(testUUID == componentUUID) << L"Expected to get equal UUID";

	std::string meshUUID = "dd36dc20-9aef-4E06-b6063e5aeb31eebc}}";
	EXPECT_EQ(lib3mf_object_setuuidutf8(pMeshObject.get(), meshUUID.c_str()), S_OK) << L"Could not set mesh-object uuid.";
	meshUUID = "dd36dc20-9aef-4e06-b606-3e5aeb31eebc";
	EXPECT_EQ(lib3mf_object_getuuidutf8(pMeshObject.get(), &bHasUUID, &testUUID[0]), S_OK) << L"Could not get mesh-object uuid.";
	EXPECT_TRUE(bHasUUID) << L"Expected to get a UUID";
	EXPECT_TRUE(testUUID == meshUUID) << L"Expected to get equal UUID";

	CustomLib3MFBase pBuildItem;
	ASSERT_EQ(lib3mf_model_addbuilditem(pModel.get(), pMeshObject.get(), NULL, &pBuildItem.get()), S_OK) << L"Could not add build item";
	EXPECT_NE(lib3mf_builditem_setuuidutf8(pBuildItem.get(), buildUUID.c_str()), S_OK) << L"Could set same UUID for build item.";
	std::string buildItemUUID = "85e6a766-broken-4e1d-bf6e-1fc581d6a811";
	EXPECT_NE(lib3mf_builditem_setuuidutf8(pBuildItem.get(), buildItemUUID.c_str()), S_OK) << L"Could set broken UUID for build item.";
	buildItemUUID = "85e6a766--4e1d-bf6e-1fc581d6a811";
	EXPECT_NE(lib3mf_builditem_setuuidutf8(pBuildItem.get(), buildItemUUID.c_str()), S_OK) << L"Could set broken UUID for build item.";
	buildItemUUID = "{26262636-C50C-4A0E-8C12-D93A3B3B41A2}";
	EXPECT_EQ(lib3mf_builditem_setuuidutf8(pBuildItem.get(), buildItemUUID.c_str()), S_OK) << L"Could not set UUID for build item.";
	buildItemUUID = "26262636-c50c-4a0e-8c12-d93a3b3b41a2";
	EXPECT_EQ(lib3mf_builditem_getuuidutf8(pBuildItem.get(), &bHasUUID, &testUUID[0]), S_OK) << L"Could not get build item uuid.";
	EXPECT_TRUE(bHasUUID) << L"Expected to get a UUID";
	EXPECT_TRUE(testUUID == buildItemUUID) << L"Expected to get equal UUID";


}

