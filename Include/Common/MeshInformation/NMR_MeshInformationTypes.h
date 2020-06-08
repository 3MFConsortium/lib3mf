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

NMR_MeshInformationTypes.h defines the basic structs and constants
to contain mesh surface information.

--*/

#ifndef __NMR_MESHINFORMATIONTYPES
#define __NMR_MESHINFORMATIONTYPES

#include "Common/NMR_Types.h"
#include "Common/Math/NMR_Vector.h"

namespace NMR {

#define MESHINFORMATION_MAXINTERNALID 9223372036854775808ULL

	typedef enum _eMeshInformationType {
		emiAbstract   = 0x0000,
		emiProperties = 0x0001,
		emiLastType
	} eMeshInformationType;

	typedef nfByte MESHINFORMATIONFACEDATA;

#pragma pack (1)
	typedef struct {
		nfUint32 m_nUniqueResourceID;
		nfUint32 m_nPropertyIDs[3];
	} MESHINFORMATION_PROPERTIES;


#pragma pack()

}

#endif // __NMR_MESHINFORMATIONTYPES
