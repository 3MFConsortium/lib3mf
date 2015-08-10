/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_Model.cpp implements the Model Class.
A model is an in memory representation of the 3MF file.

--*/

#include "Model/Classes/NMR_Model.h" 
#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Model/Classes/NMR_ModelTypes.h" 
#include "Model/Classes/NMR_ModelBuildItem.h" 
#include "Model/Classes/NMR_ModelBaseMaterials.h" 
#include "Model/Classes/NMR_ModelTexture2D.h" 

#include "Common/Mesh/NMR_Mesh.h" 
#include "Common/MeshInformation/NMR_MeshInformation.h" 
#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h" 
#include "Common/NMR_Exception.h" 
#include <sstream>

namespace NMR {

	CModel::CModel()
	{
		m_Unit = MODELUNIT_MILLIMETER;
		m_sLanguage = XML_3MF_LANG_US;
		m_nHandleCounter = 1;
	}

	CModel::~CModel()
	{
		m_BuildItems.clear();
		m_ResourceMap.clear();
		m_Resources.clear();
		m_MetaData.clear();
		m_ObjectLookup.clear();
		m_TextureLookup.clear();
		m_BaseMaterialLookup.clear();
	}

	// Merge all build items into one mesh
	void CModel::mergeToMesh(_In_ CMesh * pMesh)
	{
		for (auto iIterator = m_BuildItems.begin(); iIterator != m_BuildItems.end(); iIterator++) {
			(*iIterator)->mergeToMesh(pMesh);
		}
	}

	// Units setter/getter
	void CModel::setUnit(_In_ eModelUnit Unit)
	{
		m_Unit = Unit;
	}

	void CModel::setUnitString(_In_ std::wstring sUnitString)
	{
		const wchar_t * pUnitName = sUnitString.c_str();

		if (wcscmp(pUnitName, XML_3MF_MODELUNIT_MICROMETER) == 0)
			setUnit(MODELUNIT_MICROMETER);
		else if (wcscmp(pUnitName, XML_3MF_MODELUNIT_MILLIMETER) == 0)
			setUnit(MODELUNIT_MILLIMETER);
		else if (wcscmp(pUnitName, XML_3MF_MODELUNIT_CENTIMETER) == 0)
			setUnit(MODELUNIT_CENTIMETER);
		else if (wcscmp(pUnitName, XML_3MF_MODELUNIT_INCH) == 0)
			setUnit(MODELUNIT_INCH);
		else if (wcscmp(pUnitName, XML_3MF_MODELUNIT_FOOT) == 0)
			setUnit(MODELUNIT_FOOT);
		else if (wcscmp(pUnitName, XML_3MF_MODELUNIT_METER) == 0)
			setUnit(MODELUNIT_METER);
		else
			throw CNMRException(NMR_ERROR_INVALIDMODELUNIT);
	}

	std::wstring CModel::getUnitString()
	{
		switch (m_Unit) {
		case MODELUNIT_MICROMETER:
			return XML_3MF_MODELUNIT_MICROMETER;
		case MODELUNIT_MILLIMETER:
			return XML_3MF_MODELUNIT_MILLIMETER;
		case MODELUNIT_CENTIMETER:
			return XML_3MF_MODELUNIT_CENTIMETER;
		case MODELUNIT_INCH:
			return XML_3MF_MODELUNIT_INCH;
		case MODELUNIT_FOOT:
			return XML_3MF_MODELUNIT_FOOT;
		case MODELUNIT_METER:
			return XML_3MF_MODELUNIT_METER;
		default:
			return L"";
		}
	}

	eModelUnit CModel::getUnit()
	{
		return m_Unit;
	}

	// Language setter/getter
	void CModel::setLanguage(_In_ std::wstring sLanguage)
	{
		m_sLanguage = sLanguage;
	}

	std::wstring CModel::getLanguage()
	{
		return m_sLanguage;
	}

	// General Resource Handling
	PModelResource CModel::findResource(_In_ ModelResourceID nID)
	{
		auto iIterator = m_ResourceMap.find(nID);
		if (iIterator != m_ResourceMap.end()) {
			return iIterator->second;
		}

		return nullptr;
	}

	nfUint32 CModel::getResourceCount()
	{
		return (nfUint32)m_Resources.size();
	}

	PModelResource CModel::getResource(_In_ nfUint32 nIndex)
	{
		if (nIndex >= (nfUint32)m_Resources.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_Resources[nIndex];
	}

	void CModel::addResource(_In_ PModelResource pResource)
	{
		if (!pResource.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		// Check for resource count overflow
		if (m_MetaData.size() > XML_3MF_MAXRESOURCECOUNT)
			throw CNMRException(NMR_ERROR_INVALIDRESOURCECOUNT);

		// Check if ID already exists
		ModelResourceID ID = pResource->getResourceID();
		auto iIterator = m_ResourceMap.find(ID);
		if (iIterator != m_ResourceMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATEMODELRESOURCE);

		// Add ID to objects
		m_ResourceMap.insert(std::make_pair(ID, pResource));
		m_Resources.push_back(pResource);

		// Create correct lookup table
		addResourceToLookupTable(pResource);
	}

	// Build Handling
	void CModel::addBuildItem(_In_ PModelBuildItem pBuildItem)
	{
		if (!pBuildItem.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_BuildItems.size() >= XML_3MF_MAXBUILDITEMCOUNT)
			throw CNMRException(NMR_ERROR_INVALIDBUILDITEMCOUNT);

		m_BuildItems.push_back(pBuildItem);
	}

	nfUint32 CModel::getBuildItemCount()
	{
		return (nfUint32)m_BuildItems.size();
	}

	PModelBuildItem CModel::getBuildItem(_In_ nfUint32 nIdx)
	{
		if (nIdx > (nfUint32)m_BuildItems.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_BuildItems[nIdx];
	}

	void CModel::removeBuildItem(_In_ nfUint32 nHandle, _In_ nfBool bThrowExceptionIfNotFound)
	{
		auto iIterator = m_BuildItems.begin();
		while (iIterator != m_BuildItems.end()) {
			if ((*iIterator)->getHandle() == nHandle) {
				m_BuildItems.erase(iIterator);
				return;
			}
			iIterator++;
		}

		if (bThrowExceptionIfNotFound)
			throw CNMRException(NMR_ERROR_BUILDITEMNOTFOUND);
	}


	// Metadata setter/getter
	void CModel::addMetaData(_In_ std::wstring sName, _In_ std::wstring sValue)
	{
		if (m_MetaData.size() >= XML_3MF_MAXMETADATACOUNT)
			throw CNMRException(NMR_ERROR_INVALIDMETADATACOUNT);

		PModelMetaData pMetaData = std::make_shared<CModelMetaData>(sName, sValue);
		m_MetaData.push_back(pMetaData);
		m_MetaDataMap.insert(std::make_pair (sName, pMetaData));
	}

	nfUint32 CModel::getMetaDataCount()
	{
		return (nfUint32)m_MetaData.size();
	}

	void CModel::getMetaData(_In_ nfUint32 nIndex, _Out_ std::wstring & sName, _Out_ std::wstring & sValue)
	{
		nfUint32 nCount = getMetaDataCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		PModelMetaData pMetaData = m_MetaData[nIndex];
		__NMRASSERT(pMetaData.get () != nullptr);

		sName = pMetaData->getName();
		sValue = pMetaData->getValue();
	}

	void CModel::removeMetaData(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getMetaDataCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto iIterator = m_MetaData.begin();
		if (nIndex > 0)
			iIterator += nIndex;
		m_MetaData.erase(iIterator);
	}

	nfBool CModel::hasMetaData(_In_ std::wstring sName)
	{		
		std::map<std::wstring, PModelMetaData>::iterator iIterator = m_MetaDataMap.find (sName);
		return iIterator != m_MetaDataMap.end();
	}

	void CModel::mergeMetaData(_In_ CModel * pSourceModel)
	{
		if (pSourceModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 nCount = pSourceModel->getMetaDataCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			std::wstring sName;
			std::wstring sValue;
			pSourceModel->getMetaData(nIndex, sName, sValue);
			addMetaData(sName, sValue);

		}
	}

	// Retrieve a unique Resource ID
	ModelResourceID CModel::generateResourceID()
	{
		auto iIterator = m_ResourceMap.rbegin();
		if (iIterator != m_ResourceMap.rend())
			return iIterator->first + 1;
		else
			return 1;
	}

	// Convenience functions for objects
	_Ret_maybenull_ CModelObject * CModel::findObject(_In_ ModelResourceID nResourceID)
	{
		PModelResource pResource = findResource(nResourceID);
		if (pResource != nullptr) {
			CModelObject * pModelObject = dynamic_cast<CModelObject *> (pResource.get());
			if (pModelObject == nullptr)
				throw CNMRException (NMR_ERROR_RESOURCETYPEMISMATCH);

			return pModelObject;
		}

		return nullptr;
	}

	nfUint32 CModel::getObjectCount()
	{
		return (nfUint32)m_ObjectLookup.size();
	}

	PModelResource CModel::getObjectResource(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getObjectCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_ObjectLookup[nIndex];
	}

	CModelObject * CModel::getObject(_In_ nfUint32 nIndex)
	{
		CModelObject * pModelObject = dynamic_cast<CModelObject *> (getObjectResource(nIndex).get());
		if (pModelObject == nullptr)
			throw CNMRException(NMR_ERROR_RESOURCETYPEMISMATCH);

		return pModelObject;
	}

	// Add Resource to resource lookup tables
	void CModel::addResourceToLookupTable(_In_ PModelResource pResource)
	{
		if (pResource.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		// Add to lookup tables
		CModelObject * pModelObject = dynamic_cast<CModelObject *> (pResource.get());
		if (pModelObject != nullptr)
			m_ObjectLookup.push_back(pResource);

		CModelBaseMaterialResource * pBaseMaterial = dynamic_cast<CModelBaseMaterialResource *> (pResource.get());
		if (pBaseMaterial != nullptr)
			m_BaseMaterialLookup.push_back(pResource);

		CModelTexture2DResource * pTexture2D = dynamic_cast<CModelTexture2DResource *> (pResource.get());
		if (pTexture2D != nullptr)
			m_TextureLookup.push_back(pResource);
	}

	// Clear all build items and Resources
	void CModel::clearAll()
	{
		m_pGlobalThumbnail = nullptr;

		m_ObjectLookup.clear();
		m_BaseMaterialLookup.clear();
		m_BuildItems.clear();
		m_ResourceMap.clear();
		m_Resources.clear();
		m_MetaDataMap.clear();
		m_MetaData.clear();
		m_TextureLookup.clear();
		m_Thumbnails.clear();
	}

	_Ret_maybenull_ CModelBaseMaterialResource * CModel::findBaseMaterial(_In_ ModelResourceID nResourceID)
	{
		PModelResource pResource = findResource(nResourceID);
		if (pResource != nullptr) {
			CModelBaseMaterialResource * pBaseMaterial = dynamic_cast<CModelBaseMaterialResource *> (pResource.get());
			if (pBaseMaterial == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCETYPEMISMATCH);

			return pBaseMaterial;
		}

		return nullptr;

	}

	nfUint32 CModel::getBaseMaterialCount()
	{
		return (nfUint32) m_BaseMaterialLookup.size();

	}

	PModelResource CModel::getBaseMaterialResource(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getBaseMaterialCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_BaseMaterialLookup[nIndex];

	}

	CModelBaseMaterialResource * CModel::getBaseMaterial(_In_ nfUint32 nIndex)
	{
		CModelBaseMaterialResource * pBaseMaterial = dynamic_cast<CModelBaseMaterialResource *> (getBaseMaterialResource(nIndex).get());
		if (pBaseMaterial == nullptr)
			throw CNMRException(NMR_ERROR_RESOURCETYPEMISMATCH);

		return pBaseMaterial;
	}

	void CModel::mergeBaseMaterials(_In_ CModel * pSourceModel)
	{
		if (pSourceModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 nCount = pSourceModel->getBaseMaterialCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			CModelBaseMaterialResource * pOldMaterial = pSourceModel->getBaseMaterial(nIndex);
			__NMRASSERT(pOldMaterial != nullptr);

			PModelBaseMaterialResource pNewMaterial = std::make_shared<CModelBaseMaterialResource> (generateResourceID(), this);
			pNewMaterial->mergeFrom(pOldMaterial);
			
			addResource(pNewMaterial);
		}

	} 


	_Ret_maybenull_ CModelTexture2DResource * CModel::findTexture2D(_In_ ModelResourceID nResourceID)
	{
		PModelResource pResource = findResource(nResourceID);
		if (pResource != nullptr) {
			CModelTexture2DResource * pTexture2D = dynamic_cast<CModelTexture2DResource *> (pResource.get());
			if (pTexture2D == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCETYPEMISMATCH);

			return pTexture2D;
		}

		return nullptr;

	}

	nfUint32 CModel::getTexture2DCount()
	{
		return (nfUint32)m_TextureLookup.size();

	}

	PModelResource CModel::getTexture2DResource(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getTexture2DCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_TextureLookup[nIndex];

	}

	CModelTexture2DResource * CModel::getTexture2D(_In_ nfUint32 nIndex)
	{
		CModelTexture2DResource * pTexture2D = dynamic_cast<CModelTexture2DResource *> (getTexture2DResource(nIndex).get());
		if (pTexture2D == nullptr)
			throw CNMRException(NMR_ERROR_RESOURCETYPEMISMATCH);

		return pTexture2D;
	}

	void CModel::mergeTextures2D(_In_ CModel * pSourceModel)
	{
		if (pSourceModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 nCount = pSourceModel->getTexture2DCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			CModelTexture2DResource * pTextureResource = pSourceModel->getTexture2D(nIndex);
			if (pTextureResource == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			PModelTexture2DResource pNewTextureResource = std::make_shared<CModelTexture2DResource>(generateResourceID(), this);
			pNewTextureResource->copyFrom(pTextureResource);

			addResource(pNewTextureResource);
		}
	}

	nfUint32 CModel::createHandle()
	{
		if (m_nHandleCounter >= NMR_MAXHANDLE)
			throw CNMRException(NMR_ERROR_HANDLEOVERFLOW);

		nfUint32 nHandle = m_nHandleCounter;
		m_nHandleCounter++;

		return nHandle;
	}

	void CModel::addTextureStream(_In_ std::wstring sPath, _In_ PImportStream pStream)
	{
		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::wstring sLowerPath = sPath;
		//std::transform(sLowerPath.begin(), sLowerPath.end(), sLowerPath.begin(), towupper);

		auto iIterator = m_TextureStreamMap.find(sLowerPath);
		if (iIterator != m_TextureStreamMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATETEXTUREPATH);

		m_TextureStreams.push_back(std::make_pair(sPath, pStream));
		m_TextureStreamMap.insert(std::make_pair(sLowerPath, pStream));
	}

	void CModel::removeTextureStream(_In_ std::wstring sPath)
	{
		std::wstring sLowerPath = sPath;
		//std::transform(sLowerPath.begin(), sLowerPath.end(), sLowerPath.begin(), towupper);
		
		auto iIterator = m_TextureStreamMap.find(sLowerPath);
		if (iIterator != m_TextureStreamMap.end()) {

			// Remove Stream from list...
			auto iListIterator = m_TextureStreams.begin();
			while (iListIterator != m_TextureStreams.end()) {
				if (iListIterator->second.get() == iIterator->second.get()) {
					m_TextureStreams.erase(iListIterator);
					break;
				}
				iListIterator++;
			}

			m_TextureStreamMap.erase(iIterator);
		}
	}


	nfUint32 CModel::getTextureStreamCount()
	{
		return  (nfUint32)m_TextureStreams.size();
	}

	PImportStream CModel::getTextureStream(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getTextureStreamCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_TextureStreams[nIndex].second;
	}

	std::wstring CModel::getTextureStreamPath(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getTextureStreamCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_TextureStreams[nIndex].first;
	}

	PImportStream CModel::findTextureStream(_In_ std::wstring sPath)
	{
		std::wstring sLowerPath = sPath;
		//std::transform(sLowerPath.begin(), sLowerPath.end(), sLowerPath.begin(), towupper);

		auto iIterator = m_TextureStreamMap.find(sLowerPath);
		if (iIterator != m_TextureStreamMap.end())
			return iIterator->second;
		return nullptr;
	}

	void CModel::mergeTextureStreams(_In_ CModel * pSourceModel)
	{
		if (pSourceModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 nCount = pSourceModel->getTextureStreamCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			std::wstring sPath = pSourceModel->getTextureStreamPath(nIndex);
			PImportStream pTextureStream = pSourceModel->getTextureStream(nIndex);
			PImportStream pCopiedStream = pTextureStream->copyToMemory();
			addTextureStream(sPath, pCopiedStream);
		}

	}

}
