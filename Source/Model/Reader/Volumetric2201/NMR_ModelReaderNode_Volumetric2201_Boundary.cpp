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
CModelReaderNode_Volumetric2201_Boundary.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_Boundary.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric2201_Boundary::CModelReaderNode_Volumetric2201_Boundary(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_bHasFieldID = false;
		m_bHasSolidThreshold = false;
		m_bHasTransform = false;
	}

	void CModelReaderNode_Volumetric2201_Boundary::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}
	
	PVolumeDataBoundary CModelReaderNode_Volumetric2201_Boundary::MakeLevelset(_In_ CModel* pModel)
	{
		if (!pModel)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (!m_bHasFieldID) {
			throw CNMRException(NMR_ERROR_MISSINGVOLUMEDATAFIELDID);
		}

		PPackageResourceID pID = pModel->findPackageResourceID(pModel->currentPath(), m_nFieldID);
		if (!pID.get()) {
			throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);
		}
		
		auto pScalarField = pModel->findScalarField(pID);
		if (!pScalarField.get()) {
			throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
		}

		PVolumeDataBoundary pBoundary = std::make_shared<CVolumeDataBoundary>(pScalarField);
		if (m_bHasSolidThreshold)
			pBoundary->SetSolidThreshold(m_dSolidThreshold);
		if (m_bHasTransform)
			pBoundary->setTransform(m_Transform);
		return pBoundary;
	}
	
	void CModelReaderNode_Volumetric2201_Boundary::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMEDATA_TRANSFORM) == 0) {
			if (m_bHasTransform)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATATRANSFORM);

			m_Transform = fnMATRIX3_fromString(pAttributeValue);

			m_bHasTransform = true;
		} else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMEDATA_SOLIDTHRESHOLD) == 0) {
			if (m_bHasSolidThreshold)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATASOLIDTHRESHOLD);

			m_bHasSolidThreshold = true;

			m_dSolidThreshold = strtod(pAttributeValue, nullptr);

#ifdef __MINGW32__
			if (isNotANumber((float)m_dSolidThreshold))
#else
			if (std::isnan((float)m_dSolidThreshold))
#endif
				throw CNMRException(NMR_ERROR_INVALIDVOLUMEDATASOLIDTHRESHOLD);
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VOLUMEDATA_FIELDID) == 0) {
			if (m_bHasFieldID)
				throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATAFIELDID);

			m_bHasFieldID = true;

			m_nFieldID = fnStringToUint32(pAttributeValue);
		}
	}
	
	void CModelReaderNode_Volumetric2201_Boundary::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

	}




}
