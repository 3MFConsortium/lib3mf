/*++
--*/
#ifndef __NMR_MODELREADERNODE_BOOLEAN2307_BOOLEANSHAPE
#define __NMR_MODELREADERNODE_BOOLEAN2307_BOOLEANSHAPE

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelBooleanObject.h"

namespace NMR {
	class CModelReaderNode_Boolean2307_BooleanShape : public CModelReaderNode {
	private:
		CModel * m_pModel;
		PModelBooleanObject m_pBooleanObject;
		ModelResourceID m_nObjectID;
		std::string m_sPath;
		nfBool m_bHasObjectID;
		nfBool m_bHasTransform;
		nfBool m_bHasPath;
		NMATRIX3 m_mTransform;

	public:
		CModelReaderNode_Boolean2307_BooleanShape() = delete;
		CModelReaderNode_Boolean2307_BooleanShape(_In_ CModel * pModel, _In_ PModelBooleanObject pBooleanObject, _In_ PModelWarnings pWarnings);

		void parseXML(_In_ CXmlReader * pXMLReader) override;

	protected:
		void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue) override;
		void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace) override;
		void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader) override;
	};
}

#endif
