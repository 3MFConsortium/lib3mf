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

NMR_ModelReaderWarnings.cpp implements the Model Reader Warnings Class.
This class collects all import warnings with certain severity levels - in order to allow
a relaxed import policy on the file format.

--*/

#include "Model/Reader/NMR_ModelReaderWarnings.h" 

namespace NMR {

	CModelReaderWarning::CModelReaderWarning(std::wstring sMessage, eModelReaderWarningLevel WarningLevel, nfError nErrorCode)
	{
		m_sMessage = sMessage;
		m_WarningLevel = WarningLevel;
		m_nErrorCode = nErrorCode;
	}

	std::wstring CModelReaderWarning::getMessage()
	{
		return m_sMessage;
	}

	eModelReaderWarningLevel CModelReaderWarning::getWarningLevel()
	{
		return m_WarningLevel;
	}

	nfError CModelReaderWarning::getErrorCode()
	{
		return m_nErrorCode;
	}

	CModelReaderWarnings::CModelReaderWarnings()
	{
		setCriticalWarningLevel(mrwInvalidMandatoryValue);
	}

	eModelReaderWarningLevel CModelReaderWarnings::getCriticalWarningLevel()
	{
		return m_CriticalWarningLevel;
	}

	void CModelReaderWarnings::setCriticalWarningLevel(_In_ eModelReaderWarningLevel WarningLevel)
	{
		m_CriticalWarningLevel = WarningLevel;
	}

	void CModelReaderWarnings::addWarning(_In_ std::wstring sMessage, _In_ nfError nErrorCode, eModelReaderWarningLevel WarningLevel)
	{
		if (m_Warnings.size() < NMR_MAXWARNINGCOUNT) { // Failsafe check for Index overflows
			PModelReaderWarning pWarning = std::make_shared<CModelReaderWarning>(sMessage, WarningLevel, nErrorCode);
			m_Warnings.push_back(pWarning);
		}

		if ((nfInt32)WarningLevel < (nfInt32)m_CriticalWarningLevel)
			throw CNMRException(nErrorCode);

	}

	void CModelReaderWarnings::addException(_In_ CNMRException & Exception, _In_ eModelReaderWarningLevel WarningLevel)
	{
		if (m_Warnings.size() < NMR_MAXWARNINGCOUNT) { // Failsafe check for Index overflows
			std::string sAsciiMessage (Exception.what());
			std::wstring sMessage (sAsciiMessage.begin(), sAsciiMessage.end());

			PModelReaderWarning pWarning = std::make_shared<CModelReaderWarning>(sMessage, WarningLevel, Exception.getErrorCode());
			m_Warnings.push_back(pWarning);
		}

		if ((nfInt32)WarningLevel < (nfInt32)m_CriticalWarningLevel)
			throw Exception;
	}

	nfUint32 CModelReaderWarnings::getWarningCount()
	{
		return (nfUint32)m_Warnings.size();
	}

	PModelReaderWarning CModelReaderWarnings::getWarning(_In_ nfUint32 nIndex)
	{
		return m_Warnings[nIndex];
	}



}
