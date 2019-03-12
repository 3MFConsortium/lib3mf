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

NMR_Matrix.h defines all needed matrix calculations and transforms for
2D and 3D vectors.

--*/

#ifndef __NMR_MATRIX
#define __NMR_MATRIX

#include "Common/Math/NMR_Geometry.h" 
#include <string>

#define NMR_MATRIX_MINDETERMINANT 0.00001f
#define NMR_MATRIX_IDENTITYTHRESHOLD 0.0000001

namespace NMR {

	NMATRIX2 fnMATRIX2_identity();
	NMATRIX2 fnMATRIX2_rotation(_In_ const nfFloat fAngle);
	NMATRIX2 fnMATRIX2_translation(_In_ const NVEC2 vTranslation);
	NMATRIX2 fnMATRIX2_uniformscale(_In_ nfFloat fScale);
	NMATRIX2 fnMATRIX2_scale(_In_ nfFloat fScaleX, _In_ nfFloat fScaleY);
	NMATRIX2 fnMATRIX2_transformation(_In_ const nfFloat fAngle, _In_ const NVEC2 vTranslation);
	NMATRIX2 fnMATRIX2_multiply(_In_ const NMATRIX2 mMatrix1, _In_  const NMATRIX2 mMatrix2);
	NVEC2 fnMATRIX2_apply(_In_ const NMATRIX2 mMatrix, _In_ const NVEC2 vVector);

	NLINMATRIX2 fnLINMATRIX2_identity();
	NLINMATRIX2 fnLINMATRIX2_rotation(_In_ const nfFloat fAngle);
	NLINMATRIX2 fnLINMATRIX2_uniformscale(_In_ nfFloat fScale);
	NLINMATRIX2 fnLINMATRIX2_scale(_In_ nfFloat fScaleX, _In_ nfFloat fScaleY);
	NLINMATRIX2 fnLINMATRIX2_multiply(_In_ const NLINMATRIX2 mMatrix1, _In_  const NLINMATRIX2 mMatrix2);
	NVEC2 fnLINMATRIX2_apply(_In_ const NLINMATRIX2 mMatrix, _In_ const NVEC2 vVector);
	NLINMATRIX2 fnLINMATRIX2_invert(_In_ const NLINMATRIX2 mMatrix, _In_ nfBool bFailIfSingular);

	NMATRIX3 fnMATRIX3_identity();
	NMATRIX3 fnMATRIX3_rotation(_In_ const NVEC3 vAxis, _In_ const nfFloat fAngle);
	NMATRIX3 fnMATRIX3_translation(_In_ const NVEC3 vTranslation);
	NMATRIX3 fnMATRIX3_transformation(_In_ const NVEC3 vAxis, _In_ const nfFloat fAngle, _In_ const NVEC3 vTranslation);
	NMATRIX3 fnMATRIX3_multiply(_In_ const NMATRIX3 mMatrix1, _In_ const NMATRIX3 mMatrix2);
	NMATRIX3 fnMATRIX3_uniformscale(_In_ nfFloat fScale);
	NMATRIX3 fnMATRIX3_scale(_In_ nfFloat fScaleX, _In_ nfFloat fScaleY, _In_ nfFloat fScaleZ);
	NVEC3 fnMATRIX3_apply(_In_ const NMATRIX3 mMatrix, _In_ const NVEC3 vVector);
	nfBool fnMATRIX3_isIdentity(_In_ const NMATRIX3 mMatrix);
	std::string fnMATRIX3_toString(_In_ const NMATRIX3 mMatrix);
	NMATRIX3 fnMATRIX3_fromString(_In_ const std::string sString);

	nfBool fnMATRIX3_isplanar(_In_ const NMATRIX3 mMatrix);
}

#endif // __NMR_MATRIX
