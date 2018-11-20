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

NMR_ModelTexureAttachment.h defines the Model Texture Attachment Class.

--*/

#ifndef __NMR_MODELTEXTUREATTACHMENT
#define __NMR_MODELTEXTUREATTACHMENT

#include "Model/Classes/NMR_Model.h" 
#include "Model/Classes/NMR_ModelAttachment.h" 

#include <string>

namespace NMR {

	class CModelTextureAttachment: public CModelAttachment {

	public:
		CModelTextureAttachment() = delete;
		CModelTextureAttachment(_In_ CModel * pModel, _In_ const std::wstring sPathURI, _In_ const std::wstring sRelationShipType, _In_ PImportStream pStream) = delete;
		CModelTextureAttachment(_In_ CModel * pModel, _In_ const std::wstring sPathURI, _In_ PImportStream pStream);
		~CModelTextureAttachment();

		void setRelationShipType(_In_ const std::wstring sRelationShipType) = delete;
	};

	typedef std::shared_ptr <CModelTextureAttachment> PModelTextureAttachment;

}

#endif // __NMR_MODELTEXTUREATTACHMENT
