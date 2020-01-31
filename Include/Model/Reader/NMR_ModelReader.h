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

NMR_ModelReader.h defines the Model Reader Class.
A model reader reads in a model file and generates an in-memory representation of it.

--*/

#ifndef __NMR_MODELREADER
#define __NMR_MODELREADER

#include "Model/Classes/NMR_Model.h" 
#include "Model/Reader/NMR_ModelReaderWarnings.h" 
#include "Common/MeshImport/NMR_MeshImporter.h" 
#include "Common/3MF_ProgressMonitor.h" 

#include <list>

namespace NMR {
	class CSecureContext;
	using PSecureContext = std::shared_ptr<CSecureContext>;

	class CModelReader {
	protected:
		PModel m_pModel;
		PSecureContext m_pSecureContext;
		PImportStream m_pPrintTicketStream;
		std::string m_sPrintTicketContentType;
		std::set<std::string> m_RelationsToRead;

		PModelReaderWarnings m_pWarnings;
		PProgressMonitor m_pProgressMonitor;

		void readFromMeshImporter(_In_ CMeshImporter * pImporter);
	public:
		CModelReader() = delete;
		CModelReader(_In_ PModel pModel);
		virtual ~CModelReader() = default;

		virtual void readStream(_In_ PImportStream pStream) = 0;
		PImportStream retrievePrintTicket(_Out_ std::string & sContentType);
		PModelReaderWarnings getWarnings ();

		void addRelationToRead(_In_ std::string sRelationShipType);
		void removeRelationToRead(_In_ std::string sRelationShipType);

		void SetProgressCallback(Lib3MFProgressCallback callback, void* userData);

		PSecureContext getSecureContext() const;
	};

	typedef std::shared_ptr <CModelReader> PModelReader;

}

#endif // __NMR_MODELREADER
