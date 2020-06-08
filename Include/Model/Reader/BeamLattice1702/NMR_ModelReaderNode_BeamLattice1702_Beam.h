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
NMR_ModelReaderNode_BeamLattice1702_Beam.h covers the official 3MF beamlattice extension.
(Initial version based on private beam extension)

--*/

#ifndef __NMR_MODELREADERNODE_BEAMLATTICE1702_BEAM
#define __NMR_MODELREADERNODE_BEAMLATTICE1702_BEAM

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_ModelObject.h"

namespace NMR {
	eModelBeamLatticeCapMode stringToCapMode(const nfChar * capModeStr);

	class CModelReaderNode_BeamLattice1702_Beam : public CModelReaderNode {
	private:
		nfInt32 m_nIndex1;
		nfInt32 m_nIndex2;
		nfBool m_bHasRadius1;
		nfBool m_bHasRadius2;
		nfDouble m_dRadius1;
		nfDouble m_dRadius2;
		nfBool m_bHasTag;
		nfInt32 m_nTag;
		nfBool m_bHasCap1;
		nfBool m_bHasCap2;
		eModelBeamLatticeCapMode m_eCapMode1;
		eModelBeamLatticeCapMode m_eCapMode2;
	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace);
	public:
		CModelReaderNode_BeamLattice1702_Beam() = delete;
		CModelReaderNode_BeamLattice1702_Beam(_In_ CModel * pModel, _In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		void retrieveIndices(_Out_ nfInt32 & nIndex1, _Out_ nfInt32 & nIndex2, nfInt32 nNodeCount);
		void retrieveRadii(_Out_ nfBool & bHasRadius1, _Out_ nfDouble & dRadius1, _Out_ nfBool & bHasRadius2, _Out_ nfDouble & dRadius2);
		void retrieveCapModes(_Out_ nfBool & bHasCapMode1, _Out_ eModelBeamLatticeCapMode & eCapMode1, _Out_ nfBool & bHasCapMode2, _Out_ eModelBeamLatticeCapMode & eCapMode2);
		void retrieveTag(_Out_ nfBool & bHasTag, _Out_ nfInt32 & nTag);
	};

	typedef std::shared_ptr <CModelReaderNode_BeamLattice1702_Beam> PModelReaderNode_BeamLattice1702_Beam;

}

#endif // __NMR_MODELREADERNODE_BEAMLATTICE1702_BEAM
