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

NMR_ModelReaderWarnings.h defines the Model Reader Warnings Class.
This class collects all import warnings with certain severity levels - in order to allow
a relaxed import policy on the file format.

--*/

#ifndef __NMR_MODELREADERWARNINGS
#define __NMR_MODELREADERWARNINGS

#include "Common/NMR_Types.h" 
#include "Common/NMR_Exception.h" 
#include <memory>
#include <string>
#include <vector>

#define NMR_MAXWARNINGCOUNT 1000000000

namespace NMR {

	typedef enum _eModelWarningLevel {
		mrwFatal,
		mrwInvalidMandatoryValue,
		mrwMissingMandatoryValue,
		mrwInvalidOptionalValue
	} eModelWarningLevel;

	class CModelWarning {
	private:
		std::string m_sMessage;
		eModelWarningLevel m_WarningLevel;
		nfError m_nErrorCode;
	public:
		CModelWarning() = delete;
		CModelWarning(std::string sMessage, eModelWarningLevel WarningLevel, nfError nErrorCode);

		std::string getMessage();
		eModelWarningLevel getWarningLevel();
		nfError getErrorCode();
	};

	typedef std::shared_ptr <CModelWarning> PModelReaderWarning;

	class CModelWarnings {
	private:
		std::vector<PModelReaderWarning> m_Warnings;
		eModelWarningLevel m_CriticalWarningLevel;
	public:
		CModelWarnings();

		eModelWarningLevel getCriticalWarningLevel ();
		void setCriticalWarningLevel(_In_ eModelWarningLevel WarningLevel);

		void addWarning(_In_ nfError nErrorCode, _In_ eModelWarningLevel WarningLevel);
		void addException(const _In_ CNMRException & Exception, _In_ eModelWarningLevel WarningLevel);

		nfUint32 getWarningCount();
		PModelReaderWarning getWarning(_In_ nfUint32 nIndex);
	};

	typedef std::shared_ptr <CModelWarnings> PModelWarnings;

}

#endif // __NMR_MODELREADERWARNINGS
