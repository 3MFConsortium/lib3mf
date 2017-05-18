/*++

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

COM Interface Implementation for Model Property Handler

--*/

#include "Model/COM/NMR_COMInterface_ModelPropertyHandler.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/NMR_StringUtils.h"

#include "Common/MeshInformation/NMR_MeshInformation_BaseMaterials.h"
#include "Common/MeshInformation/NMR_MeshInformation_NodeColors.h"
#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h"

#include <cmath>

namespace NMR {

	CCOMModelPropertyHandler::CCOMModelPropertyHandler()
	{
		m_nChannel = 0;
		m_nErrorCode = NMR_SUCCESS;
	}

	void CCOMModelPropertyHandler::setMesh(_In_ PModelResource pResource)
	{
		m_pModelMeshResource = pResource;
	}

	void CCOMModelPropertyHandler::setChannel(_In_ DWORD nChannel)
	{
		m_nChannel = nChannel;
	}

	_Ret_notnull_ CMesh * CCOMModelPropertyHandler::getMesh()
	{
		CMesh * pMesh = nullptr;
		CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject *> (m_pModelMeshResource.get());
		if (pMeshObject != nullptr) {
			pMesh = pMeshObject->getMesh ();
		}

		if (pMesh == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDMESH);

		return pMesh;
	}

	_Ret_notnull_ CModelMeshObject * CCOMModelPropertyHandler::getMeshObject()
	{
		CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject *> (m_pModelMeshResource.get());
		if (pMeshObject == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDMESH);
		return pMeshObject;
	}


	LIB3MFRESULT CCOMModelPropertyHandler::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelPropertyHandler::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelPropertyHandler::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelPropertyHandler::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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


	LIB3MFMETHODIMP CCOMModelPropertyHandler::RemoveProperty(_In_ DWORD nIndex)
	{
		try {


			CMesh * pMesh = getMesh();
			CMeshInformationHandler * pInfoHandler = pMesh->getMeshInformationHandler();
			if (pInfoHandler) {
				pInfoHandler->resetFaceInformation(nIndex);
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::RemoveAllProperties()
	{
		try {
			CMesh * pMesh = getMesh();
			pMesh->clearMeshInformationHandler();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelPropertyHandler::GetPropertyType(_In_ DWORD nIndex, _Out_ eModelPropertyType * pnPropertyType)
	{

		try {
			if (!pnPropertyType)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			*pnPropertyType = MODELPROPERTYTYPE_NONE;

			CMeshInformationHandler * pInfoHandler = pMesh->getMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiBaseMaterials);
				if (pInformation != nullptr) {
					if (pInformation->faceHasData(nIndex))
						*pnPropertyType = MODELPROPERTYTYPE_BASEMATERIALS;
				}

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiNodeColors);
				if (pInformation != nullptr) {
					if (pInformation->faceHasData(nIndex))
						*pnPropertyType = MODELPROPERTYTYPE_COLOR;
				}

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiTexCoords);
				if (pInformation != nullptr) {
					if (pInformation->faceHasData(nIndex))
						*pnPropertyType = MODELPROPERTYTYPE_TEXCOORD2D;
				}

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiCompositeMaterials);
				if (pInformation != nullptr) {
					if (pInformation->faceHasData(nIndex))
						*pnPropertyType = MODELPROPERTYTYPE_COMPOSITE;
				}

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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::GetBaseMaterial(_In_ DWORD nIndex, _Out_ DWORD * pnMaterialGroupID, _Out_ DWORD * pnMaterialIndex)
	{

		try {
			if (!pnMaterialGroupID)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (!pnMaterialIndex)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			*pnMaterialGroupID = 0;
			*pnMaterialIndex = 0;

			CMeshInformationHandler * pInfoHandler = pMesh->getMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_BaseMaterials * pBaseMaterialInformation;

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiBaseMaterials);
				if (pInformation != nullptr) {
					pBaseMaterialInformation = dynamic_cast<CMeshInformation_BaseMaterials *> (pInformation);
					if (pBaseMaterialInformation != nullptr) {
						MESHINFORMATION_BASEMATERIAL * pFaceData = (MESHINFORMATION_BASEMATERIAL *)pBaseMaterialInformation->getFaceData(nIndex);
						*pnMaterialGroupID = pFaceData->m_nMaterialGroupID;
						*pnMaterialIndex = pFaceData->m_nMaterialIndex;

					}
				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::GetBaseMaterialArray(_Out_ DWORD * pnMaterialGroupIDs, _Out_ DWORD * pnMaterialIndices)
	{

		try {
			if (!pnMaterialGroupIDs)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (!pnMaterialIndices)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			DWORD * pnGroupID = pnMaterialGroupIDs;
			DWORD * pnIndex = pnMaterialIndices;


			CMeshInformationHandler * pInfoHandler = pMesh->getMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_BaseMaterials * pBaseMaterialInformation;

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiBaseMaterials);
				if (pInformation != nullptr) {
					pBaseMaterialInformation = dynamic_cast<CMeshInformation_BaseMaterials *> (pInformation);
					if (pBaseMaterialInformation != nullptr) {
						nfUint32 nFaceIndex;
						nfUint32 nFaceCount = pMesh->getFaceCount();

						for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++) {
							MESHINFORMATION_BASEMATERIAL * pFaceData = (MESHINFORMATION_BASEMATERIAL *)pBaseMaterialInformation->getFaceData(nFaceIndex);

							*pnGroupID = pFaceData->m_nMaterialGroupID;
							*pnIndex = pFaceData->m_nMaterialIndex;
							pnGroupID++;
							pnIndex++;

						}

					}
				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetBaseMaterial(_In_ DWORD nIndex, _In_ ModelResourceID nMaterialGroupID, _In_ DWORD nMaterialIndex)
	{

		try {
			CMesh * pMesh = getMesh();
			CMeshInformationHandler * pInfoHandler = pMesh->createMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_BaseMaterials * pBaseMaterialInformation;

				// Remove all other information types
				pInfoHandler->resetFaceInformation(nIndex);

				// Get Information type and create new, if not existing
				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiBaseMaterials);
				if (pInformation == nullptr) {
					PMeshInformation pNewInformation = std::make_shared<CMeshInformation_BaseMaterials>(pMesh->getFaceCount());
					pInfoHandler->addInformation(pNewInformation);
					pInformation = pNewInformation.get();
				}

				// Set Face Data
				pBaseMaterialInformation = dynamic_cast<CMeshInformation_BaseMaterials *> (pInformation);
				if (pBaseMaterialInformation != nullptr) {
					MESHINFORMATION_BASEMATERIAL * pFaceData = (MESHINFORMATION_BASEMATERIAL *)pBaseMaterialInformation->getFaceData(nIndex);
					pFaceData->m_nMaterialGroupID = nMaterialGroupID;
					pFaceData->m_nMaterialIndex = nMaterialIndex;

				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetBaseMaterialArray(_In_ ModelResourceID * pnMaterialGroupIDs, _In_ DWORD * pnMaterialIndices)
	{

		try {
			if (!pnMaterialGroupIDs)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (!pnMaterialIndices)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			CMeshInformationHandler * pInfoHandler = pMesh->createMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_BaseMaterials * pBaseMaterialInformation;

				nfUint32 nFaceCount = pMesh->getFaceCount();

				// Get Information type and create new, if not existing
				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiBaseMaterials);
				if (pInformation == nullptr) {
					PMeshInformation pNewInformation = std::make_shared<CMeshInformation_BaseMaterials>(nFaceCount);
					pInfoHandler->addInformation(pNewInformation);
					pInformation = pNewInformation.get();
				}

				DWORD * pnGroupID = pnMaterialGroupIDs;
				DWORD * pnIndex = pnMaterialIndices;

				// Set Face Data
				pBaseMaterialInformation = dynamic_cast<CMeshInformation_BaseMaterials *> (pInformation);
				if (pBaseMaterialInformation != nullptr) {
					nfUint32 nFaceIndex;

					for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++){
						if (*pnGroupID != 0) {
							pInfoHandler->resetFaceInformation(nFaceIndex);
							MESHINFORMATION_BASEMATERIAL * pFaceData = (MESHINFORMATION_BASEMATERIAL *)pBaseMaterialInformation->getFaceData(nFaceIndex);
							pFaceData->m_nMaterialGroupID = *pnGroupID;
							pFaceData->m_nMaterialIndex = *pnIndex;

						}
						pnGroupID++;
						pnIndex++;
					}

				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::GetColor(_In_ DWORD nIndex, _Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColor)
	{

		try {
			if (!pColor)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();

			nfInt32 j;
			for (j = 0; j < 3; j++) {
				pColor->m_Colors[j].m_Red = 0;
				pColor->m_Colors[j].m_Green = 0;
				pColor->m_Colors[j].m_Blue = 0;
				pColor->m_Colors[j].m_Alpha = 0;
			}

			CMeshInformationHandler * pInfoHandler = pMesh->getMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_NodeColors * pNodeColorInformation;

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiNodeColors);
				if (pInformation != nullptr) {
					pNodeColorInformation = dynamic_cast<CMeshInformation_NodeColors *> (pInformation);
					if (pNodeColorInformation != nullptr) {
						MESHINFORMATION_NODECOLOR * pFaceData = (MESHINFORMATION_NODECOLOR *)pNodeColorInformation->getFaceData(nIndex);
						for (j = 0; j < 3; j++) {
							pColor->m_Colors[j].m_Red = pFaceData->m_cColors[j] & 0xff;
							pColor->m_Colors[j].m_Green = (pFaceData->m_cColors[j] >> 8) & 0xff;
							pColor->m_Colors[j].m_Blue = (pFaceData->m_cColors[j] >> 16) & 0xff;
							pColor->m_Colors[j].m_Alpha = (pFaceData->m_cColors[j] >> 24) & 0xff;
						}
					}
				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::GetColorArray(_Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColors)
	{

		try {
			if (!pColors)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();

			MODELMESH_TRIANGLECOLOR_SRGB * pnColor = pColors;

			CMeshInformationHandler * pInfoHandler = pMesh->getMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_NodeColors * pNodeColorInformation;

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiNodeColors);
				if (pInformation != nullptr) {
					pNodeColorInformation = dynamic_cast<CMeshInformation_NodeColors *> (pInformation);
					if (pNodeColorInformation != nullptr) {
						nfUint32 nFaceIndex;
						nfUint32 nFaceCount = pMesh->getFaceCount();

						for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++) {
							MESHINFORMATION_NODECOLOR * pFaceData = (MESHINFORMATION_NODECOLOR *)pNodeColorInformation->getFaceData(nFaceIndex);

							nfInt32 j;
							for (j = 0; j < 3; j++) {
								pnColor->m_Colors[j].m_Red = pFaceData->m_cColors[j] & 0xff;
								pnColor->m_Colors[j].m_Green = (pFaceData->m_cColors[j] >> 8) & 0xff;
								pnColor->m_Colors[j].m_Blue = (pFaceData->m_cColors[j] >> 16) & 0xff;
								pnColor->m_Colors[j].m_Alpha = (pFaceData->m_cColors[j] >> 24) & 0xff;
							}
							pnColor++;

						}

					}
				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetSingleColor(_In_ DWORD nIndex, _Out_ MODELMESHCOLOR_SRGB * pColor)
	{

		try {

			if (!pColor)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();

			CMeshInformationHandler * pInfoHandler = pMesh->createMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_NodeColors * pNodeColorInformation;

				// Remove all other information types
				pInfoHandler->resetFaceInformation(nIndex);

				// Get Information type and create new, if not existing
				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiNodeColors);
				if (pInformation == nullptr) {
					PMeshInformation pNewInformation = std::make_shared<CMeshInformation_NodeColors>(pMesh->getFaceCount());
					pInfoHandler->addInformation(pNewInformation);
					pInformation = pNewInformation.get();
				}

				// Set Face Data
				pNodeColorInformation = dynamic_cast<CMeshInformation_NodeColors *> (pInformation);
				if (pNodeColorInformation != nullptr) {
					MESHINFORMATION_NODECOLOR * pFaceData = (MESHINFORMATION_NODECOLOR *)pNodeColorInformation->getFaceData(nIndex);
					nfInt32 j;
					for (j = 0; j < 3; j++) {
						nfUint32 nRed = pColor->m_Red;
						nfUint32 nBlue = pColor->m_Blue;
						nfUint32 nGreen = pColor->m_Green;
						nfUint32 nAlpha = pColor->m_Alpha;
						pFaceData->m_cColors[j] = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
					}
				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetSingleColorRGB(_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue)
	{
		MODELMESHCOLOR_SRGB Color;
		Color.m_Red = bRed;
		Color.m_Green = bGreen;
		Color.m_Blue = bBlue;
		Color.m_Alpha = 255;
		return SetSingleColor(nIndex, &Color);
	}

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetSingleColorRGBA(_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha)
	{
		MODELMESHCOLOR_SRGB Color;
		Color.m_Red = bRed;
		Color.m_Green = bGreen;
		Color.m_Blue = bBlue;
		Color.m_Alpha = bAlpha;
		return SetSingleColor(nIndex, &Color);
	}

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetSingleColorFloatRGB(_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue)
	{
		try {
			MODELMESHCOLOR_SRGB Color;
			nfInt32 nRed = (nfInt32) round(fRed * 255.0f);
			nfInt32 nGreen = (nfInt32)round(fGreen * 255.0f);
			nfInt32 nBlue = (nfInt32)round(fBlue * 255.0f);

			if ((nRed < 0) || (nRed > 255))
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if ((nGreen < 0) || (nGreen > 255))
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if ((nBlue < 0) || (nBlue > 255))
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			Color.m_Red = nRed;
			Color.m_Green = nGreen;
			Color.m_Blue = nBlue;
			Color.m_Alpha = 255;
			return SetSingleColor(nIndex, &Color);
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetSingleColorFloatRGBA(_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha)
	{
		try {
			MODELMESHCOLOR_SRGB Color;
			nfInt32 nRed = (nfInt32)round(fRed * 255.0f);
			nfInt32 nGreen = (nfInt32)round(fGreen * 255.0f);
			nfInt32 nBlue = (nfInt32)round(fBlue * 255.0f);
			nfInt32 nAlpha = (nfInt32)round(fAlpha * 255.0f);

			if ((nRed < 0) || (nRed > 255))
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if ((nGreen < 0) || (nGreen > 255))
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if ((nBlue < 0) || (nBlue > 255))
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if ((nAlpha < 0) || (nAlpha > 255))
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			Color.m_Red = nRed;
			Color.m_Green = nGreen;
			Color.m_Blue = nBlue;
			Color.m_Alpha = nAlpha;
			return SetSingleColor(nIndex, &Color);
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetSingleColorArray(_Out_ MODELMESHCOLOR_SRGB * pColors)
	{

		try {
			if (!pColors)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();

			CMeshInformationHandler * pInfoHandler = pMesh->createMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_NodeColors * pNodeColorInformation;

				nfUint32 nFaceCount = pMesh->getFaceCount();

				// Get Information type and create new, if not existing
				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiNodeColors);
				if (pInformation == nullptr) {
					PMeshInformation pNewInformation = std::make_shared<CMeshInformation_NodeColors>(nFaceCount);
					pInfoHandler->addInformation(pNewInformation);
					pInformation = pNewInformation.get();
				}

				MODELMESHCOLOR_SRGB * pnColor = pColors;

				// Set Face Data
				pNodeColorInformation = dynamic_cast<CMeshInformation_NodeColors *> (pInformation);
				if (pNodeColorInformation != nullptr) {
					nfUint32 nFaceIndex;

					for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++){
						if ((pnColor->m_Red != 0) || (pnColor->m_Green != 0) || (pnColor->m_Blue != 0) || (pnColor->m_Alpha != 0)) {
							pInfoHandler->resetFaceInformation(nFaceIndex);
							MESHINFORMATION_NODECOLOR * pFaceData = (MESHINFORMATION_NODECOLOR *)pNodeColorInformation->getFaceData(nFaceIndex);
							nfInt32 j;
							for (j = 0; j < 3; j++) {
								nfUint32 nRed = pnColor->m_Red;
								nfUint32 nBlue = pnColor->m_Blue;
								nfUint32 nGreen = pnColor->m_Green;
								nfUint32 nAlpha = pnColor->m_Alpha;
								pFaceData->m_cColors[j] = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
							}

						}
						pnColor++;
					}

				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetGradientColor(_In_ DWORD nIndex, _Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColor)
	{

		try {
			if (!pColor)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();

			CMeshInformationHandler * pInfoHandler = pMesh->createMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_NodeColors * pNodeColorInformation;

				// Remove all other information types
				pInfoHandler->resetFaceInformation(nIndex);

				// Get Information type and create new, if not existing
				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiNodeColors);
				if (pInformation == nullptr) {
					PMeshInformation pNewInformation = std::make_shared<CMeshInformation_NodeColors>(pMesh->getFaceCount());
					pInfoHandler->addInformation(pNewInformation);
					pInformation = pNewInformation.get();
				}

				// Set Face Data
				pNodeColorInformation = dynamic_cast<CMeshInformation_NodeColors *> (pInformation);
				if (pNodeColorInformation != nullptr) {
					MESHINFORMATION_NODECOLOR * pFaceData = (MESHINFORMATION_NODECOLOR *)pNodeColorInformation->getFaceData(nIndex);
					nfInt32 j;
					for (j = 0; j < 3; j++) {
						nfUint32 nRed = pColor->m_Colors[j].m_Red;
						nfUint32 nBlue = pColor->m_Colors[j].m_Blue;
						nfUint32 nGreen = pColor->m_Colors[j].m_Green;
						nfUint32 nAlpha = pColor->m_Colors[j].m_Alpha;
						pFaceData->m_cColors[j] = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
					}
				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetGradientColorArray(_Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColors)
	{
		try {
			if (!pColors)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();

			CMeshInformationHandler * pInfoHandler = pMesh->createMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_NodeColors * pNodeColorInformation;

				nfUint32 nFaceCount = pMesh->getFaceCount();

				// Get Information type and create new, if not existing
				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiNodeColors);
				if (pInformation == nullptr) {
					PMeshInformation pNewInformation = std::make_shared<CMeshInformation_NodeColors>(nFaceCount);
					pInfoHandler->addInformation(pNewInformation);
					pInformation = pNewInformation.get();
				}

				MODELMESH_TRIANGLECOLOR_SRGB * pnColor = pColors;

				// Set Face Data
				pNodeColorInformation = dynamic_cast<CMeshInformation_NodeColors *> (pInformation);
				if (pNodeColorInformation != nullptr) {
					nfUint32 nFaceIndex;

					for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++){
						if ((pnColor->m_Colors[0].m_Red != 0) || (pnColor->m_Colors[0].m_Green != 0) || (pnColor->m_Colors[0].m_Blue != 0) || (pnColor->m_Colors[0].m_Alpha != 0) ||
							(pnColor->m_Colors[1].m_Red != 0) || (pnColor->m_Colors[1].m_Green != 0) || (pnColor->m_Colors[1].m_Blue != 0) || (pnColor->m_Colors[1].m_Alpha != 0) ||
							(pnColor->m_Colors[2].m_Red != 0) || (pnColor->m_Colors[2].m_Green != 0) || (pnColor->m_Colors[2].m_Blue != 0) || (pnColor->m_Colors[2].m_Alpha != 0) ||
							(pnColor->m_Colors[3].m_Red != 0) || (pnColor->m_Colors[3].m_Green != 0) || (pnColor->m_Colors[3].m_Blue != 0) || (pnColor->m_Colors[3].m_Alpha != 0)) {

							pInfoHandler->resetFaceInformation(nFaceIndex);
							MESHINFORMATION_NODECOLOR * pFaceData = (MESHINFORMATION_NODECOLOR *)pNodeColorInformation->getFaceData(nFaceIndex);
							nfInt32 j;
							for (j = 0; j < 3; j++) {
								nfUint32 nRed = pnColor->m_Colors[j].m_Red;
								nfUint32 nBlue = pnColor->m_Colors[j].m_Blue;
								nfUint32 nGreen = pnColor->m_Colors[j].m_Green;
								nfUint32 nAlpha = pnColor->m_Colors[j].m_Alpha;
								pFaceData->m_cColors[j] = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
							}

						}
						pnColor++;
					}

				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::GetTexture(_In_ DWORD nIndex, _Out_ MODELMESHTEXTURE2D * pTexture)
	{
		try {
			if (!pTexture)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();

			nfInt32 j;
			for (j = 0; j < 3; j++) {
				pTexture->m_fU[j] = 0.0f;
				pTexture->m_fV[j] = 0.0f;
			}
			pTexture->m_nTextureID = 0;

			CMeshInformationHandler * pInfoHandler = pMesh->getMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_TexCoords * pTexCoordsInformation;

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiTexCoords);
				if (pInformation != nullptr) {
					pTexCoordsInformation = dynamic_cast<CMeshInformation_TexCoords *> (pInformation);
					if (pTexCoordsInformation != nullptr) {
						MESHINFORMATION_TEXCOORDS * pFaceData = (MESHINFORMATION_TEXCOORDS *)pTexCoordsInformation->getFaceData(nIndex);
						for (j = 0; j < 3; j++) {
							pTexture->m_fU[j] = pFaceData->m_vCoords[j].m_fields[0];
							pTexture->m_fV[j] = pFaceData->m_vCoords[j].m_fields[1];
						}
						pTexture->m_nTextureID = pFaceData->m_TextureID;
					}
				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::GetTextureArray(_Out_ MODELMESHTEXTURE2D * pTextures)
	{
		try {
			if (!pTextures)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();

			MODELMESHTEXTURE2D * pnTexture = pTextures;

			CMeshInformationHandler * pInfoHandler = pMesh->getMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_TexCoords * pTexCoordInformation;

				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiTexCoords);
				if (pInformation != nullptr) {
					pTexCoordInformation = dynamic_cast<CMeshInformation_TexCoords *> (pInformation);
					if (pTexCoordInformation != nullptr) {
						nfUint32 nFaceIndex;
						nfUint32 nFaceCount = pMesh->getFaceCount();

						for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++) {
							MESHINFORMATION_TEXCOORDS * pFaceData = (MESHINFORMATION_TEXCOORDS *)pTexCoordInformation->getFaceData(nFaceIndex);

							nfInt32 j;
							for (j = 0; j < 3; j++) {
								pnTexture->m_fU[j] = pFaceData->m_vCoords[j].m_fields[0];
								pnTexture->m_fV[j] = pFaceData->m_vCoords[j].m_fields[1];
							}
							pnTexture->m_nTextureID = pFaceData->m_TextureID;
							pnTexture++;

						}

					}
				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetTexture(_In_ DWORD nIndex, _In_ MODELMESHTEXTURE2D * pTexture)
	{
		try {

			if (!pTexture)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();
			
			CMeshInformationHandler * pInfoHandler = pMesh->createMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_TexCoords * pTexCoordInformation;

				// Remove all other information types
				pInfoHandler->resetFaceInformation(nIndex);

				// Get Information type and create new, if not existing
				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiTexCoords);
				if (pInformation == nullptr) {
					PMeshInformation pNewInformation = std::make_shared<CMeshInformation_TexCoords>(pMesh->getFaceCount());
					pInfoHandler->addInformation(pNewInformation);
					pInformation = pNewInformation.get();
				}

				// Set Face Data
				pTexCoordInformation = dynamic_cast<CMeshInformation_TexCoords *> (pInformation);
				if (pTexCoordInformation != nullptr) {
					MESHINFORMATION_TEXCOORDS * pFaceData = (MESHINFORMATION_TEXCOORDS *)pTexCoordInformation->getFaceData(nIndex);
					nfInt32 j;
					for (j = 0; j < 3; j++) {
						pFaceData->m_vCoords[j].m_fields[0] = pTexture->m_fU[j];
						pFaceData->m_vCoords[j].m_fields[1] = pTexture->m_fV[j];
					}
					pFaceData->m_TextureID = pTexture->m_nTextureID;
				}
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

	LIB3MFMETHODIMP CCOMModelPropertyHandler::SetTextureArray(_In_ MODELMESHTEXTURE2D * pTextures)
	{
		try {
			if (!pTextures)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CMesh * pMesh = getMesh();

			CMeshInformationHandler * pInfoHandler = pMesh->createMeshInformationHandler();
			if (pInfoHandler) {
				CMeshInformation * pInformation;
				CMeshInformation_TexCoords * pTexCoordInformation;

				nfUint32 nFaceCount = pMesh->getFaceCount();

				// Get Information type and create new, if not existing
				pInformation = pInfoHandler->getInformationByType(m_nChannel, emiTexCoords);
				if (pInformation == nullptr) {
					PMeshInformation pNewInformation = std::make_shared<CMeshInformation_TexCoords>(nFaceCount);
					pInfoHandler->addInformation(pNewInformation);
					pInformation = pNewInformation.get();
				}

				MODELMESHTEXTURE2D * pnTexture = pTextures;

				// Set Face Data
				pTexCoordInformation = dynamic_cast<CMeshInformation_TexCoords *> (pInformation);
				if (pTexCoordInformation != nullptr) {
					nfUint32 nFaceIndex;

					for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++){
						if (pnTexture->m_nTextureID != 0) {

							pInfoHandler->resetFaceInformation(nFaceIndex);
							MESHINFORMATION_TEXCOORDS * pFaceData = (MESHINFORMATION_TEXCOORDS *)pTexCoordInformation->getFaceData(nFaceIndex);
							nfInt32 j;
							for (j = 0; j < 3; j++) {
								pFaceData->m_vCoords[j].m_fields[0] = pnTexture->m_fU[j];
								pFaceData->m_vCoords[j].m_fields[1] = pnTexture->m_fV[j];
							}
							pFaceData->m_TextureID = pnTexture->m_nTextureID;

						}
						pnTexture++;
					}

				}
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



}
