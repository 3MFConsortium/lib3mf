/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CLib3MFModel

*/

#include "lib3mf_model.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_reader.hpp"
#include "lib3mf_writer.hpp"

#include "lib3mf_builditem.hpp"
#include "lib3mf_builditemiterator.hpp"
#include "lib3mf_meshobject.hpp"
#include "lib3mf_resourceiterator.hpp"
#include "lib3mf_componentsobject.hpp"
#include "lib3mf_basematerialgroup.hpp"
#include "lib3mf_metadatagroup.hpp"
#include "lib3mf_attachment.hpp"
#include "lib3mf_slicestack.hpp"
#include "lib3mf_nurbssurface.hpp"
#include "lib3mf_nurbssurfaceiterator.hpp"
#include "lib3mf_nurbscurve.hpp"
#include "lib3mf_nurbscurveiterator.hpp"

#include "lib3mf_texture2d.hpp"
#include "lib3mf_texture2diterator.hpp"
#include "lib3mf_basematerialgroupiterator.hpp"
#include "lib3mf_colorgroup.hpp"
#include "lib3mf_colorgroupiterator.hpp"
// Include custom headers here.

#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_ModelNurbsSurface.h"
#include "Model/Classes/NMR_ModelNurbsCurve.h"
#include "Common/Platform/NMR_ImportStream_Memory.h"
#include "Model/Classes/NMR_ModelColorGroup.h" 

#include "lib3mf_utils.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFModel 
**************************************************************************************************************************/


CLib3MFModel::CLib3MFModel()
{
	m_model = std::make_shared<NMR::CModel>();
}

NMR::CModel& CLib3MFModel::model()
{
	return *m_model;
}

void CLib3MFModel::SetUnit (const eLib3MFModelUnit eUnit)
{
	model().setUnit(NMR::eModelUnit(eUnit));
}

eLib3MFModelUnit CLib3MFModel::GetUnit ()
{
	return eLib3MFModelUnit(model().getUnit());
}

std::string CLib3MFModel::GetLanguage ()
{
	return model().getLanguage();
}

void CLib3MFModel::SetLanguage (const std::string & sLanguage)
{
	model().setLanguage(sLanguage);
}

ILib3MFWriter * CLib3MFModel::QueryWriter (const std::string & sWriterClass)
{
	return new CLib3MFWriter(sWriterClass, m_model);
}

ILib3MFReader * CLib3MFModel::QueryReader (const std::string & sReaderClass)
{
	return new CLib3MFReader(sReaderClass, m_model);
}

ILib3MFTexture2D * CLib3MFModel::GetTexture2DByID (const Lib3MF_uint32 nResourceID)
{
	NMR::PModelTexture2DResource pTexture2DResource = model().findTexture2D(nResourceID);
	if (pTexture2DResource) {
		return new CLib3MFTexture2D(pTexture2DResource);
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDTEXTURERESOURCE);
}

ILib3MFBaseMaterialGroup * CLib3MFModel::GetBaseMaterialGroupByID (const Lib3MF_uint32 nResourceID)
{
	NMR::PModelBaseMaterialResource pBaseMaterialResource = model().findBaseMaterial(nResourceID);
	if (pBaseMaterialResource) {
		return new CLib3MFBaseMaterialGroup(pBaseMaterialResource);
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDBASEMATERIALGROUP);
}

ILib3MFMeshObject * CLib3MFModel::GetMeshObjectByID (const Lib3MF_uint32 nResourceID)
{
	NMR::PModelResource pObjectResource = model().findResource(nResourceID);
	if (dynamic_cast<NMR::CModelMeshObject*>(pObjectResource.get())) {
		return new CLib3MFMeshObject(pObjectResource);
	}
	else 
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMESHOBJECT);
}

ILib3MFComponentsObject * CLib3MFModel::GetComponentsObjectByID (const Lib3MF_uint32 nResourceID)
{
	NMR::PModelResource pObjectResource = model().findResource(nResourceID);
	if (dynamic_cast<NMR::CModelComponentsObject*>(pObjectResource.get())) {
		return new CLib3MFComponentsObject(pObjectResource);
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOMPONENTSOBJECT);
}

ILib3MFColorGroup * CLib3MFModel::GetColorGroupByID(const Lib3MF_uint32 nResourceID)
{
	NMR::PModelResource pResource = model().findResource(nResourceID);
	if (dynamic_cast<NMR::CModelColorGroupResource*>(pResource.get())) {
		return new CLib3MFColorGroup(std::dynamic_pointer_cast<NMR::CModelColorGroupResource>(pResource));
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOLORGROUP);
}

ILib3MFSliceStack * CLib3MFModel::GetSliceStackByID(const Lib3MF_uint32 nResourceID)
{
	NMR::PModelResource pResource = model().findResource(nResourceID);
	if (dynamic_cast<NMR::CModelSliceStack*>(pResource.get())) {
		return new CLib3MFSliceStack(std::dynamic_pointer_cast<NMR::CModelSliceStack>(pResource));
	}
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDSLICESTACKRESOURCE);
}

std::string CLib3MFModel::GetBuildUUID (bool & bHasUUID)
{
	NMR::PUUID buildUUID = model().buildUUID();
	bHasUUID = buildUUID.get() != nullptr;
	if (bHasUUID)
		return buildUUID->toString();
	return "";
}

void CLib3MFModel::SetBuildUUID (const std::string & sUUID)
{
	NMR::PUUID pUUID = std::make_shared<NMR::CUUID>(sUUID);
	model().setBuildUUID(pUUID);
}

ILib3MFBuildItemIterator * CLib3MFModel::GetBuildItems ()
{
	auto pResult = std::unique_ptr<CLib3MFBuildItemIterator>(new CLib3MFBuildItemIterator());
	Lib3MF_uint32 nBuildItemCount = m_model->getBuildItemCount();
	Lib3MF_uint32 nIdx;

	for (nIdx = 0; nIdx < nBuildItemCount; nIdx++)
		pResult->addBuildItem(m_model->getBuildItem(nIdx));

	return pResult.release();
}


ILib3MFNurbsSurfaceIterator * CLib3MFModel::GetNurbsSurfaces()
{
	auto pResult = std::make_unique<CLib3MFNurbsSurfaceIterator>();
	Lib3MF_uint32 nResourceCount = model().getResourceCount();
	Lib3MF_uint32 nIdx;

	for (nIdx = 0; nIdx < nResourceCount; nIdx++) {
		auto resource = model().getResource(nIdx);
		if (dynamic_cast<NMR::CModelNurbsSurface *>(resource.get()))
			pResult->addResource(resource);
	}

	return pResult.release();
}


ILib3MFNurbsCurveIterator * CLib3MFModel::GetNurbsCurves()
{
	auto pResult = std::make_unique<CLib3MFNurbsCurveIterator>();
	Lib3MF_uint32 nResourceCount = model().getResourceCount();
	Lib3MF_uint32 nIdx;

	for (nIdx = 0; nIdx < nResourceCount; nIdx++) {
		auto resource = model().getResource(nIdx);
		if (dynamic_cast<NMR::CModelNurbsCurve *>(resource.get()))
			pResult->addResource(resource);
	}

	return pResult.release();

}


ILib3MFResourceIterator * CLib3MFModel::GetResources ()
{
	auto pResult = std::unique_ptr<CLib3MFResourceIterator>(new CLib3MFResourceIterator());
	Lib3MF_uint32 nResourcesCount = model().getResourceCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nResourcesCount; nIdx++) {
		auto resource = model().getResource(nIdx);
		pResult->addResource(resource);
	}
	return pResult.release();
}

ILib3MFResourceIterator * CLib3MFModel::GetObjects ()
{
	auto pResult = std::unique_ptr<CLib3MFResourceIterator>(new CLib3MFResourceIterator());
	Lib3MF_uint32 nObjectsCount = model().getObjectCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nObjectsCount; nIdx++) {
		auto resource = model().getObjectResource(nIdx);
		pResult->addResource(resource);
	}
	return pResult.release();
}

ILib3MFResourceIterator * CLib3MFModel::GetMeshObjects ()
{
	auto pResult = std::unique_ptr<CLib3MFResourceIterator>(new CLib3MFResourceIterator());
	Lib3MF_uint32 nObjectsCount = model().getObjectCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nObjectsCount; nIdx++) {
		auto resource = model().getObjectResource(nIdx);
		if (dynamic_cast<NMR::CModelMeshObject *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

ILib3MFResourceIterator * CLib3MFModel::GetComponentsObjects()
{
	auto pResult = std::unique_ptr<CLib3MFResourceIterator>(new CLib3MFResourceIterator());
	Lib3MF_uint32 nObjectsCount = model().getObjectCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nObjectsCount; nIdx++) {
		auto resource = model().getObjectResource(nIdx);
		if (dynamic_cast<NMR::CModelComponentsObject *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

ILib3MFTexture2DIterator * CLib3MFModel::GetTexture2Ds()
{
	auto pResult = std::unique_ptr<CLib3MFTexture2DIterator>(new CLib3MFTexture2DIterator());
	Lib3MF_uint32 nCount = model().getTexture2DCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getTexture2DResource(nIdx);
		if (dynamic_cast<NMR::CModelTexture2DResource *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

ILib3MFBaseMaterialGroupIterator * CLib3MFModel::GetBaseMaterialGroups()
{
	auto pResult = std::unique_ptr<CLib3MFBaseMaterialGroupIterator>(new CLib3MFBaseMaterialGroupIterator());
	Lib3MF_uint32 nCount = model().getBaseMaterialCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getBaseMaterialResource(nIdx);
		if (dynamic_cast<NMR::CModelBaseMaterialResource *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

ILib3MFColorGroupIterator * CLib3MFModel::GetColorGroups()
{
	auto pResult = std::unique_ptr<CLib3MFColorGroupIterator>(new CLib3MFColorGroupIterator());
	Lib3MF_uint32 nCount = model().getColorGroupCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getColorGroupResource(nIdx);
		if (dynamic_cast<NMR::CModelColorGroupResource *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

ILib3MFResourceIterator * CLib3MFModel::GetSliceStacks()
{
	auto pResult = std::unique_ptr<CLib3MFResourceIterator>(new CLib3MFResourceIterator());
	Lib3MF_uint32 nCount = model().getSliceStackCount();

	for (Lib3MF_uint32 nIdx = 0; nIdx < nCount; nIdx++) {
		auto resource = model().getSliceStackResource(nIdx);
		if (dynamic_cast<NMR::CModelSliceStack *>(resource.get()))
			pResult->addResource(resource);
	}
	return pResult.release();
}

ILib3MFModel * CLib3MFModel::MergeToModel ()
{
	// Create merged mesh
	NMR::PMesh pMesh = std::make_shared<NMR::CMesh>();
	model().mergeToMesh(pMesh.get());

	auto pOutModel = std::unique_ptr<CLib3MFModel>(new CLib3MFModel());

	// Copy relevant resources to new model
	NMR::CModel& newModel = pOutModel->model();

	newModel.mergeModelAttachments(&model());
	newModel.mergeTextures2D(&model());
	newModel.mergeBaseMaterials(&model());
	newModel.mergeColorGroups(&model());
	newModel.mergeMetaData(&model());

	newModel.setUnit(model().getUnit());
	newModel.setLanguage(model().getLanguage());

	NMR::PModelMeshObject pMeshObject = std::make_shared<NMR::CModelMeshObject>(newModel.generateResourceID(), &newModel, pMesh);
	newModel.addResource(pMeshObject);

	NMR::PModelBuildItem pBuildItem = std::make_shared<NMR::CModelBuildItem>(pMeshObject.get(), model().createHandle());
	newModel.addBuildItem(pBuildItem);

	return pOutModel.release();
}

ILib3MFMeshObject * CLib3MFModel::AddMeshObject ()
{
	NMR::ModelResourceID NewResourceID = model().generateResourceID();
	NMR::PMesh pNewMesh = std::make_shared<NMR::CMesh>();
	NMR::PModelMeshObject pNewResource = std::make_shared<NMR::CModelMeshObject>(NewResourceID, &model(), pNewMesh);

	model().addResource(pNewResource);
	return new CLib3MFMeshObject(pNewResource);
}

ILib3MFNurbsSurface * CLib3MFModel::AddNurbsSurface(const Lib3MF_uint32 nDegreeU, const Lib3MF_uint32 nDegreeV, const Lib3MF_uint32 nControlPointsU, const Lib3MF_uint32 nControlPointsV)
{
	NMR::ModelResourceID NewResourceID = model().generateResourceID();
	NMR::PModelNurbsSurface pNewResource = std::make_shared<NMR::CModelNurbsSurface>(NewResourceID, &model(), nDegreeU, nDegreeV, nControlPointsU, nControlPointsV);

	model().addResource(pNewResource);
	return new CLib3MFNurbsSurface (pNewResource);
	
}

ILib3MFNurbsCurve * CLib3MFModel::AddNurbsCurve(const Lib3MF_uint32 nDegree, const Lib3MF_uint32 nControlPoints)
{
	NMR::ModelResourceID NewResourceID = model().generateResourceID();
	NMR::PModelNurbsCurve pNewResource = std::make_shared<NMR::CModelNurbsCurve>(NewResourceID, &model(), nDegree, nControlPoints);

	model().addResource(pNewResource);
	return new CLib3MFNurbsCurve(pNewResource);

}



ILib3MFComponentsObject * CLib3MFModel::AddComponentsObject ()
{
	NMR::ModelResourceID NewResourceID = model().generateResourceID();
	NMR::PModelComponentsObject pNewResource = std::make_shared<NMR::CModelComponentsObject>(NewResourceID, &model());

	model().addResource(pNewResource);
	return new CLib3MFComponentsObject(pNewResource);
}

ILib3MFSliceStack * CLib3MFModel::AddSliceStack(const Lib3MF_double dZBottom)
{
	NMR::ModelResourceID NewResourceID = model().generateResourceID();
	NMR::PModelSliceStack pNewResource = std::make_shared<NMR::CModelSliceStack>(NewResourceID, &model(), dZBottom);

	model().addResource(pNewResource);

	return new CLib3MFSliceStack(pNewResource);
}


ILib3MFTexture2D * CLib3MFModel::AddTexture2DFromAttachment (ILib3MFAttachment* pTextureAttachment)
{
	NMR::PModelAttachment attachment = model().findModelAttachment(pTextureAttachment->GetPath());

	NMR::PModelTexture2DResource pResource = NMR::CModelTexture2DResource::make(model().generateResourceID(), &model(), attachment);
	model().addResource(pResource);

	return new CLib3MFTexture2D(pResource);
}

ILib3MFBaseMaterialGroup * CLib3MFModel::AddBaseMaterialGroup ()
{
	NMR::PModelBaseMaterialResource pResource = std::make_shared<NMR::CModelBaseMaterialResource>(model().generateResourceID(), &model());
	model().addResource(pResource);

	return new CLib3MFBaseMaterialGroup(pResource);
}

ILib3MFColorGroup * CLib3MFModel::AddColorGroup()
{
	NMR::PModelColorGroupResource pResource = std::make_shared<NMR::CModelColorGroupResource>(model().generateResourceID(), &model());
	model().addResource(pResource);

	return new CLib3MFColorGroup(pResource);
}

ILib3MFBuildItem * CLib3MFModel::AddBuildItem (ILib3MFObject* pObject, const sLib3MFTransform Transform)
{
	// Get Resource ID
	NMR::PackageResourceID nObjectID = pObject->GetResourceID();
	
	// Find class instance
	NMR::CModelObject * pModelObject = model().findObject(nObjectID);
	if (pModelObject == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	// Create Build item instance
	NMR::PModelBuildItem pModelBuildItem = std::make_shared<NMR::CModelBuildItem>(pModelObject, TransformToMatrix(Transform), model().createHandle());
	model().addBuildItem(pModelBuildItem);

	return new CLib3MFBuildItem(pModelBuildItem);
}

void CLib3MFModel::RemoveBuildItem (ILib3MFBuildItem* pBuildItemInstance)
{
	CLib3MFBuildItem* pLib3MFBuildItem = dynamic_cast<CLib3MFBuildItem*> (pBuildItemInstance);
	if (!pLib3MFBuildItem)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDBUILDITEM);
	model().removeBuildItem(pLib3MFBuildItem->GetHandle(), true);
}

ILib3MFMetaDataGroup * CLib3MFModel::GetMetaDataGroup ()
{
	return new CLib3MFMetaDataGroup(model().getMetaDataGroup());
}

ILib3MFAttachment * CLib3MFModel::AddAttachment (const std::string & sURI, const std::string & sRelationShipType)
{
	NMR::PImportStream pStream = std::make_shared<NMR::CImportStream_Memory>();

	NMR::PModelAttachment pModelAttachment(model().addAttachment(sURI, sRelationShipType, pStream));

	return new CLib3MFAttachment(pModelAttachment);
}

ILib3MFAttachment * CLib3MFModel::GetAttachment (const Lib3MF_uint32 nIndex)
{
	NMR::PModelAttachment pAttachment = m_model->getModelAttachment(nIndex);
	return new CLib3MFAttachment(pAttachment);
}

ILib3MFAttachment * CLib3MFModel::FindAttachment (const std::string & sURI)
{
	NMR::PModelAttachment pAttachment = m_model->findModelAttachment(sURI);

	if (pAttachment.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ATTACHMENTNOTFOUND);
	
	return new CLib3MFAttachment(pAttachment);
}

void CLib3MFModel::RemoveAttachment(ILib3MFAttachment* pAttachmentInstance)
{
	m_model->removeAttachment(pAttachmentInstance->GetPath());
}

Lib3MF_uint32 CLib3MFModel::GetAttachmentCount ()
{
	return m_model->getAttachmentCount();
}

bool CLib3MFModel::HasPackageThumbnailAttachment ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILib3MFAttachment * CLib3MFModel::CreatePackageThumbnailAttachment ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILib3MFAttachment * CLib3MFModel::GetPackageThumbnailAttachment ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFModel::RemovePackageThumbnailAttachment ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFModel::AddCustomContentType (const std::string & sExtension, const std::string & sContentType)
{
	m_model->addCustomContentType(sExtension, sContentType);
}

void CLib3MFModel::RemoveCustomContentType (const std::string & sExtension)
{
	m_model->removeCustomContentType(sExtension);
}

