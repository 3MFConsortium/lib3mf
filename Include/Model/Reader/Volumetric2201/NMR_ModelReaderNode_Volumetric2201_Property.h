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
NMR_ModelReaderNode_Volumetric2201_Property.h covers the official 3MF volumetric extension.

--*/

#ifndef __NMR_MODELREADERNODE_VOLUMETRIC2201_PROPERTY
#define __NMR_MODELREADERNODE_VOLUMETRIC2201_PROPERTY

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelVolumeData.h"
#include "Common/Math/NMR_Matrix.h"

namespace NMR {

	class CModelReaderNode_Volumetric2201_Property : public CModelReaderNode {
	private:
		nfBool m_bHasFieldID = false;
		nfBool m_bHasName = false;
		nfBool m_bHasTransform = false;
		nfBool m_bHasRequired = false;
		nfBool m_bHasChannel = false;

		ModelResourceID m_nFieldID;
		std::string m_sName;
		NMATRIX3 m_Transform;
		nfBool m_bRequired = true;
		std::string m_sChannel;

		double m_dMinFeatureSize = 0.0;
		nfBool m_bHasMinFeatureSize = false;
	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
	public:
		CModelReaderNode_Volumetric2201_Property() = delete;
		CModelReaderNode_Volumetric2201_Property(_In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);

		PVolumeDataProperty MakeProperty(_In_ CModel* pModel);
	};

	typedef std::shared_ptr <CModelReaderNode_Volumetric2201_Property> PModelReaderNode_Volumetric2201_Property;

}

#endif // __NMR_MODELREADERNODE_VOLUMETRIC2201_PROPERTY

