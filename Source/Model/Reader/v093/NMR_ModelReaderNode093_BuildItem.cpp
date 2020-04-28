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

NMR_ModelReaderNode093_BuildItem.cpp implements the Model Reader BuildItem Node Class.
A builditem reader model node is a parser for the builditem node of an XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_BuildItem.h"
#include "Model/Classes/NMR_ModelBuildItem.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelReaderNode093_BuildItem::CModelReaderNode093_BuildItem(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pModel);
		m_pModel = pModel;
		m_ObjectID = 0;
		m_bHasID = false;

		m_mTransform = fnMATRIX3_identity();
	}

	void CModelReaderNode093_BuildItem::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		if (!m_bHasID)
			throw CNMRException(NMR_ERROR_MISSINGBUILDITEMOBJECTID);

		PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->currentPath(), m_ObjectID);
		if (!pID.get())
			throw CNMRException(NMR_ERROR_COULDNOTFINDBUILDITEMOBJECT);
		CModelObject * pObject = m_pModel->findObject(pID->getUniqueID());
		if (!pObject)
			throw CNMRException(NMR_ERROR_COULDNOTFINDBUILDITEMOBJECT);
		

		// Create Build Item
		PModelBuildItem pBuildItem = std::make_shared<CModelBuildItem>(pObject, m_mTransform, m_pModel->createHandle());
		m_pModel->addBuildItem(pBuildItem);
	}

	void CModelReaderNode093_BuildItem::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_ITEM_OBJECTID) == 0) {
			if (m_bHasID)
				throw CNMRException(NMR_ERROR_DUPLICATEBUILDITEMOBJECTID);

			m_ObjectID = fnStringToUint32(pAttributeValue);
			m_ObjectID++;
			m_bHasID = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_ITEM_TRANSFORM) == 0) {
			m_mTransform = fnMATRIX3_fromString(pAttributeValue);
		}
	}


}
