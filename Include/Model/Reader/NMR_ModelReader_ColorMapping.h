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

NMR_ModelReader_ColorMapping.h defines the Model Reader Color Mapping Class.

--*/

#ifndef __NMR_MODELREADER_COLORMAPPING
#define __NMR_MODELREADER_COLORMAPPING

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include "Model/Classes/NMR_ModelTypes.h"

#include <map>
#include <memory>

namespace NMR {

	typedef struct {
		ModelResourceID m_nResourceID;
		ModelResourceIndex m_nResourceIndex;
	} READERCOLORMAPPINGTREEENTRY;

	bool operator< (_In_ const READERCOLORMAPPINGTREEENTRY & entry1, _In_ const READERCOLORMAPPINGTREEENTRY & entry2);

	class CModelReader_ColorMapping {
	protected:
		std::map <READERCOLORMAPPINGTREEENTRY, nfColor> m_ColorEntries;
		std::map <ModelResourceID, nfInt32> m_ResourceIDs;
		std::map <ModelResourceID, ModelResourceID> m_TextureMappings;
		std::map <ModelResourceID, ModelResourceIndex> m_MaterialMappings;
	public:
		CModelReader_ColorMapping();

		void registerColor(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nResourceIndex, _In_ nfColor cColor);
		nfBool findColor(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nResourceIndex, _Out_ nfColor & cColor);

		nfBool hasResource(_In_ ModelResourceID nResourceID);

		void registerTextureReference(_In_ ModelResourceID nResourceID, _In_ ModelResourceID nTextureID);
		nfBool hasTextureReference(_In_ ModelResourceID nResourceID);
		ModelResourceID getTextureReference(_In_ ModelResourceID nResourceID);

		void registerMaterialReference(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nMaterialIndex);
		nfBool hasMaterialReference(_In_ ModelResourceID nResourceID);
		nfBool getMaterialReference(_In_ ModelResourceID nResourceID, _Out_ ModelResourceIndex & nMaterialIndex);

		nfBool retrieveARessource(_Out_ ModelResourceID& nResourceID);
	};

	typedef std::shared_ptr <CModelReader_ColorMapping> PModelReader_ColorMapping;

}

#endif // __NMR_MODELREADER_COLORMAPPING


