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

NMR_OpcPackagePart.h defines an OPC Package Part in a portable way.

--*/

#ifndef __NMR_OPCPACKAGEPART
#define __NMR_OPCPACKAGEPART

#include "Common/OPC/NMR_OpcPackageRelationship.h"
#include "Common/OPC/NMR_OpcPackageTypes.h"
#include "Common/Platform/NMR_ExportStream.h"
#include "Common/Platform/NMR_ImportStream.h"

#include <string>
#include <list>

namespace NMR {

	class COpcPackageRelationship;
	typedef std::shared_ptr<COpcPackageRelationship> POpcPackageRelationship;

	class COpcPackagePart {
	protected:
		std::wstring m_sURI;
		PExportStream m_pExportStream;
		PImportStream m_pImportStream;

		std::list<POpcPackageRelationship> m_Relationships;
	public:
		COpcPackagePart(_In_ std::wstring sURI, _In_ PExportStream pExportStream);
		COpcPackagePart(_In_ std::wstring sURI, _In_ PImportStream pImportStream);

		std::wstring getURI ();
		PExportStream getExportStream ();
		PImportStream getImportStream();

		POpcPackageRelationship addRelationship(_In_ std::wstring sID, _In_ std::wstring sType, _In_ COpcPackagePart * pTargetPart);
		nfBool hasRelationships();

		void writeRelationships(_In_ PExportStream pExportStream);
	};

	typedef std::shared_ptr<COpcPackagePart> POpcPackagePart;

}

#endif // __NMR_OPCPACKAGEPART
