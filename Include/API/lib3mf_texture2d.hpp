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

Abstract: This is the class declaration of CTexture2D

*/


#ifndef __LIB3MF_TEXTURE2D
#define __LIB3MF_TEXTURE2D

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_resource.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelTexture2D.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CTexture2D 
**************************************************************************************************************************/

class CTexture2D : public virtual ITexture2D, public virtual CResource {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/
	NMR::PModelTexture2DResource texture();


public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CTexture2D(NMR::PModelTexture2DResource pResource);

	/**
	* Public member functions to implement.
	*/

	IAttachment * GetAttachment ();

	void SetAttachment (IAttachment* pAttachment);

	eLib3MFTextureType GetContentType ();

	void SetContentType (const eLib3MFTextureType eContentType);

	void GetTileStyleUV (eLib3MFTextureTileStyle & eTileStyleU, eLib3MFTextureTileStyle & eTileStyleV);

	void SetTileStyleUV (const eLib3MFTextureTileStyle eTileStyleU, const eLib3MFTextureTileStyle eTileStyleV);

	eLib3MFTextureFilter GetFilter ();

	void SetFilter (const eLib3MFTextureFilter eFilter);

};

}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_TEXTURE2D
