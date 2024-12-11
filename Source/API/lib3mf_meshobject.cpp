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

Abstract: This is a stub class definition of CMeshObject

*/

#include "lib3mf_meshobject.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CMeshObject 
**************************************************************************************************************************/

Lib3MF_uint32 CMeshObject::GetVertexCount()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CMeshObject::GetTriangleCount()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF::sPosition CMeshObject::GetVertex(const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::SetVertex(const Lib3MF_uint32 nIndex, const Lib3MF::sPosition Coordinates)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CMeshObject::AddVertex(const Lib3MF::sPosition Coordinates)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::GetVertices(Lib3MF_uint64 nVerticesBufferSize, Lib3MF_uint64* pVerticesNeededCount, Lib3MF::sPosition * pVerticesBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF::sTriangle CMeshObject::GetTriangle(const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::SetTriangle(const Lib3MF_uint32 nIndex, const Lib3MF::sTriangle Indices)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CMeshObject::AddTriangle(const Lib3MF::sTriangle Indices)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::GetTriangleIndices(Lib3MF_uint64 nIndicesBufferSize, Lib3MF_uint64* pIndicesNeededCount, Lib3MF::sTriangle * pIndicesBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::SetObjectLevelProperty(const Lib3MF_uint32 nUniqueResourceID, const Lib3MF_uint32 nPropertyID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CMeshObject::GetObjectLevelProperty(Lib3MF_uint32 & nUniqueResourceID, Lib3MF_uint32 & nPropertyID)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::SetTriangleProperties(const Lib3MF_uint32 nIndex, const Lib3MF::sTriangleProperties Properties)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::GetTriangleProperties(const Lib3MF_uint32 nIndex, Lib3MF::sTriangleProperties & sProperty)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::SetAllTriangleProperties(const Lib3MF_uint64 nPropertiesArrayBufferSize, const Lib3MF::sTriangleProperties * pPropertiesArrayBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::GetAllTriangleProperties(Lib3MF_uint64 nPropertiesArrayBufferSize, Lib3MF_uint64* pPropertiesArrayNeededCount, Lib3MF::sTriangleProperties * pPropertiesArrayBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::ClearAllProperties()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::SetGeometry(const Lib3MF_uint64 nVerticesBufferSize, const Lib3MF::sPosition * pVerticesBuffer, const Lib3MF_uint64 nIndicesBufferSize, const Lib3MF::sTriangle * pIndicesBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CMeshObject::IsManifoldAndOriented()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IBeamLattice * CMeshObject::BeamLattice()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IVolumeData * CMeshObject::GetVolumeData()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CMeshObject::SetVolumeData(IVolumeData* pTheVolumeData)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

