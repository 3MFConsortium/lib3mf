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

NMR_ModelReaderNode100_Composite.h defines the Model Reader Tex2Coord Node Class.

--*/

#ifndef __NMR_MODELREADERNODE100_COMPOSITE
#define __NMR_MODELREADERNODE100_COMPOSITE

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelCompositeMaterials.h"
#include "Model/Classes/NMR_ModelTypes.h"

namespace NMR {

	class CModelReaderNode100_Composite : public CModelReaderNode {
	private:
		CModel * m_pModel;

		PModelComposite m_pModelComposite;
		const std::vector<ModelPropertyID>& m_vctBaseMaterialPropertyIds;
	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
	public:
		CModelReaderNode100_Composite() = delete;
		CModelReaderNode100_Composite(_In_ CModel * pModel, _In_ PModelWarnings pWarnings, _In_z_ const std::vector<ModelPropertyID>& baseMaterialPropertyIds);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		PModelComposite getComposite();
	};

	typedef std::shared_ptr<CModelReaderNode100_Composite> PModelReaderNode100_Composite;

}

#endif // __NMR_MODELREADERNODE100_COMPOSITE


