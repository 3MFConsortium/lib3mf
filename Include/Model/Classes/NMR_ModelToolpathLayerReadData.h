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

#ifndef __NMR_MODELTOOLPATHLAYERREADDATA
#define __NMR_MODELTOOLPATHLAYERREADDATA

#include "Common/NMR_Types.h" 
#include "Common/NMR_PagedVector.h" 

#include "Model/Classes/NMR_ModelToolpath.h" 
#include "Model/Classes/NMR_CustomXMLTree.h" 
#include "Model/Writer/NMR_ModelWriter.h" 
#include "Model/Writer/NMR_ModelWriter_3MF.h" 

#include <vector>
#include "Common/Platform/NMR_ImportStream.h"  
#include <memory>
#include <map>
#include <string>

#define TOOLPATHSEGMENTATTRIBUTEPAGESIZE 65536

namespace NMR {

	class CModelToolpath;
	typedef std::shared_ptr <CModelToolpath> PModelToolpath;

	enum class eModelToolpathSegmentType : nfUint32 {
		HatchSegment = 0,
		LoopSegment = 1,
		PolylineSegment = 2,
		ArcSegment = 3,
		SyncSegment = 4,
		DelaySegment = 5
	};

	enum class eModelToolpathSegmentAttributeType : nfUint32 {
		Unknown = 0,
		SegmentAttributeInt64 = 1,
		SegmentAttributeDouble = 2,
	};

	typedef struct {
		eModelToolpathSegmentType m_eType;
		nfUint32 m_nProfileID;
		nfUint32 m_nPartID;
		nfUint32 m_nStartPoint;
		nfUint32 m_nPointCount;
		nfUint32 m_nOverrideFraction;
		nfInt64* m_pAttributeData;
	} TOOLPATHREADSEGMENT;

	typedef struct {
		nfInt32 m_nX;
		nfInt32 m_nY;
		nfInt32 m_nTag;
		nfInt32 m_nFactorF;
		nfInt32 m_nFactorG;
		nfInt32 m_nFactorH;
	} TOOLPATHREADPOINT;

	typedef std::vector<int64_t> CToolpathLayerAttributePage;

	class CModelToolpathLayerReadData {
	private:
		std::string m_sUUID;

		PModelToolpath m_pModelToolpath;

		CPagedVector<TOOLPATHREADSEGMENT> m_Segments;
		std::vector<std::pair<eModelToolpathSegmentAttributeType, std::pair<std::string, std::string>>> m_SegmentAttributeDefinitions;

		std::map<std::pair<std::string, std::string>, std::pair<uint32_t, eModelToolpathSegmentAttributeType>> m_SegmentAttributeMap;

		std::vector<int64_t> m_AttributesOfNextSegment;

		std::vector<CToolpathLayerAttributePage> m_AttributeBuffer;
		CToolpathLayerAttributePage * m_pCurrentAttributePage;
		size_t m_nCurrentPositionOnPage;

		int64_t* allocAttributeBuffer(uint32_t nEntryCount);

		CPagedVector<TOOLPATHREADPOINT> m_Points;
		TOOLPATHREADSEGMENT * m_pCurrentSegment;

		std::map<uint32_t, std::string> m_UUIDMap;
		std::vector<uint32_t> m_PartIDs;
		std::vector<uint32_t> m_ProfileIDs;

		std::vector<PCustomXMLTree> m_CustomXMLData;

	public:
		CModelToolpathLayerReadData() = delete;
		CModelToolpathLayerReadData(_In_ PModelToolpath pModelToolpath);

		~CModelToolpathLayerReadData();		

		std::string getUUID();

		void beginSegment(eModelToolpathSegmentType eType, nfUint32 nProfileID, nfUint32 nPartID, nfUint32 nOverrideFraction);
		void endSegment();
		void addDiscretePoint (nfInt32 nX, nfInt32 nY, nfInt32 nTag, nfInt32 nFactorF, nfInt32 nFactorG, nfInt32 nFactorH);

		nfUint32 getSegmentCount();
		void getSegmentInfo (nfUint32 nSegmentIndex, eModelToolpathSegmentType & eType, nfUint32 & nProfileID, nfUint32 & nPartID, nfUint32 & nPointCount);
		TOOLPATHREADPOINT & getSegmentPoint (nfUint32 nSegmentIndex, nfUint32 nPointIndex);

		uint32_t getPartCount();

		void getPartInformation(nfUint32 nPartIndex, nfUint32 & nPartID, std::string& sPartUUID);

		void registerPartUUID (nfUint32 nID, std::string sUUID);
		void registerProfileUUID(nfUint32 nID, std::string sUUID);
		std::string mapIDtoUUID(nfUint32 nID);

		uint32_t getCustomXMLDataCount();
		PCustomXMLTree getCustomXMLData(uint32_t nIndex);
		void addCustomXMLData(PCustomXMLTree pCustomXMLTree);

		uint32_t registerCustomSegmentAttribute (const std::string& sNameSpace, const std::string& sAttributeName, eModelToolpathSegmentAttributeType eAttributeType);

		void clearSegmentAttributes ();
		void storeSegmentAttribute (const std::string & sNameSpace, const std::string & sAttributeName, const std::string & sAttributeValue);
		int64_t getSegmentIntegerAttribute(nfUint32 nSegmentIndex, uint32_t nAttributeID);
		double getSegmentDoubleAttribute(nfUint32 nSegmentIndex, uint32_t nAttributeID);
		std::pair<uint32_t, eModelToolpathSegmentAttributeType> findSegmentAttribute(const std::string& sNameSpace, const std::string& sAttributeName, bool bMustExist);

		bool segmentHasUniformProfile(nfUint32 nSegmentIndex);

		double getUnits();

	};

	typedef std::shared_ptr <CModelToolpathLayerReadData> PModelToolpathLayerReadData;



}

#endif // __NMR_MODELTOOLPATHLAYERREADDATA

