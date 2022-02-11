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

NMR_ModelVolumeData.cpp implements the class CModelVolumeData.

--*/

#include "Model/Classes/NMR_ModelVolumeData.h"

#include "Model/Classes/NMR_ModelScalarField.h"
#include "Model/Classes/NMR_ModelVector3DField.h"

namespace NMR {

	CModelVolumeData::CModelVolumeData()
		:m_pBoundary(nullptr), m_pComposite(nullptr), m_pColor(nullptr), m_mapProperties()
	{

	}

	void CModelVolumeData::clear()
	{
		m_pBoundary.reset();
		m_pComposite.reset();
		m_pColor.reset();
		m_mapProperties.clear();
	}

	bool CModelVolumeData::HasBoundary() const
	{
		return m_pBoundary.get() != nullptr;
	}

	void CModelVolumeData::SetBoundary(PVolumeDataBoundary pLevelset)
	{
		if (!pLevelset)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pBoundary = pLevelset;
	}

	PVolumeDataBoundary CModelVolumeData::CreateBoundary(PModelScalarField pScalarField)
	{
		if (!pScalarField)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pBoundary = std::make_shared<CVolumeDataBoundary>(pScalarField);
		return m_pBoundary;
	}

	PVolumeDataBoundary CModelVolumeData::GetBoundary()
	{
		return m_pBoundary;
	}


	nfBool CModelVolumeData::hasProperty(_In_ std::string sName)
	{
		std::map<std::string, PVolumeDataProperty>::iterator iIterator = m_mapProperties.find(sName);
		return iIterator != m_mapProperties.end();
	}

	nfUint32 CModelVolumeData::GetPropertyCount() const
	{
		return (nfUint32)m_mapProperties.size();
	}

	PVolumeDataProperty CModelVolumeData::GetProperty(nfUint32 nIndex)
	{
		auto iIterator = m_mapProperties.begin();
		if (nIndex >= m_mapProperties.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		
		while (nIndex > 0) {
			nIndex--;
			iIterator++;
		}
		return iIterator->second;
	}

	PVolumeDataProperty CModelVolumeData::FindProperty(std::string sName)
	{
		std::map<std::string, PVolumeDataProperty>::iterator iIterator = m_mapProperties.find(sName);
		if (iIterator != m_mapProperties.end()) {
			return iIterator->second;
		}
		return nullptr;
	}

	void CModelVolumeData::AddProperty(PVolumeDataProperty pProperty)
	{
		if (!pProperty)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (hasProperty(pProperty->GetName())) {
			throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATAPROPERTY);
		}
		m_mapProperties.insert(std::make_pair(pProperty->GetName(), pProperty));
	}

	PVolumeDataProperty CModelVolumeData::AddProperty(std::string sName, PModelScalarField pScalarField)
	{
		if (!pScalarField)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (hasProperty(sName)) {
			throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATAPROPERTY);
		}

		PVolumeDataProperty pVolumeDataProperty = std::make_shared<CVolumeDataProperty>(pScalarField, sName);
		m_mapProperties.insert(std::make_pair(pVolumeDataProperty->GetName(), pVolumeDataProperty));
		return pVolumeDataProperty;
	}

	PVolumeDataProperty CModelVolumeData::AddProperty(std::string sName, PModelVector3DField pVector3DField)
	{
		if (!pVector3DField)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (hasProperty(sName)) {
			throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATAPROPERTY);
		}

		PVolumeDataProperty pVolumeDataProperty = std::make_shared<CVolumeDataProperty>(pVector3DField, sName);
		m_mapProperties.insert(std::make_pair(pVolumeDataProperty->GetName(), pVolumeDataProperty));
		return pVolumeDataProperty;
	}

	void CModelVolumeData::RemoveProperty(std::string sName)
	{
		m_mapProperties.erase(sName);
	}

	bool CModelVolumeData::HasColor() const
	{
		return m_pColor.get() != nullptr;
	}

	void CModelVolumeData::SetColor(PVolumeDataColor pColor)
	{
		if (!pColor)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pColor = pColor;
	}

	PVolumeDataColor CModelVolumeData::CreateColor(PModelVector3DField pVector3DField)
	{
		if (!pVector3DField)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pColor = std::make_shared<CVolumeDataColor>(pVector3DField);
		return m_pColor;
	}

	PVolumeDataColor CModelVolumeData::GetColor()
	{
		return m_pColor;
	}
}
