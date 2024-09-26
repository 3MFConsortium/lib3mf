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

NMR_ModelToolpathLayer.cpp defines the Model Toolpath Layer.

--*/

#include "Model/Classes/NMR_ModelToolpathLayerWriteData.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelBuildItem.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/Platform/NMR_XmlWriter_Native.h"
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"

#include "Common/NMR_UUID.h"

#include <sstream>
#include <algorithm>

namespace NMR {

	CModelToolpathLayerWriteData::CModelToolpathLayerWriteData(CModelToolpath * pModelToolpath, NMR::PModelWriter_3MF pModelWriter, const std::string & sPackagePath, std::map<std::string, std::string> PrefixToNameSpaceMap)
		: m_pModelToolpath (pModelToolpath), m_pModelWriter (pModelWriter), m_sPackagePath (sPackagePath), m_PrefixToNameSpaceMap (PrefixToNameSpaceMap),
		m_nCurrentLaserIndex (0), m_nFactorRange (TOOLPATHWRITER_DEFAULTFACTORRANGE)
	{
		if (pModelToolpath == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pModelWriter.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		CUUID uuid;
		m_sUUID = uuid.toString();

		m_pExportStream = std::make_shared<NMR::CExportStreamMemory>();

		m_pXmlWriter = std::make_shared<NMR::CXmlWriter_Native>(m_pExportStream);
		m_pXmlWriter->WriteStartDocument();
		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_LAYER, XML_3MF_NAMESPACE_TOOLPATHSPEC);
		m_pXmlWriter->WriteAttributeString(XML_3MF_ATTRIBUTE_XMLNS, XML_3MF_NAMESPACEPREFIX_BINARY, nullptr, XML_3MF_NAMESPACE_BINARYSPEC);

		if (m_PrefixToNameSpaceMap.find(XML_3MF_NAMESPACEPREFIX_BINARY) != m_PrefixToNameSpaceMap.end())
			throw CNMRException(NMR_ERROR_NAMESPACEPREFIXISRESERVED);

		for (auto iIter : m_PrefixToNameSpaceMap) {
			m_pXmlWriter->WriteAttributeString(XML_3MF_ATTRIBUTE_XMLNS, iIter.first.c_str (), nullptr, iIter.second.c_str ());
			m_NameSpaceToPrefixMap.insert(std::make_pair (iIter.second, iIter.first));
		}

		m_bWritingHeader = true;
		m_bWritingData = false;
		m_bWritingFinished = false;

		m_nIDCounter = 1;

	}

	CModelToolpathLayerWriteData::~CModelToolpathLayerWriteData()
	{
		if (!m_bWritingFinished)
			finishWriting ();
	}

	void CModelToolpathLayerWriteData::addCustomXMLData(PCustomXMLTree pXMLTree)
	{
		if (!m_bWritingHeader)
			throw CNMRException(NMR_ERROR_TOOLPATH_NOTWRITINGHEADER);

		if (pXMLTree.get () == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_CustomXMLData.push_back(pXMLTree); 

	}


	nfUint32 CModelToolpathLayerWriteData::RegisterProfile(PModelToolpathProfile pProfile)
	{
		if (!m_bWritingHeader)
			throw CNMRException(NMR_ERROR_TOOLPATH_NOTWRITINGHEADER);

		unsigned int nNewID = m_nIDCounter;
		m_nIDCounter++;

		m_Profiles.insert(std::make_pair(nNewID, pProfile));

		return nNewID;
	}

	nfUint32 CModelToolpathLayerWriteData::RegisterPart(PModelBuildItem pBuildItem)
	{
		if (!m_bWritingHeader)
			throw CNMRException(NMR_ERROR_TOOLPATH_NOTWRITINGHEADER);

		unsigned int nNewID = m_nIDCounter;
		m_nIDCounter++;

		m_Parts.insert(std::make_pair(nNewID, pBuildItem));

		return nNewID;
	}

	void CModelToolpathLayerWriteData::WriteHatchData(const nfUint32 nProfileID, const nfUint32 nPartID, const nfUint32 nHatchCount, const nfInt32* pX1Buffer, const nfInt32* pY1Buffer, const nfInt32* pX2Buffer, const nfInt32* pY2Buffer, const nfInt32* pTagBuffer, const nfInt32* pProfileIDBuffer, const int32_t* pScalingData1Buffer, const int32_t* pScalingData2Buffer)
	{
		std::string sPath;
		NMR::CChunkedBinaryStreamWriter * pStreamWriter = getStreamWriter(sPath);

		if (pX1Buffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pY1Buffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pX2Buffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pY2Buffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (m_bWritingHeader)
			finishHeader();
		if (!m_bWritingData)
			throw CNMRException(NMR_ERROR_TOOLPATH_NOTWRITINGDATA);

		std::string sPartID = std::to_string(nPartID);
		std::string sProfileID = std::to_string(nProfileID);

		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_SEGMENT, nullptr);
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_TYPE, nullptr, XML_3MF_TOOLPATHTYPE_HATCH);
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PROFILEID, nullptr, sProfileID.c_str());
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PARTID, nullptr, sPartID.c_str());
		
		if (m_nCurrentLaserIndex != 0) {
			std::string sLaserIndex = std::to_string(m_nCurrentLaserIndex);
			m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_LASERINDEX, nullptr, sLaserIndex.c_str());
		}

		if (pScalingData1Buffer != nullptr) {
			std::string sFactorRange = std::to_string(m_nFactorRange);
			m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_MAXFACTOR, nullptr, sFactorRange.c_str());
		}

		for (auto& customAttribute : m_CustomSegmentAttributes) {
			std::string sNameSpace = customAttribute.first.first;
			std::string sAttributeName = customAttribute.first.second;
			std::string sAttributeValue = customAttribute.second;
			std::string sAttributePrefix = findNameSpacePrefix (sNameSpace);
			m_pXmlWriter->WriteAttributeString(sAttributePrefix.c_str (), sAttributeName.c_str (), nullptr, sAttributeValue.c_str ());
		}

		if (pStreamWriter != nullptr) {
			uint32_t binaryKeyX1 = pStreamWriter->addIntArray(pX1Buffer, nHatchCount, eptDeltaPredicition);
			uint32_t binaryKeyY1 = pStreamWriter->addIntArray(pY1Buffer, nHatchCount, eptDeltaPredicition);
			uint32_t binaryKeyX2 = pStreamWriter->addIntArray(pX2Buffer, nHatchCount, eptDeltaPredicition);
			uint32_t binaryKeyY2 = pStreamWriter->addIntArray(pY2Buffer, nHatchCount, eptDeltaPredicition);
			uint32_t binaryKeyTags = 0;
			if (pTagBuffer != nullptr)
				binaryKeyTags = pStreamWriter->addIntArray(pTagBuffer, nHatchCount, eptDeltaPredicition);

			uint32_t binaryKeyProfileIDs = 0;
			if (pProfileIDBuffer != nullptr)
				binaryKeyProfileIDs = pStreamWriter->addIntArray(pProfileIDBuffer, nHatchCount, eptDeltaPredicition);

			std::string sKeyX1 = std::to_string(binaryKeyX1);
			std::string sKeyY1 = std::to_string(binaryKeyY1);
			std::string sKeyX2 = std::to_string(binaryKeyX2);
			std::string sKeyY2 = std::to_string(binaryKeyY2);

			m_pXmlWriter->WriteStartElement(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHELEMENT_HATCH, nullptr);
			m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_X1, nullptr, sKeyX1.c_str());
			m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_Y1, nullptr, sKeyY1.c_str());
			m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_X2, nullptr, sKeyX2.c_str());
			m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_Y2, nullptr, sKeyY2.c_str());

			if (binaryKeyProfileIDs != 0) {
				std::string sKeyProfileIDs = std::to_string(binaryKeyProfileIDs);
				m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_PID, nullptr, sKeyProfileIDs.c_str());
			}

			if (binaryKeyTags != 0) {
				std::string sKeyTags = std::to_string(binaryKeyTags);
				m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_TAG, nullptr, sKeyTags.c_str());
			}

			m_pXmlWriter->WriteEndElement();


		}
		else { 

			// TODO: make fast!
			uint32_t nIndex;
			for (nIndex = 0; nIndex < nHatchCount; nIndex++) {
				std::string sX1 = std::to_string(pX1Buffer[nIndex]);
				std::string sY1 = std::to_string(pY1Buffer[nIndex]);
				std::string sX2 = std::to_string(pX2Buffer[nIndex]);
				std::string sY2 = std::to_string(pY2Buffer[nIndex]);

				m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_HATCH, nullptr);
				m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_X1, nullptr, sX1.c_str());
				m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_Y1, nullptr, sY1.c_str());
				m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_X2, nullptr, sX2.c_str());
				m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_Y2, nullptr, sY2.c_str());

				if (pProfileIDBuffer != nullptr) {
					int32_t nProfileID = pProfileIDBuffer[nIndex];
					if (nProfileID != 0) {
						std::string sProfileID = std::to_string(nProfileID);
						m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PID, nullptr, sProfileID.c_str());
					}
				}

				if (pTagBuffer != nullptr) {
					int32_t nTag = pTagBuffer[nIndex];
					if (nTag != 0) {
						std::string sTag = std::to_string(nTag);
						m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_TAG, nullptr, sTag.c_str());
					}
				}

				if (pScalingData1Buffer != nullptr) {

					int32_t nScalingValue1 = pScalingData1Buffer[nIndex];
					int32_t nScalingValue2 = nScalingValue1;

					if (pScalingData2Buffer != nullptr) {
						nScalingValue2 = pScalingData2Buffer[nIndex];
					}

					if (nScalingValue1 == nScalingValue2) {
						if (nScalingValue1 != 0) {
							std::string sScalingValue = std::to_string(nScalingValue1);
							m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF, nullptr, sScalingValue.c_str());
						}
					}
					else {
						if (nScalingValue1 != 0) {
							std::string sScalingValue1 = std::to_string(nScalingValue1);
							m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF1, nullptr, sScalingValue1.c_str());
						}
						if (nScalingValue2 != 0) {
							std::string sScalingValue2 = std::to_string(nScalingValue2);
							m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF2, nullptr, sScalingValue2.c_str());
						}

					}


				}
				else {
					if (pScalingData2Buffer != nullptr) {
						throw CNMRException(NMR_ERROR_TOOLPATH_INVALIDPROFILESCALINGBUFFER);
					}

				}

				m_pXmlWriter->WriteEndElement();

			}

		}

		m_pXmlWriter->WriteFullEndElement();

	}

	void CModelToolpathLayerWriteData::WriteLoop(const nfUint32 nProfileID, const nfUint32 nPartID, const nfUint32 nPointCount, const nfInt32 * pXBuffer, const nfInt32 * pYBuffer, const int32_t* pTagDataBuffer, const int32_t* pScalingDataBuffer)
	{
		if (pXBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pYBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::string sPath;
		NMR::CChunkedBinaryStreamWriter * pStreamWriter = getStreamWriter(sPath);

		if (m_bWritingHeader)
			finishHeader();
		if (!m_bWritingData)
			throw CNMRException(NMR_ERROR_TOOLPATH_NOTWRITINGDATA);

		std::string sPartID = std::to_string(nPartID);
		std::string sProfileID = std::to_string(nProfileID);

		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_SEGMENT, nullptr);
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_TYPE, nullptr, XML_3MF_TOOLPATHTYPE_LOOP);
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PROFILEID, nullptr, sProfileID.c_str());
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PARTID, nullptr, sPartID.c_str());


		if (m_nCurrentLaserIndex != 0) {
			std::string sLaserIndex = std::to_string(m_nCurrentLaserIndex);
			m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_LASERINDEX, nullptr, sLaserIndex.c_str());
		}

		for (auto& customAttribute : m_CustomSegmentAttributes) {
			std::string sNameSpace = customAttribute.first.first;
			std::string sAttributeName = customAttribute.first.second;
			std::string sAttributeValue = customAttribute.second;
			std::string sAttributePrefix = findNameSpacePrefix(sNameSpace);
			m_pXmlWriter->WriteAttributeString(sAttributePrefix.c_str(), sAttributeName.c_str(), nullptr, sAttributeValue.c_str());
		}

		if (pStreamWriter != nullptr) {
			unsigned int binaryKeyX = pStreamWriter->addIntArray(pXBuffer, nPointCount, eptDeltaPredicition);
			unsigned int binaryKeyY = pStreamWriter->addIntArray(pYBuffer, nPointCount, eptDeltaPredicition);

			std::string sKeyX = std::to_string(binaryKeyX);
			std::string sKeyY = std::to_string(binaryKeyY);

			m_pXmlWriter->WriteStartElement(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHELEMENT_POINT, nullptr);
			m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_X1, nullptr, sKeyX.c_str());
			m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_Y1, nullptr, sKeyY.c_str());
			m_pXmlWriter->WriteEndElement();

		}
		else {

		
			uint32_t nIndex;
			for (nIndex = 0; nIndex < nPointCount; nIndex++) {
				std::string sX = std::to_string(pXBuffer[nIndex]);
				std::string sY = std::to_string(pYBuffer[nIndex]);

				m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_POINT, nullptr);
				m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_X, nullptr, sX.c_str());
				m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_Y, nullptr, sY.c_str());

				if (pTagDataBuffer != nullptr) {

					int32_t nTagValue = pTagDataBuffer[nIndex];
					if (nTagValue != 0) {
						std::string sTagValue = std::to_string(nTagValue);

						m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_TAG, nullptr, sTagValue.c_str());
					}
				}

				if (pScalingDataBuffer != nullptr) {

					int32_t nScalingValue = pScalingDataBuffer[nIndex];
					if (nScalingValue != 0) {
						std::string sScalingValue = std::to_string(nScalingValue);

						m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF, nullptr, sScalingValue.c_str());
					}
				}

				m_pXmlWriter->WriteEndElement();
			}

		}

		m_pXmlWriter->WriteFullEndElement();

	}

	void CModelToolpathLayerWriteData::WritePolyline(const nfUint32 nProfileID, const nfUint32 nPartID, const nfUint32 nPointCount, const nfInt32 * pXBuffer, const nfInt32 * pYBuffer, const int32_t* pTagDataBuffer, const int32_t* pScalingDataBuffer)
	{
		std::string sPath;

		if (pXBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pYBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		NMR::CChunkedBinaryStreamWriter * pStreamWriter = getStreamWriter(sPath);
		if (m_bWritingHeader)
			finishHeader();
		if (!m_bWritingData)
			throw CNMRException(NMR_ERROR_TOOLPATH_NOTWRITINGDATA);

		std::string sPartID = std::to_string(nPartID);
		std::string sProfileID = std::to_string(nProfileID);

		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_SEGMENT, nullptr);
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_TYPE, nullptr, XML_3MF_TOOLPATHTYPE_POLYLINE);
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PROFILEID, nullptr, sProfileID.c_str());
		m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_PARTID, nullptr, sPartID.c_str());
		if (m_nCurrentLaserIndex != 0) {
			std::string sLaserIndex = std::to_string(m_nCurrentLaserIndex);
			m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_LASERINDEX, nullptr, sLaserIndex.c_str());
		}

		if (pScalingDataBuffer != nullptr) {
			std::string sFactorRange = std::to_string(m_nFactorRange);
			m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_MAXFACTOR, nullptr, sFactorRange.c_str());
		}

		for (auto& customAttribute : m_CustomSegmentAttributes) {
			std::string sNameSpace = customAttribute.first.first;
			std::string sAttributeName = customAttribute.first.second;
			std::string sAttributeValue = customAttribute.second;
			std::string sAttributePrefix = findNameSpacePrefix(sNameSpace);
			m_pXmlWriter->WriteAttributeString(sAttributePrefix.c_str(), sAttributeName.c_str(), nullptr, sAttributeValue.c_str());
		}

		if (pStreamWriter != nullptr) {

			unsigned int binaryKeyX = pStreamWriter->addIntArray(pXBuffer, nPointCount, eptDeltaPredicition);
			unsigned int binaryKeyY = pStreamWriter->addIntArray(pYBuffer, nPointCount, eptDeltaPredicition);

			std::string sKeyX = std::to_string(binaryKeyX);
			std::string sKeyY = std::to_string(binaryKeyY);

			m_pXmlWriter->WriteStartElement(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHELEMENT_POINT, nullptr);
			m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_X1, nullptr, sKeyX.c_str());
			m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_Y1, nullptr, sKeyY.c_str());

			m_pXmlWriter->WriteEndElement();

		}
		else { 

			uint32_t nIndex;
			for (nIndex = 0; nIndex < nPointCount; nIndex++) {
				std::string sX = std::to_string(pXBuffer[nIndex]);
				std::string sY = std::to_string(pYBuffer[nIndex]);

				m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_POINT, nullptr);
				m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_X, nullptr, sX.c_str());
				m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_Y, nullptr, sY.c_str());

				if (pScalingDataBuffer != nullptr) {

					int32_t nScalingValue = pScalingDataBuffer[nIndex];
					std::string sScalingValue = std::to_string(nScalingValue);

					m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF, nullptr, sScalingValue.c_str());
				}

				m_pXmlWriter->WriteEndElement();
			}

		}

		m_pXmlWriter->WriteFullEndElement();

	}

	void CModelToolpathLayerWriteData::writeCustomXMLDataNode(PCustomXMLNode pXMLNode, const std::string& sNameSpace)
	{
		if (pXMLNode.get () == nullptr)
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);

		std::string sPrefix = findNameSpacePrefix(sNameSpace);
		std::string sNodeName = pXMLNode->getName ();
		m_pXmlWriter->WriteStartElement(sPrefix.c_str (), sNodeName.c_str (), nullptr);

		size_t nAttributeCount = pXMLNode->getAttributeCount();
		for (size_t nAttributeIndex = 0; nAttributeIndex < nAttributeCount; nAttributeIndex++) {
			auto pAttribute = pXMLNode->getAttributeByIndex(nAttributeIndex);
			std::string sName = pAttribute->getName();
			std::string sValue = pAttribute->getValue();
			m_pXmlWriter->WriteAttributeString(nullptr, sName.c_str(), nullptr, sValue.c_str());			
		}

		auto children = pXMLNode->getChildren();
	
		for (auto child: children) {
			writeCustomXMLDataNode(child, sNameSpace);
		}

		m_pXmlWriter->WriteEndElement();
	}

	void CModelToolpathLayerWriteData::finishHeader()
	{
		std::string sPath;

		NMR::CChunkedBinaryStreamWriter * pStreamWriter = getStreamWriter(sPath);

		if (!m_bWritingHeader)
			throw CNMRException(NMR_ERROR_TOOLPATH_NOTWRITINGHEADER);

		m_bWritingHeader = false;

		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_PARTS, nullptr);
		for (auto iPart : m_Parts) {
			bool bHasUUID = false;

			m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_PART, nullptr);
			std::string sID = std::to_string(iPart.first);
			std::string sUUID = iPart.second->uuid()->toString();

			m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_ID, nullptr, sID.c_str());
			m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_UUID, nullptr, sUUID.c_str());
			m_pXmlWriter->WriteEndElement();
		}

		m_pXmlWriter->WriteFullEndElement();

		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_PROFILES, nullptr);
		for (auto iProfile : m_Profiles) {
			m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_PROFILE, nullptr);
			std::string sID = std::to_string(iProfile.first);
			std::string sUUID = iProfile.second->getUUID();

			m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_ID, nullptr, sID.c_str());
			m_pXmlWriter->WriteAttributeString(nullptr, XML_3MF_TOOLPATHATTRIBUTE_UUID, nullptr, sUUID.c_str());
			m_pXmlWriter->WriteEndElement();
		}

		m_pXmlWriter->WriteFullEndElement();


		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_DATA, nullptr);
		for (auto customXMLData : m_CustomXMLData) {

			writeCustomXMLDataNode (customXMLData->getRootNode(), customXMLData->getNameSpace ());

		}

		m_pXmlWriter->WriteFullEndElement();


		m_pXmlWriter->WriteStartElement(nullptr, XML_3MF_TOOLPATHELEMENT_SEGMENTS, nullptr);

		if (pStreamWriter != nullptr)
			m_pXmlWriter->WriteAttributeString(XML_3MF_NAMESPACEPREFIX_BINARY, XML_3MF_TOOLPATHATTRIBUTE_BINARY, nullptr, sPath.c_str());

		m_bWritingData = true;


	}


	NMR::PImportStream CModelToolpathLayerWriteData::createStream()
	{
		if (m_bWritingHeader)
			finishHeader();
		if (!m_bWritingData)
			throw CNMRException(NMR_ERROR_TOOLPATH_NOTWRITINGDATA);

		m_bWritingData = false;
		if (m_bWritingFinished)
			throw CNMRException(NMR_ERROR_TOOLPATH_DATAHASBEENWRITTEN);

		m_bWritingFinished = true;

		m_pXmlWriter->WriteFullEndElement(); // segments

		m_pXmlWriter->WriteFullEndElement(); // layer
		m_pXmlWriter->WriteEndDocument();
		m_pXmlWriter->Flush();

		// TODO: Do not copy but use Pipe-based importexportstream!
		NMR::CImportStream_Shared_Memory pImportStream(m_pExportStream->getData(), m_pExportStream->getDataSize());
		return pImportStream.copyToMemory();

	}

	NMR::CChunkedBinaryStreamWriter * CModelToolpathLayerWriteData::getStreamWriter(std::string & sPath)
	{
	
		return m_pModelWriter->findBinaryStream (m_sUUID, sPath);

	}

	double CModelToolpathLayerWriteData::getUnits()
	{
		return m_pModelToolpath->getUnitFactor();
	}

	std::string CModelToolpathLayerWriteData::getUUID()
	{
		return m_sUUID;
	}

	void CModelToolpathLayerWriteData::finishWriting()
	{
		if (!m_bWritingFinished) {
			PImportStream pImportStream = createStream();
			m_pModelWriter->addAdditionalAttachment (m_sPackagePath, pImportStream, PACKAGE_TOOLPATH_RELATIONSHIP_TYPE);

		}
	}

	void CModelToolpathLayerWriteData::setCustomSegmentAttribute(const std::string& sNameSpace, const std::string& sAttributeName, const std::string& sValue)
	{
		auto key = std::make_pair(sNameSpace, sAttributeName);

		auto iIter = m_CustomSegmentAttributes.find(key);
		if (iIter != m_CustomSegmentAttributes.end()) {
			iIter->second = sValue;
		}
		else {
			m_CustomSegmentAttributes.insert(std::make_pair(key, sValue));
		}
	}

	void CModelToolpathLayerWriteData::clearCustomSegmentAttributes()
	{
		m_CustomSegmentAttributes.clear();
	}

	std::string CModelToolpathLayerWriteData::findNameSpacePrefix(const std::string& sNameSpace)
	{
		auto iIter = m_NameSpaceToPrefixMap.find(sNameSpace);
		if (iIter == m_NameSpaceToPrefixMap.end())
			throw CNMRException(NMR_ERROR_NAMESPACENOTFOUND);

		return iIter->second;
	}

	void CModelToolpathLayerWriteData::setCurrentLaserIndex(uint32_t nLaserIndex)
	{
		m_nCurrentLaserIndex = nLaserIndex;
	}

	void CModelToolpathLayerWriteData::setFactorRange(uint32_t nFactorRange)
	{
		if ((nFactorRange < TOOLPATHWRITER_MINFACTORRANGE) || (nFactorRange > TOOLPATHWRITER_MAXFACTORRANGE))
			throw CNMRException(NMR_ERROR_TOOLPATH_INVALIDFACTORRANGE);

		m_nFactorRange = nFactorRange;
	}
}

