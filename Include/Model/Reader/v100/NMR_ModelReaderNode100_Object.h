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

NMR_ModelReaderNode100_Object.h defines the Model Reader Object Node Class.
A object reader model node is a parser for the object node of an XML Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE100_OBJECT
#define __NMR_MODELREADERNODE100_OBJECT

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Mesh.h"
#include "Model/Reader/NMR_ModelReader_ColorMapping.h"
#include "Model/Reader/NMR_ModelReader_TexCoordMapping.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_ModelObject.h"


namespace NMR {

	class CModelReaderNode100_Object : public CModelReaderNode {
	private:
		CModel * m_pModel;
		ModelResourceID m_nID;
		nfBool m_bHasThumbnail;
		std::wstring m_sThumbnail;
		std::wstring m_sPartNumber;
		std::wstring m_sName;
		PUUID m_UUID;
		std::wstring m_sType;
		nfBool m_bHasType;
		PModelObject m_pObject;
		PModelReader_ColorMapping m_pColorMapping;
		PModelReader_TexCoordMapping m_pTexCoordMapping;
		nfBool m_bHasDefaultPropertyID;
		nfBool m_bHasDefaultPropertyIndex;
		ModelResourceID m_nDefaultPropertyID;
		ModelResourceIndex m_nDefaultPropertyIndex;

		nfUint32 m_nSliceStackId;
		eModelSlicesMeshResolution m_eSlicesMeshResolution;
		nfBool m_bHasMeshResolution;

		void createDefaultProperties();
		void handleBeamLatticeExtension(CModelReaderNode100_Mesh* pXMLNode);
	protected:
    virtual void OnNSAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue, _In_z_ const nfWChar * pNameSpace);
		virtual void OnAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue);
		virtual void OnNSChildElement(_In_z_ const nfWChar * pChildName, _In_z_ const nfWChar * pNameSpace, _In_ CXmlReader * pXMLReader);
	public:
		CModelReaderNode100_Object() = delete;
		CModelReaderNode100_Object(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelReader_TexCoordMapping pTexCoordMapping);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);
	};

}

#endif // __NMR_MODELREADERNODE100_OBJECT
