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

--*/

#include "Model/Classes/NMR_CustomXMLTree.h"
#include "Model/Classes/NMR_CustomXMLNode.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/Platform/NMR_XMLWriter_Native.h"
#include "Common/Platform/NMR_ExportStream_Memory.h"

namespace NMR {

	CCustomXMLTree::CCustomXMLTree(const std::string& sNameSpace, const std::string& sRootName)
		: m_sNameSpace (sNameSpace)
	{
		m_pRootNode = std::make_shared<CCustomXMLNode>(nullptr, sRootName);

	}

	CCustomXMLTree::~CCustomXMLTree()
	{

	}

	std::string CCustomXMLTree::getNameSpace()
	{
		return m_sNameSpace;
	}

	std::string CCustomXMLTree::getRootName()
	{
		return m_pRootNode->getName();
	}

	PCustomXMLNode CCustomXMLTree::getRootNode()
	{
		return m_pRootNode;
	}

	std::string CCustomXMLTree::saveToString()
	{
		auto pExportStream = std::make_shared<CExportStreamMemory>();
		auto pXMLWriter = std::make_shared<CXmlWriter_Native>(pExportStream);
		pXMLWriter->WriteStartDocument();
		pXMLWriter->WriteStartElement (nullptr, m_pRootNode->getName ().c_str (), m_sNameSpace.c_str ());

		m_pRootNode->writeContentToXML(pXMLWriter.get());

		pXMLWriter->WriteFullEndElement();

		pXMLWriter->WriteEndDocument();
		pXMLWriter->Flush();

		pExportStream->seekPosition(0, true);
		return std::string ((char*)pExportStream->getData(), pExportStream->getDataSize());
	}

}
