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

NMR_ModelReaderNode.h defines the Model Reader Node Class.
A model reader node is an abstract base class for all XML nodes of a 3MF Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE
#define __NMR_MODELREADERNODE

#include "Model/Classes/NMR_Model.h"
#include "Common/NMR_ModelWarnings.h"
#include "Common/Platform/NMR_XmlReader.h"
#include "Common/3MF_ProgressMonitor.h"

namespace NMR {

	class CModelReaderNode {
	private:
		std::string m_sName;
		nfBool m_bParsedAttributes;
		nfBool m_bParsedContent;
		nfBool m_bIsEmptyElement;

	protected:
		PProgressMonitor m_pProgressMonitor;
		PModelWarnings m_pWarnings;

		void parseName(_In_ CXmlReader * pXMLReader);
		void parseAttributes(_In_ CXmlReader * pXMLReader);
		void parseContent(_In_ CXmlReader * pXMLReader);

		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnText(_In_z_ const nfChar * pText, _In_ CXmlReader * pXMLReader);
		virtual void OnEndElement(_In_ CXmlReader * pXMLReader);

		virtual void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
	public:
		CModelReaderNode() = delete;
		CModelReaderNode(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor = nullptr);
		virtual ~CModelReaderNode() = default;

		std::string getName();
		PModelWarnings getWarnings() const;

		virtual void parseXML(_In_ CXmlReader * pXMLReader) = 0;
	};

	typedef std::shared_ptr <CModelReaderNode> PModelReaderNode;

}

#endif // __NMR_MODELREADERNODE
