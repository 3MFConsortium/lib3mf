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

NMR_MeshBeamLattice.h implements the class CMeshBeamLattice.

--*/

#include "Common/Mesh/NMR_BeamLattice.h" 

namespace NMR {

	CBeamLattice::CBeamLattice(_In_ MESHNODES &nodes) : m_Nodes(nodes)
	{ 
		m_dMinLength = 0.0001;
		m_eBallMode = eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_NONE;
		m_dDefaultBallRadius = 0.0;
	}

	void CBeamLattice::clearBeams() {
		m_Beams.clearAllData();
		m_pBeamSets.clear();
		m_OccupiedNodes.clear();
	}

	void CBeamLattice::clearBalls() {
		m_Balls.clearAllData();
	}

	void CBeamLattice::clear() {
		clearBeams();
		clearBalls();
	}

}
