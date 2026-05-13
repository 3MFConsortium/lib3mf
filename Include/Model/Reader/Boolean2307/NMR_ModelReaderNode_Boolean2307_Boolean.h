/*++
--*/
#ifndef __NMR_MODELREADERNODE_BOOLEAN2307_BOOLEAN
#define __NMR_MODELREADERNODE_BOOLEAN2307_BOOLEAN

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelObject.h"

namespace NMR {
	class CModelReaderNode_Boolean2307_Boolean : public CModelReaderNode {
	private:
		CModel * m_pModel;
		ModelResourceID m_nObjectID;
		std::string m_sPath;
		nfBool m_bHasObjectID;
		nfBool m_bHasTransform;
		nfBool m_bHasPath;
		NMATRIX3 m_mTransform;

	public:
		CModelReaderNode_Boolean2307_Boolean() = delete;
		CModelReaderNode_Boolean2307_Boolean(_In_ CModel * pModel, _In_ PModelWarnings pWarnings);

		void parseXML(_In_ CXmlReader * pXMLReader) override;
		CModelObject * getObject();
		NMATRIX3 getTransform() const;

protected:
		void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue) override;
		void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace) override;
	};
}

#endif
