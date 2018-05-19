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

#include "UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"

#include <vector>
#include <string>
#include <cmath>

#include <fstream>
#include <iterator>
#include <algorithm>

#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

namespace NMR {
	
	class DLLInterfacesTester
	{
	private:
		std::vector<MODELMESHVERTEX> m_vertices;
		MODELMESHTRIANGLE m_triangle;
		std::vector<MODELMESHBEAM> m_beams;
		const double m_dBeamLatticeDefaultRadius;
		const double m_dBeamLatticeMinLength;
		const eModelBeamLatticeCapMode m_eCapMode;
		const eModelBeamLatticeClipMode m_eClipMode;
		const std::string m_sBeamSetName;
		const std::string m_sBeamSetId;
		std::vector<DWORD> m_vBeamSetRefs;
		const std::string m_sFolderName;
		const std::string m_sFilenameReadWrite;
		const std::string m_sPartNumber;
		const std::string m_sPartName;
		
		const double EPS_TOL = 1e-5;
	public:
		
		DLLInterfacesTester() :m_vertices(3), m_beams(3),
			m_dBeamLatticeDefaultRadius(1.25), m_dBeamLatticeMinLength(0.000125),
			m_eCapMode(eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_SPHERE), m_eClipMode(eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_INSIDE),
			m_sBeamSetName("BeamSetName"), m_sBeamSetId("BeamSetId"),
			m_sFolderName("TestOutput"), m_sFilenameReadWrite("output_dllwriteread.3mf"),
			m_sPartNumber("PartNumber"), m_sPartName("PartName")
		{
			m_vBeamSetRefs.resize(2);
			m_vBeamSetRefs[0] = 2;
			m_vBeamSetRefs[1] = 0;
			for (size_t i = 0; i<m_vertices.size(); i++) {
				for (size_t j = 0; j < 3; j++)
					m_vertices[i].m_fPosition[j] = ((i + j) == 1)*1.f + ((i + j) == 2)*2.f;
				m_triangle.m_nIndices[i] = (DWORD)i;
			}
			for (size_t i = 0; i < m_beams.size(); i++) {
				m_beams[i].m_dRadius[0] = m_dBeamLatticeDefaultRadius;
				m_beams[i].m_dRadius[1] = m_dBeamLatticeDefaultRadius;
				m_beams[i].m_eCapMode[0] = m_eCapMode;
				m_beams[i].m_eCapMode[1] = m_eCapMode;
			}
			m_beams[0].m_nIndices[0] = 0; m_beams[0].m_nIndices[1] = 1;
			m_beams[1].m_nIndices[0] = 1; m_beams[1].m_nIndices[1] = 2;
			m_beams[2].m_nIndices[0] = 2; m_beams[2].m_nIndices[1] = 0;
			m_beams[0].m_dRadius[0] = 0.1;
			m_beams[0].m_dRadius[1] = 10;
			m_beams[1].m_dRadius[0] = 0.5;
			m_beams[1].m_dRadius[1] = 0.5;
			m_beams[2].m_dRadius[0] = m_dBeamLatticeDefaultRadius;
			m_beams[2].m_dRadius[1] = 2;
			m_beams[0].m_eCapMode[0] = eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_BUTT;
			m_beams[0].m_eCapMode[1] = eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_SPHERE;
			m_beams[1].m_eCapMode[0] = eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_SPHERE;
			m_beams[1].m_eCapMode[1] = eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_SPHERE;
			m_beams[2].m_eCapMode[0] = eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_HEMISPHERE;
			m_beams[2].m_eCapMode[1] = eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_BUTT;
		}

		void Write3MF(bool writeTriangles, bool writeBeams) {
			bool writeVertices = writeTriangles | writeBeams;
			CustomLib3MFBase pModel;

			// Create Model Instance
			ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create model";
			
			CustomLib3MFBase pMeshObject;
			ASSERT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << L"Could not add mesh object";
			
			if (writeVertices) {
				for (size_t i = 0; i < m_vertices.size(); i++)
					ASSERT_EQ(lib3mf_meshobject_addvertex(pMeshObject.get(), &m_vertices[i], NULL), S_OK) << L"Could not add vertex";
			}

			if (writeTriangles) {
				ASSERT_EQ(lib3mf_meshobject_addtriangle(pMeshObject.get(), &m_triangle, NULL), S_OK) << L"Could not add triangle";
			}

			if (writeBeams) {
				ASSERT_EQ(lib3mf_meshobject_setbeamlattice_radius(pMeshObject.get(), m_dBeamLatticeDefaultRadius), S_OK) << L"Could not set default beamlattice radius";
				ASSERT_EQ(lib3mf_meshobject_setbeamlattice_minlength(pMeshObject.get(), m_dBeamLatticeMinLength), S_OK) << L"Could not set beamlattice min length";
				ASSERT_EQ(lib3mf_meshobject_setbeamlattice_capmode(pMeshObject.get(), m_eCapMode), S_OK) << L"Could not set beamlattice capmode";

				for (size_t i = 0; i < m_beams.size(); i++)
					ASSERT_EQ(lib3mf_meshobject_addbeam(pMeshObject.get(), &m_beams[i], NULL), S_OK) << L"Could not add beam";
				CustomLib3MFBase pBeamSet;
				ASSERT_EQ(lib3mf_meshobject_addbeamset(pMeshObject.get(), &pBeamSet.get()), S_OK) << L"Could not add beamset";
				ASSERT_EQ(lib3mf_beamset_setnameutf8(pBeamSet.get(), m_sBeamSetName.c_str()), S_OK) << L"Could not set beamset name";
				ASSERT_EQ(lib3mf_beamset_setidentifierutf8(pBeamSet.get(), m_sBeamSetId.c_str()), S_OK) << L"Could not set beamset id";
				ASSERT_EQ(lib3mf_beamset_setrefs(pBeamSet.get(), m_vBeamSetRefs.data(), (DWORD)(m_vBeamSetRefs.size()) ), S_OK) << L"Could not set beamset refs";

				DWORD nResourceID;	// self reference to this as clipping mesh
				ASSERT_EQ(lib3mf_resource_getresourceid(pMeshObject.get(), &nResourceID), S_OK) << L"Could not get Resource ID";
				ASSERT_EQ(lib3mf_meshobject_setbeamlattice_clipping(pMeshObject.get(), m_eClipMode, nResourceID), S_OK) << L"Could not set Resource ID";
			}

			ASSERT_EQ(lib3mf_object_setpartnumberutf8(pMeshObject.get(), m_sPartNumber.c_str()), S_OK) << L"Could not set part number";

			ASSERT_EQ(lib3mf_object_setnameutf8(pMeshObject.get(), m_sPartName.c_str()), S_OK) << L"Could not set part name";

			CustomLib3MFBase pBuildItem;
			ASSERT_EQ(lib3mf_model_addbuilditem(pModel.get(), pMeshObject.get(), NULL, &pBuildItem.get()), S_OK) << L"Could not add build item";
			ASSERT_EQ(lib3mf_builditem_setpartnumberutf8(pBuildItem.get(), m_sPartNumber.c_str()), S_OK) << L"Could not set build item part number";

			CustomLib3MFBase p3MFWriter;
			ASSERT_EQ(lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << L"Could create Model Writer";
			
			ASSERT_TRUE(CreateDir(m_sFolderName.c_str())) << L"Could not create folder.";
			ASSERT_EQ(lib3mf_writer_writetofileutf8(p3MFWriter.get(),
				(m_sFolderName + separator() + m_sFilenameReadWrite).c_str()),
				S_OK) << L"Could not write 3MF file.";
		}


		void Read3MF (bool readTriangles, bool readBeams){
			HRESULT hResult;
			bool readVertices = readTriangles | readBeams;
			CustomLib3MFBase pModel;
			ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
			{
				CustomLib3MFBase p3MFReader;
				ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
				hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
					(m_sFolderName + separator() + m_sFilenameReadWrite).c_str());
				if (hResult != S_OK) {
					DWORD errorCode;
					LPCSTR errString;
					hResult = lib3mf_getlasterror(p3MFReader.get(), &errorCode, &errString);
					std::string errorMessage = std::string(errString);
				}
				ASSERT_EQ(hResult, S_OK) << L"Could not read 3MF file.";
			}

			CustomLib3MFBase pBuildItemIterator;
			ASSERT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

			// iterate build items
			BOOL bHasNext;
			ASSERT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
			while (bHasNext) {
				CustomLib3MFBase pBuildItem;
				ASSERT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

				std::string sNameBuffer = "";
				ULONG cbNeededChars = 0;
				ASSERT_EQ(lib3mf_builditem_getpartnumberutf8(pBuildItem.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of part number string of build item";
				sNameBuffer.resize(cbNeededChars);
				ASSERT_EQ(lib3mf_builditem_getpartnumberutf8(pBuildItem.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get part number of build item";
				EXPECT_TRUE(sNameBuffer == m_sPartNumber) << L"Part numbers of build item do not match.";

				CustomLib3MFBase pObjectResource;
				ASSERT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Ccould not get build item resource";

				BOOL bIsMeshObject;
				ASSERT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";

				EXPECT_TRUE(bIsMeshObject) << L"Object is not a mesh!";

				// check mesh metadata
				{
					sNameBuffer = "";
					cbNeededChars = 0;
					ASSERT_EQ(lib3mf_object_getpartnumberutf8(pObjectResource.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of part number string";
					sNameBuffer.resize(cbNeededChars);
					ASSERT_EQ(lib3mf_object_getpartnumberutf8(pObjectResource.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get part number";
					EXPECT_TRUE(sNameBuffer == m_sPartNumber) << L"Part numbers do not match.";

					ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of part name string";
					sNameBuffer.resize(cbNeededChars);
					ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get part name";
					EXPECT_TRUE(sNameBuffer == m_sPartName) << L"Part names do not match.";
				}

				// check geometry and connectivity
				DWORD nVertexCount, nTriangleCount, nBeamCount;
				ASSERT_EQ(ObtainMeshObjectInformation(pObjectResource.get(), nVertexCount, nTriangleCount, nBeamCount), S_OK) << L"Could not obtain mesh object information";

				if (readVertices) {
					ASSERT_TRUE(nVertexCount == m_vertices.size()) << L"Vertex count does not match.";
					for (size_t i = 0; i < m_vertices.size(); i++) {
						MODELMESHVERTEX vertex;
						ASSERT_EQ(lib3mf_meshobject_getvertex(pObjectResource.get(), (DWORD)i, &vertex), S_OK) << L"Could not get vertex";
						for (int j = 0; j < 3; j++)
							EXPECT_TRUE(std::fabs(m_vertices[i].m_fPosition[j] - vertex.m_fPosition[j]) < EPS_TOL) << L"Vertex position does not match.";
					}
				}

				if (readTriangles) {
					ASSERT_EQ(nTriangleCount, 1) << L"Incorrect triangle count";
					MODELMESHTRIANGLE triangle;
					ASSERT_EQ(lib3mf_meshobject_gettriangle(pObjectResource.get(), 0, &triangle), S_OK) << L"Could not get triangle";
					for (int j = 0; j < 3; j++)
						EXPECT_TRUE(triangle.m_nIndices[j] == m_triangle.m_nIndices[j]) << L"Triangle indices do not match.";
				}
				if (readBeams) {
					ASSERT_EQ(nBeamCount, m_beams.size()) << L"Incorrect beam count";
					MODELMESHBEAM beam;
					double dRadius, dMinLength;
					eModelBeamLatticeCapMode eCapMode;
					ASSERT_EQ(lib3mf_meshobject_getbeamlattice_radius(pObjectResource.get(), &dRadius), S_OK) << L"Could not get beamlattice default radius";
					ASSERT_TRUE(fabs(dRadius - m_dBeamLatticeDefaultRadius) < EPS_TOL) << L"Default radius is incorrect";
					ASSERT_EQ(lib3mf_meshobject_getbeamlattice_minlength(pObjectResource.get(), &dMinLength), S_OK) << L"Could not get beamlattice precision";
					ASSERT_TRUE(fabs(dMinLength - m_dBeamLatticeMinLength) < EPS_TOL) << L"MinLength is incorrect";
					ASSERT_EQ(lib3mf_meshobject_getbeamlattice_capmode(pObjectResource.get(), &eCapMode), S_OK) << L"Could not get beamlattice capmode";
					ASSERT_TRUE(eCapMode == m_eCapMode) << L"Cap mode is incorrect";

					DWORD nResourceIDSelf, nResourceID;	// self reference to this as clipping mesh
					eModelBeamLatticeClipMode eClipMode;
					ASSERT_EQ(lib3mf_resource_getresourceid(pObjectResource.get(), &nResourceIDSelf), S_OK) << L"Could not get Resource ID";
					ASSERT_EQ(lib3mf_meshobject_getbeamlattice_clipping(pObjectResource.get(), &eClipMode, &nResourceID), S_OK) << L"Could not get Clipping info";
					ASSERT_EQ(eClipMode, m_eClipMode) << L"Clipping mode does not match";
					ASSERT_EQ(nResourceID, nResourceIDSelf) << L"Clipping mode does not match";


					for (DWORD iBeam = 0; iBeam < nBeamCount; iBeam++) {
						ASSERT_EQ(lib3mf_meshobject_getbeam(pObjectResource.get(), iBeam, &beam), S_OK) << L"Could not get beam";
						for (int j = 0; j < 2; j++) {
							EXPECT_TRUE(beam.m_nIndices[j] == m_beams[iBeam].m_nIndices[j]) << L"Beam indices do not match.";
							EXPECT_TRUE(fabs(beam.m_dRadius[j] - m_beams[iBeam].m_dRadius[j])<EPS_TOL) << L"Beam radius does not match.";
							EXPECT_TRUE(beam.m_eCapMode[j] == m_beams[iBeam].m_eCapMode[j]) << L"Beam capmode does not match.";
						}
					}

					DWORD nBeamSetCount;
					ASSERT_EQ(lib3mf_meshobject_getbeamsetcount(pObjectResource.get(), &nBeamSetCount), S_OK) << L"Could not get beamset count";
					ASSERT_EQ(nBeamSetCount, 1) << L"BeamSetCount does not match";
					CustomLib3MFBase pBeamSet;
					ASSERT_EQ(lib3mf_meshobject_getbeamset(pObjectResource.get(), 0, &pBeamSet.get()), S_OK) << L"Could not get beamset";

					std::string sNameBuffer = "";
					ULONG cbNeededChars = 0;
					ASSERT_EQ(lib3mf_beamset_getnameutf8(pBeamSet.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of beamset name";
					sNameBuffer.resize(cbNeededChars);
					ASSERT_EQ(lib3mf_beamset_getnameutf8(pBeamSet.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get name of beamset";
					EXPECT_TRUE(sNameBuffer == m_sBeamSetName) << L"Name of beamset does not match.";

					ASSERT_EQ(lib3mf_beamset_getidentifierutf8(pBeamSet.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of beamset id";
					sNameBuffer.resize(cbNeededChars);
					ASSERT_EQ(lib3mf_beamset_getidentifierutf8(pBeamSet.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get id of beamset";
					EXPECT_TRUE(sNameBuffer == m_sBeamSetId) << L"Id of beamset does not match.";

					std::vector<DWORD> vBeamSetRefs;
					DWORD nRefCount = 0;
					ASSERT_EQ(lib3mf_beamset_getrefcount(pBeamSet.get(), &nRefCount), S_OK) << L"Could not get number of beamset refs";
					vBeamSetRefs.resize(nRefCount);
					ASSERT_EQ(lib3mf_beamset_getrefs(pBeamSet.get(), &(vBeamSetRefs[0]), nRefCount, NULL), S_OK) << L"Could not get beamset refs";
					for (DWORD iRef = 0; iRef < nRefCount; iRef++) {
						ASSERT_EQ(vBeamSetRefs[iRef], m_vBeamSetRefs[iRef]) << L"Beamset ref does not match";
					}

				}
				
				ASSERT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
			}
		}
	};
	

	TEST(ReadWrite, WriteMesh)
	{
		DLLInterfacesTester interfaceTester;
		interfaceTester.Write3MF(true, true);
	}

	TEST(ReadWrite, ReadMesh)
	{
		DLLInterfacesTester interfaceTester;
		interfaceTester.Read3MF(true, true);
	}


	TEST(ReadWrite, ReadBeamLattice)
	{
		CustomLib3MFBase pModel;
		ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		{
			CustomLib3MFBase p3MFReader;
			ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
			HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
				(sTestFilesPath + separator() + "BeamLattice" + separator() + "Box_Simple.3mf").c_str());
			DWORD nErrorCode;
			LPCSTR sErrorMessage;
			lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage );
			ASSERT_EQ(hResult,
				S_OK) << L"Could not read 3MF file.";
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

			ASSERT_EQ(nBeamCount, 12) << L"Incorrect beam count";
			MODELMESHBEAM beam;
			for (DWORD iBeam = 0; iBeam < nBeamCount; iBeam++) {
				ASSERT_EQ(lib3mf_meshobject_getbeam(pObjectResource.get(), iBeam, &beam), S_OK) << L"Could not get beam";
				// std::cout << iBeam << ": v1=" << beam.m_nIndices[0] << " v2=" << beam.m_nIndices[1] << " r1=" << beam.m_fRadius[0] << " r2=" << beam.m_fRadius[1] <<"\n";
			}

			ASSERT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		}
	}

	TEST(ReadWrite, EscapeCharacters)
	{
		std::string fileName = sTestFilesPath + separator() + "Mixed" + separator() + "texturecube_with_EscapeCharacters.3mf";
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

				
				// check mesh metadata
				{
					std::string sNameBuffer = "";
					ULONG cbNeededChars = 0;

					ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of part number string";
					sNameBuffer.resize(cbNeededChars);
					ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get part number";
					EXPECT_TRUE(sNameBuffer == "Colored Box > Black box & boring box \" < '") << L"Part names do not match.";
				}

				BOOL bIsMeshObject;
				EXPECT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";
				EXPECT_TRUE(bIsMeshObject) << L"Could build item is not a mesh object";

				EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
			}

		};
		{
			CustomLib3MFBase pModel;
			VerifyFile(fileName, pModel);
		}
	}



	bool compareFiles(const std::string& p1, const std::string& p2) {
		std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
		std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

		if (f1.fail() || f2.fail()) {
			return false; //file problem
		}

		if (f1.tellg() != f2.tellg()) {
			return false; //size mismatch
		}

		//seek back to beginning and use std::equal to compare contents
		f1.seekg(0, std::ifstream::beg);
		f2.seekg(0, std::ifstream::beg);
		return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
			std::istreambuf_iterator<char>(),
			std::istreambuf_iterator<char>(f2.rdbuf()));
	}

	TEST(ReadWrite, DualWrite) {
		CustomLib3MFBase pModel;
		std::string originalFile = "texturecube_with_EscapeCharacters.3mf";
		std::string outfile1 = std::string("TestOutput") + separator()+"DualWrite_out1.3mf";
		std::string outfile2 = std::string("TestOutput") + separator()+"DualWrite_out2.3mf";
		ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		CustomLib3MFBase p3MFReader;
		ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		ASSERT_EQ(lib3mf_reader_readfromfileutf8(p3MFReader.get(), (sTestFilesPath + separator() + "Mixed" + separator() + originalFile).c_str()), S_OK) << L"Could not read file";
		CustomLib3MFBase p3MFWriter;
		ASSERT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
		ASSERT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), outfile1.c_str()), S_OK) << "Cannot write 3mf";
		ASSERT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), outfile2.c_str()), S_OK) << "Cannot write 3mf";

		ASSERT_TRUE(compareFiles(outfile1, outfile2));
	}

	// A new one of these is create for each test
	class MemoryBufferTest : public testing::TestWithParam<const char*>
	{
	public:
		virtual void SetUp() {}
		virtual void TearDown() {}
	};

	INSTANTIATE_TEST_CASE_P(LoadFromMemoryBuffer, MemoryBufferTest,
							::testing::Values(
								"Box.3mf",
								"Texture_093.3mf",
								"3mfbase5_texture093.3mf",
								"3mfbase14_materialandcolor2.3mf"
							));

	TEST_P(MemoryBufferTest, Load)
	{
		std::string fileName = GetParam();

		// Read the file fully into a memory buffer
		std::ifstream file((sTestFilesPath + separator() + "CPP_UnitTests" + separator() + fileName).c_str(), std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		// Memory buffer
		std::vector<char> buffer(static_cast<size_t>(size));
		EXPECT_TRUE(file.read(buffer.data(), size));

		// Load the 3mf from a buffer, not a file
		using namespace NMR;
		CustomLib3MFBase pModel;
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		CustomLib3MFBase p3MFReader;
		EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";

		HRESULT hResult = lib3mf_reader_readfrombuffer(p3MFReader.get(), (BYTE*)buffer.data(), size);

		DWORD nWarningCount;
		lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount);
		EXPECT_EQ(nWarningCount, 0) << L"Warnings in the 3MF file : " << nWarningCount;
		for (DWORD iWarning = 0; iWarning<nWarningCount; iWarning++) {
			DWORD nErrorCode;
			wchar_t sWarning[4096];
			EXPECT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, sWarning, 4096, NULL), S_OK) << L"Could not get warning " << iWarning;
			EXPECT_TRUE(true) << iWarning << L": " << nErrorCode << ", " << sWarning;
		}
		DWORD nErrorCode;
		LPCSTR sErrorMessage;
		lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
		EXPECT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << hResult << ", " << nErrorCode << ", " << sErrorMessage;
	}

	TEST(FileNamesTest, ReWriteFileNames)
	{
		std::string fileName = u8"Box.3mf";	// simple name to be read on any machine
		CustomLib3MFBase pModel;
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";

		HRESULT hResult;
		DWORD nErrorCode;
		LPCSTR sErrorMessage;

		// Read the model
		{
			CustomLib3MFBase p3MFReader;
			ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could not create Model Reader";

			hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(), (sTestFilesPath + separator() + "Mixed" + separator() + fileName).c_str());
			lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
			ASSERT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << fileName;
		}

		CustomLib3MFBase p3MFWriter;
		ASSERT_EQ(lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << L"Could create Model Writer";
		// Write the the model
		{
			ASSERT_EQ(lib3mf_writer_writetofileutf8(p3MFWriter.get(),
				(std::string("TestOutput") + separator() + fileName).c_str()),
				S_OK) << L"Could not write 3MF file to orignal filename in different path";
		}
		std::string outFileName = u8"outBox_ß人物.3mf";
		// Write the the model to a non-ascii-location
		{
			ASSERT_EQ(lib3mf_writer_writetofileutf8(p3MFWriter.get(),
				(std::string("TestOutput") + separator() + outFileName).c_str()),
				S_OK) << L"Could not write 3MF file to non-ASCII-location";
		}

		// Read the model from the non-ascii-location
		{
			CustomLib3MFBase pModel2;
			EXPECT_EQ(lib3mf_createmodel(&pModel2.get()), S_OK) << L"Could not create 3MF model";

			CustomLib3MFBase p3MFReader;
			ASSERT_EQ(lib3mf_model_queryreader(pModel2.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could not create Model Reader";

			hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(), (std::string("TestOutput") + separator() + outFileName).c_str());
			lib3mf_getlasterror(pModel2.get(), &nErrorCode, &sErrorMessage);
			ASSERT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << outFileName;
		}
	}
}

