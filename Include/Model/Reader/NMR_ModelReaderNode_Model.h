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

NMR_ModelReaderNode_Model.h defines the Model Reader Model Node Class.
A model reader model node is a parser for the root node of an XML Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE_MODEL
#define __NMR_MODELREADERNODE_MODEL

#include "Model/Reader/NMR_ModelReaderNode.h"

namespace NMR {

	class CModelReaderNode_Model : public CModelReaderNode {
	protected:
		CModel * m_pModel;
		std::string m_sRequiredExtensions;
		std::map<std::string, std::string> m_ListedExtensions;

		std::string m_sPath;
		nfBool m_bHasResources;
		nfBool m_bHasBuild;

		nfBool m_bWithinIgnoredBuild;
		nfBool m_bIgnoreBuild;

		virtual void CheckRequiredExtensions();

		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace);
	public:
		CModelReaderNode_Model() = delete;
		CModelReaderNode_Model(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings, const std::string sPath, _In_ CProgressMonitor* pProgressMonitor);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		nfBool getHasResources();
		nfBool getHasBuild();

		nfBool ignoreBuild();
		void setIgnoreBuild(bool bIgnoreBuild);
	};

	typedef std::shared_ptr <CModelReaderNode_Model> PModelReaderNode_Model;

}

#endif // __NMR_MODELREADERNODE_MODEL
