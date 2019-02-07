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

#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_NurbsCurve.h"
#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_CurveKnots.h"
#include "Model/Reader/ExactGeometry1901/NMR_ModelReader_ExactGeometry1901_CurveControlPoints.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelNurbsSurface.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <cmath>

namespace NMR {

	CModelReaderNode_ExactGeometry1901_NurbsCurve::CModelReaderNode_ExactGeometry1901_NurbsCurve(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_pModel = pModel;
		m_pWarnings = pWarnings;
		m_bHasDegree = false;
		m_nDegree = 0;
		m_bHasID = false;
		m_nID = 0;

		m_pNurbsCurve = nullptr;
		m_bHadControlPoints = false;
		m_bHadKnots = false;
	}

	void CModelReaderNode_ExactGeometry1901_NurbsCurve::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		if (!m_bHasDegree)
			throw CNMRException(NMR_ERROR_NURBSMISSINGDEGREE);
		if ((m_nID == 0) || (!m_bHasID))
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		// Parse Content
		parseContent(pXMLReader);

		if (m_pNurbsCurve == nullptr)
			throw CNMRException(NMR_ERROR_NURBSINSUFFICIENTDESCRIPTION);
		

		m_pModel->addResource(m_pNurbsCurve);


	}

	
	void CModelReaderNode_ExactGeometry1901_NurbsCurve::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
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
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_NURBS_DEGREE) == 0) {
			if (m_bHasDegree)
				throw CNMRException(NMR_ERROR_NURBSDUPLICATEATTRIBUTE);

			nfUint32 nValue = fnStringToUint32(pAttributeValue);
			if ((nValue == 0) || (nValue >= MAXNURBSDEGREE))
				throw CNMRException(NMR_ERROR_INVALIDNURBSDEGREE);
			m_nDegree = nValue;
			m_bHasDegree = true;
		}
		else

			m_pWarnings->addException(CNMRException(NMR_ERROR_NURBSINVALIDATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_ExactGeometry1901_NurbsCurve::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		

	}
	
	void CModelReaderNode_ExactGeometry1901_NurbsCurve::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_NURBSSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_NURBS_KNOTS) == 0)
			{
				if (m_bHadKnots)
					throw CNMRException(NMR_ERROR_NURBSDUPLICATECURVEKNOTS);

				PModelReaderNode_ExactGeometry1901_CurveKnots pXMLNode = std::make_shared<CModelReaderNode_ExactGeometry1901_CurveKnots>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);			

				m_Knots = pXMLNode->getKnots();
				m_bHadKnots = true;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_NURBS_CONTROLPOINTS) == 0)
			{
				if (m_bHadControlPoints)
					throw CNMRException(NMR_ERROR_NURBSDUPLICATECONTROLPOINTS);
				__NMRASSERT(m_pNurbsCurve.get() == nullptr);

				nfInt32 nKnotsMultiplicitiesSum = 0;
				auto iKnotsIter = m_Knots.begin();
				while (iKnotsIter != m_Knots.end()) {
					nKnotsMultiplicitiesSum += iKnotsIter->m_Multiplicity;
					iKnotsIter++;
				}

				nfInt32 nControlPointCount = nKnotsMultiplicitiesSum - (nfInt32) (m_nDegree + 1);
				if (nControlPointCount < 1)
					throw CNMRException(NMR_ERROR_INVALIDCONTROLPOINTCOUNT);


				m_pNurbsCurve = std::make_shared<CModelNurbsCurve>(m_nID, m_pModel, m_nDegree, nControlPointCount);

				m_pNurbsCurve->addKnots(m_Knots);

				m_bHadControlPoints = true;

				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_ExactGeometry1901_CurveControlPoints>(m_pModel, m_pNurbsCurve.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue); 
		}
	}
}

