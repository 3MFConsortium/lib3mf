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

--*/

#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceStack.h"
#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRef.h"
#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Slice.h"
#include "Common/NMR_StringUtils.h"
#include "Model/Classes/NMR_ModelConstants.h"

namespace NMR {
	void CModelReaderNode_Slice1507_SliceStack::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue) {
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SLICESTACKZBOTTOM) == 0) {
			m_BottomZ = fnStringToFloat(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SLICESTACKID) == 0) {
			m_Id = fnStringToUint32(pAttributeValue);
		}
		else
			throw CNMRException(NMR_ERROR_SLICE_INVALIDATTRIBUTE);
	}

	void CModelReaderNode_Slice1507_SliceStack::OnNSChildElement(
		_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (strcmp(pChildName, XML_3MF_ELEMENT_SLICE) == 0) {
			if (!m_pSliceStackResource->AllowsGeometry())
				throw CNMRException(NMR_ERROR_SLICESTACK_SLICESANDSLICEREF);
			PModelReaderNode_Slices1507_Slice pXMLNode = nullptr;

			if (m_pSliceStackResource->getSliceCount() % PROGRESS_READSLICESUPDATE == PROGRESS_READSLICESUPDATE - 1 ) {
				m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READSLICES);
				m_pProgressMonitor->ReportProgressAndQueryCancelled(true);
			}

			pXMLNode = std::make_shared<CModelReaderNode_Slices1507_Slice>(m_pSliceStackResource.get(), m_pWarnings);
			pXMLNode->parseXML(pXMLReader);
		}
		else if (strcmp(pChildName, XML_3MF_ELEMENT_SLICEREFRESOURCE) == 0) {
			if (!m_pSliceStackResource->AllowsReferences())
				throw CNMRException(NMR_ERROR_SLICESTACK_SLICESANDSLICEREF);
			PModelReaderNode_Slice1507_SliceRef pXmlNode = std::make_shared<CModelReaderNode_Slice1507_SliceRef>(m_pWarnings);
			pXmlNode->parseXML(pXMLReader);
			
			std::string path = pXmlNode->Path();
			if (path.empty()) {
				path = m_pModel->currentPath();
			}
			PModelResource pResource = m_pModel->findResource(path, pXmlNode->SliceStackId());
			PModelSliceStack pSliceStackResource = std::dynamic_pointer_cast<CModelSliceStack>(pResource);
			if (!pSliceStackResource)
				throw CNMRException(NMR_ERROR_SLICESTACKRESOURCE_NOT_FOUND);
			if (m_pSliceStackResource->OwnPath() != m_pModel->rootPath() )
				throw CNMRException(NMR_ERROR_SLICEREFSTOODEEP);
			m_pSliceStackResource->AddSliceRef(pSliceStackResource);
		}
	}

	CModelReaderNode_Slice1507_SliceStack::CModelReaderNode_Slice1507_SliceStack(
		_In_ CModel * pModel, _In_ PModelWarnings pWarnings,
		_In_ PProgressMonitor pProgressMonitor,_In_ const std::string sSlicePath)
		: CModelReaderNode(pWarnings, pProgressMonitor)
	{
		m_sSlicePath = sSlicePath;
		m_pModel = pModel;
	}

	void CModelReaderNode_Slice1507_SliceStack::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		m_pSliceStackResource = std::make_shared<CModelSliceStack>(m_Id, m_pModel, m_BottomZ);

		// Parse Content
		parseContent(pXMLReader);

		m_pSliceStackResource->SetOwnPath(m_sSlicePath);

		m_pModel->addResource(m_pSliceStackResource);
	}
}
