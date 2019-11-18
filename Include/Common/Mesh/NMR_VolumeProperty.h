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

NMR_VolumeProperty.h defines the class CVolumeProperty.

--*/

#ifndef __NMR_VOLUMEPROPERTY
#define __NMR_VOLUMEPROPERTY

#include "Common/NMR_Types.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Common/Mesh/NMR_VolumeBase.h"

namespace NMR {

	class CVolumeProperty : public CVolumeBase {
	private:
		std::string m_sChannelName;
		std::string m_sPropertyName;
		bool m_bIsRequired;
	public:
		CVolumeProperty(std::string sName, PModelVolumetricStack pVolumetricStack);

		std::string GetChannel();
		void SetChannel(std::string sChannelName);

		std::string GetName();
		void SetName(std::string sPropertyName);

		bool IsRequired();
		void SetIsRequired(bool bIsRequired);

		void clear();
	};

	typedef std::shared_ptr<CVolumeProperty> PVolumeProperty;

}

#endif // __NMR_VOLUMEPROPERTY
