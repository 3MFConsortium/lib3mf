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

NMR_ModelWriter.h defines the Model Writer Class.
A model writer exports the in memory represenation into the 3MF file.

--*/

#ifndef __NMR_MODELWRITER
#define __NMR_MODELWRITER

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelContext.h"
#include "Common/Platform/NMR_ExportStream.h" 
#include "Common/3MF_ProgressMonitor.h" 
#include <list>

namespace NMR {

	class CModelWriter : public CModelContext{
	private:
		nfUint32 m_nDecimalPrecision;
	public:
		CModelWriter() = delete;
		CModelWriter(_In_ PModel pModel);
		virtual ~CModelWriter() = default;

		virtual void exportToStream(_In_ PExportStream pStream) = 0;

		void SetDecimalPrecision(nfUint32);
		nfUint32 GetDecimalPrecision();
	};

	typedef std::shared_ptr <CModelWriter> PModelWriter;

}

#endif // __NMR_MODELWRITER
