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

NMR_ModelReaderWarnings.cpp implements the Model Reader Warnings Class.
This class collects all import warnings with certain severity levels - in order to allow
a relaxed import policy on the file format.

--*/

#include "Common/NMR_ModelWarnings.h" 

namespace NMR {

	CModelWarning::CModelWarning(std::string sMessage, eModelWarningLevel WarningLevel, nfError nErrorCode)
	{
		m_sMessage = sMessage;
		m_WarningLevel = WarningLevel;
		m_nErrorCode = nErrorCode;
	}

	std::string CModelWarning::getMessage()
	{
		return m_sMessage;
	}

	eModelWarningLevel CModelWarning::getWarningLevel()
	{
		return m_WarningLevel;
	}

	nfError CModelWarning::getErrorCode()
	{
		return m_nErrorCode;
	}

	CModelWarnings::CModelWarnings()
	{
		setCriticalWarningLevel(mrwFatal);
	}

	eModelWarningLevel CModelWarnings::getCriticalWarningLevel()
	{
		return m_CriticalWarningLevel;
	}

	void CModelWarnings::setCriticalWarningLevel(_In_ eModelWarningLevel WarningLevel)
	{
		m_CriticalWarningLevel = WarningLevel;
	}

	void CModelWarnings::addWarning(_In_ nfError nErrorCode, eModelWarningLevel WarningLevel)
	{
		CNMRException e(nErrorCode);
		addException(e, WarningLevel);
	}

	void CModelWarnings::addException(const _In_ CNMRException & Exception, _In_ eModelWarningLevel WarningLevel)
	{
		if (m_Warnings.size() < NMR_MAXWARNINGCOUNT) { // Failsafe check for Index overflows
			std::string sMessage (Exception.what());

			PModelReaderWarning pWarning = std::make_shared<CModelWarning>(sMessage, WarningLevel, Exception.getErrorCode());
			m_Warnings.push_back(pWarning);
		}

		if ((nfInt32)WarningLevel <= (nfInt32)m_CriticalWarningLevel)
			throw Exception;
	}

	nfUint32 CModelWarnings::getWarningCount()
	{
		return (nfUint32)m_Warnings.size();
	}

	PModelReaderWarning CModelWarnings::getWarning(_In_ nfUint32 nIndex)
	{
		return m_Warnings[nIndex];
	}



}
