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

NMR_ModelImageStack.cpp implements an image stack for the volumetric extension

--*/

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelImageStack.h"
#include "Model/Classes/NMR_ModelAttachment.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {

	CModelImageStack::CModelImageStack(_In_ const ModelResourceID sID, _In_ CModel* pModel, _In_ nfUint32 nRowCount, _In_ nfUint32 nColumCount, nfUint32 nSheetCount)
		: CModelImage3D(sID, pModel), m_nRowCount(nRowCount), m_nColumnCount(nColumCount), m_nSheetCount(nSheetCount)
	{
		m_Sheets.resize(m_nSheetCount);
	}

	PModelImageStack CModelImageStack::make(_In_ const ModelResourceID sID, _In_ CModel* pModel, _In_ nfUint32 nRowCount, _In_ nfUint32 nColumCount, nfUint32 nSheetCount)
	{
		if (!pModel)
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
		if (nRowCount > MAX_IMAGESTACK_SIZE)
			throw CNMRException(NMR_ERROR_INVALIDIMAGE3DSIZE);
		if (nColumCount > MAX_IMAGESTACK_SIZE)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (nSheetCount > MAX_IMAGESTACK_SIZE)
			throw CNMRException(MAX_IMAGESTACK_SIZE);

		return std::make_shared<CModelImageStack>(CModelImageStack(sID, pModel, nRowCount, nColumCount, nSheetCount));
	}

	nfUint32 CModelImageStack::getRowCount() const
	{
		return m_nRowCount;
	}

	nfUint32 CModelImageStack::getColumnCount() const
	{
		return m_nColumnCount;
	}

	nfUint32 CModelImageStack::getSheetCount() const
	{
		return m_nSheetCount;
	}

	void CModelImageStack::setRowCount(nfUint32 nRowCount)
	{
		if ((nRowCount == 0) || (nRowCount > MAX_IMAGESTACK_SIZE))
			throw CNMRException(NMR_ERROR_INVALIDIMAGE3DSIZE);
		m_nRowCount = nRowCount;
	}

	void CModelImageStack::setColumnCount(nfUint32 nColumnCount)
	{
		if ((nColumnCount == 0) || (nColumnCount > MAX_IMAGESTACK_SIZE))
			throw CNMRException(NMR_ERROR_INVALIDIMAGE3DSIZE);
		m_nColumnCount = nColumnCount;
	}

	void CModelImageStack::setSheetCount(nfUint32 nSheetCount)
	{
		if ((nSheetCount == 0) || (nSheetCount > MAX_IMAGESTACK_SIZE))
			throw CNMRException(NMR_ERROR_INVALIDIMAGE3DSIZE);
		m_nSheetCount = nSheetCount;
		m_Sheets.resize(m_nSheetCount);
	}

	void CModelImageStack::setSheet(nfUint32 nSheetIndex, PModelAttachment pAttachment)
	{
		if (!pAttachment.get())
			throw CNMRException(NMR_ERROR_INVALIDTEXTURE);
		if (pAttachment->getRelationShipType() != PACKAGE_TEXTURE_RELATIONSHIP_TYPE)
			throw CNMRException(NMR_ERROR_INVALIDRELATIONSHIPTYPEFORTEXTURE);
		if (nSheetIndex >= m_nSheetCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		if (pAttachment->getModel() != getModel())
			throw CNMRException(NMR_ERROR_ATTACHMENTMODELMISMATCH);

		m_Sheets[nSheetIndex] = pAttachment;
	}

	PModelAttachment CModelImageStack::getSheet(nfUint32 nSheetIndex)
	{
		if (nSheetIndex >= m_nSheetCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_Sheets[nSheetIndex];
	}

	PModelAttachment CModelImageStack::createSheet(nfUint32 nSheetIndex, const std::string& sPath, PImportStream pCopiedStream)
	{
		PModelAttachment pAttachment = getModel()->addAttachment(sPath, PACKAGE_TEXTURE_RELATIONSHIP_TYPE, pCopiedStream);
		setSheet(nSheetIndex, pAttachment);
		return pAttachment;
	}

}
