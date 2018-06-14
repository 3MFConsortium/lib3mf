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

NMR_Mesh.h defines the class CMesh.

The class CMesh is not really a mesh, since it lacks the component edges and the
topological information. It only holds the nodes and the faces (triangles).
Each node,  and face have a ID, which allows to identify them. Each face have an
orientation (i.e. the face can look up or look down) and have three nodes.
The orientation is defined by the order of its nodes.

You can only add nodes and faces to mesh. You cannot remove the existing structure.

--*/

#ifndef __NMR_MESH
#define __NMR_MESH

#include "Common/Math/NMR_Geometry.h"
#include "Common/Mesh/NMR_MeshTypes.h"
#include "Common/MeshInformation/NMR_MeshInformationHandler.h"
#include "Common/NMR_Types.h"
#include "Common/Mesh/NMR_BeamLattice.h"

namespace NMR {

	class CMesh {
	private:
		MESHNODES m_Nodes;
		MESHFACES m_Faces;
		CBeamLattice m_BeamLattice;

		PMeshInformationHandler m_pMeshInformationHandler;

	public:
		CMesh();
		CMesh(_In_opt_ CMesh * pMesh);

		void mergeMesh(_In_opt_ CMesh * pMesh);
		void addToMesh(_In_opt_ CMesh * pMesh);
		void mergeMesh(_In_opt_ CMesh * pMesh, _In_ NMATRIX3 mMatrix);
		void addToMesh(_In_opt_ CMesh * pMesh, _In_ NMATRIX3 mMatrix);

		_Ret_notnull_ MESHNODE * addNode(_In_ const NVEC3 vPosition);
		_Ret_notnull_ MESHFACE * addFace(_In_ MESHNODE * pNode1, _In_ MESHNODE * pNode2, _In_ MESHNODE * pNode3);
		_Ret_notnull_ MESHBEAM * addBeam(_In_ MESHNODE * pNode1, _In_ MESHNODE * pNode2, _In_ nfDouble * pRadius1, _In_ nfDouble * pRadius2,
								_In_ nfInt32 * peCapMode1, _In_ nfInt32 * peCapMode2);
		_Ret_notnull_ PBEAMSET addBeamSet();
		
		nfUint32 getNodeCount();
		nfUint32 getFaceCount();
		nfUint32 getBeamCount();
		nfUint32 getBeamSetCount();

		_Ret_notnull_ MESHNODE * getNode(_In_ nfUint32 nIdx);
		_Ret_notnull_ MESHFACE * getFace(_In_ nfUint32 nIdx);
		_Ret_notnull_ MESHBEAM * getBeam(_In_ nfUint32 nIdx);
		_Ret_notnull_ PBEAMSET getBeamSet(_In_ nfUint32 nIdx);

		void setBeamLatticeMinLength(nfDouble dMinLength);
		nfDouble getBeamLatticeMinLength();
		void setDefaultBeamRadius(nfDouble dRadius);
		nfDouble getDefaultBeamRadius();
		void setBeamLatticeAccuracy(nfDouble dAccuracy);
		nfBool getBeamLatticeAccuracy(nfDouble * pdAccuracy);
		void setBeamLatticeCapMode(eModelBeamLatticeCapMode dRadius);
		eModelBeamLatticeCapMode getBeamLatticeCapMode();

		nfBool checkSanity();

		void clear();
		void clearBeamLattice();

		_Ret_maybenull_ CMeshInformationHandler * getMeshInformationHandler();
		_Ret_notnull_ CMeshInformationHandler * createMeshInformationHandler();
		void clearMeshInformationHandler();
	};

	typedef std::shared_ptr <CMesh> PMesh;

}

#endif // __NMR_MESH
