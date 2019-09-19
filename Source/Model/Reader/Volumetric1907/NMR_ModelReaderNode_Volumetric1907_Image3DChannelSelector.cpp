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
NMR_ModelReaderNode_Volumetric1907_Image3DChannelSelector.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_Image3DChannelSelector.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric1907_Image3DChannelSelector::CModelReaderNode_Volumetric1907_Image3DChannelSelector(_In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings),
		m_nID(0),
		m_dMinValue(0.0),
		m_dMaxValue(1.0),
		m_eTileStyleU(MODELTEXTURETILESTYLE_WRAP),
		m_eTileStyleV(MODELTEXTURETILESTYLE_WRAP),
		m_eTileStyleW(MODELTEXTURETILESTYLE_WRAP),
		m_eFilter(MODELTEXTUREFILTER_LINEAR),
		m_bHasSourceChannel(false),
		m_bHasDestinationChannel(false),
		m_bHasMinValue(false),
		m_bHasMaxValue(false),
		m_bHasTileStyleU(false),
		m_bHasTileStyleV(false),
		m_bHasTileStyleW(false),
		m_bHasFilter(false)
	{
	}

	void CModelReaderNode_Volumetric1907_Image3DChannelSelector::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}
	
	
	void CModelReaderNode_Volumetric1907_Image3DChannelSelector::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_CHANNELSELECTOR_IMAGE3DID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
		}
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_CHANNELSELECTOR_MINVALUE) == 0) {
			if (m_bHasMinValue)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICMINVALUE);

			m_dMinValue = strtod(pAttributeValue, nullptr);
			if (std::isnan(m_dMinValue))
				throw CNMRException(NMR_ERROR_INVALIDVOLUMETRICMINVALUE);

			m_bHasMinValue = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_CHANNELSELECTOR_MAXVALUE) == 0) {
			if (m_bHasMaxValue)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICMAXVALUE);

			m_dMaxValue = strtod(pAttributeValue, nullptr);
			if (std::isnan(m_dMaxValue))
				throw CNMRException(NMR_ERROR_INVALIDVOLUMETRICMAXVALUE);

			m_bHasMaxValue = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_CHANNELSELECTOR_FILTER) == 0) {
			if (m_bHasFilter)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICFILTER);

			m_eFilter = CModelTexture2DResource::filterFromString(pAttributeValue);

			m_bHasFilter = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_CHANNELSELECTOR_TILESTYLEU) == 0) {
			if (m_bHasTileStyleU)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICTILESTYLE);

			m_eTileStyleU = CModelTexture2DResource::tileStyleFromString(pAttributeValue);

			m_bHasTileStyleU = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_CHANNELSELECTOR_TILESTYLEV) == 0) {
			if (m_bHasTileStyleV)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICTILESTYLE);

			m_eTileStyleV = CModelTexture2DResource::tileStyleFromString(pAttributeValue);

			m_bHasTileStyleV = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_CHANNELSELECTOR_TILESTYLEW) == 0) {
			if (m_bHasTileStyleW)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICTILESTYLE);

			m_eTileStyleW = CModelTexture2DResource::tileStyleFromString(pAttributeValue);

			m_bHasTileStyleW = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_CHANNELSELECTOR_SRCCHANNEL) == 0) {
			if (m_bHasSourceChannel)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICTILESTYLE);

			m_sSourceChannel = pAttributeValue;
			m_bHasSourceChannel = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_CHANNELSELECTOR_DSTCHANNEL) == 0) {
			if (m_bHasDestinationChannel)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICTILESTYLE);

			m_sDestinationChannel = pAttributeValue;

			m_bHasDestinationChannel = true;
		}

	}
	
	
	PModelImage3DChannelSelector CModelReaderNode_Volumetric1907_Image3DChannelSelector::createSelector(CModel * pModel)
	{
		if (pModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGCHANNELSELECTORID);

		PPackageResourceID pImage3DID = pModel->findPackageResourceID(pModel->curPath(), m_nID);
		if (pImage3DID.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);

		PModelImage3DChannelSelector pSelector = CModelImage3DChannelSelector::make(pImage3DID, m_sSourceChannel, m_sDestinationChannel);

		pSelector->setMinValue(m_dMinValue);
		pSelector->setMaxValue(m_dMaxValue);

		if ((!m_bHasTileStyleU) || (!m_bHasTileStyleV) || (!m_bHasTileStyleW))
			throw CNMRException(NMR_ERROR_MISSINGCHANNELSELECTORTILESTYLE);

		pSelector->setTileStyleU(m_eTileStyleU);
		pSelector->setTileStyleV(m_eTileStyleV);
		pSelector->setTileStyleW(m_eTileStyleW);

		if (m_bHasFilter)
			pSelector->setFilter(m_eFilter);

		return pSelector;

	}


}
