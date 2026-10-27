/*++

Copyright (C) 2026 3MF Consortium

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

NMR_ModelReader_Toolpath1905_LaserSources.cpp reads <tp:lasersources>, <tp:lasersource> and <tp:syncgroup>.

--*/

#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_LaserSources.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include <cstdint>

namespace NMR {

	namespace {

		const char * const XMLWHITESPACE = " \t\r\n";

		// Strict xs:integer parsing: optional sign and decimal digits, surrounding whitespace allowed.
		bool parseInteger(const std::string & sValue, nfInt64 nMinimum, nfInt64 nMaximum, nfInt64 & nResult)
		{
			size_t nStart = sValue.find_first_not_of(XMLWHITESPACE);
			if (nStart == std::string::npos)
				return false;
			size_t nEnd = sValue.find_last_not_of(XMLWHITESPACE);

			bool bNegative = false;
			size_t nPosition = nStart;
			if ((sValue[nPosition] == '+') || (sValue[nPosition] == '-')) {
				bNegative = (sValue[nPosition] == '-');
				nPosition++;
			}
			if (nPosition > nEnd)
				return false;

			nfInt64 nValue = 0;
			for (; nPosition <= nEnd; nPosition++) {
				char cChar = sValue[nPosition];
				if ((cChar < '0') || (cChar > '9'))
					return false;
				nValue = nValue * 10 + (cChar - '0');
				if (nValue > 4294967296LL)
					return false;
			}
			if (bNegative)
				nValue = -nValue;

			if ((nValue < nMinimum) || (nValue > nMaximum))
				return false;

			nResult = nValue;
			return true;
		}

		bool parseLaserIndexList(const std::string & sValue, std::vector<nfUint32> & LaserIndices)
		{
			LaserIndices.clear();
			size_t nPosition = sValue.find_first_not_of(XMLWHITESPACE);
			while (nPosition != std::string::npos) {
				size_t nTokenEnd = sValue.find_first_of(XMLWHITESPACE, nPosition);
				std::string sToken = sValue.substr(nPosition, (nTokenEnd == std::string::npos) ? std::string::npos : nTokenEnd - nPosition);

				nfInt64 nLaserIndex = 0;
				if (!parseInteger(sToken, 0, XML_3MF_MAXTOOLPATHLASERINDEX, nLaserIndex))
					return false;
				LaserIndices.push_back((nfUint32)nLaserIndex);

				nPosition = (nTokenEnd == std::string::npos) ? std::string::npos : sValue.find_first_not_of(XMLWHITESPACE, nTokenEnd);
			}

			return !LaserIndices.empty();
		}

	}


	CModelReaderNode_Toolpath1905_LaserSource::CModelReaderNode_Toolpath1905_LaserSource(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings),
		m_bHasIndex(false), m_nIndex(0), m_bHasName(false), m_bHasInvalidIndex(false),
		m_bHasMinX(false), m_bHasMaxX(false), m_bHasMinY(false), m_bHasMaxY(false),
		m_nMinX(0), m_nMaxX(0), m_nMinY(0), m_nMaxY(0),
		m_bHasCenterX(false), m_bHasCenterY(false), m_nCenterX(0), m_nCenterY(0),
		m_bHasCornerRadius(false), m_nCornerRadius(0)
	{
	}

	void CModelReaderNode_Toolpath1905_LaserSource::parseXML(_In_ CXmlReader * pXMLReader)
	{
		parseName(pXMLReader);
		parseAttributes(pXMLReader);
		parseContent(pXMLReader);
	}

	void CModelReaderNode_Toolpath1905_LaserSource::readCoordinate(const nfChar * pAttributeName, const nfChar * pAttributeValue, nfBool & bHasValue, nfInt32 & nValue)
	{
		nfInt64 nParsedValue = 0;
		if (parseInteger(pAttributeValue, INT32_MIN, INT32_MAX, nParsedValue)) {
			nValue = (nfInt32)nParsedValue;
			bHasValue = true;
		}
		else {
			std::string sAttributeName = pAttributeName;
			nfUint32 nErrorCode = ((sAttributeName == XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_CENTERX) || (sAttributeName == XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_CENTERY))
				? NMR_ERROR_INVALIDTOOLPATHLASERSOURCECENTER : NMR_ERROR_INVALIDTOOLPATHLASERSOURCEBOUNDS;
			m_pWarnings->addException(CNMRException(nErrorCode, sAttributeName + "=\"" + pAttributeValue + "\" is not an integer"), mrwInvalidMandatoryValue);
		}
	}

	void CModelReaderNode_Toolpath1905_LaserSource::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_INDEX) == 0) {
			nfInt64 nValue = 0;
			if (parseInteger(pAttributeValue, 0, XML_3MF_MAXTOOLPATHLASERINDEX, nValue)) {
				m_nIndex = (nfUint32)nValue;
				m_bHasIndex = true;
			}
			else {
				m_bHasInvalidIndex = true;
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCE, std::string("index=\"") + pAttributeValue + "\" is not a non-negative integer"), mrwInvalidMandatoryValue);
			}
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_NAME) == 0) {
			m_sName = pAttributeValue;
			m_bHasName = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_MINX) == 0)
			readCoordinate(pAttributeName, pAttributeValue, m_bHasMinX, m_nMinX);
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_MAXX) == 0)
			readCoordinate(pAttributeName, pAttributeValue, m_bHasMaxX, m_nMaxX);
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_MINY) == 0)
			readCoordinate(pAttributeName, pAttributeValue, m_bHasMinY, m_nMinY);
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_MAXY) == 0)
			readCoordinate(pAttributeName, pAttributeValue, m_bHasMaxY, m_nMaxY);
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_CENTERX) == 0)
			readCoordinate(pAttributeName, pAttributeValue, m_bHasCenterX, m_nCenterX);
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_CENTERY) == 0)
			readCoordinate(pAttributeName, pAttributeValue, m_bHasCenterY, m_nCenterY);
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCE_CORNERRADIUS) == 0) {
			nfInt64 nValue = 0;
			if (parseInteger(pAttributeValue, 1, XML_3MF_MAXTOOLPATHLASERINDEX, nValue)) {
				m_nCornerRadius = (nfUint32)nValue;
				m_bHasCornerRadius = true;
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCECORNERRADIUS, std::string("cornerradius=\"") + pAttributeValue + "\" is not a positive integer"), mrwInvalidMandatoryValue);
		}
	}

	void CModelReaderNode_Toolpath1905_LaserSource::createLaserSource(_In_ CModelToolpath * pToolpath)
	{
		__NMRASSERT(pToolpath);

		if (!m_bHasIndex) {
			if (!m_bHasInvalidIndex)
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCE, "missing index"), mrwMissingMandatoryValue);
			return;
		}

		std::string sLaser = "laser index " + fnUint32ToString(m_nIndex);

		if (pToolpath->findLaserSource(m_nIndex).get() != nullptr) {
			m_pWarnings->addException(CNMRException(NMR_ERROR_DUPLICATETOOLPATHLASERSOURCEINDEX, fnUint32ToString(m_nIndex)), mrwInvalidMandatoryValue);
			return;
		}

		if (!m_bHasName)
			m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCE, sLaser + ": missing name"), mrwMissingMandatoryValue);

		PModelToolpathLaserSource pLaserSource = pToolpath->addLaserSource(m_nIndex, m_sName);

		int nBoundsCount = (m_bHasMinX ? 1 : 0) + (m_bHasMaxX ? 1 : 0) + (m_bHasMinY ? 1 : 0) + (m_bHasMaxY ? 1 : 0);
		if (nBoundsCount == 4) {
			if (CModelToolpathLaserSource::boundsAreValid(m_nMinX, m_nMaxX, m_nMinY, m_nMaxY))
				pLaserSource->setBounds(m_nMinX, m_nMaxX, m_nMinY, m_nMaxY);
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCEBOUNDS, sLaser + ": maxx must be >= minx and maxy must be >= miny"), mrwInvalidMandatoryValue);
		}
		else if (nBoundsCount > 0) {
			m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCEBOUNDS, sLaser + ": minx, maxx, miny and maxy must be given together"), mrwInvalidMandatoryValue);
		}

		if (m_bHasCenterX && m_bHasCenterY)
			pLaserSource->setCenter(m_nCenterX, m_nCenterY);
		else if (m_bHasCenterX || m_bHasCenterY)
			m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCECENTER, sLaser + ": centerx and centery must be given together"), mrwInvalidMandatoryValue);

		if (m_bHasCornerRadius) {
			if (!pLaserSource->hasBounds())
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCECORNERRADIUS, sLaser + ": cornerradius requires the scan-field bounds"), mrwInvalidMandatoryValue);
			else if (!CModelToolpathLaserSource::cornerRadiusIsValid(m_nMinX, m_nMaxX, m_nMinY, m_nMaxY, m_nCornerRadius))
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCECORNERRADIUS, sLaser + ": cornerradius must be <= half of the shorter field side"), mrwInvalidMandatoryValue);
			else
				pLaserSource->setCornerRadius(m_nCornerRadius);
		}
	}


	CModelReaderNode_Toolpath1905_SyncGroup::CModelReaderNode_Toolpath1905_SyncGroup(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings), m_bHasID(false), m_nID(0), m_bHasLasers(false), m_bHasInvalidAttribute(false)
	{
	}

	void CModelReaderNode_Toolpath1905_SyncGroup::parseXML(_In_ CXmlReader * pXMLReader)
	{
		parseName(pXMLReader);
		parseAttributes(pXMLReader);
		parseContent(pXMLReader);
	}

	void CModelReaderNode_Toolpath1905_SyncGroup::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHSYNCGROUP_ID) == 0) {
			nfInt64 nValue = 0;
			if (parseInteger(pAttributeValue, 1, XML_3MF_MAXTOOLPATHLASERINDEX, nValue)) {
				m_nID = (nfUint32)nValue;
				m_bHasID = true;
			}
			else {
				m_bHasInvalidAttribute = true;
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHSYNCGROUP, std::string("id=\"") + pAttributeValue + "\" is not a positive integer"), mrwInvalidMandatoryValue);
			}
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHSYNCGROUP_LASERS) == 0) {
			if (parseLaserIndexList(pAttributeValue, m_LaserIndices)) {
				m_bHasLasers = true;
			}
			else {
				m_bHasInvalidAttribute = true;
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHSYNCGROUP, std::string("lasers=\"") + pAttributeValue + "\" is not a list of non-negative integers"), mrwInvalidMandatoryValue);
			}
		}
	}

	void CModelReaderNode_Toolpath1905_SyncGroup::createSyncGroup(_In_ CModelToolpath * pToolpath)
	{
		__NMRASSERT(pToolpath);

		if (!m_bHasID || !m_bHasLasers) {
			if (!m_bHasInvalidAttribute)
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHSYNCGROUP, m_bHasID ? "missing lasers" : "missing id"), mrwMissingMandatoryValue);
			return;
		}

		if (pToolpath->findSyncGroup(m_nID).get() != nullptr) {
			m_pWarnings->addException(CNMRException(NMR_ERROR_DUPLICATETOOLPATHSYNCGROUPID, fnUint32ToString(m_nID)), mrwInvalidMandatoryValue);
			return;
		}

		std::vector<nfUint32> DeclaredLaserIndices;
		for (nfUint32 nLaserIndex : m_LaserIndices) {
			if (pToolpath->findLaserSource(nLaserIndex).get() != nullptr)
				DeclaredLaserIndices.push_back(nLaserIndex);
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_TOOLPATHLASERSOURCENOTFOUND, "sync group " + fnUint32ToString(m_nID) + ": laser index " + fnUint32ToString(nLaserIndex)), mrwInvalidMandatoryValue);
		}

		if (!DeclaredLaserIndices.empty())
			pToolpath->addSyncGroup(m_nID, DeclaredLaserIndices);
	}


	CModelReaderNode_Toolpath1905_LaserSources::CModelReaderNode_Toolpath1905_LaserSources(_In_ CModel * pModel, _In_ CModelToolpath * pToolpath, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings), m_pModel(pModel), m_pToolpath(pToolpath),
		m_bHasDefault(false), m_nDefault(0), m_nLaserSourceElementCount(0)
	{
		if (pToolpath == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	void CModelReaderNode_Toolpath1905_LaserSources::parseXML(_In_ CXmlReader * pXMLReader)
	{
		parseName(pXMLReader);
		parseAttributes(pXMLReader);
		parseContent(pXMLReader);

		if (m_nLaserSourceElementCount == 0)
			m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCE, "lasersources must contain at least one lasersource"), mrwMissingMandatoryValue);

		for (auto pSyncGroupNode : m_SyncGroupNodes)
			pSyncGroupNode->createSyncGroup(m_pToolpath);

		if (m_bHasDefault) {
			if (m_pToolpath->findLaserSource(m_nDefault).get() != nullptr)
				m_pToolpath->setDefaultLaserIndex(m_nDefault);
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_TOOLPATHLASERSOURCENOTFOUND, "default laser index " + fnUint32ToString(m_nDefault)), mrwInvalidMandatoryValue);
		}
	}

	void CModelReaderNode_Toolpath1905_LaserSources::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLASERSOURCES_DEFAULT) == 0) {
			nfInt64 nValue = 0;
			if (parseInteger(pAttributeValue, 0, XML_3MF_MAXTOOLPATHLASERINDEX, nValue)) {
				m_nDefault = (nfUint32)nValue;
				m_bHasDefault = true;
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_TOOLPATHLASERSOURCENOTFOUND, std::string("default=\"") + pAttributeValue + "\" is not a non-negative integer"), mrwInvalidMandatoryValue);
		}
	}

	void CModelReaderNode_Toolpath1905_LaserSources::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (XML_3MF_ISTOOLPATHNAMESPACE(pNameSpace)) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_TOOLPATHLASERSOURCE) == 0) {
				m_nLaserSourceElementCount++;

				PModelReaderNode_Toolpath1905_LaserSource pXMLNode = std::make_shared<CModelReaderNode_Toolpath1905_LaserSource>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
				pXMLNode->createLaserSource(m_pToolpath);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_TOOLPATHSYNCGROUP) == 0) {
				PModelReaderNode_Toolpath1905_SyncGroup pXMLNode = std::make_shared<CModelReaderNode_Toolpath1905_SyncGroup>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
				m_SyncGroupNodes.push_back(pXMLNode);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

}
