/*++

Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_MeshInformationHandler.h defines the Mesh Information Handler Class.
It allows to include different kinds of information in one mesh (like Textures AND colors).

--*/

#ifndef __NMR_MESHINFORMATIONHANDLER
#define __NMR_MESHINFORMATIONHANDLER

#include "Common/MeshInformation/NMR_MeshInformation.h"
#include <vector>
#include <array>

namespace NMR {

	class CMeshInformationHandler {
	protected:
		std::vector<PMeshInformation> m_pInformations;
		std::array<CMeshInformation *, emiLastType> m_pLookup;

		nfUint64 m_nInternalIDCounter;

	public:
		CMeshInformationHandler();

		void addInformation(_In_ PMeshInformation pInformation);
		void addFace(_In_ nfUint32 nNewFaceCount);

		CMeshInformation * getInformationIndexed(_In_ nfUint32 nIdx);
		PMeshInformation getPInformationIndexed(_In_ nfUint32 nIdx);
		CMeshInformation * getInformationByType(_In_ nfUint32 nChannel, _In_ eMeshInformationType eType);
		nfUint32 getInformationCount();

		void addInfoTableFrom(_In_ CMeshInformationHandler * pOtherInfoHandler, _In_ nfUint32 nCurrentFaceCount);
		void cloneFaceInfosFrom(_In_ nfUint32 nFaceIdx, _In_ CMeshInformationHandler * pOtherInfoHandler, _In_ nfUint32 nOtherFaceIndex);
		void permuteNodeInformation(_In_ nfUint32 nFaceIdx, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3);
		void resetFaceInformation(_In_ nfUint32 nFaceIdx);

		void removeInformation(_In_ eMeshInformationType eType);
		void removeInformationIndexed(_In_ nfUint32 nIndex);

	};

	typedef std::shared_ptr <CMeshInformationHandler> PMeshInformationHandler;

}

#endif // __NMR_MESHINFORMATIONHANDLER
