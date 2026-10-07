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

Abstract: This is a stub class definition of CDisp2DGroup

*/

#include "lib3mf_disp2dgroup.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_displacement2d.hpp"
#include "lib3mf_normvectorgroup.hpp"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
Class definition of CDisp2DGroup
**************************************************************************************************************************/

CDisp2DGroup::CDisp2DGroup(NMR::PModelDisp2DGroupResource pResource)
	: CResource(pResource)
{
}

NMR::PModelDisp2DGroupResource CDisp2DGroup::disp2DGroup()
{
	auto pResource = std::dynamic_pointer_cast<NMR::CModelDisp2DGroupResource>(resource());
	if (!pResource)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pResource;
}

IDisplacement2D * CDisp2DGroup::GetDisplacement2D()
{
	return new CDisplacement2D(disp2DGroup()->getDisplacement2D());
}

INormVectorGroup * CDisp2DGroup::GetNormalVectorGroup()
{
	return new CNormVectorGroup(disp2DGroup()->getNormVectorGroup());
}

Lib3MF_double CDisp2DGroup::GetHeight()
{
	return disp2DGroup()->getHeight();
}

void CDisp2DGroup::SetHeight(const Lib3MF_double dHeight)
{
	disp2DGroup()->setHeight(dHeight);
}

Lib3MF_double CDisp2DGroup::GetOffset()
{
	return disp2DGroup()->getOffset();
}

void CDisp2DGroup::SetOffset(const Lib3MF_double dOffset)
{
	disp2DGroup()->setOffset(dOffset);
}

Lib3MF_uint32 CDisp2DGroup::GetCount()
{
	return disp2DGroup()->getCount();
}

Lib3MF_uint32 CDisp2DGroup::AddCoordinate(const Lib3MF::sDisplacement2DCoordinate Coordinate)
{
	return disp2DGroup()->addCoordinate({ Coordinate.m_U, Coordinate.m_V, Coordinate.m_NormalVectorIndex, Coordinate.m_DisplacementFactor });
}

Lib3MF::sDisplacement2DCoordinate CDisp2DGroup::GetCoordinate(const Lib3MF_uint32 nIndex)
{
	auto c = disp2DGroup()->getCoordinate(nIndex);
	return Lib3MF::sDisplacement2DCoordinate{ c.m_dU, c.m_dV, c.m_nNormalVectorIndex, c.m_dDisplacementFactor };
}

void CDisp2DGroup::SetCoordinate(const Lib3MF_uint32 nIndex, const Lib3MF::sDisplacement2DCoordinate Coordinate)
{
	disp2DGroup()->setCoordinate(nIndex, { Coordinate.m_U, Coordinate.m_V, Coordinate.m_NormalVectorIndex, Coordinate.m_DisplacementFactor });
}
