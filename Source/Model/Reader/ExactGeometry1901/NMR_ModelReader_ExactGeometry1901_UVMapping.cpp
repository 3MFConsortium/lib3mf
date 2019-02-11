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

NMR_ModelReaderNode_ExactGeometry1901_NurbsSurface.cpp covers the official 3MF Exact Geometry Extension.

--*/

#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_UVMapping.h"
#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_UVCoord.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <cmath>

namespace NMR {

	CModelReaderNode_ExactGeometry1901_UVMapping::CModelReaderNode_ExactGeometry1901_UVMapping(_In_ CModel * pModel, _In_ CModelNurbsSurface * pNurbsSurface, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		if (pNurbsSurface == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pModel = pModel;
		m_pNurbsSurface = pNurbsSurface;
		m_pWarnings = pWarnings;

		m_dMinU = 0.0;
		m_dMinV = 0.0;
		m_dMaxU = 0.0;
		m_dMaxV = 0.0;

		m_bHasMinU = false;
		m_bHasMinV = false;
		m_bHasMaxU = false;
		m_bHasMaxV = false;

		if (!m_bHasMinU || !m_bHasMinV || !m_bHasMaxU || !m_bHasMaxV)
			throw CNMRException(NMR_ERROR_INVALIDUVBOUNDS);

		m_pNurbsSurface->setUVBounds(m_dMinU, m_dMinV, m_dMaxU, m_dMaxV);

	}

	void CModelReaderNode_ExactGeometry1901_UVMapping::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	
	void CModelReaderNode_ExactGeometry1901_UVMapping::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_NURBS_MINU) == 0) {
			nfDouble dValue = fnStringToDouble(pAttributeValue);
			if (std::isnan(dValue) || (dValue < -XML_3MF_MAXIMUMCOORDINATEVALUE) || (dValue > XML_3MF_MAXIMUMCOORDINATEVALUE))
				throw CNMRException(NMR_ERROR_NURBSINVALIDATTRIBUTE);
			m_dMinU = dValue;
			m_bHasMinU = true;

		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_NURBS_MINV) == 0) {
			nfDouble dValue = fnStringToDouble(pAttributeValue);
			if (std::isnan(dValue) || (dValue < -XML_3MF_MAXIMUMCOORDINATEVALUE) || (dValue > XML_3MF_MAXIMUMCOORDINATEVALUE))
				throw CNMRException(NMR_ERROR_NURBSINVALIDATTRIBUTE);
			m_dMinV = dValue;
			m_bHasMinV = true;

		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_NURBS_MAXU) == 0) {
			nfDouble dValue = fnStringToDouble(pAttributeValue);
			if (std::isnan(dValue) || (dValue < -XML_3MF_MAXIMUMCOORDINATEVALUE) || (dValue > XML_3MF_MAXIMUMCOORDINATEVALUE))
				throw CNMRException(NMR_ERROR_NURBSINVALIDATTRIBUTE);
			m_dMaxU = dValue;
			m_bHasMaxU = true;

		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_NURBS_MAXV) == 0) {
			nfDouble dValue = fnStringToDouble(pAttributeValue);
			if (std::isnan(dValue) || (dValue < -XML_3MF_MAXIMUMCOORDINATEVALUE) || (dValue > XML_3MF_MAXIMUMCOORDINATEVALUE))
				throw CNMRException(NMR_ERROR_NURBSINVALIDATTRIBUTE);
			m_dMaxV = dValue;
			m_bHasMaxV = true;

		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NURBSINVALIDATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_ExactGeometry1901_UVMapping::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		

	}
	
	void CModelReaderNode_ExactGeometry1901_UVMapping::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_NURBSSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_NURBS_UVCOORD) == 0)
			{
				PModelReaderNode_ExactGeometry1901_UVCoord pXMLNode = std::make_shared<CModelReaderNode_ExactGeometry1901_UVCoord>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				nfDouble fU;
				nfDouble fV;
				pXMLNode->retrieveCoord(fU, fV);

				m_pNurbsSurface->addUVCoordinate(fU, fV);

			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}


}
