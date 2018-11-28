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

NMR_ModelReaderNode_Triangles.h defines the Model Reader Triangles Node Class.
A triangles reader model node is a parser for the triangles node of an XML Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE100_TRIANGLES
#define __NMR_MODELREADERNODE100_TRIANGLES

#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h"
#include "Common/MeshInformation/NMR_MeshInformation_NodeColors.h"
#include "Common/MeshInformation/NMR_MeshInformation_BaseMaterials.h"
#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Reader/NMR_ModelReader_ColorMapping.h"
#include "Model/Reader/NMR_ModelReader_TexCoordMapping.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelObject.h"

namespace NMR {

	class CModelReaderNode100_Triangles : public CModelReaderNode {
	protected:
		CMesh * m_pMesh;
		CModel * m_pModel;
		PModelReader_ColorMapping m_pColorMapping;
		PModelReader_TexCoordMapping m_pTexCoordMapping;

		ModelResourceID m_nDefaultPropertyID;
		ModelResourceIndex m_nDefaultPropertyIndex;
		ModelResourceID m_nUsedPropertyID;

		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);

		_Ret_notnull_ CMeshInformation_TexCoords * createTexCoordInformation();
		_Ret_notnull_ CMeshInformation_NodeColors * createNodeColorInformation();
		_Ret_notnull_ CMeshInformation_BaseMaterials * createBaseMaterialInformation();
	public:
		CModelReaderNode100_Triangles() = delete;
		CModelReaderNode100_Triangles(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelReaderWarnings pWarnings, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelReader_TexCoordMapping pTexCoordMapping, _In_ ModelResourceID nDefaultPropertyID, _In_ ModelResourceIndex nDefaultPropertyIndex);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);
		ModelResourceID getUsedPropertyID() const;
	};

	typedef std::shared_ptr <CModelReaderNode100_Triangles> PModelReaderNode100_Triangles;

}

#endif // __NMR_MODELREADERNODE100_TRIANGLES
