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
NMR_ModelReaderNode_Volumetric1907_Image3DChannelSelector.h covers the official 3MF volumetric extension.

--*/

#ifndef __NMR_MODELREADERNODE_VOLUMETRIC1907_IMAGE3DCHANNELSELECTOR
#define __NMR_MODELREADERNODE_VOLUMETRIC1907_IMAGE3DCHANNELSELECTOR

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_ModelObject.h"

#include "Model/Classes/NMR_Model.h"

#include "Model/Classes/NMR_ModelImage3DChannelSelector.h"

namespace NMR {

	class CModelReaderNode_Volumetric1907_Image3DChannelSelector : public CModelReaderNode {
	private:

		nfUint32 m_nID;
		std::string m_sSourceChannel;
		std::string m_sDstChannel;

		nfBool m_bHasMinValue;
		nfBool m_bHasMaxValue;
		nfDouble m_dMinValue;
		nfDouble m_dMaxValue;

		nfBool m_bHasTileStyleU;
		nfBool m_bHasTileStyleV;
		nfBool m_bHasTileStyleW;
		eModelTextureTileStyle m_eTileStyleU;
		eModelTextureTileStyle m_eTileStyleV;
		eModelTextureTileStyle m_eTileStyleW;

		nfBool m_bHasFilter;
		eModelTextureFilter m_eFilter;

	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
	public:
		CModelReaderNode_Volumetric1907_Image3DChannelSelector() = delete;
		CModelReaderNode_Volumetric1907_Image3DChannelSelector(_In_ PModelReaderWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		PModelImage3DChannelSelector createSelector(CModel * pModel);

	};

	typedef std::shared_ptr <CModelReaderNode_Volumetric1907_Image3DChannelSelector> PModelReaderNode_Volumetric1907_Image3DChannelSelector;

}

#endif // __NMR_MODELREADERNODE_VOLUMETRIC1907_IMAGE3DCHANNELSELECTOR
