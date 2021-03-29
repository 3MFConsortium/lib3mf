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
NMR_ModelReaderNode_Volumetric1907_VolumetricLayer.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_VolumetricLayer.h"
#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_Image3DChannelSelector.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelMultiPropertyGroup.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric1907_VolumetricLayer::CModelReaderNode_Volumetric1907_VolumetricLayer(CModelVolumetricStack * pVolumetricStack, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings),
			m_pVolumetricStack (pVolumetricStack),
			m_bHasTransform (false),
			m_bHasDstAlpha (false),
			m_bHasSourceAlpha (false),
			m_bHasBlendMethod (false),
			m_BlendMethod (eModelBlendMethod::MODELBLENDMETHOD_MIX),
			m_dSourceAlpha (0.0),
			m_dDstAlpha (0.0)
	{

		if (pVolumetricStack == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

	}

	void CModelReaderNode_Volumetric1907_VolumetricLayer::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		if (!m_bHasBlendMethod)
			throw CNMRException(NMR_ERROR_MISSINGVOLUMETRICLAYERBLENDMETHOD);
		if (!m_bHasTransform)
			throw CNMRException(NMR_ERROR_MISSINGVOLUMETRICLAYERTRANSFORM);

		m_pVolumetricLayer = m_pVolumetricStack->addLayer(m_Transform, m_BlendMethod);
		if (m_bHasSourceAlpha)
			m_pVolumetricLayer->setSourceAlpha(m_dSourceAlpha);
		if (m_bHasDstAlpha)
			m_pVolumetricLayer->setDstAlpha(m_dDstAlpha);

		if (m_BlendMethod == eModelBlendMethod::MODELBLENDMETHOD_MIX)
		{
			if (!m_bHasSourceAlpha)
			{
				m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGVOLUMETRICSRCALPHA), mrwMissingMandatoryValue);
			}
			if (!m_bHasDstAlpha)
			{
				m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGVOLUMETRICDSTALPHA), mrwMissingMandatoryValue);
			}
		}

		// Parse Content
		parseContent(pXMLReader);
	}
	
	
	void CModelReaderNode_Volumetric1907_VolumetricLayer::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMETRICLAYER_BLENDMETHOD) == 0) {
			if (m_bHasBlendMethod)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICBLENDMETHOD);

			m_BlendMethod = MODELBLENDMETHOD_MIX;
			try {
				m_BlendMethod = CModelMultiPropertyGroupResource::stringToBlendMethod(pAttributeValue);
			}
			catch (CNMRException e) {
				m_pWarnings->addException(e, mrwInvalidMandatoryValue);
			}

			m_bHasBlendMethod = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMETRICLAYER_TRANSFORM) == 0) {
			if (m_bHasTransform)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICTRANSFORM);

			m_Transform = fnMATRIX3_fromString(pAttributeValue);			

			m_bHasTransform = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMETRICLAYER_SRCALPHA) == 0) {
			if (m_bHasSourceAlpha)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICSRCALPHA);

			m_dSourceAlpha = strtod(pAttributeValue, nullptr);
			if (isnan(m_dSourceAlpha))
				throw CNMRException(NMR_ERROR_INVALIDVOLUMETRICSRCALPHA);

			m_bHasSourceAlpha = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMETRICLAYER_DSTALPHA) == 0) {
			if (m_bHasDstAlpha)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICDSTALPHA);

			m_dDstAlpha = strtod(pAttributeValue, nullptr);
			if (isnan(m_dDstAlpha))
				throw CNMRException(NMR_ERROR_INVALIDVOLUMETRICDSTALPHA);

			m_bHasDstAlpha = true;
		}

	}
	
	void CModelReaderNode_Volumetric1907_VolumetricLayer::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_VOLUMETRICSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_MASKINGCHANNELSELECTOR) == 0)
			{
				__NMRASSERT(m_pVolumetricLayer.get() != nullptr);
				if (m_pVolumetricLayer->getMaskChannelSelector().get() != nullptr)
					throw CNMRException(NMR_ERROR_DUPLICATEMASKCHANNELSELECTOR);

				PModelReaderNode_Volumetric1907_Image3DChannelSelector pXMLNode = std::make_shared<CModelReaderNode_Volumetric1907_Image3DChannelSelector>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				m_pVolumetricLayer->setMaskChannelSelector(pXMLNode->createSelector (m_pVolumetricStack->getModel()));
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_SOURCECHANNELSELECTOR) == 0)
			{
				__NMRASSERT(m_pVolumetricLayer.get() != nullptr);

				PModelReaderNode_Volumetric1907_Image3DChannelSelector pXMLNode = std::make_shared<CModelReaderNode_Volumetric1907_Image3DChannelSelector>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				m_pVolumetricLayer->addChannelSelector(pXMLNode->createSelector(m_pVolumetricStack->getModel ()));
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}




}
