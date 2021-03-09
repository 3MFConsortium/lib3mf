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
NMR_ModelReaderNode093_Resources.cpp implements the Model Reader Resources Node
Class. A resources reader model node is a parser for the resources node of an
XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Resources.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Object.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Color.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Material.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Texture.h"

#include "Model/Reader/NMR_ModelReader_ColorMapping.h"
#include "Model/Reader/NMR_ModelReader_TexCoordMapping.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelTexture2D.h"
#include "Model/Classes/NMR_ModelBaseMaterials.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode093_Resources::CModelReaderNode093_Resources(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pModel);
		m_pModel = pModel;

		m_pColorMapping = std::make_shared<CModelReader_ColorMapping>();

	}

	void CModelReaderNode093_Resources::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode093_Resources::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	void CModelReaderNode093_Resources::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if ((strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC093) == 0) || (strcmp(pNameSpace, "") == 0)) {

			if (strcmp(pChildName, XML_3MF_ELEMENT_OBJECT) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Object>(m_pModel, m_pColorMapping, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_COLOR) == 0) {
				PModelReaderNode093_Color pXMLNode = std::make_shared<CModelReaderNode093_Color>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				ModelResourceID nResourceID = pXMLNode->retrieveID();
				ModelResourceID nTextureRefID = pXMLNode->retrieveTextureID();

				if (nTextureRefID > 0) {
					m_pColorMapping->registerTextureReference(nResourceID, nTextureRefID);
				}
				else {
					m_pColorMapping->registerColor(nResourceID, 0, pXMLNode->retrieveColor());
				}
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_TEXTURE) == 0) {
				PModelReaderNode093_Texture pXMLNode = std::make_shared<CModelReaderNode093_Texture>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_MATERIAL) == 0) {
				PModelReaderNode093_Material pXMLNode = std::make_shared<CModelReaderNode093_Material>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				ModelResourceID nResourceID = pXMLNode->retrieveID();
				auto pMaterialResource = std::make_shared<CModelBaseMaterialResource>(nResourceID, m_pModel);
				m_pModel->addResource(pMaterialResource);

				nfColor cColor;
				if (!m_pColorMapping->findColor(pXMLNode->retrieveColorID(), 0, cColor)) {
					cColor = 0xffffffff;
				}

				ModelResourceIndex nMaterialIndex;
				nMaterialIndex = pMaterialResource->addBaseMaterial(pXMLNode->retrieveName(), cColor);
				m_pColorMapping->registerMaterialReference(pXMLNode->retrieveID(), nMaterialIndex);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}


}
