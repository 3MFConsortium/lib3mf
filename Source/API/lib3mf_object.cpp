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

Abstract: This is a stub class definition of CLib3MFObject

*/

#include "lib3mf_object.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_meshobject.hpp"
#include "lib3mf_componentsobject.hpp"

// Include custom headers here.
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Model/Classes/NMR_ModelComponentsObject.h" 

using namespace Lib3MF;

/*************************************************************************************************************************
 Class definition of CLib3MFObject 
**************************************************************************************************************************/

ILib3MFObject* CLib3MFObject::fnCreateObjectFromModelResource(NMR::PModelResource pResource, bool bFailIfUnkownClass) {
	
	if (!pResource.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::CModelMeshObject * pMeshObject = dynamic_cast<NMR::CModelMeshObject *> (pResource.get());
	if (pMeshObject) {
		return new CLib3MFMeshObject(pResource);
	}

	NMR::CModelComponentsObject * pComponentsResource = dynamic_cast<NMR::CModelComponentsObject *> (pResource.get());
	if (pComponentsResource) {
		return new CLib3MFComponentsObject(pResource);
	}

	if (bFailIfUnkownClass)
		throw ELib3MFInterfaceException(NMR_ERROR_UNKNOWNMODELRESOURCE);

	return nullptr;
}

CLib3MFObject::CLib3MFObject(NMR::PModelResource pResource)
{
	m_pResource = pResource;
}


eLib3MFObjectType CLib3MFObject::GetType ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFObject::SetType (const eLib3MFObjectType eObjectType)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CLib3MFObject::GetName ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFObject::SetName (const std::string & sName)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CLib3MFObject::GetPartNumber ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFObject::SetPartNumber (const std::string & sPartNumber)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CLib3MFObject::IsMeshObject ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CLib3MFObject::IsComponentsObject ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CLib3MFObject::IsValid ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CLib3MFObject::HasMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILib3MFMetaDataGroup * CLib3MFObject::GetMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILib3MFMetaDataGroup * CLib3MFObject::CreateMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFObject::RemoveMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

