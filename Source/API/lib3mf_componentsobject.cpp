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

Abstract: This is a stub class definition of CLib3MFComponentsObject

*/

#include "lib3mf_componentsobject.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_component.hpp"
#include "lib3mf_utils.hpp"

// Include custom headers here.

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFComponentsObject 
**************************************************************************************************************************/

CLib3MFComponentsObject::CLib3MFComponentsObject(NMR::PModelResource pResource)
	: CLib3MFResource(pResource), CLib3MFObject(pResource)
{
}

NMR::CModelComponentsObject * CLib3MFComponentsObject::getComponentsObject()
{
	NMR::CModelComponentsObject * pComponentsObject = dynamic_cast<NMR::CModelComponentsObject *> (resource().get());
	if (pComponentsObject == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOMPONENTSOBJECT);

	return pComponentsObject;
}

ILib3MFComponent * CLib3MFComponentsObject::AddComponent (ILib3MFObject* pObjectResource, const sLib3MFTransform Transform)
{
	NMR::CModelComponentsObject * pComponentsObject = getComponentsObject();
	NMR::CModel * pModel = pComponentsObject->getModel();
	if (pModel == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMODEL);
	
	// Get Resource ID
	NMR::PackageResourceID nObjectID = pObjectResource->GetResourceID();

	if (GetResourceID() == nObjectID)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_FORBIDDENCYCLICREFERENCE);

	// TODO: check all ancestors

	// Find class instance
	NMR::CModelObject * pObject = pModel->findObject(nObjectID);
	if (pObject == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	// Convert Transform, if given
	NMR::NMATRIX3 mMatrix = TransformToMatrix(Transform);

	// Create Model component
	NMR::PModelComponent pNewComponent = std::make_shared<NMR::CModelComponent>(pObject, mMatrix);
	pComponentsObject->addComponent(pNewComponent);

	return new CLib3MFComponent(pNewComponent);
}

ILib3MFComponent * CLib3MFComponentsObject::GetComponent (const Lib3MF_uint32 nIndex)
{
	NMR::CModelComponentsObject * pComponentsObject = getComponentsObject();
	NMR::PModelComponent pNewComponent = pComponentsObject->getComponent(nIndex);
	return new CLib3MFComponent(pNewComponent);
}

Lib3MF_uint32 CLib3MFComponentsObject::GetComponentCount ()
{
	return getComponentsObject()->getComponentCount();
}

bool CLib3MFComponentsObject::IsMeshObject()
{
	return false;
}

bool CLib3MFComponentsObject::IsComponentsObject()
{
	return true;
}

