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

NMR_ModelWriterNode100_Model.cpp implements the Model Writer Model Node Class.
This is the class for exporting the 3mf model stream root node.

--*/

#include "Model/Writer/v100/NMR_ModelWriterNode100_Mesh.h"
#include "Model/Writer/v100/NMR_ModelWriterNode100_Model.h"

#include "Model/Classes/NMR_ModelAttachment.h"
#include "Model/Classes/NMR_ModelBuildItem.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelBaseMaterials.h"
#include "Model/Classes/NMR_ModelColorGroup.h"
#include "Model/Classes/NMR_ModelTexture2D.h"
#include "Model/Classes/NMR_ModelTexture2DGroup.h"
#include "Model/Classes/NMR_ModelCompositeMaterials.h"
#include "Model/Classes/NMR_ModelMultiPropertyGroup.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_Model.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"
#include "Model/Classes/NMR_ModelConstants_Slices.h"

#include "Common/3MF_ProgressMonitor.h"


namespace NMR {

	CModelWriterNode100_Model::CModelWriterNode100_Model(_In_ CModel * pModel, _In_ CXmlWriter * pXMLWriter, _In_ PProgressMonitor pProgressMonitor,
		_In_ nfUint32 nDecimalPrecision, nfBool bWritesRootModel) : CModelWriterNode_ModelBase(pModel, pXMLWriter, pProgressMonitor), m_nDecimalPrecision(nDecimalPrecision)
	{
		m_pPropertyIndexMapping = std::make_shared<CMeshInformation_PropertyIndexMapping>();
		m_bIsRootModel = bWritesRootModel;

		m_bWriteMaterialExtension = true;
		m_bWriteProductionExtension = true;
		m_bWriteBeamLatticeExtension = true;
		m_bWriteSliceExtension = true;
		m_bWriteSecureContentExtension = true;
		m_bWriteBaseMaterials = true;
		m_bWriteObjects = true;

		m_bWriteCustomNamespaces = true;

		// register custom NameSpaces from metadata in objects, build items and the model itself
		RegisterMetaDataNameSpaces();
	}

	void CModelWriterNode100_Model::RegisterMetaDataGroupNameSpaces(PModelMetaDataGroup mdg)
	{
		for (nfUint32 i = 0; i < mdg->getMetaDataCount(); i++)
		{
			PModelMetaData md = mdg->getMetaData(i);
			if (!md->getNameSpace().empty()) {
				std::string prefix = "customXMLNS" + std::to_string(m_pXMLWriter->GetNamespaceCount());
				std::string sDummy;
				if (!m_pXMLWriter->GetNamespacePrefix(md->getNameSpace(), sDummy)) {
					m_pXMLWriter->RegisterCustomNameSpace(md->getNameSpace(), prefix);
				}
			}
		}
	}

	void CModelWriterNode100_Model::RegisterMetaDataNameSpaces()
	{
		RegisterMetaDataGroupNameSpaces(m_pModel->getMetaDataGroup());

		nfInt32 nObjectCount = m_pModel->getObjectCount();
		for (nfInt32 nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex++) {
			PModelResource pResource = m_pModel->getObjectResource(nObjectIndex);
			CModelObject * pObject = dynamic_cast<CModelObject*> (pResource.get());
			if (pObject) {
				RegisterMetaDataGroupNameSpaces(pObject->metaDataGroup());
			}
		}

		nfInt32 nOBuildItemCount = m_pModel->getBuildItemCount();
		for (nfInt32 nBuildItemIndex = 0; nBuildItemIndex < nOBuildItemCount; nBuildItemIndex++) {
			PModelBuildItem pBuildItem = m_pModel->getBuildItem(nBuildItemIndex);
			RegisterMetaDataGroupNameSpaces(pBuildItem->metaDataGroup());
		}
	}

	void CModelWriterNode100_Model::writeToXML()
	{
		std::string sLanguage = m_pModel->getLanguage();

		writeStartElementWithNamespace(XML_3MF_ELEMENT_MODEL, PACKAGE_XMLNS_100);

		writeStringAttribute(XML_3MF_ATTRIBUTE_MODEL_UNIT, m_pModel->getUnitString());
		writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_PREFIX_XML, XML_3MF_ATTRIBUTE_MODEL_LANG, sLanguage.c_str());

		std::string sRequiredExtensions = "";
		if (m_bWriteMaterialExtension) {
			writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_XMLNS, XML_3MF_NAMESPACEPREFIX_MATERIAL, XML_3MF_NAMESPACE_MATERIALSPEC);
		}
		if (m_bWriteProductionExtension) {
			writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_XMLNS, XML_3MF_NAMESPACEPREFIX_PRODUCTION, XML_3MF_NAMESPACE_PRODUCTIONSPEC);
			if (m_pModel->RequireExtension(XML_3MF_NAMESPACEPREFIX_PRODUCTION)) {
				if (sRequiredExtensions.size() > 0)
					sRequiredExtensions = sRequiredExtensions + " ";
				sRequiredExtensions = sRequiredExtensions + XML_3MF_NAMESPACE_PRODUCTIONSPEC;
			}
		}
		if (m_bWriteBeamLatticeExtension) {
			writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_XMLNS, XML_3MF_NAMESPACEPREFIX_BEAMLATTICE, XML_3MF_NAMESPACE_BEAMLATTICESPEC);

			if (m_pModel->RequireExtension(XML_3MF_NAMESPACE_BEAMLATTICESPEC)) {
				if (sRequiredExtensions.size() > 0)
					sRequiredExtensions = sRequiredExtensions + " ";
				sRequiredExtensions = sRequiredExtensions + XML_3MF_NAMESPACEPREFIX_BEAMLATTICE;
			}
		}

		if (m_bWriteSliceExtension) {
			writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_XMLNS, XML_3MF_NAMESPACEPREFIX_SLICE, XML_3MF_NAMESPACE_SLICESPEC);
			if (m_pModel->RequireExtension(XML_3MF_NAMESPACE_SLICESPEC)) {
				if (sRequiredExtensions.size() > 0)
					sRequiredExtensions = sRequiredExtensions + " ";
				sRequiredExtensions = sRequiredExtensions + XML_3MF_NAMESPACEPREFIX_SLICE;
			}
		}

		if (m_bWriteSecureContentExtension) {
			writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_XMLNS, XML_3MF_NAMESPACEPREFIX_SECURECONTENT, XML_3MF_NAMESPACE_SECURECONTENTSPEC);
			if (m_pModel->RequireExtension(XML_3MF_NAMESPACE_SECURECONTENTSPEC)) {
				if (sRequiredExtensions.size() > 0)
					sRequiredExtensions = sRequiredExtensions + " ";
				sRequiredExtensions = sRequiredExtensions + XML_3MF_NAMESPACEPREFIX_SECURECONTENT;
			}
		}

		if (m_bWriteCustomNamespaces) {
			nfUint32 nNSCount = m_pXMLWriter->GetNamespaceCount();
			for (nfUint32 iNSCount = 0; iNSCount < nNSCount; iNSCount++) {
				writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_XMLNS, m_pXMLWriter->GetNamespacePrefix(iNSCount).c_str(), m_pXMLWriter->GetNamespace(iNSCount).c_str());
			}
		}

		if (sRequiredExtensions.size() > 0)
			writeConstStringAttribute(XML_3MF_ATTRIBUTE_REQUIREDEXTENSIONS, sRequiredExtensions.c_str());

		if (m_bIsRootModel)
			writeModelMetaData();

		writeResources();
		writeBuild();

		writeFullEndElement();
	}

	void CModelWriterNode100_Model::writeTextures2D()
	{
		nfUint32 nTextureCount = m_pModel->getTexture2DCount();
		nfUint32 nTextureIndex;

		for (nTextureIndex = 0; nTextureIndex < nTextureCount; nTextureIndex++) {
			m_pProgressMonitor->IncrementProgress(1);

			CModelTexture2DResource * pTexture2D = m_pModel->getTexture2D(nTextureIndex);

			writeStartElementWithPrefix(XML_3MF_ELEMENT_TEXTURE2D, XML_3MF_NAMESPACEPREFIX_MATERIAL);

			assertResourceIsInCurrentPath(pTexture2D->getPackageResourceID());
			writeIntAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_ID, pTexture2D->getPackageResourceID()->getModelResourceID());
			writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_PATH, pTexture2D->getAttachment()->getPathURI());
			writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_CONTENTTYPE, pTexture2D->getContentTypeString());

			if (pTexture2D->getTileStyleU() != MODELTEXTURETILESTYLE_WRAP)
				writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEU, CModelTexture2DResource::tileStyleToString(pTexture2D->getTileStyleU()));
			if (pTexture2D->getTileStyleV() != MODELTEXTURETILESTYLE_WRAP)
				writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEV, CModelTexture2DResource::tileStyleToString(pTexture2D->getTileStyleV()));
			if (pTexture2D->getFilter() != MODELTEXTUREFILTER_AUTO)
				writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_FILTER, CModelTexture2DResource::filterToString(pTexture2D->getFilter()));
			
			if (pTexture2D->hasBox2D()) {
				nfFloat fU, fV, fWidth, fHeight;
				pTexture2D->getBox2D(fU, fV, fWidth, fHeight);
				writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_BOX,
					std::to_string(fU) + " " + std::to_string(fV) + " " + std::to_string(fWidth) + " " + std::to_string(fHeight));
			}
			
			writeEndElement();

		}

	}

	void CModelWriterNode100_Model::writeModelMetaData()
	{
		if (m_bIsRootModel)
		{
			nfUint32 nMetaDataCount = m_pModel->getMetaDataCount();
			nfUint32 nMetaDataIndex;

			for (nMetaDataIndex = 0; nMetaDataIndex < nMetaDataCount; nMetaDataIndex++) {
				PModelMetaData pMetaData = m_pModel->getMetaData(nMetaDataIndex);
				writeMetaData(pMetaData);
			}
		}
	}


	void CModelWriterNode100_Model::writeBaseMaterials()
	{
		nfUint32 nMaterialCount = m_pModel->getBaseMaterialCount();

		for (nfUint32 nMaterialIndex = 0; nMaterialIndex < nMaterialCount; nMaterialIndex++) {
			m_pProgressMonitor->IncrementProgress(1);

			CModelBaseMaterialResource * pBaseMaterial = m_pModel->getBaseMaterial(nMaterialIndex);

			pBaseMaterial->buildResourceIndexMap();

			assertResourceIsInCurrentPath(pBaseMaterial->getPackageResourceID());
			ModelResourceID nResourceID = pBaseMaterial->getPackageResourceID()->getModelResourceID();

			writeStartElement(XML_3MF_ELEMENT_BASEMATERIALS);
			// Write Object ID (mandatory)
			writeIntAttribute(XML_3MF_ATTRIBUTE_BASEMATERIALS_ID, nResourceID);

			nfUint32 nElementCount = pBaseMaterial->getCount();

			UniqueResourceID nUniqueResourceID = pBaseMaterial->getPackageResourceID()->getUniqueID();

			for (nfUint32 j = 0; j < nElementCount; j++) {
				ModelPropertyID nPropertyID;
				if (!pBaseMaterial->mapResourceIndexToPropertyID(j, nPropertyID)) {
					throw CNMRException(NMR_ERROR_INVALIDPROPERTYRESOURCEID);
				}
				PModelBaseMaterial pElement = pBaseMaterial->getBaseMaterial(nPropertyID);
				
				m_pPropertyIndexMapping->registerPropertyID(nUniqueResourceID, pElement->getPropertyID(), j);

				writeStartElement(XML_3MF_ELEMENT_BASE);
				writeStringAttribute(XML_3MF_ATTRIBUTE_BASEMATERIAL_NAME, pElement->getName());
				writeStringAttribute(XML_3MF_ATTRIBUTE_BASEMATERIAL_DISPLAYCOLOR, pElement->getDisplayColorString());
				writeEndElement();
			}

			writeFullEndElement();
		}

	}


	void CModelWriterNode100_Model::writeSliceStacks() {
		nfUint32 nSliceStackCount = m_pModel->getSliceStackCount();

		for (nfUint32 nSliceStackIndex = 0; nSliceStackIndex < nSliceStackCount; nSliceStackIndex++) {
			m_pProgressMonitor->IncrementProgress(1);

			CModelSliceStack *pSliceStackResource = dynamic_cast<CModelSliceStack*>(m_pModel->getSliceStackResource(nSliceStackIndex).get());
			if (pSliceStackResource != nullptr) {
				writeSliceStack(pSliceStackResource);
			}
		}

	}


	void CModelWriterNode100_Model::writeSliceStack(_In_ CModelSliceStack *pSliceStackResource) {
		__NMRASSERT(pSliceStackResource);

		std::string sNameSpacePrefix = XML_3MF_NAMESPACEPREFIX_SLICE;

		if (pSliceStackResource->OwnPath() == m_pModel->currentPath())
		{
			writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICESTACKRESOURCE, XML_3MF_NAMESPACEPREFIX_SLICE);

			assertResourceIsInCurrentPath(pSliceStackResource->getPackageResourceID());
			writeIntAttribute(XML_3MF_ATTRIBUTE_SLICESTACKID, pSliceStackResource->getPackageResourceID()->getModelResourceID());
			writeFloatAttribute(XML_3MF_ATTRIBUTE_SLICESTACKZBOTTOM, (float)pSliceStackResource->getZBottom());


			if (pSliceStackResource->getSliceRefCount() > 0) {
				for (nfUint32 sliceRefIndex = 0; sliceRefIndex < pSliceStackResource->getSliceRefCount(); sliceRefIndex++) {
					auto sliceRef = pSliceStackResource->getSliceRef(sliceRefIndex);

					writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICEREFRESOURCE, XML_3MF_NAMESPACEPREFIX_SLICE);
					writeIntAttribute(XML_3MF_ATTRIBUTE_SLICEREF_ID, sliceRef->getPackageResourceID()->getModelResourceID());
					if (sliceRef->OwnPath() != m_pModel->currentPath()) {
						writeStringAttribute(XML_3MF_ATTRIBUTE_SLICEREF_PATH, sliceRef->OwnPath());
					}
					writeEndElement();
				}
			}

			if (pSliceStackResource->getSliceCount() > 0) {
				for (nfUint32 nSliceIndex = 0; nSliceIndex < pSliceStackResource->getSliceCount(); nSliceIndex++) {
					if (nSliceIndex % PROGRESS_SLICEUPDATE == PROGRESS_SLICEUPDATE - 1) {

						m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITESLICES);
						m_pProgressMonitor->ReportProgressAndQueryCancelled(true);
					}
					PSlice pSlice = pSliceStackResource->getSlice(nSliceIndex);
					writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICE, XML_3MF_NAMESPACEPREFIX_SLICE);

					writeFloatAttribute(XML_3MF_ATTRIBUTE_SLICEZTOP, nfFloat(pSlice->getTopZ()));
					if (pSlice->getVertexCount() >= 2) {
						writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICEVERTICES, XML_3MF_NAMESPACEPREFIX_SLICE);

						for (nfUint32 nVertexIndex = 0; nVertexIndex < pSlice->getVertexCount(); nVertexIndex++) {
							writeStartElementWithPrefix(XML_3MF_ELEMENT_VERTEX, XML_3MF_NAMESPACEPREFIX_SLICE);
							nfFloat x, y;
							pSlice->getVertex(nVertexIndex, &x, &y);
							writeFloatAttribute(XML_3MF_ATTRIBUTE_SLICEVERTEX_X, x);
							writeFloatAttribute(XML_3MF_ATTRIBUTE_SLICEVERTEX_Y, y);
							writeEndElement();
						}
						writeFullEndElement();
					}
					else {
						if (pSlice->getVertexCount() == 1)
							throw CNMRException(NMR_ERROR_SLICE_ONEVERTEX);
					}

					for (nfUint32 nPolygonIndex = 0; nPolygonIndex < pSlice->getPolygonCount(); nPolygonIndex++) {
						if (pSlice->getPolygonIndexCount(nPolygonIndex) >= 2) {
							writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICEPOLYGON, XML_3MF_NAMESPACEPREFIX_SLICE);
							writeIntAttribute(XML_3MF_ATTRIBUTE_SLICEPOLYGON_STARTV, pSlice->getPolygonIndex(nPolygonIndex, 0));

							for (nfUint32 nIndexIndex = 1; nIndexIndex < pSlice->getPolygonIndexCount(nPolygonIndex); nIndexIndex++) {
								writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICESEGMENT, XML_3MF_NAMESPACEPREFIX_SLICE);
								writeIntAttribute(XML_3MF_ATTRIBUTE_SLICESEGMENT_V2, pSlice->getPolygonIndex(nPolygonIndex, nIndexIndex));
								writeEndElement();
							}

							writeFullEndElement();
						}
						else {
							if (pSlice->getPolygonIndexCount(nPolygonIndex) == 1)
								throw CNMRException(NMR_ERROR_SLICE_ONEPOINT);
						}
					}

					writeFullEndElement();
				}
			}
			writeFullEndElement();
		}
	}

	void CModelWriterNode100_Model::writeObjects()
	{
		std::list <CModelObject *> objectList = m_pModel->getSortedObjectList();

		for (auto iIterator = objectList.begin(); iIterator != objectList.end(); iIterator++) {
			CModelObject * pObject = *iIterator;

			PPackageModelPath pPath = pObject->getPackageResourceID()->getPackageModelPath();
			if (m_pModel->currentModelPath()->getPath() != pPath->getPath())
			{
				continue;
			}

			m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITEOBJECTS);
			m_pProgressMonitor->IncrementProgress(1);
			m_pProgressMonitor->ReportProgressAndQueryCancelled(true);

			writeStartElement(XML_3MF_ELEMENT_OBJECT);
			// Write Object ID (mandatory)
			writeIntAttribute(XML_3MF_ATTRIBUTE_OBJECT_ID, pObject->getPackageResourceID()->getModelResourceID());

			// Write Object Name (optional)
			std::string sObjectName = pObject->getName();
			if (sObjectName.length() > 0)
				writeStringAttribute(XML_3MF_ATTRIBUTE_OBJECT_NAME, sObjectName);

			// Write Object Partnumber (optional)
			std::string sObjectPartNumber = pObject->getPartNumber();
			if (sObjectPartNumber.length() > 0)
				writeStringAttribute(XML_3MF_ATTRIBUTE_OBJECT_PARTNUMBER, sObjectPartNumber);

			// Write Object Type (optional)
			writeStringAttribute(XML_3MF_ATTRIBUTE_OBJECT_TYPE, pObject->getObjectTypeString());

			// Write Object Thumbnail (optional)
			PModelAttachment pThumbnail = pObject->getThumbnailAttachment();
			if (pThumbnail) {
				PModelAttachment pModelAttachment = m_pModel->findModelAttachment(pThumbnail->getPathURI());
				if (!pModelAttachment)
					throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
				if (!((pModelAttachment->getRelationShipType() == PACKAGE_TEXTURE_RELATIONSHIP_TYPE) || (pModelAttachment->getRelationShipType() == PACKAGE_THUMBNAIL_RELATIONSHIP_TYPE)))
					throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);

				writeStringAttribute(XML_3MF_ATTRIBUTE_OBJECT_THUMBNAIL, pThumbnail->getPathURI());
			}

			if (m_bWriteProductionExtension) {
				if (!pObject->uuid().get())
					throw CNMRException(NMR_ERROR_MISSINGUUID);
				writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_PRODUCTION, XML_3MF_PRODUCTION_UUID, pObject->uuid()->toString());
			}

			// Slice extension content
			if (m_bWriteSliceExtension) {
				if (pObject->getSliceStack().get()) {
					assertResourceIsInCurrentPath(pObject->getSliceStack()->getPackageResourceID());
					writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_SLICE, XML_3MF_ATTRIBUTE_OBJECT_SLICESTACKID,
						fnUint32ToString(pObject->getSliceStack()->getPackageResourceID()->getModelResourceID()));
				}
				if (pObject->slicesMeshResolution() != MODELSLICESMESHRESOLUTION_FULL) {
					writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_SLICE, XML_3MF_ATTRIBUTE_OBJECT_MESHRESOLUTION,
						XML_3MF_VALUE_OBJECT_MESHRESOLUTION_LOW);
				}
			}

			// Check if object is a mesh Object
			CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject *> (pObject);
			if (pMeshObject) {
				// Prepare Object Level Property ID and Index
				UniqueResourceID nObjectLevelPropertyID = 0;
				ModelResourceIndex nObjectLevelPropertyIndex = 0;
				CMesh* pMesh = pMeshObject->getMesh();

				if (pMesh) {
					CMeshInformationHandler * pMeshInformationHandler = pMesh->getMeshInformationHandler();
					if (pMeshInformationHandler) {
						// Get generic property handler
						CMeshInformation *pInformation = pMeshInformationHandler->getInformationByType(0, emiProperties);
						if (pInformation) {
							auto pProperties = dynamic_cast<CMeshInformation_Properties *> (pInformation);
							NMR::MESHINFORMATION_PROPERTIES * pDefaultData = (NMR::MESHINFORMATION_PROPERTIES*)pProperties->getDefaultData();
							if (pDefaultData && pDefaultData->m_nUniqueResourceID != 0) {
								nObjectLevelPropertyID = pDefaultData->m_nUniqueResourceID;
								nObjectLevelPropertyIndex = m_pPropertyIndexMapping->mapPropertyIDToIndex(nObjectLevelPropertyID, pDefaultData->m_nPropertyIDs[0]);
							}
						}
					}
				}
				// Write Object Level Attributes (only for meshes)
				if (nObjectLevelPropertyID != 0) {
					ModelResourceID nPropertyModelResourceID = m_pModel->findPackageResourceID(nObjectLevelPropertyID)->getModelResourceID();
					writeIntAttribute(XML_3MF_ATTRIBUTE_OBJECT_PID, nPropertyModelResourceID);
					writeIntAttribute(XML_3MF_ATTRIBUTE_OBJECT_PINDEX, nObjectLevelPropertyIndex);
				}
			}

			writeMetaDataGroup(pObject->metaDataGroup());

			if (pMeshObject) {
				CModelWriterNode100_Mesh ModelWriter_Mesh(pMeshObject, m_pXMLWriter, m_pProgressMonitor,
					m_pPropertyIndexMapping, m_nDecimalPrecision, m_bWriteMaterialExtension, m_bWriteBeamLatticeExtension);

				ModelWriter_Mesh.writeToXML();
			}

			// Check if object is a component Object
			CModelComponentsObject * pComponentObject = dynamic_cast<CModelComponentsObject *> (pObject);
			if (pComponentObject) {
				writeComponentsObject(pComponentObject);
			}

			writeFullEndElement();
		}

	}

	void CModelWriterNode100_Model::writeMetaData(_In_ PModelMetaData pMetaData)
	{
		writeStartElement(XML_3MF_ELEMENT_METADATA);

		std::string sNameSpace = pMetaData->getNameSpace();
		std::string sName = pMetaData->getName();
		std::string sValue = pMetaData->getValue();
		std::string sType = pMetaData->getType();
		nfBool bPreserve = pMetaData->getPreserve();

		if (sNameSpace.empty()) {
			writeStringAttribute(XML_3MF_ATTRIBUTE_METADATA_NAME, sName);
		}
		else {
			std::string sNameSpacePrefix;
			if (!m_pXMLWriter->GetNamespacePrefix(sNameSpace, sNameSpacePrefix)) {
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);
			}
			writeStringAttribute(XML_3MF_ATTRIBUTE_METADATA_NAME, composeNamespaceAndNameIntoKey(sNameSpacePrefix, sName));
		}

		if (sType != "xs:string") {
			writeStringAttribute(XML_3MF_ATTRIBUTE_METADATA_TYPE, sType);
		}
		if (bPreserve) {
			writeStringAttribute(XML_3MF_ATTRIBUTE_METADATA_PRESERVE, "1");
		}
		writeText(sValue.c_str(), (nfUint32)sValue.length());

		writeEndElement();
	}

	void CModelWriterNode100_Model::writeMetaDataGroup(_In_ PModelMetaDataGroup pMetaDataGroup)
	{
		if (pMetaDataGroup->getMetaDataCount() > 0)
		{
			writeStartElement(XML_3MF_ELEMENT_METADATAGROUP);
			for (nfUint32 index = 0; index<pMetaDataGroup->getMetaDataCount(); index++) {
				PModelMetaData pMetaData = pMetaDataGroup->getMetaData(index);
				writeMetaData(pMetaData);
			}
			writeFullEndElement();
		}
	}

	void CModelWriterNode100_Model::writeColors()
	{
		nfUint32 nCount = m_pModel->getColorGroupCount();

		for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
			m_pProgressMonitor->IncrementProgress(1);

			CModelColorGroupResource * pColorGroup = m_pModel->getColorGroup(nIndex);

			pColorGroup->buildResourceIndexMap();

			assertResourceIsInCurrentPath(pColorGroup->getPackageResourceID());
			ModelResourceID nResourceID = pColorGroup->getPackageResourceID()->getModelResourceID();

			writeStartElementWithPrefix(XML_3MF_ELEMENT_COLORGROUP, XML_3MF_NAMESPACEPREFIX_MATERIAL);
			// Write Object ID (mandatory)
			writeIntAttribute(XML_3MF_ATTRIBUTE_COLORS_ID, nResourceID);

			nfUint32 nElementCount = pColorGroup->getCount();

			UniqueResourceID nUniqueResourceID = pColorGroup->getPackageResourceID()->getUniqueID();
			for (nfUint32 j = 0; j < nElementCount; j++) {
				ModelPropertyID nPropertyID;
				if (!pColorGroup->mapResourceIndexToPropertyID(j, nPropertyID)) {
					throw CNMRException(NMR_ERROR_INVALIDPROPERTYRESOURCEID);
				}
				nfColor pElement = pColorGroup->getColor(nPropertyID);

				m_pPropertyIndexMapping->registerPropertyID(nUniqueResourceID, nPropertyID, j);

				writeStartElementWithPrefix(XML_3MF_ELEMENT_COLOR, XML_3MF_NAMESPACEPREFIX_MATERIAL);
				writeStringAttribute(XML_3MF_ATTRIBUTE_COLORS_COLOR, fnColorToString(pElement));
				writeEndElement();
			}

			writeFullEndElement();
		}
	}

	void CModelWriterNode100_Model::writeTex2Coords()
	{
		nfUint32 nGroupCount = m_pModel->getTexture2DGroupCount();

		for (nfUint32 nGroupIndex = 0; nGroupIndex < nGroupCount; nGroupIndex++) {
			m_pProgressMonitor->IncrementProgress(1);

			CModelTexture2DGroupResource * pTexture2DGroup = m_pModel->getTexture2DGroup(nGroupIndex);

			pTexture2DGroup->buildResourceIndexMap();

			assertResourceIsInCurrentPath(pTexture2DGroup->getPackageResourceID());
			ModelResourceID nResourceID = pTexture2DGroup->getPackageResourceID()->getModelResourceID();

			writeStartElementWithPrefix(XML_3MF_ELEMENT_TEX2DGROUP, XML_3MF_NAMESPACEPREFIX_MATERIAL);
			// Write Object ID (mandatory)
			writeIntAttribute(XML_3MF_ATTRIBUTE_TEX2DGROUP_ID, nResourceID);
			assertResourceIsInCurrentPath(pTexture2DGroup->getTexture2D()->getPackageResourceID());
			writeIntAttribute(XML_3MF_ATTRIBUTE_TEX2DGROUP_TEXTUREID, pTexture2DGroup->getTexture2D()->getPackageResourceID()->getModelResourceID());

			nfUint32 nElementCount = pTexture2DGroup->getCount();
			UniqueResourceID nUniqueResourceID = pTexture2DGroup->getPackageResourceID()->getUniqueID();
			for (nfUint32 j = 0; j < nElementCount; j++) {
				ModelPropertyID nPropertyID;
				if (!pTexture2DGroup->mapResourceIndexToPropertyID(j, nPropertyID)) {
					throw CNMRException(NMR_ERROR_INVALIDPROPERTYRESOURCEID);
				}
				MODELTEXTURE2DCOORDINATE uvCoordinate = pTexture2DGroup->getUVCoordinate(nPropertyID);

				m_pPropertyIndexMapping->registerPropertyID(nUniqueResourceID, nPropertyID, j);

				writeStartElementWithPrefix(XML_3MF_ELEMENT_TEX2COORD, XML_3MF_NAMESPACEPREFIX_MATERIAL);
				writeFloatAttribute(XML_3MF_ATTRIBUTE_TEXTURE_U, (float)uvCoordinate.m_dU);
				writeFloatAttribute(XML_3MF_ATTRIBUTE_TEXTURE_V, (float)uvCoordinate.m_dV);
				writeEndElement();
			}

			writeFullEndElement();
		}

	}

	void CModelWriterNode100_Model::writeCompositeMaterials()
	{
		nfUint32 nCount = m_pModel->getCompositeMaterialsCount();

		for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
			m_pProgressMonitor->IncrementProgress(1);

			CModelCompositeMaterialsResource * pCompositeMaterials = m_pModel->getCompositeMaterials(nIndex);

			pCompositeMaterials->buildResourceIndexMap();

			assertResourceIsInCurrentPath(pCompositeMaterials->getPackageResourceID());
			ModelResourceID nResourceID = pCompositeMaterials->getPackageResourceID()->getModelResourceID();
			UniqueResourceID nUniqueResourceID = pCompositeMaterials->getPackageResourceID()->getUniqueID();

			writeStartElementWithPrefix(XML_3MF_ELEMENT_COMPOSITEMATERIALS, XML_3MF_NAMESPACEPREFIX_MATERIAL);
			// Write Object ID (mandatory)
			writeIntAttribute(XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_ID, nResourceID);
			PModelBaseMaterialResource pBaseMaterialResource = pCompositeMaterials->getBaseMaterialResource();
			assertResourceIsInCurrentPath(pBaseMaterialResource->getPackageResourceID());
			ModelResourceID nBaseMaterialResourceID = pBaseMaterialResource->getPackageResourceID()->getModelResourceID();
			writeIntAttribute(XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_MATID, nBaseMaterialResourceID);

			std::vector<nfUint32> matIndices;
			std::set<ModelPropertyID> mapUsedBMPropertyIDs;
			
			nfUint32 nCompositeCount = pCompositeMaterials->getCount();
			for (nfUint32 j = 0; j < nCompositeCount; j++) {
				ModelPropertyID nPropertyID;
				if (!pCompositeMaterials->mapResourceIndexToPropertyID(j, nPropertyID)) {
					throw CNMRException(NMR_ERROR_INVALIDPROPERTYRESOURCEID);
				}
				m_pPropertyIndexMapping->registerPropertyID(nUniqueResourceID, nPropertyID, j);
				PModelComposite pModelComposite = pCompositeMaterials->getComposite(nPropertyID);

				for (auto constituents : (*pModelComposite)) {
					nfUint32 nBaseMaterialIndex = m_pPropertyIndexMapping->mapPropertyIDToIndex(nBaseMaterialResourceID, constituents.m_nPropertyID);
					if (mapUsedBMPropertyIDs.find(constituents.m_nPropertyID) == mapUsedBMPropertyIDs.end()) {
						mapUsedBMPropertyIDs.insert(constituents.m_nPropertyID);
						matIndices.push_back(nBaseMaterialIndex);
					}
				}
			}
			writeStringAttribute(XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_MATINDICES, fnVectorToSpaceDelimitedString(matIndices));

			for (nfUint32 j = 0; j < nCompositeCount; j++) {
				ModelPropertyID nPropertyID;
				if (!pCompositeMaterials->mapResourceIndexToPropertyID(j, nPropertyID)) {
					throw CNMRException(NMR_ERROR_INVALIDPROPERTYRESOURCEID);
				}
				PModelComposite pModelComposite = pCompositeMaterials->getComposite(nPropertyID);

				std::vector<nfDouble> ratios;
				writeStartElementWithPrefix(XML_3MF_ELEMENT_COMPOSITE, XML_3MF_NAMESPACEPREFIX_MATERIAL);
				for (ModelPropertyID nBMPropertyID: mapUsedBMPropertyIDs) {
					ratios.push_back(pModelComposite->GetMixingRatio(nBMPropertyID));
				}
				writeStringAttribute(XML_3MF_ATTRIBUTE_COMPOSITE_VALUES, fnVectorToSpaceDelimitedString(ratios));
				writeEndElement();
			}
			writeFullEndElement();
		}
	}


	void CModelWriterNode100_Model::writeMultiPropertyAttributes(_In_ CModelMultiPropertyGroupResource* pMultiPropertyGroup)
	{
		// assemble and write pids and blendmethods
		std::vector<UniqueResourceID> vctUniqueIDs;
		std::vector<std::string> vctBlendMethodString;

		nfUint32 nLayerCount = pMultiPropertyGroup->getLayerCount();
		for (nfUint32 iLayer = 0; iLayer < nLayerCount; iLayer++) {
			MODELMULTIPROPERTYLAYER layer = pMultiPropertyGroup->getLayer(iLayer);
			vctUniqueIDs.push_back(layer.m_nUniqueResourceID);
			if (iLayer > 0) {
				vctBlendMethodString.push_back(CModelMultiPropertyGroupResource::blendMethodToString(layer.m_nMethod));
			}
		}
		assertResourceIsInCurrentPath(pMultiPropertyGroup->getPackageResourceID());
		ModelResourceID nModelResourceID = pMultiPropertyGroup->getPackageResourceID()->getModelResourceID();
		writeIntAttribute(XML_3MF_ATTRIBUTE_MULTIPROPERTIES_ID, nModelResourceID);
		writeStringAttribute(XML_3MF_ATTRIBUTE_MULTIPROPERTIES_PIDS, fnVectorToSpaceDelimitedString(vctUniqueIDs));
		writeStringAttribute(XML_3MF_ATTRIBUTE_MULTIPROPERTIES_BLENDMETHODS, fnVectorToSpaceDelimitedString(vctBlendMethodString));
	}

	void CModelWriterNode100_Model::writeMultiPropertyMultiElements(_In_ CModelMultiPropertyGroupResource* pMultiPropertyGroup)
	{
		// assemble and write MultiPropertyElements
		assertResourceIsInCurrentPath(pMultiPropertyGroup->getPackageResourceID());
		UniqueResourceID nUniqueResourceID = pMultiPropertyGroup->getPackageResourceID()->getUniqueID();

		nfUint32 nMultiCount = pMultiPropertyGroup->getCount();
		nfUint32 nLayerCount = pMultiPropertyGroup->getLayerCount();
		for (nfUint32 iMulti = 0; iMulti < nMultiCount; iMulti++) {
			ModelPropertyID nPropertyID;
			if (!pMultiPropertyGroup->mapResourceIndexToPropertyID(iMulti, nPropertyID)) {
				throw CNMRException(NMR_ERROR_INVALIDPROPERTYRESOURCEID);
			}
			m_pPropertyIndexMapping->registerPropertyID(nUniqueResourceID, nPropertyID, iMulti);

			PModelMultiProperty pMultiProperty = pMultiPropertyGroup->getMultiProperty(nPropertyID);
			std::vector<nfUint32> vctPIndices;
			for (nfUint32 iLayer = 0; iLayer < nLayerCount; iLayer++) {
				MODELMULTIPROPERTYLAYER layer = pMultiPropertyGroup->getLayer(iLayer);
				if (iLayer < pMultiProperty->size()) {
					nfUint32 pIndex = m_pPropertyIndexMapping->mapPropertyIDToIndex(layer.m_nUniqueResourceID, (*pMultiProperty)[iLayer]);
					vctPIndices.push_back(pIndex);
				} else {
					throw CNMRException(NMR_ERROR_MULTIPROPERTIES_NOT_ENOUGH_PROPERTYIDS_SPECIFIED);
				}
			}

			writeStartElementWithPrefix(XML_3MF_ELEMENT_MULTI, XML_3MF_NAMESPACEPREFIX_MATERIAL);

			writeStringAttribute(XML_3MF_ATTRIBUTE_MULTI_PINDICES, fnVectorToSpaceDelimitedString(vctPIndices));

			writeEndElement();
		}
	}

	void CModelWriterNode100_Model::writeMultiProperties()
	{
		nfUint32 nCount = m_pModel->getMultiPropertyGroupCount();

		for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
			m_pProgressMonitor->IncrementProgress(1);

			CModelMultiPropertyGroupResource * pMultiPropertyGroup = m_pModel->getMultiPropertyGroup(nIndex);

			pMultiPropertyGroup->buildResourceIndexMap();

			writeStartElementWithPrefix(XML_3MF_ELEMENT_MULTIPROPERTIES, XML_3MF_NAMESPACEPREFIX_MATERIAL);

			writeMultiPropertyAttributes(pMultiPropertyGroup);
			
			writeMultiPropertyMultiElements(pMultiPropertyGroup);

			writeFullEndElement();
		}
	}

	void CModelWriterNode100_Model::writeResources()
	{
		writeStartElement(XML_3MF_ELEMENT_RESOURCES);

		if (m_bIsRootModel)
		{
			if (m_bWriteBaseMaterials)
				writeBaseMaterials();

			if (m_bWriteMaterialExtension) {
				writeTextures2D();
				writeColors();
				writeTex2Coords();
				writeCompositeMaterials();
				writeMultiProperties();
			}
			if (m_bWriteSliceExtension) {
				writeSliceStacks();
			}
			if (m_bWriteObjects)
				writeObjects();
		}
		else {
			if (m_bWriteSliceExtension) {
				writeSliceStacks();
			}
			if (m_bWriteObjects) {
				writeObjects();
			}
		}
		
		writeFullEndElement();
	}

	void CModelWriterNode100_Model::writeBuild()
	{
		writeStartElement(XML_3MF_ELEMENT_BUILD);
		if (m_bWriteProductionExtension) {
			if (!m_pModel->buildUUID().get()) {
				throw CNMRException(NMR_ERROR_MISSINGUUID);
			}
			writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_PRODUCTION, XML_3MF_PRODUCTION_UUID, m_pModel->buildUUID()->toString());
		}

		if (m_bIsRootModel)
		{
			m_pProgressMonitor->IncrementProgress(1);

			nfUint32 nCount = m_pModel->getBuildItemCount();
			nfUint32 nIndex;
			for (nIndex = 0; nIndex < nCount; nIndex++) {
				PModelBuildItem pBuildItem = m_pModel->getBuildItem(nIndex);

				writeStartElement(XML_3MF_ELEMENT_ITEM);

				CModelObject* pObject = pBuildItem->getObject();
				PPackageResourceID pID = pObject->getPackageResourceID();

				writeIntAttribute(XML_3MF_ATTRIBUTE_ITEM_OBJECTID, pID->getModelResourceID());
				if (pID->getPath() != m_pModel->currentPath())
					writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_PRODUCTION, XML_3MF_PRODUCTION_PATH, pID->getPath());

				if (!pBuildItem->getPartNumber().empty())
					writeStringAttribute(XML_3MF_ATTRIBUTE_ITEM_PARTNUMBER, pBuildItem->getPartNumber());


				if (m_bWriteProductionExtension) {
					if (!pBuildItem->uuid().get()) {
						throw CNMRException(NMR_ERROR_MISSINGUUID);
					}
					writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_PRODUCTION, XML_3MF_PRODUCTION_UUID, pBuildItem->uuid()->toString());
				}
				if (pBuildItem->hasTransform())
					writeStringAttribute(XML_3MF_ATTRIBUTE_ITEM_TRANSFORM, pBuildItem->getTransformString());

				if (m_bWriteSliceExtension && !pBuildItem->isValidForSlices()) {
					throw CNMRException(NMR_ERROR_SLICETRANSFORMATIONPLANAR);
				}

				writeMetaDataGroup(pBuildItem->metaDataGroup());
				if (pBuildItem->metaDataGroup()->getMetaDataCount() > 0)
					writeFullEndElement();
				else 
					writeEndElement();

			}
		}
		writeFullEndElement();
	}

	void CModelWriterNode100_Model::writeComponentsObject(_In_ CModelComponentsObject * pComponentsObject)
	{
		__NMRASSERT(pComponentsObject);

		nfUint32 nIndex;
		nfUint32 nCount = pComponentsObject->getComponentCount();

		writeStartElement(XML_3MF_ELEMENT_COMPONENTS);
		for (nIndex = 0; nIndex < nCount; nIndex++) {
			PModelComponent pComponent = pComponentsObject->getComponent(nIndex);
			writeStartElement(XML_3MF_ELEMENT_COMPONENT);
			PPackageResourceID pObjectID = pComponent->getObject()->getPackageResourceID();
			writeIntAttribute(XML_3MF_ATTRIBUTE_COMPONENT_OBJECTID, pObjectID->getModelResourceID());
			if (pObjectID->getPath() != m_pModel->currentPath()) {
				if (m_pModel->currentPath() != m_pModel->rootPath()) {
					throw CNMRException(NMR_ERROR_REFERENCESTOODEEP);
				}
				if (m_bWriteProductionExtension) {
					writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_PRODUCTION, XML_3MF_PRODUCTION_PATH, pObjectID->getPath());
				}
				else {
					throw CNMRException(NMR_ERROR_PRODUCTIONEXTENSION_REQUIRED);
				}
			}
			if (pComponent->hasTransform())
				writeStringAttribute(XML_3MF_ATTRIBUTE_COMPONENT_TRANSFORM, pComponent->getTransformString());
			if (m_bWriteProductionExtension) {
				if (!pComponent->uuid().get()) {
					throw CNMRException(NMR_ERROR_MISSINGUUID);
				}
				writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_PRODUCTION, XML_3MF_PRODUCTION_UUID, pComponent->uuid()->toString());
			}
			writeEndElement();
		}

		writeFullEndElement();
	}

}
