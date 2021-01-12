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

NMR_ModelMultiPropertyGroup.cpp implements the Model Color Group Resource Class.
A color group is an in memory representation of the 3MF color groups node.

--*/

#include "Model/Classes/NMR_ModelMultiPropertyGroup.h"

#include "Model/Classes/NMR_ModelBaseMaterials.h"
#include "Model/Classes/NMR_ModelColorGroup.h"
#include "Model/Classes/NMR_ModelCompositeMaterials.h"
#include "Model/Classes/NMR_ModelTexture2DGroup.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include <sstream>

namespace NMR {

	;
	CModelMultiPropertyGroupResource::CModelMultiPropertyGroupResource(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelResource(sID, pModel)
	{
		m_nNextPropertyID = 1;
	}

	nfUint32 CModelMultiPropertyGroupResource::addMultiProperty(_In_ PModelMultiProperty pMultiProperty)
	{
		nfUint32 nID = m_nNextPropertyID;
		if (getCount() >= XML_3MF_MAXRESOURCEINDEX) {
			throw CNMRException(NMR_ERROR_TOOMANYCOMPOSITES);
		}
		
		//// TODO check for valid PropertyID
		//for (ModelPropertyID constituent : *pMultiProperty) {
		//	
		//}

		m_vctMultiProperties.insert(std::make_pair(nID, pMultiProperty));

		m_nNextPropertyID++;

		clearResourceIndexMap();

		return nID;
	}

	nfUint32 CModelMultiPropertyGroupResource::getCount()
	{
		return (nfUint32)m_vctMultiProperties.size();
	}

	PModelMultiProperty CModelMultiPropertyGroupResource::getMultiProperty(_In_ ModelPropertyID nPropertyID)
	{
		auto iIterator = m_vctMultiProperties.find(nPropertyID);
		if (iIterator != m_vctMultiProperties.end()) {
			return iIterator->second;
		}
		else {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
	}

	void CModelMultiPropertyGroupResource::setMultiProperty(_In_ ModelPropertyID nPropertyID, _In_ PModelMultiProperty pMultiProperty)
	{
		auto iIterator = m_vctMultiProperties.find(nPropertyID);
		if (iIterator != m_vctMultiProperties.end()) {
			// TODO: check whether the respective resource contains the PropertyID
			// throw CNMRException(NMR_ERROR_PROPERTYIDNOTFOUND);
			iIterator->second = pMultiProperty;
		}
		else {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
	}

	void CModelMultiPropertyGroupResource::removeMultiProperty(_In_ ModelPropertyID nPropertyID)
	{
		m_vctMultiProperties.erase(nPropertyID);
		clearResourceIndexMap();
	}

	nfUint32 CModelMultiPropertyGroupResource::getLayerCount()
	{
		return nfUint32(m_vctLayers.size());
	}

	nfUint32 CModelMultiPropertyGroupResource::addLayer(_In_ MODELMULTIPROPERTYLAYER sLayer)
	{
		bool hasMaterial = false;
		bool hasColorGroup = false;
		for (MODELMULTIPROPERTYLAYER layer : m_vctLayers)
		{
			PModelResource pResource = Model()->findResource(layer.m_nUniqueResourceID);
			hasMaterial |= (dynamic_cast<CModelBaseMaterialResource*>(pResource.get())!=nullptr)
				|| (dynamic_cast<CModelCompositeMaterialsResource*>(pResource.get()) != nullptr);
			hasColorGroup |= (dynamic_cast<CModelColorGroupResource*>(pResource.get()) != nullptr);
		}
		
		PModelResource pResource = Model()->findResource(sLayer.m_nUniqueResourceID);
		if ((dynamic_cast<CModelMultiPropertyGroupResource*>(pResource.get()) != nullptr))
		{
			throw CNMRException(NMR_ERROR_MULTIPROPERTIES_MUST_NOT_CONTAIN_MULTIPROPERTIES);
		}

		if (!( (dynamic_cast<CModelBaseMaterialResource*>(pResource.get()) != nullptr)
			|| (dynamic_cast<CModelColorGroupResource*>(pResource.get()) != nullptr)
			|| (dynamic_cast<CModelTexture2DGroupResource*>(pResource.get()) != nullptr)
			|| (dynamic_cast<CModelCompositeMaterialsResource*>(pResource.get()) != nullptr) ) )
		{
			throw CNMRException(NMR_ERROR_MULTIPROPERTIES_INVALID_RESOURCE);
		}
		
		if (hasMaterial && ( (dynamic_cast<CModelBaseMaterialResource*>(pResource.get()) != nullptr)
			|| (dynamic_cast<CModelCompositeMaterialsResource*>(pResource.get()) != nullptr) )
			)
		{
			throw CNMRException(NMR_ERROR_MULTIPROPERTIES_MUST_NOT_CONTAIN_MULTIPLE_MATERIALS);
		}

		if (hasColorGroup && (dynamic_cast<CModelColorGroupResource*>(pResource.get()) != nullptr))
		{
			throw CNMRException(NMR_ERROR_MULTIPROPERTIES_MUST_NOT_CONTAIN_MULTIPLE_COLORGOURPS);
		}
		

		m_vctLayers.push_back(sLayer);
		return nfUint32(m_vctLayers.size() - 1);
	}

	MODELMULTIPROPERTYLAYER CModelMultiPropertyGroupResource::getLayer(_In_ nfUint32 nIndex)
	{
		if (nIndex > m_vctLayers.size() - 1) {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
		return m_vctLayers[nIndex];
	}

	void CModelMultiPropertyGroupResource::setLayer(_In_ nfUint32 nIndex, _In_ MODELMULTIPROPERTYLAYER sLayer)
	{
		if (nIndex > m_vctLayers.size() - 1) {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
		m_vctLayers[nIndex] = sLayer;
	}

	void CModelMultiPropertyGroupResource::removeLayer(_In_ nfUint32 nIndex)
	{
		if (nIndex > m_vctLayers.size() - 1) {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
		m_vctLayers.erase(m_vctLayers.begin() + nIndex);
	}

	void CModelMultiPropertyGroupResource::mergeFrom(_In_ CModelMultiPropertyGroupResource * pSourceMultiMaterialGroup)
	{
		if (pSourceMultiMaterialGroup == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		
		nfUint32 nCount = pSourceMultiMaterialGroup->getCount();
		pSourceMultiMaterialGroup->buildResourceIndexMap();
		for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
			ModelPropertyID nPropertyID;
			pSourceMultiMaterialGroup->mapResourceIndexToPropertyID(nIndex, nPropertyID);
			PModelMultiProperty pCmposite = pSourceMultiMaterialGroup->getMultiProperty(nPropertyID);
			addMultiProperty(pCmposite);
		}

		nfUint32 nLayers = pSourceMultiMaterialGroup->getLayerCount();
		for (nfUint32 nIndex = 0; nIndex < nLayers; nIndex++) {
			MODELMULTIPROPERTYLAYER layer = pSourceMultiMaterialGroup->getLayer(nIndex);
			addLayer(layer);
		}

		clearResourceIndexMap();
	}

	void CModelMultiPropertyGroupResource::buildResourceIndexMap()
	{
		m_ResourceIndexMap.clear();
		m_ResourceIndexMap.reserve(m_vctMultiProperties.size());

		auto iIterator = m_vctMultiProperties.begin();
		while (iIterator != m_vctMultiProperties.end()) {
			m_ResourceIndexMap.push_back(iIterator->first);
			iIterator++;
		}

		m_bHasResourceIndexMap = true;
	}

	std::string CModelMultiPropertyGroupResource::blendMethodToString(_In_ eModelBlendMethod eBlendMethod)
	{
		switch (eBlendMethod) {
		case MODELBLENDMETHOD_MIX: return XML_3MF_ATTRIBUTE_MULTIPROPERTIES_BLENDMETHOD_MIX;
		case MODELBLENDMETHOD_MULTIPLY: return XML_3MF_ATTRIBUTE_MULTIPROPERTIES_BLENDMETHOD_MULTIPLY;
		default:
			return XML_3MF_ATTRIBUTE_MULTIPROPERTIES_BLENDMETHOD_MIX;
		}
	}
	eModelBlendMethod CModelMultiPropertyGroupResource::stringToBlendMethod(_In_ std::string sBlendMethod)
	{
		if (sBlendMethod == XML_3MF_ATTRIBUTE_MULTIPROPERTIES_BLENDMETHOD_MIX) {
			return MODELBLENDMETHOD_MIX;
		} else if (sBlendMethod == XML_3MF_ATTRIBUTE_MULTIPROPERTIES_BLENDMETHOD_MULTIPLY) {
			return MODELBLENDMETHOD_MULTIPLY;
		}
		else {
			throw CNMRException(NMR_ERROR_INVALID_BLENDMETHOD_ATTRIBUTE);
		}
	}

}

