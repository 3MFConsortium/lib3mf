/*++

Copyright (C) 2023 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CBoundaryShape

*/

#include "lib3mf_boundaryshape.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_meshobject.hpp"
#include "lib3mf_utils.hpp"
#include "lib3mf_function.hpp"
#include "Model/Classes/NMR_ModelBoundaryShapeObject.h"
#include "Model/Classes/NMR_ModelFunction.h"
#include "Model/Classes/NMR_ModelMeshObject.h"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CBoundaryShape 
**************************************************************************************************************************/


IBoundaryShape* CBoundaryShape::fnCreateBoundaryShapeFromModelResource(NMR::PModelResource pResource, bool bFailIfUnkownClass) {

	if (!pResource.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::CModelBoundaryShapeObject * BoundaryShapeObject = dynamic_cast<NMR::CModelBoundaryShapeObject *> (pResource.get());
	if (BoundaryShapeObject) {
		return new CBoundaryShape(pResource);
	}

	if (bFailIfUnkownClass)
		throw ELib3MFInterfaceException(NMR_ERROR_UNKNOWNMODELRESOURCE);

	return nullptr;
}



IFunction * CBoundaryShape::GetFunction()
{
	auto boundaryShape = boundaryShapeObject();
	auto function = boundaryShape->getFunction();
	if (!function)
	{
		return nullptr;
	}
	return new CFunction(function);
}

NMR::PModelBoundaryShapeObject
Lib3MF::Impl::CBoundaryShape::boundaryShapeObject()
{
    NMR::PModelBoundaryShapeObject pBoundaryShape = std::dynamic_pointer_cast<NMR::CModelBoundaryShapeObject>(resource());
	if (pBoundaryShape.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pBoundaryShape;
}

CBoundaryShape::CBoundaryShape(NMR::PModelResource pResource)
    : CResource(pResource), CObject(pResource)
{
}

void CBoundaryShape::SetFunction(IFunction* pTheFunction)
{
    if(pTheFunction == nullptr)
	{
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}

    NMR::CModel* pModel = boundaryShapeObject()->getModel();
    if(pModel == nullptr)
	{
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	}

    NMR::PModelFunction pFunction =
        std::dynamic_pointer_cast<NMR::CModelFunction>(
            pModel->findResource(pTheFunction->GetUniqueResourceID()));

    if(!pFunction)
	{
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	} 
	boundaryShapeObject()->setFunction(pFunction);
}

Lib3MF::sTransform CBoundaryShape::GetTransform()
{
	return MatrixToTransform(boundaryShapeObject()->getTransform());
}

void CBoundaryShape::SetTransform(const Lib3MF::sTransform Transform)
{
	boundaryShapeObject()->setTransform(TransformToMatrix(Transform));
}

std::string CBoundaryShape::GetChannelName()
{
	return boundaryShapeObject()->getChannelName();
}

void CBoundaryShape::SetChannelName(const std::string & sChannelName)
{
	boundaryShapeObject()->setChannelName(sChannelName);
}

void CBoundaryShape::SetMinFeatureSize(const Lib3MF_double dMinFeatureSize)
{
	boundaryShapeObject()->setMinFeatureSize(dMinFeatureSize);
}

Lib3MF_double CBoundaryShape::GetMinFeatureSize()
{
	return boundaryShapeObject()->getMinFeatureSize();
}

void CBoundaryShape::SetFallBackValue(const Lib3MF_double dFallBackValue)
{
	boundaryShapeObject()->setFallBackValue(dFallBackValue);
}

Lib3MF_double CBoundaryShape::GetFallBackValue()
{
	return boundaryShapeObject()->getFallBackValue();
}

void CBoundaryShape::SetMeshBBoxOnly(const bool bMeshBBoxOnly)
{
	boundaryShapeObject()->setMeshBBoxOnly(bMeshBBoxOnly);
}

bool CBoundaryShape::GetMeshBBoxOnly()
{
	return boundaryShapeObject()->getMeshBBoxOnly();
}

void CBoundaryShape::SetMesh(IMeshObject* pTheMesh)
{
	if(pTheMesh == nullptr)
	{
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}

	NMR::CModel* pModel = boundaryShapeObject()->getModel();
	if(pModel == nullptr)
	{
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	}

	NMR::PModelMeshObject pMesh =
		std::dynamic_pointer_cast<NMR::CModelMeshObject>(
			pModel->findResource(pTheMesh->GetUniqueResourceID()));

	if(!pMesh)
	{
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	} 
	boundaryShapeObject()->setMesh(pMesh);
}

IMeshObject * CBoundaryShape::GetMesh()
{
     NMR::PModelMeshObject meshObject = boundaryShapeObject()->getMesh();

	 if (!meshObject)
	 {
		 throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	 }

	 return new CMeshObject(meshObject);
}

void CBoundaryShape::SetVolumeData(IVolumeData* pVolumeData)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IVolumeData * CBoundaryShape::GetVolumeData()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}