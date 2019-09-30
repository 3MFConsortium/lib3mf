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

Abstract: This is a stub class definition of CVolumeDataLevelset

*/

#include "lib3mf_volumedatalevelset.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVolumeDataLevelset 
**************************************************************************************************************************/

CVolumeDataLevelset::CVolumeDataLevelset(NMR::PVolumeLevelset pVolumeLevelset, NMR::CModel* pModel)
	: CVolumeDataItem(pVolumeLevelset, pModel)
{
	if (pVolumeLevelset == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

NMR::CVolumeLevelset* CVolumeDataLevelset::VolumeLevelset()
{
	auto pLevelset = std::dynamic_pointer_cast<NMR::CVolumeLevelset>(m_pVolumeBase);
	if (!pLevelset)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	return pLevelset.get();
}


Lib3MF_double CVolumeDataLevelset::GetSolidThreshold()
{
	return VolumeLevelset()->GetSolidThreshold();
}

void CVolumeDataLevelset::SetSolidThreshold(const Lib3MF_double dTheSolidThreshold)
{
	VolumeLevelset()->SetSolidThreshold(dTheSolidThreshold);
}

void CVolumeDataLevelset::SetChannel(const std::string & sChannelName)
{
	VolumeLevelset()->SetChannel(sChannelName);
}

std::string CVolumeDataLevelset::GetChannel()
{
	return VolumeLevelset()->GetChannel();
}

