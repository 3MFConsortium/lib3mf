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

	class CModel {
	private:
		// Object Resources of the model
		std::map<ModelResourceID, PModelResource> m_ResourceMap;
		std::vector<PModelResource> m_Resources;

		// Model Build Items
		std::vector<PModelBuildItem> m_BuildItems;

		// Model Properties
		eModelUnit m_Unit;
		std::wstring m_sLanguage;
		PModelThumbnail m_pGlobalThumbnail;

		// Model Metadata
		std::vector<PModelMetaData> m_MetaData;
		std::map<std::wstring, PModelMetaData> m_MetaDataMap;

		// Thumbnail Container
		std::vector<PModelThumbnail> m_Thumbnails;

		// Indexed lookup lists for standard resource types
		std::vector<PModelResource> m_ObjectLookup;

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

		// Metadata setter/getter
		void addMetaData(_In_ std::wstring sName, _In_ std::wstring sValue);
		nfUint32 getMetaDataCount();
		void getMetaData(_In_ nfUint32 nIndex, _Out_ std::wstring & sName, _Out_ std::wstring & sValue);
		void removeMetaData(_In_ nfUint32 nIndex);
		nfBool hasMetaData(_In_ std::wstring sName);

		// Retrieve a unique Resource ID
		ModelResourceID generateResourceID();

		// Convenience functions for objects
		_Ret_maybenull_ CModelObject * findObject(_In_ ModelResourceID nResourceID);
		nfUint32 getObjectCount();
		PModelResource getObjectResource(_In_ nfUint32 nIndex);
		CModelObject * getObject(_In_ nfUint32 nIndex);

		// Clear all build items and Resources
		void clearAll ();
	};

	typedef std::shared_ptr <CModel> PModel;

}

#endif // __NMR_MODEL
