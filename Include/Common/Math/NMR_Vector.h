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

NMR_Vector.cpp implements all needed direct operations on 2D and 3D vectors.

--*/

#ifndef __NMR_VECTOR
#define __NMR_VECTOR

#include "Common/Math/NMR_Geometry.h" 

namespace NMR {

	NVEC2 fnVEC2_make(_In_ nfFloat fX, _In_ nfFloat fY);
	NVEC2 fnVEC2_add(_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2);
	NVEC2 fnVEC2_sub(_In_ const NVEC2 vMinuend, _In_ const NVEC2 vSubtrahend);
	NVEC2 fnVEC2_scale(_In_ const NVEC2 vVector, _In_ nfFloat vFactor);
	NVEC2 fnVEC2_combine(_In_ const NVEC2 vVector1, _In_ nfFloat vFactor1, _In_ const NVEC2 vVector2, _In_ nfFloat vFactor2);
	nfFloat fnVEC2_dotproduct(_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2);
	nfFloat fnVEC2_crossproduct(_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2);
	nfFloat fnVEC2_length(_In_ const NVEC2 vVector);
	nfFloat fnVEC2_distance(_In_ const NVEC2 vPoint1, _In_ const NVEC2 vPoint2);
	NVEC2 fnVEC2_normalize(_In_ const NVEC2 vVector);

	NVEC3 fnVEC3_make(_In_ nfFloat fX, _In_ nfFloat fY, _In_ nfFloat fZ);
	NVEC3 fnVEC3_add(_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2);
	NVEC3 fnVEC3_sub(_In_ const NVEC3 vMinuend, _In_ const NVEC3 vSubtrahend);
	NVEC3 fnVEC3_scale(_In_ const NVEC3 vVector, _In_ nfFloat vFactor);
	NVEC3 fnVEC3_combine(_In_ const NVEC3 vVector1, _In_ nfFloat vFactor1, _In_ const NVEC3 vVector2, _In_ nfFloat vFactor2);
	nfFloat fnVEC3_dotproduct(_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2);
	NVEC3 fnVEC3_crossproduct(_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2);
	nfFloat fnVEC3_length(_In_ const NVEC3 vVector);
	nfFloat fnVEC3_distance(_In_ const NVEC3 vPoint1, _In_ const NVEC3 vPoint2);
	NVEC3 fnVEC3_normalize(_In_ const NVEC3 vVector);
	NVEC3 fnVEC3_calcTriangleNormal(_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2, _In_ const NVEC3 vVector3);

	NVEC2I fnVEC2I_make(_In_ nfInt32 nX, _In_ nfInt32 nY);
	NVEC2I fnVEC2I_floor(_In_ NVEC2 vVector, _In_ nfFloat fUnits);
	NVEC2 fnVEC2I_uncast(_In_ NVEC2I vVector, _In_ nfFloat fUnits);
	NVEC2I fnVEC2I_add(_In_ const NVEC2I vVector1, _In_ const NVEC2I vVector2);
	NVEC2I fnVEC2I_sub(_In_ const NVEC2I vMinuend, _In_ const NVEC2I vSubtrahend);
	NVEC2I fnVEC2I_scale(_In_ const NVEC2I vVector, _In_ nfInt32 vFactor);
	nfInt64 fnVEC2I_dotproduct(_In_ const NVEC2I vVector1, _In_ const NVEC2I vVector2);
	nfFloat fnVEC2I_distance(_In_ const NVEC2I vPoint1, _In_ const NVEC2I vPoint2);
	nfFloat fnVEC2I_length(_In_ const NVEC2I vVector);

	NVEC3I fnVEC3I_make(_In_ nfInt32 nX, _In_ nfInt32 nY, _In_ nfInt32 nZ);
	NVEC3I fnVEC3I_floor(_In_ NVEC3 vVector, _In_ nfFloat fUnits);
	NVEC3I fnVEC3I_round(_In_ NVEC3 vVector, _In_ nfFloat fUnits);
	NVEC3 fnVEC3I_uncast(_In_ NVEC3I vVector, _In_ nfFloat fUnits);
	NVEC3I fnVEC3I_add(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2);
	NVEC3I fnVEC3I_sub(_In_ const NVEC3I vMinuend, _In_ const NVEC3I vSubtrahend);
	NVEC3I fnVEC3I_scale(_In_ const NVEC3I vVector, _In_ nfInt32 vFactor);
	nfInt64 fnVEC3I_dotproduct(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2);
	NVEC3I fnVEC3I_crossproduct(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2);
	NVEC3I64 fnVEC3I_crossproduct64(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2);
	nfFloat fnVEC3I_length(_In_ const NVEC3I vVector);
	nfFloat fnVEC3I_distance(_In_ const NVEC3I vPoint1, _In_ const NVEC3I vPoint2);
	nfInt32 fnVEC3I_comparelexicographic(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2);
	nfBool fnVEC3I_iszero(_In_ const NVEC3I vVector);
	nfBool fnVEC3I_triangleIsDegenerate(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2, _In_ const NVEC3I vVector3);
	NVEC3I fnVEC3I_setOrderedVector(_In_ nfInt32 nValue1, _In_ nfInt32 nValue2, _In_ nfInt32 nValue3);
	nfBool fnVEC3I_checkForCollinearity(_In_ NVEC3I vVector1, _In_ NVEC3I vVector2);
	NVEC2 operator+ (_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2);
	NVEC2 operator- (_In_ const NVEC2 vMinuend, _In_ const NVEC2 vSubtrahend);
	NVEC2 operator* (_In_ const NVEC2 vVector, _In_ nfFloat vFactor);
	NVEC2 operator* (_In_ nfFloat vFactor, _In_ const NVEC2 vVector);
	nfFloat operator* (_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2);

	NVEC2I operator+ (_In_ const NVEC2I vVector1, _In_ const NVEC2I vVector2);
	NVEC2I operator- (_In_ const NVEC2I vMinuend, _In_ const NVEC2I vSubtrahend);
	NVEC2I operator* (_In_ const NVEC2I vVector, _In_ nfInt32 vFactor);
	NVEC2I operator* (_In_ nfInt32 vFactor, _In_ const NVEC2I vVector);
	nfUint64 operator* (_In_ const NVEC2I vVector1, _In_ const NVEC2I vVector2);

	NVEC3 operator+ (_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2);
	NVEC3 operator- (_In_ const NVEC3 vMinuend, _In_ const NVEC3 vSubtrahend);
	NVEC3 operator* (_In_ const NVEC3 vVector, nfFloat vFactor);
	NVEC3 operator* (nfFloat vFactor, _In_ const NVEC3 vVector);
	nfFloat operator* (_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2);

	NVEC3I operator+ (_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2);
	NVEC3I operator- (_In_ const NVEC3I vMinuend, _In_ const NVEC3I vSubtrahend);
	NVEC3I operator* (_In_ const NVEC3I vVector, _In_ nfInt32 vFactor);
	NVEC3I operator* (_In_ nfInt32 vFactor, _In_ const NVEC3I vVector);
	nfUint64 operator* (_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2);

	nfInt32 fnInt32_sign(_In_ nfInt32 nValue);
	nfInt32 fnInt64_sign(_In_ nfInt64 nValue);
	nfInt32 fnInt32_compare(_In_ nfInt32 nValue1, _In_ nfInt32 nValue2);
	nfInt32 fnInt64_compare(_In_ nfInt64 nValue1, _In_ nfInt64 nValue2);

	void fnOutboxInitialize(_Out_ NOUTBOX3 &oOutbox);
	void fnOutboxMergeVector(_Inout_ NOUTBOX3 &oOutbox, _In_ NVEC3 vVector);
	void fnOutboxMergeOutbox(_Inout_ NOUTBOX3 &oOutboxDest, _In_ NOUTBOX3 &oOutboxSource);
	nfBool fnOutboxIsValid(_In_ const NOUTBOX3 oOutbox);

	void fnOutbox3IMergeVector(_Inout_ NOUTBOX3I &oOutbox, _In_ NVEC3I vVector);
	void fnOutbox3IMergeOutbox(_Inout_ NOUTBOX3I &oOutboxDest, _In_ NOUTBOX3I &oOutboxSource);
	NOUTBOX3I fnCalcTriangleOutbox3I(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2, _In_ const NVEC3I vVector3);
	nfBool fnOutbox3IDoIntersect(_In_ const NOUTBOX3I oOutbox1, _In_ const NOUTBOX3I oOutbox2);

}

#endif // __NMR_VECTOR
