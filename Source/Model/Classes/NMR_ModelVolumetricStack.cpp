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

NMR_ModelVolumetricLayer.cpp implements a volumetric 3D layer for the volumetric extension

--*/

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelVolumetricStack.h"


#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {

	CModelVolumetricStack::CModelVolumetricStack(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelResource (sID, pModel)
	{

	}


	PModelVolumetricStack CModelVolumetricStack::make(_In_ const ModelResourceID sID, _In_ CModel * pModel)
	{
		return std::make_shared <CModelVolumetricStack> (CModelVolumetricStack (sID, pModel));
	}

	PModelVolumetricStack CModelVolumetricStack::make_from(_In_ CModelVolumetricStack * pVolumetricStack, _In_ const ModelResourceID sID, _In_ CModel * pNewModel, const std::map<PPackageResourceID, PPackageResourceID> & PackageIDMap)
	{
		if (pNewModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pVolumetricStack == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		PModelVolumetricStack pNewStack = make (pNewModel->generateResourceID (), pNewModel);
		for (auto iDstChannel : pVolumetricStack->m_DstChannels)
			pNewStack->addDstChannel(iDstChannel->getName(), iDstChannel->getBackground());
		
		for (auto iLayer : pVolumetricStack->m_Layers) {
			auto pLayer = pNewStack->addLayerFrom(iLayer.get (), PackageIDMap);

		}

		return pNewStack;

	}


	nfUint32 CModelVolumetricStack::getDstChannelCount()
	{
		return (nfUint32)m_DstChannels.size();
	}

	PModelVolumetricDstChannel CModelVolumetricStack::getDstChannel(nfUint32 nIndex)
	{
		if (nIndex >= m_DstChannels.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_DstChannels[nIndex];
	}

	PModelVolumetricDstChannel CModelVolumetricStack::addDstChannel(std::string & sName, nfDouble dBackground)
	{
		if (m_DstChannels.size() >= MAX_VOLUMETRIC_CHANNELS)
			throw CNMRException(NMR_ERROR_TOOMANYVOLUMETRICCHANNELS);

		PModelVolumetricDstChannel pDstChannel = std::make_shared<CModelVolumetricDstChannel>(sName, dBackground);
		m_DstChannels.push_back(pDstChannel);

		return pDstChannel;
	}

	void CModelVolumetricStack::removeDstChannel(nfUint32 nIndex)
	{
		if (nIndex >= m_DstChannels.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		auto iIterator = m_DstChannels.begin();
		iIterator += nIndex;
		
		m_DstChannels.erase(iIterator);
	}

	nfUint32 CModelVolumetricStack::getLayerCount()
	{
		return (nfUint32)m_Layers.size();
	}

	PModelVolumetricLayer CModelVolumetricStack::getLayer(nfUint32 nIndex)
	{
		if (nIndex >= m_Layers.size ())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_Layers[nIndex];
	}

	PModelVolumetricLayer CModelVolumetricStack::addLayer(MODELTRANSFORM Transform, eModelBlendMethod BlendMethod)
	{
		if (m_Layers.size() >= MAX_VOLUMETRIC_CHANNELS)
			throw CNMRException(NMR_ERROR_TOOMANYVOLUMETRICCHANNELS);

		PModelVolumetricLayer pLayer = CModelVolumetricLayer::make (Transform, BlendMethod);
		m_Layers.push_back(pLayer);

		return pLayer;
	}

	PModelVolumetricLayer CModelVolumetricStack::addLayerFrom(CModelVolumetricLayer * pSourceLayer, const std::map<PPackageResourceID, PPackageResourceID> & PackageIDMap)
	{
		if (m_Layers.size() >= MAX_VOLUMETRIC_CHANNELS)
			throw CNMRException(NMR_ERROR_TOOMANYVOLUMETRICCHANNELS);

		PModelVolumetricLayer pLayer = CModelVolumetricLayer::make_from(pSourceLayer, PackageIDMap);
		m_Layers.push_back(pLayer);

		return pLayer;
	}

	void CModelVolumetricStack::removeLayer(nfUint32 nIndex)
	{
		if (nIndex >= m_Layers.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		auto iIterator = m_Layers.begin();
		iIterator += nIndex;

		m_Layers.erase(iIterator);
	}


}

