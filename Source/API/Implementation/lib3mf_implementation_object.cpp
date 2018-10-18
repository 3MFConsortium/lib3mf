/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CInternalLib3MFObject
Interface version: 1.2.2

*/

#include "lib3mf_implementation_object.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF;

/*************************************************************************************************************************
 Class definition of CInternalLib3MFObject 
**************************************************************************************************************************/

CInternalLib3MFObject::CInternalLib3MFObject(CInternalLib3MFModel & model, NMR::PModelObject pModelObject)
{
	m_pModelObject = pModelObject;
	model.model().addResource(m_pModelObject);
}

NMR::CModelObject& CInternalLib3MFObject::getModelObject()
{
	if (!m_pModelObject.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);
	return *m_pModelObject.get();
};

unsigned int CInternalLib3MFObject::GetResourceID()
{
	return getModelObject().getResourceID()->getUniqueID();
}

eLib3MFObjectType CInternalLib3MFObject::GetType()
{
	switch (m_pModelObject->getObjectType()) {
	case NMR::eModelObjectType::MODELOBJECTTYPE_OTHER:
		return eLib3MFObjectType::eObjectTypeOther;
	case NMR::eModelObjectType::MODELOBJECTTYPE_MODEL:
		return eLib3MFObjectType::eObjectTypeModel;
	case NMR::eModelObjectType::MODELOBJECTTYPE_SUPPORT:
		return eLib3MFObjectType::eObjectTypeSupport;
	case NMR::eModelObjectType::MODELOBJECTTYPE_SOLIDSUPPORT:
		return eLib3MFObjectType::eObjectTypeSolidSupport;
	default:
		return eLib3MFObjectType::eObjectTypeOther;
	}

}

void CInternalLib3MFObject::SetType(const eLib3MFObjectType eObjectType)
{
	NMR::eModelObjectType internalType;
	switch (eObjectType) {
	case eLib3MFObjectType::eObjectTypeOther:
		internalType = NMR::eModelObjectType::MODELOBJECTTYPE_OTHER;
		break;
	case eLib3MFObjectType::eObjectTypeModel:
		internalType = NMR::eModelObjectType::MODELOBJECTTYPE_MODEL;
		break;
	case eLib3MFObjectType::eObjectTypeSupport:
		internalType = NMR::eModelObjectType::MODELOBJECTTYPE_SUPPORT;
		break;
	case eLib3MFObjectType::eObjectTypeSolidSupport:
		internalType = NMR::eModelObjectType::MODELOBJECTTYPE_SOLIDSUPPORT;
		break;
	default:
		internalType = NMR::eModelObjectType::MODELOBJECTTYPE_OTHER;
	}
	m_pModelObject->setObjectType(internalType);
}

std::string CInternalLib3MFObject::GetName ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFObject::SetName (const std::string sName)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CInternalLib3MFObject::GetPartNumber ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFObject::SetPartNumber (const std::string sPartNumber)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CInternalLib3MFObject::IsMeshObject ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CInternalLib3MFObject::IsComponentsObject ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CInternalLib3MFObject::IsValid ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CInternalLib3MFObject::HasMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFMetaDataGroup * CInternalLib3MFObject::GetMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFMetaDataGroup * CInternalLib3MFObject::CreateMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFObject::RemoveMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

