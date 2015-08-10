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

NMR_ModelReader_3MF_OPC.cpp implements the Model Reader Class. A model reader
reads in a 3MF file and generates an in-memory representation of it. It uses
the OPC Package functionality provided by all non-RT Windows systems.

--*/

#include "Model/Reader/NMR_ModelReader_3MF_OPC.h" 
#include "Model/Reader/NMR_ModelReaderNode_Model.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/Platform/NMR_ImportStream_COM.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 

namespace NMR {

	CModelReader_3MF_OPC::CModelReader_3MF_OPC(_In_ PModel pModel)
		: CModelReader_3MF(pModel)
	{
		// empty on purpose
	}

	void CModelReader_3MF_OPC::release3MFOPCPackage()
	{
		m_pPackagePartSet = nullptr;
		m_pPackageRelationshipSet = nullptr;
		m_pModelRelationshipSet = nullptr;
		m_pModelPart = nullptr;
	}

	PImportStream CModelReader_3MF_OPC::extract3MFOPCPackage(_In_ PImportStream pPackageStream)
	{
		HRESULT hResult;
		__NMRASSERT(pPackageStream != nullptr);

		// Cast to COM Streams
		CImportStream_COM * pCOMImportStream = dynamic_cast<CImportStream_COM *> (pPackageStream.get());
		if (pCOMImportStream == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDSTREAMTYPE);
		CComPtr<IStream> pCOMPackageStream = pCOMImportStream->getCOMStream ();

		// Define result model stream;
		CComPtr<IStream> pModelStream = nullptr;

		// Reset all values
		m_pPackagePartSet = nullptr;
		m_pPackageRelationshipSet = nullptr;
		m_pModelRelationshipSet = nullptr;
		m_pModelPart = nullptr;

		// Create OPC Factory
		CComPtr<IOpcFactory> pFactory;
		hResult = CoCreateInstance(__uuidof(OpcFactory), NULL, CLSCTX_INPROC_SERVER, __uuidof(IOpcFactory), (LPVOID*)&pFactory);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCFACTORYCREATEFAILED, hResult);

		// Read OPC Package
		CComPtr<IOpcPackage> pPackage;
		hResult = pFactory->ReadPackageFromStream(pCOMPackageStream, OPC_READ_DEFAULT, &pPackage);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCREADFAILED, hResult);

		// Read	OPC Package Part Set
		hResult = pPackage->GetPartSet(&m_pPackagePartSet);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCPARTSETREADFAILED, hResult);

		// Read OPC Relationship Set
		hResult = pPackage->GetRelationshipSet(&m_pPackageRelationshipSet);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPSETREADFAILED, hResult);

		// Extract Model from OPC Package
		__NMRASSERT(m_pPackagePartSet != nullptr);
		__NMRASSERT(m_pPackageRelationshipSet != nullptr);
		m_pModelPart = getPartFromPackage(m_pPackagePartSet, m_pPackageRelationshipSet, PACKAGE_START_PART_RELATIONSHIP_TYPE, PACKAGE_3D_MODEL_CONTENT_TYPE);

		// Extract Model Stream
		__NMRASSERT(m_pModelPart != nullptr);
		hResult = m_pModelPart->GetContentStream(&pModelStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTGETMODELSTREAM, hResult);

		// Read Model Relationships
		hResult = m_pModelPart->GetRelationshipSet(&m_pModelRelationshipSet);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_MODELRELATIONSHIPSETREADFAILED, hResult);

		extractTexturesFromRelationships(m_pPackagePartSet, m_pModelRelationshipSet); 

		return std::make_shared<CImportStream_COM>(pModelStream);
	}

	CComPtr<IOpcRelationship> CModelReader_3MF_OPC::getSingleRelationShipByType(_In_ IOpcRelationshipSet * pRelationshipSet, _In_ LPCWSTR pszRelationshipType)
	{
		__NMRASSERT(pRelationshipSet != nullptr);
		__NMRASSERT(pszRelationshipType != nullptr);
		HRESULT hResult;

		// Retrieve Enumerator
		CComPtr<IOpcRelationshipEnumerator> pEnumerator;
		hResult = pRelationshipSet->GetEnumeratorForType(pszRelationshipType, &pEnumerator);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPENUMERATIONFAILED, hResult);

		// Enumerate the first two relationships
		CComPtr<IOpcRelationship> pFirstRelationShip = nullptr;
		CComPtr<IOpcRelationship> pSecondRelationShip = nullptr;

		// Get the first relationship
		BOOL bHasFirst;
		hResult = pEnumerator->MoveNext(&bHasFirst);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPENUMERATIONFAILED, hResult);

		if (bHasFirst) {
			hResult = pEnumerator->GetCurrent(&pFirstRelationShip);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPENUMERATIONFAILED, hResult);

			// Get the first relationship
			BOOL bHasSecond;
			hResult = pEnumerator->MoveNext(&bHasSecond);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPENUMERATIONFAILED, hResult);

			if (bHasSecond) {
				hResult = pEnumerator->GetCurrent(&pSecondRelationShip);
				if (hResult != S_OK)
					throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPENUMERATIONFAILED, hResult);
			}
		}

		// We may only have exactly one relationship, otherwise it is ambigouus
		if (!pFirstRelationShip)
			throw CNMRException(NMR_ERROR_OPCRELATIONSHIPNOTFOUND);
		if (pSecondRelationShip)
			throw CNMRException(NMR_ERROR_OPCRELATIONSHIPNOTUNIQUE);

		return pFirstRelationShip;
	}

	CComPtr<IOpcPart> CModelReader_3MF_OPC::getRelationshipTargetPart(_In_ IOpcPartSet* pPartSet, _In_ IOpcRelationship * pRelationship, _In_opt_ LPCWSTR pszExpectedContentType)
	{
		__NMRASSERT(pPartSet != nullptr);
		__NMRASSERT(pRelationship != nullptr);
		HRESULT hResult;

		CComPtr<IOpcUri> pSourceURI;
		hResult = pRelationship->GetSourceUri(&pSourceURI);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPSOURCEURIFAILED, hResult);

		CComPtr<IUri> pTargetURI;
		hResult = pRelationship->GetTargetUri(&pTargetURI);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPTARGETURIFAILED, hResult);

		CComPtr<IOpcPartUri> pPartURI;
		hResult = pSourceURI->CombinePartUri(pTargetURI, &pPartURI);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPCOMBINEURIFAILED, hResult);

		CComPtr<IOpcPart> pPart;
		hResult = pPartSet->GetPart(pPartURI, &pPart);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPGETPARTFAILED, hResult);

		__NMRASSERT(pPart != nullptr);

		// Check optional content type match, if provided
		if (pszExpectedContentType) {
			wchar_t * pszContentType = nullptr;
			hResult = pPart->GetContentType(&pszContentType);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_OPCGETCONTENTTYPEFAILED, hResult);
			if (!pszContentType)
				throw CNMRException(NMR_ERROR_OPCGETCONTENTTYPEFAILED);

			nfBool bContentTypeIsValid = wcscmp(pszContentType, pszExpectedContentType) == 0;

			CoTaskMemFree(pszContentType);

			if (!bContentTypeIsValid)
				throw CNMRException(NMR_ERROR_OPCCONTENTTYPEMISMATCH);
		}

		return pPart;
	}

	CComPtr<IOpcPart> CModelReader_3MF_OPC::getPartFromPackage(_In_ IOpcPartSet* pPartSet, _In_ IOpcRelationshipSet* pRelationshipSet, _In_ LPCWSTR pszRelationshipType, _In_opt_ LPCWSTR pszExpectedContentType)
	{
		__NMRASSERT(pPartSet != nullptr);
		__NMRASSERT(pRelationshipSet != nullptr);
		__NMRASSERT(pszRelationshipType != nullptr);

		CComPtr<IOpcRelationship> pRelationship = getSingleRelationShipByType(pRelationshipSet, pszRelationshipType);
		return getRelationshipTargetPart(pPartSet, pRelationship, pszExpectedContentType);
	}

	void CModelReader_3MF_OPC::extractTexturesFromRelationships(_In_ IOpcPartSet* pPartSet, _In_ IOpcRelationshipSet* pRelationshipSet)
	{
		__NMRASSERT(pRelationshipSet != nullptr);
		HRESULT hResult;

		// Retrieve Enumerator
		CComPtr<IOpcRelationshipEnumerator> pEnumerator;
		hResult = pRelationshipSet->GetEnumeratorForType(PACKAGE_TEXTURE_RELATIONSHIP_TYPE, &pEnumerator);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPENUMERATIONFAILED, hResult);


		// Iterate through the relationships and retrieve textures
		BOOL bHasNext = true;
		while (bHasNext) {
			hResult = pEnumerator->MoveNext(&bHasNext);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPENUMERATIONFAILED, hResult);

			if (bHasNext) {

				CComPtr<IOpcRelationship> pRelationShip = nullptr;
				hResult = pEnumerator->GetCurrent(&pRelationShip);
				if (hResult != S_OK)
					throw CNMRException_Windows(NMR_ERROR_OPCRELATIONSHIPENUMERATIONFAILED, hResult);

				CComPtr<IOpcPart> pPart = getRelationshipTargetPart(m_pPackagePartSet, pRelationShip, nullptr);

				// Extract Model Stream
				CComPtr<IStream> pTextureStream = nullptr;
				__NMRASSERT(pPart != nullptr);
				hResult = pPart->GetContentStream(&pTextureStream);
				if (hResult != S_OK)
					throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTGETTEXTURESTREAM, hResult);

				// Create Import Stream for the texture
				PImportStream pImportStream = std::make_shared<CImportStream_COM>(pTextureStream);

				// Copy Stream of the texture in the memory
				PImportStream pCopiedStream = pImportStream->copyToMemory ();

				// Find out part URI
				CComPtr<IOpcPartUri> pPartUri;
				hResult = pPart->GetName(&pPartUri);
				if (hResult != S_OK)
					throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTGETTEXTUREURI, hResult);

				// Transform to absolute path
				wchar_t * pszAbsoluteUri = nullptr;
				hResult = pPartUri->GetAbsoluteUri(&pszAbsoluteUri);
				if (hResult != S_OK)
					throw CNMRException_Windows(NMR_ERROR_OPCCOULDNOTGETTEXTUREURI, hResult);
				std::wstring sPartUri(pszAbsoluteUri);
				SysFreeString(pszAbsoluteUri);

				// Add Texture Stream to Model
				addTextureStream(sPartUri, pCopiedStream);
			}

		}

	}


}
