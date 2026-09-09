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

Abstract: This is a stub class definition of CDisplacementMeshObject

*/

#include "lib3mf_displacementmeshobject.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_disp2dgroup.hpp"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
Class definition of CDisplacementMeshObject
**************************************************************************************************************************/

CDisplacementMeshObject::CDisplacementMeshObject(NMR::PModelDisplacementMeshObject pResource)
	: CResource(pResource), CObject(pResource), CMeshObject(pResource)
{
}

NMR::PModelDisplacementMeshObject CDisplacementMeshObject::displacementMeshObject()
{
	auto pResource = std::dynamic_pointer_cast<NMR::CModelDisplacementMeshObject>(resource());
	if (!pResource)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pResource;
}

bool CDisplacementMeshObject::IsDisplacementMeshObject()
{
	return true;
}

bool CDisplacementMeshObject::HasTriangleDisplacement(const Lib3MF_uint32 nIndex)
{
	return displacementMeshObject()->hasTriangleDisplacement(nIndex);
}

void CDisplacementMeshObject::SetTriangleDisplacement(const Lib3MF_uint32 nIndex, IDisp2DGroup* pDisp2DGroup, const Lib3MF::sTriangleDisplacement Displacement)
{
	if (!pDisp2DGroup)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	auto pResource = displacementMeshObject()->getModel()->findResource(pDisp2DGroup->GetResourceID());
	auto pGroup = std::dynamic_pointer_cast<NMR::CModelDisp2DGroupResource>(pResource);
	if (!pGroup)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	displacementMeshObject()->setTriangleDisplacement(nIndex, pGroup,
		Displacement.m_DisplacementIndices[0], Displacement.m_DisplacementIndices[1], Displacement.m_DisplacementIndices[2]);
}

Lib3MF::sTriangleDisplacement CDisplacementMeshObject::GetTriangleDisplacement(const Lib3MF_uint32 nIndex, IDisp2DGroup*& pDisp2DGroup)
{
	auto d = displacementMeshObject()->getTriangleDisplacement(nIndex);
	pDisp2DGroup = new CDisp2DGroup(d.m_pGroup);
	return Lib3MF::sTriangleDisplacement{ { d.m_nIndices[0], d.m_nIndices[1], d.m_nIndices[2] } };
}

void CDisplacementMeshObject::ClearTriangleDisplacement(const Lib3MF_uint32 nIndex)
{
	displacementMeshObject()->clearTriangleDisplacement(nIndex);
}
