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

NMR_ModelReaderNode_ExactGeometry1901_UVTCoord.h covers the official 3MF Exact Geometry Extension.


--*/

#ifndef __NMR_MODELREADERNODE_EXACTGEOMETRY1901_UVTCOORD
#define __NMR_MODELREADERNODE_EXACTGEOMETRY1901_UVTCOORD

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelNurbsSurface.h"

namespace NMR {

	class CModelReaderNode_ExactGeometry1901_UVTCoord : public CModelReaderNode {
	private:
		CModel * m_pModel;
		PModelReaderWarnings m_pWarnings;

		nfDouble m_U;
		nfBool m_bHasU;
		nfDouble m_V;
		nfBool m_bHasV;

	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
	public:
		CModelReaderNode_ExactGeometry1901_UVTCoord() = delete;
		CModelReaderNode_ExactGeometry1901_UVTCoord(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		void retrieveCoord(_Out_ nfDouble & dU, _Out_ nfDouble & dV);

	};

	typedef std::shared_ptr <CModelReaderNode_ExactGeometry1901_UVTCoord> PModelReaderNode_ExactGeometry1901_UVTCoord;

}

#endif // __NMR_MODELREADERNODE_EXACTGEOMETRY1901_UVTCOORD



