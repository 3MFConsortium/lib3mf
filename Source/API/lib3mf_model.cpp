/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CModel 
**************************************************************************************************************************/

IPackagePart * CModel::RootModelPart()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IPackagePart * CModel::FindOrCreatePackagePart(const std::string & sAbsolutePath)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::SetUnit(const Lib3MF::eModelUnit eUnit)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF::eModelUnit CModel::GetUnit()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CModel::GetLanguage()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::SetLanguage(const std::string & sLanguage)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IWriter * CModel::QueryWriter(const std::string & sWriterClass)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IReader * CModel::QueryReader(const std::string & sReaderClass)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IResource * CModel::GetResourceByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ITexture2D * CModel::GetTexture2DByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF::ePropertyType CModel::GetPropertyTypeByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IBaseMaterialGroup * CModel::GetBaseMaterialGroupByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ITexture2DGroup * CModel::GetTexture2DGroupByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ICompositeMaterials * CModel::GetCompositeMaterialsByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMultiPropertyGroup * CModel::GetMultiPropertyGroupByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMeshObject * CModel::GetMeshObjectByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IComponentsObject * CModel::GetComponentsObjectByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IColorGroup * CModel::GetColorGroupByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ISliceStack * CModel::GetSliceStackByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILevelSet * CModel::GetLevelSetByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CModel::GetBuildUUID(bool & bHasUUID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::SetBuildUUID(const std::string & sUUID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IBuildItemIterator * CModel::GetBuildItems()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF::sBox CModel::GetOutbox()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IResourceIterator * CModel::GetResources()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IObjectIterator * CModel::GetObjects()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMeshObjectIterator * CModel::GetMeshObjects()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IComponentsObjectIterator * CModel::GetComponentsObjects()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ITexture2DIterator * CModel::GetTexture2Ds()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IBaseMaterialGroupIterator * CModel::GetBaseMaterialGroups()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IColorGroupIterator * CModel::GetColorGroups()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ITexture2DGroupIterator * CModel::GetTexture2DGroups()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ICompositeMaterialsIterator * CModel::GetCompositeMaterials()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMultiPropertyGroupIterator * CModel::GetMultiPropertyGroups()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ISliceStackIterator * CModel::GetSliceStacks()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IImage3DIterator * CModel::GetImage3Ds()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IModel * CModel::MergeToModel()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::MergeFromModel(IModel* pModelInstance)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMeshObject * CModel::AddMeshObject()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IComponentsObject * CModel::AddComponentsObject()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ISliceStack * CModel::AddSliceStack(const Lib3MF_double dZBottom)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ITexture2D * CModel::AddTexture2DFromAttachment(IAttachment* pTextureAttachment)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IBaseMaterialGroup * CModel::AddBaseMaterialGroup()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IColorGroup * CModel::AddColorGroup()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ITexture2DGroup * CModel::AddTexture2DGroup(ITexture2D* pTexture2DInstance)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ICompositeMaterials * CModel::AddCompositeMaterials(IBaseMaterialGroup* pBaseMaterialGroupInstance)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMultiPropertyGroup * CModel::AddMultiPropertyGroup()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IImageStack * CModel::AddImageStack(const Lib3MF_uint32 nColumnCount, const Lib3MF_uint32 nRowCount, const Lib3MF_uint32 nSheetCount)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IImageStack * CModel::GetImageStackByID(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IBuildItem * CModel::AddBuildItem(IObject* pObject, const Lib3MF::sTransform Transform)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::RemoveBuildItem(IBuildItem* pBuildItemInstance)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMetaDataGroup * CModel::GetMetaDataGroup()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IAttachment * CModel::AddAttachment(const std::string & sURI, const std::string & sRelationShipType)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::RemoveAttachment(IAttachment* pAttachmentInstance)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IAttachment * CModel::GetAttachment(const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IAttachment * CModel::FindAttachment(const std::string & sURI)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CModel::GetAttachmentCount()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CModel::HasPackageThumbnailAttachment()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IAttachment * CModel::CreatePackageThumbnailAttachment()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IAttachment * CModel::GetPackageThumbnailAttachment()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::RemovePackageThumbnailAttachment()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::AddCustomContentType(const std::string & sExtension, const std::string & sContentType)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::RemoveCustomContentType(const std::string & sExtension)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::SetRandomNumberCallback(const Lib3MF::RandomNumberCallback pTheCallback, const Lib3MF_pvoid pUserData)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IKeyStore * CModel::GetKeyStore()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IFunctionIterator * CModel::GetFunctions()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IImplicitFunction * CModel::AddImplicitFunction()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IFunctionFromImage3D * CModel::AddFunctionFromImage3D(IImage3D* pImage3DInstance)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IVolumeData * CModel::AddVolumeData()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILevelSet * CModel::AddLevelSet()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILevelSetIterator * CModel::GetLevelSets()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CModel::RemoveResource(IResource* pResource)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

