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

Abstract: This is the class declaration of CLib3MFModel

*/


#ifndef __LIB3MF_LIB3MFMODEL
#define __LIB3MF_LIB3MFMODEL

#include "lib3mf_interfaces.hpp"


// Include custom headers here.


namespace Lib3MF {


/*************************************************************************************************************************
 Class declaration of CLib3MFModel 
**************************************************************************************************************************/

class CLib3MFModel : public virtual ILib3MFModel {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/


	/**
	* Public member functions to implement.
	*/

	void SetUnit (const eLib3MFModelUnit eUnit);

	eLib3MFModelUnit GetUnit ();

	std::string GetLanguage ();

	void SetLanguage (const std::string & sLanguage);

	ILib3MFWriter * QueryWriter (const std::string & sWriterClass);

	ILib3MFReader * QueryReader (const std::string & sReaderClass);

	ILib3MFResource * GetResourceByID (const unsigned int nResourceID);

	ILib3MFTexture2D * GetTexture2DByID (const unsigned int nResourceID);

	ILib3MFBaseMaterial * GetBaseMaterialByID (const unsigned int nResourceID);

	ILib3MFMeshObject * GetMeshObjectByID (const unsigned int nResourceID);

	ILib3MFComponentsObject * GetComponentsObjectByID (const unsigned int nResourceID);

	std::string GetBuildUUID (bool & bHasUUID);

	void SetBuildUUID (const std::string & sUUID);

	ILib3MFBuildItemIterator * GetBuildItems ();

	ILib3MFResourceIterator * GetResources ();

	ILib3MFResourceIterator * GetObjects ();

	ILib3MFResourceIterator * GetMeshObjects ();

	ILib3MFResourceIterator * GetComponentsObjects ();

	ILib3MFResourceIterator * Get2DTextures ();

	ILib3MFResourceIterator * GetBaseMaterials ();

	ILib3MFModel * MergeToModel ();

	ILib3MFMeshObject * AddMeshObject ();

	ILib3MFComponentsObject * AddComponentsObject ();

	ILib3MFTexture2D * AddTexture2DFromAttachment (ILib3MFAttachment* pTextureAttachment);

	ILib3MFBaseMaterial * AddBaseMaterialGroup ();

	ILib3MFBuildItem * AddBuildItem (ILib3MFObject* pObject, const sLib3MFTransform Transform);

	void RemoveBuildItem (ILib3MFBuildItem* pBuildItemInstance);

	ILib3MFMetaDataGroup * GetMetaDataGroup ();

	ILib3MFAttachment * AddAttachment (const std::string & sURI, const std::string & sRelationShipType);

	ILib3MFAttachment * GetAttachment (const unsigned int nIndex);

	ILib3MFAttachment * FindAttachment (const std::string & sURI);

	unsigned int GetAttachmentCount ();

	unsigned long long GetAttachmentSize (const unsigned int nIndex);

	std::string GetAttachmentPath (const unsigned int nIndex);

	bool HasPackageThumbnailAttachment ();

	ILib3MFAttachment * CreatePackageThumbnailAttachment ();

	ILib3MFAttachment * GetPackageThumbnailAttachment ();

	void RemovePackageThumbnailAttachment ();

	void AddCustomContentType (const std::string & sExtension, const std::string & sContentType);

	void RemoveCustomContentType (const std::string & sExtension);

};

}

#endif // __LIB3MF_LIB3MFMODEL
