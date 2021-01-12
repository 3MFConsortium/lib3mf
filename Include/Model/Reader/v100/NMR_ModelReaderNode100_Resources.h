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

NMR_ModelReaderNode100_Resources.h defines the Model Reader Resources Node Class.
A resources reader model node is a parser for the resources node of an XML Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE100_RESOURCES
#define __NMR_MODELREADERNODE100_RESOURCES

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelTexture2DGroup.h"

namespace NMR {

	class CModelReaderNode100_Resources : public CModelReaderNode {
	protected:
		CModel * m_pModel;
		std::string m_sPath;

		int m_nProgressCount;

		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar *  pAttributeValue);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
	public:
		CModelReaderNode100_Resources() = delete;
		CModelReaderNode100_Resources(_In_ CModel * pModel, _In_ PModelWarnings pWarnings, _In_z_ const std::string sPath, _In_ PProgressMonitor pProgressMonitor);
		virtual void parseXML(_In_ CXmlReader * pXMLReader);
	};

}

#endif // __NMR_MODELREADERNODE100_RESOURCES
