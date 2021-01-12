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
NMR_ModelReader.cpp implements the Model Reader Class.
A model reader reads in a model file and generates an in-memory representation of it.

--*/

#include "Model/Reader/NMR_ModelReader.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/Platform/NMR_ImportStream.h" 
#include "Common/NMR_SecureContext.h"
#include "Common/3MF_ProgressMonitor.h"
#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Model/Classes/NMR_ModelBuildItem.h" 

namespace NMR {

	CModelReader::CModelReader(_In_ PModel pModel)
		:CModelContext(pModel)
	{
	}

	void CModelReader::readFromMeshImporter(_In_ CMeshImporter * pImporter)
	{
		__NMRASSERT(pImporter);

		// Create Empty Mesh
		PMesh pMesh = std::make_shared<CMesh>();

		// Import Mesh
		pImporter->loadMesh(pMesh.get(), nullptr);

		// Add Single Mesh to Model 
		PModelMeshObject pMeshObject = std::make_shared<CModelMeshObject>(model()->generateResourceID(), model().get(), pMesh);
		model()->addResource(pMeshObject);

		// Add Build Item to Model 
		PModelBuildItem pBuildItem = std::make_shared<CModelBuildItem>(pMeshObject.get(), model()->createHandle());
		model()->addBuildItem(pBuildItem);
	}

	PImportStream CModelReader::retrievePrintTicket(_Out_ std::string & sContentType)
	{
		sContentType = m_sPrintTicketContentType;
		return m_pPrintTicketStream;
	}

	void CModelReader::addRelationToRead(_In_ std::string sRelationShipType)
	{
		m_RelationsToRead.insert(sRelationShipType);
	}

	void CModelReader::removeRelationToRead(_In_ std::string sRelationShipType)
	{
		m_RelationsToRead.erase(sRelationShipType);
	}

}
