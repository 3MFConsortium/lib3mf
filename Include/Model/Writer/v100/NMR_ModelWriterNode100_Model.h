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

NMR_ModelWriterNode_Model.h defines the Model Writer Model Node Class.
This is the class for exporting the 3mf model stream root node.

--*/

#ifndef __NMR_MODELWRITERNODE100_MODEL
#define __NMR_MODELWRITERNODE100_MODEL

#include "Model/Writer/NMR_ModelWriterNode.h" 
#include "Model/Writer/NMR_ModelWriter_ColorMapping.h" 
#include "Model/Classes/NMR_ModelComponentsObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Common/Platform/NMR_XmlWriter.h"
#include "Model/Writer/NMR_ModelWriter_TexCoordMappingContainer.h"

namespace NMR {

	class CModelWriterNode100_Model : public CModelWriterNode {
	protected:
		PModelWriter_ColorMapping m_pColorMapping;
		PModelWriter_TexCoordMappingContainer m_pTexCoordMappingContainer;
		ModelResourceID m_ResourceCounter;
		
		nfBool m_bWriteMaterialExtension;
		nfBool m_bWriteProductionExtension;
		nfBool m_bWriteBeamLatticeExtension;
		nfBool m_bWriteSliceExtension;
		nfBool m_bWriteBaseMaterials;
		nfBool m_bWriteObjects;
		nfBool m_bIsRootModel;
		nfBool m_bWriteCustomNamespaces;

		void writeModelMetaData();
		void writeMetaData(_In_ PModelMetaData pMetaData);
		void writeMetaDataGroup(_In_ PModelMetaDataGroup pMetaDataGroup);

		void writeResources();
		void writeBaseMaterials();
		void writeTextures2D();
		void writeColors();
		void writeTex2Coords();
		void writeObjects();
		void writeBuild();
		void writeSliceStack(_In_ CModelSliceStack *pSliceStackResource);

		void writeSliceStacks();

		void writeComponentsObject(_In_ CModelComponentsObject * pComponentsObject);

		ModelResourceID generateOutputResourceID();

		void RegisterMetaDataGroupNameSpaces(PModelMetaDataGroup mdg);
		void CModelWriterNode100_Model::RegisterMetaDataNameSpaces();

	public:
		CModelWriterNode100_Model() = delete;
		CModelWriterNode100_Model(_In_ CModel * pModel, _In_ CXmlWriter * pXMLWriter, _In_ CProgressMonitor * pProgressMonitor);
		CModelWriterNode100_Model(_In_ CModel * pModel, _In_ CXmlWriter * pXMLWriter, _In_ CProgressMonitor * pProgressMonitor, nfBool bWritesRootModel);
		
		virtual void writeToXML();
	};

}

#endif // __NMR_MODELWRITERNODE100_MODEL
