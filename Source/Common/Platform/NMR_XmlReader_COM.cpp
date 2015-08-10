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

NMR_XmlReader_COM.cpp implements the XML Reader Class using XMLLite on Windows.

--*/

#include "Common/Platform/NMR_XmlReader_COM.h" 
#include "Common/Platform/NMR_ImportStream_COM.h" 
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CXmlReader_COM::CXmlReader_COM(_In_ PImportStream pImportStream)
		: CXmlReader(pImportStream)
	{
		HRESULT hResult;

		// Cast to COM Streams
		CImportStream_COM * pCOMImportStream = dynamic_cast<CImportStream_COM *> (pImportStream.get());
		if (pCOMImportStream == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDSTREAMTYPE);
		CComPtr<IStream> pCOMStream = pCOMImportStream->getCOMStream();

		// Create XML Reader
		hResult = CreateXmlReader(__uuidof(IXmlReader), (void**)&m_pXMLReader, nullptr);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_CREATEXMLREADERFAILED, hResult);

		// Set XML Reader Input
		hResult = m_pXMLReader->SetInput(pCOMStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_SETXMLREADERINPUTFAILED, hResult);

		// Set XML Reader Properties
		hResult = m_pXMLReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_SETXMLPROPERTIESFAILED, hResult);
	}

	void CXmlReader_COM::GetLocalName(_Outptr_result_buffer_maybenull_(*pcwchLocalName + 1) const nfWChar ** ppwszLocalName, _Out_opt_ nfUint32 *pcwchLocalName)
	{
		HRESULT hResult = m_pXMLReader->GetLocalName(ppwszLocalName, pcwchLocalName);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTGETLOCALXMLNAME, hResult);
	}

	void CXmlReader_COM::GetValue(_Outptr_result_buffer_maybenull_(*pcwchValue + 1)  const nfWChar ** ppwszValue, _Out_opt_  nfUint32 *pcwchValue)
	{
		HRESULT hResult = m_pXMLReader->GetValue(ppwszValue, pcwchValue);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTGETXMLTEXT, hResult);
	}

	void CXmlReader_COM::GetNamespaceURI(_Outptr_result_buffer_maybenull_(*pcwchValue + 1)  const nfWChar ** ppwszValue, _Out_opt_  nfUint32 *pcwchValue)
	{
		HRESULT hResult = m_pXMLReader->GetNamespaceUri(ppwszValue, pcwchValue);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTGETNAMESPACE, hResult);
	}

	nfBool CXmlReader_COM::Read(_Out_ eXmlReaderNodeType & NodeType)
	{
		XmlNodeType XmlLiteNodeType;
		NodeType = XMLREADERNODETYPE_UNKNOWN;

		HRESULT hResult = m_pXMLReader->Read(&XmlLiteNodeType);

		if (hResult == S_FALSE)
			return false;

		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_READXMLNODEFAILED, hResult);

		switch (XmlLiteNodeType) {
		case XmlNodeType_Element: NodeType = XMLREADERNODETYPE_STARTELEMENT; break;
		case XmlNodeType_EndElement: NodeType = XMLREADERNODETYPE_ENDELEMENT; break;
		case XmlNodeType_Text: NodeType = XMLREADERNODETYPE_TEXT; break;
		}

		return true;
	}

	nfBool CXmlReader_COM::IsEOF()
	{
		return m_pXMLReader->IsEOF() != 0;
	}

	nfBool CXmlReader_COM::IsEmptyElement()
	{
		return m_pXMLReader->IsEmptyElement() != 0;
	}

	nfBool CXmlReader_COM::MoveToFirstAttribute()
	{
		HRESULT hResult = m_pXMLReader->MoveToFirstAttribute();
		if (hResult == S_FALSE) // If there are no attributes, then return.
			return false;
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTGETXMLATTRIBUTES, hResult);

		return true;
	}

	nfBool CXmlReader_COM::MoveToNextAttribute()
	{
		HRESULT hResult = m_pXMLReader->MoveToNextAttribute();
		return (hResult == S_OK);
	}

	nfBool CXmlReader_COM::IsDefault()
	{
		return m_pXMLReader->IsDefault() != 0;
	}

}
