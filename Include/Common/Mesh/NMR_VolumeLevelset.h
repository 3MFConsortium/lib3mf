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

NMR_VolumeLevelset.h defines the class CVolumeLevelset.

--*/

#ifndef __NMR_VOLUMELEVELSET
#define __NMR_VOLUMELEVELSET

#include "Common/NMR_Types.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Common/Mesh/NMR_VolumeBase.h"
#include <string>

namespace NMR {

	class CVolumeLevelset : public CVolumeBase {
	private:
		nfDouble m_dSolidThreshold;
		std::string m_sChannelName;
	public:
		CVolumeLevelset(PModelVolumetricStack pVolumetricStack);

		nfDouble GetSolidThreshold();
		void SetSolidThreshold(nfDouble dSolidThreshold);

		std::string GetChannel();
		void SetChannel(std::string sChannelName);
		
		void clear();
	};

	typedef std::shared_ptr<CVolumeLevelset> PVolumeLevelset;

}

#endif // __NMR_VOLUMELEVELSET
