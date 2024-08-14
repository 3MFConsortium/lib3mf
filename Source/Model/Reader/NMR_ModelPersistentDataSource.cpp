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

--*/

#include "Model/Reader/NMR_ModelPersistentDataSource.h" 

#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 

namespace NMR {

	CModelPersistentDataSource::CModelPersistentDataSource(PImportStream pImportStream)
		: m_pImportStream (pImportStream)
	{

		if (pImportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}


	CModelPersistentDataSource::~CModelPersistentDataSource()
	{
		invalidateSource();
	}


	PImportStream CModelPersistentDataSource::getImportStream()
	{
		if (m_pImportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_DATASOURCEISINVALIDATED);

		return m_pImportStream;
	}

	PIOpcPackageReader CModelPersistentDataSource::getOpcPackageReader()
	{
		if (m_pPackageReader.get() == nullptr)
			throw CNMRException(NMR_ERROR_DATASOURCEISINVALIDATED);

		return m_pPackageReader;

	}

	void CModelPersistentDataSource::setOPCPackageReader(PIOpcPackageReader pPackageReader)
	{
		releaseOPCPackageReader();

		m_pPackageReader = pPackageReader;
	}

	void CModelPersistentDataSource::releaseOPCPackageReader()
	{
		if (m_pPackageReader.get() != nullptr)
			m_pPackageReader->close();

		m_pPackageReader = nullptr;
	}


	bool CModelPersistentDataSource::isValid()
	{
		return (m_pImportStream.get() != nullptr);
	}

	void CModelPersistentDataSource::invalidateSource()
	{
		releaseOPCPackageReader();
		m_pImportStream = nullptr;
	}
	
}
