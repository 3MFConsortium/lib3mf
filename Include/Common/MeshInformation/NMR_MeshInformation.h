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
	
NMR_MeshInformation.h defines the Mesh Information Class.
This is a base class for handling all the mesh-related linear information (like face colors,
textures, etc...). It provides abstract functions to interpolate and reconstruct informations while
the mesh topology is changing.

--*/

#ifndef __NMR_MESHINFORMATION
#define __NMR_MESHINFORMATION
      
#include "Common/MeshInformation/NMR_MeshInformationContainer.h"
#include "Common/MeshInformation/NMR_MeshInformationTypes.h"
#include "Common/Math/NMR_Vector.h"

namespace NMR {

	class CMeshInformation;

	typedef std::shared_ptr <CMeshInformation> PMeshInformation;

	class CMeshInformation {
	protected:
		PMeshInformationContainer m_pContainer;
		nfUint64 m_nInternalID;

	public:
		CMeshInformation();

		_Ret_notnull_ MESHINFORMATIONFACEDATA * getFaceData(nfUint32 nFaceIndex);
		_Ret_notnull_ MESHINFORMATIONFACEDATA * addFaceData(_In_ nfUint32 nNewFaceCount);
		void resetFaceInformation(_In_ nfUint32 nFaceIndex);
		void resetAllFaceInformation();

		virtual void invalidateFace(_In_ MESHINFORMATIONFACEDATA * pData) = 0;

		virtual eMeshInformationType getType() = 0;
		virtual void cloneFaceInfosFrom(_In_ nfUint32 nFaceIndex, _In_ CMeshInformation * pOtherInformation, _In_ nfUint32 nOtherFaceIndex) = 0;
		virtual PMeshInformation cloneInstance(_In_ nfUint32 nCurrentFaceCount) = 0;
		virtual void permuteNodeInformation(_In_ nfUint32 nFaceIndex, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3) = 0;
		virtual void mergeInformationFrom (_In_ CMeshInformation * pInformation) = 0;
		virtual nfUint32 getBackupSize() = 0;
		virtual nfBool faceHasData(_In_ nfUint32 nFaceIndex) = 0;

		virtual void setInternalID(nfUint64 nInternalID);
		virtual nfUint64 getInternalID();

	};

}

#endif // __NMR_MESHINFORMATION
