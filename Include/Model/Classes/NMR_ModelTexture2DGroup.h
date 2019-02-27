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

NMR_ModelTexture2DGroup.h defines the Model Texture2D Group Resource Class.
A model color group resource is an in memory representation of the 3MF
texture2d group resource object.

--*/

#ifndef __NMR_TEXTURE2DGROUP
#define __NMR_TEXTURE2DGROUP

#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Model/Classes/NMR_Model.h"
#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelTexture2DGroupResource : public CModelResource {
	private:
		PModelTexture2DResource m_pTexture2D;
		std::map<ModelPropertyID, MODELTEXTURE2DCOORDINATE> m_pUVCoordinates;
		ModelPropertyID m_nNextPropertyID;

	public:
		CModelTexture2DGroupResource() = delete;
		CModelTexture2DGroupResource(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ PModelTexture2DResource pTexture2D);

		nfUint32 addUVCoordinate(_In_ MODELTEXTURE2DCOORDINATE UV);

		nfUint32 getCount();
		void removePropertyID(_In_ ModelPropertyID nPropertyID);
		void setUVCoordinate(_In_ ModelPropertyID nPropertyID, _In_ MODELTEXTURE2DCOORDINATE sCoordinate);

		MODELTEXTURE2DCOORDINATE getUVCoordinate(_In_ ModelPropertyID nPropertyID);

		void mergeFrom(_In_ CModelTexture2DGroupResource * pSourceMaterial);
		void buildResourceIndexMap();

		PModelTexture2DResource getTexture2D();
	};

	typedef std::shared_ptr <CModelTexture2DGroupResource> PModelTexture2DGroupResource;
}

#endif // __NMR_TEXTURE2DGROUP
