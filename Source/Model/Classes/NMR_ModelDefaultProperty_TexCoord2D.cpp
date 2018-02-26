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

NMR_ModelDefaultProperty_TexCoord2D.cpp implements the Model Default Property Class.

--*/

#include "Model/Classes/NMR_ModelDefaultProperty_TexCoord2D.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 
#include <sstream>

namespace NMR {

	CModelDefaultProperty_TexCoord2D::CModelDefaultProperty_TexCoord2D(_In_ ModelResourceID nTextureID, _In_ nfFloat fU, _In_ nfFloat fV)
		: CModelDefaultProperty()
	{
		if (nTextureID == 0)
			throw CNMRException(NMR_ERROR_INVALIDDEFAULTPID);
		m_nTextureID = nTextureID;
		m_fU = fU;
		m_fV = fV;
	}

	CModelDefaultProperty_TexCoord2D::~CModelDefaultProperty_TexCoord2D()
	{
	}

	ModelResourceID CModelDefaultProperty_TexCoord2D::getTextureID()
	{
		return m_nTextureID;
	}

	void CModelDefaultProperty_TexCoord2D::setTextureID(_In_ ModelResourceID nTextureID)
	{
		m_nTextureID = nTextureID;
	}

	nfFloat CModelDefaultProperty_TexCoord2D::getU()
	{
		return m_fU;
	}

	void CModelDefaultProperty_TexCoord2D::setU(_In_ nfFloat fU)
	{
		m_fU = fU;
	}

	nfFloat CModelDefaultProperty_TexCoord2D::getV()
	{
		return m_fV;
	}

	void CModelDefaultProperty_TexCoord2D::setV(_In_ nfFloat fV)
	{
		m_fV = fV;
	}

}

