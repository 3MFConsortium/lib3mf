/*++

Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_OpcPackageWriter.h defines an OPC Package writer in a portable way.

--*/

#ifndef __NMR_OPCPACKAGEWRITER
#define __NMR_OPCPACKAGEWRITER

#include "Common/Platform/NMR_ExportStream.h"
#include "Common/Platform/NMR_ExportStream_ZIP.h"
#include "Common/Platform/NMR_PortableZIPWriter.h"
#include "Common/OPC/NMR_OpcPackagePart.h"
#include "Common/OPC/NMR_OpcPackageTypes.h"
#include "Common/OPC/NMR_OpcPackageRelationship.h"
#include <list>
#include <map>

namespace NMR {

	class COpcPackageWriter {
	protected:
		PExportStream m_pExportStream;
		std::list <POpcPackagePart> m_Parts;
		PPortableZIPWriter m_pZIPWriter;

		std::map<std::string, std::string> m_ContentTypes;
		std::list<POpcPackageRelationship> m_RootRelationships;

		void finishPackage();
		void writeContentTypes();
		void writeRootRelationships();
	public:
		COpcPackageWriter(_In_ PExportStream pExportStream);
		~COpcPackageWriter();

		POpcPackagePart addPart(_In_ std::string sPath);

		void addContentType(_In_ std::string sExtension, _In_ std::string sContentType);
		POpcPackageRelationship addRootRelationship(_In_ std::string sID, _In_ std::string sType, _In_ COpcPackagePart * pTargetPart);

	};

	typedef std::shared_ptr<COpcPackageWriter> POpcPackageWriter;

}

#endif // __NMR_XMLWRITER
