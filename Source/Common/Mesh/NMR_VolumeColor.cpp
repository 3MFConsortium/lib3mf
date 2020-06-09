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

NMR_VolumeColor.cpp implements the class CVolumeColor.

--*/

#include "Common/Mesh/NMR_VolumeColor.h" 

namespace NMR {

	CVolumeColor::CVolumeColor(PModelVolumetricStack pVolumetricStack)
		: CVolumeBase(pVolumetricStack)
	{

	}

	void CVolumeColor::clear()
	{

	}

	void CVolumeColor::SetChannel(const eModelColorChannel eTheColorChannel, const std::string & sChannelName)
	{
		if (!GetVolumetricStack()->hasDstChannel(sChannelName)) {
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		}

		if (eTheColorChannel > eModelColorChannel::MODELCOLORCHANNEL_BLUE)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (eTheColorChannel == eModelColorChannel::MODELCOLORCHANNEL_RED)
			m_sChannelRed = sChannelName;
		else if (eTheColorChannel == eModelColorChannel::MODELCOLORCHANNEL_GREEN)
			m_sChannelGreen = sChannelName;
		else if (eTheColorChannel == eModelColorChannel::MODELCOLORCHANNEL_BLUE)
			m_sChannelBlue = sChannelName;
	}

	std::string CVolumeColor::GetChannel(const eModelColorChannel eTheColorChannel)
	{
		if (eTheColorChannel > eModelColorChannel::MODELCOLORCHANNEL_BLUE)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (eTheColorChannel == eModelColorChannel::MODELCOLORCHANNEL_RED)
			return m_sChannelRed;
		else if (eTheColorChannel == eModelColorChannel::MODELCOLORCHANNEL_GREEN)
			return m_sChannelGreen;
		else
			return m_sChannelBlue;
	}

}
