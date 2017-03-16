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

NMR_XmlWriter_COM.cpp implements the XML Writer Class using XMLLite on Windows.

--*/

#include "Common/Platform/NMR_XmlWriter_COM.h" 
#include "Common/Platform/NMR_ExportStream_COM.h" 
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CXmlWriter_COM::CXmlWriter_COM(_In_ PExportStream pExportStream)
		: CXmlWriter(pExportStream)
	{
		HRESULT hResult;
		if (pExportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		// Create XML Writer
		hResult = CreateXmlWriter(__uuidof(IXmlWriter), (void**)&m_pXMLWriter, nullptr);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTCREATEXMLWRITER, hResult);

		// Cast to COM Streams
		CExportStream_COM * pCOMExportStream = dynamic_cast<CExportStream_COM *> (pExportStream.get());
		if (pCOMExportStream == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDSTREAMTYPE);
		m_pCOMStream = pCOMExportStream->getCOMStream();

		// Set XML Writer Output
		hResult = m_pXMLWriter->SetOutput(m_pCOMStream);
		if (hResult != S_OK)
		  throw CNMRException_Windows(NMR_ERROR_COULDNOTSETXMLOUTPUT, hResult);

		// Set XML Writer Property
		hResult = m_pXMLWriter->SetProperty(XmlWriterProperty_Indent, true);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSETXMLPROPERTY, hResult);
	}

	void CXmlWriter_COM::WriteStartDocument()
	{
		HRESULT hResult;
		hResult = m_pXMLWriter->WriteStartDocument(XmlStandalone_Omit);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITEXMLSTARTDOCUMENT, hResult);
	}

	void CXmlWriter_COM::WriteEndDocument()
	{
		HRESULT hResult;
		hResult = m_pXMLWriter->WriteEndDocument();
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITEXMLENDDOCUMENT, hResult);
	}

	void CXmlWriter_COM::Flush()
	{
		HRESULT hResult;
		hResult = m_pXMLWriter->Flush();
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTFLUSHXMLWRITER, hResult);
	}

	void CXmlWriter_COM::WriteAttributeString(_In_opt_ LPCWSTR pwszPrefix, _In_opt_ LPCWSTR pwszLocalName, _In_opt_ LPCWSTR pwszNamespaceUri, _In_opt_ LPCWSTR pwszValue)
	{
		HRESULT hResult = m_pXMLWriter->WriteAttributeString(pwszPrefix, pwszLocalName, pwszNamespaceUri, pwszValue);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITEXMLATTRIBUTE, hResult);

	}

	void CXmlWriter_COM::WriteStartElement(_In_opt_  LPCWSTR pwszPrefix, _In_  LPCWSTR pwszLocalName, _In_opt_  LPCWSTR pwszNamespaceUri)
	{
		HRESULT hResult = m_pXMLWriter->WriteStartElement(pwszPrefix, pwszLocalName, pwszNamespaceUri);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITEXMLSTARTELEMENT, hResult);
	}

	void CXmlWriter_COM::WriteEndElement()
	{
		HRESULT hResult = m_pXMLWriter->WriteEndElement();
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITEXMLENDELEMENT, hResult);

	}
	
	void CXmlWriter_COM::WriteFullEndElement()
	{
		HRESULT hResult = m_pXMLWriter->WriteFullEndElement();
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITEXMLENDELEMENT, hResult);
	}

}
