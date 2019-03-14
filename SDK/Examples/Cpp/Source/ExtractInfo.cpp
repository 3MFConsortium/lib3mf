/*++

Copyright (C) 2019 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MICROSOFT AND/OR NETFABB BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

ExtractInfo.cpp : 3MF Read Example

--*/

#include <iostream>
#include <string>
#include <algorithm>

#include "lib3mf.hpp"

using namespace Lib3MF;


void printVersion() {
	Lib3MF_uint32 nMajor, nMinor, nMicro;
	std::string sReleaseInfo, sBuildInfo;
	CLib3MFWrapper::GetLibraryVersion(nMajor, nMinor, nMicro, sReleaseInfo, sBuildInfo);
	std::cout << "Lib3MF version = " << nMajor << "." << nMinor << "." << nMicro;
	if (!sReleaseInfo.empty()) {
		std::cout << "-" << sReleaseInfo;
	}
	if (!sBuildInfo.empty()) {
		std::cout << "+" << sBuildInfo;
	}
	std::cout << std::endl;
}

void ShowThumbnailInformation(PLib3MFModel model)
{
	/*
	// TODO: this is not yet implemented in Lib3MF
	*/
}


void ShowMetaDataInformation(PLib3MFMetaDataGroup metaDataGroup)
{
	Lib3MF_uint32 nMetaDataCount = metaDataGroup->GetMetaDataCount();

	for (Lib3MF_uint32 iMeta = 0; iMeta < nMetaDataCount; iMeta++) {

		PLib3MFMetaData metaData = metaDataGroup->GetMetaData(iMeta);
		std::string sMetaDataValue = metaData->GetValue();
		std::string sMetaDataName = metaData->GetName();
		std::cout << "Metadatum: " << iMeta << ":" << std::endl;
		std::cout << "Name  = \"" << sMetaDataName << "\"" << std::endl;
		std::cout << "Value = \"" << sMetaDataValue << "\"" << std::endl;
	}
}

void ShowObjectProperties(PLib3MFObject object)
{
	std::cout << "   Name:            \"" << object->GetName() << "\"" << std::endl;
	std::cout << "   PartNumber:      \"" << object->GetPartNumber() << "\"" << std::endl;

	switch (object->GetType()) {
	case eObjectTypeModel:
		std::cout << "   Object type:     model" << std::endl;
		break;
	case eObjectTypeSupport:
		std::cout << "   Object type:     support" << std::endl;
		break;
	case eObjectTypeSolidSupport:
		std::cout << "   Object type:     solidsupport" << std::endl;
		break;
	case eObjectTypeOther:
		std::cout << "   Object type:     other" << std::endl;
		break;
	default:
		std::cout << "   Object type:     invalid" << std::endl;
		break;
	}
}

void ExtractInfoExample(std::string sFileName) {
	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Read example" << std::endl;
	printVersion();
	std::cout << "------------------------------------------------------------------" << std::endl;

	PLib3MFModel model = CLib3MFWrapper::CreateModel();

	// Import Model from 3MF File
	{
		PLib3MFReader reader = model->QueryReader("3mf");
		// And deactivate the strict mode (default is "false", anyway. This just demonstrates where/how to use it).
		reader->SetStrictModeActive(false);
		reader->ReadFromFile(sFileName);

		for (Lib3MF_uint32 iWarning = 0; iWarning < reader->GetWarningCount(); iWarning++) {
			Lib3MF_uint32 nErrorCode;
			std::string sWarningMessage = reader->GetWarning(iWarning, nErrorCode);
			std::cout << "Encountered warning #" << nErrorCode << " : " << sWarningMessage << std::endl;
		}
	}
	ShowThumbnailInformation(model);

	ShowMetaDataInformation(model->GetMetaDataGroup());

	PLib3MFObjectIterator objectIterator = model->GetObjects();
	while (objectIterator->MoveNext()) {

		PLib3MFObject object = objectIterator->GetCurrentObject();
		if (object->IsMeshObject()) {
			std::cout << "components object #" << object->GetResourceID() << ": " << std::endl;
			ShowMeshObjectInformation(model->GetMeshObjectByID(object->GetResourceID()));
		}
		else if (object->IsComponentsObject()) {
			std::cout << "components object #" << object->GetResourceID() << ": " << std::endl;
			ShowComponentsObjectInformation(model->GetComponentsObjectByID(object->GetResourceID()));
		}
		else {
			std::cout << "unknown object #" << object->GetResourceID() << ": " << std::endl;
		}
	}

	std::cout << "done" << std::endl;
}


int main(int argc, char** argv) {

	// Parse Arguments
	if (argc != 2) {
		std::cout << "Usage: " << std::endl;
		std::cout << "ExtractInfo.exe model.3mf" << std::endl;
		return -1;
	}
	try {
		ExtractInfoExample(argv[1]);
	}
	catch (ELib3MFException &e) {
		std::cout << e.what() << std::endl;
		return e.getErrorCode();
	}
	return 0;
}



void ShowMeshObjectInformation(PLib3MFMeshObject meshObject)
{
	std::cout << "mesh object #" << meshObject->GetResourceID() << ": " << std::endl;

	ShowObjectProperties(meshObject);

	Lib3MF_uint64 nVertexCount = meshObject->GetVertexCount();
	Lib3MF_uint64 nTriangleCount = meshObject->GetTriangleCount();
	PLib3MFBeamLattice beamLattice = meshObject->BeamLattice();

	//DWORD nSliceStackId;
	//hResult = lib3mf_meshobject_getslicestackid(pMeshObject, &nSliceStackId);
	//if (hResult != LIB3MF_OK)
	//	return hResult;
	//DWORD nTotalPolygons = 0;
	//if (nSliceStackId > 0) {
	//	PLib3MFSliceStack *pSliceStack;
	//	hResult = lib3mf_model_getslicestackById(pModel, nSliceStackId, &pSliceStack);
	//	if (hResult != LIB3MF_OK)
	//		return hResult;
	//	hResult = lib3mf_slicestack_getslicecount(pSliceStack, &nSliceCount);
	//	if (hResult != LIB3MF_OK)
	//		return hResult;
	//	for (DWORD iSlice = 0; iSlice < nSliceCount; iSlice++) {
	//		PLib3MFSlice *pSlice;
	//		hResult = lib3mf_slicestack_getslice(pSliceStack, iSlice, &pSlice);
	//		if (hResult != LIB3MF_OK)
	//			return hResult;
	//		DWORD nPolygonCount;
	//		hResult = lib3mf_slice_getpolygoncount(pSlice, &nPolygonCount);
	//		if (hResult != LIB3MF_OK)
	//			return hResult;
	//		for (DWORD polInd = 0; polInd < nPolygonCount; polInd++) {
	//			DWORD nPolygonIndexCount;
	//			hResult = lib3mf_slice_getpolygonindexcount(pSlice, polInd, &nPolygonIndexCount);
	//			if (hResult != LIB3MF_OK)
	//				return hResult;
	//			nTotalPolygons += nPolygonIndexCount;
	//			std::vector<DWORD> vctPoldIndices(nPolygonIndexCount);
	//			hResult = lib3mf_slice_getpolygonindices(pSlice, polInd, &vctPoldIndices[0], nPolygonIndexCount);
	//			if (hResult != LIB3MF_OK)
	//				return hResult;
	//			/*
	//			if (iSlice == 0) {
	//				DWORD nSliceVertexCount;
	//				hResult = lib3mf_slice_getvertexcount(pSlice, &nSliceVertexCount);
	//				if (hResult != LIB3MF_OK)
	//					return hResult;
	//				std::vector<MODELSLICEVERTEX> vctSliceVertices(nSliceVertexCount);
	//				hResult = lib3mf_slice_getvertices(pSlice, &vctSliceVertices[0], nSliceVertexCount);
	//				if (hResult != LIB3MF_OK)
	//					return hResult;
	//				std::cout << "$$POLYLINE/1,1," << nPolygonIndexCount;
	//				for (DWORD iVert = 0; iVert < nPolygonIndexCount; iVert++) {
	//					std::cout << "," << vctSliceVertices[vctPoldIndices[iVert]].m_fPosition[0] << ","
	//						<< vctSliceVertices[vctPoldIndices[iVert]].m_fPosition[1];
	//				}
	//				std::cout << std::endl;
	//			}
	//			*/
	//		}
	//	}
	//}
	//if (nSliceStackId > 0) {
	//	std::cout << "   Slice count:  " << nSliceCount << "; PolygonCount = " << nTotalPolygons << std::endl;
	//}

	// Output data
	std::cout << "   Vertex count:    " << nVertexCount << std::endl;
	std::cout << "   Triangle count:  " << nTriangleCount << std::endl;

	Lib3MF_uint64 nBeamCount = beamLattice->GetBeamCount();
	if (nBeamCount > 0) {
		std::cout << "   Beam count:  " << nBeamCount << std::endl;
		Lib3MF_uint32 nRepresentationMesh;
		if (beamLattice->GetRepresentation(nRepresentationMesh))
			std::cout << "   |_Representation Mesh ID:  " << nRepresentationMesh << std::endl;
		eLib3MFBeamLatticeClipMode eClipMode;
		Lib3MF_uint32 nClippingMesh;
		beamLattice->GetClipping(eClipMode, nClippingMesh);
		if (eClipMode != eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeNoClipMode)
			std::cout << "   |_Clipping Mesh ID:  " << nClippingMesh << "(mode=" << eClipMode << ")" << std::endl;
		if (beamLattice->GetBeamSetCount() > 0) {
			std::cout << "   |_BeamSet count:  " << beamLattice->GetBeamSetCount() << std::endl;
		}
	}

}

void ShowComponentsObjectInformation(PLib3MFComponentsObject componentsObject)
{
	std::cout << "components object #" << componentsObject->GetResourceID() << ": " << std::endl;

	ShowObjectProperties(componentsObject);

	//// Retrieve Component
	//DWORD nComponentCount;
	//DWORD nIndex;
	//hResult = lib3mf_componentsobject_getcomponentcount(pComponentsObject, &nComponentCount);
	//if (hResult != LIB3MF_OK)
	//	return hResult;

	//std::cout << "   Component count:    " << nComponentCount << std::endl;

	//for (nIndex = 0; nIndex < nComponentCount; nIndex++) {
	//	DWORD nResourceID;
	//	PLib3MFModelComponent * pComponent;
	//	hResult = lib3mf_componentsobject_getcomponent(pComponentsObject, nIndex, &pComponent);
	//	if (hResult != LIB3MF_OK) {
	//		return hResult;
	//	}

	//	hResult = lib3mf_component_getobjectresourceid(pComponent, &nResourceID);
	//	if (hResult != LIB3MF_OK) {
	//		lib3mf_release(pComponent);
	//		return hResult;
	//	}

	//	std::cout << "   Component " << nIndex << ":    Object ID:   " << nResourceID << std::endl;
	//	BOOL bHasTransform;
	//	hResult = lib3mf_component_hastransform(pComponent, &bHasTransform);
	//	if (hResult != LIB3MF_OK) {
	//		lib3mf_release(pComponent);
	//		return hResult;
	//	}

	//	if (bHasTransform) {
	//		MODELTRANSFORM Transform;

	//		// Retrieve Transform
	//		hResult = lib3mf_component_gettransform(pComponent, &Transform);
	//		if (hResult != LIB3MF_OK) {
	//			lib3mf_release(pComponent);
	//			return hResult;
	//		}

	//		std::cout << "                   Transformation:  [ " << Transform.m_fFields[0][0] << " " << Transform.m_fFields[0][1] << " " << Transform.m_fFields[0][2] << " " << Transform.m_fFields[0][3] << " ]" << std::endl;
	//		std::cout << "                                    [ " << Transform.m_fFields[1][0] << " " << Transform.m_fFields[1][1] << " " << Transform.m_fFields[1][2] << " " << Transform.m_fFields[1][3] << " ]" << std::endl;
	//		std::cout << "                                    [ " << Transform.m_fFields[2][0] << " " << Transform.m_fFields[2][1] << " " << Transform.m_fFields[2][2] << " " << Transform.m_fFields[2][3] << " ]" << std::endl;
	//	}
	//	else {
	//		std::cout << "                   Transformation:  none" << std::endl;

	//	}


	//}
}





//int _tmain()
//{
//
//	// Iterate through all the Build items
//	hResult = lib3mf_model_getbuilditems(pModel, &pBuildItemIterator);
//	if (hResult != LIB3MF_OK) {
//		std::cout << "could not get build items: " << std::hex << hResult << std::endl;
//		lib3mf_release(pBuildItemIterator);
//		lib3mf_release(pModel);
//		return -1;
//	}
//
//	hResult = lib3mf_builditemiterator_movenext(pBuildItemIterator, &pbHasNext);
//	if (hResult != LIB3MF_OK) {
//		std::cout << "could not get next build item: " << std::hex << hResult << std::endl;
//		lib3mf_release(pBuildItemIterator);
//		lib3mf_release(pModel);
//		return -1;
//	}
//
//	while (pbHasNext) {
//
//		DWORD ResourceID;
//		MODELTRANSFORM Transform;
//		PLib3MFModelBuildItem * pBuildItem;
//		// Retrieve Build Item
//		hResult = lib3mf_builditemiterator_getcurrent(pBuildItemIterator, &pBuildItem);
//		if (hResult != LIB3MF_OK) {
//			std::cout << "could not get build item: " << std::hex << hResult << std::endl;
//			lib3mf_release(pBuildItemIterator);
//			lib3mf_release(pModel);
//			return -1;
//		}
//
//		// Retrieve Resource
//		PLib3MFModelObjectResource * pObjectResource;
//		hResult = lib3mf_builditem_getobjectresource(pBuildItem, &pObjectResource);
//		if (hResult != LIB3MF_OK) {
//			std::cout << "could not get build item resource: " << std::hex << hResult << std::endl;
//			lib3mf_release(pBuildItem);
//			lib3mf_release(pBuildItemIterator);
//			lib3mf_release(pModel);
//			return -1;
//		}
//
//		// Retrieve Resource ID
//		hResult = lib3mf_resource_getresourceid(pObjectResource, &ResourceID);
//		if (hResult != LIB3MF_OK) {
//			std::cout << "could not get resource id: " << std::hex << hResult << std::endl;
//			lib3mf_release(pObjectResource);
//			lib3mf_release(pBuildItem);
//			lib3mf_release(pBuildItemIterator);
//			lib3mf_release(pModel);
//			return -1;
//		}
//
//		// Release Object Resource ID
//		lib3mf_release(pObjectResource);
//
//		// Output
//		std::cout << "------------------------------------------------------" << std::endl;
//		std::cout << "Build item (Object #" << ResourceID << "): " << std::endl;
//
//		// Check Object Transform
//		BOOL bHasTransform;
//		hResult = lib3mf_builditem_hasobjecttransform(pBuildItem, &bHasTransform);
//		if (hResult != LIB3MF_OK) {
//			lib3mf_release(pBuildItem);
//			lib3mf_release(pBuildItemIterator);
//			lib3mf_release(pModel);
//			std::cout << "could not check object transform: " << std::hex << hResult << std::endl;
//			return -1;
//		}
//
//		if (bHasTransform) {
//			// Retrieve Transform
//			hResult = lib3mf_builditem_getobjecttransform(pBuildItem, &Transform);
//			if (hResult != LIB3MF_OK) {
//				lib3mf_release(pBuildItem);
//				lib3mf_release(pBuildItemIterator);
//				lib3mf_release(pModel);
//				std::cout << "could not get object transform: " << std::hex << hResult << std::endl;
//				return -1;
//			}
//
//			std::cout << "   Transformation:  [ " << Transform.m_fFields[0][0] << " " << Transform.m_fFields[0][1] << " " << Transform.m_fFields[0][2] << " " << Transform.m_fFields[0][3] << " ]" << std::endl;
//			std::cout << "                    [ " << Transform.m_fFields[1][0] << " " << Transform.m_fFields[1][1] << " " << Transform.m_fFields[1][2] << " " << Transform.m_fFields[1][3] << " ]" << std::endl;
//			std::cout << "                    [ " << Transform.m_fFields[2][0] << " " << Transform.m_fFields[2][1] << " " << Transform.m_fFields[2][2] << " " << Transform.m_fFields[2][3] << " ]" << std::endl;
//		}
//		else {
//			std::cout << "   Transformation:  none" << std::endl;
//
//		}
//
//		// Retrieve Mesh Part Number Length
//		std::wstring sPartNumber;
//		DWORD nNeededChars;
//		hResult = lib3mf_builditem_getpartnumber(pBuildItem, NULL, 0, &nNeededChars);
//		if (hResult != LIB3MF_OK) {
//			lib3mf_release(pBuildItem);
//			lib3mf_release(pBuildItemIterator);
//			lib3mf_release(pModel);
//			return hResult;
//		}
//
//		// Retrieve Mesh Name
//		if (nNeededChars > 0) {
//			std::vector<wchar_t> pBuffer;
//			pBuffer.resize(nNeededChars + 1);
//			hResult = lib3mf_builditem_getpartnumber(pBuildItem, &pBuffer[0], nNeededChars + 1, NULL);
//			pBuffer[nNeededChars] = 0;
//			sPartNumber = std::wstring(&pBuffer[0]);
//		}
//
//		// Output Part number in local codepage
//		std::string sAnsiPartNumber(sPartNumber.begin(), sPartNumber.end());
//		std::cout << "   Part number:     \"" << sAnsiPartNumber << "\"" << std::endl;
//
//		// Release Build Item
//		lib3mf_release(pBuildItem);
//
//		// Move to next Item
//		hResult = lib3mf_builditemiterator_movenext(pBuildItemIterator, &pbHasNext);
//		if (hResult != LIB3MF_OK) {
//			std::cout << "could not get next build item: " << std::hex << hResult << std::endl;
//			return -1;
//		}
//	}
//
//	std::cout << "------------------------------------------------------" << std::endl;
//
//	// Release Build Item Iterator
//	lib3mf_release(pBuildItemIterator);
//
//	// Release Model
//	lib3mf_release(pModel);
//
//
//	std::cout << "done" << std::endl;
//
//
//	return 0;
//}

