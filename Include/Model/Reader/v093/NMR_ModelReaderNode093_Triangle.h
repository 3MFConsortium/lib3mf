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

NMR_ModelReaderNode093_Triangle.h defines the Model Reader Triangle Node Class.
A triangle reader model node is a parser for the triangle node of an XML Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE093_TRIANGLE
#define __NMR_MODELREADERNODE093_TRIANGLE

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelObject.h"

namespace NMR {

	class CModelReaderNode093_Triangle : public CModelReaderNode {
	protected:
		nfInt32 m_nIndex1;
		nfInt32 m_nIndex2;
		nfInt32 m_nIndex3;
		nfInt32 m_nTextureIndex1;
		nfInt32 m_nTextureIndex2;
		nfInt32 m_nTextureIndex3;
		nfInt32 m_nColorID1;
		nfInt32 m_nColorID2;
		nfInt32 m_nColorID3;
		nfInt32 m_nMaterialID;
		nfInt32 m_nTextureID;

		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
	public:
		CModelReaderNode093_Triangle() = delete;
		CModelReaderNode093_Triangle(_In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);
		void retrieveIndices(_Out_ nfInt32 & nIndex1, _Out_ nfInt32 & nIndex2, _Out_ nfInt32 & nIndex3, nfInt32 nNodeCount);
		nfBool retrieveTextureIndices(_Out_ nfInt32 & nIndex1, _Out_ nfInt32 & nIndex2, _Out_ nfInt32 & nIndex3);

		nfBool retrieveColorIDs(_Out_ nfInt32 & nColorID1, _Out_ nfInt32 & nColorID2, _Out_ nfInt32 & nColorID3);
		nfInt32 retrieveTextureID();
		nfInt32 retrieveMaterialID();
	};

	typedef std::shared_ptr <CModelReaderNode093_Triangle> PModelReaderNode093_Triangle;

}

#endif // __NMR_MODELREADERNODE093_TRIANGLE
