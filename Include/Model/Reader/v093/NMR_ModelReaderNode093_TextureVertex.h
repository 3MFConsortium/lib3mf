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

NMR_ModelReaderNode_TextureVertex.h defines the Model Reader Texture Vertex Node Class.
A texture vertex reader model node is a parser for the texture vertex node of an XML Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE093_TEXTUREVERTEX
#define __NMR_MODELREADERNODE093_TEXTUREVERTEX

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelObject.h"

namespace NMR {

	class CModelReaderNode093_TextureVertex : public CModelReaderNode {
	private:
		nfFloat m_fU;
		nfFloat m_fV;
		nfBool m_bHasU;
		nfBool m_bHasV;
	public:
		CModelReaderNode093_TextureVertex() = delete;
		CModelReaderNode093_TextureVertex(_In_ PModelWarnings pWarnings);

		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		void retrieveTexCoords(_Out_ nfFloat & fU, _Out_ nfFloat & fV);
	};

	typedef std::shared_ptr <CModelReaderNode093_TextureVertex> PModelReaderNode093_TextureVertex;

}

#endif // __NMR_MODELREADERNODE093_TEXTUREVERTEX
