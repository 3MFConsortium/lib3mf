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

#define TOOLPATHREADSEGMENTFLAG_HASFACTORF 0x01
#define TOOLPATHREADSEGMENTFLAG_HASFACTORG 0x02
#define TOOLPATHREADSEGMENTFLAG_HASFACTORH 0x04
#define TOOLPATHREADSEGMENTFLAG_HASFACTORE 0x08

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
		nfUint32 m_nFlags;
		nfUint32 m_nProfileID;
		nfUint32 m_nPartID;
		nfUint32 m_nStartPoint;
		nfUint32 m_nPointCount;
		nfUint32 m_nOverrideInterpolationCount;
		nfInt64* m_pAttributeData;
	} TOOLPATHREADSEGMENT;

	typedef struct {
		nfInt32 m_nX;
		nfInt32 m_nY;
		nfInt32 m_nTag;
		nfUint32 m_nFlags;
		nfDouble m_nFactorF;
		nfDouble m_nFactorG;
		nfDouble m_nFactorH;
		nfDouble m_nFactorE;
		uint32_t m_nOverrideStart;
		uint32_t m_nOverrideCount;
	} TOOLPATHREADPOINT;

	typedef struct {
		nfDouble m_dParameter;
		nfDouble m_dFactorF;
		nfDouble m_dFactorG;
		nfDouble m_dFactorH;
		nfDouble m_dFactorE;
	} TOOLPATHREADOVERRIDE;


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
		CPagedVector<TOOLPATHREADOVERRIDE> m_OverrideInterpolations;
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

		void beginSegment(eModelToolpathSegmentType eType, nfUint32 nProfileID, nfUint32 nPartID);
		void endSegment();
		void addDiscretePoint (nfInt32 nX, nfInt32 nY, nfInt32 nTag, bool bHasFactorF, nfDouble nFactorF, bool bHasFactorG, nfDouble nFactorG, bool bHasFactorH, nfDouble nFactorH, bool bHasFactorE, nfDouble nFactorE, uint32_t nOverrideStart, uint32_t nOverrideCount);

		TOOLPATHREADOVERRIDE& getOverrideInterpolationData(uint32_t nGlobalIndex);
		uint32_t getGlobalOverrideInterpolationCount();
		void addOverrideInterpolation (double dParameter, double dFactorF, double dFactorG, double dFactorH, double dFactorE);

		nfUint32 getSegmentCount();
		void getSegmentInfo (nfUint32 nSegmentIndex, eModelToolpathSegmentType & eType, nfUint32 & nProfileID, nfUint32 & nPartID, nfUint32 & nPointCount);
		eModelToolpathSegmentType getSegmentType(nfUint32 nSegmentIndex);
		TOOLPATHREADPOINT & getSegmentPoint (nfUint32 nSegmentIndex, nfUint32 nPointIndex);
		void getSegmentHatchOverrideInterpolationIndices(nfUint32 nSegmentIndex, nfUint32 nHatchIndex, nfUint32 & nOverrideStartIndex, nfUint32 & nOverrideCount);
		uint32_t getSegmentOverrideInterpolationCount(uint32_t nSegmentIndex);
        bool segmentHasModificationFactors(nfUint32 nSegmentIndex, Lib3MF::eToolpathProfileModificationFactor modificationFactor);

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

		double getUnits();

	};

	typedef std::shared_ptr <CModelToolpathLayerReadData> PModelToolpathLayerReadData;



}

#endif // __NMR_MODELTOOLPATHLAYERREADDATA

