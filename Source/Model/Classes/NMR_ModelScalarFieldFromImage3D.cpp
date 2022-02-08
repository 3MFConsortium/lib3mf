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

NMR_ModelScalarFieldFromImage3D.cpp implements the Model ScalarFieldFromImage3D Class.
It selects a single channel from an Image3D, scales and offsets the values, 
interpolates in UVW and provides a scalar field.
--*/

#include "Model/Classes/NMR_ModelScalarFieldFromImage3D.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	CModelScalarFieldFromImage3D::CModelScalarFieldFromImage3D(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ PPackageResourceID Image3DID)
		: CModelScalarField(sID, pModel), m_Image3DResourceID(Image3DID)
	{
	
	}

	PPackageResourceID CModelScalarFieldFromImage3D::getImage3DResourceID() const
	{
		return m_Image3DResourceID;
	}

	void CModelScalarFieldFromImage3D::setImage3DResourceID(PPackageResourceID Image3DID)
	{
		if (!getModel()->findImage3D(Image3DID))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_Image3DResourceID = Image3DID;
	}

	nfDouble CModelScalarFieldFromImage3D::getOffset() const
	{
		return m_dOffset;
	}
	void CModelScalarFieldFromImage3D::setOffset(nfDouble dOffset)
	{
		m_dOffset = dOffset;
	}
	nfDouble CModelScalarFieldFromImage3D::getScale() const
	{
		return m_dScale;
	}
	void CModelScalarFieldFromImage3D::setScale(nfDouble dScale)
	{
		m_dScale = dScale;
	}

	eModelColorChannel CModelScalarFieldFromImage3D::getChannel() const
	{
		return m_eChannel;
	}
	void CModelScalarFieldFromImage3D::setChannel(eModelColorChannel eColorChannel)
	{
		m_eChannel = eColorChannel;
	}

	eModelTextureFilter CModelScalarFieldFromImage3D::getFilter() const
	{
		return m_eFilter;
	}

	void CModelScalarFieldFromImage3D::setFilter(eModelTextureFilter eFilter)
	{
		m_eFilter = eFilter;
	}

	eModelTextureTileStyle CModelScalarFieldFromImage3D::getTileStyleU() const
	{
		return m_eTileStyleU;
	}
	void CModelScalarFieldFromImage3D::setTileStyleU(eModelTextureTileStyle eTileStyleU)
	{
		m_eTileStyleU = eTileStyleU;
	}
	eModelTextureTileStyle CModelScalarFieldFromImage3D::getTileStyleV() const
	{
		return m_eTileStyleV;
	}
	void CModelScalarFieldFromImage3D::setTileStyleV(eModelTextureTileStyle eTileStyleV)
	{
		m_eTileStyleV = eTileStyleV;
	}
	eModelTextureTileStyle CModelScalarFieldFromImage3D::getTileStyleW() const
	{
		return m_eTileStyleW;
	}
	void CModelScalarFieldFromImage3D::setTileStyleW(eModelTextureTileStyle eTileStyleW)
	{
		m_eTileStyleU = eTileStyleW;
	}

	std::string CModelScalarFieldFromImage3D::channelToString(_In_ eModelColorChannel eChannel)
	{
		switch (eChannel) {
		case eModelColorChannel::MODELCOLORCHANNEL_RED: return XML_3MF_ATTRIBUTE_SCALARFIELDFROMIMAGE3_CHANNEL_RED;
		case eModelColorChannel::MODELCOLORCHANNEL_GREEN: return XML_3MF_ATTRIBUTE_SCALARFIELDFROMIMAGE3_CHANNEL_GREEN;
		case eModelColorChannel::MODELCOLORCHANNEL_BLUE: return XML_3MF_ATTRIBUTE_SCALARFIELDFROMIMAGE3_CHANNEL_BLUE;
		case eModelColorChannel::MODELCOLORCHANNEL_ALPHA: return XML_3MF_ATTRIBUTE_SCALARFIELDFROMIMAGE3_CHANNEL_ALPHA;
		default:
			return XML_3MF_ATTRIBUTE_SCALARFIELDFROMIMAGE3_CHANNEL_RED;
		}
	}

	eModelColorChannel CModelScalarFieldFromImage3D::channelFromString(_In_ std::string sValue)
	{
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_SCALARFIELDFROMIMAGE3_CHANNEL_RED) == 0)
			return MODELCOLORCHANNEL_RED;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_SCALARFIELDFROMIMAGE3_CHANNEL_GREEN) == 0)
			return MODELCOLORCHANNEL_GREEN;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_SCALARFIELDFROMIMAGE3_CHANNEL_BLUE) == 0)
			return MODELCOLORCHANNEL_BLUE;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_SCALARFIELDFROMIMAGE3_CHANNEL_ALPHA) == 0)
			return MODELCOLORCHANNEL_ALPHA;
		throw CNMRException(-1); // TODO: #define NMR_ERROR_INVALIDCHANNELNAME
		return MODELCOLORCHANNEL_RED;
	}

}
