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

NMR_ModelVector3DFieldComposed.cpp implements the Model Vector3DFieldComposed Class.
It composes two vector 3d fields (and potentially a scalar field).
--*/

#include "Model/Classes/NMR_ModelVector3DFieldComposed.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include <cstring>

namespace NMR {

	CModelVector3DFieldComposed::CModelVector3DFieldComposed(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelVector3DField(sID, pModel),
		m_pVector3DFieldReference1(std::make_shared<CVector3DFieldReference>(nullptr)),
		m_pVector3DFieldReference2(std::make_shared<CVector3DFieldReference>(nullptr)),
		m_pScalarFieldReferenceMask(std::make_shared<CScalarFieldReference>(nullptr))
	{
	
	}

	nfDouble CModelVector3DFieldComposed::getFactor1() const
	{
		return m_dFactor1;
	}
	void CModelVector3DFieldComposed::setFactor1(nfDouble dFactor1)
	{
		m_dFactor1 = dFactor1;
	}

	nfDouble CModelVector3DFieldComposed::getFactor2() const
	{
		return m_dFactor2;
	}
	void CModelVector3DFieldComposed::setFactor2(nfDouble dFactor2)
	{
		m_dFactor2 = dFactor2;
	}

	eModelCompositionMethod CModelVector3DFieldComposed::getMethod() const
	{
		return m_eMethod;
	}
	void CModelVector3DFieldComposed::setMethod(eModelCompositionMethod eMethod)
	{
		m_eMethod = eMethod;
	}
	eModelCompositionSpace CModelVector3DFieldComposed::getSpace() const
	{
		return m_eSpace;
	}
	void CModelVector3DFieldComposed::setSpace(eModelCompositionSpace eCompositionSpace)
	{
		m_eSpace = eCompositionSpace;
	}

	PVector3DFieldReference CModelVector3DFieldComposed::Vector3DFieldReference1()
	{
		return m_pVector3DFieldReference1;
	}
	PVector3DFieldReference CModelVector3DFieldComposed::Vector3DFieldReference2()
	{
		return m_pVector3DFieldReference2;
	}
	PScalarFieldReference CModelVector3DFieldComposed::ScalarFieldReferenceMask()
	{
		return m_pScalarFieldReferenceMask;
	}

	std::string CModelVector3DFieldComposed::methodToString(_In_ eModelCompositionMethod eMethod)
	{
		switch (eMethod) {
		case eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MASK: return XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_MASK;
		case eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MIN: return XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_MIN;
		case eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MAX: return XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_MAX;
		case eModelCompositionMethod::MODELCOMPOSITIONMETHOD_WEIGHTEDSUM: return XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_WEIGHTEDSUM;
		case eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MULTIPLY: return XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_MULTIPLY;
		default:
			return XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_MAX;
		}
	}

	eModelCompositionMethod CModelVector3DFieldComposed::methodFromString(_In_ std::string sValue)
	{
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_MASK) == 0)
			return eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MASK;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_MIN) == 0)
			return eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MIN;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_MAX) == 0)
			return eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MAX;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_WEIGHTEDSUM) == 0)
			return eModelCompositionMethod::MODELCOMPOSITIONMETHOD_WEIGHTEDSUM;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_SCALARFIELDCOMPOSED_METHOD_MULTIPLY) == 0)
			return eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MULTIPLY;
		throw CNMRException(-1); // TODO: #define NMR_ERROR_INVALIDCOMPOSITIONMETHOD
		return eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MAX;
	}

	std::string CModelVector3DFieldComposed::spaceToString(_In_ eModelCompositionSpace eSpace)
	{
		switch (eSpace) {
		case eModelCompositionSpace::MODELCOMPOSITIONSPACE_RAW: return XML_3MF_ATTRIBUTE_VECTOR3DFIELDCOMPOSED_SPACE_RAW;
		case eModelCompositionSpace::MODELCOMPOSITIONSPACE_LINEAR: return XML_3MF_ATTRIBUTE_VECTOR3DFIELDCOMPOSED_SPACE_LINEAR;
		default:
			return XML_3MF_ATTRIBUTE_VECTOR3DFIELDCOMPOSED_SPACE_RAW;
		}
	}

	eModelCompositionSpace CModelVector3DFieldComposed::spaceFromString(_In_ std::string sValue)
	{
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_VECTOR3DFIELDCOMPOSED_SPACE_RAW) == 0)
			return eModelCompositionSpace::MODELCOMPOSITIONSPACE_RAW;
		if (strcmp(sValue.c_str(), XML_3MF_ATTRIBUTE_VECTOR3DFIELDCOMPOSED_SPACE_LINEAR) == 0)
			return eModelCompositionSpace::MODELCOMPOSITIONSPACE_LINEAR;
		throw CNMRException(-1); // TODO: #define NMR_ERROR_INVALIDCOMPOSITIONSPACE
		return eModelCompositionSpace::MODELCOMPOSITIONSPACE_RAW;
	}

}
