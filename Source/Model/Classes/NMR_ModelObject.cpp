/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_ModelObject.cpp implements the Model Object Class.
A model object is an in memory representation of the 3MF model object.

--*/

#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	CModelObject::CModelObject(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelResource(sID, pModel)
	{
		m_ObjectType = MODELOBJECTTYPE_MODEL;
	}

	void CModelObject::mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix)
	{
		// empty on purpose, to be implemented by child classes
	}

	void CModelObject::mergeToMesh(_In_ CMesh * pMesh)
	{
		mergeToMesh(pMesh, fnMATRIX3_identity());
	}

	eModelObjectType CModelObject::getObjectType()
	{
		return m_ObjectType;
	}

	void CModelObject::setObjectType(_In_ eModelObjectType ObjectType)
	{
		m_ObjectType = ObjectType;
	}

	std::wstring CModelObject::getName()
	{
		return m_sName;
	}

	void CModelObject::setName(_In_ std::wstring sName)
	{
		m_sName = sName;
	}

	std::wstring CModelObject::getPartNumber()
	{
		return m_sPartNumber;
	}

	void CModelObject::setPartNumber(_In_ std::wstring sPartNumber)
	{
		m_sPartNumber = sPartNumber;
	}

	nfBool CModelObject::setObjectTypeString(_In_ std::wstring sTypeString, _In_ nfBool bRaiseException)
	{
		if (sTypeString == XML_3MF_OBJECTTYPE_OTHER) {
			m_ObjectType = MODELOBJECTTYPE_OTHER;
			return true;
		}
		if (sTypeString == XML_3MF_OBJECTTYPE_MODEL) {
			m_ObjectType = MODELOBJECTTYPE_MODEL;
			return true;
		}
		if (sTypeString == XML_3MF_OBJECTTYPE_SUPPORT) {
			m_ObjectType = MODELOBJECTTYPE_SUPPORT;
			return true;
		}

		if (bRaiseException)
			throw CNMRException(NMR_ERROR_INVALIDMODELOBJECTTYPE);

		return false;
	}

	std::wstring CModelObject::getObjectTypeString()
	{
		switch (m_ObjectType) {
		case MODELOBJECTTYPE_OTHER:
			return std::wstring(XML_3MF_OBJECTTYPE_OTHER);
		case MODELOBJECTTYPE_MODEL:
			return std::wstring(XML_3MF_OBJECTTYPE_MODEL);
		case MODELOBJECTTYPE_SUPPORT:
			return std::wstring(XML_3MF_OBJECTTYPE_SUPPORT);
        default:
            return L"";
		}
	}
	
}
