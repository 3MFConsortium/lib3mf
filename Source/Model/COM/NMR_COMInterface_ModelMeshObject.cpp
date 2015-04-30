/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

COM Interface Implementation for Mesh Object Classes

--*/

#include "Model/COM/NMR_COMInterface_ModelMeshObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/NMR_StringUtils.h" 
#include <math.h>

namespace NMR {

	CCOMModelMeshObject::CCOMModelMeshObject()
	{
		// empty on purpose
	}

	_Ret_notnull_ CModelMeshObject * CCOMModelMeshObject::getMeshObject()
	{
		if (m_pResource.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDMESH);

		CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject *> (m_pResource.get());
		if (pMeshObject == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDMESH);

		return pMeshObject;
	}

	_Ret_notnull_ CMesh * CCOMModelMeshObject::getMesh()
	{
		CModelMeshObject * pMeshObject = getMeshObject();
		__NMRASSERT(pMeshObject);

		return pMeshObject->getMesh();
	}

	void CCOMModelMeshObject::setResource(_In_ PModelResource pModelResource)
	{
		m_pResource = pModelResource;
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetVertexCount(_Out_ DWORD * pnVertexCount)
	{
		if (!pnVertexCount)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			*pnVertexCount = pMesh->getNodeCount();
			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetTriangleCount(_Out_ DWORD * pnTriangleCount)
	{
		if (!pnTriangleCount)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			*pnTriangleCount = pMesh->getFaceCount();
			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetVertex(_In_ DWORD nIndex, _Out_ MODELMESHVERTEX * pVertex)
	{
		UINT j;

		if (!pVertex)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// retrieve node and return position
			MESHNODE * pNode = pMesh->getNode(nIndex);
			for (j = 0; j < 3; j++)
				pVertex->m_fPosition[j] = pNode->m_position.m_fields[j];

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetVertex(_In_ DWORD nIndex, _In_ MODELMESHVERTEX * pVertex)
	{
		UINT j;

		if (!pVertex)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Create Position Vector and check input
			NVEC3 vPosition;
			for (j = 0; j < 3; j++) {
				FLOAT fCoord = pVertex->m_fPosition[j];
				if (fabs(fCoord) > NMR_MESH_MAXCOORDINATE)
					throw CNMRException_Windows(NMR_ERROR_INVALIDCOORDINATES, LIB3MF_INVALIDARG);
				vPosition.m_fields[j] = fCoord;
			}

			// Set position to node
			MESHNODE * pNode = pMesh->getNode(nIndex);
			pNode->m_position = vPosition;

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::AddVertex(_In_ MODELMESHVERTEX * pVertex, _Out_opt_ DWORD * pnIndex)
	{
		UINT j;

		if (!pVertex)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Create Position Vector and check input
			NVEC3 vPosition;
			for (j = 0; j < 3; j++) {
				FLOAT fCoord = pVertex->m_fPosition[j];
				if (fabs(fCoord) > NMR_MESH_MAXCOORDINATE)
					throw CNMRException_Windows(NMR_ERROR_INVALIDCOORDINATES, LIB3MF_INVALIDARG);
				vPosition.m_fields[j] = fCoord;
			}

			// Set position to node
			MESHNODE * pNode = pMesh->addNode(vPosition);
			if (pnIndex) {
				*pnIndex = pNode->m_index;
			}

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetTriangle(_In_ DWORD nIndex, _Out_ MODELMESHTRIANGLE * pTriangle)
	{
		UINT j;

		if (!pTriangle)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// retrieve node and return position
			MESHFACE * pFace = pMesh->getFace(nIndex);
			for (j = 0; j < 3; j++)
				pTriangle->m_nIndices[j] = pFace->m_nodeindices[j];

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetTriangle(_In_ DWORD nIndex, _In_ MODELMESHTRIANGLE * pTriangle)
	{
		UINT j;

		if (!pTriangle)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Check for input validity
			UINT nNodeCount = pMesh->getNodeCount();
			for (j = 0; j < 3; j++)
				if (pTriangle->m_nIndices[j] >= nNodeCount)
					throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);
			if ((pTriangle->m_nIndices[0] == pTriangle->m_nIndices[1]) ||
				(pTriangle->m_nIndices[0] == pTriangle->m_nIndices[2]) ||
				(pTriangle->m_nIndices[1] == pTriangle->m_nIndices[2]))
				throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);

			// retrieve node and return position
			MESHFACE * pFace = pMesh->getFace(nIndex);
			for (j = 0; j < 3; j++)
				pFace->m_nodeindices[j] = pTriangle->m_nIndices[j];

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::AddTriangle(_In_ MODELMESHTRIANGLE * pTriangle, _Out_opt_ DWORD * pnIndex)
	{
		UINT j;

		if (!pTriangle)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Check for input validity
			UINT nNodeCount = pMesh->getNodeCount();
			for (j = 0; j < 3; j++)
				if (pTriangle->m_nIndices[j] >= nNodeCount)
					throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);
			if ((pTriangle->m_nIndices[0] == pTriangle->m_nIndices[1]) ||
				(pTriangle->m_nIndices[0] == pTriangle->m_nIndices[2]) ||
				(pTriangle->m_nIndices[1] == pTriangle->m_nIndices[2]))
				throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);

			MESHNODE * pNodes[3];
			for (j = 0; j < 3; j++)
				pNodes[j] = pMesh->getNode(pTriangle->m_nIndices[j]);

			// retrieve node and return position
			MESHFACE * pFace = pMesh->addFace(pNodes[0], pNodes[1], pNodes[2]);
			if (pnIndex) {
				*pnIndex = pFace->m_index;
			}

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetVertices(_Out_ MODELMESHVERTEX * pVertices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnVertexCount)
	{
		UINT j, nIndex;

		if (!pVertices)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Check Buffer size
			UINT nNodeCount = pMesh->getNodeCount();
			if (pnVertexCount)
				*pnVertexCount = nNodeCount;

			if (nBufferSize < nNodeCount)
				return LIB3MF_INVALIDARG;

			MODELMESHVERTEX * pVertex = pVertices;
			for (nIndex = 0; nIndex < nNodeCount; nIndex++) {
				// retrieve node and return position
				MESHNODE * pNode = pMesh->getNode(nIndex);
				for (j = 0; j < 3; j++)
					pVertex->m_fPosition[j] = pNode->m_position.m_fields[j];
				pVertex++;
			}

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetTriangleIndices(_Out_ MODELMESHTRIANGLE * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnTriangleCount)
	{
		UINT j, nIndex;

		if (!pIndices)
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Check Buffer size
			UINT nFaceCount = pMesh->getFaceCount();
			if (pnTriangleCount)
				*pnTriangleCount = nFaceCount;

			if (nBufferSize < nFaceCount)
				return LIB3MF_INVALIDARG;

			MODELMESHTRIANGLE * pTriangle = pIndices;
			for (nIndex = 0; nIndex < nFaceCount; nIndex++) {
				// retrieve node and return position
				MESHFACE * pFace = pMesh->getFace(nIndex);
				for (j = 0; j < 3; j++)
					pTriangle->m_nIndices[j] = pFace->m_nodeindices[j];
				pTriangle++;
			}

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetGeometry(_In_ MODELMESHVERTEX * pVertices, _In_ DWORD nVertexCount, _In_ MODELMESHTRIANGLE * pTriangles, _In_ DWORD nTriangleCount)
	{
		UINT j, nIndex;

		if ((!pVertices) || (!pTriangles))
			return LIB3MF_POINTER;

		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Clear old mesh
			pMesh->clear();

			// Rebuild Mesh Coordinates
			MODELMESHVERTEX * pVertex = pVertices;
			for (nIndex = 0; nIndex < nVertexCount; nIndex++) {
				NVEC3 vPosition;
				for (j = 0; j < 3; j++) {
					FLOAT fCoord = pVertex->m_fPosition[j];
					if (fabs(fCoord) > NMR_MESH_MAXCOORDINATE)
						throw CNMRException_Windows(NMR_ERROR_INVALIDCOORDINATES, LIB3MF_INVALIDARG);
					vPosition.m_fields[j] = fCoord;
				}
				pMesh->addNode(vPosition);

				pVertex++;
			}

			// Rebuild Mesh Faces
			MODELMESHTRIANGLE * pTriangle = pTriangles;
			for (nIndex = 0; nIndex < nTriangleCount; nIndex++) {
				MESHNODE * pNodes[3];

				for (j = 0; j < 3; j++) {
					if (pTriangle->m_nIndices[j] >= nVertexCount)
						throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);
					pNodes[j] = pMesh->getNode(pTriangle->m_nIndices[j]);
				}

				if ((pTriangle->m_nIndices[0] == pTriangle->m_nIndices[1]) ||
					(pTriangle->m_nIndices[0] == pTriangle->m_nIndices[2]) ||
					(pTriangle->m_nIndices[1] == pTriangle->m_nIndices[2]))
					throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);

				pMesh->addFace(pNodes[0], pNodes[1], pNodes[2]);

				pTriangle++;
			}

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetResourceID(_Out_ DWORD * nResourceID)
	{
		if (nResourceID == nullptr)
			return LIB3MF_POINTER;

		if (m_pResource.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			*nResourceID = m_pResource->getResourceID();

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetType(_Out_ DWORD * pObjectType)
	{
		if (pObjectType == nullptr)
			return LIB3MF_POINTER;

		if (m_pResource.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			*pObjectType = (DWORD)pObject->getObjectType();

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetType(_In_ DWORD ObjectType)
	{
		if (m_pResource.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			pObject->setObjectType((eModelObjectType) ObjectType);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetPartNumber(_In_z_ LPCWSTR pwszPartNumber)
	{
		if (!pwszPartNumber)
			return LIB3MF_POINTER;

		try {
			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			std::wstring sPartNumber(pwszPartNumber);
			pObject->setPartNumber(pwszPartNumber);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetPartNumber(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				return LIB3MF_FAIL;

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(pObject->getPartNumber(), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetName(_In_z_ LPCWSTR pwszName)
	{
		if (!pwszName)
			return LIB3MF_POINTER;

		try {
			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			std::wstring sName(pwszName);
			pObject->setName(pwszName);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetName(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				return LIB3MF_FAIL;

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(pObject->getName(), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

}
