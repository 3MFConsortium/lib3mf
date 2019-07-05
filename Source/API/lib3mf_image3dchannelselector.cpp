/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CImage3DChannelSelector

*/

#include "lib3mf_image3dchannelselector.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_image3d.hpp"

#include "Model/Classes/NMR_ModelImage3D.h"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CImage3DChannelSelector 
**************************************************************************************************************************/

CImage3DChannelSelector::CImage3DChannelSelector(NMR::PModel pModel, NMR::PModelImage3DChannelSelector pSelector)
	:  m_pSelector (pSelector), m_pModel (pModel)
{
	if (pModel.get () == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	if (pSelector.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	
}


IImage3D * CImage3DChannelSelector::GetImage()
{
	auto pImage3D = m_pModel->findImage3D (m_pSelector->getImage3DID());
	if (pImage3D.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	return new CImage3D (pImage3D);	
}

void CImage3DChannelSelector::SetImage(IImage3D* pImage3D)
{
	if (pImage3D == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CImage3D * pImageClass = dynamic_cast<CImage3D *> (pImage3D);
	if (pImageClass == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	auto pNMRImage = pImageClass->getModelImage3D();
	if (pNMRImage.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	m_pSelector->setImage3DID(pNMRImage->getResourceID());
}

void CImage3DChannelSelector::SetSourceChannel(const std::string & sChannelName)
{
	m_pSelector->setSourceChannel(sChannelName);
}

std::string CImage3DChannelSelector::GetSourceChannel()
{
	return m_pSelector->getSourceChannel();
}

void CImage3DChannelSelector::SetDestinationChannel(const std::string & sChannelName)
{
	m_pSelector->setDstChannel(sChannelName);
}

std::string CImage3DChannelSelector::GetDestinationChannel()
{
	return m_pSelector->getDstChannel();
}

void CImage3DChannelSelector::SetFilter(const Lib3MF::eTextureFilter eFilter)
{
	m_pSelector->setFilter(NMR::eModelTextureFilter(eFilter));

}

Lib3MF::eTextureFilter CImage3DChannelSelector::GetFilter()
{
	return eLib3MFTextureFilter(m_pSelector->getFilter());
}

void CImage3DChannelSelector::SetTileStyles(const Lib3MF::eTextureTileStyle eTileStyleU, const Lib3MF::eTextureTileStyle eTileStyleV, const Lib3MF::eTextureTileStyle eTileStyleW)
{
	m_pSelector->setTileStyleU(NMR::eModelTextureTileStyle(eTileStyleU));
	m_pSelector->setTileStyleV(NMR::eModelTextureTileStyle(eTileStyleV));
	m_pSelector->setTileStyleW(NMR::eModelTextureTileStyle(eTileStyleW));
}

void CImage3DChannelSelector::GetTileStyles(Lib3MF::eTextureTileStyle & eTileStyleU, Lib3MF::eTextureTileStyle & eTileStyleV, Lib3MF::eTextureTileStyle & eTileStyleW)
{
	eTileStyleU = eLib3MFTextureTileStyle(m_pSelector->getTileStyleU());
	eTileStyleV = eLib3MFTextureTileStyle(m_pSelector->getTileStyleV());
	eTileStyleW = eLib3MFTextureTileStyle(m_pSelector->getTileStyleW());
}

void CImage3DChannelSelector::SetValueRange(const Lib3MF_double dMin, const Lib3MF_double dMax)
{
	m_pSelector->setMinValue(dMin);
	m_pSelector->setMaxValue(dMax);
}

void CImage3DChannelSelector::GetValueRange(Lib3MF_double & dMin, Lib3MF_double & dMax)
{
	dMin = m_pSelector->getMinValue();
	dMax = m_pSelector->getMaxValue();
}


NMR::PModelImage3DChannelSelector CImage3DChannelSelector::getModelSelector()
{
	return m_pSelector;
}

