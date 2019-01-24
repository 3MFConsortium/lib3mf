/*++

Copyright (C) 2018 3MF Consortium

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

NMR_ModelObject.cpp implements the Model Object Class.
A model object is an in memory representation of the 3MF model object.

--*/

#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	CModelObject::CModelObject(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelResource(sID, pModel)
	{
		m_ObjectType = MODELOBJECTTYPE_MODEL;
		setUUID(std::make_shared<CUUID>());
		m_pSliceStackId = 0;
		m_eSlicesMeshResolution = MODELSLICESMESHRESOLUTION_FULL;
		m_MetaDataGroup = std::make_shared<CModelMetaDataGroup>();
	}

	void CModelObject::mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix)
	{
		// empty on purpose, to be implemented by child classes
	}

	void CModelObject::mergeToMesh(_In_ CMesh * pMesh)
	{
		mergeToMesh(pMesh, fnMATRIX3_identity());
	}

	eModelObjectType CModelObject::getObjectType()
	{
		return m_ObjectType;
	}

	void CModelObject::setObjectType(_In_ eModelObjectType ObjectType)
	{
		m_ObjectType = ObjectType;
	}

	std::string CModelObject::getName()
	{
		return m_sName;
	}

	void CModelObject::setName(_In_ std::string sName)
	{
		m_sName = sName;
	}

	std::string CModelObject::getPartNumber()
	{
		return m_sPartNumber;
	}

	void CModelObject::setPartNumber(_In_ std::string sPartNumber)
	{
		m_sPartNumber = sPartNumber;
	}

	PUUID CModelObject::uuid()
	{
		return m_UUID;
	}

	void CModelObject::setUUID(PUUID uuid)
	{
		getModel()->registerUUID(uuid);
		getModel()->unRegisterUUID(m_UUID);
		m_UUID = uuid;
	}

	PModelMetaDataGroup CModelObject::metaDataGroup()
	{
		return m_MetaDataGroup;
	}

	nfBool CModelObject::setObjectTypeString(_In_ std::string sTypeString, _In_ nfBool bRaiseException)
	{
		if (sTypeString == XML_3MF_OBJECTTYPE_OTHER) {
			setObjectType(MODELOBJECTTYPE_OTHER);
			return true;
		}
		if (sTypeString == XML_3MF_OBJECTTYPE_MODEL) {
			setObjectType(MODELOBJECTTYPE_MODEL);
			return true;
		}
		if (sTypeString == XML_3MF_OBJECTTYPE_SUPPORT) {
			setObjectType(MODELOBJECTTYPE_SUPPORT);
			return true;
		}
		if (sTypeString == XML_3MF_OBJECTTYPE_SOLIDSUPPORT) {
			setObjectType(MODELOBJECTTYPE_SOLIDSUPPORT);
			return true;
		}
		if (sTypeString == XML_3MF_OBJECTTYPE_SURFACE) {
			setObjectType(MODELOBJECTTYPE_SURFACE);
			return true;
		}

		if (bRaiseException)
			throw CNMRException(NMR_ERROR_INVALIDMODELOBJECTTYPE);

		return false;
	}

	std::string CModelObject::getObjectTypeString()
	{
		switch (m_ObjectType) {
		case MODELOBJECTTYPE_OTHER:
			return std::string(XML_3MF_OBJECTTYPE_OTHER);
		case MODELOBJECTTYPE_MODEL:
			return std::string(XML_3MF_OBJECTTYPE_MODEL);
		case MODELOBJECTTYPE_SUPPORT:
			return std::string(XML_3MF_OBJECTTYPE_SUPPORT);
		case MODELOBJECTTYPE_SOLIDSUPPORT:
			return std::string(XML_3MF_OBJECTTYPE_SOLIDSUPPORT);
		case MODELOBJECTTYPE_SURFACE:
			return std::string(XML_3MF_OBJECTTYPE_SURFACE);
		default:
			return "";
		}
	}

	void CModelObject::setSliceStackId(PPackageResourceID nSliceStackId) {
		m_pSliceStackId = nSliceStackId;
	}

	PPackageResourceID CModelObject::getSliceStackId() {
		return m_pSliceStackId;
	}

	void CModelObject::setSlicesMeshResolution(eModelSlicesMeshResolution eMeshResolution) {
		m_eSlicesMeshResolution = eMeshResolution;
	}

	eModelSlicesMeshResolution CModelObject::slicesMeshResolution() const {
		return m_eSlicesMeshResolution;
	}

	void CModelObject::setDefaultProperty(_In_ PModelDefaultProperty pModelDefaultProperty)
	{
		m_pModelDefaultProperty = pModelDefaultProperty;
	}

	PModelDefaultProperty CModelObject::getDefaultProperty()
	{
		return m_pModelDefaultProperty;
	}

	void CModelObject::setThumbnail(_In_ std::string sThumbnail)
	{
		m_sThumbnail = sThumbnail;
	}

	std::string CModelObject::getThumbnail()
	{
		return m_sThumbnail;
	}
}
