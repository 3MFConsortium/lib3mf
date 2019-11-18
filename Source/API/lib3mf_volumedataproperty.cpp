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

Abstract: This is a stub class definition of CVolumeDataProperty

*/

#include "lib3mf_volumedataproperty.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVolumeDataProperty 
**************************************************************************************************************************/

CVolumeDataProperty::CVolumeDataProperty(NMR::PVolumeProperty pVolumeProperty, NMR::CModel* pModel)
	: CVolumeDataItem(pVolumeProperty, pModel)
{
	if (pVolumeProperty == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

NMR::CVolumeProperty* CVolumeDataProperty::VolumeProperty()
{
	auto pProperty = std::dynamic_pointer_cast<NMR::CVolumeProperty>(m_pVolumeBase);
	if (!pProperty)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	return pProperty.get();
}

void CVolumeDataProperty::SetChannel(const std::string & sChannelName)
{
	VolumeProperty()->SetChannel(sChannelName);
}

std::string CVolumeDataProperty::GetChannel()
{
	return VolumeProperty()->GetChannel();
}

void CVolumeDataProperty::SetName(const std::string & sPropertyName)
{
	VolumeProperty()->SetName(sPropertyName);
}

std::string CVolumeDataProperty::GetName()
{
	return VolumeProperty()->GetName();
}

void CVolumeDataProperty::SetIsRequired(const bool bIsRequired)
{
	VolumeProperty()->SetIsRequired(bIsRequired);
}

bool CVolumeDataProperty::IsRequired()
{
	return VolumeProperty()->IsRequired();
}

