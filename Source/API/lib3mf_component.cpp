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

Abstract: This is a stub class definition of CComponent

*/

#include "lib3mf_component.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_utils.hpp"
#include "lib3mf_object.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CComponent 
**************************************************************************************************************************/

CComponent::CComponent(NMR::PModelComponent pComponent)
{
	if (pComponent.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOMPONENT);
	m_pComponent = pComponent;
}


IObject * CComponent::GetObjectResource()
{
	NMR::PModelResource pModelObject = m_pComponent->getModel()->findResource(m_pComponent->getObject()->getPackageResourceID());
	if (!pModelObject.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMODELRESOURCE);

	std::unique_ptr<IObject> pObject(CObject::fnCreateObjectFromModelResource(pModelObject, true));
	if (pObject == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	return pObject.release();
}

Lib3MF_uint32 CComponent::GetObjectResourceID()
{
	return m_pComponent->getObject()->getPackageResourceID()->getUniqueID();
}

std::string CComponent::GetUUID(bool & bHasUUID)
{
	NMR::PUUID pUUID = m_pComponent->uuid();
	bHasUUID = (pUUID.get() != nullptr);
	if (bHasUUID)
		return pUUID->toString(); 
	else
		return "";
}

void CComponent::SetUUID(const std::string & sUUID)
{
	NMR::PUUID pUUID = std::make_shared<NMR::CUUID>(sUUID);
	m_pComponent->setUUID(pUUID);
}

bool CComponent::HasTransform()
{
	return m_pComponent->hasTransform();
}

void CComponent::SetTransform (const sLib3MFTransform Transform)
{
	m_pComponent->setTransform(TransformToMatrix(Transform));
}

sLib3MFTransform CComponent::GetTransform()
{
	const NMR::NMATRIX3 matrix = m_pComponent->getTransform();
	return MatrixToTransform(matrix);
}

