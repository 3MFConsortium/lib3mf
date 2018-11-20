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

NMR_ModelWriterNode100_Model.cpp implements the Model Writer Model Node Class.
This is the class for exporting the 3mf model stream root node.

--*/

#include "Model/Writer/v100/NMR_ModelWriterNode100_Mesh.h"
#include "Model/Writer/v100/NMR_ModelWriterNode100_Model.h"

#include "Model/Classes/NMR_ModelAttachment.h"
#include "Model/Classes/NMR_ModelBuildItem.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelBaseMaterials.h"
#include "Model/Classes/NMR_ModelTexture2D.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelDefaultProperty_Color.h"
#include "Model/Classes/NMR_ModelDefaultProperty_BaseMaterial.h"
#include "Model/Classes/NMR_ModelDefaultProperty_TexCoord2D.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include "Common/MeshInformation/NMR_MeshInformation_NodeColors.h"
#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h"
#include "Common/MeshInformation/NMR_MeshInformation_Slices.h"

#include "Common/3MF_ProgressMonitor.h"


namespace NMR {

	CModelWriterNode100_Model::CModelWriterNode100_Model(_In_ CModel * pModel, _In_ CXmlWriter * pXMLWriter, _In_ CProgressMonitor * pProgressMonitor)
		:CModelWriterNode(pModel, pXMLWriter, pProgressMonitor)
	{
		m_ResourceCounter = pModel->generateResourceID();

		m_pColorMapping = std::make_shared<CModelWriter_ColorMapping>(generateOutputResourceID ());

		m_pTexCoordMappingContainer = std::make_shared<CModelWriter_TexCoordMappingContainer>();
		m_bWriteMaterialExtension = true;
		m_bWriteProductionExtension = true;
		m_bWriteBeamLatticeExtension = true;
		m_bWriteSliceExtension = true;
		m_bWriteBaseMaterials = true;
		m_bWriteObjects = true;
		m_bIsRootModel = true;
		m_pSliceStackResource = NULL;

		nfInt32 nObjectCount = pModel->getObjectCount();
		nfInt32 nObjectIndex;

		for (nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex++) {
			PModelResource pResource = pModel->getObjectResource(nObjectIndex);

			CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject*> (pResource.get());
			if (pMeshObject) {
				CMesh * pMesh = pMeshObject->getMesh();
				if (pMesh) {
					if (m_bWriteMaterialExtension) {
						calculateColors(pMesh);
						calculateTexCoords(pMesh);
					}
				}
			}

			if (m_bWriteMaterialExtension) {
				// Register Default Property Resources
				CModelObject * pObject = dynamic_cast<CModelObject*> (pResource.get());
				if (pObject) {
					PModelDefaultProperty pProperty = pObject->getDefaultProperty();

					CModelDefaultProperty_Color * pColorProperty = dynamic_cast<CModelDefaultProperty_Color *> (pProperty.get());
					if (pColorProperty != nullptr) {
						nfColor cColor = pColorProperty->getColor();
						if (cColor != 0)
							m_pColorMapping->registerColor(cColor);
					}

					CModelDefaultProperty_TexCoord2D * pTexCoord2DProperty = dynamic_cast<CModelDefaultProperty_TexCoord2D *> (pProperty.get());
					if (pTexCoord2DProperty != nullptr) {
						PModelWriter_TexCoordMapping pTexCoordMapping = m_pTexCoordMappingContainer->findTexture(pTexCoord2DProperty->getTextureID());
						if (pTexCoordMapping.get() == nullptr) {
							pTexCoordMapping = m_pTexCoordMappingContainer->addTexture(pTexCoord2DProperty->getTextureID(), generateOutputResourceID());
						}

						pTexCoordMapping->registerTexCoords(pTexCoord2DProperty->getU(), pTexCoord2DProperty->getV());
					}
				}
			}
		}
	}

	CModelWriterNode100_Model::CModelWriterNode100_Model(_In_ CModel * pModel, _In_ CXmlWriter * pXMLWriter, _In_ CProgressMonitor * pProgressMonitor,
		CModelSliceStackResource *pSliceStackResource) : CModelWriterNode(pModel, pXMLWriter, pProgressMonitor)
	{
		m_bWriteMaterialExtension = false;
		m_bWriteMaterialExtension = false;
		m_bWriteBeamLatticeExtension = false;
		m_bWriteBaseMaterials = false;
		m_bWriteObjects = false;
		m_bIsRootModel = false;
		m_bWriteSliceExtension = true;
		m_pSliceStackResource = pSliceStackResource;
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

		if (sRequiredExtensions.size()>0)
			writeConstStringAttribute(XML_3MF_ATTRIBUTE_REQUIREDEXTENSIONS, sRequiredExtensions.c_str());

		writeMetaData();
		writeResources();
		writeBuild();

		writeFullEndElement();
	}

	void CModelWriterNode100_Model::writeTextures2D()
	{
		nfUint32 nTextureCount = m_pModel->getTexture2DCount();
		nfUint32 nTextureIndex;

		for (nTextureIndex = 0; nTextureIndex < nTextureCount; nTextureIndex++) {
			CModelTexture2DResource * pTexture2D = m_pModel->getTexture2D(nTextureIndex);

			writeStartElementWithPrefix(XML_3MF_ELEMENT_TEXTURE2D, XML_3MF_NAMESPACEPREFIX_MATERIAL);
			writeIntAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_ID, pTexture2D->getResourceID()->getUniqueID());
			writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_PATH, pTexture2D->getPath());
			writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_CONTENTTYPE, pTexture2D->getContentTypeString());

			if (pTexture2D->getTileStyleU() != MODELTEXTURETILESTYLE_WRAP)
				writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEU, CModelTexture2DResource::tileStyleToString(pTexture2D->getTileStyleU()));
			if (pTexture2D->getTileStyleV() != MODELTEXTURETILESTYLE_WRAP)
				writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEV, CModelTexture2DResource::tileStyleToString(pTexture2D->getTileStyleV()));
			
			if (pTexture2D->hasBox2D()) {
				nfFloat fU, fV, fWidth, fHeight;
				pTexture2D->getBox2D(fU, fV, fWidth, fHeight);
				writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_BOX,
					std::to_string(fU) + " " + std::to_string(fV) + " " + std::to_string(fWidth) + " " + std::to_string(fHeight));
			}
			
			writeEndElement();

		}

	}

	void CModelWriterNode100_Model::writeMetaData()
	{
		if (m_bIsRootModel)
		{
			nfUint32 nMetaDataCount = m_pModel->getMetaDataCount();
			nfUint32 nMetaDataIndex;

			for (nMetaDataIndex = 0; nMetaDataIndex < nMetaDataCount; nMetaDataIndex++) {
				std::string sKey;
				std::string sValue;
				m_pModel->getMetaData(nMetaDataIndex, sKey, sValue);

				// TODO: translate namespace within metadatum to namespace identifier
				writeStartElement(XML_3MF_ELEMENT_METADATA);
				writeStringAttribute(XML_3MF_ATTRIBUTE_METADATA_NAME, sKey);
				writeText(sValue.c_str(), (nfUint32)sValue.length());
				writeEndElement();
			}
		}
	}


	void CModelWriterNode100_Model::writeBaseMaterials()
	{
		nfUint32 nMaterialCount = m_pModel->getBaseMaterialCount();
		nfUint32 nMaterialIndex;
		nfUint32 j;

		for (nMaterialIndex = 0; nMaterialIndex < nMaterialCount; nMaterialIndex++) {
			CModelBaseMaterialResource * pBaseMaterial = m_pModel->getBaseMaterial(nMaterialIndex);

			writeStartElement(XML_3MF_ELEMENT_BASEMATERIALS);
			// Write Object ID (mandatory)
			writeIntAttribute(XML_3MF_ATTRIBUTE_BASEMATERIALS_ID, pBaseMaterial->getResourceID()->getUniqueID());

			nfUint32 nElementCount = pBaseMaterial->getCount();

			for (j = 0; j < nElementCount; j++) {
				PModelBaseMaterial pElement = pBaseMaterial->getBaseMaterial(j);
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
		nfUint32 nSliceStackIndex;

		if (m_pSliceStackResource == nullptr) {
			for (nSliceStackIndex = 0; nSliceStackIndex < nSliceStackCount; nSliceStackIndex++) {
				CModelSliceStackResource *pSliceStackResource = dynamic_cast<CModelSliceStackResource*>(m_pModel->getSliceStackResource(nSliceStackIndex).get());
				if (pSliceStackResource != nullptr) {
					writeSliceStack(pSliceStackResource);
				}
			}
		}
		else {
			writeSliceStack(m_pSliceStackResource);
		}
	}


	void CModelWriterNode100_Model::writeSliceStack(_In_ CModelSliceStackResource *pSliceStackResource) {
		__NMRASSERT(pSliceStackResource);

		std::string sNameSpacePrefix = XML_3MF_NAMESPACEPREFIX_SLICE;

		writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICESTACKRESOURCE, XML_3MF_NAMESPACEPREFIX_SLICE);

		writeIntAttribute(XML_3MF_ATTRIBUTE_SLICESTACKID, pSliceStackResource->getResourceID()->getUniqueID());
		writeFloatAttribute(XML_3MF_ATTRIBUTE_SLICESTACKZBOTTOM, pSliceStackResource->getSliceStack()->getBottomZ());

		if (pSliceStackResource->getSliceStack()->usesSliceRef() && (m_pSliceStackResource == NULL)) {
			writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICEREFRESOURCE, XML_3MF_NAMESPACEPREFIX_SLICE);
			writeIntAttribute(XML_3MF_ATTRIBUTE_SLICEREF_ID, pSliceStackResource->getResourceID()->getUniqueID());
			writeStringAttribute(XML_3MF_ATTRIBUTE_SLICEREF_PATH, pSliceStackResource->sliceRefPath());
			writeEndElement();
		}
		else {
			nfUint32 nSliceIndex;
			for (nSliceIndex = 0; nSliceIndex < pSliceStackResource->getSliceStack()->getSliceCount(); nSliceIndex++) {
				if (nSliceIndex % PROGRESS_SLICEUPDATE == PROGRESS_SLICEUPDATE-1) {
					if (m_pProgressMonitor && !m_pProgressMonitor->Progress(-1, ProgressIdentifier::PROGRESS_WRITESLICES))
						throw CNMRException(NMR_USERABORTED);
				}
				PSlice pSlice = pSliceStackResource->getSliceStack()->getSlice(nSliceIndex);

				writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICE, XML_3MF_NAMESPACEPREFIX_SLICE);

				writeFloatAttribute(XML_3MF_ATTRIBUTE_SLICEZTOP, pSlice->getTopZ());

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

				for (nfUint32 nPolygonIndex = 0; nPolygonIndex < pSlice->getNumberOfPolygons(); nPolygonIndex++) {
					if (pSlice->getPolygonIndexCount(nPolygonIndex) >= 2) {
						writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICEPOLYGON, XML_3MF_NAMESPACEPREFIX_SLICE);
						writeIntAttribute(XML_3MF_ATTRIBUTE_SLICEPOLYGON_STARTV, pSlice->getPolygonIndex(nPolygonIndex, 0));

						for (nfUint32 nIndexIndex = 1; nIndexIndex < pSlice->getPolygonIndexCount(nPolygonIndex); nIndexIndex++) {
							writeStartElementWithPrefix(XML_3MF_ELEMENT_SLICESEGMENT, XML_3MF_NAMESPACEPREFIX_SLICE);
							writeIntAttribute(XML_3MF_ATTRIBUTE_SLICESEGMENT_V2, pSlice->getPolygonIndex(nPolygonIndex, nIndexIndex));
							writeFullEndElement();
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

	void CModelWriterNode100_Model::writeObjects()
	{
		nfUint32 nObjectCount = m_pModel->getObjectCount();
		nfUint32 nObjectIndex;

		for (nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex++) {
			if (m_pProgressMonitor && !m_pProgressMonitor->Progress(-1, ProgressIdentifier::PROGRESS_WRITENOBJECTS))
				throw CNMRException(NMR_USERABORTED);

			CModelObject * pObject = m_pModel->getObject(nObjectIndex);

			writeStartElement(XML_3MF_ELEMENT_OBJECT);
			// Write Object ID (mandatory)
			writeIntAttribute(XML_3MF_ATTRIBUTE_OBJECT_ID, pObject->getResourceID()->getUniqueID());

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
			std::string sThumbnailPath = pObject->getThumbnail();
			if (!sThumbnailPath.empty()) {
				PModelAttachment pAttachment = m_pModel->findModelAttachment(sThumbnailPath);
				if (!pAttachment)
					throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
				if (! (pAttachment->getRelationShipType() == PACKAGE_TEXTURE_RELATIONSHIP_TYPE))
					throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);

				writeStringAttribute(XML_3MF_ATTRIBUTE_OBJECT_THUMBNAIL, sThumbnailPath);
			}

			if (m_bWriteProductionExtension) {
				if (!pObject->uuid().get())
					throw CNMRException(NMR_ERROR_MISSINGUUID);
				writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_PRODUCTION, XML_3MF_PRODUCTION_UUID, pObject->uuid()->toString());
			}

			// Write Default Property Indices
			ModelResourceID nPropertyID = 0;
			ModelResourceIndex nPropertyIndex = 0;

			PModelDefaultProperty pProperty = pObject->getDefaultProperty();

			if (m_bWriteMaterialExtension) {
				// Color Properties
				CModelDefaultProperty_Color * pColorProperty = dynamic_cast<CModelDefaultProperty_Color *> (pProperty.get());
				if (pColorProperty != nullptr) {
					if (m_pColorMapping->findColor(pColorProperty->getColor(), nPropertyIndex)) {
						nPropertyID = m_pColorMapping->getResourceID();
					}
				}

				// TexCoord2D Properties
				CModelDefaultProperty_TexCoord2D * pTexCoord2DProperty = dynamic_cast<CModelDefaultProperty_TexCoord2D *> (pProperty.get());
				if (pTexCoord2DProperty != nullptr) {
					PModelWriter_TexCoordMapping pTexCoordMapping = m_pTexCoordMappingContainer->findTexture(pTexCoord2DProperty->getTextureID());
					if (pTexCoordMapping.get() != nullptr) {
						if (pTexCoordMapping->findTexCoords(pTexCoord2DProperty->getU(), pTexCoord2DProperty->getV(), nPropertyIndex)) {
							nPropertyID = pTexCoordMapping->getResourceID();
						}
					}
				}
			}

			// Base Material Properties
			CModelDefaultProperty_BaseMaterial * pBaseMaterialProperty = dynamic_cast<CModelDefaultProperty_BaseMaterial *> (pProperty.get());
			if (pBaseMaterialProperty != nullptr) {
				nPropertyID = pBaseMaterialProperty->getResourceID();
				nPropertyIndex = pBaseMaterialProperty->getResourceIndex();
			}

			// Slice extension content
			if (m_bWriteSliceExtension) {
				if (pObject->getSliceStackId() != 0) {
					writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_SLICE, XML_3MF_ATTRIBUTE_OBJECT_SLICESTACKID,
						fnUint32ToString(pObject->getSliceStackId()->getUniqueID()));
				}
				if (pObject->slicesMeshResolution() != MODELSLICESMESHRESOLUTION_FULL) {
					writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_SLICE, XML_3MF_ATTRIBUTE_OBJECT_MESHRESOLUTION,
						XML_3MF_VALUE_OBJECT_MESHRESOLUTION_LOW);
				}
			}

			// Check if object is a mesh Object
			CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject *> (pObject);
			if (pMeshObject) {
				// Write Attributes (only for meshes)
				if ( nPropertyID != 0) {
					writeIntAttribute(XML_3MF_ATTRIBUTE_OBJECT_PID, nPropertyID);
					writeIntAttribute(XML_3MF_ATTRIBUTE_OBJECT_PINDEX, nPropertyIndex);
				}

				CModelWriterNode100_Mesh ModelWriter_Mesh(pMeshObject, m_pXMLWriter, m_pProgressMonitor,
					m_pColorMapping, m_pTexCoordMappingContainer, m_bWriteMaterialExtension, m_bWriteBeamLatticeExtension);
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

	void CModelWriterNode100_Model::writeColors()
	{
		nfUint32 nCount = m_pColorMapping->getCount();
		nfUint32 nIndex;
		if (nCount > 0) {
			writeStartElementWithPrefix(XML_3MF_ELEMENT_COLORGROUP, XML_3MF_NAMESPACEPREFIX_MATERIAL);
			writeIntAttribute(XML_3MF_ATTRIBUTE_COLORS_ID, m_pColorMapping->getResourceID());
			for (nIndex = 0; nIndex < nCount; nIndex++) {
				nfColor cColor = m_pColorMapping->getColor(nIndex);
				writeStartElementWithPrefix(XML_3MF_ELEMENT_COLOR, XML_3MF_NAMESPACEPREFIX_MATERIAL);
				writeStringAttribute(XML_3MF_ATTRIBUTE_COLORS_COLOR, fnColorToString(cColor));
				writeEndElement();
			}

			writeFullEndElement();
		}
	}

	void CModelWriterNode100_Model::writeTex2Coords()
	{
		nfUint32 nGroupCount = m_pTexCoordMappingContainer->getCount();
		nfUint32 nGroupIndex;

		for (nGroupIndex = 0; nGroupIndex < nGroupCount; nGroupIndex++) {
			PModelWriter_TexCoordMapping pMapping = m_pTexCoordMappingContainer->getMapping(nGroupIndex);

			nfUint32 nCount = pMapping->getCount();
			nfUint32 nIndex;
			if (nCount > 0) {
				writeStartElementWithPrefix(XML_3MF_ELEMENT_TEX2DGROUP, XML_3MF_NAMESPACEPREFIX_MATERIAL);
				writeIntAttribute(XML_3MF_ATTRIBUTE_TEX2DGROUP_ID, pMapping->getResourceID());
				writeIntAttribute(XML_3MF_ATTRIBUTE_TEX2DGROUP_TEXTUREID, pMapping->getTextureID());
				for (nIndex = 0; nIndex < nCount; nIndex++) {
					nfFloat fU, fV;
					pMapping->getTexCoords(nIndex, fU, fV);
					writeStartElementWithPrefix(XML_3MF_ELEMENT_TEX2COORD, XML_3MF_NAMESPACEPREFIX_MATERIAL);
					writeFloatAttribute(XML_3MF_ATTRIBUTE_TEXTURE_U, fU);
					writeFloatAttribute(XML_3MF_ATTRIBUTE_TEXTURE_V, fV);
					writeEndElement();
				}

				writeFullEndElement();
			}
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
			nfUint32 nCount = m_pModel->getBuildItemCount();
			nfUint32 nIndex;
			for (nIndex = 0; nIndex < nCount; nIndex++) {
				PModelBuildItem pBuildItem = m_pModel->getBuildItem(nIndex);

				writeStartElement(XML_3MF_ELEMENT_ITEM);
				writeIntAttribute(XML_3MF_ATTRIBUTE_ITEM_OBJECTID, pBuildItem->getObjectID());
				if (!pBuildItem->getPartNumber().empty())
					writeStringAttribute(XML_3MF_ATTRIBUTE_ITEM_PARTNUMBER, pBuildItem->getPartNumber());

				if (m_bWriteProductionExtension) {
					if (!pBuildItem->uuid().get()) {
						throw CNMRException(NMR_ERROR_MISSINGUUID);
					}
					writePrefixedStringAttribute(XML_3MF_NAMESPACEPREFIX_PRODUCTION, XML_3MF_PRODUCTION_UUID, pBuildItem->uuid()->toString());
				}
				if (pBuildItem->hasTransform()) {
					writeStringAttribute(XML_3MF_ATTRIBUTE_ITEM_TRANSFORM, pBuildItem->getTransformString());
				}
				if (m_bWriteSliceExtension
					&& !pBuildItem->isValidForSlices()) {
					throw CNMRException(NMR_ERROR_SLICETRANSFORMATIONPLANAR);
				}
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
			writeIntAttribute(XML_3MF_ATTRIBUTE_COMPONENT_OBJECTID, pComponent->getObjectID());
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

	void CModelWriterNode100_Model::calculateColors(_In_ CMesh * pMesh)
	{
		nfUint32 nFaceCount = pMesh->getFaceCount();
		nfUint32 nIndex;

		CMeshInformationHandler * pHandler = pMesh->getMeshInformationHandler();

		if (pHandler) {
			CMeshInformation_NodeColors * pNodeColorInfo = dynamic_cast<CMeshInformation_NodeColors *> (pHandler->getInformationByType(0, emiNodeColors));
			if (pNodeColorInfo) {
				for (nIndex = 0; nIndex < nFaceCount; nIndex++) {
					MESHINFORMATION_NODECOLOR * pFaceData = (MESHINFORMATION_NODECOLOR*)pNodeColorInfo->getFaceData(nIndex);
					nfInt32 j;
					for (j = 0; j < 3; j++) {
						if (pFaceData->m_cColors[j] != 0)
							m_pColorMapping->registerColor(pFaceData->m_cColors[j]);
					}
				}

			}

		}

	}

	void CModelWriterNode100_Model::calculateTexCoords(_In_ CMesh * pMesh)
	{
		nfUint32 nFaceCount = pMesh->getFaceCount();
		nfUint32 nIndex;

		CMeshInformationHandler * pHandler = pMesh->getMeshInformationHandler();

		if (pHandler) {
			CMeshInformation_TexCoords * pTexCoordInfo = dynamic_cast<CMeshInformation_TexCoords *> (pHandler->getInformationByType(0, emiTexCoords));
			if (pTexCoordInfo) {
				for (nIndex = 0; nIndex < nFaceCount; nIndex++) {
					MESHINFORMATION_TEXCOORDS * pFaceData = (MESHINFORMATION_TEXCOORDS*)pTexCoordInfo->getFaceData(nIndex);
					if (pFaceData->m_TextureID != 0) {

						nfInt32 j;
						PModelWriter_TexCoordMapping pMapping = m_pTexCoordMappingContainer->findTexture(pFaceData->m_TextureID);
						if (pMapping.get() == nullptr) {
							pMapping = m_pTexCoordMappingContainer->addTexture(pFaceData->m_TextureID, generateOutputResourceID ());

						}

						for (j = 0; j < 3; j++) {
							pMapping->registerTexCoords(pFaceData->m_vCoords[j].m_fields[0], pFaceData->m_vCoords[j].m_fields[1]);
						}
					}
				}

			}

		}

	}

	ModelResourceID CModelWriterNode100_Model::generateOutputResourceID()
	{
		ModelResourceID nResourceID = m_ResourceCounter;
		if (nResourceID >= XML_3MF_MAXRESOURCECOUNT)
			throw CNMRException(NMR_ERROR_INVALIDRESOURCECOUNT);
			
		m_ResourceCounter++;

		return nResourceID;

	}

}
