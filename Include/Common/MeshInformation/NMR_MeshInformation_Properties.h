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

NMR_MeshInformation_Properties.h defines the Property Information Class.

--*/

#ifndef __NMR_MESHINFORMATION_PROPERTIES
#define __NMR_MESHINFORMATION_PROPERTIES

#include "Common/MeshInformation/NMR_MeshInformation.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include <list>
#include <map>

namespace NMR {

	class CMeshInformation_PropertyIndexMapping {
	private:
		std::map<std::pair<UniqueResourceID, ModelPropertyID>, nfUint32> m_IDMap;
	public:
		CMeshInformation_PropertyIndexMapping();

		nfUint32 mapPropertyIDToIndex(UniqueResourceID nUniqueResourceID, ModelPropertyID nPropertyID);
		
		UniqueResourceID getDefaultResourceID();
		nfUint32 getDefaultResourceIndex();

		nfUint32 registerPropertyID(UniqueResourceID nUniqueResourceID, ModelPropertyID nPropertyID, nfUint32 nResourceIndex);

	};

	typedef std::shared_ptr <CMeshInformation_PropertyIndexMapping> PMeshInformation_PropertyIndexMapping;


	class CMeshInformation_Properties : public CMeshInformation {
	private:
		std::shared_ptr<MESHINFORMATION_PROPERTIES> m_pDefaultProperty;
	protected:
	public:
		CMeshInformation_Properties();
		CMeshInformation_Properties(nfUint32 nCurrentFaceCount);

		void invalidateFace(_In_ MESHINFORMATIONFACEDATA * pData) override;

		eMeshInformationType getType() override;
		void cloneDefaultInfosFrom(_In_ CMeshInformation * pOtherInformation) override;
		void cloneFaceInfosFrom(_In_ nfUint32 nFaceIndex, _In_ CMeshInformation * pOtherInformation, _In_ nfUint32 nOtherFaceIndex) override;
		PMeshInformation cloneInstance(_In_ nfUint32 nCurrentFaceCount) override;
		void permuteNodeInformation(_In_ nfUint32 nFaceIndex, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3) override;
		nfUint32 getBackupSize() override;
		void mergeInformationFrom(_In_ CMeshInformation * pInformation) override;
		nfBool faceHasData(_In_ nfUint32 nFaceIndex) override;

		void setDefaultData(MESHINFORMATIONFACEDATA* pData) override;
		MESHINFORMATIONFACEDATA* getDefaultData() override;
	};

	typedef std::shared_ptr <CMeshInformation_Properties> PMeshInformation_Properties;

}

#endif // __NMR_MESHINFORMATION_PROPERTIES

