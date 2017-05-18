/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_ModelTexture2D.cpp implements the Model Texture Class.

--*/

#include "Model/Classes/NMR_ModelTexture2D.h"
#include "Model/Classes/NMR_ModelTextureAttachment.h"
#include "Common/NMR_Exception.h"
#include <sstream>
#include <algorithm>

namespace NMR {

	CModelTexture2DResource::CModelTexture2DResource(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		:CModelResource(sID, pModel)
	{
		m_sPath = L"";
		m_ContentType = MODELTEXTURETYPE_UNKNOWN;
		m_bHasBox2D = false;
		m_fBox2D_U = 0.0f;
		m_fBox2D_V = 0.0f;
		m_fBox2D_Width = 1.0f;
		m_fBox2D_Height = 1.0f;
		m_sTileStyleU = L"";
		m_sTileStyleV = L"";
	}

		// getters/setters Path
	std::wstring CModelTexture2DResource::getPath()
	{
		return m_sPath;
	}

	void CModelTexture2DResource::setPath(_In_ std::wstring sPath)
	{
		// throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		m_sPath = sPath;
	}

	PImportStream CModelTexture2DResource::getTextureStream()
	{
		CModel * pModel = getModel();
		__NMRASSERT(pModel != nullptr);
		PModelAttachment pAttachment = pModel->findModelAttachment(m_sPath);
		if (pAttachment) 
			return pAttachment->getStream();
		return nullptr;
	}


	// getters/setters ContentType
	eModelTexture2DType CModelTexture2DResource::getContentType()
	{
		return m_ContentType;
	}

	void CModelTexture2DResource::setContentType(_In_ eModelTexture2DType ContentType)
	{
		m_ContentType = ContentType;
	}

	std::wstring CModelTexture2DResource::getContentTypeString()
	{
		switch (m_ContentType) {
		case MODELTEXTURETYPE_PNG: return L"image/png";
		case MODELTEXTURETYPE_JPEG: return L"image/jpeg";
		default: return L"";
		}

	}

	void CModelTexture2DResource::setContentTypeString(_In_ std::wstring sValue, _In_ nfBool bFailIfUnknown)
	{
        std::transform(sValue.begin(), sValue.end(), sValue.begin(), ::tolower);

		if (wcscmp(sValue.c_str(), L"image/png") == 0) {
			m_ContentType = MODELTEXTURETYPE_PNG;
			return;
		}

		if (wcscmp(sValue.c_str(), L"image/jpeg") == 0) {
			m_ContentType = MODELTEXTURETYPE_JPEG;
			return;
		}

		if (bFailIfUnknown)
			throw CNMRException(NMR_ERROR_INVALIDCONTENTTYPE);

		m_ContentType = MODELTEXTURETYPE_UNKNOWN;

	}


	// getters/setters Box2D
	nfBool CModelTexture2DResource::getBox2D(_Out_ nfFloat & fU, _Out_ nfFloat & fV, _Out_ nfFloat & fWidth, _Out_ nfFloat & fHeight)
	{
		if (m_bHasBox2D) {
			fU = m_fBox2D_U;
			fV = m_fBox2D_V;
			fWidth = m_fBox2D_Width;
			fHeight = m_fBox2D_Height;
			return true;
		}
		else {
			fU = 0.0f;
			fV = 0.0f;
			fWidth = 1.0f;
			fHeight = 1.0f;
			return false;
		}
	}

	void CModelTexture2DResource::setBox2D(_In_ nfFloat fU, _In_ nfFloat fV, _In_ nfFloat fWidth, _In_ nfFloat fHeight)
	{
		m_bHasBox2D = true;
		m_fBox2D_U = fU;
		m_fBox2D_V = fV;
		m_fBox2D_Width = fWidth;
		m_fBox2D_Height = fHeight;
	}

	void CModelTexture2DResource::clearBox2D()
	{
		m_bHasBox2D = false;
		m_fBox2D_U = 0.0f;
		m_fBox2D_V = 0.0f;
		m_fBox2D_Width = 1.0f;
		m_fBox2D_Height = 1.0f;
	}

	nfBool CModelTexture2DResource::hasBox2D()
	{
		return m_bHasBox2D;
	}

	// getters/setters TileStyle
	std::wstring CModelTexture2DResource::getTileStyleU()
	{
		return m_sTileStyleU;
	}

	std::wstring CModelTexture2DResource::getTileStyleV()
	{
		return m_sTileStyleV;
	}

	void CModelTexture2DResource::setTileStyleU(_In_ std::wstring sValue)
	{
		m_sTileStyleU = sValue;
	}

	void CModelTexture2DResource::setTileStyleV(_In_ std::wstring sValue)
	{
		m_sTileStyleV = sValue;
	}

	void CModelTexture2DResource::copyFrom(_In_ CModelTexture2DResource * pSourceTexture)
	{
		if (pSourceTexture == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		setPath(pSourceTexture->getPath());
		setContentType(pSourceTexture->getContentType ());
		setTileStyleU(pSourceTexture->getTileStyleU());
		setTileStyleV(pSourceTexture->getTileStyleV());
		if (pSourceTexture->hasBox2D()) {
			nfFloat fU, fV, fWidth, fHeight;
			pSourceTexture->getBox2D(fU, fV, fWidth, fHeight);
			setBox2D(fU, fV, fWidth, fHeight);
		}
		else {
			clearBox2D();
		}
	}

}
