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
		: CModelResource(sID, pModel)
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

	nfUint32 CModelVolumetricStack::addDstChannel(const std::string & sName, nfDouble dBackground)
	{
		if (m_DstChannels.size() >= MAX_VOLUMETRIC_CHANNELS)
			throw CNMRException(NMR_ERROR_TOOMANYVOLUMETRICCHANNELS);
		if (findDstChannel (sName) < m_DstChannels.size())
			throw CNMRException(NMR_ERROR_DUPLICATEVOLUMETRICCHANNEL);

		nfUint32 nNewIndex = (nfUint32) m_DstChannels.size();

		PModelVolumetricDstChannel pDstChannel = std::make_shared<CModelVolumetricDstChannel>(sName, dBackground);
		m_DstChannels.push_back(pDstChannel);

		m_DstChannelNameMap.insert(std::make_pair(sName, nNewIndex));
		return nNewIndex;
	}

	void CModelVolumetricStack::removeDstChannel(nfUint32 nIndex)
	{
		if (nIndex >= m_DstChannels.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		auto iIterator = m_DstChannels.begin();
		iIterator += nIndex;
		
		m_DstChannelNameMap.erase((*iIterator)->getName ());
		m_DstChannels.erase(iIterator);
	}

	nfUint32 CModelVolumetricStack::findDstChannel(const std::string & sName)
	{
		auto iIter = m_DstChannelNameMap.find(sName);
		if (iIter != m_DstChannelNameMap.end())
			return iIter->second;

		return 0xfffffff;
	}

	bool CModelVolumetricStack::hasDstChannel(const std::string & sName)
	{
		auto iIter = m_DstChannelNameMap.find(sName);
		if (iIter != m_DstChannelNameMap.end())
			return true;

		return false;

	}


	void CModelVolumetricStack::clearDstChannels()
	{
		m_DstChannelNameMap.clear();
		m_DstChannels.clear();
	}

	void CModelVolumetricStack::clearUnusedDstChannels()
	{
		nfUint32 nChannelCount = getDstChannelCount();
		if (nChannelCount > 0) {
			std::set <std::string> m_Channels;

			for (auto iLayer : m_Layers) {
				nfUint32 nSelectorCount = iLayer->getChannelSelectorCount();
				for (nfUint32 nIndex = 0; nIndex < nSelectorCount; nIndex++) {
					auto pSelector = iLayer->getChannelSelector(nIndex);
					m_Channels.insert(pSelector->getSourceChannel());
					m_Channels.insert(pSelector->getDstChannel());
				}
			}

			for (nfUint32 nIndex = nChannelCount - 1; nIndex >= 0; nIndex--) {
				auto pDstChannel = getDstChannel(nIndex);
				auto iIter = m_Channels.find(pDstChannel->getName());
				if (iIter == m_Channels.end())
					removeDstChannel(nIndex);
			}
		}

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

	PModelVolumetricLayer CModelVolumetricStack::addLayer(NMATRIX3 Transform, eModelBlendMethod BlendMethod)
	{
		if (m_Layers.size() >= MAX_VOLUMETRIC_CHANNELS)
			throw CNMRException(NMR_ERROR_TOOMANYVOLUMETRICCHANNELS);

		PModelVolumetricLayer pLayer = CModelVolumetricLayer::make (Transform, BlendMethod);
		pLayer->setInternalIndex(getLayerCount());
		m_Layers.push_back(pLayer);

		return pLayer;
	}

	PModelVolumetricLayer CModelVolumetricStack::addLayerFrom(CModelVolumetricLayer * pSourceLayer, const std::map<PPackageResourceID, PPackageResourceID> & PackageIDMap)
	{
		if (m_Layers.size() >= MAX_VOLUMETRIC_CHANNELS)
			throw CNMRException(NMR_ERROR_TOOMANYVOLUMETRICCHANNELS);

		PModelVolumetricLayer pLayer = CModelVolumetricLayer::make_from(pSourceLayer, PackageIDMap);
		pLayer->setInternalIndex (getLayerCount ());
		m_Layers.push_back(pLayer);

		return pLayer;
	}


	void CModelVolumetricStack::removeLayerByIndex(nfUint32 nIndex)
	{
		nfUint32 nCount = getLayerCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		m_Layers.erase(m_Layers.begin () + nIndex);		
	}

	void CModelVolumetricStack::removeLayer(CModelVolumetricLayer * pLayer)
	{
		if (pLayer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 nIndex = pLayer->getInternalIndex();
		if (m_Layers[nIndex].get() != pLayer)
			throw CNMRException(NMR_ERROR_COULDNOTREMOVEVOLUMETRICLAYER);

		removeLayerByIndex (nIndex);
	}

	void CModelVolumetricStack::reIndexLayer(CModelVolumetricLayer * pLayer, nfUint32 nNewIndex)
	{
		nfUint32 nCount = getLayerCount();
		if (nNewIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		if (pLayer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		__NMRASSERT (nCount > 0);

		nfUint32 nOldIndex = pLayer->getInternalIndex();
		PModelVolumetricLayer pOldLayer = m_Layers[nOldIndex];
		if (pOldLayer.get () != pLayer)
			throw CNMRException(NMR_ERROR_COULDNOTREINDEXVOLUMETRICLAYER);

		if (nOldIndex < nNewIndex) {
			for (nfUint32 nIndex = nOldIndex; nIndex < nNewIndex; nIndex++) {
				auto pReindexLayer = m_Layers[nIndex + 1];
				m_Layers[nIndex] = pReindexLayer;
				pReindexLayer->setInternalIndex(nIndex);
			}
			m_Layers[nNewIndex] = pOldLayer;
			pOldLayer->setInternalIndex(nNewIndex);


		} else if (nOldIndex > nNewIndex) {
			for (nfUint32 nIndex = nOldIndex; nIndex > nNewIndex; nIndex--) {
				auto pReindexLayer = m_Layers[nIndex - 1];
				m_Layers[nIndex] = pReindexLayer;
				pReindexLayer->setInternalIndex(nIndex);
			}
			m_Layers[nNewIndex] = pOldLayer;
			pOldLayer->setInternalIndex(nNewIndex);

		}
	}

	void CModelVolumetricStack::clear()
	{
		m_Layers.clear();
		clearDstChannels();
	}

}

