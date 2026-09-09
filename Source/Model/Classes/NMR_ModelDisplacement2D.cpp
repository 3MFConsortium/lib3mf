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

#include "Model/Classes/NMR_ModelDisplacement2D.h"
#include "Model/Classes/NMR_ModelTexture2D.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include <algorithm>
#include <cctype>

namespace NMR {

	CModelDisplacement2DResource::CModelDisplacement2DResource(ModelResourceID nID, CModel * pModel, PModelAttachment pAttachment)
		: CModelResource(nID, pModel), m_eChannel(MODELCOLORCHANNEL_GREEN),
		m_eTileStyleU(MODELTEXTURETILESTYLE_WRAP), m_eTileStyleV(MODELTEXTURETILESTYLE_WRAP),
		m_eFilter(MODELTEXTUREFILTER_AUTO)
	{
		setAttachment(pAttachment);
	}

	PModelAttachment CModelDisplacement2DResource::getAttachment() const
	{
		return m_pAttachment;
	}

	void CModelDisplacement2DResource::validateAttachment(PModelAttachment pAttachment, CModel * pModel)
	{
		if (!pAttachment)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pAttachment->getModel() != pModel)
			throw CNMRException(NMR_ERROR_ATTACHMENTMODELMISMATCH);
		if (pAttachment->getRelationShipType() != PACKAGE_TEXTURE_RELATIONSHIP_TYPE)
			throw CNMRException(NMR_ERROR_INVALIDRELATIONSHIPTYPEFORTEXTURE);
		std::string sPath = pAttachment->getPathURI();
		std::transform(sPath.begin(), sPath.end(), sPath.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		if (sPath.size() < 4 || sPath.compare(sPath.size() - 4, 4, ".png") != 0)
			throw CNMRException(NMR_ERROR_INVALIDTEXTURETYPE);
	}

	void CModelDisplacement2DResource::setAttachment(PModelAttachment pAttachment)
	{
		validateAttachment(pAttachment, getModel());
		m_pAttachment = pAttachment;
	}

	eModelColorChannel CModelDisplacement2DResource::getChannel() const { return m_eChannel; }

	void CModelDisplacement2DResource::setChannel(eModelColorChannel eChannel)
	{
		if (eChannel < MODELCOLORCHANNEL_RED || eChannel > MODELCOLORCHANNEL_ALPHA)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_eChannel = eChannel;
	}

	void CModelDisplacement2DResource::setChannelString(const std::string & sValue)
	{
		if (sValue == "R") setChannel(MODELCOLORCHANNEL_RED);
		else if (sValue == "G") setChannel(MODELCOLORCHANNEL_GREEN);
		else if (sValue == "B") setChannel(MODELCOLORCHANNEL_BLUE);
		else if (sValue == "A") setChannel(MODELCOLORCHANNEL_ALPHA);
		else throw CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE);
	}

	std::string CModelDisplacement2DResource::getChannelString() const
	{
		switch (m_eChannel) {
		case MODELCOLORCHANNEL_RED: return "R";
		case MODELCOLORCHANNEL_GREEN: return "G";
		case MODELCOLORCHANNEL_BLUE: return "B";
		case MODELCOLORCHANNEL_ALPHA: return "A";
		default: return "G";
		}
	}

	eModelTextureTileStyle CModelDisplacement2DResource::getTileStyleU() const { return m_eTileStyleU; }
	eModelTextureTileStyle CModelDisplacement2DResource::getTileStyleV() const { return m_eTileStyleV; }
	void CModelDisplacement2DResource::setTileStyleU(eModelTextureTileStyle eStyle) { m_eTileStyleU = eStyle; }
	void CModelDisplacement2DResource::setTileStyleV(eModelTextureTileStyle eStyle) { m_eTileStyleV = eStyle; }
	void CModelDisplacement2DResource::setTileStyleUString(const std::string & sValue) { m_eTileStyleU = CModelTexture2DResource::tileStyleFromString(sValue); }
	void CModelDisplacement2DResource::setTileStyleVString(const std::string & sValue) { m_eTileStyleV = CModelTexture2DResource::tileStyleFromString(sValue); }
	eModelTextureFilter CModelDisplacement2DResource::getFilter() const { return m_eFilter; }
	void CModelDisplacement2DResource::setFilter(eModelTextureFilter eFilter) { m_eFilter = eFilter; }
	void CModelDisplacement2DResource::setFilterString(const std::string & sValue) { m_eFilter = CModelTexture2DResource::filterFromString(sValue); }
}
