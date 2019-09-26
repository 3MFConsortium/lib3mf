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

NMR_OpcPackageRelationship.h defines an OPC Package Relationship Object.

--*/

#ifndef __NMR_OPCPACKAGERELATIONSHIPREADER
#define __NMR_OPCPACKAGERELATIONSHIPREADER

#include "Common/3MF_ProgressMonitor.h"
#include "Common/Platform/NMR_ImportStream.h"
#include "Common/Platform/NMR_XmlReader.h"
#include "Common/OPC/NMR_OpcPackageRelationship.h"
#include <string>
#include <vector>
#include <map>


namespace NMR {

	class COpcPackageRelationshipReader {
	private:
		std::map<std::string, POpcPackageRelationship> m_IDToRelationShips;

		void parseRootNode(_In_ CXmlReader * pXMLReader);
		void parseChildNode(_In_ CXmlReader * pXMLReader);
		void parseAttributes(_In_ CXmlReader * pXMLReader);
	public:
		COpcPackageRelationshipReader(_In_ PImportStream pImportStream, _In_ PProgressMonitor);
		~COpcPackageRelationshipReader();

		nfUint32 getCount();
		POpcPackageRelationship getRelationShip(_In_ nfUint32 nIndex);
	};

	typedef std::shared_ptr<COpcPackageRelationshipReader> POpcPackageRelationshipReader;
}

#endif // __NMR_OPCPACKAGERELATIONSHIPREADER

