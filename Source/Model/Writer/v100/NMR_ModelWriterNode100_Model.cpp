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

NMR_ModelWriterNode100_Model.cpp implements the Model Writer Model Node Class.
This is the class for exporting the 3mf model stream root node.

--*/

#include "Model/Writer/v100/NMR_ModelWriterNode100_Mesh.h"
#include "Model/Writer/v100/NMR_ModelWriterNode100_Model.h"

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

namespace NMR {

	CModelWriterNode100_Model::CModelWriterNode100_Model(_In_ CModel * pModel, _In_ CXmlWriter * pXMLWriter)
		:CModelWriterNode(pModel, pXMLWriter)
	{
		m_ResourceCounter = pModel->generateResourceID();

		m_pColorMapping = std::make_shared<CModelWriter_ColorMapping>(generateOutputResourceID ());

		m_pTexCoordMappingContainer = std::make_shared<CModelWriter_TexCoordMappingContainer>();

		nfInt32 nObjectCount = pModel->getObjectCount();
		nfInt32 nObjectIndex;

		for (nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex++) {
			PModelResource pResource = pModel->getObjectResource(nObjectIndex);

			CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject*> (pResource.get());
			if (pMeshObject) {
				CMesh * pMesh = pMeshObject->getMesh();
				if (pMesh) {
					calculateColors(pMesh);
					calculateTexCoords(pMesh);
				}
			}

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

	void CModelWriterNode100_Model::writeToXML()
	{
		std::wstring sLanguage = m_pModel->getLanguage();

		writeStartElementWithNamespace(XML_3MF_ELEMENT_MODEL, PACKAGE_XMLNS_100);

		writeStringAttribute(XML_3MF_ATTRIBUTE_MODEL_UNIT, m_pModel->getUnitString());
		writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_PREFIX_XML, XML_3MF_ATTRIBUTE_MODEL_LANG, sLanguage.c_str());
		writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_XMLNS, XML_3MF_NAMESPACEPREFIX_MATERIAL, XML_3MF_NAMESPACE_MATERIALSPEC);

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
			writeIntAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_ID, pTexture2D->getResourceID());
			writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_PATH, pTexture2D->getPath());
			writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_CONTENTTYPE, pTexture2D->getContentTypeString());

			std::wstring sTileStyle;
			sTileStyle = pTexture2D->getTileStyleU();
			if (sTileStyle.size() > 0)
				writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEU, sTileStyle);

			sTileStyle = pTexture2D->getTileStyleV();
			if (sTileStyle.size() > 0)
				writeStringAttribute(XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEV, sTileStyle);

			writeEndElement();

		}

	}

	void CModelWriterNode100_Model::writeMetaData()
	{
		nfUint32 nMetaDataCount = m_pModel->getMetaDataCount();
		nfUint32 nMetaDataIndex;

		for (nMetaDataIndex = 0; nMetaDataIndex < nMetaDataCount; nMetaDataIndex++) {
			std::wstring sKey;
			std::wstring sValue;
			m_pModel->getMetaData(nMetaDataIndex, sKey, sValue);

			writeStartElement(XML_3MF_ELEMENT_METADATA);
			writeStringAttribute(XML_3MF_ATTRIBUTE_METADATA_NAME, sKey);
			writeText(sValue.c_str(), (nfUint32)sValue.length());
			writeEndElement();
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
			writeIntAttribute(XML_3MF_ATTRIBUTE_BASEMATERIALS_ID, pBaseMaterial->getResourceID());

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

	void CModelWriterNode100_Model::writeObjects()
	{
		nfUint32 nObjectCount = m_pModel->getObjectCount();
		nfUint32 nObjectIndex;

		for (nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex++) {
			CModelObject * pObject = m_pModel->getObject(nObjectIndex);

			writeStartElement(XML_3MF_ELEMENT_OBJECT);
			// Write Object ID (mandatory)
			writeIntAttribute(XML_3MF_ATTRIBUTE_OBJECT_ID, pObject->getResourceID());

			// Write Object Name (optional)
			std::wstring sObjectName = pObject->getName();
			if (sObjectName.length() > 0)
				writeStringAttribute(XML_3MF_ATTRIBUTE_OBJECT_NAME, sObjectName);

			// Write Object Partnumber (optional)
			std::wstring sObjectPartNumber = pObject->getPartNumber();
			if (sObjectPartNumber.length() > 0)
				writeStringAttribute(XML_3MF_ATTRIBUTE_OBJECT_PARTNUMBER, sObjectPartNumber);

			// Write Object Type (optional)
			writeStringAttribute(XML_3MF_ATTRIBUTE_OBJECT_TYPE, pObject->getObjectTypeString());

			// Write Default Property Indices
			ModelResourceID nPropertyID = 0;
			ModelResourceIndex nPropertyIndex = 0;

			PModelDefaultProperty pProperty = pObject->getDefaultProperty();
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

			// Base Material Properties
			CModelDefaultProperty_BaseMaterial * pBaseMaterialProperty = dynamic_cast<CModelDefaultProperty_BaseMaterial *> (pProperty.get());
			if (pBaseMaterialProperty != nullptr) {
				nPropertyID = pBaseMaterialProperty->getResourceID();
				nPropertyIndex = pBaseMaterialProperty->getResourceIndex();
			}

			// Write Attributes
			if (nPropertyID != 0) {
				writeIntAttribute(XML_3MF_ATTRIBUTE_OBJECT_PID, nPropertyID);
				writeIntAttribute(XML_3MF_ATTRIBUTE_OBJECT_PINDEX, nPropertyIndex);
			}


			// Check if object is a mesh Object
			CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject *> (pObject);
			if (pMeshObject) {
				CModelWriterNode100_Mesh ModelWriter_Mesh(pMeshObject, m_pXMLWriter, m_pColorMapping, m_pTexCoordMappingContainer);
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
				writeStringAttribute(XML_3MF_ATTRIBUTE_COLORS_COLOR, fnColorToWString(cColor));
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

		writeBaseMaterials();
		writeTextures2D();
		writeColors();
		writeTex2Coords();
		writeObjects();

		writeFullEndElement();
	}

	void CModelWriterNode100_Model::writeBuild()
	{
		writeStartElement(XML_3MF_ELEMENT_BUILD);
		nfUint32 nCount = m_pModel->getBuildItemCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			PModelBuildItem pBuildItem = m_pModel->getBuildItem(nIndex);

			writeStartElement(XML_3MF_ELEMENT_ITEM);
			writeIntAttribute(XML_3MF_ATTRIBUTE_ITEM_OBJECTID, pBuildItem->getObjectID());
			if (pBuildItem->hasTransform())
				writeStringAttribute(XML_3MF_ATTRIBUTE_ITEM_TRANSFORM, pBuildItem->getTransformString());
			writeEndElement();
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
