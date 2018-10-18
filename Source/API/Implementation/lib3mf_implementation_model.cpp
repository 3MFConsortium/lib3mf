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

Abstract: This is a stub class definition of CInternalLib3MFModel
Interface version: 1.2.2

*/

#include "lib3mf_implementation_model.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_implementation_meshobject.hpp"
#include "lib3mf_implementation_writer.hpp"
#include "lib3mf_implementation_builditem.hpp"

#include "Model/Writer/NMR_ModelWriter.h"
#include "Model/Writer/NMR_ModelWriter_3MF_Native.h" 
#include "Model/Writer/NMR_ModelWriter_STL.h"

using namespace Lib3MF;

/*************************************************************************************************************************
 Class definition of CInternalLib3MFModel 
**************************************************************************************************************************/

CInternalLib3MFModel::CInternalLib3MFModel()
{
	m_model = std::make_shared<NMR::CModel>();
};

NMR::CModel& CInternalLib3MFModel::model()
{
	return *m_model;
};


void CInternalLib3MFModel::SetUnit (const eLib3MFModelUnit eUnit)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

eLib3MFModelUnit CInternalLib3MFModel::GetUnit ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CInternalLib3MFModel::GetLanguage ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFModel::SetLanguage (const std::string sLanguage)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFWriter * CInternalLib3MFModel::QueryWriter (const std::string sWriterClass)
{
	NMR::PModelWriter pWriter = nullptr;

	// Create specified writer instance
	if (sWriterClass.compare("3mf") == 0) {
		pWriter = std::make_shared<NMR::CModelWriter_3MF_Native>(std::make_shared<NMR::CModel>(model()));
	} else if (sWriterClass.compare("stl") == 0) {
		pWriter = std::make_shared<NMR::CModelWriter_STL>(std::make_shared<NMR::CModel>(model()));
	}

	if (!pWriter)
		throw ELib3MFInterfaceException(NMR_ERROR_UNKNOWNWRITERCLASS);

	return new CInternalLib3MFWriter(pWriter);
}

IInternalLib3MFReader * CInternalLib3MFModel::QueryReader (const std::string sReaderClass)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFResource * CInternalLib3MFModel::GetResourceByID (const unsigned int nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFTexture2D * CInternalLib3MFModel::GetTexture2DByID (const unsigned int nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFBaseMaterial * CInternalLib3MFModel::GetBaseMaterialByID (const unsigned int nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFMeshObject * CInternalLib3MFModel::GetMeshObjectByID (const unsigned int nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFComponentsObject * CInternalLib3MFModel::GetComponentsObjectByID (const unsigned int nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CInternalLib3MFModel::GetBuildUUID (bool & bHasUUID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFModel::SetBuildUUID (const std::string sUUID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFBuildItemIterator * CInternalLib3MFModel::GetBuildItems ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFResourceIterator * CInternalLib3MFModel::GetResources ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFResourceIterator * CInternalLib3MFModel::GetObjects ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFResourceIterator * CInternalLib3MFModel::GetMeshObjects ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFResourceIterator * CInternalLib3MFModel::GetComponentsObjects ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFResourceIterator * CInternalLib3MFModel::Get2DTextures ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFResourceIterator * CInternalLib3MFModel::GetBaseMaterials ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFModel * CInternalLib3MFModel::MergeToModel ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFMeshObject * CInternalLib3MFModel::AddMeshObject ()
{
	return new CInternalLib3MFMeshObject(*this);
}

IInternalLib3MFComponentsObject * CInternalLib3MFModel::AddComponentsObject ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFTexture2D * CInternalLib3MFModel::AddTexture2DFromAttachment (IInternalLib3MFAttachment& pTextureAttachment)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFBaseMaterial * CInternalLib3MFModel::AddBaseMaterialGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFBuildItem * CInternalLib3MFModel::AddBuildItem (IInternalLib3MFObject& pObject, const sLib3MFTransform Transform)
{
	NMR::CModelObject *pModelObject = model().findObject(pObject.GetResourceID());
	if (!pModelObject)
		throw ELib3MFInterfaceException(NMR_ERROR_RESOURCENOTFOUND);

	NMR::PModelBuildItem pModelBuildItem = std::make_shared<NMR::CModelBuildItem>(pModelObject, model().createHandle() );
	model().addBuildItem(pModelBuildItem);

	return new CInternalLib3MFBuildItem(pModelBuildItem);
}

void CInternalLib3MFModel::RemoveBuildItem (IInternalLib3MFBuildItem& pBuildItemInstance)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFMetaDataGroup * CInternalLib3MFModel::GetMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFAttachment * CInternalLib3MFModel::AddAttachment (const std::string sURI, const std::string sRelationShipType)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFAttachment * CInternalLib3MFModel::GetAttachment (const unsigned int nIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFAttachment * CInternalLib3MFModel::FindAttachment (const std::string sURI)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

unsigned int CInternalLib3MFModel::GetAttachmentCount ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

unsigned long long CInternalLib3MFModel::GetAttachmentSize (const unsigned int nIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CInternalLib3MFModel::GetAttachmentPath (const unsigned int nIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CInternalLib3MFModel::HasPackageThumbnailAttachment ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFAttachment * CInternalLib3MFModel::CreatePackageThumbnailAttachment ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFAttachment * CInternalLib3MFModel::GetPackageThumbnailAttachment ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFModel::RemovePackageThumbnailAttachment ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFModel::AddCustomContentType (const std::string sExtension, const std::string sContentType)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFModel::RemoveCustomContentType (const std::string sExtension)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

