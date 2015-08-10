/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

NMR_ModelWriter_3MF_OPC.cpp implements the 3MF Model Writer Class
for Win32. This model writer exports the in memory represenation into a 3MF file,
using the Windows OPC Library Functions.

--*/

#include "Model/Writer/NMR_ModelWriter_3MF_OPC.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/Platform/NMR_ExportStream_COM.h" 
#include "Common/Platform/NMR_ImportStream_COM.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/Platform/NMR_XmlWriter.h" 
#include <sstream>

namespace NMR {
	
	CModelWriter_3MF_OPC::CModelWriter_3MF_OPC(_In_ PModel pModel) : CModelWriter_3MF(pModel)
	{
		m_nRelationIDCounter = 0;
	}

	// These are OPC dependent functions
	void CModelWriter_3MF_OPC::createPackage(_In_ CModel * pModel)
	{
		__NMRASSERT(pModel != nullptr);

		HRESULT hResult;

		m_pPackagePartSet = nullptr;
		m_pPackageRelationshipSet = nullptr;
		m_pPackage = nullptr;
		m_pFactory = nullptr;

		// Create OPC Factory
		hResult = CoCreateInstance(__uuidof(OpcFactory), NULL, CLSCTX_INPROC_SERVER, __uuidof(IOpcFactory), (LPVOID*)&m_pFactory);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCFACTORYCREATEFAILED, hResult);

		// Create OPC Package
		hResult = m_pFactory->CreatePackage(&m_pPackage);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCPACKAGECREATEFAILED, hResult);

		// Read	OPC Package Part Set
		hResult = m_pPackage->GetPartSet(&m_pPackagePartSet);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCPARTSETREADFAILED, hResult);

		// Read OPC Relationship Set
		hResult = m_pPackage->GetRelationshipSet(&m_pPackageRelationshipSet);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPSETREADFAILED, hResult);

		CComPtr<IOpcPart> pModelPart = createOPCModelPart(PACKAGE_3D_MODEL_URI, PACKAGE_3D_MODEL_CONTENT_TYPE, pModel);
		addRelationship(m_pPackageRelationshipSet, PACKAGE_START_PART_RELATIONSHIP_TYPE, pModelPart);

		addTextureParts (pModel, pModelPart);
	}

	void CModelWriter_3MF_OPC::releasePackage()
	{
		m_pPackagePartSet = nullptr;
		m_pPackageRelationshipSet = nullptr;
		m_pPackage = nullptr;
		m_pFactory = nullptr;
	}

	void CModelWriter_3MF_OPC::writePackageToStream(_In_ PExportStream pStream)
	{
		__NMRASSERT(m_pPackage != nullptr);
		__NMRASSERT(m_pFactory != nullptr);
		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		// Cast to COM Streams
		CExportStream_COM * pCOMExportStream = dynamic_cast<CExportStream_COM *> (pStream.get());
		if (pCOMExportStream == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDSTREAMTYPE);
		CComPtr<IStream> pCOMStream = pCOMExportStream->getCOMStream();

		HRESULT hResult = m_pFactory->WritePackageToStream(m_pPackage, OPC_WRITE_DEFAULT, pCOMStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITEOPCPACKAGETOSTREAM, hResult);
	}

	CComPtr<IOpcPart> CModelWriter_3MF_OPC::createOPCPart(_In_ LPCWSTR pwszUriString, _In_ LPCWSTR pwszContentType)
	{
		__NMRASSERT(pwszUriString != nullptr);
		__NMRASSERT(pwszContentType != nullptr);
		__NMRASSERT(m_pFactory != nullptr);
		__NMRASSERT(m_pPackagePartSet != nullptr);
		HRESULT hResult;

		CComPtr<IOpcPartUri> pPartUri;
		hResult = m_pFactory->CreatePartUri(pwszUriString, &pPartUri);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTCREATEOPCPARTURI, hResult);

		CComPtr<IOpcPart> pPart;
		hResult = m_pPackagePartSet->CreatePart(pPartUri, pwszContentType, OPC_COMPRESSION_MAXIMUM, &pPart);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTCREATEOPCPART, hResult);

		CComPtr<IStream> pContentStream;
		hResult = pPart->GetContentStream(&pContentStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTGETCONTENTSTREAM, hResult);

		ULARGE_INTEGER nSize;
		nSize.HighPart = 0;
		nSize.LowPart = 0;
		hResult = pContentStream->SetSize(nSize);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTRESIZESTREAM, hResult);

		LARGE_INTEGER nPosition;
		nPosition.HighPart = 0;
		nPosition.LowPart = 0;
		hResult = pContentStream->Seek(nPosition, STREAM_SEEK_SET, nullptr);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTSEEKSTREAM, hResult);

		return pPart;
	}

	CComPtr<IOpcPart> CModelWriter_3MF_OPC::createOPCPartFromStream(_In_ LPCWSTR pwszUriString, _In_ LPCWSTR pwszContentType, _In_ IStream * pStream)
	{
		__NMRASSERT(pwszUriString != nullptr);
		__NMRASSERT(pwszContentType != nullptr);
		__NMRASSERT(pStream != nullptr);
		HRESULT hResult;

		CComPtr<IOpcPart> pPart = createOPCPart(pwszUriString, pwszContentType);
		CComPtr<IStream> pContentStream;
		hResult = pPart->GetContentStream(&pContentStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTGETCONTENTSTREAM, hResult);

		ULARGE_INTEGER nMaxSize;
		nMaxSize.HighPart = 0xffffffff;
		nMaxSize.LowPart = 0xffffffff;
		hResult = pStream->CopyTo(pContentStream, nMaxSize, nullptr, nullptr);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTCOPYSTREAM, hResult);

		return pPart;
	}

	CComPtr<IOpcPart> CModelWriter_3MF_OPC::createOPCModelPart(_In_ LPCWSTR pwszUriString, _In_ LPCWSTR pwszContentType, _In_ CModel * pModel)
	{
		__NMRASSERT(pwszUriString != nullptr);
		__NMRASSERT(pwszContentType != nullptr);
		__NMRASSERT(pModel != nullptr);
		HRESULT hResult;

		CComPtr<IOpcPart> pPart = createOPCPart(pwszUriString, pwszContentType);
		CComPtr<IStream> pContentStream;
		hResult = pPart->GetContentStream(&pContentStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTGETCONTENTSTREAM, hResult);

		PExportStream pExportStream = std::make_shared<CExportStream_COM>(pContentStream);
		writeModelStream(pExportStream, pModel);

		return pPart;
	}

	CComPtr<IOpcRelationship> CModelWriter_3MF_OPC::addRelationship(_In_ IOpcRelationshipSet * pRelationshipSet, _In_ LPCWSTR pwszRelationshipType, _In_ IOpcPart * pOPCPart)
	{
		__NMRASSERT(pRelationshipSet != nullptr);
		__NMRASSERT(pwszRelationshipType != nullptr);
		__NMRASSERT(pOPCPart != nullptr);
		HRESULT hResult;

		// Create Unique ID String
		std::wstringstream sStream;
		sStream << L"rel" << m_nRelationIDCounter;
		m_nRelationIDCounter++;
		std::wstring sID = sStream.str();

		// Create Uri
		CComPtr<IOpcPartUri> pPartUri;
		hResult = pOPCPart->GetName(&pPartUri);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTRETRIEVEOPCPARTNAME, hResult);

		// Create Relationship
		CComPtr<IOpcRelationship> pRelationship;
		hResult = pRelationshipSet->CreateRelationship(sID.c_str(), pwszRelationshipType, pPartUri, OPC_URI_TARGET_MODE_INTERNAL, &pRelationship);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTCREATEOPCRELATIONSHIP, hResult);

		return pRelationship;
	}

	void CModelWriter_3MF_OPC::addTextureParts(_In_ CModel * pModel, _In_ CComPtr<IOpcPart> pModelPart)
	{
		__NMRASSERT(pModel);
		__NMRASSERT(pModelPart != nullptr);

		nfUint32 nCount = pModel->getTextureStreamCount();
		nfUint32 nIndex;
		HRESULT hResult;

		if (nCount > 0) {

			CComPtr<IOpcRelationshipSet> pPartRelationShipSet;
			hResult = pModelPart->GetRelationshipSet(&pPartRelationShipSet);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPSETREADFAILED, hResult);

			for (nIndex = 0; nIndex < nCount; nIndex++) {
				PImportStream pStream = pModel->getTextureStream(nIndex);
				std::wstring sPath = pModel->getTextureStreamPath(nIndex);

				if (pStream.get() == nullptr)
					throw CNMRException(NMR_ERROR_INVALIDPARAM);

				// Seek to stream start
				pStream->seekPosition(0, true);

				CImportStream_COM * pCastedStream = dynamic_cast<CImportStream_COM *> (pStream.get());

				if (pCastedStream != nullptr){
					IStream * pCOMStream = pCastedStream->getCOMStream();
					CComPtr<IOpcPart> pTexturePart = createOPCPartFromStream(sPath.c_str(), PACKAGE_TEXTURE_CONTENT_TYPE, pCOMStream);
					addRelationship(pPartRelationShipSet, PACKAGE_TEXTURE_RELATIONSHIP_TYPE, pTexturePart);
				}
			}
		}

	}

}
