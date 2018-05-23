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

NMR_OpcPackageRelationship.cpp implements an OPC Package Relationship Object.

--*/

#include "Common/OPC/NMR_OpcPackageRelationship.h" 
#include "Common/OPC/NMR_OpcPackageTypes.h"
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 

namespace NMR {

	COpcPackageRelationship::COpcPackageRelationship(_In_ std::string sID, _In_ std::string sType, _In_ std::string sTargetPartURI)
	{
		m_sID = sID;
		m_sType = sType;
		if ( sTargetPartURI.empty()) {	// || (sTargetPartURI[0] != L'/')
			// must not be emtpy
			throw CNMRException(NMR_ERROR_INVALIDOPCPARTURI);
		}
		std::string sName = fnExtractFileName(sTargetPartURI);
		if ( sName.empty() || (sName[0] == '.') ) {
			// must not be emtpy or start with a '.'
			throw CNMRException(NMR_ERROR_INVALIDOPCPARTURI);
		}
		m_sTargetPartURI = sTargetPartURI;
	}

	std::string COpcPackageRelationship::getID()
	{
		return m_sID;
	}

	std::string COpcPackageRelationship::getType()
	{
		return m_sType;
	}

	std::string COpcPackageRelationship::getTargetPartURI()
	{
		return m_sTargetPartURI;
	}

	void COpcPackageRelationship::writeToXML(_In_ CXmlWriter * pXMLWriter)
	{
		if (pXMLWriter == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::string sTarget = fnIncludeLeadingPathDelimiter(m_sTargetPartURI);

		pXMLWriter->WriteStartElement(nullptr, OPC_RELS_RELATIONSHIP_NODE, nullptr);
		pXMLWriter->WriteAttributeString(nullptr, OPC_RELS_ATTRIB_TYPE, nullptr, m_sType.c_str());
		pXMLWriter->WriteAttributeString(nullptr, OPC_RELS_ATTRIB_TARGET, nullptr, sTarget.c_str());
		pXMLWriter->WriteAttributeString(nullptr, OPC_RELS_ATTRIB_ID, nullptr, m_sID.c_str());
		pXMLWriter->WriteEndElement();
	}


}
