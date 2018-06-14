/*++

Copyright (C) 2018 3MF Consortium

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

#ifndef __NMR_OPCPACKAGERELATIONSHIP
#define __NMR_OPCPACKAGERELATIONSHIP

#include "Common/Platform/NMR_XmlWriter.h"
#include "Common/OPC/NMR_OpcPackagePart.h"
#include <string>
#include <list>
#include <map>

namespace NMR {

	class COpcPackagePart;
	typedef std::shared_ptr<COpcPackagePart> POpcPackagePart;

	class COpcPackageRelationship {
	private:
		std::string m_sID;
		std::string m_sType;
		std::string m_sTargetPartURI;

	public:
		COpcPackageRelationship(_In_ std::string sID, _In_ std::string sType, _In_ std::string sTargetPartURI);
		std::string getID ();
		std::string getType ();
		std::string getTargetPartURI();

		void writeToXML(_In_ CXmlWriter * pXMLWriter);

	};

	typedef std::shared_ptr<COpcPackageRelationship> POpcPackageRelationship;
}

#endif // __NMR_OPCPACKAGERELATIONSHIP

