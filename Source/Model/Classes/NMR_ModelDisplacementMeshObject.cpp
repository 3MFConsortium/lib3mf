/*++

Copyright (C) 2026 3MF Consortium

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

In-memory representation and serialization support for the 3MF Displacement extension.

--*/

#include "Model/Classes/NMR_ModelDisplacementMeshObject.h"
#include "Common/NMR_Exception.h"
#include "Common/Math/NMR_Vector.h"
#include <cmath>

namespace NMR {

	CModelDisplacementMeshObject::CModelDisplacementMeshObject(ModelResourceID nID, CModel * pModel)
		: CModelMeshObject(nID, pModel)
	{
		CModelMeshObject::setObjectType(MODELOBJECTTYPE_MODEL);
	}

	nfBool CModelDisplacementMeshObject::hasTriangleDisplacement(nfUint32 nTriangleIndex) const
	{
		if (nTriangleIndex >= const_cast<CModelDisplacementMeshObject *>(this)->getMesh()->getFaceCount())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_TriangleDisplacements.find(nTriangleIndex) != m_TriangleDisplacements.end();
	}

	void CModelDisplacementMeshObject::setTriangleDisplacement(nfUint32 nTriangleIndex,
		PModelDisp2DGroupResource pGroup, nfUint32 nD1, nfUint32 nD2, nfUint32 nD3)
	{
		if (nTriangleIndex >= getMesh()->getFaceCount())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		if (!pGroup || pGroup->getModel() != getModel())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (nD1 >= pGroup->getCount() || nD2 >= pGroup->getCount() || nD3 >= pGroup->getCount())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto pFace = getMesh()->getFace(nTriangleIndex);
		auto vFaceNormal = fnVEC3_calcTriangleNormal(
			getMesh()->getNode(pFace->m_nodeindices[0])->m_position,
			getMesh()->getNode(pFace->m_nodeindices[1])->m_position,
			getMesh()->getNode(pFace->m_nodeindices[2])->m_position);
		auto pNormals = pGroup->getNormVectorGroup();
		for (nfUint32 nCoordinateIndex : { nD1, nD2, nD3 }) {
			auto coordinate = pGroup->getCoordinate(nCoordinateIndex);
			auto normal = pNormals->getVector(coordinate.m_nNormalVectorIndex);
			const nfDouble dScalarProduct = normal[0] * vFaceNormal.m_values.x +
				normal[1] * vFaceNormal.m_values.y + normal[2] * vFaceNormal.m_values.z;
			if (dScalarProduct < 0.0)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
		}
		m_TriangleDisplacements[nTriangleIndex] = { pGroup, { nD1, nD2, nD3 } };
	}

	MODELMESHTRIANGLEDISPLACEMENT CModelDisplacementMeshObject::getTriangleDisplacement(nfUint32 nTriangleIndex) const
	{
		if (nTriangleIndex >= const_cast<CModelDisplacementMeshObject *>(this)->getMesh()->getFaceCount())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		auto iIterator = m_TriangleDisplacements.find(nTriangleIndex);
		if (iIterator == m_TriangleDisplacements.end())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return iIterator->second;
	}

	void CModelDisplacementMeshObject::clearTriangleDisplacement(nfUint32 nTriangleIndex)
	{
		if (nTriangleIndex >= getMesh()->getFaceCount())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		m_TriangleDisplacements.erase(nTriangleIndex);
	}

	nfUint32 CModelDisplacementMeshObject::getDisplacedTriangleCount() const { return (nfUint32)m_TriangleDisplacements.size(); }

	void CModelDisplacementMeshObject::setObjectType(eModelObjectType ObjectType)
	{
		if (ObjectType != MODELOBJECTTYPE_MODEL)
			throw CNMRException(NMR_ERROR_INVALIDOBJECT);
		CModelMeshObject::setObjectType(ObjectType);
	}

	nfBool CModelDisplacementMeshObject::isValid()
	{
		if (getObjectType() != MODELOBJECTTYPE_MODEL)
			return false;
		nfDouble dSignedVolumeTimesSix = 0.0;
		for (nfUint32 nFaceIndex = 0; nFaceIndex < getMesh()->getFaceCount(); ++nFaceIndex) {
			auto pFace = getMesh()->getFace(nFaceIndex);
			auto a = getMesh()->getNode(pFace->m_nodeindices[0])->m_position;
			auto b = getMesh()->getNode(pFace->m_nodeindices[1])->m_position;
			auto c = getMesh()->getNode(pFace->m_nodeindices[2])->m_position;
			dSignedVolumeTimesSix += static_cast<nfDouble>(a.m_values.x) * (b.m_values.y * c.m_values.z - b.m_values.z * c.m_values.y)
				- static_cast<nfDouble>(a.m_values.y) * (b.m_values.x * c.m_values.z - b.m_values.z * c.m_values.x)
				+ static_cast<nfDouble>(a.m_values.z) * (b.m_values.x * c.m_values.y - b.m_values.y * c.m_values.x);
		}
		if (!std::isfinite(dSignedVolumeTimesSix) || dSignedVolumeTimesSix <= 0.0)
			return false;
		for (const auto & entry : m_TriangleDisplacements) {
			if (entry.first >= getMesh()->getFaceCount() || !entry.second.m_pGroup)
				return false;
			auto pFace = getMesh()->getFace(entry.first);
			auto vFaceNormal = fnVEC3_calcTriangleNormal(
				getMesh()->getNode(pFace->m_nodeindices[0])->m_position,
				getMesh()->getNode(pFace->m_nodeindices[1])->m_position,
				getMesh()->getNode(pFace->m_nodeindices[2])->m_position);
			for (nfUint32 nIndex : entry.second.m_nIndices) {
				if (nIndex >= entry.second.m_pGroup->getCount())
					return false;
				auto coordinate = entry.second.m_pGroup->getCoordinate(nIndex);
				auto normal = entry.second.m_pGroup->getNormVectorGroup()->getVector(coordinate.m_nNormalVectorIndex);
				if (normal[0] * vFaceNormal.m_values.x + normal[1] * vFaceNormal.m_values.y + normal[2] * vFaceNormal.m_values.z < 0.0)
					return false;
			}
		}
		return CModelMeshObject::isValid();
	}

	void CModelDisplacementMeshObject::mergeToMesh(CMesh * pMesh, const NMATRIX3 mMatrix)
	{
		if (!m_TriangleDisplacements.empty())
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		CModelMeshObject::mergeToMesh(pMesh, mMatrix);
	}

	ResourceDependencies CModelDisplacementMeshObject::getDependencies()
	{
		auto dependencies = CModelMeshObject::getDependencies();
		std::set<UniqueResourceID> seen;
		for (const auto & dependency : dependencies)
			seen.insert(dependency->getUniqueID());
		for (const auto & entry : m_TriangleDisplacements) {
			auto pID = entry.second.m_pGroup->getPackageResourceID();
			if (seen.insert(pID->getUniqueID()).second)
				dependencies.push_back(pID);
		}
		return dependencies;
	}
}
