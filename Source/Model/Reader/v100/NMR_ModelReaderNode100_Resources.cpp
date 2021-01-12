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
NMR_ModelReaderNode100_Resources.cpp implements the Model Reader Resources Node
Class. A resources reader model node is a parser for the resources node of an
XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Resources.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Object.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_BaseMaterials.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Colors.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Tex2DGroup.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Texture2D.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_CompositeMaterials.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_MultiProperties.h"
#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceStack.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Model/Classes/NMR_ModelConstants_Slices.h"

namespace NMR {

	CModelReaderNode100_Resources::CModelReaderNode100_Resources(_In_ CModel * pModel, _In_ PModelWarnings pWarnings, _In_z_ const std::string sPath,
		_In_ PProgressMonitor pProgressMonitor)
		: CModelReaderNode(pWarnings, pProgressMonitor)
	{
		__NMRASSERT(pModel);
		__NMRASSERT(sPath);
		m_pModel = pModel;
		m_sPath = sPath;
		m_nProgressCount = 0;
	}

	void CModelReaderNode100_Resources::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_Resources::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	void CModelReaderNode100_Resources::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_OBJECT) == 0) {
				m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READRESOURCES);
				m_pProgressMonitor->ReportProgressAndQueryCancelled(true);

				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Object>(m_pModel, m_pWarnings, m_pProgressMonitor);
				pXMLNode->parseXML(pXMLReader);

			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_BASEMATERIALS) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_BaseMaterials>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_MATERIALSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_COLORGROUP) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Colors>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_TEX2DGROUP) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Tex2DGroup>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_TEXTURE2D) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Texture2D>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_COMPOSITEMATERIALS) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_CompositeMaterials>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_MULTIPROPERTIES) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_MultiProperties>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			// there might be other things, that are not yet properly implemented in lib3MF
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_SLICESPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_SLICESTACKRESOURCE) == 0) {

				m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READRESOURCES);
				m_pProgressMonitor->ReportProgressAndQueryCancelled(true);

				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_Slice1507_SliceStack>(
					m_pModel, m_pWarnings, m_pProgressMonitor, m_sPath.c_str());
				pXMLNode->parseXML(pXMLReader);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}


	}


}
