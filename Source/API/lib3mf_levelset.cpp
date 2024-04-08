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

Abstract: This is a stub class definition of CLevelSet

*/

#include "lib3mf_levelset.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_meshobject.hpp"
#include "lib3mf_utils.hpp"
#include "lib3mf_function.hpp"
#include "lib3mf_volumedata.hpp"
#include "Model/Classes/NMR_ModelLevelSetObject.h"
#include "Model/Classes/NMR_ModelFunction.h"
#include "Model/Classes/NMR_ModelMeshObject.h"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLevelSet 
**************************************************************************************************************************/


ILevelSet* CLevelSet::fnCreateLevelSetFromModelResource(NMR::PModelResource pResource, bool bFailIfUnkownClass) {

	if (!pResource.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::CModelLevelSetObject * LevelSetObject = dynamic_cast<NMR::CModelLevelSetObject *> (pResource.get());
	if (LevelSetObject) {
		return new CLevelSet(pResource);
	}

	if (bFailIfUnkownClass)
		throw ELib3MFInterfaceException(NMR_ERROR_UNKNOWNMODELRESOURCE);

	return nullptr;
}



IFunction * CLevelSet::GetFunction()
{
	auto levelSet = levelSetObject();
	auto function = levelSet->getFunction();
	if (!function)
	{
		return nullptr;
	}
	return new CFunction(function);
}

NMR::PModelLevelSetObject
Lib3MF::Impl::CLevelSet::levelSetObject()
{
    NMR::PModelLevelSetObject pLevelSet = std::dynamic_pointer_cast<NMR::CModelLevelSetObject>(resource());
	if (pLevelSet.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pLevelSet;
}

CLevelSet::CLevelSet(NMR::PModelResource pResource)
    : CResource(pResource), CObject(pResource)
{
}

void CLevelSet::SetFunction(IFunction* pTheFunction)
{
    if(pTheFunction == nullptr)
	{
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}

    NMR::CModel* pModel = levelSetObject()->getModel();
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
	levelSetObject()->setFunction(pFunction);
}

Lib3MF::sTransform CLevelSet::GetTransform()
{
	return MatrixToTransform(levelSetObject()->getTransform());
}

void CLevelSet::SetTransform(const Lib3MF::sTransform Transform)
{
	levelSetObject()->setTransform(TransformToMatrix(Transform));
}

std::string CLevelSet::GetChannelName()
{
	return levelSetObject()->getChannelName();
}

void CLevelSet::SetChannelName(const std::string & sChannelName)
{
	levelSetObject()->setChannelName(sChannelName);
}

void CLevelSet::SetMinFeatureSize(const Lib3MF_double dMinFeatureSize)
{
	levelSetObject()->setMinFeatureSize(dMinFeatureSize);
}

Lib3MF_double CLevelSet::GetMinFeatureSize()
{
	return levelSetObject()->getMinFeatureSize();
}

void CLevelSet::SetFallBackValue(const Lib3MF_double dFallBackValue)
{
	levelSetObject()->setFallBackValue(dFallBackValue);
}

Lib3MF_double CLevelSet::GetFallBackValue()
{
	return levelSetObject()->getFallBackValue();
}

void CLevelSet::SetMeshBBoxOnly(const bool bMeshBBoxOnly)
{
	levelSetObject()->setMeshBBoxOnly(bMeshBBoxOnly);
}

bool CLevelSet::GetMeshBBoxOnly()
{
	return levelSetObject()->getMeshBBoxOnly();
}

void CLevelSet::SetMesh(IMeshObject* pTheMesh)
{
	if(pTheMesh == nullptr)
	{
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}

	NMR::CModel* pModel = levelSetObject()->getModel();
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
	levelSetObject()->setMesh(pMesh);
}

IMeshObject * CLevelSet::GetMesh()
{
     NMR::PModelMeshObject meshObject = levelSetObject()->getMesh();

	 if (!meshObject)
	 {
		 throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	 }

	 return new CMeshObject(meshObject);
}

void CLevelSet::SetVolumeData(IVolumeData* pTheVolumeData)
{
	NMR::CModel * pModel = levelSetObject()->getModel();
	if (pModel == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);

	NMR::PModelResource pResource = pModel->findResource(pTheVolumeData->GetResourceID());

	NMR::PModelVolumeData pVolumeData = std::dynamic_pointer_cast<NMR::CModelVolumeData>(pResource);

	if (pVolumeData == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);

	levelSetObject()->setVolumeData(pVolumeData);
}

bool Lib3MF::Impl::CLevelSet::IsMeshObject()
{
    return false;
}

IVolumeData * CLevelSet::GetVolumeData()
{
	NMR::PModelVolumeData pVolumeData = levelSetObject()->getVolumeData();
	if (pVolumeData == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return new CVolumeData(pVolumeData);
}