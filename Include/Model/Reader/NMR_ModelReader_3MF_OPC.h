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

NMR_ModelReader_3MF_OPC.h implements the Model Reader Class.
A model reader reads in a 3MF file and generates an in-memory representation of it.
It uses the OPC Package functionality provided by all non-RT Windows systems.

--*/

#ifndef __NMR_MODELREADER_3MF_OPC
#define __NMR_MODELREADER_3MF_OPC

#include "Model/Reader/NMR_ModelReader_3MF.h" 
#include "Model/Reader/NMR_ModelReader.h"
#include "Model/Classes/NMR_Model.h"
#include "Common/Platform/NMR_IStream.h"
#include "Common/Platform/NMR_XmlReader.h"
#include "Common/Platform/NMR_CComPtr.h"

#include <list>
#include <Msopc.h>

namespace NMR {

	class CModelReader_3MF_OPC : public CModelReader_3MF {
	private:
		// Package Parts and RelationShips
		CComPtr<IOpcPartSet> m_pPackagePartSet;
		CComPtr<IOpcRelationshipSet> m_pPackageRelationshipSet;

		// Model Parts, RelationShips and XML Stream
		CComPtr<IOpcPart> m_pModelPart;
		CComPtr<IOpcRelationshipSet> m_pModelRelationshipSet;

	protected:
		CComPtr<IOpcRelationship> getSingleRelationShipByType(_In_ IOpcRelationshipSet * pRelationshipSet, _In_ LPCWSTR pszRelationshipType);
		CComPtr<IOpcPart> getRelationshipTargetPart(_In_ IOpcPartSet* pPartSet, _In_ IOpcRelationship * pRelationship, _In_opt_ LPCWSTR pszExpectedContentType);
		CComPtr<IOpcPart> getPartFromPackage(_In_ IOpcPartSet* pPartSet, _In_ IOpcRelationshipSet* pRelationshipSet, _In_ LPCWSTR pszRelationshipType, _In_opt_ LPCWSTR pszExpectedContentType);

		void extractModelDataFromRelationships(_In_ IOpcPartSet* pPartSet, _In_ IOpcRelationshipSet* pRelationshipSet);
		void extractTexturesFromRelationships(_In_ IOpcPartSet* pPartSet, _In_ IOpcRelationshipSet* pRelationshipSet);
		void extractCustomDataFromRelationships(_In_ IOpcPartSet* pPartSet, _In_ IOpcRelationshipSet* pRelationshipSet);
		void extractSliceDataFromRelationships(_In_ IOpcPartSet* pPartSet, _In_ IOpcRelationshipSet* pRelationshipSet);

		void extractModel();
		virtual PImportStream extract3MFOPCPackage(_In_ PImportStream pPackageStream);
		virtual void release3MFOPCPackage();

	public:
		CModelReader_3MF_OPC() = delete;
		CModelReader_3MF_OPC(_In_ PModel pModel);
	};

	typedef std::shared_ptr <CModelReader_3MF_OPC> PModelReader_3MF_OPC;

}

#endif // __NMR_MODELREADER_3MF_OPC
