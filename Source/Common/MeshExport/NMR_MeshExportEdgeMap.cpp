/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_MeshExportEdgeMap.cpp implements the Mesh Export Edge Map Class.
This class acts as a container for additional topology information.

--*/

#include "Common/MeshExport/NMR_MeshExportEdgeMap.h" 
#include "Common/NMR_Exception.h" 
#include <math.h>

namespace NMR {

	CMeshExportEdgeMap::CMeshExportEdgeMap(nfUint32 nFaceCount)
	{
		nfUint32 nIdx;
		m_FaceInfos.resize(nFaceCount);

		NMESHEXPORTFACE ExportFace;
		ExportFace.m_edgeindices[0] = -1;
		ExportFace.m_edgeindices[1] = -1;
		ExportFace.m_edgeindices[2] = -1;

		for (nIdx = 0; nIdx < nFaceCount; nIdx++) 
			m_FaceInfos[nIdx] = ExportFace;
	}

	NMESHEXPORTFACE CMeshExportEdgeMap::getFaceData(_In_ nfInt32 nIdx)
	{
		return m_FaceInfos[nIdx];
	}

	void CMeshExportEdgeMap::setFaceData(_In_ nfInt32 nIdx, _In_ NMESHEXPORTFACE ExportFace)
	{
		m_FaceInfos[nIdx] = ExportFace;
	}

}
