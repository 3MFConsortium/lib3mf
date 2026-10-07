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

#ifndef __NMR_MODELDISPLACEMENT2D
#define __NMR_MODELDISPLACEMENT2D

#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelAttachment.h"

namespace NMR {

	class CModelDisplacement2DResource : public CModelResource {
	private:
		PModelAttachment m_pAttachment;
		eModelColorChannel m_eChannel;
		eModelTextureTileStyle m_eTileStyleU;
		eModelTextureTileStyle m_eTileStyleV;
		eModelTextureFilter m_eFilter;

	public:
		CModelDisplacement2DResource() = delete;
		CModelDisplacement2DResource(_In_ ModelResourceID nID, _In_ CModel * pModel, _In_ PModelAttachment pAttachment);
		static void validateAttachment(_In_ PModelAttachment pAttachment, _In_ CModel * pModel);

		PModelAttachment getAttachment() const;
		void setAttachment(_In_ PModelAttachment pAttachment);

		eModelColorChannel getChannel() const;
		void setChannel(_In_ eModelColorChannel eChannel);
		void setChannelString(_In_ const std::string & sValue);
		std::string getChannelString() const;

		eModelTextureTileStyle getTileStyleU() const;
		eModelTextureTileStyle getTileStyleV() const;
		void setTileStyleU(_In_ eModelTextureTileStyle eStyle);
		void setTileStyleV(_In_ eModelTextureTileStyle eStyle);
		void setTileStyleUString(_In_ const std::string & sValue);
		void setTileStyleVString(_In_ const std::string & sValue);

		eModelTextureFilter getFilter() const;
		void setFilter(_In_ eModelTextureFilter eFilter);
		void setFilterString(_In_ const std::string & sValue);
	};

	typedef std::shared_ptr<CModelDisplacement2DResource> PModelDisplacement2DResource;
}

#endif
