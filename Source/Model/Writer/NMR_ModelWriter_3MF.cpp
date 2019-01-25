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

NMR_ModelWriter_3MF.cpp implements the 3MF Model Writer Class.
A model writer exports the in memory represenation into a 3MF file.

--*/

#include "Model/Writer/NMR_ModelWriter_3MF.h"
#include "Model/Writer/v100/NMR_ModelWriterNode100_Model.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <sstream>

#include "Common/Platform/NMR_XmlWriter.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/3MF_ProgressMonitor.h"

namespace NMR {

	CModelWriter_3MF::CModelWriter_3MF(_In_ PModel pModel) : CModelWriter(pModel)
	{
		// empty on purpose
	}

	void CModelWriter_3MF::exportToStream(_In_ PExportStream pStream)
	{
		if (pStream == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pProgressMonitor->ResetLevels();
		if (!m_pProgressMonitor->Progress(0.01, ProgressIdentifier::PROGRESS_CREATEOPCPACKAGE))
			throw CNMRException(NMR_USERABORTED);

		// Create new OPC Package
		createPackage(m_pModel.get());

		if (!m_pProgressMonitor->Progress(0.05, ProgressIdentifier::PROGRESS_WRITEMODELSTOSTREAM))
			throw CNMRException(NMR_USERABORTED);

		m_pProgressMonitor->PushLevel(0.05, 0.99);
		// Write Package to Stream
		writePackageToStream(pStream);
		m_pProgressMonitor->PopLevel();

		m_pProgressMonitor->Progress(0.99, ProgressIdentifier::PROGRESS_CLEANUP);
		
		// Release Memory
		releasePackage();

		if (!m_pProgressMonitor->Progress(1, ProgressIdentifier::PROGRESS_DONE))
			throw CNMRException(NMR_USERABORTED);
	}

	void CModelWriter_3MF::writeSlicestackStream(_In_ CXmlWriter *pXMLWriter, _In_ CModel * pModel, _In_ CModelSliceStack *pSliceStackResource)
	{
		__NMRASSERT(pSliceStackResource != nullptr);
		if (pXMLWriter == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		pXMLWriter->WriteStartDocument();
		CModelWriterNode100_Model ModelNode(pModel, pXMLWriter, m_pProgressMonitor.get(), pSliceStackResource);
		ModelNode.writeToXML();

		pXMLWriter->WriteEndDocument();
		pXMLWriter->Flush();
	}

	void CModelWriter_3MF::writeModelStream(_In_ CXmlWriter * pXMLWriter, _In_ CModel * pModel)
	{
		__NMRASSERT(pModel != nullptr);
		if (pXMLWriter == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		pXMLWriter->WriteStartDocument();

		CModelWriterNode100_Model ModelNode(pModel, pXMLWriter, m_pProgressMonitor.get());
		ModelNode.writeToXML();

		pXMLWriter->WriteEndDocument();

		pXMLWriter->Flush();
	}

}
