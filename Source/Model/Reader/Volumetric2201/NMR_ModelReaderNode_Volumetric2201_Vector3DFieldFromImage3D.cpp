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
NMR_ModelReaderNode_Volumetric2201_Vector3DFieldFromImage3D.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Volumetric2201_Vector3DFieldFromImage3D.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelTexture2D.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric2201_Vector3DFieldFromImage3D::CModelReaderNode_Volumetric2201_Vector3DFieldFromImage3D(_In_ CModel* pModel, _In_ CModelVector3DFieldFromImage3D* pVector3DFieldFromImage3D, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings),
		m_pModel(pModel),
		m_pVector3DFieldFromImage3D(pVector3DFieldFromImage3D)
	{
		if (pModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_pVector3DFieldFromImage3D == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	void CModelReaderNode_Volumetric2201_Vector3DFieldFromImage3D::parseXML(_In_ CXmlReader* pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}


	void CModelReaderNode_Volumetric2201_Vector3DFieldFromImage3D::OnAttribute(_In_z_ const nfChar* pAttributeName, _In_z_ const nfChar* pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VECTOR3DFIELDFROMIMAGE3D_IMAGE3DID) == 0) {
			if (m_bHasImage3DID)
				throw CNMRException(-1); // TODO NMR_ERROR_DUPLICATEIMAGE3DID
			PPackageResourceID pImage3DID = m_pModel->findPackageResourceID(m_pModel->currentPath(), fnStringToUint32(pAttributeValue));
			m_pVector3DFieldFromImage3D->setImage3DResourceID(pImage3DID);
			m_bHasImage3DID = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VECTOR3DFIELDFROMIMAGE3_OFFSET) == 0) {
			if (m_bHasOffset)
				throw CNMRException(-1); // TODO NMR_ERROR_DUPLICATE....
			m_pVector3DFieldFromImage3D->setOffset(fnStringToDouble(pAttributeValue));
			m_bHasOffset = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VECTOR3DFIELDFROMIMAGE3_SCALE) == 0) {
			if (m_bHasScale)
				throw CNMRException(-1); // TODO NMR_ERROR_DUPLICATE....
			m_pVector3DFieldFromImage3D->setScale(fnStringToDouble(pAttributeValue));
			m_bHasScale = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VECTOR3DFIELDFROMIMAGE3_TILESTYLEU) == 0) {
			if (m_bHasTileStyleU)
				throw CNMRException(-1); // TODO NMR_ERROR_DUPLICATE....
			m_pVector3DFieldFromImage3D->setTileStyleU(CModelTexture2DResource::tileStyleFromString(pAttributeValue));
			m_bHasTileStyleU = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VECTOR3DFIELDFROMIMAGE3_TILESTYLEV) == 0) {
			if (m_bHasTileStyleV)
				throw CNMRException(-1); // TODO NMR_ERROR_DUPLICATE....
			m_pVector3DFieldFromImage3D->setTileStyleV(CModelTexture2DResource::tileStyleFromString(pAttributeValue));
			m_bHasTileStyleV = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VECTOR3DFIELDFROMIMAGE3_TILESTYLEW) == 0) {
			if (m_bHasTileStyleW)
				throw CNMRException(-1); // TODO NMR_ERROR_DUPLICATE....
			m_pVector3DFieldFromImage3D->setTileStyleW(CModelTexture2DResource::tileStyleFromString(pAttributeValue));
			m_bHasTileStyleW = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTURE2D_FILTER) == 0) {
			if (m_bHasFilter)
				throw CNMRException(-1); // TODO NMR_ERROR_DUPLICATE....
			m_pVector3DFieldFromImage3D->setFilter(CModelTexture2DResource::filterFromString(pAttributeValue));
			m_bHasFilter = true;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_Volumetric2201_Vector3DFieldFromImage3D::OnNSChildElement(_In_z_ const nfChar* pChildName, _In_z_ const nfChar* pNameSpace, _In_ CXmlReader* pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_VOLUMETRICSPEC) == 0) {
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}


}
