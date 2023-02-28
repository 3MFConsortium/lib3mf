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

NMR_ModelVector3DFieldConstant.cpp implements the Model Vector3DFieldConstant Class.
--*/

#include "Model/Classes/NMR_ModelVector3DFieldConstant.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	CModelVector3DFieldConstant::CModelVector3DFieldConstant(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelVector3DField(sID, pModel)
	{
	
	}

	nfDouble CModelVector3DFieldConstant::getValueX() const
	{
		return m_dValueX;
	}
	void CModelVector3DFieldConstant::setValueX(nfDouble dValueX)
	{
		m_dValueX = dValueX;
	}

	nfDouble CModelVector3DFieldConstant::getValueY() const
	{
		return m_dValueY;
	}
	void CModelVector3DFieldConstant::setValueY(nfDouble dValueY)
	{
		m_dValueY = dValueY;
	}

	nfDouble CModelVector3DFieldConstant::getValueZ() const
	{
		return m_dValueZ;
	}
	void CModelVector3DFieldConstant::setValueZ(nfDouble dValueZ)
	{
		m_dValueZ = dValueZ;
	}

}
