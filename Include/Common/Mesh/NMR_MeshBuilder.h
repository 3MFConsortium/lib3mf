/*++

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

NMR_MeshBuilder.h defines the class CMeshBuilder.

The class CMeshBuilder is auxiliary class, which construct a valid mesh by adding single faces and nodes.
In addition already existing meshes can be added.

--*/

#ifndef __NMR_MESHBUILDER
#define __NMR_MESHBUILDER

#include "Common/Mesh/NMR_Mesh.h" 
#include "Common/MeshInformation/NMR_MeshInformationHandler.h" 
#include "Common/Math/NMR_VectorTree.h" 

namespace NMR {

	class CMeshBuilder {
	private:
		CVectorTree m_VectorTree;
		CPagedVector<NVEC3> m_Nodes;
		CPagedVector<NVEC3I> m_Faces;

	public:
		CMeshBuilder();
		CMeshBuilder(_In_ nfFloat fUnits);

		nfUint32 addNode(_In_ const NVEC3 vPoint);
		nfUint32 addFace(_In_ const NVEC3 vPoint1, _In_ const NVEC3 vPoint2, _In_ const NVEC3 vPoint3);

		PMesh createMesh(_In_ nfBool bIgnoreInvalidFaces);
		void addToMesh(_In_ CMesh * pMesh, _In_ nfBool bIgnoreInvalidFaces);
	};

	typedef std::shared_ptr <CMeshBuilder> PMeshBuilder;

}

#endif // __NMR_MESHBUILDER
