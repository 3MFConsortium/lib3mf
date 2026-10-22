/*++

Copyright (C) 2026 3MF Consortium

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

In-memory representation and serialization support for the 3MF Displacement extension.

--*/

#ifndef __NMR_MODELREADERNODE_DISPLACEMENT2310
#define __NMR_MODELREADERNODE_DISPLACEMENT2310

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelDisplacement2D.h"
#include "Model/Classes/NMR_ModelNormVectorGroup.h"
#include "Model/Classes/NMR_ModelDisp2DGroup.h"
#include <set>

namespace NMR {

	enum class eDisplacementResourceKind { Displacement2D, NormVectorGroup, Disp2DGroup };

	class CModelReaderNode_Displacement2310_Resource : public CModelReaderNode {
	private:
		CModel * m_pModel;
		eDisplacementResourceKind m_Kind;
		ModelResourceID m_nID, m_nDispID, m_nNormID;
		std::string m_sPath, m_sChannel, m_sTileStyleU, m_sTileStyleV, m_sFilter;
		nfDouble m_dHeight, m_dOffset;
		bool m_bHasHeight;
		std::set<std::string> m_Attributes;
		PModelNormVectorGroupResource m_pNormGroup;
		PModelDisp2DGroupResource m_pDispGroup;
	protected:
		void OnAttribute(const nfChar * pName, const nfChar * pValue) override;
		void OnNSChildElement(const nfChar * pName, const nfChar * pNamespace, CXmlReader * pXMLReader) override;
	public:
		CModelReaderNode_Displacement2310_Resource() = delete;
		CModelReaderNode_Displacement2310_Resource(CModel * pModel, PModelWarnings pWarnings, eDisplacementResourceKind kind);
		void parseXML(CXmlReader * pXMLReader) override;
	};
}

#endif
