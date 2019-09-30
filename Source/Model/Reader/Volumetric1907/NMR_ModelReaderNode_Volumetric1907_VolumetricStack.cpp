/*++

Copyright (C) 2019 3MF Consortium

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
NMR_ModelReaderNode_Volumetric1907_VolumetricStack.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_VolumetricStack.h"
#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_VolumetricDstChannel.h"
#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_VolumetricLayer.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric1907_VolumetricStack::CModelReaderNode_Volumetric1907_VolumetricStack(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings),
			m_pModel (pModel),
			m_nID (0)
	
	{
		if (pModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

	}

	void CModelReaderNode_Volumetric1907_VolumetricStack::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		m_pVolumetricStack = CModelVolumetricStack::make(m_nID, m_pModel);

		// Parse Content
		parseContent(pXMLReader);

		m_pModel->addResource(m_pVolumetricStack);

	}
	
	
	void CModelReaderNode_Volumetric1907_VolumetricStack::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMETRICSTACK_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
		}

	}
	
	void CModelReaderNode_Volumetric1907_VolumetricStack::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_VOLUMETRICSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_DSTCHANNEL) == 0)
			{
				PModelReaderNode_Volumetric1907_VolumetricDstChannel pXMLNode = std::make_shared<CModelReaderNode_Volumetric1907_VolumetricDstChannel>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				std::string sName = pXMLNode->getName();
				if (sName == "")
					throw CNMRException(NMR_ERROR_MISSINGVOLUMETRICDSTCHANNELNAME);

				__NMRASSERT(m_pVolumetricStack.get() != nullptr);
				m_pVolumetricStack->addDstChannel(sName, pXMLNode->getBackground());

			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_VOLUMETRICLAYER) == 0)
			{
				__NMRASSERT(m_pVolumetricStack.get() != nullptr);

				PModelReaderNode_Volumetric1907_VolumetricLayer pXMLNode = std::make_shared<CModelReaderNode_Volumetric1907_VolumetricLayer>(m_pVolumetricStack.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}




}
