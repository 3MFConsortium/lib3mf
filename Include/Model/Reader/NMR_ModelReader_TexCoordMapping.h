/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

NMR_ModelReader_TexcoordMapping.h defines the Model Reader Texture Mapping Class.

--*/

#ifndef __NMR_MODELREADER_TEXCOORDMAPPING
#define __NMR_MODELREADER_TEXCOORDMAPPING

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include "Model/Classes/NMR_ModelTypes.h"

#include <map>
#include <memory>

namespace NMR {

	typedef struct {
		ModelResourceID m_nResourceID;
		ModelResourceIndex m_nResourceIndex;
	} READERTEXCOORDMAPPINGTREEENTRY;

	typedef struct {
		ModelResourceID m_nTextureID;
		nfFloat m_fU;
		nfFloat m_fV;
	} READERTEXCOORDMAPPINGVALUE;

	bool operator< (_In_ const READERTEXCOORDMAPPINGTREEENTRY & entry1, _In_ const READERTEXCOORDMAPPINGTREEENTRY & entry2);

	class CModelReader_TexCoordMapping {
	protected:
		std::map <READERTEXCOORDMAPPINGTREEENTRY, READERTEXCOORDMAPPINGVALUE> m_TexCoordEntries;
		std::map <ModelResourceID, nfInt32> m_ResourceIDs;
	public:
		CModelReader_TexCoordMapping();

		void registerTexCoords(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nResourceIndex, _In_ ModelResourceID nTextureID, _In_ nfFloat fU, _In_ nfFloat fV);
		nfBool findTexCoords(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nResourceIndex, _Out_ ModelResourceID & nTextureID, _Out_ nfFloat & fU, _Out_ nfFloat & fV);

		nfBool hasResource(_In_ ModelResourceID nResourceID);
		nfBool retrieveARessource(_Out_ ModelResourceID& nResourceID);
	};

	typedef std::shared_ptr <CModelReader_TexCoordMapping> PModelReader_TexCoordMapping;

}

#endif // __NMR_MODELREADER_TEXCOORDMAPPING

