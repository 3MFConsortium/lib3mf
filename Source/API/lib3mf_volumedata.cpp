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

Abstract: This is a stub class definition of CVolumeData

*/

#include "lib3mf_volumedata.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_volumedataproperty.hpp"
#include "lib3mf_volumedataboundary.hpp"

// Include custom headers here.
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelScalarField.h"
#include "Model/Classes/NMR_ModelScalarField.h"
#include "Model/Classes/NMR_ModelVector3DField.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVolumeData 
**************************************************************************************************************************/

CVolumeData::CVolumeData(NMR::PModelMeshObject pMeshObject, NMR::PModelVolumeData pVolumeData)
	:CBase(), m_pMeshObject(pMeshObject), m_pVolumeData(pVolumeData)
{
}

IVolumeDataBoundary* CVolumeData::GetBoundary()
{
	auto pBoundary = m_pVolumeData->getBoundary();
	if (!pBoundary) {
		return nullptr;
	}
	return new CVolumeDataBoundary(pBoundary);
}

IVolumeDataBoundary* CVolumeData::CreateNewBoundary(IScalarField* pTheScalarField)
{
	NMR::CModel* pModel = m_pMeshObject->getModel();

	NMR::PModelResource pResource = pModel->findResource(pTheScalarField->GetUniqueResourceID());
	NMR::PModelScalarField pScalarField = std::dynamic_pointer_cast<NMR::CModelScalarField>(pResource);

	if (!pScalarField)
	{
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}

	NMR::PVolumeDataBoundary pBoundary = m_pVolumeData->createBoundary(pScalarField);
	return new CVolumeDataBoundary(pBoundary);
}

void CVolumeData::RemoveBoundary()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IVolumeDataComposite * CVolumeData::GetComposite()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IVolumeDataComposite * CVolumeData::CreateNewComposite()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CVolumeData::RemoveComposite()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IVolumeDataColor * CVolumeData::GetColor()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IVolumeDataColor * CVolumeData::CreateNewColor(IVector3DField* pTheVector3DField)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CVolumeData::RemoveColor()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CVolumeData::GetPropertyCount()
{
	return m_pVolumeData->getPropertyCount();
}

IVolumeDataProperty * CVolumeData::GetProperty(const Lib3MF_uint32 nIndex)
{
	NMR::PVolumeDataProperty pProperty = m_pVolumeData->getProperty(nIndex);
	if (!pProperty) {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}
	return new CVolumeDataProperty(pProperty);
}

IVolumeDataProperty* CVolumeData::AddPropertyFromScalarField(const std::string& sName, IScalarField* pTheScalarField)
{
	NMR::CModel* pModel = m_pMeshObject->getModel();

	NMR::PModelResource pResource = pModel->findResource(pTheScalarField->GetUniqueResourceID());
	NMR::PModelScalarField pScalarField = std::dynamic_pointer_cast<NMR::CModelScalarField>(pResource);

	if (!pScalarField) {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}
	auto pProperty = m_pVolumeData->addProperty(sName, pScalarField);

	if (!pProperty) {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}
	return new CVolumeDataProperty(pProperty);
}

IVolumeDataProperty* CVolumeData::AddPropertyFromVector3DField(const std::string& sName, IVector3DField* pTheVector3DField)
{
	NMR::CModel* pModel = m_pMeshObject->getModel();

	NMR::PModelResource pResource = pModel->findResource(pTheVector3DField->GetUniqueResourceID());
	NMR::PModelVector3DField pVector3DField = std::dynamic_pointer_cast<NMR::CModelVector3DField>(pResource);

	if (!pVector3DField) {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}
	auto pProperty = m_pVolumeData->addProperty(sName, pVector3DField);

	if (!pProperty) {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}
	return new CVolumeDataProperty(pProperty);
}

void CVolumeData::RemoveProperty(const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

