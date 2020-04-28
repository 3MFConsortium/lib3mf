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

NMR_ModelReaderNode100_Mesh.h defines the Model Reader Mesh Node Class.
A mesh reader model node is a parser for the mesh node of an XML Model Stream.

--*/

#ifndef __NMR_MODELREADERNODE100_MESH
#define __NMR_MODELREADERNODE100_MESH

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Reader/NMR_ModelReader_TexCoordMapping.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelObject.h"

namespace NMR {

	class CModelReaderNode100_Mesh : public CModelReaderNode {
	private:
		CMesh * m_pMesh;
		CModel * m_pModel;

		PPackageResourceID m_pObjectLevelPropertyID;
		ModelResourceIndex m_nObjectLevelPropertyIndex;

		eModelBeamLatticeClipMode m_eClipMode;
		nfBool m_bHasClippingMeshID;
		ModelResourceID m_nClippingMeshID;
		nfBool m_bHasRepresentationMeshID;
		ModelResourceID m_nRepresentationMeshID;
	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
	public:
		CModelReaderNode100_Mesh() = delete;
		CModelReaderNode100_Mesh(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, _In_ PPackageResourceID m_pObjectLevelPropertyID, _In_ ModelResourceIndex nDefaultPropertyIndex);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);
		void retrieveClippingInfo(_Out_ eModelBeamLatticeClipMode &eClipMode, _Out_ nfBool & bHasClippingMode, _Out_ ModelResourceID & nClippingMeshID);
		void retrieveRepresentationInfo(_Out_ nfBool & bHasRepresentation, _Out_ ModelResourceID & nRepresentationMeshID);
	};
	typedef std::shared_ptr <CModelReaderNode100_Mesh> PModelReaderNode100_Mesh;
}

#endif // __NMR_MODELREADERNODE100_MESH
