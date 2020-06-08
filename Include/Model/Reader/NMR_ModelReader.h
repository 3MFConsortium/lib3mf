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

#include "Model/Classes/NMR_ModelContext.h"
#include "Common/NMR_ModelWarnings.h" 
#include "Common/MeshImport/NMR_MeshImporter.h" 

#include <list>
#include <set>

namespace NMR {

	class CModelReader : public CModelContext {
	protected:
		PImportStream m_pPrintTicketStream;
		std::string m_sPrintTicketContentType;
		std::set<std::string> m_RelationsToRead;


		void readFromMeshImporter(_In_ CMeshImporter * pImporter);
	public:
		CModelReader() = delete;
		CModelReader(_In_ PModel pModel);
		virtual ~CModelReader() = default;

		virtual void readStream(_In_ PImportStream pStream) = 0;
		PImportStream retrievePrintTicket(_Out_ std::string & sContentType);

		void addRelationToRead(_In_ std::string sRelationShipType);
		void removeRelationToRead(_In_ std::string sRelationShipType);
	};

	typedef std::shared_ptr <CModelReader> PModelReader;

}

#endif // __NMR_MODELREADER
