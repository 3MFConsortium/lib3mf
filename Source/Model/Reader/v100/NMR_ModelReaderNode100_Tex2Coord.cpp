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

NMR_ModelReaderNode100_Tex2Coord.cpp implements the Model Reader Tex2Coord Node Class.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Tex2Coord.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_Tex2Coord::CModelReaderNode100_Tex2Coord(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{

		m_pModel = pModel;
		m_bHasU = false;
		m_bHasV = false;
		m_sUVCoordinate.m_dU = 0.0;
		m_sUVCoordinate.m_dV = 0.0;
	}

	void CModelReaderNode100_Tex2Coord::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode100_Tex2Coord::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTUREVERTEX_U) == 0) {
			try {
				m_sUVCoordinate.m_dU = fnStringToDouble(pAttributeValue);
			}
			catch (...)
			{
			}
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTUREVERTEX_V) == 0) {
			try {
				m_sUVCoordinate.m_dV = fnStringToDouble(pAttributeValue);
			}
			catch (...)
			{
			}
		}
	}

	nfBool CModelReaderNode100_Tex2Coord::hasU()
	{
		return m_bHasU;
	}

	nfBool CModelReaderNode100_Tex2Coord::hasV()
	{
		return m_bHasV;
	}

	MODELTEXTURE2DCOORDINATE CModelReaderNode100_Tex2Coord::getUV()
	{
		return m_sUVCoordinate;
	}
}
