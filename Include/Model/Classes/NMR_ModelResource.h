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

NMR_ModelResource.h defines the Model Resource Class.
A model resource is an in memory representation of the 3MF
resource object.

--*/

#ifndef __NMR_MODELRESOURCE
#define __NMR_MODELRESOURCE

#include "Model/Classes/NMR_ModelMetaData.h" 
#include "Common/NMR_Types.h" 
#include "Model/Classes/NMR_ModelTypes.h" 
#include "Model/Classes/NMR_Model.h" 

#include <string>

namespace NMR {

	class CModelResource {
	private:
		CModel * m_pModel;
		PPackageResourceID m_pPackageResourceID;

	protected:
		std::vector<ModelPropertyID> m_ResourceIndexMap;
		nfBool m_bHasResourceIndexMap;
		CModel * Model();
		
	public:
		CModelResource() = delete;
		// CModelResource(_In_ const PPackageResourceID sResourceID, _In_ CModel * pModel);
		CModelResource(_In_ const ModelResourceID sResourceID, _In_ CModel * pModel);
		virtual ~CModelResource();
		
		virtual PPackageResourceID getPackageResourceID();
		void setPackageResourceID(PPackageResourceID pID);

		bool mapResourceIndexToPropertyID (_In_ ModelResourceIndex nPropertyIndex, _Out_ ModelPropertyID & nPropertyID);
		void clearResourceIndexMap();
		virtual void buildResourceIndexMap();
		nfBool hasResourceIndexMap();

		_Ret_notnull_ CModel * getModel();
	};

	typedef std::shared_ptr <CModelResource> PModelResource;

}

#endif // __NMR_MODELRESOURCE
