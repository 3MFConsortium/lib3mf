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

NMR_ModelReader_3MF_Native.h implements the Model Reader Class.
A model reader reads in a 3MF file and generates an in-memory representation of it.
It uses libzip and irrxml to parse the OPC package.

--*/

#ifndef __NMR_MODELREADER_3MF_NATIVE
#define __NMR_MODELREADER_3MF_NATIVE

#include "Model/Reader/NMR_ModelReader_3MF.h" 
#include "Model/Reader/NMR_ModelReader.h"
#include "Model/Classes/NMR_Model.h"
#include "Common/Platform/NMR_XmlReader.h"
#include "Model/Reader/NMR_KeyStoreOpcPackageReader.h"
#include "Common/OPC/NMR_OpcPackagePart.h"
#include <list>

namespace NMR {

	class CModelReader_3MF_Native : public CModelReader_3MF {
	private:
		PKeyStoreOpcPackageReader m_pPackageReader;

	protected:
		void extractCustomDataFromRelationships(_In_ std::string& sTargetPartURIDir, _In_ COpcPackagePart * pModelPart);
		void extractTexturesFromRelationships(_In_ std::string& sTargetPartURIDir, _In_ COpcPackagePart * pModelPart);
		void extractModelDataFromRelationships(_In_ std::string& sTargetPartURIDir, _In_ COpcPackagePart * pModelPart);
		void checkContentTypes();
	
		virtual PImportStream extract3MFOPCPackage(_In_ PImportStream pPackageStream);
		virtual void release3MFOPCPackage();

	public:
		CModelReader_3MF_Native() = delete;
		CModelReader_3MF_Native(_In_ PModel pModel);
	};

	typedef std::shared_ptr <CModelReader_3MF_Native> PModelReader_3MF_Native;

}

#endif // __NMR_MODELREADER_3MF_NATIVE
