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

NMR_ModelImageStack.h defines a 3D image stack for the volumetric extension

--*/

#ifndef __NMR_MODELIMAGE3D
#define __NMR_MODELIMAGE3D

#include "Common/NMR_Types.h" 


#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"
#include <vector>
#include "Common/Platform/NMR_ImportStream.h"
#include <memory>
#include <map>
#include <string>

#define MAX_IMAGE3D_SIZE (1024 * 1024 * 1024)

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelImage3D : public CModelResource {
	private:
		std::string m_sName;
		nfUint32 m_nSizeX;
		nfUint32 m_nSizeY;
		nfUint32 m_nSheetCount;
		
		std::vector <PModelAttachment> m_Sheets;
		std::vector <nfDouble> m_minValues; // Array with the same size of m_Sheets containing the min double value of the source data, which has been converted to integer bitmap pixels of the attachment picture
		std::vector <nfDouble> m_maxValues; // Array of max values of the source data of each sheet
	
	protected:
		CModelImage3D(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ nfUint32 nSizeX, _In_ nfUint32 nSizeY, nfUint32 nSheetCount);

	public:
		CModelImage3D() = delete;
		
		static PModelImage3D make(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ nfUint32 nSizeX, _In_ nfUint32 nSizeY, nfUint32 nSheetCount);
		
		nfUint32 getSizeX ();
		nfUint32 getSizeY ();
		nfUint32 getSheetCount ();
		
		void setSheet (nfUint32 nSheetIndex, PModelAttachment pAttachment, const nfDouble dMin, const nfDouble dMax);
		PModelAttachment getSheet (nfUint32 nSheetIndex);		
		PModelAttachment createSheet(nfUint32 nSheetIndex, const std::string & sPath, PImportStream pCopiedStream, const nfDouble dMin, const nfDouble dMax);
		nfDouble getSheetMinValue (nfUint32 nSheetIndex);
		void setSheetMinValue (nfUint32 nSheetIndex, nfDouble minVal);
		nfDouble getSheetMaxValue (nfUint32 nSheetIndex);
		void setSheetMaxValue (nfUint32 nSheetIndex, nfDouble maxVal);

	};

	typedef std::shared_ptr <CModelImage3D> PModelImage3D;

}

#endif // __NMR_MODELIMAGE3D

