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

NMR_ModelReaderNode100_Triangle.h defines the Model Reader Triangle Node Class.
A triangle reader model node is a parser for the triangle node of an XML Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE100_TRIANGLE
#define __NMR_MODELREADERNODE100_TRIANGLE

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelObject.h"

namespace NMR {

	class CModelReaderNode100_Triangle : public CModelReaderNode {
	protected:
		nfInt32 m_nPropertyID;
		nfInt32 m_nPropertyIndex1;
		nfInt32 m_nPropertyIndex2;
		nfInt32 m_nPropertyIndex3;
		nfInt32 m_nIndex1;
		nfInt32 m_nIndex2;
		nfInt32 m_nIndex3;

		virtual void OnAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue);
		virtual void OnChildElement(_In_z_ const nfWChar * pChildName, _In_ CXmlReader * pXMLReader);
	public:
		CModelReaderNode100_Triangle() = delete;
		CModelReaderNode100_Triangle(_In_ PModelReaderWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);
		void retrieveIndices(_Out_ nfInt32 & nIndex1, _Out_ nfInt32 & nIndex2, _Out_ nfInt32 & nIndex3, nfInt32 nNodeCount);
		nfBool retrieveProperties(_Out_ ModelResourceID & nPropertyID, _Out_ ModelResourceIndex & nPropertyIndex1, _Out_ ModelResourceIndex & nPropertyIndex2, _Out_ ModelResourceIndex & nPropertyIndex3);
	};

	typedef std::shared_ptr <CModelReaderNode100_Triangle> PModelReaderNode100_Triangle;

}

#endif // __NMR_MODELREADERNODE100_TRIANGLE
