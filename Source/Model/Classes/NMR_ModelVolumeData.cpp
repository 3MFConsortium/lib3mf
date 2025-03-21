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

#include "Model/Classes/NMR_ModelFunction.h"
#include "lib3mf_interfaceexception.hpp"

namespace NMR 
{
	CModelVolumeData::CModelVolumeData(ModelResourceID sResourceID, CModel * pModel) : CModelResource(sResourceID, pModel)
	{
		clear();
	}	
	
	void CModelVolumeData::clear()
	{
		m_pComposite.reset();
		m_pColor.reset();
		m_mapProperties.clear();
	}

	nfBool CModelVolumeData::hasProperty(std::string sName)
	{
		std::map<std::string, PVolumeDataProperty>::iterator iIterator = m_mapProperties.find(sName);
		return iIterator != m_mapProperties.end();
	}

	nfUint32 CModelVolumeData::getPropertyCount() const
	{
		return (nfUint32)m_mapProperties.size();
	}

	PVolumeDataProperty CModelVolumeData::getProperty(nfUint32 nIndex)
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

	PVolumeDataProperty CModelVolumeData::findProperty(std::string sName)
	{
		std::map<std::string, PVolumeDataProperty>::iterator iIterator = m_mapProperties.find(sName);
		if (iIterator != m_mapProperties.end()) {
			return iIterator->second;
		}
		return nullptr;
	}

	void CModelVolumeData::addProperty(PVolumeDataProperty pProperty)
	{
		if (!pProperty)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (hasProperty(pProperty->getName())) {
			throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATAPROPERTY);
		}
		m_mapProperties.insert(std::make_pair(pProperty->getName(), pProperty));
	}

	PVolumeDataProperty CModelVolumeData::addProperty(
		std::string sName, PModelFunction pfunction)
	{
		if(!pfunction) throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if(hasProperty(sName))
		{
				throw CNMRException(
					NMR_ERROR_DUPLICATEVOLUMEDATAPROPERTY);
		}

		PVolumeDataProperty pVolumeDataProperty =
			std::make_shared<CVolumeDataProperty>(pfunction, sName);
		m_mapProperties.insert(std::make_pair(pVolumeDataProperty->getName(), pVolumeDataProperty));
		return pVolumeDataProperty;
    }

    void CModelVolumeData::removeProperty(std::string sName)
	{
		m_mapProperties.erase(sName);
	}

	bool CModelVolumeData::hasColor() const
	{
		return m_pColor.get() != nullptr;
	}

	void CModelVolumeData::setColor(PVolumeDataColor pColor)
	{
		if (!pColor)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pColor = pColor;
	}

    PVolumeDataColor CModelVolumeData::createColor(PModelFunction pfunction)
    {
    	if(!pfunction)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

        m_pColor = std::make_shared<CVolumeDataColor>(pfunction);
        return m_pColor;
	}

	PVolumeDataColor CModelVolumeData::getColor()
	{
		return m_pColor;
	}

	void CModelVolumeData::removeColor()
	{
		m_pColor.reset();
	}

	bool CModelVolumeData::hasComposite() const
	{
		return  m_pComposite.get() != nullptr;
	}


	PVolumeDataComposite CModelVolumeData::getComposite()
	{
		return m_pComposite;
	}
	PVolumeDataComposite CModelVolumeData::createComposite(/* basematerialgroupd*/)
	{
		m_pComposite = std::make_shared<CVolumeDataComposite>();
		return m_pComposite;
	}

	void CModelVolumeData::setComposite(PVolumeDataComposite pComposite)
	{
		if (!pComposite)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pComposite = pComposite;
	}

	void CModelVolumeData::removeComposite()
	{
		m_pComposite.reset();
	}

	ResourceDependencies CModelVolumeData::getDependencies()
	{
		ResourceDependencies dependencies;

		if (m_pColor)
		{
			dependencies.push_back(packageResourceIDFromResourceID(m_pColor->getFunctionResourceID()));
		}

		for (auto iIterator = m_mapProperties.begin(); iIterator != m_mapProperties.end(); iIterator++)
		{
			PVolumeDataProperty pProperty = iIterator->second;
			if (pProperty)
			{
				dependencies.push_back(packageResourceIDFromResourceID(pProperty->getFunctionResourceID()));
			}
		}

		if (m_pComposite)
		{
			for (nfUint32 i = 0u; i < m_pComposite->materialMappingCount(); i++)
			{
				dependencies.push_back(packageResourceIDFromResourceID(m_pComposite->getMaterialMapping(i)->getFunctionResourceID()));
			}
			
		}

		return dependencies;
	}

	PPackageResourceID
	CModelVolumeData::packageResourceIDFromResourceID(
		UniqueResourceID uniqueResourceID)
	{
		auto* model = getModel();
		if(!model)
		{
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM,
											"Model must not be nullptr.");
		}

		auto referendedResource = model->findResource(uniqueResourceID);
		if(!referendedResource)
		{
			throw ELib3MFInterfaceException(
				LIB3MF_ERROR_INVALIDPARAM,
				"Referenced resource must not be nullptr.");
		}

		return referendedResource->getPackageResourceID();
	}
}  // namespace NMR
