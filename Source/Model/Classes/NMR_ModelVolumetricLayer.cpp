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

NMR_ModelVolumetricLayer.cpp implements the Model Volumetric Layer Class.

--*/

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelVolumetricLayer.h"


#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {


	CModelVolumetricLayer::CModelVolumetricLayer(MODELTRANSFORM Transform, eModelBlendMethod BlendMethod)
		: m_Transform (Transform), m_BlendMethod (BlendMethod), m_dSourceAlpha (0.0), m_dDstAlpha (0.0), m_nInternalIndex (0)
	{

	}

	CModelVolumetricLayer::~CModelVolumetricLayer()
	{
		
	}

	PModelVolumetricLayer CModelVolumetricLayer::make(MODELTRANSFORM Transform, eModelBlendMethod BlendMethod)
	{
		return std::make_shared<CModelVolumetricLayer>(CModelVolumetricLayer(Transform, BlendMethod));
	}

	PModelVolumetricLayer CModelVolumetricLayer::make_from(CModelVolumetricLayer * pVolumetricLayer, const std::map<PPackageResourceID, PPackageResourceID> & PackageIDMap)
	{
		if (pVolumetricLayer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		PModelVolumetricLayer pNewLayer = std::make_shared<CModelVolumetricLayer>(CModelVolumetricLayer (pVolumetricLayer->getTransform(), pVolumetricLayer->getBlendMethod()));
		pNewLayer->setSourceAlpha (pVolumetricLayer->getSourceAlpha());
		pNewLayer->setDstAlpha(pVolumetricLayer->getDstAlpha());		

		auto pMaskSelector = pVolumetricLayer->getMaskChannelSelector();
		if (pMaskSelector.get() != nullptr)
			pNewLayer->setMaskChannelSelector (CModelImage3DChannelSelector::make_from (pMaskSelector.get(), PackageIDMap));

		for (auto iChannelSelector : pVolumetricLayer->m_ChannelSelectors) {
			pNewLayer->addChannelSelector(CModelImage3DChannelSelector::make_from(iChannelSelector.get(), PackageIDMap));

		}
		
		return pNewLayer;
	}


	MODELTRANSFORM CModelVolumetricLayer::getTransform()
	{
		return m_Transform;
	}

	eModelBlendMethod CModelVolumetricLayer::getBlendMethod()
	{
		return m_BlendMethod;
	}

	nfDouble CModelVolumetricLayer::getSourceAlpha()
	{
		return m_dSourceAlpha;
	}

	nfDouble CModelVolumetricLayer::getDstAlpha()
	{
		return m_dDstAlpha;
	}

	void CModelVolumetricLayer::setTransform(MODELTRANSFORM Transform)
	{
		m_Transform = Transform;
	}

	void CModelVolumetricLayer::setBlendMethod(eModelBlendMethod BlendMethod)
	{
		m_BlendMethod = BlendMethod;
	}

	void CModelVolumetricLayer::setSourceAlpha(nfDouble srcAlpha)
	{
		m_dSourceAlpha = srcAlpha;
	}

	void CModelVolumetricLayer::setDstAlpha(nfDouble dstAlpha)
	{
		m_dDstAlpha = dstAlpha;
	}


	PModelImage3DChannelSelector CModelVolumetricLayer::getMaskChannelSelector()
	{
		return m_MaskChannelSelector;
	}

	void CModelVolumetricLayer::setMaskChannelSelector(PModelImage3DChannelSelector ChannelSelector)
	{
		m_MaskChannelSelector = ChannelSelector;
	}

	void CModelVolumetricLayer::addChannelSelector(PModelImage3DChannelSelector ChannelSelector)
	{
		if (m_ChannelSelectors.size() >= MAX_VOLUMETRIC_CHANNELS)
			throw CNMRException(NMR_ERROR_TOOMANYCHANNELSELECTORS);

		m_ChannelSelectors.push_back(ChannelSelector);
	}

	nfUint32 CModelVolumetricLayer::getChannelSelectorCount()
	{
		return (nfUint32) m_ChannelSelectors.size();
	}

	PModelImage3DChannelSelector CModelVolumetricLayer::getChannelSelector(nfUint32 nIndex)
	{
		if (nIndex >= m_ChannelSelectors.size())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		return m_ChannelSelectors[nIndex];
	}

	void CModelVolumetricLayer::removeChannelSelector(nfUint32 nIndex)
	{
		if (nIndex >= m_ChannelSelectors.size())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		auto iIter = m_ChannelSelectors.begin();
		iIter += nIndex;
		m_ChannelSelectors.erase (iIter);
	}

	void CModelVolumetricLayer::clearChannelSelectors()
	{
		m_ChannelSelectors.clear();
	}

	void CModelVolumetricLayer::setInternalIndex(nfUint32 nIndex)
	{
		m_nInternalIndex = nIndex;
	}

	nfUint32 CModelVolumetricLayer::getInternalIndex()
	{
		return m_nInternalIndex;
	}



}

