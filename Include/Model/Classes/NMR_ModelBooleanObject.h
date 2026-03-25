/*++

Copyright (C) 2026 3MF Consortium

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

Minimal in-memory representation of a boolean object.

--*/

#ifndef __NMR_MODELBOOLEANOBJECT
#define __NMR_MODELBOOLEANOBJECT

#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelComponent.h"

namespace NMR {

	enum class eModelBooleanOperation {
		Union = 0,
		Difference = 1,
		Intersection = 2
	};

	class CModelBooleanObject : public CModelObject {
	private:
		PModelComponent m_pBaseObject;
		std::vector<PModelComponent> m_Operands;
		eModelBooleanOperation m_eOperation;

	public:
		CModelBooleanObject() = delete;
		CModelBooleanObject(_In_ const ModelResourceID sID, _In_ CModel * pModel);
		~CModelBooleanObject() override;

		void setBaseObject(_In_ CModelObject * pObject, _In_ const NMATRIX3 & mTransform);
		CModelObject * getBaseObject() const;
		void setBaseTransform(_In_ const NMATRIX3 & mTransform);
		NMATRIX3 getBaseTransform() const;

		void setOperation(_In_ eModelBooleanOperation eOperation);
		eModelBooleanOperation getOperation() const;
		std::string getOperationString() const;
		nfBool setOperationString(_In_ const std::string & sOperation, _In_ nfBool bRaiseException);

		void addOperand(_In_ CModelObject * pObject, _In_ const NMATRIX3 & mTransform);
		nfUint32 getOperandCount() const;
		PModelComponent getOperand(_In_ nfUint32 nIdx) const;

		void mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix) override;
		nfBool isValid() override;
		nfBool hasSlices(nfBool bRecursive) override;
		nfBool isValidForSlices(const NMATRIX3& totalParentMatrix) override;
		void extendOutbox(_Out_ NOUTBOX3& vOutBox, _In_ const NMATRIX3 mAccumulatedMatrix) override;
		ResourceDependencies getDependencies() override;
	};

	typedef std::shared_ptr<CModelBooleanObject> PModelBooleanObject;
}

#endif
