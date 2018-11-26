/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CLib3MFMeshObject

*/

#include "lib3mf_meshobject.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF;

/*************************************************************************************************************************
 Class definition of CLib3MFMeshObject 
**************************************************************************************************************************/

CLib3MFMeshObject::CLib3MFMeshObject(NMR::PModelResource pResource)
	: CLib3MFObject(pResource)
{
}

Lib3MF_uint32 CLib3MFMeshObject::GetVertexCount ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CLib3MFMeshObject::GetTriangleCount ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::SetVertex (const Lib3MF_uint32 nIndex, const sLib3MFPosition Coordinates)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CLib3MFMeshObject::AddVertex (const sLib3MFPosition Coordinates)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

sLib3MFTriangle CLib3MFMeshObject::GetTriangle (const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::SetTriangle (const Lib3MF_uint32 nIndex, const sLib3MFTriangle Indices)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CLib3MFMeshObject::AddTriangle (const sLib3MFTriangle Indices)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::GetTriangleIndices (Lib3MF_uint64 nIndicesBufferSize, Lib3MF_uint64* pIndicesNeededCount, sLib3MFTriangle * pIndicesBuffer)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::SetTriangleIndices (const Lib3MF_uint64 nIndicesBufferSize, const sLib3MFTriangle * pIndicesBuffer)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_double CLib3MFMeshObject::GetBeamLattice_MinLength ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::SetBeamLattice_MinLength (const Lib3MF_double dMinLength)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_double CLib3MFMeshObject::GetBeamLattice_Radius ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::SetBeamLattice_Radius (const Lib3MF_double dRadius)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

eLib3MFBeamLatticeCapMode CLib3MFMeshObject::GetBeamLattice_CapMode ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::SetBeamLattice_CapMode (const eLib3MFBeamLatticeCapMode eCapMode)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::GetBeamLattice_Clipping (eLib3MFBeamLatticeClipMode & eClipMode, Lib3MF_uint32 & nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::SetBeamLattice_Clipping (const eLib3MFBeamLatticeClipMode eClipMode, const Lib3MF_uint32 nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::GetBeamLattice_Representation (bool & bHasRepresentation, Lib3MF_uint32 & nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::SetBeamLattice_Representation (const Lib3MF_uint32 nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CLib3MFMeshObject::GetBeamCount ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CLib3MFMeshObject::AddBeam (const sLib3MFBeam BeamInfo)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFMeshObject::SetBeam (const Lib3MF_uint32 nIndex, const sLib3MFBeam BeamInfo)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CLib3MFMeshObject::GetBeamSetCount ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILib3MFBeamSet * CLib3MFMeshObject::AddBeamSet ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILib3MFBeamSet * CLib3MFMeshObject::GetBeamSet (const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CLib3MFMeshObject::IsManifoldAndOriented ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CLib3MFMeshObject::IsMeshObject()
{
	return true;
}

bool CLib3MFMeshObject::IsComponentsObject()
{
	return false;
}

