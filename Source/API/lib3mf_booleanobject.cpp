/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CBooleanObject

*/

#include "lib3mf_booleanobject.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_meshobject.hpp"
#include "Model/Classes/NMR_ModelBooleanObject.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "lib3mf_utils.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CBooleanObject 
**************************************************************************************************************************/

CBooleanObject::CBooleanObject(NMR::PModelResource pResource)
	: CResource(pResource), CObject(pResource)
{
}

NMR::PModelBooleanObject CBooleanObject::booleanObject()
{
	auto pBooleanObject = std::dynamic_pointer_cast<NMR::CModelBooleanObject>(resource());
	if (!pBooleanObject)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pBooleanObject;
}

bool CBooleanObject::IsBooleanObject()
{
	return true;
}

bool CBooleanObject::IsMeshObject()
{
	return false;
}

bool CBooleanObject::IsComponentsObject()
{
	return false;
}

bool CBooleanObject::IsLevelSetObject()
{
	return false;
}

void CBooleanObject::SetBaseObject(IObject* pBaseObject, const Lib3MF::sTransform Transform)
{
	if (!pBaseObject)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	auto pBaseResource = booleanObject()->getModel()->findResource(
		booleanObject()->getModel()->currentPath(),
		pBaseObject->GetResourceID());
	auto pBaseModelObject = std::dynamic_pointer_cast<NMR::CModelObject>(pBaseResource);
	if (!pBaseModelObject)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	if (dynamic_cast<NMR::CModelComponentsObject *>(pBaseModelObject.get()) != nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOMPONENTSOBJECT);

	booleanObject()->setBaseObject(pBaseModelObject.get(), Lib3MF::TransformToMatrix(Transform));
}

IObject * CBooleanObject::GetBaseObject()
{
	auto pBaseObject = booleanObject()->getBaseObject();
	if (!pBaseObject)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);

	auto pResource = booleanObject()->getModel()->findResource(pBaseObject->getPackageResourceID());
	return CObject::fnCreateObjectFromModelResource(pResource, true);
}

void CBooleanObject::SetBaseTransform(const Lib3MF::sTransform Transform)
{
	booleanObject()->setBaseTransform(Lib3MF::TransformToMatrix(Transform));
}

Lib3MF::sTransform CBooleanObject::GetBaseTransform()
{
	return Lib3MF::MatrixToTransform(booleanObject()->getBaseTransform());
}

void CBooleanObject::SetOperation(const Lib3MF::eBooleanOperation eOperation)
{
	booleanObject()->setOperation((NMR::eModelBooleanOperation)eOperation);
}

Lib3MF::eBooleanOperation CBooleanObject::GetOperation()
{
	return (Lib3MF::eBooleanOperation)booleanObject()->getOperation();
}

void CBooleanObject::SetCSGModeEnabled(const bool bCSGModeEnabled)
{
	booleanObject()->setCSGModeEnabled(bCSGModeEnabled);
}

bool CBooleanObject::GetCSGModeEnabled()
{
	return booleanObject()->getCSGModeEnabled();
}

void CBooleanObject::SetExtractionGridResolution(const Lib3MF_uint32 nGridResolution)
{
	booleanObject()->setExtractionGridResolution(nGridResolution);
}

Lib3MF_uint32 CBooleanObject::GetExtractionGridResolution()
{
	return booleanObject()->getExtractionGridResolution();
}

Lib3MF_uint32 CBooleanObject::GetOperandCount()
{
	return booleanObject()->getOperandCount();
}

void CBooleanObject::AddOperand(IMeshObject* pOperandObject, const Lib3MF::sTransform Transform)
{
	if (!pOperandObject)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	auto pOperandResource = booleanObject()->getModel()->findResource(
		booleanObject()->getModel()->currentPath(),
		pOperandObject->GetResourceID());
	auto pMeshObject = std::dynamic_pointer_cast<NMR::CModelMeshObject>(pOperandResource);
	if (!pMeshObject)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMESHOBJECT);

	booleanObject()->addOperand(pMeshObject.get(), Lib3MF::TransformToMatrix(Transform));
}

Lib3MF::sTransform CBooleanObject::GetOperand(const Lib3MF_uint32 nIndex, IMeshObject*& pOperandObject)
{
	auto operand = booleanObject()->getOperand(nIndex);
	auto pMeshObject = dynamic_cast<NMR::CModelMeshObject *>(operand->getObject());
	if (!pMeshObject)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMESHOBJECT);

	auto pResource = booleanObject()->getModel()->findResource(pMeshObject->getPackageResourceID());
	pOperandObject = new CMeshObject(pResource);
	return Lib3MF::MatrixToTransform(operand->getTransform());
}
