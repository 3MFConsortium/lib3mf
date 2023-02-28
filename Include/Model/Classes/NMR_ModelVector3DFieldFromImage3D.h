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

NMR_ModelVector3DFieldFromImage3D.h defines the Model Vector3DField Class.
A model scalar field is the base class of different scalar field specializations.
--*/

#ifndef __NMR_MODELVECTOR3DFIELDFROMIMAGE3D
#define __NMR_MODELVECTOR3DFIELDFROMIMAGE3D

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelVector3DField.h"
#include "Common/NMR_Types.h"
#include "Common/Math/NMR_Matrix.h"

#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelVector3DFieldFromImage3D : public CModelVector3DField {
	private:
		PPackageResourceID m_Image3DResourceID;

		nfDouble m_dOffset = 0;
		nfDouble m_dScale = 1;

		eModelTextureFilter m_eFilter = eModelTextureFilter::MODELTEXTUREFILTER_LINEAR;

		eModelTextureTileStyle m_eTileStyleU = eModelTextureTileStyle::MODELTEXTURETILESTYLE_WRAP;
		eModelTextureTileStyle m_eTileStyleV = eModelTextureTileStyle::MODELTEXTURETILESTYLE_WRAP;
		eModelTextureTileStyle m_eTileStyleW = eModelTextureTileStyle::MODELTEXTURETILESTYLE_WRAP;
		
	public:
		CModelVector3DFieldFromImage3D() = delete;
		CModelVector3DFieldFromImage3D(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ PPackageResourceID Image3DID);

		PPackageResourceID getImage3DResourceID() const;
		void setImage3DResourceID(PPackageResourceID ID);

		nfDouble getOffset() const;
		void setOffset(nfDouble);
		nfDouble getScale() const;
		void setScale(nfDouble);

		eModelTextureFilter getFilter() const;
		void setFilter(eModelTextureFilter);

		eModelTextureTileStyle getTileStyleU() const;
		void setTileStyleU(eModelTextureTileStyle);
		eModelTextureTileStyle getTileStyleV() const;
		void setTileStyleV(eModelTextureTileStyle);
		eModelTextureTileStyle getTileStyleW() const;
		void setTileStyleW(eModelTextureTileStyle);
	};

	typedef std::shared_ptr <CModelVector3DFieldFromImage3D> PModelVector3DFieldFromImage3D;
}

#endif // __NMR_MODELVECTOR3DFIELDFROMIMAGE3D
