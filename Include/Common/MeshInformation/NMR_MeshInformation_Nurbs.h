/*++

Copyright (C) 2018 3MF Consortium

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

NMR_MeshInformation_Nurbs.h defines the Property Information Class.

--*/

#ifndef __NMR_MESHINFORMATION_NURBS
#define __NMR_MESHINFORMATION_NURBS

#include "Common/MeshInformation/NMR_MeshInformation.h"
#include <list>
#include <map>

namespace NMR {


	class CMeshInformation_Nurbs : public CMeshInformation {
	private:
		std::shared_ptr<MESHINFORMATION_NURBS> m_pDefaultProperty;
	protected:
	public:
		CMeshInformation_Nurbs();
		CMeshInformation_Nurbs(nfUint32 nCurrentFaceCount);

		virtual void invalidateFace(_In_ MESHINFORMATIONFACEDATA * pData);

		virtual eMeshInformationType getType();
		virtual void cloneDefaultInfosFrom(_In_ CMeshInformation * pOtherInformation);
		virtual void cloneFaceInfosFrom(_In_ nfUint32 nFaceIndex, _In_ CMeshInformation * pOtherInformation, _In_ nfUint32 nOtherFaceIndex);
		virtual PMeshInformation cloneInstance(_In_ nfUint32 nCurrentFaceCount);
		virtual void permuteNodeInformation(_In_ nfUint32 nFaceIndex, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3);
		virtual nfUint32 getBackupSize();
		virtual void mergeInformationFrom(_In_ CMeshInformation * pInformation);
		virtual nfBool faceHasData(_In_ nfUint32 nFaceIndex);

		virtual void setDefaultData(MESHINFORMATIONFACEDATA* pData);
		virtual MESHINFORMATIONFACEDATA* getDefaultData();

	};

	typedef std::shared_ptr <CMeshInformation_Nurbs> PMeshInformation_Nurbs;

}

#endif // __NMR_MESHINFORMATION_NURBS

