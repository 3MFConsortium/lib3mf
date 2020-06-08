/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CComponentsObject

*/

#include "lib3mf_componentsobject.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_component.hpp"
#include "lib3mf_utils.hpp"

// Include custom headers here.

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CComponentsObject 
**************************************************************************************************************************/


IComponentsObject* CComponentsObject::fnCreateComponentsObjectFromModelResource(NMR::PModelResource pResource, bool bFailIfUnkownClass)
{

	if (!pResource.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::CModelComponentsObject * pMeshObject = dynamic_cast<NMR::CModelComponentsObject *> (pResource.get());
	if (pMeshObject) {
		return new CComponentsObject(pResource);
	}

	if (bFailIfUnkownClass)
		throw ELib3MFInterfaceException(NMR_ERROR_UNKNOWNMODELRESOURCE);

	return nullptr;
}

CComponentsObject::CComponentsObject(NMR::PModelResource pResource)
	: CResource(pResource), CObject(pResource)
{
}

NMR::CModelComponentsObject * CComponentsObject::getComponentsObject()
{
	NMR::CModelComponentsObject * pComponentsObject = dynamic_cast<NMR::CModelComponentsObject *> (resource().get());
	if (pComponentsObject == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOMPONENTSOBJECT);

	return pComponentsObject;
}

IComponent * CComponentsObject::AddComponent(IObject* pObjectResource, const sLib3MFTransform Transform)
{
	NMR::CModelComponentsObject * pComponentsObject = getComponentsObject();
	NMR::CModel * pModel = pComponentsObject->getModel();
	if (pModel == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMODEL);
	
	// Get Resource ID
	NMR::UniqueResourceID nObjectID = pObjectResource->GetResourceID();

	if (GetResourceID() == nObjectID)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_FORBIDDENCYCLICREFERENCE);

	// TODO: check all ancestors to avoid circular references

	// Find class instance
	NMR::CModelObject * pObject = pModel->findObject(nObjectID);
	if (pObject == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	// Convert Transform, if given
	NMR::NMATRIX3 mMatrix = TransformToMatrix(Transform);

	// Create Model component
	NMR::PModelComponent pNewComponent = std::make_shared<NMR::CModelComponent>(pObject, mMatrix);
	pComponentsObject->addComponent(pNewComponent);

	return new CComponent(pNewComponent);
}

IComponent * CComponentsObject::GetComponent (const Lib3MF_uint32 nIndex)
{
	NMR::CModelComponentsObject * pComponentsObject = getComponentsObject();
	NMR::PModelComponent pNewComponent = pComponentsObject->getComponent(nIndex);
	return new CComponent(pNewComponent);
}

Lib3MF_uint32 CComponentsObject::GetComponentCount ()
{
	return getComponentsObject()->getComponentCount();
}

bool CComponentsObject::IsMeshObject()
{
	return false;
}

bool CComponentsObject::IsComponentsObject()
{
	return true;
}

