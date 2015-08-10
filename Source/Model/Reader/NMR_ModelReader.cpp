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
NMR_ModelReader.cpp implements the Model Reader Class.
A model reader reads in a model file and generates an in-memory representation of it.

--*/

#include "Model/Reader/NMR_ModelReader.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/Platform/NMR_ImportStream.h" 

#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Model/Classes/NMR_ModelBuildItem.h" 

namespace NMR {

	CModelReader::CModelReader(_In_ PModel pModel)
	{
		if (!pModel.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pModel = pModel;
		m_pWarnings = std::make_shared<CModelReaderWarnings>();

		// Clear all legacy settings
		m_pModel->clearAll();
	}

	void CModelReader::readFromMeshImporter(_In_ CMeshImporter * pImporter)
	{
		__NMRASSERT(pImporter);

		// Create Empty Mesh
		PMesh pMesh = std::make_shared<CMesh>();

		// Import Mesh
		pImporter->loadMesh(pMesh.get(), nullptr);

		// Add Single Mesh to Model 
		PModelMeshObject pMeshObject = std::make_shared<CModelMeshObject>(m_pModel->generateResourceID(), m_pModel.get(), pMesh);
		m_pModel->addResource(pMeshObject);

		// Add Build Item to Model 
		PModelBuildItem pBuildItem = std::make_shared<CModelBuildItem>(pMeshObject.get(), m_pModel->createHandle());
		m_pModel->addBuildItem(pBuildItem);
	}

	PImportStream CModelReader::retrievePrintTicket(_Out_ std::wstring & sContentType)
	{
		sContentType = m_sPrintTicketContentType;
		return m_pPrintTicketStream;
	}

	PModelReaderWarnings CModelReader::getWarnings()
	{
		return m_pWarnings;
	}

}
