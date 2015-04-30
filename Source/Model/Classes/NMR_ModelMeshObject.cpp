/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_ModelMeshObject.cpp implements the Model Mesh Object Class.
A model mesh object is an in memory representation of the 3MF
mesh object.

--*/

#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 

namespace NMR {

	CModelMeshObject::CModelMeshObject(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelObject(sID, pModel)
	{
		m_pMesh = std::make_shared<CMesh>();
	}

	CModelMeshObject::CModelMeshObject(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ PMesh pMesh)
		: CModelObject(sID, pModel)
	{
		m_pMesh = pMesh;
		if (m_pMesh.get () == nullptr)
			m_pMesh = std::make_shared<CMesh>();
	}
	
	CModelMeshObject::~CModelMeshObject()
	{
		m_pMesh = NULL;
	}

	_Ret_notnull_ CMesh * CModelMeshObject::getMesh()
	{
		return m_pMesh.get();
	}

	void CModelMeshObject::setMesh(_In_ PMesh pMesh)
	{
		if (!pMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pMesh = pMesh;
	}

	void CModelMeshObject::mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix)
	{
		__NMRASSERT(pMesh);
		pMesh->mergeMesh(m_pMesh.get(), mMatrix);
	}

}
