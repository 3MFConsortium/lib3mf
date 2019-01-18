/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CLib3MFPropertyHandler

*/


#ifndef __LIB3MF_LIB3MFPROPERTYHANDLER
#define __LIB3MF_LIB3MFPROPERTYHANDLER

#include "lib3mf_interfaces.hpp"


// Include custom headers here.


namespace Lib3MF {
namespace Impl {

/*************************************************************************************************************************
 Class declaration of CLib3MFPropertyHandler 
**************************************************************************************************************************/

class CLib3MFPropertyHandler : public virtual ILib3MFPropertyHandler {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/


	/**
	* Public member functions to implement.
	*/

	void RemoveProperty (const Lib3MF_uint32 nIndex);

	void RemoveAllProperties ();

	eLib3MFPropertyType GetPropertyType (const Lib3MF_uint32 nIndex);

	void GetBaseMaterial (const Lib3MF_uint32 nIndex, Lib3MF_uint32 & nMaterialGroupId, Lib3MF_uint32 & nMaterialIndex);

	void SetBaseMaterial (const Lib3MF_uint32 nIndex, const Lib3MF_uint32 nMaterialGroupId, const Lib3MF_uint32 nMaterialIndex);

	void SetBaseMaterialArray (const Lib3MF_uint32 nMaterialGroupIdsBufferSize, const Lib3MF_uint32 * pMaterialGroupIdsBuffer, const Lib3MF_uint32 nMaterialIndicesBufferSize, const Lib3MF_uint32 * pMaterialIndicesBuffer);

	void GetBaseMaterialArray (Lib3MF_uint32 nMaterialGroupIdsBufferSize, Lib3MF_uint32* pMaterialGroupIdsNeededCount, Lib3MF_uint32 * pMaterialGroupIdsBuffer, Lib3MF_uint32 nMaterialIndicesBufferSize, Lib3MF_uint32* pMaterialIndicesNeededCount, Lib3MF_uint32 * pMaterialIndicesBuffer);

	void SetSingleColor (const Lib3MF_uint32 nIndex, const sLib3MFColorRGBA Color);

	void SetSingleColorRGB (const Lib3MF_uint32 nIndex, const Lib3MF_uint8 nRed, const Lib3MF_uint8 nGreen, const Lib3MF_uint8 nBlue);

	void SetSingleColorRGBA (const Lib3MF_uint32 nIndex, const Lib3MF_uint8 nRed, const Lib3MF_uint8 nGreen, const Lib3MF_uint8 nBlue, const Lib3MF_uint8 nAlpha);

	void SetSingleColorFloatRGB (const Lib3MF_uint32 nIndex, const float fRed, const float fGreen, const float fBlue);

	void SetSingleColorFloatRGBA (const Lib3MF_uint32 nIndex, const float fRed, const float fGreen, const float fBlue, const float fAlpha);

};

}
}

#endif // __LIB3MF_LIB3MFPROPERTYHANDLER
