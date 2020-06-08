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

NMR_ModelWriter_STL.cpp implements the STL Model Writer Class.
A model writer exports the in memory representation into a binary STL file.

--*/

#include "Model/Writer/NMR_ModelWriter_STL.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/MeshExport/NMR_MeshExporter_STL.h"
#include <sstream>

namespace NMR {

	CModelWriter_STL::CModelWriter_STL(_In_ PModel pModel) : CModelWriter(pModel)
	{
		// empty on purpose
	}

	void CModelWriter_STL::exportToStream(_In_ PExportStream pStream)
	{
		if (!pStream.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		// Create Merged Mesh
		PMesh pMesh = std::make_shared<CMesh>();
		model()->mergeToMesh(pMesh.get());

		// Export Merged Mesh to STL
		PMeshExporter pExporter = std::make_shared<CMeshExporter_STL>(pStream);
		pExporter->exportMesh(pMesh.get(), nullptr);
	}

}
