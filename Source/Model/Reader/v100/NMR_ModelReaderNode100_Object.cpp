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

NMR_ModelReaderNode100_Object.cpp implements the Model Reader Object Node Class.
A object reader model node is a parser for the object node of an XML Model
Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Object.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Mesh.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_MetaData.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Components.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelAttachment.h"

#include "Model/Classes/NMR_ModelDefaultProperty.h"
#include "Model/Classes/NMR_ModelDefaultProperty_BaseMaterial.h"
#include "Model/Classes/NMR_ModelDefaultProperty_Color.h"
#include "Model/Classes/NMR_ModelDefaultProperty_TexCoord2D.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_Object::CModelReaderNode100_Object(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings, _In_ CProgressMonitor * pProgressMonitor, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelReader_TexCoordMapping pTexCoordMapping)
		: CModelReaderNode(pWarnings, pProgressMonitor)
	{
		// Initialize variables
		if (!pColorMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (!pTexCoordMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_nID = 0;
		m_sType = "";
		m_bHasType = false;

		m_pModel = pModel;
		m_pObject = NULL; 
		m_sThumbnail = "";
		m_sPartNumber = "";
		m_sName = "";

		m_bHasThumbnail = false;
		m_bHasDefaultPropertyID = false;
		m_bHasDefaultPropertyIndex = false;
		m_nDefaultPropertyID = 0;
		m_nDefaultPropertyIndex = 0;

		m_pColorMapping = pColorMapping;
		m_pTexCoordMapping = pTexCoordMapping;
		m_nSliceStackId = 0;
		m_eSlicesMeshResolution = MODELSLICESMESHRESOLUTION_FULL;
		m_bHasMeshResolution = false;
	}

	void CModelReaderNode100_Object::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Use parameter and assign to model Object
		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELOBJECTID);

		// Parse Content
		parseContent(pXMLReader);

		// Check, if we have created an object
		if (!m_pObject.get())
			throw CNMRException(NMR_ERROR_MISSINGOBJECTCONTENT);

		// Set Object Parameters
		m_pObject->setName(m_sName);
		m_pObject->setPartNumber(m_sPartNumber);
		
		if (m_bHasThumbnail)
		{
			PModelAttachment pAttachment = m_pModel->findModelAttachment(m_sThumbnail);
			if (pAttachment) {
				if (!(pAttachment->getRelationShipType() == PACKAGE_TEXTURE_RELATIONSHIP_TYPE))
					m_pWarnings->addException(CNMRException(NMR_ERROR_NOTEXTURESTREAM), mrwInvalidMandatoryValue);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NOTEXTURESTREAM), mrwInvalidMandatoryValue);

			m_pObject->setThumbnail(m_sThumbnail);
		}

		// Set Production references
		if (!m_UUID.get()) {
			if (pXMLReader->NamespaceRegistered(XML_3MF_NAMESPACE_PRODUCTIONSPEC)) {
				m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGUUID), mrwMissingMandatoryValue);
			}
			m_UUID = std::make_shared<CUUID>();
		}
		m_pObject->setUUID(m_UUID);
	}

	void CModelReaderNode100_Object::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEOBJECTID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_TYPE) == 0) {
			if (m_bHasType)
				throw CNMRException(NMR_ERROR_DUPLICATEOBJECTTYPE);

			// Convert to integer and make a input and range check!
			m_sType = pAttributeValue;
			m_bHasType = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_THUMBNAIL) == 0) {
			if (m_bHasThumbnail)
				throw CNMRException(NMR_ERROR_DUPLICATEOBJECTTHUMBNAIL);
			m_sThumbnail = pAttributeValue;
			m_bHasThumbnail = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_NAME) == 0) {
			m_sName = pAttributeValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_PARTNUMBER) == 0) {
			m_sPartNumber = pAttributeValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_PID) == 0) {
			if (m_bHasDefaultPropertyID)
				throw CNMRException(NMR_ERROR_DUPLICATEPID);
			m_bHasDefaultPropertyID = true;
			m_nDefaultPropertyID = fnStringToUint32(pAttributeValue);
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_PINDEX) == 0) {
			if (m_bHasDefaultPropertyIndex)
				throw CNMRException(NMR_ERROR_DUPLICATEPINDEX);
			m_bHasDefaultPropertyIndex = true;
			m_nDefaultPropertyIndex = fnStringToUint32(pAttributeValue);
		}

	}

	void CModelReaderNode100_Object::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace) {
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pNameSpace);
		__NMRASSERT(pAttributeValue);

		if (strcmp(XML_3MF_NAMESPACE_SLICESPEC, pNameSpace) == 0) {
			if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_SLICESTACKID) == 0) {
				if (m_nSliceStackId != 0)
					m_pWarnings->addException(CNMRException(NMR_ERROR_DUPLICATE_SLICESTACKID), eModelReaderWarningLevel::mrwInvalidOptionalValue);
				m_nSliceStackId = fnStringToUint32(pAttributeValue);
			}
			else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_MESHRESOLUTION) == 0) {
				if (m_bHasMeshResolution) 
					m_pWarnings->addException(CNMRException(NMR_ERROR_DUPLICATE_MESHRESOLUTION), eModelReaderWarningLevel::mrwInvalidOptionalValue);
				m_bHasMeshResolution = true;
				if (strcmp(pAttributeValue, XML_3MF_VALUE_OBJECT_MESHRESOLUTION_FULL)==0) {
					m_eSlicesMeshResolution = MODELSLICESMESHRESOLUTION_FULL;
				}
				else if (strcmp(pAttributeValue, XML_3MF_VALUE_OBJECT_MESHRESOLUTION_LOW) == 0) {
					m_eSlicesMeshResolution = MODELSLICESMESHRESOLUTION_LOW;
				}
				else
					m_pWarnings->addException(CNMRException(NMR_ERROR_INVALID_MESHRESOLUTION), eModelReaderWarningLevel::mrwInvalidOptionalValue);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
		}
		

		if (strcmp(XML_3MF_NAMESPACE_PRODUCTIONSPEC, pNameSpace) == 0) {
			if (strcmp(XML_3MF_PRODUCTION_UUID, pAttributeName) == 0) {
				if (m_UUID.get())
					m_pWarnings->addException(CNMRException(NMR_ERROR_DUPLICATEUUID), eModelReaderWarningLevel::mrwInvalidMandatoryValue);
				m_UUID = std::make_shared<CUUID>(pAttributeValue);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
		}
	}

	void CModelReaderNode100_Object::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {
			// Read a mesh object
			if (strcmp(pChildName, XML_3MF_ELEMENT_MESH) == 0) {
				// If we already have parsed an object, the node is duplicate
				if (m_pObject.get())
					throw CNMRException(NMR_ERROR_AMBIGUOUSOBJECTDEFINITON);

				// Create Empty Mesh
				PMesh pMesh = std::make_shared<CMesh>();
				// Create Mesh Object
				m_pObject = std::make_shared<CModelMeshObject>(m_nID, m_pModel, pMesh);
				// Set Object Type (might fail, if string is invalid)
				if (m_bHasType) {
					if (!m_pObject->setObjectTypeString(m_sType, false))
						m_pWarnings->addWarning(MODELREADERWARNING_INVALIDMODELOBJECTTYPE, NMR_ERROR_INVALIDMODELOBJECTTYPE, mrwInvalidOptionalValue);
				}
				

				if (m_pProgressMonitor)
					m_pProgressMonitor->PushLevel(0, 1);
				// Read Mesh
				PModelReaderNode100_Mesh pXMLNode = std::make_shared<CModelReaderNode100_Mesh>(m_pModel, pMesh.get(), m_pWarnings, m_pProgressMonitor, m_pColorMapping, m_pTexCoordMapping, m_nDefaultPropertyID, m_nDefaultPropertyIndex);
				pXMLNode->parseXML(pXMLReader);
				if (m_pProgressMonitor)
					m_pProgressMonitor->PopLevel();

				// Add Object to Parent
				m_pModel->addResource(m_pObject);

				// Handle BeamLattice Data
				handleBeamLatticeExtension(pXMLNode.get());

				// Create Default Properties
				createDefaultProperties();
			}
			// Read a component object
			else if (strcmp(pChildName, XML_3MF_ELEMENT_COMPONENTS) == 0) {
				// If we already have parsed an object, the node is duplicate
				if (m_pObject.get())
					throw CNMRException(NMR_ERROR_AMBIGUOUSOBJECTDEFINITON);

				// Create Component List Object
				PModelComponentsObject pComponentsObject = std::make_shared<CModelComponentsObject>(m_nID, m_pModel);
				m_pObject = pComponentsObject;
				// Set Object Type (might fail, if string is invalid)
				if (m_bHasType) {
					if (!m_pObject->setObjectTypeString(m_sType, false))
						m_pWarnings->addWarning(MODELREADERWARNING_INVALIDMODELOBJECTTYPE, NMR_ERROR_INVALIDMODELOBJECTTYPE, mrwInvalidOptionalValue);
				}

				// Read Components
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Components>(pComponentsObject.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Add Object to Parent
				m_pModel->addResource(m_pObject);
				
				if (m_nDefaultPropertyID != 0)
					m_pWarnings->addException(CNMRException(NMR_ERROR_DEFAULTPID_ON_COMPONENTSOBJECT), mrwInvalidOptionalValue);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);

			// In any case (component object or mesh object)
			if (m_nSliceStackId > 0) {
				PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->curPath(), m_nSliceStackId);
				if (!pID.get())
					throw CNMRException(NMR_ERROR_SLICESTACKRESOURCE_NOT_FOUND);
				PModelResource pResource = m_pModel->findResource(pID->getUniqueID());
				CModelSliceStackResource* pSliceStackResource = dynamic_cast<CModelSliceStackResource*>(pResource.get());
				if (pSliceStackResource) {
					if ((m_pObject->getObjectType() == MODELOBJECTTYPE_MODEL) || (MODELOBJECTTYPE_SOLIDSUPPORT)) {
						if (!pSliceStackResource->getSliceStack()->areAllPolygonsClosed()) {
							throw CNMRException(NMR_ERROR_SLICEPOLYGONNOTCLOSED);
						}
					}
				}
				else
					throw CNMRException(NMR_ERROR_SLICESTACKRESOURCE_NOT_FOUND);
				
				m_pObject->setSliceStackId(pID);
				m_pObject->setSlicesMeshResolution(m_eSlicesMeshResolution);
			}

		}

	}

	// Create the default property from m_nDefaultPropertyID, if defined
	void CModelReaderNode100_Object::createDefaultProperties()
	{
		if (m_pObject.get() == nullptr)
			return;

		if (m_nDefaultPropertyID != 0) {
			bool hasBeenSet = false;
			// Assign Default Resource Property
			PPackageResourceID pRID = m_pModel->findPackageResourceID(m_pModel->curPath(), m_nDefaultPropertyID);

			CModelBaseMaterialResource * pResource = nullptr;
			if (pRID.get())
				pResource = m_pModel->findBaseMaterial(pRID->getUniqueID());
			if (pResource != nullptr) {
				m_pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_BaseMaterial>(pRID->getUniqueID(), m_nDefaultPropertyIndex));
				hasBeenSet = true;
			}

			// Assign Default ColorResource Property
			nfColor cColor;
			if (m_pColorMapping->findColor(m_nDefaultPropertyID, m_nDefaultPropertyIndex, cColor)) {
				m_pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_Color>(cColor));
				hasBeenSet = true;
			}
			
			// Assign Default TextureResource Property
			ModelResourceID nTextureID;
			nfFloat fU;
			nfFloat fV;
			if (m_pTexCoordMapping->findTexCoords(m_nDefaultPropertyID, m_nDefaultPropertyIndex, nTextureID, fU, fV)) {
				if (nTextureID != 0) {
					PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->curPath(), nTextureID);
					if (pID.get()) {
						m_pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_TexCoord2D>(pID->getUniqueID(), fU, fV));
						hasBeenSet = true;
					}
				}
			}

			if (!hasBeenSet) {
				m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGDEFAULTPID), mrwMissingMandatoryValue);
			}
		}
	}

	void CModelReaderNode100_Object::handleBeamLatticeExtension(CModelReaderNode100_Mesh* pXMLNode)
	{
		CModelMeshObject* pMeshObject = dynamic_cast<CModelMeshObject*>(m_pObject.get());
		if (pMeshObject == nullptr || pXMLNode == nullptr)
			return;

		if (pMeshObject->getMesh()->getBeamCount() > 0) {
			if ( (pMeshObject->getObjectType() != MODELOBJECTTYPE_MODEL) && (pMeshObject->getObjectType() != MODELOBJECTTYPE_SOLIDSUPPORT)) {
				m_pWarnings->addException(CNMRException(NMR_ERROR_BEAMLATTICE_INVALID_OBJECTTYPE), mrwInvalidOptionalValue);
			}
		}

		eModelBeamLatticeClipMode eClipMode;
		nfBool bHasClippingMeshID;
		ModelResourceID nClippingMeshID;

		pXMLNode->retrieveClippingInfo(eClipMode, bHasClippingMeshID, nClippingMeshID);
		if (bHasClippingMeshID) {
			PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->curPath(), nClippingMeshID);
			if (!pID.get()) {
				m_pWarnings->addWarning(MODELREADERWARNING_BEAMLATTICECLIPPINGRESOURCENOTDEFINED, NMR_ERROR_BEAMLATTICECLIPPINGRESOURCENOTDEFINED, mrwInvalidMandatoryValue);
			}
			else {
				CModelObject * pModelObject = m_pModel->findObject(pID->getUniqueID());
				if (pModelObject) {
					pMeshObject->getBeamLatticeAttributes()->m_bHasClippingMeshID = bHasClippingMeshID;
					pMeshObject->getBeamLatticeAttributes()->m_nClippingMeshID = pID;
					pMeshObject->getBeamLatticeAttributes()->m_eClipMode = eClipMode;
				}
				else {
					pMeshObject->getBeamLatticeAttributes()->m_bHasClippingMeshID = false;
					m_pWarnings->addWarning(MODELREADERWARNING_BEAMLATTICECLIPPINGRESOURCENOTDEFINED, NMR_ERROR_BEAMLATTICECLIPPINGRESOURCENOTDEFINED, mrwInvalidMandatoryValue);
				}
			}
		}

		nfBool bHasRepresentationMeshID;
		ModelResourceID nRepresentationMeshID;
		pXMLNode->retrieveRepresentationInfo(bHasRepresentationMeshID, nRepresentationMeshID);
		if (nRepresentationMeshID) {
			PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->curPath(), nRepresentationMeshID);
			if (!pID.get()) {
				m_pWarnings->addWarning(MODELREADERWARNING_BEAMLATTICEREPRESENTATIONRESOURCENOTDEFINED, NMR_ERROR_BEAMLATTICE_INVALID_REPRESENTATIONRESOURCE, mrwInvalidMandatoryValue);
			}
			else {
				CModelObject * pModelObject = m_pModel->findObject(pID->getUniqueID());
				if (pModelObject) {
					pMeshObject->getBeamLatticeAttributes()->m_bHasRepresentationMeshID = bHasRepresentationMeshID;
					pMeshObject->getBeamLatticeAttributes()->m_nRepresentationID = pID;
				}
				else {
					pMeshObject->getBeamLatticeAttributes()->m_bHasRepresentationMeshID = false;
					m_pWarnings->addWarning(MODELREADERWARNING_BEAMLATTICEREPRESENTATIONRESOURCENOTDEFINED, NMR_ERROR_BEAMLATTICE_INVALID_REPRESENTATIONRESOURCE, mrwInvalidMandatoryValue);
				}
			}
		}
	}

}
