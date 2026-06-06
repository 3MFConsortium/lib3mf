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

#ifndef __NMR_TOOLPATHREADERNODE_POINT
#define __NMR_TOOLPATHREADERNODE_POINT

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelToolpathLayerReadData.h"
#include "Common/NMR_ModelWarnings.h"

namespace NMR {

	class CToolpathReaderNode_Point : public CModelReaderNode {
	protected:

		nfInt32 m_nX;
		nfInt32 m_nY;
		nfInt32 m_nTag;
		nfDouble m_dFactorF;
		nfDouble m_dFactorG;
		nfDouble m_dFactorH;
		nfDouble m_dFactorE;
		bool m_bHasFactorF;
		bool m_bHasFactorG;
		bool m_bHasFactorH;
		bool m_bHasFactorE;

		nfBool m_bHasX;
		nfBool m_bHasY;

		CModelToolpathLayerReadData * m_pReadData;

		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace);
	public:
		CToolpathReaderNode_Point() = delete;
		CToolpathReaderNode_Point(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, CModelToolpathLayerReadData * pReadData);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		nfInt32 getX() const;
		nfInt32 getY() const;
		nfInt32 getTag() const;
		nfDouble getFactorF() const;
		nfDouble getFactorG() const;
		nfDouble getFactorH() const;
		nfDouble getFactorE() const;

		bool hasFactorF() const;
		bool hasFactorG() const;
		bool hasFactorH() const;
		bool hasFactorE() const;


	};

	typedef std::shared_ptr <CToolpathReaderNode_Point> PToolpathReaderNode_Point;

}

#endif // __NMR_TOOLPATHREADERNODE_POINT
