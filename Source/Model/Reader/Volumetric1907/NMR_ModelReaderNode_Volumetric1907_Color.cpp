/*++

Copyright (C) 2020 3MF Consortium

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
CModelReaderNode_Volumetric1907_Color.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_Color.h"
#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_ColorChannel.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric1907_Color::CModelReaderNode_Volumetric1907_Color(_In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_bHasStackId = false;
		m_bHasTransform = false;
		m_bHasRedChannel = false;
		m_bHasGreenChannel = false;
		m_bHasBlueChannel = false;
	}

	void CModelReaderNode_Volumetric1907_Color::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	PVolumeColor CModelReaderNode_Volumetric1907_Color::MakeColor(_In_ CModel* pModel)
	{
		if (!pModel)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (!m_bHasRedChannel || !m_bHasRedChannel || !m_bHasRedChannel)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (!m_bHasStackId) {
			throw CNMRException(NMR_ERROR_MISSINGVOLUMEDATASTACKID);
		}

		PPackageResourceID pID = pModel->findPackageResourceID(pModel->curPath(), m_nStackID);
		if (!pID.get()) {
			throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);
		}

		PModelVolumetricStack pStack = pModel->findVolumetricStack(pID->getUniqueID());
		if (!pStack.get()) {
			throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);
		}

		PVolumeColor pColor = std::make_shared<CVolumeColor>(pStack);
		pColor->SetChannel(eModelColorChannel::MODELCOLORCHANNEL_RED,   m_sRedChannel);
		pColor->SetChannel(eModelColorChannel::MODELCOLORCHANNEL_GREEN, m_sGreenChannel);
		pColor->SetChannel(eModelColorChannel::MODELCOLORCHANNEL_BLUE,  m_sBlueChannel);

		if (m_bHasTransform)
			pColor->SetTransform(m_Transform);
			
		return pColor;
	}

	void CModelReaderNode_Volumetric1907_Color::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMEDATA_TRANSFORM) == 0) {
			if (m_bHasTransform)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATATRANSFORM);

			m_Transform = fnMATRIX3_fromString(pAttributeValue);

			m_bHasTransform = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMEDATA_VOLUMETRICSTACKID) == 0) {
			if (m_bHasStackId)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATASTACKID);

			m_bHasStackId = true;

			m_nStackID = fnStringToUint32(pAttributeValue);
		}
	}
	
	void CModelReaderNode_Volumetric1907_Color::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_VOLUMETRICSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_VOLUMETRIC_COLOR_RED) == 0)
			{
				PModelReaderNode_Volumetric1907_ColorChannel pXMLNode = std::make_shared<CModelReaderNode_Volumetric1907_ColorChannel>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				if (m_bHasRedChannel)
					throw CNMRException(NMR_ERROR_DUPLICATECOLORVALUE);

				m_sRedChannel = pXMLNode->getChannel();
				if (m_sRedChannel == "")
					throw CNMRException(NMR_ERROR_MISSINGVOLUMETRICDSTCHANNELNAME);

				m_bHasRedChannel = true;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_VOLUMETRIC_COLOR_GREEN) == 0)
			{
				PModelReaderNode_Volumetric1907_ColorChannel pXMLNode = std::make_shared<CModelReaderNode_Volumetric1907_ColorChannel>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				if (m_bHasGreenChannel)
					throw CNMRException(NMR_ERROR_DUPLICATECOLORVALUE);

				m_sGreenChannel = pXMLNode->getChannel();
				if (m_sGreenChannel == "")
					throw CNMRException(NMR_ERROR_MISSINGVOLUMETRICDSTCHANNELNAME);

				m_bHasGreenChannel = true;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_VOLUMETRIC_COLOR_BLUE) == 0)
			{
				PModelReaderNode_Volumetric1907_ColorChannel pXMLNode = std::make_shared<CModelReaderNode_Volumetric1907_ColorChannel>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				if (m_bHasBlueChannel)
					throw CNMRException(NMR_ERROR_DUPLICATECOLORVALUE);

				m_sBlueChannel = pXMLNode->getChannel();
				if (m_sBlueChannel == "")
					throw CNMRException(NMR_ERROR_MISSINGVOLUMETRICDSTCHANNELNAME);

				m_bHasBlueChannel = true;
			}
		}
	}
}
