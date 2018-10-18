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

Abstract: This is a stub class definition of CInternalLib3MFMeshObject
Interface version: 1.2.2

*/

#include "lib3mf_implementation_meshobject.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Model/Classes/NMR_ModelMeshObject.h" 

using namespace Lib3MF;

/*************************************************************************************************************************
 Class definition of CInternalLib3MFMeshObject 
**************************************************************************************************************************/

CInternalLib3MFMeshObject::CInternalLib3MFMeshObject(CInternalLib3MFModel & model)
	:CInternalLib3MFObject(model, std::make_shared<NMR::CModelMeshObject>(model.model().generateResourceID(), &model.model()))
{

}

NMR::CModelMeshObject& CInternalLib3MFMeshObject::getModelMeshObject()
{
	NMR::CModelObject& p = getModelObject();
	return dynamic_cast<NMR::CModelMeshObject&>(p);
};

NMR::CMesh& CInternalLib3MFMeshObject::getMesh()
{
	NMR::CMesh* pMesh = getModelMeshObject().getMesh();
	if (!pMesh)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);
	return *pMesh;
}

unsigned int CInternalLib3MFMeshObject::GetVertexCount ()
{
	return getMesh().getNodeCount();
}

unsigned int CInternalLib3MFMeshObject::GetTriangleCount ()
{
	return getMesh().getFaceCount();
}

void CInternalLib3MFMeshObject::SetVertex (const unsigned int nIndex, const sLib3MFPosition Coordinates)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

unsigned int CInternalLib3MFMeshObject::AddVertex (const sLib3MFPosition Coordinates)
{
	NMR::CMesh& mesh = getMesh();
	// Create Position Vector and check input
	NMR::NVEC3 vPosition;
	for (int j = 0; j < 3; j++) {
		vPosition.m_fields[j] = Coordinates.m_coordinates[j];
		if (fabs(vPosition.m_fields[j]) > NMR_MESH_MAXCOORDINATE)
			throw ELib3MFInterfaceException(NMR_ERROR_INVALIDCOORDINATES);
	}
	NMR::MESHNODE * pNode = mesh.addNode(vPosition);
	if (pNode)
		return pNode->m_index;
	throw ELib3MFInterfaceException(LIB3MF_ERROR_GENERICEXCEPTION);
}

sLib3MFTriangle CInternalLib3MFMeshObject::GetTriangle (const unsigned int nIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::SetTriangle (const unsigned int nIndex, const sLib3MFTriangle Indices)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

unsigned int CInternalLib3MFMeshObject::AddTriangle (const sLib3MFTriangle Indices)
{
	NMR::CMesh& mesh = getMesh();

	// Check for input validity
	NMR::nfUint32 nNodeCount = mesh.getNodeCount();
	for (NMR::nfUint32 j = 0; j < 3; j++)
		if (Indices.m_indices[j] >= nNodeCount)
			throw ELib3MFInterfaceException(NMR_ERROR_INVALIDINDEX);
	if ((Indices.m_indices[0] == Indices.m_indices[1]) ||
		(Indices.m_indices[0] == Indices.m_indices[2]) ||
		(Indices.m_indices[1] == Indices.m_indices[2]))
		throw ELib3MFInterfaceException(NMR_ERROR_INVALIDINDEX);

	// retrieve nodes and add face
	NMR::MESHNODE * pNodes[3];
	for (NMR::nfUint32 j = 0; j < 3; j++)
		pNodes[j] = mesh.getNode(Indices.m_indices[j]);

	NMR::MESHFACE * pFace = mesh.addFace(pNodes[0], pNodes[1], pNodes[2]);
	if (pFace) {
		return pFace->m_index;
	}
	throw ELib3MFInterfaceException(LIB3MF_ERROR_GENERICEXCEPTION);
}

void CInternalLib3MFMeshObject::GetTriangleIndices (unsigned int nIndicesBufferSize, unsigned int * pIndicesNeededCount, sLib3MFTriangle * pIndicesBuffer)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::SetTriangleIndices (const unsigned int nIndicesBufferSize, const sLib3MFTriangle * pIndicesBuffer)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

double CInternalLib3MFMeshObject::GetBeamLattice_MinLength ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::SetBeamLattice_MinLength (const double dMinLength)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

double CInternalLib3MFMeshObject::GetBeamLattice_Radius ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::SetBeamLattice_Radius (const double dRadius)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

eLib3MFBeamLatticeCapMode CInternalLib3MFMeshObject::GetBeamLattice_CapMode ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::SetBeamLattice_CapMode (const eLib3MFBeamLatticeCapMode eCapMode)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::GetBeamLattice_Clipping (eLib3MFBeamLatticeClipMode & eClipMode, unsigned int & nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::SetBeamLattice_Clipping (const eLib3MFBeamLatticeClipMode eClipMode, const unsigned int nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::GetBeamLattice_Representation (bool & bHasRepresentation, unsigned int & nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::SetBeamLattice_Representation (const unsigned int nResourceID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

unsigned int CInternalLib3MFMeshObject::GetBeamCount ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

unsigned int CInternalLib3MFMeshObject::AddBeam (const sLib3MFBeam BeamInfo)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CInternalLib3MFMeshObject::SetBeam (const unsigned int nIndex, const sLib3MFBeam BeamInfo)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

unsigned int CInternalLib3MFMeshObject::GetBeamSetCount ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFBeamSet * CInternalLib3MFMeshObject::AddBeamSet ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInternalLib3MFBeamSet * CInternalLib3MFMeshObject::GetBeamSet (const unsigned int nIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CInternalLib3MFMeshObject::IsManifoldAndOriented ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

