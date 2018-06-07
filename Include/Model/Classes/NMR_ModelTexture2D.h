/*++

Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_ModelTexture2D.h defines the Model Texture 2D Class.

--*/

#ifndef __NMR_MODELTEXTURE2D
#define __NMR_MODELTEXTURE2D

#include "Common/NMR_Types.h" 


#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"  
#include <vector>
#include "Common/Platform/NMR_ImportStream.h"  
#include <memory>
#include <map>
#include <string>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelTexture2DResource : public CModelResource {
	private:
		// PModelAttachment m_pAttachment;
		std::string m_sPath;	// this is how CModelTexture2DResource connects to an attachment
		eModelTexture2DType m_ContentType;
		
		nfBool m_bHasBox2D;
		nfFloat m_fBox2D_U;
		nfFloat m_fBox2D_V;
		nfFloat m_fBox2D_Width;
		nfFloat m_fBox2D_Height;
		eModelTextureTileStyle m_eTileStyleU;
		eModelTextureTileStyle m_eTileStyleV;
	public:
		CModelTexture2DResource() = delete;
		CModelTexture2DResource(_In_ const ModelResourceID sID, _In_ CModel * pModel);

		// getters/setters Path
		std::string getPath();
		void setPath (_In_ std::string sPath);
		
		// getters TextureStream
		PImportStream getTextureStream();

		// getters/setters ContentType
		eModelTexture2DType getContentType();
		void setContentType(_In_ eModelTexture2DType ContentType);
		std::string getContentTypeString();
		void setContentTypeString(_In_ std::string sValue, _In_ nfBool bFailIfUnknown);
		void setTileStyleUString(_In_ std::string sValue);
		void setTileStyleVString(_In_ std::string sValue);
		
		// getters/setters Box2D
		nfBool getBox2D (_Out_ nfFloat & fU, _Out_ nfFloat & fV, _Out_ nfFloat & fWidth, _Out_ nfFloat & fHeight);
		void setBox2D (_In_ nfFloat fU, _In_ nfFloat fV, _In_ nfFloat fWidth, _In_ nfFloat fHeight);
		void clearBox2D ();
		nfBool hasBox2D();
		
		// getters/setters TileStyle
		eModelTextureTileStyle getTileStyleU();
		eModelTextureTileStyle getTileStyleV();
		void setTileStyleU(_In_ eModelTextureTileStyle sStyle);
		void setTileStyleV(_In_ eModelTextureTileStyle sStyle);
		
		// copy all parameters from source
		void copyFrom(_In_ CModelTexture2DResource * pSourceTexture);

		static eModelTextureTileStyle tileStyleFromString(_In_ std::string sValue);
		static std::string tileStyleToString(_In_ eModelTextureTileStyle eTileStyle);
	};

	typedef std::shared_ptr <CModelTexture2DResource> PModelTexture2DResource;

}

#endif // __NMR_MODELTEXTURE2D

