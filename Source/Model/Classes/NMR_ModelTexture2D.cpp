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

NMR_ModelTexture2D.cpp implements the Model Texture Class.

--*/

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelTexture2D.h"
#include "Model/Classes/NMR_ModelTextureAttachment.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {

	CModelTexture2DResource::CModelTexture2DResource(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		:CModelResource(sID, pModel)
	{
		m_sPath = "";
		m_ContentType = MODELTEXTURETYPE_UNKNOWN;
		m_bHasBox2D = false;
		m_fBox2D_U = 0.0f;
		m_fBox2D_V = 0.0f;
		m_fBox2D_Width = 1.0f;
		m_fBox2D_Height = 1.0f;
		m_eTileStyleU = MODELTEXTURETILESTYLE_WRAP;
		m_eTileStyleV = MODELTEXTURETILESTYLE_WRAP;
	}

		// getters/setters Path
	std::string CModelTexture2DResource::getPath()
	{
		return m_sPath;
	}

	void CModelTexture2DResource::setPath(_In_ std::string sPath)
	{
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

	std::string CModelTexture2DResource::getContentTypeString()
	{
		switch (m_ContentType) {
		case MODELTEXTURETYPE_PNG: return PACKAGE_PNG_CONTENT_TYPE;
		case MODELTEXTURETYPE_JPEG: return PACKAGE_JPG_CONTENT_TYPE;
		default: return "";
		}

	}

	void CModelTexture2DResource::setContentTypeString(_In_ std::string sValue, _In_ nfBool bFailIfUnknown)
	{
		std::transform(sValue.begin(), sValue.end(), sValue.begin(), ::tolower);

		if (strcmp(sValue.c_str(), PACKAGE_PNG_CONTENT_TYPE) == 0) {
			m_ContentType = MODELTEXTURETYPE_PNG;
			return;
		}

		if (strcmp(sValue.c_str(), PACKAGE_JPG_CONTENT_TYPE) == 0) {
			m_ContentType = MODELTEXTURETYPE_JPEG;
			return;
		}

		if (bFailIfUnknown)
			throw CNMRException(NMR_ERROR_INVALIDCONTENTTYPE);

		m_ContentType = MODELTEXTURETYPE_UNKNOWN;

	}

	
	void CModelTexture2DResource::setTileStyleUString(_In_ std::string sValue)
	{
		m_eTileStyleU = CModelTexture2DResource::tileStyleFromString(sValue);
	}

	void CModelTexture2DResource::setTileStyleVString(_In_ std::string sValue)
	{
		m_eTileStyleV = CModelTexture2DResource::tileStyleFromString(sValue);
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
	eModelTextureTileStyle CModelTexture2DResource::getTileStyleU()
	{
		return m_eTileStyleU;
	}

	eModelTextureTileStyle CModelTexture2DResource::getTileStyleV()
	{
		return m_eTileStyleV;
	}

	void CModelTexture2DResource::setTileStyleU(_In_ eModelTextureTileStyle sStyle)
	{
		m_eTileStyleU = sStyle;
	}

	void CModelTexture2DResource::setTileStyleV(_In_ eModelTextureTileStyle sStyle)
	{
		m_eTileStyleV = sStyle;
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

	eModelTextureTileStyle CModelTexture2DResource::tileStyleFromString(_In_ std::string sValue)
	{
		std::transform(sValue.begin(), sValue.end(), sValue.begin(), ::tolower);
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_CLAMP) == 0)
			return MODELTEXTURETILESTYLE_CLAMP;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_WRAP) == 0)
			return MODELTEXTURETILESTYLE_WRAP;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_MIRROR) == 0)
			return MODELTEXTURETILESTYLE_MIRROR;
		throw CNMRException(NMR_ERROR_INVALIDTILESTYLE);
		return MODELTEXTURETILESTYLE_WRAP;
	}

	std::string CModelTexture2DResource::tileStyleToString(_In_ eModelTextureTileStyle eTileStyle)
	{
		switch (eTileStyle) {
			case MODELTEXTURETILESTYLE_MIRROR: return XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_MIRROR;
			case MODELTEXTURETILESTYLE_CLAMP: return XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_CLAMP;
			default:
				return XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_WRAP;
		}
	}

}
