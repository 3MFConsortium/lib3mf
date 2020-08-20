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
NMR_ModelReaderNode_BeamLattice1702_BeamLattice.h covers the official 3MF beamlattice extension.
(Initial version based on private beam extension)


--*/

#ifndef __NMR_MODELREADERNODE_BEAMLATTICE1702_BEAMLATTICE
#define __NMR_MODELREADERNODE_BEAMLATTICE1702_BEAMLATTICE

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_ModelObject.h"

namespace NMR {

	class CModelReaderNode_BeamLattice1702_BeamLattice : public CModelReaderNode {
	private:
		CModel * m_pModel;
		CMesh * m_pMesh;

		eModelBeamLatticeClipMode m_eClipMode;
		nfBool m_bHasClippingMeshID;
		ModelResourceID m_nClippingMeshID;
		nfBool m_bHasRepresentationMeshID;
		ModelResourceID m_nRepresentationMeshID;
		nfDouble m_dDefaultRadius;
		eModelBeamLatticeCapMode m_eDefaultCapMode;
		nfDouble m_dDefaultBallRadius;
	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
	public:
		CModelReaderNode_BeamLattice1702_BeamLattice() = delete;
		CModelReaderNode_BeamLattice1702_BeamLattice(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelWarnings pWarnings);

		void retrieveClippingInfo(_Out_ eModelBeamLatticeClipMode &eClipMode, _Out_ nfBool & bHasClippingMode, _Out_ ModelResourceID & nClippingMeshID);
		void retrieveRepresentationInfo(_Out_ nfBool & bHasRepresentation, _Out_ ModelResourceID & nRepresentationMeshID);
		void validateBallOptions(_In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

	};

	typedef std::shared_ptr <CModelReaderNode_BeamLattice1702_BeamLattice> PModelReaderNode_BeamLattice1702_BeamLattice;

}

#endif // __NMR_MODELREADERNODE_BEAMLATTICE1702_BEAMLATTICE
