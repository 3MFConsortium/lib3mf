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

	IPackagePath * RootModel() override;

	void SetUnit (const eLib3MFModelUnit eUnit);

	eLib3MFModelUnit GetUnit ();

	std::string GetLanguage ();

	void SetLanguage (const std::string & sLanguage);

	IWriter * QueryWriter (const std::string & sWriterClass);

	IReader * QueryReader (const std::string & sReaderClass);

	ITexture2D * GetTexture2DByID (const Lib3MF_uint32 nUniqueResourceID);

	eLib3MFPropertyType GetPropertyTypeByID(const Lib3MF_uint32 nUniqueResourceID);

	IBaseMaterialGroup * GetBaseMaterialGroupByID(const Lib3MF_uint32 nUniqueResourceID);

	IMeshObject * GetMeshObjectByID(const Lib3MF_uint32 nUniqueResourceID);

	IComponentsObject * GetComponentsObjectByID(const Lib3MF_uint32 nUniqueResourceID);

	IColorGroup * GetColorGroupByID(const Lib3MF_uint32 nUniqueResourceID);

	ITexture2DGroup * GetTexture2DGroupByID(const Lib3MF_uint32 nUniqueResourceID);

	ICompositeMaterials * GetCompositeMaterialsByID(const Lib3MF_uint32 nUniqueResourceID);

	IMultiPropertyGroup * GetMultiPropertyGroupByID(const Lib3MF_uint32 nUniqueResourceID);

	ISliceStack * GetSliceStackByID(const Lib3MF_uint32 nUniqueResourceID);

	std::string GetBuildUUID (bool & bHasUUID);

	void SetBuildUUID (const std::string & sUUID);

	IBuildItemIterator * GetBuildItems ();

	IResourceIterator * GetResources ();

	IObjectIterator * GetObjects ();

	IMeshObjectIterator * GetMeshObjects ();

	IComponentsObjectIterator * GetComponentsObjects ();

	ITexture2DIterator * GetTexture2Ds ();

	IBaseMaterialGroupIterator * GetBaseMaterialGroups ();

	IColorGroupIterator * GetColorGroups();

	ITexture2DGroupIterator * GetTexture2DGroups();

	ICompositeMaterialsIterator * GetCompositeMaterials();

	IMultiPropertyGroupIterator * GetMultiPropertyGroups();

	ISliceStackIterator * GetSliceStacks();

	IModel * MergeToModel ();

	IMeshObject * AddMeshObject ();

	IComponentsObject * AddComponentsObject ();

	ISliceStack * AddSliceStack(const Lib3MF_double dZBottom);

	ITexture2D * AddTexture2DFromAttachment (IAttachment* pTextureAttachment);

	IBaseMaterialGroup * AddBaseMaterialGroup ();

	IColorGroup * AddColorGroup();

	ITexture2DGroup * AddTexture2DGroup(ITexture2D* pTexture2DInstance);

	ICompositeMaterials * AddCompositeMaterials(IBaseMaterialGroup* pBaseMaterialGroupInstance);

	IMultiPropertyGroup * AddMultiPropertyGroup();

	IBuildItem * AddBuildItem (IObject* pObject, const sLib3MFTransform Transform);

	void RemoveBuildItem (IBuildItem* pBuildItemInstance);

	IMetaDataGroup * GetMetaDataGroup ();

	IAttachment * AddAttachment (const std::string & sURI, const std::string & sRelationShipType);

	void RemoveAttachment(IAttachment* pAttachmentInstance);

	IAttachment * GetAttachment (const Lib3MF_uint32 nIndex);

	IAttachment * FindAttachment (const std::string & sURI);

	Lib3MF_uint32 GetAttachmentCount ();

	bool HasPackageThumbnailAttachment ();

	IAttachment * CreatePackageThumbnailAttachment ();

	IAttachment * GetPackageThumbnailAttachment ();

	void RemovePackageThumbnailAttachment ();

	void AddCustomContentType (const std::string & sExtension, const std::string & sContentType);

	void RemoveCustomContentType (const std::string & sExtension);

	Lib3MF::sBox GetOutbox();

	IKeyStore * GetKeyStore() override;

};

}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // __LIB3MF_MODEL
