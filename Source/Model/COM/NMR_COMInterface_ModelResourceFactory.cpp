/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

COM Interface Implementation for Model Resource Factory Class

--*/

#include "Model/COM/NMR_COMInterface_ModelResourceFactory.h" 
#include "Model/COM/NMR_COMInterface_ModelMeshObject.h" 
#include "Model/COM/NMR_COMInterface_ModelComponentsObject.h" 

#include "Common/NMR_Exception_Windows.h" 

#include "Model/Classes/NMR_Model.h" 
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Model/Classes/NMR_ModelComponentsObject.h" 

namespace NMR {

	void fnCreateModelObjectResourceFromClass(_In_ PModelResource pResource, _In_ nfBool bFailIfUnkownClass, _Outptr_ ILib3MFModelObjectResource ** pCOMResource)
	{
		__NMRASSERT(pCOMResource);
		if (!pResource.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		ILib3MFModelObjectResource * pResult = nullptr;

		CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject *> (pResource.get());
		if (pMeshObject) {
			CCOMObject <CCOMModelMeshObject> * pComObject = new CCOMObject <CCOMModelMeshObject>();
			pComObject->setResource(pResource);
			pResult = pComObject;
		}

		CModelComponentsObject * pComponentResource = dynamic_cast<CModelComponentsObject *> (pResource.get());
		if (pComponentResource) {
			CCOMObject <CCOMModelComponentsObject> * pComObject = new CCOMObject <CCOMModelComponentsObject>();
			pComObject->setResource(pResource);
			pResult = pComObject;
		}

		if (bFailIfUnkownClass && (pResult == nullptr))
			throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);

		*pCOMResource = pResult;
	}

	void fnCreateModelResourceFromClass(_In_ PModelResource pResource, _In_ nfBool bFailIfUnkownClass, _Outptr_ ILib3MFModelResource ** pCOMResource)
	{
		__NMRASSERT(pCOMResource);
		if (!pResource.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		ILib3MFModelResource * pResult = nullptr;

		// First try Object Resource types
		ILib3MFModelObjectResource * pObjectResource = nullptr;
		fnCreateModelObjectResourceFromClass(pResource, false, &pObjectResource);
		if (pObjectResource)
			pResult = pObjectResource;

		if (bFailIfUnkownClass && (pResult == nullptr))
			throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);

		*pCOMResource = pResult;
	}

}
