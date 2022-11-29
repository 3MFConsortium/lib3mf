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

NMR_ToolpathReader.cpp implements the Toolpath Reader Class.

--*/

#include "Model/ToolpathReader/NMR_ToolpathReader.h"
#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Layer.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/MeshImport/NMR_MeshImporter_STL.h"
#include "Common/Platform/NMR_Platform.h"
#include "Model/Classes/NMR_ModelAttachment.h" 
#include "Model/Reader/NMR_ModelReader_InstructionElement.h"
#include "Model/Classes/NMR_ModelConstants.h"


#include "Common/3MF_ProgressMonitor.h"

namespace NMR {

	CToolpathReader::CToolpathReader(PModelToolpath pModelToolpath, _In_ nfBool bAllowBinaryStreams)
		: m_bAllowBinaryStreams (bAllowBinaryStreams)
	{
		if (!pModelToolpath.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pProgressMonitor = std::make_shared<CProgressMonitor>();
		m_pWarnings = std::make_shared<CModelWarnings>();

		m_pReadData = std::make_shared<CModelToolpathLayerReadData> (pModelToolpath);
	}


	void CToolpathReader::readStream(_In_ PImportStream pStream)
	{
		__NMRASSERT(pStream != nullptr);

		nfBool bHasModel = false;

		pStream->seekPosition(0, true);
						
		// Create XML Reader
		PXmlReader pXMLReader = fnCreateXMLReaderInstance(pStream, m_pProgressMonitor);

		eXmlReaderNodeType NodeType;
		// Read all XML Root Nodes
		while (!pXMLReader->IsEOF()) {
			if (!pXMLReader->Read(NodeType))
				break;

			// Get Node Name
			LPCSTR pszLocalName = nullptr;
			pXMLReader->GetLocalName(&pszLocalName, nullptr);
			if (!pszLocalName)
				throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

			if (strcmp(pszLocalName, XML_3MF_ATTRIBUTE_PREFIX_XML) == 0) {
				PModelReader_InstructionElement pXMLNode = std::make_shared<CModelReader_InstructionElement>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader.get());
			}

			// Compare with Model Node Name
			if (strcmp(pszLocalName, XML_3MF_ELEMENT_LAYER) == 0) {
				if (bHasModel)
					throw CNMRException(NMR_ERROR_DUPLICATEMODELNODE);
				bHasModel = true;

				PToolpathReaderNode_Layer pXMLNode = std::make_shared<CToolpathReaderNode_Layer>(m_pWarnings, m_pProgressMonitor, m_pReadData.get ());
				//pXMLNode->setBinaryStreamCollection(m_pBinaryStreamCollection);

				pXMLNode->parseXML(pXMLReader.get());

				//if (!pXMLNode->getHasResources())
					//throw CNMRException(NMR_ERROR_NORESOURCES);
				//if (!pXMLNode->getHasBuild())
					//throw CNMRException(NMR_ERROR_NOBUILD);
			}

		}

		//if (!bHasModel)
			//throw CNMRException(NMR_ERROR_NOMODELNODE);

	}

	PModelToolpathLayerReadData CToolpathReader::getReadData()
	{
		return m_pReadData;
	}


}
