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

Abstract: This is the class declaration of CModel

*/


#ifndef __LIB3MF_MODEL
#define __LIB3MF_MODEL

#include "lib3mf_interfaces.hpp"
#include "lib3mf_base.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif


// Include custom headers here.
#include "Model/Classes/NMR_Model.h" 
#include "Model/Classes/NMR_KeyStore.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CModel 
**************************************************************************************************************************/

class CModel : public virtual IModel, public virtual CBase {
private:

	NMR::PModel m_model;

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CModel();

	NMR::CModel& model();

	/**
	* Public member functions to implement.
	*/

	IPackagePart * RootModelPart() override;

	IPackagePart * FindOrCreatePackagePart(const std::string & sAbsolutePath) override;

	void SetUnit (const eLib3MFModelUnit eUnit) override;

	eLib3MFModelUnit GetUnit() override;

	std::string GetLanguage() override;

	void SetLanguage(const std::string & sLanguage) override;

	IWriter * QueryWriter(const std::string & sWriterClass) override;

	IReader * QueryReader(const std::string & sReaderClass) override;

	ITexture2D * GetTexture2DByID(const Lib3MF_uint32 nUniqueResourceID) override;

	eLib3MFPropertyType GetPropertyTypeByID(const Lib3MF_uint32 nUniqueResourceID) override;

	IBaseMaterialGroup * GetBaseMaterialGroupByID(const Lib3MF_uint32 nUniqueResourceID) override;

	IMeshObject * GetMeshObjectByID(const Lib3MF_uint32 nUniqueResourceID) override;

	IComponentsObject * GetComponentsObjectByID(const Lib3MF_uint32 nUniqueResourceID) override;

	IColorGroup * GetColorGroupByID(const Lib3MF_uint32 nUniqueResourceID) override;

	ITexture2DGroup * GetTexture2DGroupByID(const Lib3MF_uint32 nUniqueResourceID) override;

	ICompositeMaterials * GetCompositeMaterialsByID(const Lib3MF_uint32 nUniqueResourceID) override;

	IMultiPropertyGroup * GetMultiPropertyGroupByID(const Lib3MF_uint32 nUniqueResourceID) override;

	ISliceStack * GetSliceStackByID(const Lib3MF_uint32 nUniqueResourceID) override;

	std::string GetBuildUUID (bool & bHasUUID) override;

	void SetBuildUUID (const std::string & sUUID) override;

	IBuildItemIterator * GetBuildItems() override;

	IResourceIterator * GetResources() override;

	IObjectIterator * GetObjects() override;

	IMeshObjectIterator * GetMeshObjects() override;

	IComponentsObjectIterator * GetComponentsObjects() override;

	ITexture2DIterator * GetTexture2Ds() override;

	IBaseMaterialGroupIterator * GetBaseMaterialGroups() override;

	IColorGroupIterator * GetColorGroups() override;

	ITexture2DGroupIterator * GetTexture2DGroups() override;

	ICompositeMaterialsIterator * GetCompositeMaterials() override;

	IMultiPropertyGroupIterator * GetMultiPropertyGroups() override;

	ISliceStackIterator * GetSliceStacks() override;

	IModel * MergeToModel() override;

	IMeshObject * AddMeshObject() override;

	IComponentsObject * AddComponentsObject() override;

	ISliceStack * AddSliceStack(const Lib3MF_double dZBottom) override;

	ITexture2D * AddTexture2DFromAttachment(IAttachment* pTextureAttachment) override;

	IBaseMaterialGroup * AddBaseMaterialGroup() override;

	IColorGroup * AddColorGroup() override;

	ITexture2DGroup * AddTexture2DGroup(ITexture2D* pTexture2DInstance) override;

	ICompositeMaterials * AddCompositeMaterials(IBaseMaterialGroup* pBaseMaterialGroupInstance) override;

	IMultiPropertyGroup * AddMultiPropertyGroup() override;

	IBuildItem * AddBuildItem(IObject* pObject, const sLib3MFTransform Transform) override;

	void RemoveBuildItem(IBuildItem* pBuildItemInstance) override;

	IMetaDataGroup * GetMetaDataGroup() override;

	IAttachment * AddAttachment(const std::string & sURI, const std::string & sRelationShipType) override;

	void RemoveAttachment(IAttachment* pAttachmentInstance) override;

	IAttachment * GetAttachment(const Lib3MF_uint32 nIndex) override;

	IAttachment * FindAttachment(const std::string & sURI) override;

	Lib3MF_uint32 GetAttachmentCount() override;

	bool HasPackageThumbnailAttachment() override;

	IAttachment * CreatePackageThumbnailAttachment() override;

	IAttachment * GetPackageThumbnailAttachment() override;

	void RemovePackageThumbnailAttachment() override;

	void AddCustomContentType(const std::string & sExtension, const std::string & sContentType) override;

	void RemoveCustomContentType(const std::string & sExtension) override;

	Lib3MF::sBox GetOutbox() override;

	IKeyStore * GetKeyStore();

	void SetRandomNumberCallback(const Lib3MF::RandomNumberCallback pTheCallback, const Lib3MF_pvoid pUserData);
};

}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // __LIB3MF_MODEL
