/*++

Copyright (C) 2018 3MF Consortium

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
#include "Model/Classes/NMR_ModelAttachment.h" 
#include "Model/Classes/NMR_ModelTextureAttachment.h" 
#include "Model/Classes/NMR_ModelBuildItem.h" 
#include "Model/Classes/NMR_ModelBaseMaterials.h" 
#include "Model/Classes/NMR_ModelTexture2D.h" 
#include "Model/Classes/NMR_ModelSliceStack.h"
#include "Model/Classes/NMR_ModelMetaDataGroup.h" 

#include "Common/Mesh/NMR_Mesh.h" 
#include "Common/MeshInformation/NMR_MeshInformation.h" 
#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h" 
#include "Common/NMR_Exception.h" 
#include <sstream>
#include <memory>

#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRefModel.h"
#include "Common/Platform/NMR_XmlReader.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ImportStream_Memory.h"

#include "Common/NMR_StringUtils.h" 
namespace NMR {

	CModel::CModel()
	{
		m_Unit = MODELUNIT_MILLIMETER;
		m_sLanguage = XML_3MF_LANG_US;
		m_nHandleCounter = 1;
		m_sCurPath = "";

		setBuildUUID(std::make_shared<CUUID>());
		m_MetaDataGroup = std::make_shared<CModelMetaDataGroup>();
	}

	CModel::~CModel()
	{
		m_MetaDataGroup->clear();

		m_BuildItems.clear();
		m_ResourceMap.clear();
		m_resourceHandler.clear();
		m_Resources.clear();
		m_ObjectLookup.clear();
		m_TextureLookup.clear();
		m_BaseMaterialLookup.clear();
	}

	const std::string CModel::curPath()
	{
		return m_sCurPath;
	}

	void CModel::setCurPath(const std::string sPath)
	{
		m_sCurPath = sPath;
	}

	const std::string CModel::rootPath()
	{
		return m_sRootPath;
	}

	void CModel::setRootPath(const std::string sPath)
	{
		m_sRootPath = sPath;
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

	void CModel::setUnitString(_In_ std::string sUnitString)
	{
		const nfChar * pUnitName = sUnitString.c_str();

		if (strcmp(pUnitName, XML_3MF_MODELUNIT_MICROMETER) == 0)
			setUnit(MODELUNIT_MICROMETER);
		else if (strcmp(pUnitName, XML_3MF_MODELUNIT_MILLIMETER) == 0)
			setUnit(MODELUNIT_MILLIMETER);
		else if (strcmp(pUnitName, XML_3MF_MODELUNIT_CENTIMETER) == 0)
			setUnit(MODELUNIT_CENTIMETER);
		else if (strcmp(pUnitName, XML_3MF_MODELUNIT_INCH) == 0)
			setUnit(MODELUNIT_INCH);
		else if (strcmp(pUnitName, XML_3MF_MODELUNIT_FOOT) == 0)
			setUnit(MODELUNIT_FOOT);
		else if (strcmp(pUnitName, XML_3MF_MODELUNIT_METER) == 0)
			setUnit(MODELUNIT_METER);
		else
			throw CNMRException(NMR_ERROR_INVALIDMODELUNIT);
	}

	std::string CModel::getUnitString()
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
			return "";
		}
	}

	eModelUnit CModel::getUnit()
	{
		return m_Unit;
	}

	// Language setter/getter
	void CModel::setLanguage(_In_ std::string sLanguage)
	{
		m_sLanguage = sLanguage;
	}

	std::string CModel::getLanguage()
	{
		return m_sLanguage;
	}

	// General Resource Handling
	PModelResource CModel::findResource(_In_ std::string path, ModelResourceID nID)
	{
		PPackageResourceID pID = m_resourceHandler.findRessourceID(path, nID);
		if (pID.get())
			return findResource(pID);
		else
			return nullptr;
	}

	PModelResource CModel::findResource(_In_ PackageResourceID nID)
	{
		PPackageResourceID pID = m_resourceHandler.findRessourceID(nID);
		if (pID.get())
			return findResource(pID);
		else
			return nullptr;
	}

	PModelResource CModel::findResource(_In_ PPackageResourceID pID)
	{
		PackageResourceID uID = pID->getUniqueID();

		auto iIterator = m_ResourceMap.find(uID);
		if (iIterator != m_ResourceMap.end()) {
			return iIterator->second;
		}
		return nullptr;
	}

	PPackageResourceID CModel::findPackageResourceID(_In_ std::string path, ModelResourceID nID)
	{
		return m_resourceHandler.findRessourceID(path, nID);
	}
	PPackageResourceID CModel::findPackageResourceID(_In_ PackageResourceID nID)
	{
		return m_resourceHandler.findRessourceID(nID);
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
		if (getMetaDataCount() > XML_3MF_MAXRESOURCECOUNT)
			throw CNMRException(NMR_ERROR_INVALIDRESOURCECOUNT);

		// Check if ID already exists
		PackageResourceID nID = pResource->getResourceID()->getUniqueID();
		auto iIterator = m_ResourceMap.find(nID);
		if (iIterator != m_ResourceMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATEMODELRESOURCE);

		// Add ID to objects
		m_ResourceMap.insert(std::make_pair(nID, pResource));
		m_Resources.push_back(pResource);

		// Create correct lookup table
		addResourceToLookupTable(pResource);
	}

	// Metadata setter/getter
	PModelMetaData CModel::addMetaData(_In_ std::string sNameSpace, _In_ std::string sName, _In_ std::string sValue, _In_ std::string sType, _In_ nfBool bPreserve)
	{
		return m_MetaDataGroup->addMetaData(sNameSpace, sName, sValue, sType, bPreserve);
	}

	nfUint32  CModel::getMetaDataCount()
	{
		return m_MetaDataGroup->getMetaDataCount();
	}

	PModelMetaData CModel::getMetaData(_In_ nfUint32 nIndex)
	{
		return m_MetaDataGroup->getMetaData(nIndex);
	}

	void  CModel::removeMetaData(_In_ nfUint32 nIndex)
	{
		m_MetaDataGroup->removeMetaData(nIndex);
	}

	nfBool  CModel::hasMetaData(_In_ std::string sKey)
	{
		return m_MetaDataGroup->hasMetaData(sKey);
	}

	void  CModel::mergeMetaData(_In_ CModel * pSourceModel)
	{
		if (!pSourceModel)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_MetaDataGroup->mergeMetaData(pSourceModel->m_MetaDataGroup.get());
	}

	PModelMetaDataGroup CModel::getMetaDataGroup()
	{
		return m_MetaDataGroup;
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

	PUUID CModel::buildUUID()
	{
		return m_buildUUID;
	}

	void CModel::setBuildUUID(PUUID pUUID)
	{
		registerUUID(pUUID);
		unRegisterUUID(m_buildUUID);
		m_buildUUID = pUUID;
	}

	void CModel::unRegisterUUID(PUUID pUUID)
	{
		usedUUIDs.erase(std::remove(usedUUIDs.begin(), usedUUIDs.end(), pUUID), usedUUIDs.end());
	}

	void CModel::registerUUID(PUUID pUUID)
	{
		if (pUUID.get()) {
			for (auto iUUID : usedUUIDs) {
				if (*iUUID.get() == *pUUID.get()) {
					throw CNMRException(NMR_ERROR_UUID_NOT_UNIQUE);
				}
			}
			usedUUIDs.push_back(pUUID);
		}
	}


	// Retrieve a unique Resource ID

	ModelResourceID CModel::generateResourceID()
	{
		// TODO
		auto iIterator = m_ResourceMap.rbegin();
		if (iIterator != m_ResourceMap.rend())
			return iIterator->first + 1;
		else
			return 1;
	}

	PPackageResourceID CModel::generatePackageResourceID(_In_ std::string path, ModelResourceID nID)	// per package
	{
		return m_resourceHandler.getNewRessourceID(path, nID);
	}

	// Convenience functions for objects
	_Ret_maybenull_ CModelObject * CModel::findObject(_In_ PackageResourceID nResourceID)
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

	// Returns 0: if equal, 1 if A before B, -1 if A after B
	nfInt32 CModel::compareObjectsByResourceID(CModelResource* pObjectResourceA, CModelResource* pObjectResourceB)
	{
		nfUint32 nCount = getObjectCount();
		if ( (pObjectResourceA == nullptr) || (pObjectResourceB == nullptr) )
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);

		nfInt32 indexA = -1;
		nfInt32 indexB = -1;

		for (nfUint32 i = 0; i < nCount; i++) {
			if (m_ObjectLookup[i].get() == pObjectResourceA) {
				indexA = i;
			}
			if (m_ObjectLookup[i].get() == pObjectResourceB) {
				indexB = i;
			}
		}

		if ( (indexA==-1) || (indexB == -1))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (indexA < indexB) return 1;
		if (indexA > indexB) return -1;
		return 0;
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

		CModelSliceStack *pSliceStack = dynamic_cast<CModelSliceStack *>(pResource.get());
		if (pSliceStack != nullptr) 
			m_SliceStackLookup.push_back(pResource);
	}

	// Clear all build items and Resources
	void CModel::clearAll()
	{
		m_pPackageThumbnailAttachment = nullptr;

		m_MetaDataGroup->clear();
		m_ObjectLookup.clear();
		m_BaseMaterialLookup.clear();
		m_BuildItems.clear();
		m_ResourceMap.clear();
		m_Resources.clear();
		m_TextureLookup.clear();
		m_SliceStackLookup.clear();
	}

	_Ret_maybenull_ PModelBaseMaterialResource CModel::findBaseMaterial(_In_ PackageResourceID nResourceID)
	{
		PModelResource pResource = findResource(nResourceID);
		if (pResource != nullptr) {
			PModelBaseMaterialResource pBaseMaterialResource = std::dynamic_pointer_cast<CModelBaseMaterialResource>(pResource);
			if (pBaseMaterialResource.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCETYPEMISMATCH);
			return pBaseMaterialResource;
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


	_Ret_maybenull_ CModelTexture2DResource * CModel::findTexture2D(_In_ PackageResourceID nResourceID)
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
	
	PModelAttachment CModel::addPackageThumbnail(_In_ std::string sPath, _In_ PImportStream pStream)
	{
		if (m_pPackageThumbnailAttachment.get() == nullptr)
		{
			m_pPackageThumbnailAttachment = std::make_shared<CModelAttachment>(this,
				sPath,
				PACKAGE_THUMBNAIL_RELATIONSHIP_TYPE, pStream);
		}
		return m_pPackageThumbnailAttachment;
	}

	PModelAttachment CModel::addPackageThumbnail()
	{
		return addPackageThumbnail(PACKAGE_THUMBNAIL_URI_BASE + std::string("/") + "thumbnail.png", std::make_shared<CImportStream_Memory>());
	}

	void CModel::removePackageThumbnail()
	{
		m_pPackageThumbnailAttachment.reset();
	}

	PModelAttachment CModel::getPackageThumbnail()
	{
		return m_pPackageThumbnailAttachment;
	}

	PModelAttachment CModel::addAttachment(_In_ const std::string sPath, _In_ const std::string sRelationShipType, PImportStream pCopiedStream)
	{
		if (pCopiedStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::string sLowerPath = sPath;
		//std::transform(sLowerPath.begin(), sLowerPath.end(), sLowerPath.begin(), towupper);

		auto iIterator = m_AttachmentURIMap.find(sLowerPath);
		if (iIterator != m_AttachmentURIMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATEATTACHMENTPATH);

		PModelAttachment pAttachment = std::make_shared<CModelAttachment>(this, sPath, sRelationShipType, pCopiedStream);
		m_Attachments.push_back(pAttachment);
		m_AttachmentURIMap.insert(std::make_pair(sLowerPath, pAttachment));

		return pAttachment;
	}

	PModelAttachment CModel::getModelAttachment(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getAttachmentCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_Attachments[nIndex];
	}

	nfUint32 CModel::getAttachmentCount()
	{
		return (nfUint32)m_Attachments.size();
	}

	std::string CModel::getModelAttachmentPath(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getAttachmentCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		PModelAttachment pAttachment = m_Attachments[nIndex];

		return pAttachment->getPathURI();
	}

	PModelAttachment CModel::findModelAttachment(_In_ std::string sPath)
	{
		auto iIterator = m_AttachmentURIMap.find(sPath);
		if (iIterator != m_AttachmentURIMap.end()) {
			return iIterator->second;
		}

		return nullptr;
	}

	void CModel::removeAttachment(_In_ const std::string sPath)
	{
		auto iIterator = m_AttachmentURIMap.find(sPath);
		if (iIterator != m_AttachmentURIMap.end()) {
			auto iVectorIterator = m_Attachments.begin();
			while (iVectorIterator != m_Attachments.end()) {
				if (iVectorIterator->get() == iIterator->second.get()) {
					m_Attachments.erase(iVectorIterator);
					break;
				}
				iVectorIterator++;
			}

			m_AttachmentURIMap.erase(iIterator);
		}
	}


	void CModel::mergeModelAttachments(_In_ CModel * pSourceModel)
	{
		if (pSourceModel == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 nCount = pSourceModel->getAttachmentCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			PModelAttachment pModelAttachment = pSourceModel->getModelAttachment(nIndex);
			if (pModelAttachment == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			// TODO: probably need to copy stream
			addAttachment(pModelAttachment->getPathURI(), pModelAttachment->getRelationShipType(), pModelAttachment->getStream());
		}
	}

	template <typename T>
	void moveItemToBack(std::vector<T>& v, size_t itemIndex)
	{
		auto it = v.begin() + itemIndex;
		std::rotate(it, it + 1, v.end());
	}

	PModelAttachment CModel::addProductionAttachment(_In_ const std::string sPath, _In_ const std::string sRelationShipType, PImportStream pCopiedStream, nfBool bForceUnique)
	{
		if (pCopiedStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::string sLowerPath = sPath;
		//std::transform(sLowerPath.begin(), sLowerPath.end(), sLowerPath.begin(), towupper);
		PModelAttachment pAttachment;
		auto iIterator = m_ProductionAttachmentURIMap.find(sLowerPath);
		if (iIterator != m_ProductionAttachmentURIMap.end()) {
			if (bForceUnique)
				throw CNMRException(NMR_ERROR_DUPLICATEATTACHMENTPATH);
			else {
				pAttachment = iIterator->second;
				auto it = std::find(m_ProductionAttachments.begin(), m_ProductionAttachments.end(), iIterator->second);
				moveItemToBack(m_ProductionAttachments, it - m_ProductionAttachments.begin());
			}
		}
		else 
		{
			pAttachment = std::make_shared<CModelAttachment>(this, sPath, sRelationShipType, pCopiedStream);
			m_ProductionAttachments.push_back(pAttachment);
			m_ProductionAttachmentURIMap.insert(std::make_pair(sLowerPath, pAttachment));
		}

		return pAttachment;
	}

	PModelAttachment CModel::getProductionModelAttachment(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getProductionAttachmentCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_ProductionAttachments[nIndex];

	}

	nfUint32 CModel::getProductionAttachmentCount()
	{
		return (nfUint32)m_ProductionAttachments.size();
	}

	std::string CModel::getProductionModelAttachmentPath(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getProductionAttachmentCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		PModelAttachment pProductionAttachment = m_ProductionAttachments[nIndex];

		return pProductionAttachment->getPathURI();

	}

	PModelAttachment CModel::findProductionModelAttachment(_In_ std::string sPath)
	{
		auto iIterator = m_ProductionAttachmentURIMap.find(sPath);
		if (iIterator != m_ProductionAttachmentURIMap.end()) {
			return iIterator->second;
		}

		return nullptr;
	}

	void CModel::removeProductionAttachment(_In_ const std::string sPath)
	{
		auto iIterator = m_ProductionAttachmentURIMap.find(sPath);
		if (iIterator != m_ProductionAttachmentURIMap.end()) {
			auto iVectorIterator = m_ProductionAttachments.begin();
			while (iVectorIterator != m_ProductionAttachments.end()) {
				if (iVectorIterator->get() == iIterator->second.get()) {
					m_ProductionAttachments.erase(iVectorIterator);
					break;
				}
				iVectorIterator++;
			}

			m_ProductionAttachmentURIMap.erase(iIterator);
		}
	}


	std::map<std::string, std::string> CModel::getCustomContentTypes()
	{
		return m_CustomContentTypes;
	}

	void CModel::addCustomContentType(_In_ const std::string sExtension, _In_ const std::string sContentType)
	{
		m_CustomContentTypes.insert(std::make_pair(sExtension, sContentType));
	}

	void CModel::removeCustomContentType(_In_ const std::string sExtension)
	{
		m_CustomContentTypes.erase(sExtension);
	}

	nfBool CModel::contentTypeIsDefault(_In_ const std::string sExtension)
	{
		if (strcmp(sExtension.c_str(), PACKAGE_3D_RELS_EXTENSION) == 0)
			return true;
		if (strcmp(sExtension.c_str(), PACKAGE_3D_MODEL_EXTENSION) == 0)
			return true;
		if (strcmp(sExtension.c_str(), PACKAGE_3D_TEXTURE_EXTENSION) == 0)
			return true;
		if (strcmp(sExtension.c_str(), PACKAGE_3D_PNG_EXTENSION) == 0)
			return true;
		if (strcmp(sExtension.c_str(), PACKAGE_3D_JPEG_EXTENSION) == 0)
			return true;
		if (strcmp(sExtension.c_str(), PACKAGE_3D_JPG_EXTENSION) == 0)
			return true;

		return false;
	}

	// returns whether a specific extension has to be marked as required
	nfBool  CModel::RequireExtension(_In_ const std::string sExtension) {
		// loop over all ressources to check for slices, beamlattices, production-references,
		// that make it necessary to mark these extensions as required
		if (sExtension == XML_3MF_NAMESPACE_BEAMLATTICESPEC) {
			nfBool bRequireBeamLattice = false;
			for (nfUint32 i = 0; i < m_ObjectLookup.size(); i++) {
				CModelMeshObject* pMeshObject = dynamic_cast<CModelMeshObject*>(m_ObjectLookup[i].get());
				if (pMeshObject == nullptr || pMeshObject->getMesh() == nullptr)
					continue;
				if (pMeshObject->getMesh()->getBeamCount() > 0) {
					bRequireBeamLattice = true;
					break;
				}
			}
			return bRequireBeamLattice;
		}

		if (sExtension == XML_3MF_NAMESPACE_SLICESPEC) {
			nfBool bRequireSliceExtension = false;
			for (nfUint32 i = 0; i < m_ObjectLookup.size(); i++) {
				CModelMeshObject* pMeshObject = dynamic_cast<CModelMeshObject*>(m_ObjectLookup[i].get());
				if (pMeshObject == nullptr || pMeshObject->getMesh() == nullptr)
					continue;
				if (!pMeshObject->getSliceStack().get())
					continue;
				if (pMeshObject->slicesMeshResolution() == MODELSLICESMESHRESOLUTION_LOW) {
					bRequireSliceExtension = true;
					break;
				}
			}
			return bRequireSliceExtension;
		}

		if (sExtension == XML_3MF_NAMESPACE_PRODUCTIONSPEC) {
			// We do not write out models that require the production specification
			// i.e. we do not make use of the "path"-redirection.
			// Thus, never mark the production extension is required.
			return false;
		}

		return false;
	}

	nfUint32 CModel::getSliceStackCount() {
		return (nfUint32)m_SliceStackLookup.size();
	}

	PModelResource CModel::getSliceStackResource(_In_ nfUint32 nIndex) {
		nfUint32 nCount = getSliceStackCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_SliceStackLookup[nIndex];
	}

	CSliceStack * CModel::getSliceStack(_In_ nfUint32 nIndex)
	{
		PModelResource pResouce = getSliceStackResource(nIndex);
		CModelSliceStack * pSliceStackObject = dynamic_cast<CModelSliceStack *> (pResouce.get());
		if (pSliceStackObject == nullptr)
			throw CNMRException(NMR_ERROR_RESOURCETYPEMISMATCH);

		CSliceStack *pSliceStack = pSliceStackObject->getSliceStack().get();
		return pSliceStack;
	}


	void CModel::removeReferencedSliceStackResources()
	{
		// A slicestack resource that is referenced via the sliceref-attribute must not be referenced via the
		// slicestack-id of a mesh-object.
		// All referenced slicestack resources have already been merged into the slicestacks that reference them.
		// Therefore, remove all referencd slicestack resources.
		std::vector<PModelResource> vctToRemove;
		for (auto pSliceStackResource : m_SliceStackLookup) {
			CModelSliceStack * pSliceStackObject = dynamic_cast<CModelSliceStack *> (pSliceStackResource.get());
			if (pSliceStackObject->NumSliceRefsToMe() > 0) {
				vctToRemove.push_back(pSliceStackResource);
			}
			else {
				pSliceStackObject->sliceRefPath();
			}
		}
		for (auto resource : vctToRemove)
		{
			m_Resources.erase(std::find(m_Resources.begin(), m_Resources.end(), resource));
			m_SliceStackLookup.erase(std::find(m_SliceStackLookup.begin(), m_SliceStackLookup.end(), resource));
			m_ResourceMap.erase(resource->getResourceID()->getUniqueID());
		}

	}
}
