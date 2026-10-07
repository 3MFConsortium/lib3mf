/*++

Copyright (C) 2019 3MF Consortium

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

NMR_ModelBuildItem.cpp implements the Model Build Item Class.
A model build item is an in memory representation of the 3MF
build item.

--*/

#include "Model/Classes/NMR_ModelBuildItem.h"
#include "Common/NMR_Exception.h"
#include <sstream>

namespace NMR {

	CModelBuildItem::CModelBuildItem(_In_ CModelObject * pObject, _In_ nfUint32 nHandle)
	{
		if (!pObject)
			throw CNMRException (NMR_ERROR_INVALIDPARAM);
		m_pObject = pObject;
		m_mTransform = fnMATRIX3_identity ();
		m_nHandle = nHandle;
		m_MetaDataGroup = std::make_shared<CModelMetaDataGroup>();

		setUUID(std::make_shared<CUUID>());
	}

	CModelBuildItem::CModelBuildItem(_In_ CModelObject * pObject, _In_ const NMATRIX3 mTransform, _In_ nfUint32 nHandle)
	{
		if (!pObject)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pObject = pObject;
		m_mTransform = mTransform;
		m_nHandle = nHandle;
		m_MetaDataGroup = std::make_shared<CModelMetaDataGroup>();

		setUUID(std::make_shared<CUUID>());
	}

	CModelBuildItem::~CModelBuildItem()
	{
		m_pObject = nullptr;
	}

	CModelObject * CModelBuildItem::getObject()
	{
		return m_pObject;
	}

	NMATRIX3 CModelBuildItem::getTransform()
	{
		return m_mTransform;
	}

	void CModelBuildItem::setTransform(_In_ const NMATRIX3 mTransform)
	{
		m_mTransform = mTransform;
	}

	nfBool CModelBuildItem::hasTransform()
	{
		return (!fnMATRIX3_isIdentity(m_mTransform));
	}

	std::string CModelBuildItem::getTransformString()
	{
		return fnMATRIX3_toString(m_mTransform);
	}

	std::string CModelBuildItem::getPartNumber()
	{
		return m_sPartNumber;
	}

	void CModelBuildItem::setPartNumber(_In_ std::string sPartNumber)
	{
		m_sPartNumber = sPartNumber;
	}

	CModel* CModelBuildItem::getModel() {
		return m_pObject->getModel();
	}

	PUUID CModelBuildItem::uuid()
	{
		return m_UUID;
	}

	void CModelBuildItem::setUUID(PUUID uuid)
	{
		getModel()->registerUUID(uuid);
		getModel()->unRegisterUUID(m_UUID);
		m_UUID = uuid;
	}

	PModelMetaDataGroup CModelBuildItem::metaDataGroup()
	{
		return m_MetaDataGroup;
	}

	std::string CModelBuildItem::path()
	{
		return m_sPath;
	}

	void CModelBuildItem::setPath(std::string sPath)
	{
		m_sPath = sPath;
	}

	void CModelBuildItem::mergeToMesh(_In_ CMesh * pMesh)
	{
		__NMRASSERT(pMesh);
		m_pObject->mergeToMesh(pMesh, m_mTransform);
	}

	nfUint32 CModelBuildItem::getHandle()
	{
		return m_nHandle;
	}

	bool CModelBuildItem::isValidForSlices() {
		NMATRIX3 mat = getTransform();
		return m_pObject->isValidForSlices(mat);
	}
}
