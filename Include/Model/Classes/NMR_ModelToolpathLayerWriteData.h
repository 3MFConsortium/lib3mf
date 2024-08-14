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

NMR_ModelToolpath.h defines the Model Toolpath Layer Data.

--*/

#ifndef __NMR_MODELTOOLPATHLAYERWRITEDATA
#define __NMR_MODELTOOLPATHLAYERWRITEDATA

#include "Common/NMR_Types.h" 

#include "Model/Classes/NMR_ModelToolpath.h" 
#include "Model/Writer/NMR_ModelWriter.h" 
#include "Model/Writer/NMR_ModelWriter_3MF.h" 

#include "Common/Platform/NMR_XmlWriter.h" 
#include "Common/Platform/NMR_ExportStream_Memory.h" 

#include <vector>
#include "Common/Platform/NMR_ImportStream.h"  
#include <memory>
#include <map>
#include <string>

#include "Common/ChunkedBinaryStream/NMR_ChunkedBinaryStreamWriter.h"

namespace NMR {

	class CModelToolpath;
	typedef std::shared_ptr <CModelToolpath> PModelToolpath;

	class CModelToolpathLayerWriteData {
	private:
		std::string m_sUUID;
		std::string m_sPackagePath;

		CModelToolpath * m_pModelToolpath;

		NMR::PModelWriter_3MF m_pModelWriter;

		NMR::PXmlWriter m_pXmlWriter;
		NMR::PExportStreamMemory m_pExportStream;

		std::map <unsigned int, PModelToolpathProfile> m_Profiles;
		std::map <unsigned int, PModelBuildItem> m_Parts;

		bool m_bWritingHeader;
		bool m_bWritingData;
		bool m_bWritingFinished;
		unsigned int m_nIDCounter;

		std::vector<PCustomXMLTree> m_CustomXMLData;
		std::map<std::pair<std::string, std::string>, std::string> m_CustomSegmentAttributes;
		std::map<std::string, std::string> m_NameSpaceToPrefixMap;
		std::map<std::string, std::string> m_PrefixToNameSpaceMap;

		double m_dUnits;

		NMR::CChunkedBinaryStreamWriter * getStreamWriter(std::string & sPath);

		NMR::PImportStream createStream();

		void writeCustomXMLDataNode(PCustomXMLNode pXMLNode, const std::string & sNameSpace);

		std::string findNameSpacePrefix(const std::string& sNameSpace);

	public:
		CModelToolpathLayerWriteData() = delete;
		CModelToolpathLayerWriteData(CModelToolpath * pModelToolpath, NMR::PModelWriter_3MF pModelWriter, const std::string & sPackagePath, std::map<std::string, std::string> PrefixToNameSpaceMap);

		~CModelToolpathLayerWriteData();

		void addCustomXMLData (PCustomXMLTree pXMLTree);

		nfUint32 RegisterProfile(PModelToolpathProfile pProfile);

		nfUint32 RegisterPart(PModelBuildItem pBuildItem);

		void WriteHatchData(const nfUint32 nProfileID, const nfUint32 nPartID, const nfUint32 nHatchCount, const nfInt32 * pX1Buffer, const nfInt32 * pY1Buffer, const nfInt32 * pX2Buffer, const nfInt32 * pY2Buffer);

		void WriteLoop(const nfUint32 nProfileID, const nfUint32 nPartID, const nfUint32 nPointCount, const nfInt32 * pXBuffer, const nfInt32 * pYBuffer);

		void WritePolyline(const nfUint32 nProfileID, const nfUint32 nPartID, const nfUint32 nPointCount, const nfInt32 * pXBuffer, const nfInt32 * pYBuffer);

		void finishHeader();

		double getUnits();

		std::string getUUID();

		void finishWriting();

		void setCustomSegmentAttribute(const std::string& sNameSpace, const std::string& sAttributeName, const std::string& sValue);

		void clearCustomSegmentAttributes();



	};

	typedef std::shared_ptr <CModelToolpathLayerWriteData> PModelToolpathLayerWriteData;

}

#endif // __NMR_MODELTOOLPATHLAYERWRITEDATA

