/*++

Copyright (C) 2018 3MF Consortium

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
#include "Model/COM/NMR_COMInterface_ModelPropertyHandler.h"
#include "Model/COM/NMR_COMInterface_ModelDefaultPropertyHandler.h"
#include "Model/COM/NMR_COMInterface_ModelMeshBeamSet.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <cmath>

namespace NMR {

	CCOMModelMeshObject::CCOMModelMeshObject()
	{
		m_nErrorCode = NMR_SUCCESS;
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

	LIB3MFRESULT CCOMModelMeshObject::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelMeshObject::handleNMRException(_In_ CNMRException * pException)
	{
		__NMRASSERT(pException);

		m_nErrorCode = pException->getErrorCode();
		m_sErrorMessage = std::string(pException->what());

		CNMRException_Windows * pWinException = dynamic_cast<CNMRException_Windows *> (pException);
		if (pWinException != nullptr) {
			return pWinException->getHResult();
		}
		else {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFRESULT CCOMModelMeshObject::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
	{
		if (!pErrorCode)
			return LIB3MF_POINTER;

		*pErrorCode = m_nErrorCode;
		if (pErrorMessage) {
			if (m_nErrorCode != NMR_SUCCESS) {
				*pErrorMessage = m_sErrorMessage.c_str();
			}
			else {
				*pErrorMessage = nullptr;
			}
		}

		return LIB3MF_OK;
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
		try {
			if (!pnVertexCount)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			*pnVertexCount = pMesh->getNodeCount();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetTriangleCount(_Out_ DWORD * pnTriangleCount)
	{
		try {
			if (!pnTriangleCount)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			*pnTriangleCount = pMesh->getFaceCount();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeamCount(_Out_ DWORD * pnBeamCount)
	{
		try {
			if (!pnBeamCount)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			*pnBeamCount = pMesh->getBeamCount();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetVertex(_In_ DWORD nIndex, _Out_ MODELMESHVERTEX * pVertex)
	{
		UINT j;

		try {
			if (!pVertex)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// retrieve node and return position
			MESHNODE * pNode = pMesh->getNode(nIndex);
			for (j = 0; j < 3; j++)
				pVertex->m_fPosition[j] = pNode->m_position.m_fields[j];

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetVertex(_In_ DWORD nIndex, _In_ MODELMESHVERTEX * pVertex)
	{
		UINT j;


		try {
			if (!pVertex)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

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

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::AddVertex(_In_ MODELMESHVERTEX * pVertex, _Out_opt_ DWORD * pnIndex)
	{
		UINT j;

		try {
			if (!pVertex)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

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

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetTriangle(_In_ DWORD nIndex, _Out_ MODELMESHTRIANGLE * pTriangle)
	{
		UINT j;

		try {
			if (!pTriangle)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// retrieve triangle and return indices
			MESHFACE * pFace = pMesh->getFace(nIndex);
			for (j = 0; j < 3; j++)
				pTriangle->m_nIndices[j] = pFace->m_nodeindices[j];

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetTriangle(_In_ DWORD nIndex, _In_ MODELMESHTRIANGLE * pTriangle)
	{
		UINT j;

		try {
			if (!pTriangle)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

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

			// retrieve triangle and set indices
			MESHFACE * pFace = pMesh->getFace(nIndex);
			for (j = 0; j < 3; j++)
				pFace->m_nodeindices[j] = pTriangle->m_nIndices[j];

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::AddTriangle(_In_ MODELMESHTRIANGLE * pTriangle, _Out_opt_ DWORD * pnIndex)
	{
		UINT j;

		try {
			if (!pTriangle)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

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

			// retrieve nodes and add face
			MESHNODE * pNodes[3];
			for (j = 0; j < 3; j++)
				pNodes[j] = pMesh->getNode(pTriangle->m_nIndices[j]);

			MESHFACE * pFace = pMesh->addFace(pNodes[0], pNodes[1], pNodes[2]);
			if (pnIndex) {
				*pnIndex = pFace->m_index;
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}



	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeamLatticeMinLength(_In_ DOUBLE *pdMinLength)
	{
		try {
			if (!pdMinLength)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			*pdMinLength = pMesh->getBeamLatticeMinLength();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetBeamLatticeMinLength(_In_ DOUBLE dMinLength)
	{
		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			pMesh->setBeamLatticeMinLength(dMinLength);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeamLatticeRadius(_In_ DOUBLE *pdRadius)
	{
		try {
			if (!pdRadius)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			*pdRadius = pMesh->getDefaultBeamRadius();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetBeamLatticeRadius(_In_ DOUBLE dRadius)
	{
		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			pMesh->setDefaultBeamRadius(dRadius);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeamLatticeCapMode(_Out_ eModelBeamLatticeCapMode *peCapMode)
	{
		try {
			if (!peCapMode)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			*peCapMode = pMesh->getBeamLatticeCapMode();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetBeamLatticeCapMode(_Out_ eModelBeamLatticeCapMode eCapMode)
	{
		try {
			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			pMesh->setBeamLatticeCapMode(eCapMode);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeamLatticeClipping(_Out_ eModelBeamLatticeClipMode * peClipMode, _Out_ DWORD *pnResourceID)
	{
		try {
			if ((!peClipMode) || (!pnResourceID))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			if (!pObject->getBeamLatticeAttributes()->m_bHasClippingMeshID) {
				*peClipMode = eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE;
			}
			else {
				*peClipMode = pObject->getBeamLatticeAttributes()->m_eClipMode;
				*pnResourceID = pObject->getBeamLatticeAttributes()->m_nClippingMeshID->getUniqueID();
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetBeamLatticeClipping(_In_ eModelBeamLatticeClipMode eClipMode, _In_ DWORD nResourceID)
	{
		try {
			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			if (eClipMode == eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE) {
				pObject->getBeamLatticeAttributes()->m_eClipMode = eClipMode;
				pObject->getBeamLatticeAttributes()->m_bHasClippingMeshID = false;
				pObject->getBeamLatticeAttributes()->m_nClippingMeshID = nullptr;
			}
			else {
				CModel* pModel = pObject->getModel();
				__NMRASSERT(pModel);

				CModelMeshObject * pClippingObject = dynamic_cast<CModelMeshObject*>(pModel->findObject(nResourceID));
				if (pClippingObject == nullptr) {
					throw CNMRException(LIB3MF_INVALIDARG);
				}
				// check, if this object will be written before 
				nfInt32 nComp = pModel->compareObjectsByResourceID(pClippingObject, pObject);
				if (nComp < 0) { // pClippingObject has been defined after pObject
					throw CNMRException(LIB3MF_INVALIDARG);
				}

				pObject->getBeamLatticeAttributes()->m_eClipMode = eClipMode;
				pObject->getBeamLatticeAttributes()->m_bHasClippingMeshID = true;
				pObject->getBeamLatticeAttributes()->m_nClippingMeshID = pClippingObject->getResourceID();
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeamLatticeRepresentation(_Out_ BOOL *pbHasRepresentation, _Out_ DWORD *pnResourceID)
	{
		try {
			if (!pbHasRepresentation || !pnResourceID)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			*pbHasRepresentation = pObject->getBeamLatticeAttributes()->m_bHasRepresentationMeshID;
			if (pObject->getBeamLatticeAttributes()->m_bHasRepresentationMeshID) {
				*pnResourceID = pObject->getBeamLatticeAttributes()->m_nRepresentationID->getUniqueID();
			}
			else {
				*pnResourceID = 0;
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetBeamLatticeRepresentation(_In_ DWORD nResourceID)
	{
		try {
			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			CModel* pModel = pObject->getModel();
			__NMRASSERT(pModel);

			if (nResourceID == 0) {
				pObject->getBeamLatticeAttributes()->m_bHasRepresentationMeshID = false;
				pObject->getBeamLatticeAttributes()->m_nRepresentationID = nullptr;
			}
			else {
				CModelMeshObject * pRepresentationObject = dynamic_cast<CModelMeshObject*>(pModel->findObject(nResourceID));
				if (pRepresentationObject == nullptr) {
					throw CNMRException(LIB3MF_INVALIDARG);
				}
				// check, if this object will be written before 
				nfInt32 nComp = pModel->compareObjectsByResourceID(pRepresentationObject, pObject);
				if (nComp < 0) { // pClippingObject has been defined after pObject
					throw CNMRException(LIB3MF_INVALIDARG);
				}

				pObject->getBeamLatticeAttributes()->m_bHasRepresentationMeshID = true;
				pObject->getBeamLatticeAttributes()->m_nRepresentationID = pRepresentationObject->getResourceID();
			}
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeam(_In_ DWORD nIndex, _Out_ MODELMESHBEAM * pBeam)
	{
		UINT j;

		try {
			if (!pBeam)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// retrieve beam and return indices and additional information
			MESHBEAM * pMeshBeam = pMesh->getBeam(nIndex);
			for (j = 0; j < 2; j++) {
				pBeam->m_nIndices[j] = pMeshBeam->m_nodeindices[j];
				pBeam->m_dRadius[j] = pMeshBeam->m_radius[j];
				pBeam->m_eCapMode[j] = (eModelBeamLatticeCapMode)(pMeshBeam->m_capMode[j]);
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModelMeshObject::SetBeam(_In_ DWORD nIndex, _In_ MODELMESHBEAM * pBeam)
	{
		UINT j;

		try {
			if (!pBeam)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Check for input validity
			UINT nNodeCount = pMesh->getNodeCount();
			for (j = 0; j < 2; j++)
				if (pBeam->m_nIndices[j] >= nNodeCount)
					throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);
			if (pBeam->m_nIndices[0] == pBeam->m_nIndices[1])
				throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);

			// retrieve beam and set nodes and additional information
			MESHBEAM * pMeshBeam = pMesh->getBeam(nIndex);
			for (j = 0; j < 2; j++) {
				pMeshBeam->m_nodeindices[j] = pBeam->m_nIndices[j];
				pMeshBeam->m_radius[j] = pBeam->m_dRadius[j];
				pMeshBeam->m_capMode[j] = pBeam->m_eCapMode[j];
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModelMeshObject::AddBeam(_In_ MODELMESHBEAM * pBeam, _Out_opt_ DWORD * pnIndex)
	{
		UINT j;

		try {
			if (!pBeam)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			DWORD objectType;
			GetType(&objectType);
			if ((objectType != eModelObjectType::MODELOBJECTTYPE_MODEL) && (objectType != eModelObjectType::MODELOBJECTTYPE_SOLIDSUPPORT)) {
				throw CNMRException(NMR_ERROR_BEAMLATTICE_INVALID_OBJECTTYPE);
			}

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Check for input validity
			UINT nNodeCount = pMesh->getNodeCount();
			for (j = 0; j < 2; j++)
				if (pBeam->m_nIndices[j] >= nNodeCount)
					throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);
			if (pBeam->m_nIndices[0] == pBeam->m_nIndices[1])
				throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);

			// retrieve nodes and add beam
			MESHNODE * pNodes[2];
			for (j = 0; j < 2; j++)
				pNodes[j] = pMesh->getNode(pBeam->m_nIndices[j]);

			nfInt32 c1 = pBeam->m_eCapMode[0];
			nfInt32 c2 = pBeam->m_eCapMode[1];
			MESHBEAM * pMeshBeam = pMesh->addBeam(pNodes[0], pNodes[1], &pBeam->m_dRadius[0], &pBeam->m_dRadius[1], &c1, &c2);
			if (pnIndex) {
				*pnIndex = pMeshBeam->m_index;
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}
	
	LIB3MFMETHODIMP CCOMModelMeshObject::GetVertices(_Out_ MODELMESHVERTEX * pVertices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnVertexCount)
	{
		UINT j, nIndex;

		try {
			if (!pVertices)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Check Buffer size
			UINT nNodeCount = pMesh->getNodeCount();
			if (pnVertexCount)
				*pnVertexCount = nNodeCount;

			if (nBufferSize < nNodeCount)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			MODELMESHVERTEX * pVertex = pVertices;
			for (nIndex = 0; nIndex < nNodeCount; nIndex++) {
				// retrieve node and return position
				MESHNODE * pNode = pMesh->getNode(nIndex);
				for (j = 0; j < 3; j++)
					pVertex->m_fPosition[j] = pNode->m_position.m_fields[j];
				pVertex++;
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetTriangleIndices(_Out_ MODELMESHTRIANGLE * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnTriangleCount)
	{
		UINT j, nIndex;

		try {
			if (!pIndices)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Check Buffer size
			UINT nFaceCount = pMesh->getFaceCount();
			if (pnTriangleCount)
				*pnTriangleCount = nFaceCount;

			if (nBufferSize < nFaceCount)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			MODELMESHTRIANGLE * pTriangle = pIndices;
			for (nIndex = 0; nIndex < nFaceCount; nIndex++) {
				// retrieve triangle and return position
				MESHFACE * pFace = pMesh->getFace(nIndex);
				for (j = 0; j < 3; j++)
					pTriangle->m_nIndices[j] = pFace->m_nodeindices[j];
				pTriangle++;
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}



	LIB3MFMETHODIMP CCOMModelMeshObject::GetUUIDUTF8(_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer)
	{
		try {
			if (!pbHasUUID || !pszBuffer)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pMeshObject = getMeshObject();
			if (!pMeshObject)
				throw CNMRException(NMR_ERROR_INVALIDMESH);

			PUUID pUUID = pMeshObject->uuid();
			*pbHasUUID = (pUUID.get()!=nullptr);
			nfUint32 nNeededChars = 0;
			if (*pbHasUUID)
				fnStringToBufferSafe(pUUID->toString(), pszBuffer, 37, &nNeededChars);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetUUIDUTF8(_In_ LPCSTR pszUUID)
	{
		try
		{
			if (!pszUUID)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pMeshObject = getMeshObject();
			if (!pMeshObject)
				throw CNMRException(NMR_ERROR_INVALIDMESH);

			PUUID pUUID = std::make_shared<CUUID>(pszUUID);
			pMeshObject->setUUID(pUUID);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetBeamIndices(_In_ MODELMESHBEAM * pIndices, _In_ DWORD nBufferSize)
	{
		UINT j, nIndex;

		try {
			if ((nBufferSize > 0) && (!pIndices) )
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (nBufferSize > 0) {
				DWORD objectType;
				GetType(&objectType);
				if ((objectType != eModelObjectType::MODELOBJECTTYPE_MODEL) && (objectType != eModelObjectType::MODELOBJECTTYPE_SOLIDSUPPORT)) {
					throw CNMRException(NMR_ERROR_BEAMLATTICE_INVALID_OBJECTTYPE);
				}
			}

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			pMesh->clearBeamLattice();

			// Check for input validity
			UINT nNodeCount = pMesh->getNodeCount();

			for (nIndex = 0; nIndex < nBufferSize; nIndex++) {
				MODELMESHBEAM &beam = pIndices[nIndex];
				for (j = 0; j < 2; j++)
					if (beam.m_nIndices[j] >= nNodeCount)
						throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);
				if (beam.m_nIndices[0] == beam.m_nIndices[1])
					throw CNMRException_Windows(NMR_ERROR_INVALIDINDEX, LIB3MF_INVALIDARG);

				// retrieve nodes and add beam
				MESHNODE * pNodes[2];
				for (j = 0; j < 2; j++)
					pNodes[j] = pMesh->getNode(beam.m_nIndices[j]);

				nfInt32 c1 = beam.m_eCapMode[0];
				nfInt32 c2 = beam.m_eCapMode[1];
				pMesh->addBeam(pNodes[0], pNodes[1], &beam.m_dRadius[0], &beam.m_dRadius[1], &c1, &c2);
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeamIndices(_Out_ MODELMESHBEAM * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnBeamCount)
	{
		UINT j, nIndex;

		try {
			if (!pIndices)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			// Check Buffer size
			UINT nBeamCount = pMesh->getBeamCount();
			if (pnBeamCount)
				*pnBeamCount = nBeamCount;

			if (nBufferSize < nBeamCount)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			MODELMESHBEAM * pBeam = pIndices;
			for (nIndex = 0; nIndex < nBeamCount; nIndex++) {
				// retrieve node and return position
				MESHBEAM * pMeshBeam = pMesh->getBeam(nIndex);
				for (j = 0; j < 2; j++) {
					pBeam->m_nIndices[j] = pMeshBeam->m_nodeindices[j];
					pBeam->m_dRadius[j] = pMeshBeam->m_radius[j];
					pBeam->m_eCapMode[j] = (eModelBeamLatticeCapMode)pMeshBeam->m_capMode[j];
				}
				pBeam++;
			}

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}
	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeamSetCount(_Out_ DWORD * pnBeamSetCount)
	{
		try {
			if (!pnBeamSetCount)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			__NMRASSERT(pMesh);

			*pnBeamSetCount = pMesh->getBeamSetCount();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::AddBeamSet(_Outptr_ ILib3MFModelMeshBeamSet ** ppBeamSet)
	{
		try {
			if (!ppBeamSet)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			CCOMObject<CCOMModelMeshBeamSet> * pNewBeamSet = new CCOMObject<CCOMModelMeshBeamSet>();
			pNewBeamSet->AddRef();
			pNewBeamSet->setBeamSet(pObject->getMesh()->addBeamSet());
			*ppBeamSet = pNewBeamSet;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetBeamSet(_In_ DWORD nIndex, _Outptr_ ILib3MFModelMeshBeamSet ** ppBeamSet)
	{
		try {
			if (!ppBeamSet)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			CCOMObject<CCOMModelMeshBeamSet> * pNewBeamSet = new CCOMObject<CCOMModelMeshBeamSet>();
			pNewBeamSet->AddRef();
			pNewBeamSet->setBeamSet(pObject->getMesh()->getBeamSet(nIndex));
			*ppBeamSet = pNewBeamSet;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetGeometry(_In_ MODELMESHVERTEX * pVertices, _In_ DWORD nVertexCount, _In_ MODELMESHTRIANGLE * pTriangles, _In_ DWORD nTriangleCount)
	{
		UINT j, nIndex;


		try {
			if ((!pVertices) || (!pTriangles))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

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

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetResourceID(_Out_ DWORD * pnResourceID)
	{
		try {
			if (pnResourceID == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (m_pResource.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			*pnResourceID = m_pResource->getResourceID()->getUniqueID();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetType(_Out_ DWORD * pObjectType)
	{

		try {
			if (pObjectType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (m_pResource.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			*pObjectType = (DWORD)pObject->getObjectType();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetType(_In_ DWORD ObjectType)
	{
		try {
			if (m_pResource.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			pObject->setObjectType((eModelObjectType)ObjectType);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetPartNumber(_In_z_ LPCWSTR pwszPartNumber)
	{
		try {
			if (!pwszPartNumber)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			pObject->setPartNumber(fnUTF16toUTF8(pwszPartNumber));

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetPartNumberUTF8(_In_z_ LPCSTR pszPartNumber)
	{
		try {
			if (!pszPartNumber)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			std::string sUTF8PartNumber(pszPartNumber);
			pObject->setPartNumber(sUTF8PartNumber);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetPartNumber(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(fnUTF8toUTF16(pObject->getPartNumber()), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetPartNumberUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(pObject->getPartNumber(), pszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetName(_In_z_ LPCWSTR pwszName)
	{
		try {
			if (!pwszName)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			pObject->setName(fnUTF16toUTF8(pwszName));

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetNameUTF8(_In_z_ LPCSTR pszName)
	{
		try {
			if (!pszName)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			pObject->setName(pszName);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetName(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(fnUTF8toUTF16(pObject->getName()), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetNameUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			std::string sUTF8Name = pObject->getName();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sUTF8Name, pszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModelMeshObject::IsMeshObject(_Out_ BOOL * pbIsMeshObject)
	{
		try
		{
			if (!pbIsMeshObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*pbIsMeshObject = true;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::IsComponentsObject(_Out_ BOOL * pbIsComponentsObject)
	{
		try
		{
			if (!pbIsComponentsObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*pbIsComponentsObject = false;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::HasSlices(_Out_ BOOL * pbHasSlices)
	{
		try {
			if (!pbHasSlices)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			*pbHasSlices = pObject->hasSlices();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::IsValidObject(_Out_ BOOL * pbIsValid)
	{
		try {
			if (!pbIsValid)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			*pbIsValid = pObject->isValid();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::IsManifoldAndOriented(_Out_ BOOL * pbIsOrientedAndManifold)
	{

		try {
			if (!pbIsOrientedAndManifold)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			*pbIsOrientedAndManifold = pObject->isManifoldAndOriented();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::CreatePropertyHandler(_Outptr_ ILib3MFPropertyHandler ** ppPropertyHandler)
	{
		return CreateMultiPropertyHandler(0, ppPropertyHandler);
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::CreateMultiPropertyHandler(_In_ DWORD nChannel, _Outptr_ ILib3MFPropertyHandler ** ppPropertyHandler)
	{
		try {
			if (!ppPropertyHandler)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelPropertyHandler> * pNewPropertyHandler = new CCOMObject<CCOMModelPropertyHandler>();
			pNewPropertyHandler->AddRef();
			pNewPropertyHandler->setChannel(nChannel);
			pNewPropertyHandler->setMesh(m_pResource);
			*ppPropertyHandler = pNewPropertyHandler;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModelMeshObject::CreateDefaultPropertyHandler(_Outptr_ ILib3MFDefaultPropertyHandler ** ppPropertyHandler)
	{
		try {
			if (!ppPropertyHandler)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelDefaultPropertyHandler> * pNewPropertyHandler = new CCOMObject<CCOMModelDefaultPropertyHandler>();
			pNewPropertyHandler->AddRef();
			pNewPropertyHandler->setChannel(0);
			pNewPropertyHandler->setResource(m_pResource);
			*ppPropertyHandler = pNewPropertyHandler;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::CreateDefaultMultiPropertyHandler(_In_ DWORD nChannel, _Outptr_ ILib3MFDefaultPropertyHandler ** ppPropertyHandler)
	{
		try {
			if (!ppPropertyHandler)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelDefaultPropertyHandler> * pNewPropertyHandler = new CCOMObject<CCOMModelDefaultPropertyHandler>();
			pNewPropertyHandler->AddRef();
			pNewPropertyHandler->setChannel(nChannel);
			pNewPropertyHandler->setResource(m_pResource);
			*ppPropertyHandler = pNewPropertyHandler;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetThumbnailPathUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			std::string sUTF8Path = pObject->getThumbnail();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sUTF8Path, pszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetThumbnailPathUTF8(_In_z_ LPCSTR pszName)
	{
		try {
			if (pszName == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			CModelMeshObject * pObject = getMeshObject();
			__NMRASSERT(pObject);

			std::string sUTF8ThumbnailPath(pszName);
			pObject->setThumbnail(sUTF8ThumbnailPath.c_str());

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetSliceStackId(_In_ DWORD nSliceStackId)
	{
		try {
			PPackageResourceID pID = getMeshObject()->getModel()->findPackageResourceID(nSliceStackId);
			getMeshObject()->setSliceStackId(pID);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetSliceStackId(_Out_ DWORD *pnSliceStackId)
	{
		try {
			if (!pnSliceStackId)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PPackageResourceID pID = getMeshObject()->getSliceStackId();
			if (!pID.get()) {
				*pnSliceStackId = 0;
			}
			else {
				*pnSliceStackId = pID->getUniqueID();
			}
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::GetSlicesMeshResolution(_Out_ eModelSlicesMeshResolution *peSlicesMeshResolution)
	{
		try {
			if (!peSlicesMeshResolution)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*peSlicesMeshResolution = getMeshObject()->slicesMeshResolution();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshObject::SetSlicesMeshResolution(_In_ eModelSlicesMeshResolution eSlicesMeshResolution)
	{
		try {
			getMeshObject()->setSlicesMeshResolution(eSlicesMeshResolution);
			
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

}
