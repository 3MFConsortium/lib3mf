/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CVector3DFieldFromImage3D

*/


#ifndef __LIB3MF_VECTOR3DFIELDFROMIMAGE3D
#define __LIB3MF_VECTOR3DFIELDFROMIMAGE3D

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_vector3dfield.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelVector3DFieldFromImage3D.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CVector3DFieldFromImage3D 
**************************************************************************************************************************/

class CVector3DFieldFromImage3D : public virtual IVector3DFieldFromImage3D, public virtual CVector3DField {
private:

	/**
	* Put private members here.
	*/
	NMR::CModelVector3DFieldFromImage3D* vector3DFieldFromImage3D();

protected:

	/**
	* Put protected members here.
	*/

public:
	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CVector3DFieldFromImage3D() = delete;
	CVector3DFieldFromImage3D(NMR::PModelVector3DFieldFromImage3D pVector3DFieldFromImage3D);


	/**
	* Public member functions to implement.
	*/

	IImage3D * GetImage() override;

	void SetImage(IImage3D* pImage3D) override;

	void SetFilter(const Lib3MF::eTextureFilter eFilter) override;

	Lib3MF::eTextureFilter GetFilter() override;

	void SetTileStyles(const Lib3MF::eTextureTileStyle eTileStyleU, const Lib3MF::eTextureTileStyle eTileStyleV, const Lib3MF::eTextureTileStyle eTileStyleW) override;

	void GetTileStyles(Lib3MF::eTextureTileStyle & eTileStyleU, Lib3MF::eTextureTileStyle & eTileStyleV, Lib3MF::eTextureTileStyle & eTileStyleW) override;

	Lib3MF_double GetOffset() override;

	void SetOffset(const Lib3MF_double dOffset) override;

	Lib3MF_double GetScale() override;

	void SetScale(const Lib3MF_double dScale) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_VECTOR3DFIELDFROMIMAGE3D
