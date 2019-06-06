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

Abstract: This is a stub class definition of CBeamLattice

*/

#include "lib3mf_beamlattice.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_beamset.hpp"
// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CBeamLattice 
**************************************************************************************************************************/

CBeamLattice::CBeamLattice(NMR::PModelMeshObject pMeshObject, NMR::PModelMeshBeamLatticeAttributes pAttributes):
	m_pMeshObject(pMeshObject),m_mesh(*pMeshObject->getMesh()), m_pAttributes(pAttributes)
{
	
}

Lib3MF_double CBeamLattice::GetMinLength ()
{
	return m_mesh.getBeamLatticeMinLength();
}

void CBeamLattice::SetMinLength (const Lib3MF_double dMinLength)
{
	return m_mesh.setBeamLatticeMinLength(dMinLength);
}

void CBeamLattice::GetClipping (eLib3MFBeamLatticeClipMode & eClipMode, Lib3MF_uint32 & nResourceID)
{
	if (!m_pAttributes->m_bHasClippingMeshID) {
		eClipMode = eBeamLatticeClipMode::NoClipMode;
		nResourceID = 0;
	}
	else {
		eClipMode = eBeamLatticeClipMode(m_pAttributes->m_eClipMode);
		nResourceID = m_pAttributes->m_nClippingMeshID->getUniqueID();
	}
}

void CBeamLattice::SetClipping (const eLib3MFBeamLatticeClipMode eClipMode, const Lib3MF_uint32 nResourceID)
{
	if ( ((int)eClipMode == (NMR::eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE)) || (nResourceID == 0) ){
		m_pAttributes->m_eClipMode = NMR::eModelBeamLatticeClipMode(eClipMode);
		m_pAttributes->m_bHasClippingMeshID = false;
		m_pAttributes->m_nClippingMeshID = nullptr;
	}
	else {
		NMR::CModel* pModel = m_pMeshObject->getModel();

		NMR::CModelMeshObject * pClippingObject = dynamic_cast<NMR::CModelMeshObject*>(pModel->findObject(nResourceID));
		if (pClippingObject == nullptr) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
		}
		// check, if this object will be written before 
		NMR::nfInt32 nComp = pModel->compareObjectsByResourceID(pClippingObject, m_pMeshObject.get());
		if (nComp < 0) { // pClippingObject has been defined after m_pMeshObject
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
		}

		m_pAttributes->m_eClipMode = NMR::eModelBeamLatticeClipMode(eClipMode);;
		m_pAttributes->m_bHasClippingMeshID = true;
		m_pAttributes->m_nClippingMeshID = pClippingObject->getResourceID();
	}
}

bool CBeamLattice::GetRepresentation (Lib3MF_uint32 & nResourceID)
{
	if (m_pAttributes->m_bHasRepresentationMeshID) {
		nResourceID = m_pAttributes->m_nRepresentationID->getUniqueID();
		return true;
	}
	else {
		nResourceID = 0;
		return false;
	}
}

void CBeamLattice::SetRepresentation (const Lib3MF_uint32 nResourceID)
{
	if (nResourceID == 0) {
		m_pAttributes->m_bHasRepresentationMeshID = false;
		m_pAttributes->m_nRepresentationID = nullptr;
	}
	else {
		NMR::CModel* pModel = m_pMeshObject->getModel();

		NMR::CModelMeshObject * pRepresentationObject = dynamic_cast<NMR::CModelMeshObject*>(pModel->findObject(nResourceID));
		if (pRepresentationObject == nullptr) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
		}
		// check, if this object will be written before 
		NMR::nfInt32 nComp = pModel->compareObjectsByResourceID(pRepresentationObject, m_pMeshObject.get());
		if (nComp < 0) { // pClippingObject has been defined after m_pMeshObject
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
		}

		m_pAttributes->m_bHasRepresentationMeshID = true;
		m_pAttributes->m_nRepresentationID = pRepresentationObject->getResourceID();
	}
}

Lib3MF_uint32 CBeamLattice::GetBeamCount ()
{
	return m_mesh.getBeamCount();
}

sLib3MFBeam CBeamLattice::GetBeam (const Lib3MF_uint32 nIndex)
{
	sLib3MFBeam beam;
	NMR::MESHBEAM* meshBeam = m_mesh.getBeam(nIndex);
	beam.m_CapModes[0] = (eLib3MFBeamLatticeCapMode)(meshBeam->m_capMode[0]);
	beam.m_CapModes[1] = (eLib3MFBeamLatticeCapMode)meshBeam->m_capMode[1];

	beam.m_Indices[0] = meshBeam->m_nodeindices[0];
	beam.m_Indices[1] = meshBeam->m_nodeindices[1];

	beam.m_Radii[0] = meshBeam->m_radius[0];
	beam.m_Radii[1] = meshBeam->m_radius[1];
	return beam;
}

bool isBeamValid(const Lib3MF_uint32 nNodeCount, const sLib3MFBeam& BeamInfo)
{
	for (int j = 0; j < 2; j++) {
		if (BeamInfo.m_Indices[j] >= nNodeCount)
			return false;
		if (BeamInfo.m_Radii[j] <= 0)
			return false;
	}
	if (BeamInfo.m_Indices[0] == BeamInfo.m_Indices[1])
		return false;
	return true;
}

Lib3MF_uint32 CBeamLattice::AddBeam (const sLib3MFBeam BeamInfo)
{
	if (!m_pMeshObject->isValidForBeamLattices())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_BEAMLATTICE_INVALID_OBJECTTYPE);

	// Check for input validity
	if (!isBeamValid(m_mesh.getNodeCount(), BeamInfo))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	// retrieve nodes and add beam
	NMR::MESHNODE * pNodes[2];
	for (int j = 0; j < 2; j++)
		pNodes[j] = m_mesh.getNode(BeamInfo.m_Indices[j]);

	NMR::MESHBEAM * pMeshBeam = m_mesh.addBeam(pNodes[0], pNodes[1], BeamInfo.m_Radii[0], BeamInfo.m_Radii[1], (int)BeamInfo.m_CapModes[0], (int)BeamInfo.m_CapModes[1]);
	return pMeshBeam->m_index;
}

void CBeamLattice::SetBeam (const Lib3MF_uint32 nIndex, const sLib3MFBeam BeamInfo)
{
	if (!isBeamValid(m_mesh.getNodeCount(), BeamInfo))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::MESHBEAM* meshBeam = m_mesh.getBeam(nIndex);
	meshBeam->m_capMode[0] = (int)BeamInfo.m_CapModes[0];
	meshBeam->m_capMode[1] = (int)BeamInfo.m_CapModes[1];

	meshBeam->m_nodeindices[0] = BeamInfo.m_Indices[0];
	meshBeam->m_nodeindices[1] = BeamInfo.m_Indices[1];

	meshBeam->m_radius[0] = BeamInfo.m_Radii[0];
	meshBeam->m_radius[1] = BeamInfo.m_Radii[1];
}

void CBeamLattice::SetBeams(const Lib3MF_uint64 nBeamInfoBufferSize, const sLib3MFBeam * pBeamInfoBuffer)
{
	if ((nBeamInfoBufferSize>0) && (!m_pMeshObject->isValidForBeamLattices()))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_BEAMLATTICE_INVALID_OBJECTTYPE);

	m_mesh.clearBeamLattice();

	const sLib3MFBeam* pBeamInfoCurrent = pBeamInfoBuffer;
	for (Lib3MF_uint32 nIndex = 0; nIndex < nBeamInfoBufferSize; nIndex++)
	{
		if (!isBeamValid(m_mesh.getNodeCount(), *pBeamInfoCurrent))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

		NMR::MESHNODE * pNodes[2];
		for (int j = 0; j < 2; j++)
			pNodes[j] = m_mesh.getNode(pBeamInfoCurrent->m_Indices[j]);
		
		NMR::MESHBEAM * pMeshBeam = m_mesh.addBeam(pNodes[0], pNodes[1], pBeamInfoCurrent->m_Radii[0], pBeamInfoCurrent->m_Radii[1], (int)pBeamInfoCurrent->m_CapModes[0], (int)pBeamInfoCurrent->m_CapModes[1]);
		pBeamInfoCurrent++;
	}

}

void CBeamLattice::GetBeams(Lib3MF_uint64 nBeamInfoBufferSize, Lib3MF_uint64* pBeamInfoNeededCount, sLib3MFBeam * pBeamInfoBuffer)
{
	Lib3MF_uint32 beamCount = m_mesh.getBeamCount();
	if (pBeamInfoNeededCount)
		*pBeamInfoNeededCount = beamCount;

	if (nBeamInfoBufferSize >= beamCount && pBeamInfoBuffer)
	{
		sLib3MFBeam* beam = pBeamInfoBuffer;
		for (Lib3MF_uint32 i = 0; i < beamCount; i++)
		{
			const NMR::MESHBEAM* meshBeam = m_mesh.getBeam(i);
			beam->m_CapModes[0] = (eLib3MFBeamLatticeCapMode)meshBeam->m_capMode[0];
			beam->m_CapModes[1] = (eLib3MFBeamLatticeCapMode)meshBeam->m_capMode[1];

			beam->m_Indices[0] = meshBeam->m_nodeindices[0];
			beam->m_Indices[1] = meshBeam->m_nodeindices[1];

			beam->m_Radii[0] = meshBeam->m_radius[0];
			beam->m_Radii[1] = meshBeam->m_radius[1];
			beam++;
		}
	}
}

Lib3MF_uint32 CBeamLattice::GetBeamSetCount ()
{
	return m_mesh.getBeamSetCount();
}

IBeamSet * CBeamLattice::AddBeamSet ()
{
	return new CBeamSet(m_mesh.addBeamSet(), m_pMeshObject);
}

IBeamSet * CBeamLattice::GetBeamSet (const Lib3MF_uint32 nIndex)
{
	return new CBeamSet(m_mesh.getBeamSet(nIndex), m_pMeshObject);
}

