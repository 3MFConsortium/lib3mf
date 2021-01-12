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

NMR_ModelReader_3MF.h defines the Model Reader Class for
3MF Files. A 3MF model reader reads in a 3MF file and generates an in-memory representation of it.

--*/

#ifndef __NMR_MODELREADER_3MF
#define __NMR_MODELREADER_3MF

#include "Model/Reader/NMR_ModelReader.h" 
#include "Common/NMR_SecureContentTypes.h"
#include <string>
#include <map>

namespace NMR {

	class CModelReader_3MF : public CModelReader {
	protected:
		virtual PImportStream extract3MFOPCPackage(_In_ PImportStream pPackageStream) = 0;
		virtual void release3MFOPCPackage() = 0;

	public:
		CModelReader_3MF() = delete;
		CModelReader_3MF(_In_ PModel pModel);

		virtual void readStream(_In_ PImportStream pStream);
		virtual void addTextureAttachment(_In_ std::string sPath, _In_ PImportStream pStream);
	};

	typedef std::shared_ptr <CModelReader_3MF> PModelReader_3MF;

}

#endif // __NMR_MODELREADER_3MF
