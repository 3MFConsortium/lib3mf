/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CBooleanObject

*/


#ifndef __LIB3MF_BOOLEANOBJECT
#define __LIB3MF_BOOLEANOBJECT

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_object.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelBooleanObject.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CBooleanObject 
**************************************************************************************************************************/

class CBooleanObject : public virtual IBooleanObject, public virtual CObject {
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


	CBooleanObject() = delete;
	CBooleanObject(NMR::PModelResource pResource);

	NMR::PModelBooleanObject booleanObject();

	/**
	* Public member functions to implement.
	*/

	bool IsBooleanObject() override;
	bool IsMeshObject() override;
	bool IsComponentsObject() override;
	bool IsLevelSetObject() override;

	void SetBaseObject(IObject* pBaseObject, const Lib3MF::sTransform Transform) override;

	IObject * GetBaseObject() override;

	void SetBaseTransform(const Lib3MF::sTransform Transform) override;

	Lib3MF::sTransform GetBaseTransform() override;

	void SetOperation(const Lib3MF::eBooleanOperation eOperation) override;

	Lib3MF::eBooleanOperation GetOperation() override;

	void SetCSGModeEnabled(const bool bCSGModeEnabled) override;

	bool GetCSGModeEnabled() override;

	void SetExtractionGridResolution(const Lib3MF_uint32 nGridResolution) override;

	Lib3MF_uint32 GetExtractionGridResolution() override;

	Lib3MF_uint32 GetOperandCount() override;

	void AddOperand(IMeshObject* pOperandObject, const Lib3MF::sTransform Transform) override;

	Lib3MF::sTransform GetOperand(const Lib3MF_uint32 nIndex, IMeshObject*& pOperandObject) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_BOOLEANOBJECT
