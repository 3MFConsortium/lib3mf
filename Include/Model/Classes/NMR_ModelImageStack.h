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

NMR_ModelImageStack.h defines an image stack for the volumetric extension

--*/

#ifndef __NMR_MODELIMAGESTACK
#define __NMR_MODELIMAGESTACK

#include <memory>
#include <map>
#include <string>
#include <vector>
#include "Common/NMR_Types.h" 
#include "Model/Classes/NMR_ModelImage3D.h"
#include "Common/Platform/NMR_ImportStream.h"

constexpr auto MAX_IMAGESTACK_SIZE = (1024 * 1024 * 1024);

namespace NMR {

	class CModelImageStack : public CModelImage3D {
	private:
		nfUint32 m_nRowCount;
		nfUint32 m_nColumnCount;
		nfUint32 m_nSheetCount;
		
		std::vector <PModelAttachment> m_Sheets;
	protected:
		CModelImageStack(_In_ const ModelResourceID sID, _In_ CModel* pModel, _In_ nfUint32 nRowCount, _In_ nfUint32 nColumCount, nfUint32 nSheetCount);

	public:
		CModelImageStack() = delete;
		
		static std::shared_ptr<CModelImageStack> make(_In_ const ModelResourceID sID, _In_ CModel* pModel, _In_ nfUint32 nRowCount, _In_ nfUint32 nColumCount, nfUint32 nSheetCount);
		
		nfUint32 getRowCount() const;
		nfUint32 getColumnCount() const;
		nfUint32 getSheetCount() const;
		void setRowCount(nfUint32 nRowCount);
		void setColumnCount(nfUint32 nColumnCount);
		void setSheetCount(nfUint32 nSheetCount);
		
		void setSheet(nfUint32 nSheetIndex, PModelAttachment pAttachment);
		PModelAttachment getSheet(nfUint32 nSheetIndex);
		PModelAttachment createSheet(nfUint32 nSheetIndex, const std::string & sPath, PImportStream pCopiedStream);
	};

	typedef std::shared_ptr<CModelImageStack> PModelImageStack;

}

#endif // __NMR_MODELIMAGESTACK

