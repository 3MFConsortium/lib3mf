/*++

Copyright (C) 2018 Autodesk Inc.

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

Abstract: Progress Monitor

--*/

#ifndef __NMR_PROGRESSMONITOR
#define __NMR_PROGRESSMONITOR

#include "Common/3MF_ProgressTypes.h"

#include <memory>
#include <mutex>
#include <stack>

namespace NMR
{
#define PROGRESS_SLICEUPDATE 1000
#define PROGRESS_NODEUPDATE 100000
#define PROGRESS_TRIANGLEUPDATE 100000
#define PROGRESS_READUPDATE 20000
#define PROGRESS_READSLICESUPDATE 100
#define PROGRESS_READBUFFERUPDATE 100

	class CProgressMonitor
	{
	public:
		CProgressMonitor();
		void SetProgressCallback(Lib3MFProgressCallback callback, void* userData);
		void ClearProgressCallback();
		// Returns true if the last callback call returned false
		bool WasAborted();
		// If Progress() returns false, the task calling it should try to abort
		bool QueryCancelled();
		bool Progress(double progress, ProgressIdentifier identifier);

		static void GetProgressMessage(ProgressIdentifier progressIdentifier, const char ** progressString);

		void PushLevel(double relativeStart, double relativeEnd);
		std::pair<double, double> PopLevel();
		void ResetLevels();
	private:
		Lib3MFProgressCallback m_progressCallback;
		void* m_userData;
		bool m_lastCallbackResult;
		std::stack<std::pair<double, double>> m_levels;

		std::pair<double, double> Level();
		std::mutex m_callbackMutex;
	};

	typedef std::shared_ptr <CProgressMonitor> PProgressMonitor;
};

#endif // __NMR_PROGRESSMONITOR
