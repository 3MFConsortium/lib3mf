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

NMR_MeshImporter_STL.h defines the Mesh Importer Class.
This is a derived class for Importing the binary STL and color STL Mesh Format.

--*/

#ifndef __NMR_MESHIMPORTER_STL
#define __NMR_MESHIMPORTER_STL

#include "Common/NMR_Types.h" 
#include "Common/Math/NMR_Geometry.h" 
#include "Common/Mesh/NMR_Mesh.h" 
#include "Common/MeshImport/NMR_MeshImporter.h" 
#include "Common/NMR_Architecture_Utils.h"

#include <vector>

namespace NMR {

#pragma pack (1)
	typedef struct MESHFORMAT_STL_FACET {
		NVEC3 m_normal;
		NVEC3 m_vertices[3];
		nfUint16 m_attribute;
		void swapByteOrder() {
			m_attribute = swapBytes(m_attribute);
			for (nfUint32 i = 0; i < 3; i++) {
				m_normal.m_fields[i] = swapBytes(m_normal.m_fields[i]);
				for (nfUint32 j = 0; j < 3; j++) {
					m_vertices[i].m_fields[j] = swapBytes(m_vertices[i].m_fields[j]);
				}
			}
		}
	} MESHFORMAT_STL_FACET;
#pragma pack()

	class CMeshImporter_STL : public CMeshImporter {
	private:
		nfFloat m_fUnits;
		nfBool m_bIgnoreInvalidFaces;
		nfBool m_bImportColors;

	public:
		CMeshImporter_STL();
		CMeshImporter_STL(_In_ PImportStream pStream);
		CMeshImporter_STL(_In_ PImportStream pStream, _In_ nfFloat fUnits);
		CMeshImporter_STL(_In_ PImportStream pStream, _In_ nfFloat fUnits, _In_ nfBool bImportColors);

		void setUnits(_In_ nfFloat fUnits);
		nfFloat getUnits();
		void setIgnoreInvalidFaces(_In_ nfBool bIgnoreInvalidFaces);
		nfBool getIgnoreInvalidFaces();
		void setImportColors(_In_ nfBool bImportColors);
		nfBool getImportColors();

		virtual void loadMesh(_In_ CMesh * pMesh, _In_opt_ NMATRIX3 * pmMatrix);
	};

}

#endif // __NMR_MESHIMPORTER_STL
