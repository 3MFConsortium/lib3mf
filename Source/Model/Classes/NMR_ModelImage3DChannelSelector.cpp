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

NMR_ModelImage3DChannelSelector.cpp implements a 3D channel selector for the volumetric extension.

--*/

#include "Model/Classes/NMR_ModelImage3DChannelSelector.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {


	CModelImage3DChannelSelector::CModelImage3DChannelSelector(PPackageResourceID sImage3DId, std::string sSourceChannel, std::string sDstChannel)
		: m_sImage3DID (sImage3DId), 
		  m_sSourceChannel (sSourceChannel), 
		  m_sDstChannel (sDstChannel),
		  m_eFilter (MODELTEXTUREFILTER_LINEAR),
		  m_eTileStyleU (MODELTEXTURETILESTYLE_WRAP),
		  m_eTileStyleV (MODELTEXTURETILESTYLE_WRAP),
		  m_eTileStyleW (MODELTEXTURETILESTYLE_WRAP),
		  m_dMinValue (0.0),
		  m_dMaxValue (1.0),
		  m_nInternalIndex (0)
	{
	}


	CModelImage3DChannelSelector::~CModelImage3DChannelSelector()
	{

	}

	PModelImage3DChannelSelector CModelImage3DChannelSelector::make(PPackageResourceID sImage3DId, std::string sSourceChannel, std::string sDstChannel)
	{
		return std::make_shared<CModelImage3DChannelSelector>(CModelImage3DChannelSelector (sImage3DId, sSourceChannel, sDstChannel));
	}

	PModelImage3DChannelSelector CModelImage3DChannelSelector::make_from(CModelImage3DChannelSelector * pChannelSelector, const std::map<PPackageResourceID, PPackageResourceID> & PackageIDMap)
	{
		if (pChannelSelector == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		auto iIterator = PackageIDMap.find (pChannelSelector->getImage3DID ());
		if (iIterator == PackageIDMap.end ())
			throw CNMRException(NMR_ERROR_COULDNOTMAPPACKAGEID);

		PPackageResourceID pNewImage3DID = iIterator->second;

		PModelImage3DChannelSelector pNewSelector = std::make_shared<CModelImage3DChannelSelector>(CModelImage3DChannelSelector (pNewImage3DID, pChannelSelector->getSourceChannel (), pChannelSelector->getDstChannel()));
		pNewSelector->setFilter(pChannelSelector->getFilter());
		pNewSelector->setMinValue(pChannelSelector->getMinValue());
		pNewSelector->setTileStyleU(pChannelSelector->getTileStyleU());
		pNewSelector->setTileStyleV(pChannelSelector->getTileStyleV());
		pNewSelector->setTileStyleW(pChannelSelector->getTileStyleW());
		return pNewSelector;
	}


	PPackageResourceID CModelImage3DChannelSelector::getImage3DID()
	{
		return m_sImage3DID;
	}

	std::string CModelImage3DChannelSelector::getSourceChannel()
	{
		return m_sSourceChannel;
	}

	std::string CModelImage3DChannelSelector::getDstChannel()
	{
		return m_sDstChannel;
	}


	nfDouble CModelImage3DChannelSelector::getMinValue()
	{
		return m_dMinValue;
	}

	nfDouble CModelImage3DChannelSelector::getMaxValue()
	{
		return m_dMaxValue;
	}

	eModelTextureTileStyle CModelImage3DChannelSelector::getTileStyleU()
	{
		return m_eTileStyleU;
	}

	eModelTextureTileStyle CModelImage3DChannelSelector::getTileStyleV()
	{
		return m_eTileStyleV;
	}

	eModelTextureTileStyle CModelImage3DChannelSelector::getTileStyleW()
	{
		return m_eTileStyleW;
	}

	eModelTextureFilter CModelImage3DChannelSelector::getFilter()
	{
		return m_eFilter;
	}


	void CModelImage3DChannelSelector::setImage3DID(PPackageResourceID sValue)
	{
		m_sImage3DID = sValue;
	}

	void CModelImage3DChannelSelector::setSourceChannel(std::string sValue)
	{
		m_sSourceChannel = sValue;
	}

	void CModelImage3DChannelSelector::setDstChannel(std::string sValue)
	{
		m_sDstChannel = sValue;
	}

	void CModelImage3DChannelSelector::setMinValue(nfDouble dValue)
	{
		m_dMinValue = dValue;
	}

	void CModelImage3DChannelSelector::setMaxValue(nfDouble dValue)
	{
		m_dMaxValue = dValue;
	}

	void CModelImage3DChannelSelector::setTileStyleU(eModelTextureTileStyle eValue)
	{
		m_eTileStyleU = eValue;
	}

	void CModelImage3DChannelSelector::setTileStyleV(eModelTextureTileStyle eValue)
	{
		m_eTileStyleV = eValue;
	}

	void CModelImage3DChannelSelector::setTileStyleW(eModelTextureTileStyle eValue)
	{
		m_eTileStyleW = eValue;
	}

	void CModelImage3DChannelSelector::setFilter(eModelTextureFilter eValue)
	{
		m_eFilter = eValue;
	}

	void CModelImage3DChannelSelector::setInternalIndex(nfUint32 nIndex)
	{
		m_nInternalIndex = nIndex;
	}

	nfUint32 CModelImage3DChannelSelector::getInternalIndex()
	{
		return m_nInternalIndex;
	}

}

