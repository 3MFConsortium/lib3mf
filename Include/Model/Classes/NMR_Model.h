/*++

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

NMR_Model.h defines the Model Class. A model is an in memory representation of the 3MF file.

--*/

#ifndef __NMR_MODEL
#define __NMR_MODEL

#include "Model/Classes/NMR_ModelTypes.h"
#include "Common/Platform/NMR_ImportStream.h"

#include "Common/Math/NMR_Matrix.h" 
#include "Common/Mesh/NMR_Mesh.h" 

#include <list>
#include <map>
#include <set>
#include <vector>

namespace NMR {

	class CModelBuildItem;
	typedef std::shared_ptr <CModelBuildItem> PModelBuildItem;

	class CModelResource;
	typedef std::shared_ptr <CModelResource> PModelResource;

	class CModelObject;
	typedef std::shared_ptr <CModelObject> PModelObject;

	class CModelMetaData;
	typedef std::shared_ptr <CModelMetaData> PModelMetaData;

	class CModelThumbnail;
	typedef std::shared_ptr <CModelThumbnail> PModelThumbnail;

	class CModelAttachment;
	typedef std::shared_ptr <CModelAttachment> PModelAttachment;

	class CModelBaseMaterialResource;
	typedef std::shared_ptr <CModelBaseMaterialResource> PModelBaseMaterialResource;

	class CModelTexture2DResource;
	typedef std::shared_ptr <CModelTexture2DResource> PModelTexture2DResource;

	class CModel {
	private:
		// Object Resources of the model
		std::map<ModelResourceID, PModelResource> m_ResourceMap;
		std::vector<PModelResource> m_Resources;

		// Model Build Items
		std::vector<PModelBuildItem> m_BuildItems;

		// Model Properties
		nfUint32 m_nHandleCounter;
		eModelUnit m_Unit;
		std::wstring m_sLanguage;
		PModelThumbnail m_pGlobalThumbnail;

		// Model Metadata
		std::vector<PModelMetaData> m_MetaData;
		std::map<std::wstring, PModelMetaData> m_MetaDataMap;

		// Thumbnail Container
		std::vector<PModelThumbnail> m_Thumbnails;

		// Texture Streams
		std::map<std::wstring, PImportStream> m_TextureStreamMap;
		std::vector<std::pair <std::wstring, PImportStream>> m_TextureStreams;

		// Model Attachments
		std::vector<PModelAttachment> m_Attachments;
		std::map<std::wstring, PModelAttachment> m_AttachmentURIMap;

		// Custom Attachment Content Types
		std::map<std::wstring, std::wstring> m_CustomContentTypes;

		// Indexed lookup lists for standard resource types
		std::vector<PModelResource> m_ObjectLookup;
		std::vector<PModelResource> m_BaseMaterialLookup;
		std::vector<PModelResource> m_TextureLookup;

		// Add Resource to resource lookup tables
		void addResourceToLookupTable(_In_ PModelResource pResource);

	public:
		CModel();
		~CModel();

		// Merge all build items into one mesh
		void mergeToMesh(_In_ CMesh * pMesh);

		// Units setter/getter
		void setUnit(_In_ eModelUnit Unit);
		void setUnitString(_In_ std::wstring sUnitString);
		eModelUnit getUnit();
		std::wstring getUnitString();

		// Language setter/getter
		void setLanguage(_In_ std::wstring sLanguage);
		std::wstring getLanguage();

		// General Resource Handling
		PModelResource findResource(_In_ ModelResourceID nID);
		nfUint32 getResourceCount();
		PModelResource getResource(_In_ nfUint32 nIndex);
		void addResource(_In_ PModelResource pResource);

		// Build Handling
		void addBuildItem(_In_ PModelBuildItem pBuildItem);
		nfUint32 getBuildItemCount();
		PModelBuildItem getBuildItem(_In_ nfUint32 nIdx);
		// Removes a build item identified by its handle
		void removeBuildItem(_In_ nfUint32 nHandle, _In_ nfBool bThrowExceptionIfNotFound);

		// Metadata setter/getter
		void addMetaData(_In_ std::wstring sName, _In_ std::wstring sValue);
		nfUint32 getMetaDataCount();
		void getMetaData(_In_ nfUint32 nIndex, _Out_ std::wstring & sName, _Out_ std::wstring & sValue);
		void removeMetaData(_In_ nfUint32 nIndex);
		nfBool hasMetaData(_In_ std::wstring sName);
		void mergeMetaData(_In_ CModel * pSourceModel);

		// Retrieve a unique Resource ID
		ModelResourceID generateResourceID();

		// Convenience functions for objects
		_Ret_maybenull_ CModelObject * findObject(_In_ ModelResourceID nResourceID);
		nfUint32 getObjectCount();
		PModelResource getObjectResource(_In_ nfUint32 nIndex);
		CModelObject * getObject(_In_ nfUint32 nIndex);

		// Convenience functions for base materials
		_Ret_maybenull_ CModelBaseMaterialResource * findBaseMaterial(_In_ ModelResourceID nResourceID);
		nfUint32 getBaseMaterialCount();
		PModelResource getBaseMaterialResource(_In_ nfUint32 nIndex);
		CModelBaseMaterialResource * getBaseMaterial(_In_ nfUint32 nIndex);
		void mergeBaseMaterials(_In_ CModel * pSourceModel);

		// Convenience functions for 2D Textures
		_Ret_maybenull_ CModelTexture2DResource * findTexture2D(_In_ ModelResourceID nResourceID);
		nfUint32 getTexture2DCount();
		PModelResource getTexture2DResource(_In_ nfUint32 nIndex);
		CModelTexture2DResource * getTexture2D(_In_ nfUint32 nIndex);
		void mergeTextures2D(_In_ CModel * pSourceModel);

		// Clear all build items and Resources
		void clearAll ();

		// Creates a unique handle for identifying child classes (e.g. build items)
		nfUint32 createHandle();

		// Calculate Texture Streams
		void addTextureStream(_In_ std::wstring sPath, _In_ PImportStream pStream);
		void removeTextureStream(_In_ std::wstring sPath);
		nfUint32 getTextureStreamCount();
		PImportStream getTextureStream(_In_ nfUint32 nIndex);
		std::wstring getTextureStreamPath(_In_ nfUint32 nIndex);
		PImportStream findTextureStream(_In_ std::wstring sPath);
		void mergeTextureStreams(_In_ CModel * pSourceModel);

		// Custom Attachments
		PModelAttachment addAttachment(_In_ const std::wstring sPath, _In_ const std::wstring sRelationShipType, PImportStream pCopiedStream);
		void removeAttachment(_In_ const std::wstring sPath);
		nfUint32 getAttachmentCount();
		PModelAttachment getModelAttachment(_In_ nfUint32 nIndex);
		std::wstring getModelAttachmentPath(_In_ nfUint32 nIndex);
		PModelAttachment findModelAttachment(_In_ std::wstring sPath);

		// Custom Content Types
		std::map<std::wstring, std::wstring> getCustomContentTypes();
		void addCustomContentType(_In_ const std::wstring sExtension, _In_ const std::wstring sContentType);
		void removeCustomContentType(_In_ const std::wstring sExtension);
		nfBool contentTypeIsDefault(_In_ const std::wstring sExtension);


	};

	typedef std::shared_ptr <CModel> PModel;

}

#endif // __NMR_MODEL
