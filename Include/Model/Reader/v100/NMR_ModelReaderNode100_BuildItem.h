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

NMR_ModelReaderNode100_BuildItem.h defines the Model Reader BuildItem Node Class.
A builditem reader model node is a parser for the builditem node of an XML Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE100_BUILDITEM
#define __NMR_MODELREADERNODE100_BUILDITEM

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelMetaDataGroup.h"

namespace NMR {

	class CModelReaderNode100_BuildItem : public CModelReaderNode {
	private:
		CModel * m_pModel;
		nfInt32 m_ObjectID;
		nfBool m_bHasID;
		std::string m_sPartNumber;
		NMATRIX3 m_mTransform;

		// Production Extension
		std::string m_sPath;
		PUUID m_UUID;

		bool m_hasPath;
		PModelMetaDataGroup m_MetaDataGroup;
	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
	public:
		CModelReaderNode100_BuildItem() = delete;
		CModelReaderNode100_BuildItem(_In_ CModel * pModel, _In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);
	};

}

#endif // __NMR_MODELREADERNODE100_BUILDITEM
