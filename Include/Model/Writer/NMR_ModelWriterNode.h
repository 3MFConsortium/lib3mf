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

NMR_ModelWriterNode.h defines the Model Writer Node Class.
This is the abstract base class for all 3MF model stream exporters.

--*/

#ifndef __NMR_MODELWRITERNODE
#define __NMR_MODELWRITERNODE

#include "Common/Platform/NMR_XmlWriter.h"
#include "Common/3MF_ProgressMonitor.h"

namespace NMR {

	class CModelWriterNode {
	protected:
		CXmlWriter * m_pXMLWriter;
		PProgressMonitor m_pProgressMonitor;

		void writeStringAttribute(_In_z_ const nfChar * pAttributeName, _In_ std::string sAttributeValue);
		void writePrefixedStringAttribute(_In_z_ const nfChar * pPrefix, _In_ const nfChar * pAttributeName, std::string sAttributeValue);

		void writeConstStringAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		void writeConstPrefixedStringAttribute(_In_z_ const nfChar * pPrefix, _In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);

		void writeIntAttribute(_In_z_ const nfChar * pAttributeName, _In_ nfInt32 nAttributeValue);
		void writeUintAttribute(_In_z_ const nfChar * pAttributeName, _In_ nfUint32 nAttributeValue);
		void writeFloatAttribute(_In_z_ const nfChar * pAttributeName, _In_ nfFloat fAttributeValue);

		void writeStartElement(_In_z_ const nfChar * pElementName);
		void writeStartElementWithNamespace(_In_z_ const nfChar * pElementName, _In_z_ const nfChar * pNameSpace);
		void writeStartElementWithPrefix(_In_z_ const nfChar * pElementName, _In_z_ const nfChar * pPrefix);
		void writeEndElement();
		void writeFullEndElement();
		void writeText(_In_z_ const nfChar * pwszText, _In_ nfUint32 cbLength);
	public:
		CModelWriterNode() = delete;
		CModelWriterNode(_In_ CXmlWriter * pXMLWriter, _In_ PProgressMonitor pProgressMonitor);

		virtual void writeToXML () = 0;
	};

}

#endif // __NMR_MODELWRITERNODE
