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

NMR_MeshBeamLattice.h defines the class CMeshBeamLattice.

--*/

#ifndef __NMR_MESHBEAMLATTICE
#define __NMR_MESHBEAMLATTICE

#include "Common/Math/NMR_Geometry.h"
#include "Common/Mesh/NMR_MeshTypes.h"
#include "Common/NMR_Types.h"
#include "Model/Classes/NMR_ModelTypes.h"

#include <unordered_set>

namespace NMR {

	class CBeamLattice {
	private:
		friend class CMesh;

		MESHNODES &m_Nodes;	// reference to the nodes of the parent mesh
		std::unordered_set<nfInt32> m_OccupiedNodes;    // datastructure used to ensure that balls are only placed at nodes with beams
		MESHBEAMS m_Beams;
		std::vector<PBEAMSET> m_pBeamSets;
		MESHBALLS m_Balls;
		
		nfDouble m_dMinLength;
		eModelBeamLatticeBallMode m_eBallMode;
		nfDouble m_dDefaultBallRadius;
	public:
		CBeamLattice(_In_ MESHNODES &nodes);

		void clearBeams();
		void clearBalls();
		void clear();
	};

	typedef std::shared_ptr <CBeamLattice> PBeamLattice;

}

#endif // __NMR_BEAMLATTICE
