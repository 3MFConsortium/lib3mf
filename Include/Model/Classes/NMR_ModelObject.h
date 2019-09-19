/*++

Copyright (C) 2019 3MF Consortium

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

NMR_ModelObject.h defines the Model Object Class.
A model object is an in memory representation of the 3MF
model object.

--*/

#ifndef __NMR_MODELOBJECT
#define __NMR_MODELOBJECT

#include "Model/Classes/NMR_Model.h" 
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_ModelAttachment.h"
#include "Model/Classes/NMR_ModelMetaDataGroup.h" 
#include "Model/Classes/NMR_ModelSliceStack.h"
#include "Common/NMR_Types.h" 
#include "Common/Math/NMR_Matrix.h" 

#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelObject : public CModelResource { 
	private:
		std::string m_sName;
		std::string m_sPartNumber;
		PModelSliceStack m_pSliceStack;
		eModelSlicesMeshResolution m_eSlicesMeshResolution;
		PModelMetaDataGroup m_MetaDataGroup;

		nfUint32 m_nComponentDepthLevel;

	private:
		PUUID m_UUID;
		PModelAttachment m_pThumbnailAttachment;
		eModelObjectType m_ObjectType;
	public:
		CModelObject() = delete;
		CModelObject(_In_ const ModelResourceID sID, _In_ CModel * pModel);

		// setter/getter for name
		// the name may be an arbitrary string
		std::string getName();
		void setName(_In_ std::string sName);

		// setter/getter for part number
		// the part number may be an arbitrary string
		std::string getPartNumber();
		void setPartNumber(_In_ std::string sPartNumber);

		// Production Extension
		PUUID uuid();
		void setUUID(PUUID uuid);

		// MetaDataGroup
		PModelMetaDataGroup metaDataGroup();

		// setter/getter for the object type
		eModelObjectType getObjectType();
		virtual void setObjectType(_In_ eModelObjectType ObjectType);
		std::string getObjectTypeString();
		nfBool setObjectTypeString(_In_ std::string sTypeString, _In_ nfBool bRaiseException);

		// Merge the object into a mesh object
		virtual void mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix);
		void mergeToMesh(_In_ CMesh * pMesh);

		// check, if the object is a valid object description
		virtual nfBool isValid() = 0;

		virtual nfBool hasSlices(nfBool bRecursive) = 0;
		virtual nfBool isValidForSlices(const NMATRIX3& totalParentMatrix) = 0;

		void assignSliceStack(PModelSliceStack pSliceStackId);
		PModelSliceStack getSliceStack();

		void setSlicesMeshResolution(eModelSlicesMeshResolution eMeshResolution);
		eModelSlicesMeshResolution slicesMeshResolution() const;

		// Set/Get Thumbnail
		void clearThumbnailAttachment();
		void setThumbnailAttachment(_In_ PModelAttachment pThumbnailAttachment, bool bThrowIfIncorrect);
		PModelAttachment getThumbnailAttachment();

		// Calculates the component Depths
		nfUint32 getComponentDepthLevel ();
		void clearComponentDepthLevel();
		virtual void calculateComponentDepthLevel (nfUint32 nLevel);

		virtual void calculateOutbox(_Out_ NOUTBOX3& vOutBox, _In_ const NMATRIX3 mAccumulatedMatrix) = 0;

	};

	typedef std::shared_ptr <CModelObject> PModelObject;
}

#endif // __NMR_MODELOBJECT
