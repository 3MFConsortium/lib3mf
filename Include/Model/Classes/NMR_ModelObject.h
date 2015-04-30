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

NMR_ModelObject.h defines the Model Object Class.
A model object is an in memory representation of the 3MF
model object.

--*/

#ifndef __NMR_MODELOBJECT
#define __NMR_MODELOBJECT

#include "Model/Classes/NMR_Model.h" 
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_ModelMetaData.h" 
#include "Model/Classes/NMR_ModelThumbnail.h" 
#include "Common/NMR_Types.h" 
#include "Common/Math/NMR_Matrix.h" 

#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelObject : public CModelResource {
	private:
		std::wstring m_sName;
		std::wstring m_sPartNumber;
		PModelThumbnail m_pThumbnail;
		eModelObjectType m_ObjectType;
		
	public:
		CModelObject() = delete;
		CModelObject(_In_ const ModelResourceID sID, _In_ CModel * pModel);

		// setter/getter for name
		// the name may be an arbitrary string
		std::wstring getName();
		void setName(_In_ std::wstring sName);

		// setter/getter for part number
		// the part number may be an arbitrary string
		std::wstring getPartNumber();
		void setPartNumber(_In_ std::wstring sPartNumber);

		// setter/getter for the object type
		eModelObjectType getObjectType();
		void setObjectType(_In_ eModelObjectType ObjectType);
		std::wstring getObjectTypeString();
		nfBool setObjectTypeString(_In_ std::wstring sTypeString, _In_ nfBool bRaiseException);

		// Merge the object into a mesh object
		virtual void mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix);
		void mergeToMesh(_In_ CMesh * pMesh);
	};

	typedef std::shared_ptr <CModelObject> PModelObject;
}

#endif // __NMR_MODELOBJECT
