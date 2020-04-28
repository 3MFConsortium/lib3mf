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

NMR_ModelReaderNode093_Component.cpp implements the Model Component Reader
Node Class.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Component.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/Math/NMR_Matrix.h"

namespace NMR {

	CModelReaderNode093_Component::CModelReaderNode093_Component(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pModel);
		m_pModel = pModel;
		m_ObjectID = 0;
		m_bHasID = false;
		m_mTransform = fnMATRIX3_identity();
	}

	void CModelReaderNode093_Component::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode093_Component::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_COMPONENT_OBJECTID) == 0) {
			if (m_bHasID)
				throw CNMRException(NMR_ERROR_DUPLICATECOMPONENTOBJECTID);

			m_ObjectID = fnStringToUint32(pAttributeValue);
			m_ObjectID++;
			m_bHasID = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_COMPONENT_TRANSFORM) == 0) {
			m_mTransform = fnMATRIX3_fromString(pAttributeValue);
		}
	}

	CModelObject * CModelReaderNode093_Component::getObject()
	{
		if (!m_bHasID)
			throw CNMRException(NMR_ERROR_MISSINGMODELOBJECTID);

		PPackageResourceID pRID = m_pModel->findPackageResourceID(m_pModel->currentPath(), m_ObjectID);
		if (pRID.get())
			return m_pModel->findObject(pRID->getUniqueID());
		return nullptr;
	}

	NMATRIX3 CModelReaderNode093_Component::getTransform()
	{
		return m_mTransform;
	}

}
