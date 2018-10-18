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

Abstract: This is the class declaration of CInternalLib3MFModel
Interface version: 1.2.2

*/


#ifndef __LIB3MF_LIB3MFMODEL
#define __LIB3MF_LIB3MFMODEL

#include "lib3mf_interfaces.hpp"


// Include custom headers here.
#include "Model/Classes/NMR_Model.h" 

namespace Lib3MF {


/*************************************************************************************************************************
 Class declaration of CInternalLib3MFModel 
**************************************************************************************************************************/

class CInternalLib3MFModel : public virtual IInternalLib3MFModel {
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

	CInternalLib3MFModel();

	NMR::CModel& model();

	/**
	* Public member functions to implement.
	*/

	void SetUnit (const eLib3MFModelUnit eUnit);

	eLib3MFModelUnit GetUnit ();

	std::string GetLanguage ();

	void SetLanguage (const std::string sLanguage);

	IInternalLib3MFWriter * QueryWriter (const std::string sWriterClass);

	IInternalLib3MFReader * QueryReader (const std::string sReaderClass);

	IInternalLib3MFResource * GetResourceByID (const unsigned int nResourceID);

	IInternalLib3MFTexture2D * GetTexture2DByID (const unsigned int nResourceID);

	IInternalLib3MFBaseMaterial * GetBaseMaterialByID (const unsigned int nResourceID);

	IInternalLib3MFMeshObject * GetMeshObjectByID (const unsigned int nResourceID);

	IInternalLib3MFComponentsObject * GetComponentsObjectByID (const unsigned int nResourceID);

	std::string GetBuildUUID (bool & bHasUUID);

	void SetBuildUUID (const std::string sUUID);

	IInternalLib3MFBuildItemIterator * GetBuildItems ();

	IInternalLib3MFResourceIterator * GetResources ();

	IInternalLib3MFResourceIterator * GetObjects ();

	IInternalLib3MFResourceIterator * GetMeshObjects ();

	IInternalLib3MFResourceIterator * GetComponentsObjects ();

	IInternalLib3MFResourceIterator * Get2DTextures ();

	IInternalLib3MFResourceIterator * GetBaseMaterials ();

	IInternalLib3MFModel * MergeToModel ();

	IInternalLib3MFMeshObject * AddMeshObject ();

	IInternalLib3MFComponentsObject * AddComponentsObject ();

	IInternalLib3MFTexture2D * AddTexture2DFromAttachment (IInternalLib3MFAttachment& pTextureAttachment);

	IInternalLib3MFBaseMaterial * AddBaseMaterialGroup ();

	IInternalLib3MFBuildItem * AddBuildItem (IInternalLib3MFObject& pObject, const sLib3MFTransform Transform);

	void RemoveBuildItem (IInternalLib3MFBuildItem& pBuildItemInstance);

	IInternalLib3MFMetaDataGroup * GetMetaDataGroup ();

	IInternalLib3MFAttachment * AddAttachment (const std::string sURI, const std::string sRelationShipType);

	IInternalLib3MFAttachment * GetAttachment (const unsigned int nIndex);

	IInternalLib3MFAttachment * FindAttachment (const std::string sURI);

	unsigned int GetAttachmentCount ();

	unsigned long long GetAttachmentSize (const unsigned int nIndex);

	std::string GetAttachmentPath (const unsigned int nIndex);

	bool HasPackageThumbnailAttachment ();

	IInternalLib3MFAttachment * CreatePackageThumbnailAttachment ();

	IInternalLib3MFAttachment * GetPackageThumbnailAttachment ();

	void RemovePackageThumbnailAttachment ();

	void AddCustomContentType (const std::string sExtension, const std::string sContentType);

	void RemoveCustomContentType (const std::string sExtension);

};

}

#endif // __LIB3MF_LIB3MFMODEL
