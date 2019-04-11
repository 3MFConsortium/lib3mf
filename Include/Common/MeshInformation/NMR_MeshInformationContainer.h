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

NMR_MeshInformationContainer.h defines the Mesh Information Container Class.
This class provides a memory container for holding the texture information state of a complete mesh structure.

--*/

#ifndef __NMR_MESHINFORMATIONCONTAINER
#define __NMR_MESHINFORMATIONCONTAINER

#include "Common/MeshInformation/NMR_MeshInformationTypes.h"
#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

#include <vector>
#include <memory>

#define MESHINFORMATIONCOUNTER_BUFFERSIZE 256

namespace NMR {

	class CMeshInformationContainer {
	private:
		nfUint32 m_nFaceCount;
		nfUint32 m_nRecordSize;
		std::vector<MESHINFORMATIONFACEDATA *> m_DataBlocks;
		MESHINFORMATIONFACEDATA * m_CurrentDataBlock;

	public:
		CMeshInformationContainer();
		CMeshInformationContainer(nfUint32 nCurrentFaceCount, nfUint32 nRecordSize);
		~CMeshInformationContainer();
		_Ret_notnull_ MESHINFORMATIONFACEDATA * addFaceData(nfUint32 nNewFaceCount);
		_Ret_notnull_ MESHINFORMATIONFACEDATA * getFaceData(nfUint32 nIdx);

		nfUint32 getCurrentFaceCount();
		void clear();
	};

	typedef std::shared_ptr <CMeshInformationContainer> PMeshInformationContainer;
}

#endif // __NMR_MESHINFORMATIONCONTAINER
