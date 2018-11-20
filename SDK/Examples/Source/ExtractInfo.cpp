/*++

Copyright (C) 2018 3MF Consortium

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

#ifndef __GNUC__
#include <tchar.h>
#include <Windows.h>
#endif // __GNUC__

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Plain C Includes of 3MF Library
#include "NMR_DLLInterfaces.h"

// Use NMR namespace for the interfaces
using namespace NMR;


HRESULT ShowObjectProperties(_In_ PLib3MFModelObjectResource * pObject)
{
	HRESULT hResult;
	DWORD nNeededChars;
	std::vector<wchar_t> pBuffer;
	std::wstring sName;
	std::wstring sPartNumber;

	// Retrieve Mesh Name Length
	hResult = lib3mf_object_getname(pObject, NULL, 0, &nNeededChars);
	if (hResult != LIB3MF_OK)
		return hResult;

	// Retrieve Mesh Name
	if (nNeededChars > 0) {
		pBuffer.resize(nNeededChars + 1);
		hResult = lib3mf_object_getname(pObject, &pBuffer[0], nNeededChars + 1, NULL);
		pBuffer[nNeededChars] = 0;
		sName = std::wstring(&pBuffer[0]);
	}

	// Output Name in local codepage
	std::string sAnsiName(sName.begin(), sName.end());
	std::cout << "   Name:            \"" << sAnsiName << "\"" << std::endl;

	// Retrieve Mesh Part Number Length
	hResult = lib3mf_object_getpartnumber(pObject, NULL, 0, &nNeededChars);
	if (hResult != LIB3MF_OK)
		return hResult;

	// Retrieve Mesh Name
	if (nNeededChars > 0) {
		pBuffer.resize(nNeededChars + 1);
		hResult = lib3mf_object_getpartnumber(pObject, &pBuffer[0], nNeededChars + 1, NULL);
		pBuffer[nNeededChars] = 0;
		sPartNumber = std::wstring(&pBuffer[0]);
	}

	// Output Part number in local codepage
	std::string sAnsiPartNumber(sPartNumber.begin(), sPartNumber.end());
	std::cout << "   Part number:     \"" << sAnsiPartNumber << "\"" << std::endl;

	// Output Object type
	DWORD ObjectType;
	hResult = lib3mf_object_gettype(pObject, &ObjectType);
	if (hResult != LIB3MF_OK)
		return hResult;

	switch (ObjectType) {
	case MODELOBJECTTYPE_MODEL:
		std::cout << "   Object type:     model" << std::endl;
		break;
	case MODELOBJECTTYPE_SUPPORT:
		std::cout << "   Object type:     support" << std::endl;
		break;
	case MODELOBJECTTYPE_SOLIDSUPPORT:
		std::cout << "   Object type:     solidsupport" << std::endl;
		break;
	case MODELOBJECTTYPE_OTHER:
		std::cout << "   Object type:     other" << std::endl;
		break;
	default:
		std::cout << "   Object type: invalid" << std::endl;
		break;

	}


	return LIB3MF_OK;
}


HRESULT ShowMeshObjectInformation(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ PLib3MFModel * pModel)
{
	HRESULT hResult;
	DWORD nVertexCount, nTriangleCount, nBeamCount, nSliceCount = 0;
	DWORD nBeamSetCount;
	BOOL bHasRepresentation;
	DWORD nRepresentationMesh, nClippingMesh;
	eModelBeamLatticeClipMode nClipMode;

	hResult = ShowObjectProperties(pMeshObject);
	if (hResult != LIB3MF_OK)
		return hResult;

	// Retrieve Mesh Vertex Count
	hResult = lib3mf_meshobject_getvertexcount(pMeshObject, &nVertexCount);
	if (hResult != LIB3MF_OK)
		return hResult;

	// Retrieve Mesh Triangle Count
	hResult = lib3mf_meshobject_gettrianglecount(pMeshObject, &nTriangleCount);
	if (hResult != LIB3MF_OK)
		return hResult;

	// Retrieve Mesh Beam Count
	hResult = lib3mf_meshobject_getbeamcount(pMeshObject, &nBeamCount);
	if (hResult != LIB3MF_OK)
		return hResult;
	if (nBeamCount > 0) {
		hResult = lib3mf_meshobject_getbeamsetcount(pMeshObject, &nBeamSetCount);
		if (hResult != LIB3MF_OK)
			return hResult;
		hResult = lib3mf_meshobject_getbeamlattice_representation(pMeshObject, &bHasRepresentation, &nRepresentationMesh);
		if (hResult != LIB3MF_OK)
			return hResult;
		hResult = lib3mf_meshobject_getbeamlattice_clipping(pMeshObject, &nClipMode, &nClippingMesh);
		if (hResult != LIB3MF_OK)
			return hResult;
	}

	DWORD nSliceStackId;
	hResult = lib3mf_meshobject_getslicestackid(pMeshObject, &nSliceStackId);
	if (hResult != LIB3MF_OK)
		return hResult;
	DWORD nTotalPolygons = 0;
	if (nSliceStackId > 0) {
		PLib3MFSliceStack *pSliceStack;
		hResult = lib3mf_model_getslicestackById(pModel, nSliceStackId, &pSliceStack);
		if (hResult != LIB3MF_OK)
			return hResult;
		hResult = lib3mf_slicestack_getslicecount(pSliceStack, &nSliceCount);
		if (hResult != LIB3MF_OK)
			return hResult;
		for (DWORD iSlice = 0; iSlice < nSliceCount; iSlice++) {
			PLib3MFSlice *pSlice;
			hResult = lib3mf_slicestack_getslice(pSliceStack, iSlice, &pSlice);
			if (hResult != LIB3MF_OK)
				return hResult;
			DWORD nPolygonCount;
			hResult = lib3mf_slice_getpolygoncount(pSlice, &nPolygonCount);
			if (hResult != LIB3MF_OK)
				return hResult;
			for (DWORD polInd = 0; polInd < nPolygonCount; polInd++) {
				DWORD nPolygonIndexCount;
				hResult = lib3mf_slice_getpolygonindexcount(pSlice, polInd, &nPolygonIndexCount);
				if (hResult != LIB3MF_OK)
					return hResult;
				nTotalPolygons += nPolygonIndexCount;
				std::vector<DWORD> vctPoldIndices(nPolygonIndexCount);
				hResult = lib3mf_slice_getpolygonindices(pSlice, polInd, &vctPoldIndices[0], nPolygonIndexCount);
				if (hResult != LIB3MF_OK)
					return hResult;
				/*
				if (iSlice == 0) {
					DWORD nSliceVertexCount;
					hResult = lib3mf_slice_getvertexcount(pSlice, &nSliceVertexCount);
					if (hResult != LIB3MF_OK)
						return hResult;
					std::vector<MODELSLICEVERTEX> vctSliceVertices(nSliceVertexCount);
					hResult = lib3mf_slice_getvertices(pSlice, &vctSliceVertices[0], nSliceVertexCount);
					if (hResult != LIB3MF_OK)
						return hResult;
					std::cout << "$$POLYLINE/1,1," << nPolygonIndexCount;
					for (DWORD iVert = 0; iVert < nPolygonIndexCount; iVert++) {
						std::cout << "," << vctSliceVertices[vctPoldIndices[iVert]].m_fPosition[0] << ","
							<< vctSliceVertices[vctPoldIndices[iVert]].m_fPosition[1];
					}
					std::cout << std::endl;
				}
				*/
			}
		}
	}

	// Output data
	std::cout << "   Vertex count:    " << nVertexCount << std::endl;
	std::cout << "   Triangle count:  " << nTriangleCount << std::endl;
	if (nBeamCount > 0) {
		std::cout << "   Beam count:  " << nBeamCount << std::endl;
		if (bHasRepresentation)
			std::cout << "   |_Representation Mesh ID:  " << nRepresentationMesh << std::endl;
		if (nClipMode != MODELBEAMLATTICECLIPMODE_NONE)
			std::cout << "   |_Clipping Mesh ID:  " << nClippingMesh << "(mode=" << nClipMode << ")" << std::endl;
		if (nBeamSetCount > 0) {
			std::cout << "   |_BeamSet count:  " << nBeamSetCount << std::endl;
		}
	}
	if (nSliceStackId > 0) {
		std::cout << "   Slice count:  " << nSliceCount << "; PolygonCount = " << nTotalPolygons << std::endl;
	}

	return LIB3MF_OK;
}

HRESULT ShowComponentsObjectInformation(_In_ PLib3MFModelComponentsObject * pComponentsObject)
{
	HRESULT hResult;
	hResult = ShowObjectProperties(pComponentsObject);
	if (hResult != LIB3MF_OK)
		return hResult;

	// Retrieve Component
	DWORD nComponentCount;
	DWORD nIndex;
	hResult = lib3mf_componentsobject_getcomponentcount(pComponentsObject, &nComponentCount);
	if (hResult != LIB3MF_OK)
		return hResult;

	std::cout << "   Component count:    " << nComponentCount << std::endl;

	for (nIndex = 0; nIndex < nComponentCount; nIndex++) {
		DWORD nResourceID;
		PLib3MFModelComponent * pComponent;
		hResult = lib3mf_componentsobject_getcomponent(pComponentsObject, nIndex, &pComponent);
		if (hResult != LIB3MF_OK) {
			return hResult;
		}

		hResult = lib3mf_component_getobjectresourceid(pComponent, &nResourceID);
		if (hResult != LIB3MF_OK) {
			lib3mf_release(pComponent);
			return hResult;
		}

		std::cout << "   Component " << nIndex << ":    Object ID:   " << nResourceID << std::endl;
		BOOL bHasTransform;
		hResult = lib3mf_component_hastransform(pComponent, &bHasTransform);
		if (hResult != LIB3MF_OK) {
			lib3mf_release(pComponent);
			return hResult;
		}

		if (bHasTransform) {
			MODELTRANSFORM Transform;

			// Retrieve Transform
			hResult = lib3mf_component_gettransform(pComponent, &Transform);
			if (hResult != LIB3MF_OK) {
				lib3mf_release(pComponent);
				return hResult;
			}

			std::cout << "                   Transformation:  [ " << Transform.m_fFields[0][0] << " " << Transform.m_fFields[0][1] << " " << Transform.m_fFields[0][2] << " " << Transform.m_fFields[0][3] << " ]" << std::endl;
			std::cout << "                                    [ " << Transform.m_fFields[1][0] << " " << Transform.m_fFields[1][1] << " " << Transform.m_fFields[1][2] << " " << Transform.m_fFields[1][3] << " ]" << std::endl;
			std::cout << "                                    [ " << Transform.m_fFields[2][0] << " " << Transform.m_fFields[2][1] << " " << Transform.m_fFields[2][2] << " " << Transform.m_fFields[2][3] << " ]" << std::endl;
		}
		else {
			std::cout << "                   Transformation:  none" << std::endl;

		}


	}

	return LIB3MF_OK;
}

HRESULT ShowThumbnailInformation(_In_ PLib3MFModel * pModel)
{
	/*
	// TODO: this is not yet implemented in Lib3MF
	HRESULT hResult;
	PLib3MFModelThumbnailIterator * pThumbnails;
	hResult = lib3mf_model_getthumbnails(pModel, &pThumbnails);
	if (hResult != LIB3MF_OK) {
		return hResult;
	}

	PLib3MFModelThumbnail * pThumbnail;
	hResult = lib3mf_thumbnailiterator_getcurrent(pThumbnails, &pThumbnail);
	if (hResult != LIB3MF_OK) {
		lib3mf_release(pThumbnails);
		return hResult;
	}
	lib3mf_release(pThumbnails);
	*/
	return LIB3MF_OK;
}


HRESULT ShowMetaDataInformation(_In_ PLib3MFModel * pModel)
{
	HRESULT hResult;
	DWORD nMetaCount;

	hResult = lib3mf_model_getmetadatacount(pModel, &nMetaCount);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not metadata count: " << std::hex << hResult << std::endl;
		return -1;
	}

	for (DWORD iMeta = 0; iMeta < nMetaCount; iMeta++) {
		std::string sMetaDataKey;
		std::string sMetaDataValue;
		DWORD nNeededChars;
		hResult = lib3mf_model_getmetadatakeyutf8(pModel, iMeta, NULL, 0, &nNeededChars);
		if (hResult != LIB3MF_OK) {
			return hResult;
		}
		// Retrieve Mesh Name
		if (nNeededChars > 0) {
			std::vector<char> pBuffer;
			pBuffer.resize(nNeededChars + 1);
			hResult = lib3mf_model_getmetadatakeyutf8(pModel, iMeta, &pBuffer[0], nNeededChars + 1, NULL);
			if (hResult != LIB3MF_OK) {
				return hResult;
			}
			pBuffer[nNeededChars] = 0;
			sMetaDataKey = std::string(&pBuffer[0]);
		}

		hResult = lib3mf_model_getmetadatavalueutf8(pModel, iMeta, NULL, 0, &nNeededChars);
		if (hResult != LIB3MF_OK) {
			return hResult;
		}
		// Retrieve Mesh Name
		if (nNeededChars > 0) {
			std::vector<char> pBuffer;
			pBuffer.resize(nNeededChars + 1);
			hResult = lib3mf_model_getmetadatavalueutf8(pModel, iMeta, &pBuffer[0], nNeededChars + 1, NULL);
			if (hResult != LIB3MF_OK) {
				return hResult;
			}
			pBuffer[nNeededChars] = 0;
			sMetaDataValue = std::string(&pBuffer[0]);
		}

		std::cout << "Metadatum: " << iMeta << ":" << std::endl;
		std::cout << "Key   = \"" << sMetaDataKey << "\"" << std::endl;
		std::cout << "Value = \"" << sMetaDataValue << "\"" << std::endl;

	}
	return LIB3MF_OK;
}

#ifndef __GNUC__
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif // __GNUC__
{
	// General Variables
	HRESULT hResult;
	DWORD nInterfaceVersionMajor, nInterfaceVersionMinor, nInterfaceVersionMicro;
	BOOL pbHasNext;
	DWORD nWarningCount, nErrorCode, nNeededChars;
	LPCSTR pszErrorMessage;

	// Objects
	PLib3MFModel * pModel;
	PLib3MFModelReader * p3MFReader;
	PLib3MFModelBuildItemIterator * pBuildItemIterator;
	PLib3MFModelResourceIterator * pResourceIterator;


	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Read example" << std::endl;
	std::cout << "------------------------------------------------------------------" << std::endl;

	// Check 3MF Library Version
	hResult = lib3mf_getinterfaceversion(&nInterfaceVersionMajor, &nInterfaceVersionMinor, &nInterfaceVersionMicro);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not get 3MF Library version: " << std::hex << hResult << std::endl;
		return -1;
	}

	if ((nInterfaceVersionMajor != NMR_APIVERSION_INTERFACE_MAJOR)) {
		std::cout << "invalid 3MF Library major version: " << NMR_APIVERSION_INTERFACE_MAJOR << std::endl;
		return -1;
	}
	if (!(nInterfaceVersionMinor >= NMR_APIVERSION_INTERFACE_MINOR)) {
		std::cout << "invalid 3MF Library minor version: " << NMR_APIVERSION_INTERFACE_MINOR << std::endl;
		return -1;
	}

	// Create Model Instance
	hResult = lib3mf_createmodel(&pModel);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create model: " << std::hex << hResult << std::endl;
		return -1;
	}

	// Parse Arguments
	if (argc != 2) {
		std::cout << "Usage: " << std::endl;
		std::cout << "ExtractInfo.exe model.3mf" << std::endl;
		return -1;
	}

	// Read 3MF file
	std::string sFilename(argv[1]);
	std::string sAnsiFilename(sFilename.begin(), sFilename.end());


	// Create Model Reader
	hResult = lib3mf_model_queryreader(pModel, "3mf", &p3MFReader);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create model reader: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pModel, &nErrorCode, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorCode << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pModel);
		return -1;
	}

	// And deactivate the strict mode (default is "false", anyway. This just demonstrates where/how to use it).
	hResult = lib3mf_reader_setstrictmodeactive(p3MFReader, false);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not set strict mode: " << std::hex << hResult << std::endl;
	}

	// Import Model from 3MF File
	bool bErrorOnRead = false;
	std::cout << "reading " << sAnsiFilename << "..." << std::endl;
	hResult = lib3mf_reader_readfromfileutf8(p3MFReader, sFilename.c_str());
	if (hResult != LIB3MF_OK) {
		std::cout << "could not read file: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(p3MFReader, &nErrorCode, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorCode << ": " << pszErrorMessage << std::endl;
		// don't abort right away
		bErrorOnRead = true;
	}

	// Check warnings in any case:
	hResult = lib3mf_reader_getwarningcount(p3MFReader, &nWarningCount);
	if (hResult != LIB3MF_OK) {
		std::cout << "could get warning count: " << std::hex << hResult << std::endl;
		lib3mf_release(p3MFReader);
		lib3mf_release(pModel);
		return -1;
	}

	for (DWORD iWarning = 0; iWarning < nWarningCount; iWarning++) {
		hResult = lib3mf_reader_getwarningutf8(p3MFReader, iWarning, &nErrorCode, NULL, 0, &nNeededChars);
		if (hResult != LIB3MF_OK) {
			std::cout << "could get warning : " << std::hex << hResult << std::endl;
			lib3mf_release(p3MFReader);
			lib3mf_release(pModel);
			return hResult;
		}

		std::string sWarning;
		sWarning.resize(nNeededChars + 1);
		hResult = lib3mf_reader_getwarningutf8(p3MFReader, iWarning, &nErrorCode, &sWarning[0], nNeededChars + 1, NULL);
		if (hResult != LIB3MF_OK) {
			std::cout << "could get warning : " << std::hex << hResult << std::endl;
			lib3mf_release(p3MFReader);
			lib3mf_release(pModel);
			return hResult;
		}
		// Insert custom warning handling here
		std::cout << "Encountered warning : " << sWarning << std::endl;
	}

	// Finally stop if we had an error on read
	if (bErrorOnRead) {
		lib3mf_release(p3MFReader);
		lib3mf_release(pModel);
		return -1;
	}

	// Release model reader
	lib3mf_release(p3MFReader);

	// get thumbnail info
	hResult = ShowThumbnailInformation(pModel);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not get thumbnail informtation: " << std::hex << hResult << std::endl;
		lib3mf_release(pModel);
		return -1;
	}

	// get metadata
	hResult = ShowMetaDataInformation(pModel);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not get metadata informtation: " << std::hex << hResult << std::endl;
		lib3mf_release(pModel);
		return -1;
	}

	// Iterate through all the Objects
	hResult = lib3mf_model_getobjects(pModel, &pResourceIterator);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not get object: " << std::hex << hResult << std::endl;
		lib3mf_release(pModel);
		return -1;
	}

	hResult = lib3mf_resourceiterator_movenext(pResourceIterator, &pbHasNext);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not get next object: " << std::hex << hResult << std::endl;
		lib3mf_release(pResourceIterator);
		lib3mf_release(pModel);
		return -1;
	}

	while (pbHasNext) {
		PLib3MFModelResource * pResource;
		PLib3MFModelMeshObject * pMeshObject;
		PLib3MFModelComponentsObject * pComponentsObject;
		ModelResourceID ResourceID;

		// get current resource
		hResult = lib3mf_resourceiterator_getcurrent(pResourceIterator, &pResource);
		if (hResult != LIB3MF_OK) {
			std::cout << "could not get resource: " << std::hex << hResult << std::endl;
			lib3mf_release(pResourceIterator);
			lib3mf_release(pModel);
			return -1;
		}

		// get resource ID
		hResult = lib3mf_resource_getresourceid(pResource, &ResourceID);
		if (hResult != LIB3MF_OK) {
			std::cout << "could not get resource id: " << std::hex << hResult << std::endl;
			lib3mf_release(pResource);
			lib3mf_release(pResourceIterator);
			lib3mf_release(pModel);
			return -1;
		}

		// Query mesh interface
		BOOL bIsMeshObject;
		hResult = lib3mf_object_ismeshobject(pResource, &bIsMeshObject);
		if ((hResult == LIB3MF_OK) && (bIsMeshObject)) {
			std::cout << "------------------------------------------------------" << std::endl;
			std::cout << "mesh object #" << ResourceID << ": " << std::endl;

			pMeshObject = pResource;

			// Show Mesh Object Information
			hResult = ShowMeshObjectInformation(pMeshObject, pModel);
			if (hResult != LIB3MF_OK) {
				lib3mf_release(pResource);
				lib3mf_release(pResourceIterator);
				lib3mf_release(pModel);
				return -1;
			}
		}


		// Query component interface
		BOOL bIsComponentsObject;
		hResult = lib3mf_object_iscomponentsobject(pResource, &bIsComponentsObject);
		if ((hResult == LIB3MF_OK) && (bIsComponentsObject)) {
			std::cout << "------------------------------------------------------" << std::endl;
			std::cout << "component object #" << ResourceID << ": " << std::endl;

			pComponentsObject = (PLib3MFModelComponentsObject*)pResource;

			// Show Component Object Information
			hResult = ShowComponentsObjectInformation(pComponentsObject);
			if (hResult != LIB3MF_OK) {
				lib3mf_release(pResource);
				lib3mf_release(pResourceIterator);
				lib3mf_release(pModel);
				return -1;
			}
		}

		// free instances
		lib3mf_release(pResource);

		hResult = lib3mf_resourceiterator_movenext(pResourceIterator, &pbHasNext);
		if (hResult != LIB3MF_OK) {
			std::cout << "could not get next object: " << std::hex << hResult << std::endl;
			return -1;
		}
	}

	// Release Resource Iterator
	lib3mf_release(pResourceIterator);


	// Iterate through all the Build items
	hResult = lib3mf_model_getbuilditems(pModel, &pBuildItemIterator);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not get build items: " << std::hex << hResult << std::endl;
		lib3mf_release(pBuildItemIterator);
		lib3mf_release(pModel);
		return -1;
	}

	hResult = lib3mf_builditemiterator_movenext(pBuildItemIterator, &pbHasNext);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not get next build item: " << std::hex << hResult << std::endl;
		lib3mf_release(pBuildItemIterator);
		lib3mf_release(pModel);
		return -1;
	}

	while (pbHasNext) {

		DWORD ResourceID;
		MODELTRANSFORM Transform;
		PLib3MFModelBuildItem * pBuildItem;
		// Retrieve Build Item
		hResult = lib3mf_builditemiterator_getcurrent(pBuildItemIterator, &pBuildItem);
		if (hResult != LIB3MF_OK) {
			std::cout << "could not get build item: " << std::hex << hResult << std::endl;
			lib3mf_release(pBuildItemIterator);
			lib3mf_release(pModel);
			return -1;
		}

		// Retrieve Resource
		PLib3MFModelObjectResource * pObjectResource;
		hResult = lib3mf_builditem_getobjectresource(pBuildItem, &pObjectResource);
		if (hResult != LIB3MF_OK) {
			std::cout << "could not get build item resource: " << std::hex << hResult << std::endl;
			lib3mf_release(pBuildItem);
			lib3mf_release(pBuildItemIterator);
			lib3mf_release(pModel);
			return -1;
		}

		// Retrieve Resource ID
		hResult = lib3mf_resource_getresourceid(pObjectResource, &ResourceID);
		if (hResult != LIB3MF_OK) {
			std::cout << "could not get resource id: " << std::hex << hResult << std::endl;
			lib3mf_release(pObjectResource);
			lib3mf_release(pBuildItem);
			lib3mf_release(pBuildItemIterator);
			lib3mf_release(pModel);
			return -1;
		}

		// Release Object Resource ID
		lib3mf_release(pObjectResource);

		// Output
		std::cout << "------------------------------------------------------" << std::endl;
		std::cout << "Build item (Object #" << ResourceID << "): " << std::endl;

		// Check Object Transform
		BOOL bHasTransform;
		hResult = lib3mf_builditem_hasobjecttransform(pBuildItem, &bHasTransform);
		if (hResult != LIB3MF_OK) {
			lib3mf_release(pBuildItem);
			lib3mf_release(pBuildItemIterator);
			lib3mf_release(pModel);
			std::cout << "could not check object transform: " << std::hex << hResult << std::endl;
			return -1;
		}

		if (bHasTransform) {
			// Retrieve Transform
			hResult = lib3mf_builditem_getobjecttransform(pBuildItem, &Transform);
			if (hResult != LIB3MF_OK) {
				lib3mf_release(pBuildItem);
				lib3mf_release(pBuildItemIterator);
				lib3mf_release(pModel);
				std::cout << "could not get object transform: " << std::hex << hResult << std::endl;
				return -1;
			}

			std::cout << "   Transformation:  [ " << Transform.m_fFields[0][0] << " " << Transform.m_fFields[0][1] << " " << Transform.m_fFields[0][2] << " " << Transform.m_fFields[0][3] << " ]" << std::endl;
			std::cout << "                    [ " << Transform.m_fFields[1][0] << " " << Transform.m_fFields[1][1] << " " << Transform.m_fFields[1][2] << " " << Transform.m_fFields[1][3] << " ]" << std::endl;
			std::cout << "                    [ " << Transform.m_fFields[2][0] << " " << Transform.m_fFields[2][1] << " " << Transform.m_fFields[2][2] << " " << Transform.m_fFields[2][3] << " ]" << std::endl;
		}
		else {
			std::cout << "   Transformation:  none" << std::endl;

		}

		// Retrieve Mesh Part Number Length
		std::wstring sPartNumber;
		DWORD nNeededChars;
		hResult = lib3mf_builditem_getpartnumber(pBuildItem, NULL, 0, &nNeededChars);
		if (hResult != LIB3MF_OK) {
			lib3mf_release(pBuildItem);
			lib3mf_release(pBuildItemIterator);
			lib3mf_release(pModel);
			return hResult;
		}

		// Retrieve Mesh Name
		if (nNeededChars > 0) {
			std::vector<wchar_t> pBuffer;
			pBuffer.resize(nNeededChars + 1);
			hResult = lib3mf_builditem_getpartnumber(pBuildItem, &pBuffer[0], nNeededChars + 1, NULL);
			pBuffer[nNeededChars] = 0;
			sPartNumber = std::wstring(&pBuffer[0]);
		}

		// Output Part number in local codepage
		std::string sAnsiPartNumber(sPartNumber.begin(), sPartNumber.end());
		std::cout << "   Part number:     \"" << sAnsiPartNumber << "\"" << std::endl;

		// Release Build Item
		lib3mf_release(pBuildItem);

		// Move to next Item
		hResult = lib3mf_builditemiterator_movenext(pBuildItemIterator, &pbHasNext);
		if (hResult != LIB3MF_OK) {
			std::cout << "could not get next build item: " << std::hex << hResult << std::endl;
			return -1;
		}
	}

	std::cout << "------------------------------------------------------" << std::endl;

	// Release Build Item Iterator
	lib3mf_release(pBuildItemIterator);

	// Release Model
	lib3mf_release(pModel);


	std::cout << "done" << std::endl;


	return 0;
}

