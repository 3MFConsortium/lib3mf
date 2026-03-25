/*++
--*/
#include "Model/Reader/Boolean2307/NMR_ModelReaderNode_Boolean2307_BooleanShape.h"

#include "Model/Reader/Boolean2307/NMR_ModelReaderNode_Boolean2307_Boolean.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/Math/NMR_Matrix.h"

namespace NMR {
	CModelReaderNode_Boolean2307_BooleanShape::CModelReaderNode_Boolean2307_BooleanShape(_In_ CModel * pModel, _In_ PModelBooleanObject pBooleanObject, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings), m_pModel(pModel), m_pBooleanObject(pBooleanObject), m_nObjectID(0), m_bHasObjectID(false), m_bHasTransform(false), m_bHasPath(false), m_mTransform(fnMATRIX3_identity())
	{
	}

	void CModelReaderNode_Boolean2307_BooleanShape::parseXML(_In_ CXmlReader * pXMLReader)
	{
		parseName(pXMLReader);
		parseAttributes(pXMLReader);
		parseContent(pXMLReader);

		if (!m_bHasObjectID)
			throw CNMRException(NMR_ERROR_MISSINGMODELOBJECTID);

		PPackageResourceID pRID;
		if (m_bHasPath) {
			if (m_pModel->currentPath() != m_pModel->rootPath())
				throw CNMRException(NMR_ERROR_REFERENCESTOODEEP);
			pRID = m_pModel->findPackageResourceID(m_sPath, m_nObjectID);
		}
		else {
			pRID = m_pModel->findPackageResourceID(m_pModel->currentPath(), m_nObjectID);
		}

		auto pObject = pRID ? m_pModel->findObject(pRID->getUniqueID()) : nullptr;
		if (!pObject)
			throw CNMRException(NMR_ERROR_COULDNOTFINDCOMPONENTOBJECT);

		m_pBooleanObject->setBaseObject(pObject, m_mTransform);
		if (m_pBooleanObject->getOperandCount() == 0)
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);
	}

	void CModelReaderNode_Boolean2307_BooleanShape::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BOOLEAN_OBJECTID) == 0) {
			if (m_bHasObjectID)
				throw CNMRException(NMR_ERROR_DUPLICATECOMPONENTOBJECTID);
			m_nObjectID = fnStringToUint32(pAttributeValue);
			m_bHasObjectID = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BOOLEAN_TRANSFORM) == 0) {
			if (m_bHasTransform)
				throw CNMRException(NMR_ERROR_DUPLICATETRANSFORM);
			m_mTransform = fnMATRIX3_fromString(pAttributeValue);
			m_bHasTransform = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BOOLEAN_PATH) == 0) {
			if (m_bHasPath)
				throw CNMRException(NMR_ERROR_DUPLICATEPATH);
			m_sPath = pAttributeValue;
			if (!fnStartsWithPathDelimiter(m_sPath))
				m_pWarnings->addException(CNMRException(NMR_ERROR_PATH_NOT_ABSOLUTE), mrwInvalidOptionalValue);
			m_bHasPath = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BOOLEAN_OPERATION) == 0) {
			if (!m_pBooleanObject->setOperationString(pAttributeValue, false))
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_Boolean2307_BooleanShape::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_BOOLEANSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_BOOLEAN) == 0) {
				auto pXMLNode = std::make_shared<CModelReaderNode_Boolean2307_Boolean>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
				auto pObject = pXMLNode->getObject();
				if (!pObject)
					throw CNMRException(NMR_ERROR_COULDNOTFINDCOMPONENTOBJECT);
				m_pBooleanObject->addOperand(pObject, pXMLNode->getTransform());
			}
			else {
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
			}
		}
		else {
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}
}
