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

NMR_OpcPackageContentTypeReader.h defines a reader for the OPC Package Content Types

--*/

#ifndef __NMR_OPCPACKAGECONTENTTYPESREADER
#define __NMR_OPCPACKAGECONTENTTYPESREADER

#include "Common/3MF_ProgressMonitor.h"
#include "Common/Platform/NMR_ImportStream.h"
#include "Common/Platform/NMR_XmlReader.h"
#include <string>
#include <vector>
#include <map>


namespace NMR {

	struct COpcPackageContentType {
		std::string m_extension;
		std::string m_contentType;
		COpcPackageContentType(std::string extension, std::string contentType) {
			m_extension = extension;
			m_contentType = contentType;
		}
	};
	struct COpcPackageContentType_Override {
		std::string m_partName;
		std::string m_contentType;
		COpcPackageContentType_Override(std::string partName, std::string contentType) {
			m_partName = partName;
			m_contentType = contentType;
		}
	};
	typedef std::shared_ptr<COpcPackageContentType> POpcPackageContentType;
	typedef std::shared_ptr<COpcPackageContentType_Override> POpcPackageContentType_Override;

	class COpcPackageContentTypesReader {
	private:
		std::vector<POpcPackageContentType> m_ContentTypes;
		std::vector<POpcPackageContentType_Override> m_Overrides;

		void parseRootNode(_In_ CXmlReader * pXMLReader);
		void parseChildNode(_In_ CXmlReader * pXMLReader, _In_ bool bOverride);
		void parseAttributes(_In_ CXmlReader * pXMLReader);
	public:
		COpcPackageContentTypesReader(_In_ PImportStream pImportStream, PProgressMonitor pProgressMonitor);
		~COpcPackageContentTypesReader();

		nfUint32 getCount();
		POpcPackageContentType getContentType(_In_ nfUint32 nIndex);
		nfUint32 getOverrideCount();
		POpcPackageContentType_Override getOverrideContentType(_In_ nfUint32 nIndex);
	};

	typedef std::shared_ptr<COpcPackageContentTypesReader> POpcPackageContentTypesReader;
}

#endif // __NMR_OPCPACKAGECONTENTTYPESREADER

