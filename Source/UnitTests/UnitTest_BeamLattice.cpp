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

UnitTest_ReadWrite.cpp: Defines Unittests for reading and writing of 3MFs

--*/

#include "UnitTests/UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"

#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"


void Box_Simple()
{
	using namespace NMR;
	
	CustomLib3MFBase pModel;
	ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
	{
		CustomLib3MFBase p3MFReader;
		ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
			(sTestFilesPath + separator() + "BeamLattice" + separator() + "Box_Simple.3mf").c_str());
		DWORD nErrorCode;
		LPCSTR sErrorMessage;
		lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
		ASSERT_EQ(hResult,	S_OK) << L"Could not read 3MF file.";
	}

	CustomLib3MFBase pBuildItemIterator;
	ASSERT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

	// iterate build items
	BOOL bHasNext;
	ASSERT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
	while (bHasNext) {
		CustomLib3MFBase pBuildItem;
		ASSERT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

		CustomLib3MFBase pObjectResource;
		ASSERT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Ccould not get build item resource";

		BOOL bIsMeshObject;
		ASSERT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";

		EXPECT_TRUE(bIsMeshObject) << L"Object is not a mesh!";

		// check mesh metadata
		{
			std::string sNameBuffer = "";
			ULONG cbNeededChars = 0;
			ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of part name string";
			sNameBuffer.resize(cbNeededChars);
			ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get part name";
			EXPECT_TRUE(sNameBuffer == "Box") << L"Part names do not match.";
		}

		// check geometry and connectivity
		DWORD nVertexCount, nTriangleCount, nBeamCount;
		ASSERT_EQ(ObtainMeshObjectInformation(pObjectResource.get(),
			nVertexCount, nTriangleCount, nBeamCount),
			S_OK) << L"Could not obtain mesh object information";

		ASSERT_TRUE(nVertexCount == 8) << L"Vertex count does not match.";

		ASSERT_EQ(nBeamCount, (DWORD)(12)) << L"Incorrect beam count";
		MODELMESHBEAM beam;
		for (DWORD iBeam = 0; iBeam < nBeamCount; iBeam++) {
			ASSERT_EQ(lib3mf_meshobject_getbeam(pObjectResource.get(), iBeam, &beam), S_OK) << L"Could not get beam";
		}

		ASSERT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
	}
}

void ReadAttributes_Positive()
{
	using namespace NMR;

	CustomLib3MFBase pModel;
	ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
	{
		CustomLib3MFBase p3MFReader;
		ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
			(sTestFilesPath + separator() + "BeamLattice" + separator() + "Box_Attributes_Positive.3mf").c_str());
		DWORD nErrorCode;
		LPCSTR sErrorMessage;
		lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
		ASSERT_EQ(hResult, S_OK) << L"Could not read 3MF file.";
		DWORD nWarningCount;
		ASSERT_EQ(lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount), S_OK) << L"Could not get warning count";
		ASSERT_EQ(nWarningCount, (DWORD)(0)) << L"Warnings encountered.";
	}

	CustomLib3MFBase pResourceIterator;
	ASSERT_EQ(lib3mf_model_getmeshobjects(pModel.get(), &pResourceIterator.get()), S_OK) << L"Could not get meshobject iterator.";;

	// iterate build items
	BOOL bHasNext;
	ASSERT_EQ(lib3mf_resourceiterator_movenext(pResourceIterator.get(), &bHasNext), S_OK) << L"Could not get next meshobject.";

	std::vector<eModelBeamLatticeClipMode> vClipModes;
	vClipModes.push_back(eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE);
	vClipModes.push_back(eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_OUTSIDE);
	vClipModes.push_back(eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_INSIDE);

	std::vector<DWORD> vClipResourceIDs;
	vClipResourceIDs.push_back(0);
	vClipResourceIDs.push_back(2);
	vClipResourceIDs.push_back(1);

	int nIndex = 0;
	while (bHasNext) {
		CustomLib3MFBase pModelResource;
		ASSERT_EQ(lib3mf_resourceiterator_getcurrent(pResourceIterator.get(), &pModelResource.get()), S_OK) << L"Could not retrieve meshobject.";

		BOOL bIsMeshObject;
		ASSERT_EQ(NMR::lib3mf_object_ismeshobject(pModelResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";

		EXPECT_TRUE(bIsMeshObject) << L"Object is not a mesh!";

		eModelBeamLatticeClipMode eClipMode;
		DWORD nResourceID;
		ASSERT_EQ(lib3mf_meshobject_getbeamlattice_clipping(pModelResource.get(), &eClipMode, &nResourceID), S_OK) << L"Could not get clipping of beam lattice";
		ASSERT_EQ(vClipModes[nIndex], eClipMode) << L"Clip mode does not match";
		if (eClipMode != eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE) {
			// ASSERT_EQ(vClipResourceIDs[nIndex], nResourceID) << L"Clipping Resource ID does not match";
			ASSERT_TRUE( (vClipResourceIDs[nIndex] == 0 || nResourceID !=0) ) << L"Clipping Resource ID does not match";
		}

		ASSERT_EQ(lib3mf_resourceiterator_movenext(pResourceIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		nIndex++;
	}
}

void ReadAttributes_Negative(std::string fName)
{
	using namespace NMR;

	CustomLib3MFBase pModel;
	ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
	
	// Handling if broken clippingmesh would be an error
	//{
	//	CustomLib3MFBase p3MFReader;
	//	ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
	//	HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
	//		(sTestFilesPath + separator() + "BeamLattice" + separator() + "Box_Attributes_Negative.3mf").c_str());
	//	DWORD nErrorCode;
	//	LPCSTR sErrorMessage;
	//	ASSERT_EQ(lib3mf_getlasterror(p3MFReader.get(), &nErrorCode, &sErrorMessage), S_OK) << L"Could not get error";
	//	ASSERT_NE(hResult, S_OK) << L"Could read 3MF file which actually should have failed";
	//}
	{
		CustomLib3MFBase p3MFReader;
		ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
			(sTestFilesPath + separator() + "BeamLattice" + separator() + fName).c_str());
		ASSERT_EQ(hResult, S_OK) << L"Could read 3MF file.";
		DWORD nWarningCount;
		ASSERT_EQ(lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount), S_OK) << L"Could not get warning count";
		ASSERT_EQ(nWarningCount, (DWORD)(1)) << L"Different number of warnings occured.";
		DWORD nErrorCode;
		wchar_t sWarning[2048];
		ASSERT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), 0, &nErrorCode, sWarning, 2048, NULL), S_OK) << L"Could not get warning";
	}

	CustomLib3MFBase pResourceIterator;
	ASSERT_EQ(lib3mf_model_getmeshobjects(pModel.get(), &pResourceIterator.get()), S_OK) << L"Could not get meshobject iterator.";;

	// iterate build items
	BOOL bHasNext;
	ASSERT_EQ(lib3mf_resourceiterator_movenext(pResourceIterator.get(), &bHasNext), S_OK) << L"Could not get next meshobject.";


	std::vector<eModelBeamLatticeClipMode> vClipModes;
	std::vector<DWORD> vClipResourceIDs;
	std::vector<DWORD> vRepresentationResourceIDs;
	if (fName == "Box_Attributes_Negative_1.3mf") {
		vClipModes.push_back(eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE);
		vClipModes.push_back(eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE);
		vClipResourceIDs.push_back(0);
		vClipResourceIDs.push_back(0);
		vRepresentationResourceIDs.push_back(0);
		vRepresentationResourceIDs.push_back(0);
	}
	if (fName == "Box_Attributes_Negative_2.3mf") {
		vClipModes.push_back(eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE);
		vClipModes.push_back(eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_OUTSIDE);
		vClipModes.push_back(eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_INSIDE);
		vClipResourceIDs.push_back(0);
		vClipResourceIDs.push_back(1);
		vClipResourceIDs.push_back(2);
		vRepresentationResourceIDs.push_back(0);
		vRepresentationResourceIDs.push_back(0);
		vRepresentationResourceIDs.push_back(1);
	}

	int nIndex = 0;
	while (bHasNext) {
		CustomLib3MFBase pModelResource;
		ASSERT_EQ(lib3mf_resourceiterator_getcurrent(pResourceIterator.get(), &pModelResource.get()), S_OK) << L"Could not retrieve meshobject.";

		BOOL bIsMeshObject;
		ASSERT_EQ(NMR::lib3mf_object_ismeshobject(pModelResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";

		EXPECT_TRUE(bIsMeshObject) << L"Object is not a mesh!";

		eModelBeamLatticeClipMode eClipMode;
		DWORD nResourceID;
		ASSERT_EQ(lib3mf_meshobject_getbeamlattice_clipping(pModelResource.get(), &eClipMode, &nResourceID), S_OK) << L"Could not get clipping mesh of beam lattice";
		ASSERT_EQ(vClipModes[nIndex], eClipMode) << L"Clip mode does not match";
		if (eClipMode != eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE) {
			ASSERT_EQ(vClipResourceIDs[nIndex], nResourceID) << L"Clipping Resource ID does not match";
		}

		BOOL bHasRepresentation;
		ASSERT_EQ(lib3mf_meshobject_getbeamlattice_representation(pModelResource.get(), &bHasRepresentation, &nResourceID), S_OK) << L"Could not get representation mesh of beam lattice";
		if (bHasRepresentation) {
			ASSERT_EQ(vRepresentationResourceIDs[nIndex], nResourceID) << L"Representation Resource ID does not match";
		}

		ASSERT_EQ(lib3mf_resourceiterator_movenext(pResourceIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		nIndex++;
	}
}


void WriteAttributes_Negative() {
	using namespace NMR;
	CustomLib3MFBase pModel;

	// Create Model Instance
	ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create model";

	CustomLib3MFBase pMeshObject;
	ASSERT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << L"Could not add first mesh object";
	CustomLib3MFBase pMeshObject2;
	ASSERT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObject2.get()), S_OK) << L"Could not add second mesh object";

	DWORD nResourceID, nResourceID2;
	ASSERT_EQ(lib3mf_resource_getresourceid(pMeshObject.get(), &nResourceID), S_OK) << L"Could not get resource Id";
	ASSERT_EQ(lib3mf_resource_getresourceid(pMeshObject2.get(), &nResourceID2), S_OK) << L"Could not get second resource Id";

	ASSERT_NE(lib3mf_meshobject_setbeamlattice_clipping(pMeshObject.get(),
		eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_INSIDE, nResourceID2), S_OK) << L"Could set clipping mode allthough it should not work";

	ASSERT_NE(lib3mf_meshobject_setbeamlattice_clipping(pMeshObject.get(),
		eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_INSIDE, nResourceID + nResourceID2 + 1), S_OK) << L"Could set clipping mode allthough it should not work";

	ASSERT_NE(lib3mf_meshobject_setbeamlattice_representation(pMeshObject.get(), nResourceID2), S_OK) << L"Could set representation mesh allthough it should work";
	
	ASSERT_NE(lib3mf_meshobject_setbeamlattice_representation(pMeshObject.get(), nResourceID + nResourceID2 + 1), S_OK) << L"Could set representation mesh allthough it should not work";
}


void WriteBeamLattice_Negative()
{
	using namespace NMR;
	CustomLib3MFBase pModel;

	// Create Model Instance
	ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create model";

	CustomLib3MFBase pMeshObject;
	ASSERT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << L"Could not add mesh object";

	MODELMESHBEAM beam;
	beam.m_nIndices[0] = 0;
	beam.m_nIndices[1] = 1;
	ASSERT_NE(lib3mf_meshobject_addbeam(pMeshObject.get(), &beam,  NULL), S_OK) << L"Could add beam allthough this should not have worked";

	MODELMESHVERTEX vert;
	vert.m_fPosition[0] = 0;
	vert.m_fPosition[1] = 0;
	vert.m_fPosition[2] = 0;
	ASSERT_EQ(lib3mf_meshobject_addvertex(pMeshObject.get(), &vert, NULL), S_OK) << L"Could not add vertex";
	vert.m_fPosition[0] = 2;
	vert.m_fPosition[1] = 2;
	vert.m_fPosition[2] = 2;
	ASSERT_EQ(lib3mf_meshobject_addvertex(pMeshObject.get(), &vert, NULL), S_OK) << L"Could not add vertex";

	ASSERT_EQ(lib3mf_meshobject_addbeam(pMeshObject.get(), &beam, NULL), S_OK) << L"Could not add beam.";

	ASSERT_EQ(lib3mf_object_settype(pMeshObject.get(), MODELOBJECTTYPE_MODEL), S_OK) << L"Could not set type to MODEL";
	ASSERT_EQ(lib3mf_object_settype(pMeshObject.get(), MODELOBJECTTYPE_SOLIDSUPPORT), S_OK) << L"Could not set type to SOLIDSUPPORT";
	ASSERT_NE(lib3mf_object_settype(pMeshObject.get(), MODELOBJECTTYPE_SUPPORT), S_OK) << L"Could set type to SUPPORT";

	ASSERT_EQ(lib3mf_meshobject_setbeamindices(pMeshObject.get(), NULL, 0), S_OK) << L"Could not remove beams";

	ASSERT_EQ(lib3mf_object_settype(pMeshObject.get(), MODELOBJECTTYPE_SUPPORT), S_OK) << L"Could not set type to SUPPORT";
	ASSERT_NE(lib3mf_meshobject_addbeam(pMeshObject.get(), &beam, NULL), S_OK) << L"Could add beam to model of type support";
}

TEST(BeamLattice, ReadBoxSimple)
{
	Box_Simple();
}

TEST(BeamLattice, ReadAttributes_Positive)
{
	ReadAttributes_Positive();
}

// A new one of these is create for each test
class BeamLattice_ReadNegative : public testing::TestWithParam<const char*>
{
public:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(InstantiationName,
	BeamLattice_ReadNegative,
	::testing::Values("Box_Attributes_Negative_1.3mf",
		"Box_Attributes_Negative_2.3mf"));

TEST_P(BeamLattice_ReadNegative, ReadAttributes_Negative)
{
	ReadAttributes_Negative(GetParam());
}

TEST(BeamLattice, WriteAttributes_Negative)
{
	WriteAttributes_Negative();
}

TEST(BeamLattice, WriteBeamLattice_Negative)
{
	WriteBeamLattice_Negative();
}


