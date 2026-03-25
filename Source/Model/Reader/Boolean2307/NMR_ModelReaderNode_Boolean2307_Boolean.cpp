/*++
--*/
#include "Model/Reader/Boolean2307/NMR_ModelReaderNode_Boolean2307_Boolean.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/Math/NMR_Matrix.h"

namespace NMR {
	CModelReaderNode_Boolean2307_Boolean::CModelReaderNode_Boolean2307_Boolean(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings), m_pModel(pModel), m_nObjectID(0), m_bHasObjectID(false), m_bHasTransform(false), m_bHasPath(false), m_mTransform(fnMATRIX3_identity())
	{
	}

	void CModelReaderNode_Boolean2307_Boolean::parseXML(_In_ CXmlReader * pXMLReader)
	{
		parseName(pXMLReader);
		parseAttributes(pXMLReader);
		parseContent(pXMLReader);
	}

	void CModelReaderNode_Boolean2307_Boolean::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
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
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}

	CModelObject * CModelReaderNode_Boolean2307_Boolean::getObject()
	{
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

		if (pRID.get())
			return m_pModel->findObject(pRID->getUniqueID());

		return nullptr;
	}

	NMATRIX3 CModelReaderNode_Boolean2307_Boolean::getTransform() const
	{
		return m_mTransform;
	}
}
