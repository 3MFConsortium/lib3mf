/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CBeamLattice

*/

#include "lib3mf_beamlattice.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CBeamLattice 
**************************************************************************************************************************/

Lib3MF_double CBeamLattice::GetMinLength()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::SetMinLength(const Lib3MF_double dMinLength)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::GetClipping(Lib3MF::eBeamLatticeClipMode & eClipMode, Lib3MF_uint32 & nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::SetClipping(const Lib3MF::eBeamLatticeClipMode eClipMode, const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CBeamLattice::GetRepresentation(Lib3MF_uint32 & nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::SetRepresentation(const Lib3MF_uint32 nUniqueResourceID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::GetBallOptions(Lib3MF::eBeamLatticeBallMode & eBallMode, Lib3MF_double & dBallRadius)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::SetBallOptions(const Lib3MF::eBeamLatticeBallMode eBallMode, const Lib3MF_double dBallRadius)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CBeamLattice::GetBeamCount()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF::sBeam CBeamLattice::GetBeam(const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CBeamLattice::AddBeam(const Lib3MF::sBeam BeamInfo)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::SetBeam(const Lib3MF_uint32 nIndex, const Lib3MF::sBeam BeamInfo)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::SetBeams(const Lib3MF_uint64 nBeamInfoBufferSize, const Lib3MF::sBeam * pBeamInfoBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::GetBeams(Lib3MF_uint64 nBeamInfoBufferSize, Lib3MF_uint64* pBeamInfoNeededCount, Lib3MF::sBeam * pBeamInfoBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CBeamLattice::GetBallCount()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF::sBall CBeamLattice::GetBall(const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CBeamLattice::AddBall(const Lib3MF::sBall BallInfo)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::SetBall(const Lib3MF_uint32 nIndex, const Lib3MF::sBall BallInfo)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::SetBalls(const Lib3MF_uint64 nBallInfoBufferSize, const Lib3MF::sBall * pBallInfoBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CBeamLattice::GetBalls(Lib3MF_uint64 nBallInfoBufferSize, Lib3MF_uint64* pBallInfoNeededCount, Lib3MF::sBall * pBallInfoBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CBeamLattice::GetBeamSetCount()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IBeamSet * CBeamLattice::AddBeamSet()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IBeamSet * CBeamLattice::GetBeamSet(const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

