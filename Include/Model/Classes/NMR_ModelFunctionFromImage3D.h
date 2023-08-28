/*++

Copyright (C) 2023 3MF Consortium

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

CModelFunctionFromImage3D.h defines a function that can be used inside of an implicit model to access a 3D image.
--*/
#pragma once

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelImplicitFunction.h"
#include "Common/NMR_Types.h"


#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelFunctionFromImage3D : public CModelFunction{
	private:
		UniqueResourceID m_image3DUniqueResourceID{};
		ModelResourceID m_image3DModelResourceID{};
		
		eModelTextureTileStyle m_tileStyleU = {};
		eModelTextureTileStyle m_tileStyleV = {};
		eModelTextureTileStyle m_tileStyleW = {};

		eModelTextureFilter m_filter = {eModelTextureFilter::MODELTEXTUREFILTER_LINEAR};

		double m_offset = {};
		double m_scale = {1.};
	public:
		CModelFunctionFromImage3D() = delete;
		CModelFunctionFromImage3D(const ModelResourceID sID, CModel * pModel);

		void setImage3DUniqueResourceID(const UniqueResourceID sID);
		UniqueResourceID getImage3DUniqueResourceID();

		void setImage3DModelResourceID(const ModelResourceID sID);
		ModelResourceID getImage3DModelResourceID();

		void setTileStyleU(const eModelTextureTileStyle tileStyleU);
		eModelTextureTileStyle getTileStyleU();

		void setTileStyleV(const eModelTextureTileStyle tileStyleV);
		eModelTextureTileStyle getTileStyleV();

		void setTileStyleW(const eModelTextureTileStyle tileStyleW);
		eModelTextureTileStyle getTileStyleW();

		void setFilter(const eModelTextureFilter filter);
		eModelTextureFilter getFilter();

		void setOffset(double offset);
		double getOffset();

		void setScale(double scale);
		double getScale();
	};

	typedef std::shared_ptr <CModelFunctionFromImage3D> PModelFunctionFromImage3D;
}

