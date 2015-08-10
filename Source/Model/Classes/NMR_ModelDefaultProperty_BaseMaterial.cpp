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

NMR_ModelDefaultProperty_BaseMaterial.cpp implements the Model Default Property Class.

--*/

#include "Model/Classes/NMR_ModelDefaultProperty_BaseMaterial.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 
#include <sstream>

namespace NMR {

	CModelDefaultProperty_BaseMaterial::CModelDefaultProperty_BaseMaterial(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nResourceIndex)
		: CModelDefaultProperty()
	{
		m_nResourceID = nResourceID;
		m_nResourceIndex = nResourceIndex;
	}
	
	CModelDefaultProperty_BaseMaterial::~CModelDefaultProperty_BaseMaterial()
	{
	}

	ModelResourceID CModelDefaultProperty_BaseMaterial::getResourceID ()
	{
		return m_nResourceID;
	}
	
	void CModelDefaultProperty_BaseMaterial::setResourceID (_In_ ModelResourceID nResourceID)
	{
		m_nResourceID = nResourceID;
	}
	
	ModelResourceIndex CModelDefaultProperty_BaseMaterial::getResourceIndex()
	{
		return m_nResourceIndex;
	}
	
	void CModelDefaultProperty_BaseMaterial::setResourceIndex(_In_ ModelResourceID nResourceIndex)
	{
		m_nResourceIndex = nResourceIndex;	
	}

}
