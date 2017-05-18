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

NMR_ModelReader_3MF_Native.cpp implements the Model Reader Class.
A model reader reads in a 3MF file and generates an in-memory representation of it.
It uses libzip and irrxml to parse the OPC package.

--*/

#include "Model/Reader/NMR_ModelReader_3MF_Native.h" 
#include "Model/Reader/NMR_ModelReaderNode_Model.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Model/Classes/NMR_ModelAttachment.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelReader_3MF_Native::CModelReader_3MF_Native(_In_ PModel pModel)
		: CModelReader_3MF(pModel)
	{
		// empty on purpose
	}

	PImportStream CModelReader_3MF_Native::extract3MFOPCPackage(_In_ PImportStream pPackageStream)
	{
		m_pPackageReader = std::make_shared<COpcPackageReader>(pPackageStream, m_pWarnings);

		COpcPackageRelationship * pModelRelation = m_pPackageReader->findRootRelation(PACKAGE_START_PART_RELATIONSHIP_TYPE, true);
		if (pModelRelation == nullptr)
			throw CNMRException(NMR_ERROR_OPCRELATIONSHIPSETREADFAILED);

		std::wstring sTargetPartURI = pModelRelation->getTargetPartURI();
		POpcPackagePart pModelPart = m_pPackageReader->createPart (sTargetPartURI);
		if (pModelPart == nullptr)
			throw CNMRException(NMR_ERROR_OPCCOULDNOTGETMODELSTREAM);

		m_pModel->setRootPath(sTargetPartURI.c_str());

		// calculate current level at this stage
		std::wstring sTargetPartURIDir = fnExtractFileDir(sTargetPartURI);
		
		extractTexturesFromRelationships(sTargetPartURIDir, pModelPart.get());
		extractCustomDataFromRelationships(sTargetPartURIDir, pModelPart.get());
		extractModelDataFromRelationships(sTargetPartURIDir, pModelPart.get());
		
		nfUint32 prodAttCount = m_pModel->getProductionAttachmentCount();
		for (nfUint32 i = 0; i < prodAttCount; i++)
		{
			PModelAttachment pProdAttachment = m_pModel->getProductionModelAttachment(i);
			std::wstring pathURI = pProdAttachment->getPathURI();
			POpcPackagePart pSubModelPart = m_pPackageReader->createPart(pathURI);
			extractModelDataFromRelationships(sTargetPartURIDir, pSubModelPart.get());
			extractTexturesFromRelationships(sTargetPartURIDir, pSubModelPart.get());
		}

		COpcPackageRelationship * pThumbnailRelation = m_pPackageReader->findRootRelation(PACKAGE_THUMBNAIL_RELATIONSHIP_TYPE, true);
		if (pThumbnailRelation != nullptr) {
			std::wstring sTargetPartURI = pThumbnailRelation->getTargetPartURI();
			POpcPackagePart pThumbnailPart = m_pPackageReader->createPart(sTargetPartURI);
			if (pThumbnailPart == nullptr)
				throw CNMRException(NMR_ERROR_OPCCOULDNOTGETTHUMBNAILSTREAM);
			PImportStream pThumbnailStream = pThumbnailPart->getImportStream()->copyToMemory();
			m_pModel->addPackageThumbnail()->setStream(pThumbnailStream);
		}
		
		return pModelPart->getImportStream();
	}
	
	void CModelReader_3MF_Native::release3MFOPCPackage()
	{
		m_pPackageReader = nullptr;
	}

	void CModelReader_3MF_Native::extractTexturesFromRelationships(_In_ std::wstring& sTargetPartURIDir, _In_ COpcPackagePart * pModelPart)
	{
		if (pModelPart == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::list<POpcPackageRelationship> RelationShips = pModelPart->getRelationShips();
		std::list<POpcPackageRelationship>::iterator iIterator;
		
		for (iIterator = RelationShips.begin(); iIterator != RelationShips.end(); iIterator++) {
			std::wstring sType = (*iIterator)->getType();
			if ( (wcscmp(sType.c_str(), PACKAGE_TEXTURE_RELATIONSHIP_TYPE) == 0) || 
				 (wcscmp(sType.c_str(), PACKAGE_THUMBNAIL_RELATIONSHIP_TYPE) == 0) ) // TODO: remove this
				 // this is to allow object thumbnails to come from the OPC Thumbnail relationship type
			{
				std::wstring sURI = (*iIterator)->getTargetPartURI();
				// TODO: this logic might not be correct yet
				if (!fnStartsWithPathDelimiter(sURI)) 
					sURI = sTargetPartURIDir + sURI;

				PModelAttachment pModelAttachment = m_pModel->findModelAttachment(sURI);
				if (!pModelAttachment) {
					POpcPackagePart pTexturePart = m_pPackageReader->createPart(sURI);
					PImportStream pTextureAttachmentStream = pTexturePart->getImportStream();
					PImportStream pMemoryStream = pTextureAttachmentStream->copyToMemory();

					if (pMemoryStream->retrieveSize() == 0)
						m_pWarnings->addException(CNMRException(NMR_ERROR_IMPORTSTREAMISEMPTY), mrwMissingMandatoryValue);

					// Add Texture Attachment to Model
					addTextureAttachment(sURI, pMemoryStream);
				}
			}
		}
	}


	void CModelReader_3MF_Native::extractCustomDataFromRelationships(_In_ std::wstring& sTargetPartURIDir, _In_ COpcPackagePart * pModelPart)
	{
		if (pModelPart == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::list<POpcPackageRelationship> RelationShips = pModelPart->getRelationShips();
		std::list<POpcPackageRelationship>::iterator iIterator;

		for (iIterator = RelationShips.begin(); iIterator != RelationShips.end(); iIterator++) {
			std::wstring sRelationShipType = (*iIterator)->getType();
			auto iRelationIterator = m_RelationsToRead.find(sRelationShipType);
			if (iRelationIterator != m_RelationsToRead.end()) {
				std::wstring sURI = (*iIterator)->getTargetPartURI();
				if (!fnStartsWithPathDelimiter(sURI))
					sURI = sTargetPartURIDir + sURI;
				POpcPackagePart pPart = m_pPackageReader->createPart(sURI);
				PImportStream pAttachmentStream = pPart->getImportStream();
				PImportStream pMemoryStream = pAttachmentStream->copyToMemory();
				
				if (pMemoryStream->retrieveSize() == 0)
					m_pWarnings->addException(CNMRException(NMR_ERROR_IMPORTSTREAMISEMPTY), mrwMissingMandatoryValue);

				// Add Attachment Stream to Model
				m_pModel->addAttachment(sURI, sRelationShipType, pMemoryStream);
			}
		}
	}
	
	void CModelReader_3MF_Native::extractModelDataFromRelationships(_In_ std::wstring& sTargetPartURIDir, _In_ COpcPackagePart * pModelPart)
	{
		if (pModelPart == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::list<POpcPackageRelationship> RelationShips = pModelPart->getRelationShips();
		std::list<POpcPackageRelationship>::iterator iIterator;

		for (iIterator = RelationShips.begin(); iIterator != RelationShips.end(); iIterator++) {
			std::wstring sRelationShipType = (*iIterator)->getType();
			if (wcscmp(sRelationShipType.c_str(), PACKAGE_START_PART_RELATIONSHIP_TYPE) == 0) {
				std::wstring sURI = (*iIterator)->getTargetPartURI();
				if (!fnStartsWithPathDelimiter(sURI))
					sURI = sTargetPartURIDir + sURI;
				POpcPackagePart pPart = m_pPackageReader->createPart(sURI);
				
				
				// first, check if this attachment already is in model
				PModelAttachment pModelAttachment = m_pModel->findProductionModelAttachment(sURI);
				if (pModelAttachment) {
					// this attachment is already read
					m_pModel->addProductionAttachment(sURI, sRelationShipType, pModelAttachment->getStream(), false);
				}
				else {
					// this is the first time this attachment is read
					PImportStream pAttachmentStream = pPart->getImportStream();
					PImportStream pMemoryStream = pAttachmentStream->copyToMemory();
					if (pMemoryStream->retrieveSize() == 0)
						m_pWarnings->addException(CNMRException(NMR_ERROR_IMPORTSTREAMISEMPTY), mrwMissingMandatoryValue);
					m_pModel->addProductionAttachment(sURI, sRelationShipType, pMemoryStream, true);
				}
			}
		}
	}
	

	void CModelReader_3MF_Native::checkContentTypes()
	{
		//throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);	
	}


}
