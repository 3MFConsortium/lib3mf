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

		// Factor values. For constant factors, the start and end value are identical.
		// For linear factors (x1/x2), the start value is the first and the end value the second.
		nfDouble m_dFactorF1;
		nfDouble m_dFactorG1;
		nfDouble m_dFactorH1;
		nfDouble m_dFactorE1;
		nfDouble m_dFactorF2;
		nfDouble m_dFactorG2;
		nfDouble m_dFactorH2;
		nfDouble m_dFactorE2;

		// Constant-factor presence (x).
		bool m_bHasConstantFactorF;
		bool m_bHasConstantFactorG;
		bool m_bHasConstantFactorH;
		bool m_bHasConstantFactorE;

		// Linear-factor presence (x1/x2 individually, for completeness validation).
		bool m_bHasFactorF1;
		bool m_bHasFactorF2;
		bool m_bHasFactorG1;
		bool m_bHasFactorG2;
		bool m_bHasFactorH1;
		bool m_bHasFactorH2;
		bool m_bHasFactorE1;
		bool m_bHasFactorE2;

		// Tracks the last parsed <sub> parameter for strictly-increasing validation.
		bool m_bHasOverrideInterpolation;
		nfDouble m_dLastSubParameter;

		nfBool m_bHasX;
		nfBool m_bHasY;

		CModelToolpathLayerReadData * m_pReadData;

		virtual void OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader);
		virtual void OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue);
		virtual void OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace);

		// Validates that every linear factor has both x1 and x2.
		void validateLinearFactors();
	public:
		CToolpathReaderNode_Point() = delete;
		CToolpathReaderNode_Point(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, CModelToolpathLayerReadData * pReadData);

		virtual void parseXML(_In_ CXmlReader * pXMLReader);

		nfInt32 getX() const;
		nfInt32 getY() const;
		nfInt32 getTag() const;

		// Start (first) and end (second) factor values.
		nfDouble getStartFactorF() const;
		nfDouble getStartFactorG() const;
		nfDouble getStartFactorH() const;
		nfDouble getStartFactorE() const;
		nfDouble getEndFactorF() const;
		nfDouble getEndFactorG() const;
		nfDouble getEndFactorH() const;
		nfDouble getEndFactorE() const;

		bool hasFactorF() const;
		bool hasFactorG() const;
		bool hasFactorH() const;
		bool hasFactorE() const;

		bool isLinearFactorF() const;
		bool isLinearFactorG() const;
		bool isLinearFactorH() const;
		bool isLinearFactorE() const;

		bool hasOverrideInterpolation() const;


	};

	typedef std::shared_ptr <CToolpathReaderNode_Point> PToolpathReaderNode_Point;

}

#endif // __NMR_TOOLPATHREADERNODE_POINT
