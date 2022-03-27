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
NMR_ModelReaderNode_Volumetric2201_ScalarFieldComposed.h covers the official 3MF volumetric extension.

--*/

#ifndef __NMR_MODELREADERNODE_VOLUMETRIC2201_SCALARFIELDCOMPOSED
#define __NMR_MODELREADERNODE_VOLUMETRIC2201_SCALARFIELDCOMPOSED

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelScalarFieldComposed.h"

namespace NMR {

	class CModelReaderNode_Volumetric2201_ScalarFieldComposed : public CModelReaderNode {
	private:
	protected:
		CModel* m_pModel;

		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
	public:
		nfBool m_bHasMethod = false;
		eModelCompositionMethod m_eMethod = eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MAX;

		nfBool m_bHasScalarFieldId1 = false;
		PPackageResourceID m_pScalarFieldId1 = nullptr;
		nfBool m_bHasScalarFieldId2 = false;
		PPackageResourceID m_pScalarFieldId2 = nullptr;

		nfBool m_bHasTransform1 = false;
		NMR::NMATRIX3 m_mTransform1 = NMR::fnMATRIX3_identity();
		nfBool m_bHasTransform2 = false;
		NMR::NMATRIX3 m_mTransform2 = NMR::fnMATRIX3_identity();;

		nfBool m_bHasFactor1 = false;
		double m_dFactor1 = 1.;
		nfBool m_bHasFactor2 = false;
		double m_dFactor2 = 1.;

		nfBool m_bHasScalarFieldIdMask = false;
		PPackageResourceID m_pScalarFieldIdMask = nullptr;
		nfBool m_bHasTransformMask = false;
		NMR::NMATRIX3 m_mTransformMask = NMR::fnMATRIX3_identity();;

		CModelReaderNode_Volumetric2201_ScalarFieldComposed() = delete;
		CModelReaderNode_Volumetric2201_ScalarFieldComposed(_In_ CModel* pModel, _In_ PModelWarnings pWarnings);

		PModelScalarFieldComposed makeScalarFieldComposed(ModelResourceID);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);
	};

	typedef std::shared_ptr<CModelReaderNode_Volumetric2201_ScalarFieldComposed> PModelReaderNode_Volumetric2201_ScalarFieldComposed;
}

#endif // __NMR_MODELREADERNODE_VOLUMETRIC2201_SCALARFIELDCOMPOSED
