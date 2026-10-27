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

NMR_ModelReader_Toolpath1905_LaserSources.h reads <tp:lasersources>, <tp:lasersource> and <tp:syncgroup>.
Violations of the 4.5 constraints are reported as warnings, so they only fail the read in strict mode.
The offending part is dropped, so that the model stays consistent.

--*/

#ifndef __NMR_MODELREADERNODE_TOOLPATH1905_LASERSOURCES
#define __NMR_MODELREADERNODE_TOOLPATH1905_LASERSOURCES

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelToolpath.h"

#include <vector>

namespace NMR {

	class CModelReaderNode_Toolpath1905_LaserSource : public CModelReaderNode {
	private:
		nfBool m_bHasIndex;
		nfUint32 m_nIndex;
		nfBool m_bHasName;
		std::string m_sName;
		nfBool m_bHasInvalidIndex;

		nfBool m_bHasMinX, m_bHasMaxX, m_bHasMinY, m_bHasMaxY;
		nfInt32 m_nMinX, m_nMaxX, m_nMinY, m_nMaxY;
		nfBool m_bHasCenterX, m_bHasCenterY;
		nfInt32 m_nCenterX, m_nCenterY;
		nfBool m_bHasCornerRadius;
		nfUint32 m_nCornerRadius;

		void readCoordinate(const nfChar * pAttributeName, const nfChar * pAttributeValue, nfBool & bHasValue, nfInt32 & nValue);

	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);

	public:
		CModelReaderNode_Toolpath1905_LaserSource() = delete;
		CModelReaderNode_Toolpath1905_LaserSource(_In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		void createLaserSource(_In_ CModelToolpath * pToolpath);
	};

	typedef std::shared_ptr <CModelReaderNode_Toolpath1905_LaserSource> PModelReaderNode_Toolpath1905_LaserSource;


	class CModelReaderNode_Toolpath1905_SyncGroup : public CModelReaderNode {
	private:
		nfBool m_bHasID;
		nfUint32 m_nID;
		nfBool m_bHasLasers;
		std::vector<nfUint32> m_LaserIndices;
		nfBool m_bHasInvalidAttribute;

	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);

	public:
		CModelReaderNode_Toolpath1905_SyncGroup() = delete;
		CModelReaderNode_Toolpath1905_SyncGroup(_In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		// Must be called after all laser sources have been created.
		void createSyncGroup(_In_ CModelToolpath * pToolpath);
	};

	typedef std::shared_ptr <CModelReaderNode_Toolpath1905_SyncGroup> PModelReaderNode_Toolpath1905_SyncGroup;


	class CModelReaderNode_Toolpath1905_LaserSources : public CModelReaderNode {
	private:
		CModel * m_pModel;
		CModelToolpath * m_pToolpath;

		nfBool m_bHasDefault;
		nfUint32 m_nDefault;
		nfUint32 m_nLaserSourceElementCount;
		std::vector<PModelReaderNode_Toolpath1905_SyncGroup> m_SyncGroupNodes;

	protected:
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);

	public:
		CModelReaderNode_Toolpath1905_LaserSources() = delete;
		CModelReaderNode_Toolpath1905_LaserSources(_In_ CModel * pModel, _In_ CModelToolpath * pToolpath, _In_ PModelWarnings pWarnings);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);
	};

	typedef std::shared_ptr <CModelReaderNode_Toolpath1905_LaserSources> PModelReaderNode_Toolpath1905_LaserSources;

}

#endif // __NMR_MODELREADERNODE_TOOLPATH1905_LASERSOURCES
