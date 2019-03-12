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

NMR_MeshExporter.h defines the Mesh Exporter Class.
This is an abstract base class for exporting different Mesh Formats.

--*/

#ifndef __NMR_MESHEXPORTER
#define __NMR_MESHEXPORTER

#include "Common/NMR_Types.h" 
#include "Common/Math/NMR_Geometry.h" 
#include "Common/Mesh/NMR_Mesh.h" 
#include "Common/MeshExport/NMR_MeshExportEdgeMap.h"
#include "Common/Platform/NMR_ExportStream.h" 

#include <vector>

namespace NMR {

	class CMeshExporter {
	private:
		PExportStream m_stream;
	public:
		CMeshExporter();
		CMeshExporter(_In_ PExportStream pStream);

		void setStream(_In_ PExportStream pStream);
		CExportStream * getStream();

		virtual void exportMeshEx(_In_ CMesh * pMesh, _In_opt_ NMATRIX3 * pmMatrix, _In_opt_ CMeshExportEdgeMap * pExportEdgeMap) = 0;

		void exportMesh(_In_ CMesh * pMesh, _In_opt_ NMATRIX3 * pmMatrix);
	};

	typedef std::shared_ptr <CMeshExporter> PMeshExporter;

}

#endif // __NMR_MESHEXPORTER
