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

NMR_OpcPackageReader.h defines an OPC Package reader in a portable way.

--*/	

#ifndef __NMR_OPCPACKAGEREADER
#define __NMR_OPCPACKAGEREADER

#include "Common/OPC/NMR_IOpcPackageReader.h"
#include "Common/Platform/NMR_ImportStream.h"
#include "Common/OPC/NMR_OpcPackagePart.h"
#include "Common/OPC/NMR_OpcPackageTypes.h"
#include "Common/OPC/NMR_OpcPackageRelationship.h"
#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_ModelWarnings.h"
#include "Libraries/libzip/zip.h"
#include <list>
#include <vector>
#include <map>
#include <string>

namespace NMR {

	class COpcPackageReader: public IOpcPackageReader {
	protected:
		PModelWarnings m_pWarnings;
		PProgressMonitor m_pProgressMonitor;

		// ZIP Handling Variables
		std::vector<nfByte> m_Buffer;
		zip_error_t m_ZIPError;
		zip_t * m_ZIParchive;
		std::map <std::string, nfUint64> m_ZIPEntries;
		std::map <std::string, POpcPackagePart> m_Parts;

		std::string m_relationShipExtension;
		
		std::map<std::string, std::string> m_ContentTypes;
		std::list<POpcPackageRelationship> m_RootRelationships;

		void releaseZIP();

		PImportStream openZIPEntry(_In_ std::string sName);
		PImportStream openZIPEntryIndexed(_In_ nfUint64 nIndex);

		void readContentTypes();
		void readRootRelationships();

	public:
		COpcPackageReader(_In_ PImportStream pImportStream, _In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor);
		~COpcPackageReader();

		_Ret_maybenull_ COpcPackageRelationship * findRootRelation(_In_ std::string sRelationType, _In_ nfBool bMustBeUnique) override;
		POpcPackagePart createPart(_In_ std::string sPath) override;
		nfUint64 getPartSize(_In_ std::string sPath) override;
	};

	typedef std::shared_ptr<COpcPackageReader> POpcPackageReader;

}

#endif // __NMR_OPCPACKAGEREADER
