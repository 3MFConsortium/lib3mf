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

Abstract: This is a stub class definition of CModel

*/

#include "lib3mf_model.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_reader.hpp"
#include "lib3mf_writer.hpp"

#include "lib3mf_builditem.hpp"
#include "lib3mf_builditemiterator.hpp"
#include "lib3mf_meshobject.hpp"
#include "lib3mf_objectiterator.hpp"
#include "lib3mf_meshobjectiterator.hpp"
#include "lib3mf_resourceiterator.hpp"
#include "lib3mf_componentsobject.hpp"
#include "lib3mf_componentsobjectiterator.hpp"
#include "lib3mf_basematerialgroup.hpp"
#include "lib3mf_metadatagroup.hpp"
#include "lib3mf_attachment.hpp"
#include "lib3mf_slicestack.hpp"
#include "lib3mf_slicestackiterator.hpp"
#include "lib3mf_texture2d.hpp"
#include "lib3mf_texture2diterator.hpp"
#include "lib3mf_basematerialgroupiterator.hpp"
#include "lib3mf_colorgroup.hpp"
#include "lib3mf_colorgroupiterator.hpp"
#include "lib3mf_texture2dgroup.hpp"
#include "lib3mf_texture2dgroupiterator.hpp"
#include "lib3mf_compositematerials.hpp"
#include "lib3mf_compositematerialsiterator.hpp"
#include "lib3mf_multipropertygroup.hpp"
#include "lib3mf_multipropertygroupiterator.hpp"
#include "lib3mf_packagepart.hpp"
#include "lib3mf_keystore.hpp"


// Include custom headers here.
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Model/Classes/NMR_ModelColorGroup.h"
#include "Model/Classes/NMR_ModelTexture2DGroup.h"
#include "Model/Classes/NMR_ModelMultiPropertyGroup.h"
#include "Common/NMR_SecureContentTypes.h"
#include "lib3mf_utils.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CModel 
**************************************************************************************************************************/


CModel::CModel()
{
	m_model = std::make_shared<NMR::CModel>();
}

NMR::CModel& CModel::model()
{
	return *m_model;
}

IPackagePart* CModel::RootModelPart()
{
	return new CPackagePart(model().rootModelPath());
}

IPackagePart * CModel::FindOrCreatePackagePart(const std::string & sAbsolutePath)
{
	return new CPackagePart(model().findOrCreateModelPath(sAbsolutePath));
}

void CModel::SetUnit (const eLib3MFModelUnit eUnit)
{
	model().setUnit(NMR::eModelUnit(eUnit));
}

eLib3MFModelUnit CModel::GetUnit ()
{
	return eLib3MFModelUnit(model().getUnit());
}

std::string CModel::GetLanguage ()
{
	return model().getLanguage();
}

void CModel::SetLanguage (const std::string & sLanguage)
{
	model().setLanguage(sLanguage);
}

IWriter * CModel::QueryWriter (const std::string & sWriterClass)
{
	return new CWriter(sWriterClass, m_model);
}

IReader * CModel::QueryReader (const std::string & sReaderClass)
{
	return new CReader(sReaderClass, m_model);
}

ITexture2D * CModel::GetTexture2DByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PModelTexture2DResource pTexture2DResource = model().findTexture2D(nUniqueResourceID);
	if (pTexture2DResource) {
		return new CTexture2D(pTexture2DResource);
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDTEXTURERESOURCE);
}

eLib3MFPropertyType CModel::GetPropertyTypeByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PModelResource pResource = model().findResource(nUniqueResourceID);
	if (!pResource) {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);
	}

	if (dynamic_cast<NMR::CModelBaseMaterialResource*>(pResource.get())) {
		return ePropertyType::BaseMaterial;
	}
	else if ((dynamic_cast<NMR::CModelColorGroupResource*>(pResource.get()))) {
		return ePropertyType::Colors;
	}
	else if ((dynamic_cast<NMR::CModelTexture2DGroupResource*>(pResource.get()))) {
		return ePropertyType::TexCoord;
	}
	else if ((dynamic_cast<NMR::CModelCompositeMaterialsResource*>(pResource.get()))) {
		return ePropertyType::Composite;
	}
	else if ((dynamic_cast<NMR::CModelMultiPropertyGroupResource*>(pResource.get()))) {
		return ePropertyType::Multi;
	}
	else
		return ePropertyType::NoPropertyType;
}

IBaseMaterialGroup * CModel::GetBaseMaterialGroupByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PPackageResourceID pID = model().findPackageResourceID(nUniqueResourceID);
	if (pID) {
		NMR::PModelBaseMaterialResource pBaseMaterialResource = model().findBaseMaterial(pID);
		if (pBaseMaterialResource) {
			return new CBaseMaterialGroup(pBaseMaterialResource);
		}
		else
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDBASEMATERIALGROUP);
	}
	else {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);
	}
	
}

IMeshObject * CModel::GetMeshObjectByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PModelResource pObjectResource = model().findResource(nUniqueResourceID);
	if (dynamic_cast<NMR::CModelMeshObject*>(pObjectResource.get())) {
		return new CMeshObject(pObjectResource);
	}
	else 
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMESHOBJECT);
}

IComponentsObject * CModel::GetComponentsObjectByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PModelResource pObjectResource = model().findResource(nUniqueResourceID);
	if (dynamic_cast<NMR::CModelComponentsObject*>(pObjectResource.get())) {
		return new CComponentsObject(pObjectResource);
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOMPONENTSOBJECT);
}

IColorGroup * CModel::GetColorGroupByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PModelResource pResource = model().findResource(nUniqueResourceID);
	if (dynamic_cast<NMR::CModelColorGroupResource*>(pResource.get())) {
		return new CColorGroup(std::dynamic_pointer_cast<NMR::CModelColorGroupResource>(pResource));
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOLORGROUP);
}

ISliceStack * CModel::GetSliceStackByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PModelResource pResource = model().findResource(nUniqueResourceID);
	if (dynamic_cast<NMR::CModelSliceStack*>(pResource.get())) {
		return new CSliceStack(std::dynamic_pointer_cast<NMR::CModelSliceStack>(pResource));
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDSLICESTACKRESOURCE);
}

ITexture2DGroup * CModel::GetTexture2DGroupByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PModelResource pResource = model().findResource(nUniqueResourceID);
	if (dynamic_cast<NMR::CModelTexture2DGroupResource*>(pResource.get())) {
		return new CTexture2DGroup(std::dynamic_pointer_cast<NMR::CModelTexture2DGroupResource>(pResource));
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDTEXTURE2DGROUP);
}

ICompositeMaterials * CModel::GetCompositeMaterialsByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PModelResource pResource = model().findResource(nUniqueResourceID);
	if (dynamic_cast<NMR::CModelCompositeMaterialsResource*>(pResource.get())) {
		return new CCompositeMaterials(std::dynamic_pointer_cast<NMR::CModelCompositeMaterialsResource>(pResource));
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOMPOSITEMATERIALS);
}

IMultiPropertyGroup * CModel::GetMultiPropertyGroupByID(const Lib3MF_uint32 nUniqueResourceID)
{
	NMR::PModelResource pResource = model().findResource(nUniqueResourceID);
	if (dynamic_cast<NMR::CModelMultiPropertyGroupResource*>(pResource.get())) {
		return new CMultiPropertyGroup(std::dynamic_pointer_cast<NMR::CModelMultiPropertyGroupResource>(pResource));
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMULTIPROPERTYGROUP);
}

std::string CModel::GetBuildUUID (bool & bHasUUID)
{
	NMR::PUUID buildUUID = model().buildUUID();
	bHasUUID = buildUUID.get() != nullptr;
	if (bHasUUID)
		return buildUUID->toString();
	return "";
}

void CModel::SetBuildUUID (const std::string & sUUID)
{
	NMR::PUUID pUUID = std::make_shared<NMR::CUUID>(sUUID);
	model().setBuildUUID(pUUID);
}

IBuildItemIterator * CModel::GetBuildItems ()
{
	auto pResult = std::unique_ptr<CBuildItemIterator>(new CBuildItemIterator());
	Lib3MF_uint32 nBuildItemCount = m_model->getBuildItemCount();
	Lib3MF_uint32 nIdx;

	for (nIdx = 0; nIdx < nBuildItemCount; nIdx++)
		pResult->addBuildItem(m_model->getBuildItem(nIdx));

	return pResult.release();
}

IResourceIterator * CModel::GetResources ()
{
	auto pResult = std::unique_ptr<CResourceIterator>(new CResourceIterator());
	Lib3MF_uint32 nResourcesCount = model().getResourceCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nResourcesCount; nIdx++) {
		auto resource = model().getResource(nIdx);
		pResult->addResource(resource);
	}
	return pResult.release();
}

IObjectIterator * CModel::GetObjects ()
{
	auto pResult = std::unique_ptr<CObjectIterator>(new CObjectIterator());
	Lib3MF_uint32 nObjectsCount = model().getObjectCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nObjectsCount; nIdx++) {
		auto resource = model().getObjectResource(nIdx);
		pResult->addResource(resource);
	}
	return pResult.release();
}

IMeshObjectIterator * CModel::GetMeshObjects ()
{
	auto pResult = std::unique_ptr<CMeshObjectIterator>(new CMeshObjectIterator());
	Lib3MF_uint32 nObjectsCount = model().getObjectCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nObjectsCount; nIdx++) {
		auto resource = model().getObjectResource(nIdx);
		if (dynamic_cast<NMR::CModelMeshObject *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

IComponentsObjectIterator * CModel::GetComponentsObjects()
{
	auto pResult = std::unique_ptr<CComponentsObjectIterator>(new CComponentsObjectIterator());
	Lib3MF_uint32 nObjectsCount = model().getObjectCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nObjectsCount; nIdx++) {
		auto resource = model().getObjectResource(nIdx);
		if (dynamic_cast<NMR::CModelComponentsObject *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

ITexture2DIterator * CModel::GetTexture2Ds()
{
	auto pResult = std::unique_ptr<CTexture2DIterator>(new CTexture2DIterator());
	Lib3MF_uint32 nCount = model().getTexture2DCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getTexture2DResource(nIdx);
		if (dynamic_cast<NMR::CModelTexture2DResource *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

IBaseMaterialGroupIterator * CModel::GetBaseMaterialGroups()
{
	auto pResult = std::unique_ptr<CBaseMaterialGroupIterator>(new CBaseMaterialGroupIterator());
	Lib3MF_uint32 nCount = model().getBaseMaterialCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getBaseMaterialResource(nIdx);
		if (dynamic_cast<NMR::CModelBaseMaterialResource *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

IColorGroupIterator * CModel::GetColorGroups()
{
	auto pResult = std::unique_ptr<CColorGroupIterator>(new CColorGroupIterator());
	Lib3MF_uint32 nCount = model().getColorGroupCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getColorGroupResource(nIdx);
		if (dynamic_cast<NMR::CModelColorGroupResource *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

ITexture2DGroupIterator * CModel::GetTexture2DGroups()
{
	auto pResult = std::unique_ptr<CTexture2DGroupIterator>(new CTexture2DGroupIterator());
	Lib3MF_uint32 nCount = model().getTexture2DGroupCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getTexture2DGroupResource(nIdx);
		if (dynamic_cast<NMR::CModelTexture2DGroupResource *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

ICompositeMaterialsIterator * CModel::GetCompositeMaterials()
{
	auto pResult = std::unique_ptr<CCompositeMaterialsIterator>(new CCompositeMaterialsIterator());
	Lib3MF_uint32 nCount = model().getCompositeMaterialsCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getCompositeMaterialsResource(nIdx);
		if (dynamic_cast<NMR::CModelCompositeMaterialsResource *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

IMultiPropertyGroupIterator * CModel::GetMultiPropertyGroups()
{
	auto pResult = std::unique_ptr<CMultiPropertyGroupIterator>(new CMultiPropertyGroupIterator());
	Lib3MF_uint32 nCount = model().getMultiPropertyGroupCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getMultiPropertyGroupResource(nIdx);
		if (dynamic_cast<NMR::CModelMultiPropertyGroupResource *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}


ISliceStackIterator * CModel::GetSliceStacks()
{
	auto pResult = std::unique_ptr<CSliceStackIterator>(new CSliceStackIterator());
	Lib3MF_uint32 nCount = model().getSliceStackCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getSliceStackResource(nIdx);
		if (dynamic_cast<NMR::CModelSliceStack *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

IModel * CModel::MergeToModel ()
{
	// Create merged mesh
	NMR::PMesh pMesh = std::make_shared<NMR::CMesh>();
	model().mergeToMesh(pMesh.get());

	auto pOutModel = std::unique_ptr<CModel>(new CModel());

	// Copy relevant resources to new model
	NMR::CModel& newModel = pOutModel->model();

	NMR::UniqueResourceIDMapping oldToNewUniqueResourceIDs;

	newModel.mergeModelAttachments(&model());
	newModel.mergeTextures2D(&model(), oldToNewUniqueResourceIDs);
	newModel.mergeBaseMaterials(&model(), oldToNewUniqueResourceIDs);
	newModel.mergeColorGroups(&model(), oldToNewUniqueResourceIDs);
	newModel.mergeTexture2DGroups(&model(), oldToNewUniqueResourceIDs);
	newModel.mergeCompositeMaterials(&model(), oldToNewUniqueResourceIDs);
	newModel.mergeMultiPropertyGroups(&model(), oldToNewUniqueResourceIDs);
	newModel.mergeMetaData(&model());

	pMesh->patchMeshInformationResources(oldToNewUniqueResourceIDs);

	newModel.setUnit(model().getUnit());
	newModel.setLanguage(model().getLanguage());

	NMR::PModelMeshObject pMeshObject = std::make_shared<NMR::CModelMeshObject>(newModel.generateResourceID(), &newModel, pMesh);
	newModel.addResource(pMeshObject);

	NMR::PModelBuildItem pBuildItem = std::make_shared<NMR::CModelBuildItem>(pMeshObject.get(), model().createHandle());
	newModel.addBuildItem(pBuildItem);

	return pOutModel.release();
}

IMeshObject * CModel::AddMeshObject ()
{
	NMR::ModelResourceID NewResourceID = model().generateResourceID();
	NMR::PMesh pNewMesh = std::make_shared<NMR::CMesh>();
	NMR::PModelMeshObject pNewResource = std::make_shared<NMR::CModelMeshObject>(NewResourceID, &model(), pNewMesh);

	model().addResource(pNewResource);
	return new CMeshObject(pNewResource);
}

IComponentsObject * CModel::AddComponentsObject ()
{
	NMR::ModelResourceID NewResourceID = model().generateResourceID();
	NMR::PModelComponentsObject pNewResource = std::make_shared<NMR::CModelComponentsObject>(NewResourceID, &model());

	model().addResource(pNewResource);
	return new CComponentsObject(pNewResource);
}

ISliceStack * CModel::AddSliceStack(const Lib3MF_double dZBottom)
{
	NMR::ModelResourceID NewResourceID = model().generateResourceID();
	NMR::PModelSliceStack pNewResource = std::make_shared<NMR::CModelSliceStack>(NewResourceID, &model(), dZBottom);

	model().addResource(pNewResource);

	return new CSliceStack(pNewResource);
}


ITexture2D * CModel::AddTexture2DFromAttachment (IAttachment* pTextureAttachment)
{
	NMR::PModelAttachment attachment = model().findModelAttachment(pTextureAttachment->GetPath());

	NMR::PModelTexture2DResource pResource = NMR::CModelTexture2DResource::make(model().generateResourceID(), &model(), attachment);
	model().addResource(pResource);

	return new CTexture2D(pResource);
}

IBaseMaterialGroup * CModel::AddBaseMaterialGroup ()
{
	NMR::PModelBaseMaterialResource pResource = std::make_shared<NMR::CModelBaseMaterialResource>(model().generateResourceID(), &model());
	model().addResource(pResource);

	return new CBaseMaterialGroup(pResource);
}

IColorGroup * CModel::AddColorGroup()
{
	NMR::PModelColorGroupResource pResource = std::make_shared<NMR::CModelColorGroupResource>(model().generateResourceID(), &model());
	model().addResource(pResource);

	return new CColorGroup(pResource);
}

ITexture2DGroup * CModel::AddTexture2DGroup(ITexture2D* pTexture2DInstance)
{
	NMR::UniqueResourceID nTexture2DID = pTexture2DInstance->GetResourceID();

	// Find class instance
	NMR::PModelTexture2DResource pModelTexture2DObject = model().findTexture2D(nTexture2DID);
	if (pModelTexture2DObject == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND); 

	NMR::PModelTexture2DGroupResource pResource = std::make_shared<NMR::CModelTexture2DGroupResource>(model().generateResourceID(), &model(), pModelTexture2DObject);
	model().addResource(pResource);

	return new CTexture2DGroup(pResource);
}

ICompositeMaterials * CModel::AddCompositeMaterials(IBaseMaterialGroup* pBaseMaterialGroupInstance)
{
	NMR::UniqueResourceID nBaseMaterialGroupID = pBaseMaterialGroupInstance->GetUniqueResourceID();
	NMR::PPackageResourceID pID = model().findPackageResourceID(nBaseMaterialGroupID);
	// Find class instance
	NMR::PModelBaseMaterialResource pModelBaseMaterialGroup = model().findBaseMaterial(pID);
	if (pModelBaseMaterialGroup == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	NMR::PModelCompositeMaterialsResource pResource = std::make_shared<NMR::CModelCompositeMaterialsResource>(model().generateResourceID(), &model(), pModelBaseMaterialGroup);
	model().addResource(pResource);

	return new CCompositeMaterials(pResource);
}

IMultiPropertyGroup * CModel::AddMultiPropertyGroup()
{
	NMR::PModelMultiPropertyGroupResource pResource = std::make_shared<NMR::CModelMultiPropertyGroupResource>(model().generateResourceID(), &model());
	model().addResource(pResource);

	return new CMultiPropertyGroup(pResource);
}

IBuildItem * CModel::AddBuildItem (IObject* pObject, const sLib3MFTransform Transform)
{
	// Get Resource ID
	NMR::UniqueResourceID nObjectID = pObject->GetResourceID();
	
	// Find class instance
	NMR::CModelObject * pModelObject = model().findObject(nObjectID);
	if (pModelObject == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	// Create Build item instance
	NMR::PModelBuildItem pModelBuildItem = std::make_shared<NMR::CModelBuildItem>(pModelObject, TransformToMatrix(Transform), model().createHandle());
	model().addBuildItem(pModelBuildItem);

	return new CBuildItem(pModelBuildItem);
}

void CModel::RemoveBuildItem (IBuildItem* pBuildItemInstance)
{
	CBuildItem* pLib3MFBuildItem = dynamic_cast<CBuildItem*> (pBuildItemInstance);
	if (!pLib3MFBuildItem)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDBUILDITEM);
	model().removeBuildItem(pLib3MFBuildItem->GetHandle(), true);
}

IMetaDataGroup * CModel::GetMetaDataGroup ()
{
	return new CMetaDataGroup(model().getMetaDataGroup());
}

IAttachment * CModel::AddAttachment (const std::string & sURI, const std::string & sRelationShipType)
{
	NMR::PImportStream pStream = std::make_shared<NMR::CImportStream_Unique_Memory>();

	NMR::PModelAttachment pModelAttachment(model().addAttachment(sURI, sRelationShipType, pStream));

	return new CAttachment(pModelAttachment);
}

IAttachment * CModel::GetAttachment (const Lib3MF_uint32 nIndex)
{
	NMR::PModelAttachment pAttachment = m_model->getModelAttachment(nIndex);
	return new CAttachment(pAttachment);
}

IAttachment * CModel::FindAttachment (const std::string & sURI)
{
	NMR::PModelAttachment pAttachment = m_model->findModelAttachment(sURI);

	if (pAttachment.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ATTACHMENTNOTFOUND);
	
	return new CAttachment(pAttachment);
}

void CModel::RemoveAttachment(IAttachment* pAttachmentInstance)
{
	m_model->removeAttachment(pAttachmentInstance->GetPath());
}

Lib3MF_uint32 CModel::GetAttachmentCount ()
{
	return m_model->getAttachmentCount();
}

bool CModel::HasPackageThumbnailAttachment()
{
	return m_model->getPackageThumbnail() != nullptr;
}

IAttachment * CModel::CreatePackageThumbnailAttachment()
{
	NMR::PModelAttachment pModelAttachment;
	if (HasPackageThumbnailAttachment())
	{
		pModelAttachment = m_model->getPackageThumbnail();
	}
	else {
		pModelAttachment = m_model->addPackageThumbnail();
	}
	if (pModelAttachment) {
		return new CAttachment(pModelAttachment);
	}
	else {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ATTACHMENTNOTFOUND);
	}
}

IAttachment * CModel::GetPackageThumbnailAttachment()
{
	if (HasPackageThumbnailAttachment())
	{
		return new CAttachment(m_model->getPackageThumbnail());
	}
	else {
		return nullptr;
	}
}

void CModel::RemovePackageThumbnailAttachment()
{
	m_model->removePackageThumbnail();
}

void CModel::AddCustomContentType (const std::string & sExtension, const std::string & sContentType)
{
	m_model->addCustomContentType(sExtension, sContentType);
}

void CModel::RemoveCustomContentType (const std::string & sExtension)
{
	m_model->removeCustomContentType(sExtension);
}

Lib3MF::sBox CModel::GetOutbox()
{
	NMR::NOUTBOX3 sOutbox;
	NMR::fnOutboxInitialize(sOutbox);

	for (NMR::nfUint32 iBuildItem = 0; iBuildItem < model().getBuildItemCount(); iBuildItem++) {
		auto pBuildItem = model().getBuildItem(iBuildItem);
		pBuildItem->getObject()->extendOutbox(sOutbox, pBuildItem->getTransform());
	}

	sBox s;
	s.m_MinCoordinate[0] = sOutbox.m_min.m_fields[0];
	s.m_MinCoordinate[1] = sOutbox.m_min.m_fields[1];
	s.m_MinCoordinate[2] = sOutbox.m_min.m_fields[2];

	s.m_MaxCoordinate[0] = sOutbox.m_max.m_fields[0];
	s.m_MaxCoordinate[1] = sOutbox.m_max.m_fields[1];
	s.m_MaxCoordinate[2] = sOutbox.m_max.m_fields[2];
	return s;
}

IKeyStore * Lib3MF::Impl::CModel::GetKeyStore() {
	return new CKeyStore(m_model);
}

void Lib3MF::Impl::CModel::SetRandomNumberCallback(Lib3MF::RandomNumberCallback pTheCallback, Lib3MF_pvoid pUserData) {
	if (nullptr == pTheCallback)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	NMR::CryptoRandGenDescriptor descriptor;
	descriptor.m_pUserData = pUserData;
	descriptor.m_fnRNG = [pTheCallback](NMR::nfByte * buffer, NMR::nfUint64 size, void * userData) {
		Lib3MF_uint64 generated = 0;
		(*pTheCallback)((Lib3MF_uint64)buffer, size, userData, &generated);
		if (generated > 0)
			return generated;
		throw NMR::CNMRException(NMR_ERROR_CALCULATIONTERMINATED);
	};

	m_model->setCryptoRandCallback(descriptor);
}

