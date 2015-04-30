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

NMR_ModelReader_3MF.cpp implements the Model Reader Class for
3MF Files. A 3MF model reader reads in a 3MF file and generates an in-memory representation of it.

--*/

#include "Model/Reader/NMR_ModelReader_3MF.h"
#include "Model/Reader/NMR_ModelReaderNode_Model.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelBuildItem.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/MeshImport/NMR_MeshImporter_STL.h"
#include "Common/Platform/NMR_Platform.h"

namespace NMR {

	CModelReader_3MF::CModelReader_3MF(_In_ PModel pModel)
		: CModelReader(pModel)
	{
		// empty on purpose
	}

	void CModelReader_3MF::readStream(_In_ PImportStream pStream)
	{
		__NMRASSERT(pStream != nullptr);

		// Extract Stream from Package
		PImportStream pModelStream = extract3MFOPCPackage(pStream);
		pModelStream->seekPosition(0, true);

		// Create XML Reader
		PXmlReader pXMLReader = fnCreateXMLReaderInstance(pModelStream);

		eXmlReaderNodeType NodeType;
		// Read all XML Root Nodes
		while (!pXMLReader->IsEOF()) {
			if (!pXMLReader->Read(NodeType))
				break;

			// Get Node Name
			LPCWSTR pwszQualifiedName = nullptr;
			pXMLReader->GetQualifiedName(&pwszQualifiedName, nullptr);
			if (!pwszQualifiedName)
				throw CNMRException(NMR_ERROR_COULDNOTGETQUALIFIEDXMLNAME);

			// Compare with Model Node Name
			if (wcscmp(pwszQualifiedName, XML_3MF_ELEMENT_MODEL) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_Model>(m_pModel.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader.get());
			}
		}

		// Release Memory of 3MF Package
		release3MFOPCPackage();
	}

}
