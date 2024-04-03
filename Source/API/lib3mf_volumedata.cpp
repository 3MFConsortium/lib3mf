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
#include "lib3mf_volumedatacolor.hpp"
#include "lib3mf_volumedatacomposite.hpp"

// Include custom headers here.
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelFunction.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVolumeData 
**************************************************************************************************************************/


CVolumeData::CVolumeData(NMR::PModelResource pResource)
	: CResource(pResource)
{
	if (!pResource.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	m_pVolumeData = std::dynamic_pointer_cast<NMR::CModelVolumeData>(pResource);
	if (!m_pVolumeData)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);
}


IVolumeDataComposite * CVolumeData::GetComposite()
{
	auto pComposite = m_pVolumeData->getComposite();
	if (!pComposite) {
		return nullptr;
	}
	return new CVolumeDataComposite(pComposite);
}

IVolumeDataComposite * CVolumeData::CreateNewComposite()
{
	auto pComposite = m_pVolumeData->createComposite();
	if (!pComposite) {
		return nullptr;
	}
	return new CVolumeDataComposite(pComposite);
}

void CVolumeData::RemoveComposite()
{
	m_pVolumeData->removeComposite();
}

IVolumeDataColor * CVolumeData::GetColor()
{
	auto pColor = m_pVolumeData->getColor();
	if (!pColor) {
		return nullptr;
	}
	return new CVolumeDataColor(pColor);
}

void CVolumeData::RemoveColor()
{
	m_pVolumeData->removeColor();
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

void CVolumeData::RemoveProperty(const Lib3MF_uint32 nIndex)
{
	auto pProperty = m_pVolumeData->getProperty(nIndex);
	if (!pProperty)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	m_pVolumeData->removeProperty(pProperty->getName());
}


IVolumeDataColor* Lib3MF::Impl::CVolumeData::CreateNewColor(
    IFunction* pTheFunction)
{
	NMR::CModel* pModel = m_pVolumeData->getModel();

	NMR::PModelResource pResource = pModel->findResource(pTheFunction->GetUniqueResourceID());
	NMR::PModelFunction pFunction = std::dynamic_pointer_cast<NMR::CModelFunction>(pResource);

	if (!pFunction)
	{
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}

	NMR::PVolumeDataColor pColor = m_pVolumeData->createColor(pFunction);
	if(!pColor)
	{
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}
	return new CVolumeDataColor(pColor);
}

IVolumeDataProperty * Lib3MF::Impl::CVolumeData::AddPropertyFromFunction(const std::string & sName, IFunction * pTheFunction)
{
	NMR::CModel* pModel = m_pVolumeData->getModel();

	NMR::PModelResource pResource = pModel->findResource(pTheFunction->GetUniqueResourceID());
	NMR::PModelFunction pFunction = std::dynamic_pointer_cast<NMR::CModelFunction>(pResource);

	if (!pFunction)
	{
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}

	NMR::PVolumeDataProperty pProperty = m_pVolumeData->addProperty(sName, pFunction);
	if(!pProperty)
	{
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}
	return new CVolumeDataProperty(pProperty);
}
