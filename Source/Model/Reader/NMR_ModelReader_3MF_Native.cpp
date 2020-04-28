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

NMR_ModelReader_3MF_Native.cpp implements the Model Reader Class.
A model reader reads in a 3MF file and generates an in-memory representation of it.
It uses libzip and irrxml to parse the OPC package.

--*/

#include "Model/Reader/NMR_ModelReader_3MF_Native.h" 
#include "Model/Reader/NMR_ModelReaderNode_ModelBase.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Model/Classes/NMR_ModelAttachment.h" 
#include "Model/Classes/NMR_KeyStore.h" 
#include "Common/NMR_SecureContext.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include "Common/Platform/NMR_Platform.h"
#include "Model/Reader/NMR_ModelReader_InstructionElement.h"

namespace NMR {

	CModelReader_3MF_Native::CModelReader_3MF_Native(_In_ PModel pModel)
		: CModelReader_3MF(pModel)
	{
		// empty on purpose
	}



	PImportStream CModelReader_3MF_Native::extract3MFOPCPackage(_In_ PImportStream pPackageStream)
	{
		m_pPackageReader = std::make_shared<CKeyStoreOpcPackageReader>(pPackageStream, *this);

		COpcPackageRelationship * pModelRelation = m_pPackageReader->findRootRelation(PACKAGE_START_PART_RELATIONSHIP_TYPE, true);
		if (pModelRelation == nullptr)
			throw CNMRException(NMR_ERROR_OPCRELATIONSHIPSETREADFAILED);

		std::string sTargetPartURI = pModelRelation->getTargetPartURI();
		POpcPackagePart pModelPart = m_pPackageReader->createPart(sTargetPartURI);
		if (pModelPart == nullptr)
			throw CNMRException(NMR_ERROR_OPCCOULDNOTGETMODELSTREAM);

		model()->setRootPath(sTargetPartURI.c_str());

		// calculate current level at this stage
		std::string sTargetPartURIDir = fnExtractFileDir(sTargetPartURI);
		
		extractTexturesFromRelationships(sTargetPartURIDir, pModelPart.get());
		extractCustomDataFromRelationships(sTargetPartURIDir, pModelPart.get());
		extractModelDataFromRelationships(sTargetPartURIDir, pModelPart.get());
		
		nfUint32 prodAttCount = model()->getProductionAttachmentCount();
		for (nfUint32 i = 0; i < prodAttCount; i++)
		{
			PModelAttachment pProdAttachment = model()->getProductionModelAttachment(i);
			std::string pathURI = pProdAttachment->getPathURI();
			POpcPackagePart pSubModelPart = m_pPackageReader->createPart(pathURI);
			extractModelDataFromRelationships(sTargetPartURIDir, pSubModelPart.get());
			extractTexturesFromRelationships(sTargetPartURIDir, pSubModelPart.get());
		}

		COpcPackageRelationship * pThumbnailRelation = m_pPackageReader->findRootRelation(PACKAGE_THUMBNAIL_RELATIONSHIP_TYPE, true);
		if (pThumbnailRelation != nullptr) {
			std::string sTargetPartURI = pThumbnailRelation->getTargetPartURI();
			POpcPackagePart pThumbnailPart = m_pPackageReader->createPart(sTargetPartURI);
			if (pThumbnailPart == nullptr)
				throw CNMRException(NMR_ERROR_OPCCOULDNOTGETTHUMBNAILSTREAM);
			PImportStream pThumbnailStream = pThumbnailPart->getImportStream()->copyToMemory();
			model()->addPackageThumbnail()->setStream(pThumbnailStream);
			monitor()->IncrementProgress((double)pThumbnailStream->retrieveSize());
			monitor()->ReportProgressAndQueryCancelled(true);
		}
		
		return pModelPart->getImportStream();
	}
	
	void CModelReader_3MF_Native::release3MFOPCPackage()
	{
		//foreach part, finalize encryption contexts
		m_pPackageReader->close();
		m_pPackageReader = nullptr;
	}

	void CModelReader_3MF_Native::extractTexturesFromRelationships(_In_ std::string& sTargetPartURIDir, _In_ COpcPackagePart * pModelPart)
	{
		if (pModelPart == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		monitor()->SetProgressIdentifier(PROGRESS_READTEXTURETACHMENTS);

		std::multimap<std::string, POpcPackageRelationship>& RelationShips = pModelPart->getRelationShips();
		
		for (auto iIterator = RelationShips.begin(); iIterator != RelationShips.end(); iIterator++) {
			auto theRelationShip = iIterator->second;
			std::string sType = theRelationShip->getType();

			if ( (sType == PACKAGE_TEXTURE_RELATIONSHIP_TYPE) || 
				 (sType == PACKAGE_THUMBNAIL_RELATIONSHIP_TYPE) )
				 // this is to allow object thumbnails to come from both
				 // the Texture or the OPC Thumbnail relationship type
				 // to comply with older versions of the 3mf specification
			{
				std::string sURI = theRelationShip->getTargetPartURI();
				// TODO: this logic might not be correct yet
				if (!fnStartsWithPathDelimiter(sURI))
					sURI = sTargetPartURIDir + sURI;

				PModelAttachment pModelAttachment = model()->findModelAttachment(sURI);
				if (!pModelAttachment) {
					POpcPackagePart pTexturePart = m_pPackageReader->createPart(sURI);
					PImportStream pTextureAttachmentStream = pTexturePart->getImportStream();
					PImportStream pMemoryStream = pTextureAttachmentStream->copyToMemory();

					if (pMemoryStream->retrieveSize() == 0)
						warnings()->addException(CNMRException(NMR_ERROR_IMPORTSTREAMISEMPTY), mrwMissingMandatoryValue);

					// Add Texture Attachment to Model
					addTextureAttachment(sURI, pMemoryStream);

					monitor()->IncrementProgress((double)pMemoryStream->retrieveSize());
					monitor()->ReportProgressAndQueryCancelled(true);
				}
			}
		}
	}


	void CModelReader_3MF_Native::extractCustomDataFromRelationships(_In_ std::string& sTargetPartURIDir, _In_ COpcPackagePart * pModelPart)
	{
		if (pModelPart == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		monitor()->SetProgressIdentifier(PROGRESS_READCUSTOMATTACHMENTS);

		std::multimap<std::string, POpcPackageRelationship>& RelationShips = pModelPart->getRelationShips();

		for (auto iIterator = RelationShips.begin(); iIterator != RelationShips.end(); iIterator++) {
			auto theRelationShip = iIterator->second;

			std::string sRelationShipType = theRelationShip->getType();
			std::string sURI = theRelationShip->getTargetPartURI();

			if (!fnStartsWithPathDelimiter(sURI))
				sURI = sTargetPartURIDir + sURI;

			auto iRelationIterator = m_RelationsToRead.find(sRelationShipType);
			if (iRelationIterator != m_RelationsToRead.end()) {
				POpcPackagePart pPart = m_pPackageReader->createPart(sURI);
				PImportStream pAttachmentStream = pPart->getImportStream();
				try {
					PImportStream pMemoryStream = pAttachmentStream->copyToMemory();

					if (pMemoryStream->retrieveSize() == 0)
						warnings()->addException(CNMRException(NMR_ERROR_IMPORTSTREAMISEMPTY), mrwMissingMandatoryValue);

					// Add Attachment Stream to Model
					model()->addAttachment(sURI, sRelationShipType, pMemoryStream);

					monitor()->IncrementProgress((double)pMemoryStream->retrieveSize());
					monitor()->ReportProgressAndQueryCancelled(true);
				}
				catch (CNMRException &e) {
					if (e.getErrorCode() == NMR_ERROR_INVALIDBUFFERSIZE)
						warnings()->addException(CNMRException(NMR_ERROR_ATTACHMENTTOOLARGE), mrwMissingMandatoryValue);
					else
						throw;
				}
			}
			else {
				if (   (sRelationShipType != PACKAGE_START_PART_RELATIONSHIP_TYPE)
					&& (sRelationShipType != PACKAGE_TEXTURE_RELATIONSHIP_TYPE)
					&& (sRelationShipType != PACKAGE_THUMBNAIL_RELATIONSHIP_TYPE) )
				{
					monitor()->DecreaseMaxProgress((double)m_pPackageReader->getPartSize(sURI));
					monitor()->ReportProgressAndQueryCancelled(true);
				}
			}
		}
	}
	
	void CModelReader_3MF_Native::extractModelDataFromRelationships(_In_ std::string& sTargetPartURIDir, _In_ COpcPackagePart * pModelPart)
	{
		if (pModelPart == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::multimap<std::string, POpcPackageRelationship>& RelationShips = pModelPart->getRelationShips();

		for (auto iIterator = RelationShips.begin(); iIterator != RelationShips.end(); iIterator++) {
			auto theRelationShip = iIterator->second;

			std::string sRelationShipType = theRelationShip->getType();
			
			if (sRelationShipType == PACKAGE_START_PART_RELATIONSHIP_TYPE) {
				std::string sURI = theRelationShip->getTargetPartURI();
				if (!fnStartsWithPathDelimiter(sURI))
					sURI = sTargetPartURIDir + sURI;

				POpcPackagePart pPart = m_pPackageReader->createPart(sURI);

				// first, check if this attachment already is in model
				PModelAttachment pModelAttachment = model()->findProductionModelAttachment(sURI);
				if (pModelAttachment) {
					// this attachment is already read
					model()->addProductionAttachment(sURI, sRelationShipType, pModelAttachment->getStream(), false);
				}
				else {
					// this is the first time this attachment is read
					PImportStream pAttachmentStream = pPart->getImportStream();
					PImportStream pMemoryStream = pAttachmentStream->copyToMemory();
					if (pMemoryStream->retrieveSize() == 0)
						warnings()->addException(CNMRException(NMR_ERROR_IMPORTSTREAMISEMPTY), mrwMissingMandatoryValue);
					model()->addProductionAttachment(sURI, sRelationShipType, pMemoryStream, true);
				}
			}
		}
	}
	

	void CModelReader_3MF_Native::checkContentTypes()
	{
		//throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);	
	}


}
