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

Abstract: This is the class declaration of CInternalLib3MFPropertyHandler
Interface version: 1.2.2

*/


#ifndef __LIB3MF_LIB3MFPROPERTYHANDLER
#define __LIB3MF_LIB3MFPROPERTYHANDLER

#include "lib3mf_interfaces.hpp"


// Include custom headers here.


namespace Lib3MF {


/*************************************************************************************************************************
 Class declaration of CInternalLib3MFPropertyHandler 
**************************************************************************************************************************/

class CInternalLib3MFPropertyHandler : public virtual IInternalLib3MFPropertyHandler {
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

	void RemoveProperty (const unsigned int nIndex);

	void RemoveAllProperties ();

	eLib3MFPropertyType GetPropertyType (const unsigned int nIndex);

	void GetBaseMaterial (const unsigned int nIndex, unsigned int & nMaterialGroupId, unsigned int & nMaterialIndex);

	void SetBaseMaterial (const unsigned int nIndex, const unsigned int nMaterialGroupId, const unsigned int nMaterialIndex);

	void SetBaseMaterialArray (const unsigned int nMaterialGroupIdsBufferSize, const unsigned int * pMaterialGroupIdsBuffer, const unsigned int nMaterialIndicesBufferSize, const unsigned int * pMaterialIndicesBuffer);

	void GetBaseMaterialArray (unsigned int nMaterialGroupIdsBufferSize, unsigned int * pMaterialGroupIdsNeededCount, unsigned int * pMaterialGroupIdsBuffer, unsigned int nMaterialIndicesBufferSize, unsigned int * pMaterialIndicesNeededCount, unsigned int * pMaterialIndicesBuffer);

	void SetSingleColor (const unsigned int nIndex, const sLib3MFColorRGBA Color);

	void SetSingleColorRGB (const unsigned int nIndex, const unsigned char nRed, const unsigned char nGreen, const unsigned char nBlue);

	void SetSingleColorRGBA (const unsigned int nIndex, const unsigned char nRed, const unsigned char nGreen, const unsigned char nBlue, const unsigned char nAlpha);

	void SetSingleColorFloatRGB (const unsigned int nIndex, const float fRed, const float fGreen, const float fBlue);

	void SetSingleColorFloatRGBA (const unsigned int nIndex, const float fRed, const float fGreen, const float fBlue, const float fAlpha);

};

}

#endif // __LIB3MF_LIB3MFPROPERTYHANDLER
