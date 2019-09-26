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

NMR_MeshInformation.cpp implements the Mesh Information Class.
This is a base class for handling all the mesh-related linear information
(like face colors, textures, etc...). It provides abstract functions to
interpolate and reconstruct informations while the mesh topology is changing.

--*/

#include "Common/MeshInformation/NMR_MeshInformation.h" 
#include "Common/NMR_Exception.h" 
#include <cmath>

namespace NMR {

	CMeshInformation::CMeshInformation()
	{
		m_nInternalID = 0;
	}

	_Ret_notnull_ MESHINFORMATIONFACEDATA * CMeshInformation::getFaceData(nfUint32 nFaceIndex)
	{
		if (!m_pContainer)
			throw CNMRException(NMR_ERROR_NOMESHINFORMATIONCONTAINER);
		return m_pContainer->getFaceData(nFaceIndex);
	}

	void CMeshInformation::resetFaceInformation(_In_ nfUint32 nFaceIndex)
	{
		MESHINFORMATIONFACEDATA * pData = getFaceData(nFaceIndex);
		if (pData)
			this->invalidateFace(pData);
	}

	_Ret_notnull_ MESHINFORMATIONFACEDATA * CMeshInformation::addFaceData(_In_ nfUint32 nNewFaceCount)
	{
		return m_pContainer->addFaceData(nNewFaceCount);
	}

	void CMeshInformation::resetAllFaceInformation()
	{
		nfUint32 nCount = m_pContainer->getCurrentFaceCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			resetFaceInformation(nIndex);
		}
	}

	void CMeshInformation::setInternalID(nfUint64 nInternalID)
	{
		m_nInternalID = nInternalID;
	}

	nfUint64 CMeshInformation::getInternalID()
	{
		return m_nInternalID;
	}

}
