/*++

Copyright (C) 2023 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CFunctionFromImage3D

*/


#ifndef __LIB3MF_FUNCTIONFROMIMAGE3D
#define __LIB3MF_FUNCTIONFROMIMAGE3D

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_function.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelFunctionFromImage3D.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CFunctionFromImage3D 
**************************************************************************************************************************/

class CFunctionFromImage3D : public virtual IFunctionFromImage3D, public virtual CFunction {
private:

	/**
	* Put private members here.
	*/
	NMR::CModelFunctionFromImage3D* functionfromimage3d();

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CFunctionFromImage3D() = delete;
	CFunctionFromImage3D(NMR::PModelResource pResource);

	/**
	* Public member functions to implement.
	*/

	/**
	* IFunctionFromImage3D::GetImage3D - Returns the selected 3D image.
	* @return image instance
	*/
	IImage3D * GetImage3D() override;

	/**
	* IFunctionFromImage3D::SetImage3D - Sets the 3D image of the selector.
	* @param[in] pImage3D - image instance
	*/
	void SetImage3D(IImage3D* pImage3D) override;

	/**
	* IFunctionFromImage3D::SetFilter - Sets the texture filter of the selector.
	* @param[in] eFilter - texture filter
	*/
	void SetFilter(const Lib3MF::eTextureFilter eFilter) override;

	/**
	* IFunctionFromImage3D::GetFilter - Returns the texture filter of the selector.
	* @return texture filter
	*/
	Lib3MF::eTextureFilter GetFilter() override;

	/**
	* IFunctionFromImage3D::SetTileStyles - Sets the tile styles of the selector.
	* @param[in] eTileStyleU - tile style in U
	* @param[in] eTileStyleV - tile style in V
	* @param[in] eTileStyleW - tile style in W
	*/
	void SetTileStyles(const Lib3MF::eTextureTileStyle eTileStyleU, const Lib3MF::eTextureTileStyle eTileStyleV, const Lib3MF::eTextureTileStyle eTileStyleW) override;

	/**
	* IFunctionFromImage3D::GetTileStyles - Retrieves the tile styles of the selector.
	* @param[out] eTileStyleU - tile style in U
	* @param[out] eTileStyleV - tile style in V
	* @param[out] eTileStyleW - tile style in W
	*/
	void GetTileStyles(Lib3MF::eTextureTileStyle & eTileStyleU, Lib3MF::eTextureTileStyle & eTileStyleV, Lib3MF::eTextureTileStyle & eTileStyleW) override;

	/**
	* IFunctionFromImage3D::GetOffset - returns the offset value for the pixel values in the Image3D
	* @return the offset value for the pixel values in the Image3D
	*/
	Lib3MF_double GetOffset() override;

	/**
	* IFunctionFromImage3D::SetOffset - Sets the offset value for the pixel values in the Image3D
	* @param[in] dOffset - the offset value for the pixel values in the Image3D
	*/
	void SetOffset(const Lib3MF_double dOffset) override;

	/**
	* IFunctionFromImage3D::GetScale - returns the scale value for the pixel values in the Image3D
	* @return the scale value for the pixel values in the Image3D
	*/
	Lib3MF_double GetScale() override;

	/**
	* IFunctionFromImage3D::SetScale - Sets the scale value for the pixel values in the Image3D
	* @param[in] dScale - the scale value for the pixel values in the Image3D
	*/
	void SetScale(const Lib3MF_double dScale) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_FUNCTIONFROMIMAGE3D
