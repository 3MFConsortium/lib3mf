/*++

Copyright (C) 2015 Microsoft Corporation
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

NMR_ModelWriterNode.cpp implements Model Writer Node Class.
This is the abstract base class for all 3MF model stream exporters.

--*/

#include "Model/Writer/NMR_ModelWriterNode.h" 

#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include <sstream>

namespace NMR {

	CModelWriterNode::CModelWriterNode(_In_ CModel * pModel, _In_ CXmlWriter * pXMLWriter, _In_ CProgressMonitor * pProgressMonitor)
	{
		__NMRASSERT(pModel);
		__NMRASSERT(pXMLWriter);
		m_pModel = pModel;
		m_pXMLWriter = pXMLWriter;
		m_pProgressMonitor = pProgressMonitor;
	}

	void CModelWriterNode::writeStringAttribute(_In_z_ const nfWChar * pAttributeName, _In_ std::wstring sAttributeValue)
	{
		writeConstStringAttribute(pAttributeName, sAttributeValue.c_str ());
	}

	void CModelWriterNode::writePrefixedStringAttribute(_In_z_ const nfWChar * pPrefix, _In_ const nfWChar * pAttributeName, std::wstring sAttributeValue)
	{
		writeConstPrefixedStringAttribute(pPrefix, pAttributeName, sAttributeValue.c_str());
	}

	void CModelWriterNode::writeConstStringAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		__NMRASSERT(m_pXMLWriter);

		m_pXMLWriter->WriteAttributeString(nullptr, pAttributeName, nullptr, pAttributeValue);
	}

	void CModelWriterNode::writeConstPrefixedStringAttribute(_In_z_ const nfWChar * pPrefix, _In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		__NMRASSERT(pPrefix);
		__NMRASSERT(m_pXMLWriter);

		m_pXMLWriter->WriteAttributeString(pPrefix, pAttributeName, nullptr, pAttributeValue);
	}

	void CModelWriterNode::writeIntAttribute(_In_z_ const nfWChar * pAttributeName, _In_ nfInt32 nAttributeValue)
	{
		std::wstringstream sStream;
		sStream << nAttributeValue;
		writeConstStringAttribute(pAttributeName, sStream.str().c_str());
	}

	void CModelWriterNode::writeUintAttribute(_In_z_ const nfWChar * pAttributeName, _In_ nfUint32 nAttributeValue)
	{
		std::wstringstream sStream;
		sStream << nAttributeValue;
		writeConstStringAttribute(pAttributeName, sStream.str().c_str());
	}

	void CModelWriterNode::writeFloatAttribute(_In_z_ const nfWChar * pAttributeName, _In_ nfFloat fAttributeValue)
	{
		std::wstringstream sStream;
		sStream << fAttributeValue;
		writeConstStringAttribute(pAttributeName, sStream.str().c_str());
	}

	void CModelWriterNode::writeStartElement(_In_z_ const nfWChar * pElementName)
	{
		m_pXMLWriter->WriteStartElement(nullptr, pElementName, nullptr);
	}

	void CModelWriterNode::writeStartElementWithNamespace(_In_z_ const nfWChar * pElementName, _In_z_ const nfWChar * pNameSpace)
	{
		m_pXMLWriter->WriteStartElement(nullptr, pElementName, pNameSpace);
	}

	void CModelWriterNode::writeStartElementWithPrefix(_In_z_ const nfWChar * pElementName, _In_z_ const nfWChar * pPrefix)
	{
		m_pXMLWriter->WriteStartElement(pPrefix, pElementName, nullptr);
	}

	void CModelWriterNode::writeEndElement()
	{
		m_pXMLWriter->WriteEndElement();
	}

	void CModelWriterNode::writeFullEndElement()
	{
		m_pXMLWriter->WriteFullEndElement();
	}

	void CModelWriterNode::writeText(_In_z_ const nfWChar * pwszText, _In_ nfUint32 cbLength)
	{
		m_pXMLWriter->WriteText(pwszText, cbLength);
	}


}
