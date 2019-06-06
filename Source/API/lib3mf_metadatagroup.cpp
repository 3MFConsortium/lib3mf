/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CMetaDataGroup

*/

#include "lib3mf_metadatagroup.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_metadata.hpp"
// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CMetaDataGroup 
**************************************************************************************************************************/

CMetaDataGroup::CMetaDataGroup(NMR::PModelMetaDataGroup pMetaDataGroup)
	: m_pModelMetaDataGroup(pMetaDataGroup)
{

}

Lib3MF_uint32 CMetaDataGroup::GetMetaDataCount ()
{
	return m_pModelMetaDataGroup->getMetaDataCount();
}

IMetaData * CMetaDataGroup::GetMetaData (const Lib3MF_uint32 nIndex)
{
	return new CMetaData(m_pModelMetaDataGroup->getMetaData(nIndex));
}

IMetaData * CMetaDataGroup::GetMetaDataByKey (const std::string & sNameSpace, const std::string & sName)
{
	for (NMR::nfUint32 i = 0; i < m_pModelMetaDataGroup->getMetaDataCount(); i++) {
		NMR::PModelMetaData pMetaData = m_pModelMetaDataGroup->getMetaData(i);
		if (sNameSpace.empty()) {
			if (pMetaData->getName() == sName) {
				return new CMetaData(pMetaData);
			}
		}
		else {
			if (pMetaData->getKey() == sNameSpace + ":" + sName) {
				return new CMetaData(pMetaData);
			}
		}
	}
	throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

void CMetaDataGroup::RemoveMetaDataByIndex (const Lib3MF_uint32 nIndex)
{
	m_pModelMetaDataGroup->removeMetaData(nIndex); 
}

void CMetaDataGroup::RemoveMetaData(IMetaData* pTheMetaData)
{
	for (NMR::nfUint32 i = 0; i < m_pModelMetaDataGroup->getMetaDataCount(); i++) {
		NMR::PModelMetaData pMetaData = m_pModelMetaDataGroup->getMetaData(i);
		if (pTheMetaData->GetName() == pMetaData->getName()) {
			m_pModelMetaDataGroup->removeMetaData(i);
			return;
		}
	}
	throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

IMetaData * CMetaDataGroup::AddMetaData(const std::string & sNameSpace, const std::string & sName, const std::string & sValue, const std::string & sType, const bool bMustPreserve)
{
	NMR::PModelMetaData pModelMetaData = m_pModelMetaDataGroup->addMetaData(sNameSpace, sName, sValue, sType, bMustPreserve);
	return new CMetaData(pModelMetaData);
}

