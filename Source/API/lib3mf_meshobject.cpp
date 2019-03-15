/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

#include "lib3mf_beamlattice.hpp"
// Include custom headers here.

#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"
#include <cmath>

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFMeshObject 
**************************************************************************************************************************/

CLib3MFMeshObject::CLib3MFMeshObject(NMR::PModelResource pResource)
	: CLib3MFResource(pResource), CLib3MFObject(pResource)
{

}

NMR::PModelMeshObject CLib3MFMeshObject::meshObject()
{
	NMR::PModelMeshObject pMesh = std::dynamic_pointer_cast<NMR::CModelMeshObject>(resource());
	if (pMesh == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pMesh;
}

NMR::CMesh* CLib3MFMeshObject::mesh()
{
	return meshObject()->getMesh();
}

Lib3MF_uint32 CLib3MFMeshObject::GetVertexCount ()
{
	return mesh()->getNodeCount();
}

Lib3MF_uint32 CLib3MFMeshObject::GetTriangleCount ()
{
	return mesh()->getFaceCount();
}

void CLib3MFMeshObject::SetVertex (const Lib3MF_uint32 nIndex, const sLib3MFPosition Coordinates)
{
	NMR::MESHNODE* node = mesh()->getNode(nIndex);
	node->m_position.m_fields[0] = Coordinates.m_Coordinates[0];
	node->m_position.m_fields[1] = Coordinates.m_Coordinates[1];
	node->m_position.m_fields[2] = Coordinates.m_Coordinates[2];
}

sLib3MFPosition CLib3MFMeshObject::GetVertex(const Lib3MF_uint32 nIndex)
{
	NMR::MESHNODE* node = mesh()->getNode(nIndex);
	sLib3MFPosition pos;
	pos.m_Coordinates[0] = node->m_position.m_fields[0];
	pos.m_Coordinates[1] = node->m_position.m_fields[1];
	pos.m_Coordinates[2] = node->m_position.m_fields[2];
	return pos;
}

Lib3MF_uint32 CLib3MFMeshObject::AddVertex (const sLib3MFPosition Coordinates)
{
	return mesh()->addNode(Coordinates.m_Coordinates[0], Coordinates.m_Coordinates[1], Coordinates.m_Coordinates[2])->m_index;
}

void CLib3MFMeshObject::GetVertices(Lib3MF_uint64 nVerticesBufferSize, Lib3MF_uint64* pVerticesNeededCount, sLib3MFPosition * pVerticesBuffer)
{
	Lib3MF_uint32 nodeCount = mesh()->getNodeCount();
	if (pVerticesNeededCount)
		*pVerticesNeededCount = nodeCount;

	if (nVerticesBufferSize >= nodeCount && pVerticesBuffer)
	{
		for (Lib3MF_uint32 i = 0; i < nodeCount; i++)
		{
			const NMR::MESHNODE* node = mesh()->getNode(i);
			pVerticesBuffer[i].m_Coordinates[0] = node->m_position.m_fields[0];
			pVerticesBuffer[i].m_Coordinates[1] = node->m_position.m_fields[1];
			pVerticesBuffer[i].m_Coordinates[2] = node->m_position.m_fields[2];
		}
	}
}

sLib3MFTriangle CLib3MFMeshObject::GetTriangle (const Lib3MF_uint32 nIndex)
{
	sLib3MFTriangle t;
	NMR::MESHFACE* mf = mesh()->getFace(nIndex);

	t.m_Indices[0] = mf->m_nodeindices[0];
	t.m_Indices[1] = mf->m_nodeindices[1];
	t.m_Indices[2] = mf->m_nodeindices[2];

	return t;
}

void CLib3MFMeshObject::SetTriangle (const Lib3MF_uint32 nIndex, const sLib3MFTriangle Indices)
{
	NMR::MESHFACE* mf = mesh()->getFace(nIndex);

	mf->m_nodeindices[0] = Indices.m_Indices[0];
	mf->m_nodeindices[1] = Indices.m_Indices[1];
	mf->m_nodeindices[2] = Indices.m_Indices[2];
}

Lib3MF_uint32 CLib3MFMeshObject::AddTriangle(const sLib3MFTriangle Indices)
{
	return mesh()->addFace(Indices.m_Indices[0], Indices.m_Indices[1], Indices.m_Indices[2])->m_index;
}

void CLib3MFMeshObject::GetTriangleIndices (Lib3MF_uint64 nIndicesBufferSize, Lib3MF_uint64* pIndicesNeededCount, sLib3MFTriangle * pIndicesBuffer)
{
	Lib3MF_uint32 faceCount = mesh()->getFaceCount();
	if (pIndicesNeededCount)
		*pIndicesNeededCount = faceCount;

	if (nIndicesBufferSize >= faceCount && pIndicesBuffer)
	{
		for (Lib3MF_uint32 i = 0; i < faceCount; i++)
		{
			const NMR::MESHFACE* face = mesh()->getFace(i);
			pIndicesBuffer[i].m_Indices[0] = face->m_nodeindices[0];
			pIndicesBuffer[i].m_Indices[1] = face->m_nodeindices[1];
			pIndicesBuffer[i].m_Indices[2] = face->m_nodeindices[2];
		}
	}
}

void CLib3MFMeshObject::SetTriangleProperties(const Lib3MF_uint32 nIndex, const sLib3MFTriangleProperties Properties)
{
	auto pMesh = mesh();
	auto pInformationHandler = pMesh->createMeshInformationHandler();
	NMR::CMeshInformation_Properties * pInformation = dynamic_cast<NMR::CMeshInformation_Properties *> (pInformationHandler->getInformationByType(0, NMR::emiProperties));

	if (pInformation == nullptr) {
		NMR::PMeshInformation_Properties pNewInformation = std::make_shared<NMR::CMeshInformation_Properties> (pMesh->getFaceCount ());
		pInformationHandler->addInformation(pNewInformation);

		pInformation = pNewInformation.get();
	}

	NMR::MESHINFORMATION_PROPERTIES * pFaceData = (NMR::MESHINFORMATION_PROPERTIES*)pInformation->getFaceData(nIndex);
	if (pFaceData != nullptr) {
		pFaceData->m_nResourceID = Properties.m_ResourceID;
		for (unsigned j = 0; j < 3; j++) {
			pFaceData->m_nPropertyIDs[j] = Properties.m_PropertyIDs[j];
		}
	}

}

void CLib3MFMeshObject::GetTriangleProperties(const Lib3MF_uint32 nIndex, sLib3MFTriangleProperties & sProperty)
{

	auto pMesh = mesh();
	auto pInformationHandler = pMesh->createMeshInformationHandler();
	NMR::CMeshInformation_Properties * pInformation = dynamic_cast<NMR::CMeshInformation_Properties *> (pInformationHandler->getInformationByType(0, NMR::emiProperties));

	if (pInformation == nullptr) {
		NMR::PMeshInformation_Properties pNewInformation = std::make_shared<NMR::CMeshInformation_Properties>(pMesh->getFaceCount());
		pInformationHandler->addInformation(pNewInformation);

		pInformation = pNewInformation.get();
	}

	NMR::MESHINFORMATION_PROPERTIES * pFaceData = (NMR::MESHINFORMATION_PROPERTIES*)pInformation->getFaceData(nIndex);
	if (pFaceData != nullptr) {
		sProperty.m_ResourceID = pFaceData->m_nResourceID;
		for (unsigned j = 0; j < 3; j++) {
			sProperty.m_PropertyIDs[j] = pFaceData->m_nPropertyIDs[j];
		}

	}
	else {
		sProperty.m_ResourceID = 0;
		for (unsigned j = 0; j < 3; j++) {
			sProperty.m_PropertyIDs[j] = 0;
		}

	}
}

void CLib3MFMeshObject::SetAllTriangleProperties(const Lib3MF_uint64 nPropertiesArrayBufferSize, const sLib3MFTriangleProperties * pPropertiesArrayBuffer)
{
	auto pMesh = mesh();
	uint32_t nFaceCount = pMesh->getFaceCount();

	if (nPropertiesArrayBufferSize != nFaceCount)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPROPERTYCOUNT);
	if (pPropertiesArrayBuffer == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	auto pInformationHandler = pMesh->createMeshInformationHandler();
	NMR::CMeshInformation_Properties * pInformation = dynamic_cast<NMR::CMeshInformation_Properties *> (pInformationHandler->getInformationByType(0, NMR::emiProperties));

	if (pInformation == nullptr) {
		NMR::PMeshInformation_Properties pNewInformation = std::make_shared<NMR::CMeshInformation_Properties>(pMesh->getFaceCount());
		pInformationHandler->addInformation(pNewInformation);

		pInformation = pNewInformation.get();
	}

	const sLib3MFTriangleProperties * pProperty = pPropertiesArrayBuffer;

	uint32_t nIndex;
	for (nIndex = 0; nIndex < nFaceCount; nIndex++) {

		NMR::MESHINFORMATION_PROPERTIES * pFaceData = (NMR::MESHINFORMATION_PROPERTIES*)pInformation->getFaceData(nIndex);
		if (pFaceData != nullptr) {
			pFaceData->m_nResourceID = pProperty->m_ResourceID;
			for (unsigned j = 0; j < 3; j++) {
				pFaceData->m_nPropertyIDs[j] = pProperty->m_PropertyIDs[j];
			}
		}

		pProperty++;
	}
}

void CLib3MFMeshObject::GetAllTriangleProperties(Lib3MF_uint64 nPropertiesArrayBufferSize, Lib3MF_uint64* pPropertiesArrayNeededCount, sLib3MFTriangleProperties * pPropertiesArrayBuffer)
{
	auto pMesh = mesh();
	uint32_t nFaceCount = pMesh->getFaceCount();

	if (pPropertiesArrayNeededCount)
		*pPropertiesArrayNeededCount = nFaceCount;

	if (nPropertiesArrayBufferSize >= nFaceCount && pPropertiesArrayBuffer)
	{
		auto pInformationHandler = pMesh->createMeshInformationHandler();
		NMR::CMeshInformation_Properties * pInformation = dynamic_cast<NMR::CMeshInformation_Properties *> (pInformationHandler->getInformationByType(0, NMR::emiProperties));

		if (pInformation == nullptr) {
			NMR::PMeshInformation_Properties pNewInformation = std::make_shared<NMR::CMeshInformation_Properties>(pMesh->getFaceCount());
			pInformationHandler->addInformation(pNewInformation);

			pInformation = pNewInformation.get();
		}

		sLib3MFTriangleProperties * pProperty = pPropertiesArrayBuffer;

		uint32_t nIndex;
		for (nIndex = 0; nIndex < nFaceCount; nIndex++) {

			NMR::MESHINFORMATION_PROPERTIES * pFaceData = (NMR::MESHINFORMATION_PROPERTIES*)pInformation->getFaceData(nIndex);
			if (pFaceData != nullptr) {
				pProperty->m_ResourceID = pFaceData->m_nResourceID;
				for (unsigned j = 0; j < 3; j++) {
					pProperty->m_PropertyIDs[j] = pFaceData->m_nPropertyIDs[j];
				}
			}
			pProperty++;
		}
	}
}


void CLib3MFMeshObject::SetGeometry(const Lib3MF_uint64 nVerticesBufferSize, const sLib3MFPosition * pVerticesBuffer, const Lib3MF_uint64 nIndicesBufferSize, const sLib3MFTriangle * pIndicesBuffer)
{
	if ( ((!pVerticesBuffer) && (nVerticesBufferSize>0)) || ((!pIndicesBuffer) && (nIndicesBufferSize>0)))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::CMesh * pMesh = mesh();

	// Clear old mesh
	pMesh->clear();

	// Rebuild Mesh Coordinates
	const sLib3MFPosition * pVertex = pVerticesBuffer;
	for (Lib3MF_uint64 nIndex = 0; nIndex < nVerticesBufferSize; nIndex++) {
		for (int j = 0; j < 3; j++) {
			if (std::fabs(pVertex->m_Coordinates[j]) > NMR_MESH_MAXCOORDINATE)
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
		}
		pMesh->addNode(pVertex->m_Coordinates[0], pVertex->m_Coordinates[1], pVertex->m_Coordinates[2]);

		pVertex++;
	}

	// Rebuild Mesh Faces
	const sLib3MFTriangle * pTriangle = pIndicesBuffer;
	for (int nIndex = 0; nIndex < nIndicesBufferSize; nIndex++) {
		NMR::MESHNODE * pNodes[3];

		for (int j = 0; j < 3; j++) {
			if (pTriangle->m_Indices[j] >= nVerticesBufferSize)
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
			pNodes[j] = pMesh->getNode(pTriangle->m_Indices[j]);
		}

		if ((pTriangle->m_Indices[0] == pTriangle->m_Indices[1]) ||
			(pTriangle->m_Indices[0] == pTriangle->m_Indices[2]) ||
			(pTriangle->m_Indices[1] == pTriangle->m_Indices[2]))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

		pMesh->addFace(pNodes[0], pNodes[1], pNodes[2]);

		pTriangle++;
	}
}

bool CLib3MFMeshObject::IsManifoldAndOriented ()
{
	return meshObject()->isManifoldAndOriented();
}

bool CLib3MFMeshObject::IsMeshObject()
{
	return true;
}

bool CLib3MFMeshObject::IsComponentsObject()
{
	return false;
}

bool CLib3MFMeshObject::IsValid()
{
	return meshObject()->isValid();
}

ILib3MFBeamLattice* CLib3MFMeshObject::BeamLattice()
{
	return new CLib3MFBeamLattice(meshObject(), meshObject()->getBeamLatticeAttributes());
}
