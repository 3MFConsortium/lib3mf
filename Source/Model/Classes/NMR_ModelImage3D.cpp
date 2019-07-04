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

NMR_ModelImage3D.cpp implements a 3D image stack for the volumetric extension

--*/

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelImage3D.h"
#include "Model/Classes/NMR_ModelAttachment.h"

#include "Common/Platform/NMR_ImportStream_Memory.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {


	CModelImage3D::CModelImage3D(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ nfUint32 nSizeX, _In_ nfUint32 nSizeY, nfUint32 nSheetCount)
		: CModelResource(sID, pModel), m_nSizeX(nSizeX), m_nSizeY(nSizeY), m_nSheetCount(nSheetCount)
	{

		if ((nSizeX == 0) || (nSizeX > MAX_IMAGE3D_SIZE))
			throw CNMRException(NMR_ERROR_INVALIDIMAGE3DSIZE);
		if ((nSizeY == 0) || (nSizeY > MAX_IMAGE3D_SIZE))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if ((nSheetCount == 0) || (nSheetCount > MAX_IMAGE3D_SIZE))
			throw CNMRException(MAX_IMAGE3D_SIZE);

		m_Sheets.resize (m_nSheetCount);
	}
		
	
	PModelImage3D CModelImage3D::make(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ nfUint32 nSizeX, _In_ nfUint32 nSizeY, nfUint32 nSheetCount)
	{
		return std::make_shared<CModelImage3D>(CModelImage3D (sID, pModel, nSizeX, nSizeY, nSheetCount));
	}


	nfUint32 CModelImage3D::getSizeX()
	{
		return m_nSizeX;
	}

	nfUint32 CModelImage3D::getSizeY()
	{
		return m_nSizeY;
	}

	nfUint32 CModelImage3D::getSheetCount()
	{
		return m_nSheetCount;
	}

	void CModelImage3D::setSheet(nfUint32 nSheetIndex, PModelAttachment pAttachment)
	{
		if (!pAttachment.get())
			throw CNMRException(NMR_ERROR_INVALIDTEXTURE);
		if (pAttachment->getRelationShipType() != PACKAGE_TEXTURE_RELATIONSHIP_TYPE)
			throw CNMRException(NMR_ERROR_INVALIDRELATIONSHIPTYPEFORTEXTURE);
		if (nSheetIndex >= m_nSheetCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		if (pAttachment->getModel () != getModel ()) 
			throw CNMRException(NMR_ERROR_ATTACHMENTMODELMISMATCH);

		m_Sheets[nSheetIndex] = pAttachment;
	}
		
	PModelAttachment CModelImage3D::getSheet(nfUint32 nSheetIndex)
	{
		if (nSheetIndex >= m_nSheetCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_Sheets [nSheetIndex];

	}

	PModelAttachment CModelImage3D::createSheet(nfUint32 nSheetIndex, const std::string & sPath, PImportStream pCopiedStream)
	{

		PModelAttachment pAttachment = getModel()->addAttachment(sPath, PACKAGE_TEXTURE_RELATIONSHIP_TYPE, pCopiedStream);
		setSheet(nSheetIndex, pAttachment);

		return pAttachment;
	}

}
