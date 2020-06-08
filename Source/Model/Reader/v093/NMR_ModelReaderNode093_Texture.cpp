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

NMR_ModelReaderNode093_Texture.cpp implements the Model Reader Texture Class.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Texture.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelTexture2D.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode093_Texture::CModelReaderNode093_Texture(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		// Initialize variables
		m_nID = 0;

		m_pModel = pModel;
	}

	void CModelReaderNode093_Texture::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Use parameter and assign to model Object
		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);


		PModelAttachment pAttachment = m_pModel->findModelAttachment(m_sPath);
		m_pTexture2DResource = CModelTexture2DResource::make(m_nID, m_pModel, pAttachment);
		m_pModel->addResource(m_pTexture2DResource);

		m_pTexture2DResource->setContentTypeString(m_sContentType, true);

		if (!m_sTileStyleU.empty()) {
			try {
				m_pTexture2DResource->setTileStyleUString(m_sTileStyleU);
			}
			catch (CNMRException & e) {
				if (e.getErrorCode() == NMR_ERROR_INVALIDTILESTYLE)
					m_pWarnings->addException(e, mrwInvalidOptionalValue);
				else
					throw e;
			}
		}
		if (!m_sTileStyleV.empty()) {
			try {
				m_pTexture2DResource->setTileStyleVString(m_sTileStyleV);
			}
			catch (CNMRException & e) {
				if (e.getErrorCode() == NMR_ERROR_INVALIDTILESTYLE)
					m_pWarnings->addException(e, mrwInvalidOptionalValue);
				else
					throw e;
			}
		}

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode093_Texture::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTURE2D_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
			// Increase ID to avoid 0 (was allowed for 0.9.3)
			m_nID++;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTURE2D_PATH) == 0) {
			m_sPath = std::string(pAttributeValue);
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTURE2D_CONTENTTYPE) == 0) {
			m_sContentType = std::string(pAttributeValue);
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEU) == 0) {
			m_sTileStyleU = std::string(pAttributeValue);
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEV) == 0) {
			m_sTileStyleV = std::string(pAttributeValue);
		}

	}

	ModelResourceID CModelReaderNode093_Texture::getID()
	{
		return m_nID;
	}

	std::string CModelReaderNode093_Texture::getPath()
	{
		return m_sPath;
	}

	std::string CModelReaderNode093_Texture::getContentType()
	{
		return m_sContentType;
	}

	std::string CModelReaderNode093_Texture::getTileStyleU()
	{
		return m_sTileStyleU;
	}

	std::string CModelReaderNode093_Texture::getTileStyleV()
	{
		return m_sTileStyleV;
	}

}

