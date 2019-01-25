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

#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_NurbsSurface.h"
#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_UKnots.h"
#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_VKnots.h"
#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_ControlPoints.h"
#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_UVMapping.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelNurbsSurface.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <cmath>

namespace NMR {

	CModelReaderNode_ExactGeometry1901_NurbsSurface::CModelReaderNode_ExactGeometry1901_NurbsSurface(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_pModel = pModel;
		m_pWarnings = pWarnings;
		m_bHasDegreeU = false;
		m_nDegreeU = 0;
		m_bHasDegreeV = false;
		m_nDegreeV = 0;
		m_bHasID = false;
		m_nID = 0;

		m_pNurbsSurface = nullptr;
		m_bHadControlPoints = false;
		m_bHadUKnots = false;
		m_bHadVKnots = false;
		m_bHadUVMapping = false;
	}

	void CModelReaderNode_ExactGeometry1901_NurbsSurface::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		if ((!m_bHasDegreeU) || (!m_bHasDegreeV))
			throw CNMRException(NMR_ERROR_NURBSMISSINGDEGREE);
		if ((m_nID == 0) || (!m_bHasID))
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		// Parse Content
		parseContent(pXMLReader);


	}

	
	void CModelReaderNode_ExactGeometry1901_NurbsSurface::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_NURBS_ID) == 0) {

			if (m_bHasID)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			nfUint32 nValue = fnStringToUint32(pAttributeValue);
			m_nID = nValue;
			m_bHasID = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_NURBS_DEGREEU) == 0) {
			if (m_bHasDegreeU)
				throw CNMRException(NMR_ERROR_NURBSDUPLICATEATTRIBUTE);

			nfUint32 nValue = fnStringToUint32(pAttributeValue);
			if ((nValue == 0) || (nValue >= MAXNURBSDEGREE))
				throw CNMRException(NMR_ERROR_INVALIDNURBSDEGREE);
			m_nDegreeU = nValue;
			m_bHasDegreeU = true;
		} else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_NURBS_DEGREEV) == 0) {
			if (m_bHasDegreeV)
				throw CNMRException(NMR_ERROR_NURBSDUPLICATEATTRIBUTE);

			nfUint32 nValue = fnStringToUint32(pAttributeValue);
			if ((nValue == 0) || (nValue >= MAXNURBSDEGREE))
				throw CNMRException(NMR_ERROR_INVALIDNURBSDEGREE);
			m_nDegreeV = nValue;
			m_bHasDegreeV = true;
		}
		else

			m_pWarnings->addException(CNMRException(NMR_ERROR_NURBSINVALIDATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_ExactGeometry1901_NurbsSurface::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		

	}
	
	void CModelReaderNode_ExactGeometry1901_NurbsSurface::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_NURBSSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_NURBS_UKNOTS) == 0)
			{
				if (m_bHadUKnots)
					throw CNMRException(NMR_ERROR_NURBSDUPLICATEUKNOTS);

				PModelReaderNode_ExactGeometry1901_UKnots pXMLNode = std::make_shared<CModelReaderNode_ExactGeometry1901_UKnots>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);			

				m_KnotsU = pXMLNode->getKnots();
				m_bHadUKnots = true;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_NURBS_VKNOTS) == 0)
			{
				if (m_bHadVKnots)
					throw CNMRException(NMR_ERROR_NURBSDUPLICATEVKNOTS);

				PModelReaderNode_ExactGeometry1901_VKnots pXMLNode = std::make_shared<CModelReaderNode_ExactGeometry1901_VKnots>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				m_KnotsV = pXMLNode->getKnots();
				m_bHadVKnots = true;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_NURBS_CONTROLPOINTS) == 0)
			{
				if (m_bHadControlPoints)
					throw CNMRException(NMR_ERROR_NURBSDUPLICATECONTROLPOINTS);
				__NMRASSERT(m_pNurbsSurface.get() == nullptr);

				nfUint32 nControlPointCountU = m_nDegreeU + ((nfUint32)m_KnotsU.size()) + 1;
				nfUint32 nControlPointCountV = m_nDegreeV + ((nfUint32)m_KnotsV.size()) + 1;

				m_pNurbsSurface = std::make_shared<CModelNurbsSurface>(m_nID, m_pModel, m_nDegreeU, m_nDegreeV, nControlPointCountU, nControlPointCountV);

				m_pNurbsSurface->addKnotsU(m_KnotsU);
				m_pNurbsSurface->addKnotsV(m_KnotsV);

				m_bHadControlPoints = true;

				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_ExactGeometry1901_ControlPoints>(m_pModel, m_pNurbsSurface.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_NURBS_UVMAPPING) == 0)
			{
				if (m_pNurbsSurface.get() == nullptr)
					throw CNMRException(NMR_ERROR_INVALIDNURBSSURFACEORDER);

				if (m_bHadUVMapping)
					throw CNMRException(NMR_ERROR_NURBSDUPLICATEUVMAPPING);

				PModelReaderNode_ExactGeometry1901_UVMapping pXMLNode = std::make_shared<CModelReaderNode_ExactGeometry1901_UVMapping>(m_pModel, m_pNurbsSurface.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				m_bHadUVMapping = true;
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue); 
		}
	}
}
