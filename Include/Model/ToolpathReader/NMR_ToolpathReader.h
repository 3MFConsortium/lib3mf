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

NMR_ModelReader_ToolpathReader.h defines the Toolpath Layer Reader.

--*/

#ifndef __NMR_TOOLPATHREADER
#define __NMR_TOOLPATHREADER

#include <string>
#include <map>
#include "Common/NMR_ModelWarnings.h" 
#include "Model/Classes/NMR_ModelToolpathLayerReadData.h" 
#include "Common/3MF_ProgressMonitor.h" 


namespace NMR {

	class CToolpathReader {
	protected:
		nfBool m_bAllowBinaryStreams;

		PProgressMonitor m_pProgressMonitor;
		PModelWarnings m_pWarnings;

		PModelToolpathLayerReadData m_pReadData;

	public:
		CToolpathReader() = delete;
		CToolpathReader(PModelToolpath pModelToolpath, nfBool bAllowBinaryStreams);

		void readStream(_In_ PImportStream pStream);

		PModelToolpathLayerReadData getReadData();
	};

	typedef std::shared_ptr <CToolpathReader> PToolpathReader;

}

#endif // __NMR_TOOLPATHREADER

