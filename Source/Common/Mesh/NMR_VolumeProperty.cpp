/*++

Copyright (C) 2019 3MF Consortium

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

Abstract:

NMR_VolumeProperty.cpp implements the class CVolumeProperty.

--*/

#include "Common/Mesh/NMR_VolumeProperty.h" 

namespace NMR {

	CVolumeProperty::CVolumeProperty(std::string sName, PModelVolumetricStack pVolumetricStack)
		: CVolumeBase(pVolumetricStack), m_bIsRequired(true)
	{
		m_sPropertyName = sName;
	}

	void CVolumeProperty::clear()
	{

	}

	std::string CVolumeProperty::GetChannel()
	{
		return m_sChannelName;
	}

	void CVolumeProperty::SetChannel(std::string sChannelName)
	{
		if (!GetVolumetricStack()->hasDstChannel(sChannelName)) {
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		}
		m_sChannelName = sChannelName;
	}

	std::string CVolumeProperty::GetName()
	{
		return m_sPropertyName;
	}
	void CVolumeProperty::SetName(std::string sPropertyName)
	{
		m_sPropertyName = sPropertyName;
	}

	bool CVolumeProperty::IsRequired()
	{
		return m_bIsRequired;
	}

	void CVolumeProperty::SetIsRequired(bool bIsRequired)
	{
		m_bIsRequired = bIsRequired;
	}
}
