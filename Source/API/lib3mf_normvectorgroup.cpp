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

Abstract: This is a stub class definition of CNormVectorGroup

*/

#include "lib3mf_normvectorgroup.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
Class definition of CNormVectorGroup
**************************************************************************************************************************/

CNormVectorGroup::CNormVectorGroup(NMR::PModelNormVectorGroupResource pResource)
	: CResource(pResource)
{
}

NMR::PModelNormVectorGroupResource CNormVectorGroup::normVectorGroup()
{
	auto pResource = std::dynamic_pointer_cast<NMR::CModelNormVectorGroupResource>(resource());
	if (!pResource)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pResource;
}

Lib3MF_uint32 CNormVectorGroup::GetCount()
{
	return normVectorGroup()->getCount();
}

Lib3MF_uint32 CNormVectorGroup::AddVector(const Lib3MF::sVector Vector)
{
	return normVectorGroup()->addVector({ Vector.m_Coordinates[0], Vector.m_Coordinates[1], Vector.m_Coordinates[2] });
}

Lib3MF::sVector CNormVectorGroup::GetVector(const Lib3MF_uint32 nIndex)
{
	auto v = normVectorGroup()->getVector(nIndex);
	return Lib3MF::sVector{ { v[0], v[1], v[2] } };
}

void CNormVectorGroup::SetVector(const Lib3MF_uint32 nIndex, const Lib3MF::sVector Vector)
{
	normVectorGroup()->setVector(nIndex, { Vector.m_Coordinates[0], Vector.m_Coordinates[1], Vector.m_Coordinates[2] });
}
