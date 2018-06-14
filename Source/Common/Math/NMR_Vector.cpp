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

NMR_Vector.cpp implements all needed direct operations on 2D and 3D vectors.

--*/

#include "Common/Math/NMR_Geometry.h" 
#include "Common/Math/NMR_Vector.h" 
#include "Common/NMR_Exception.h" 
#include <cmath>
#include <algorithm>
#include <limits>

namespace NMR {

	NVEC2 fnVEC2_make(_In_ nfFloat fX, _In_ nfFloat fY)
	{
		NVEC2 vResult;
		vResult.m_values.x = fX;
		vResult.m_values.y = fY;
		return vResult;
	}

	NVEC2 fnVEC2_add(_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2)
	{
		nfInt32 j;
		NVEC2 vResult;

		for (j = 0; j < 2; j++)
			vResult.m_fields[j] = vVector1.m_fields[j] + vVector2.m_fields[j];

		return vResult;
	}

	NVEC2 fnVEC2_sub(_In_ const NVEC2 vMinuend, _In_ const NVEC2 vSubtrahend)
	{
		nfInt32 j;
		NVEC2 vResult;

		for (j = 0; j < 2; j++)
			vResult.m_fields[j] = vMinuend.m_fields[j] - vSubtrahend.m_fields[j];

		return vResult;
	}


	NVEC2 fnVEC2_scale(_In_ const NVEC2 vVector, _In_ nfFloat vFactor)
	{
		nfInt32 j;
		NVEC2 vResult;

		for (j = 0; j < 2; j++)
			vResult.m_fields[j] = vVector.m_fields[j] * vFactor;

		return vResult;
	}

	NVEC2 fnVEC2_combine(_In_ const NVEC2 vVector1, _In_ nfFloat vFactor1, _In_ const NVEC2 vVector2, _In_ nfFloat vFactor2)
	{
		nfInt32 j;
		NVEC2 vResult;

		for (j = 0; j < 2; j++)
			vResult.m_fields[j] = vVector1.m_fields[j] * vFactor1 + vVector2.m_fields[j] * vFactor2;

		return vResult;
	}

	nfFloat fnVEC2_dotproduct(_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2)
	{
		return vVector1.m_values.x * vVector2.m_values.x + vVector1.m_values.y * vVector2.m_values.y;
	}

	nfFloat fnVEC2_crossproduct(_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2)
	{
		return vVector1.m_values.x * vVector2.m_values.y - vVector1.m_values.y * vVector2.m_values.x;
	}

	nfFloat fnVEC2_length(_In_ const NVEC2 vVector)
	{
		return sqrtf(vVector.m_values.x * vVector.m_values.x + vVector.m_values.y * vVector.m_values.y);
	}

	nfFloat fnVEC2_distance(_In_ const NVEC2 vPoint1, _In_ const NVEC2 vPoint2)
	{
		return fnVEC2_length(fnVEC2_sub(vPoint1, vPoint2));
	}

	NVEC2 fnVEC2_normalize(_In_ const NVEC2 vVector)
	{
		nfFloat fLength = fnVEC2_length(vVector);
		if (fLength < NMR_VECTOR_MINNORMALIZELENGTH)
			throw CNMRException(NMR_ERROR_NORMALIZEDZEROVECTOR);

		return fnVEC2_scale(vVector, 1.0f / fLength);
	}

	NVEC2I fnVEC2I_make(_In_ nfInt32 nX, _In_ nfInt32 nY)
	{
		NVEC2I vResult;
		vResult.m_values.x = nX;
		vResult.m_values.y = nY;
		return vResult;
	}

	NVEC2I fnVEC2I_floor(_In_ NVEC2 vVector, _In_ nfFloat fUnits)
	{
		if ((fUnits < NMR_VECTOR_MINUNITS) || (fUnits > NMR_VECTOR_MAXUNITS))
			throw CNMRException(NMR_ERROR_INVALIDUNITS);

		NVEC2I vResult;
		vResult.m_values.x = (nfInt32) floor(vVector.m_values.x / fUnits);
		vResult.m_values.y = (nfInt32) floor(vVector.m_values.y / fUnits);
		return vResult;
	}

	NVEC2 fnVEC2I_uncast(_In_ NVEC2I vVector, _In_ nfFloat fUnits)
	{
		if ((fUnits < NMR_VECTOR_MINUNITS) || (fUnits > NMR_VECTOR_MAXUNITS))
			throw CNMRException(NMR_ERROR_INVALIDUNITS);

		NVEC2 vResult;
		vResult.m_values.x = (nfFloat)(vVector.m_values.x) * fUnits;
		vResult.m_values.y = (nfFloat)(vVector.m_values.y) * fUnits;
		return vResult;
	}

	NVEC2I fnVEC2I_add(_In_ const NVEC2I vVector1, _In_ const NVEC2I vVector2)
	{
		nfInt32 j;
		NVEC2I vResult;

		for (j = 0; j < 2; j++)
			vResult.m_fields[j] = vVector1.m_fields[j] + vVector2.m_fields[j];

		return vResult;
	}

	NVEC2I fnVEC2I_sub(_In_ const NVEC2I vMinuend, _In_ const NVEC2I vSubtrahend)
	{
		nfInt32 j;
		NVEC2I vResult;

		for (j = 0; j < 2; j++)
			vResult.m_fields[j] = vMinuend.m_fields[j] - vSubtrahend.m_fields[j];

		return vResult;
	}

	NVEC2I fnVEC2I_scale(_In_ const NVEC2I vVector, _In_ nfInt32 vFactor)
	{
		nfInt32 j;
		NVEC2I vResult;

		for (j = 0; j < 2; j++)
			vResult.m_fields[j] = vVector.m_fields[j] * vFactor;

		return vResult;
	}

	nfInt64 fnVEC2I_dotproduct(_In_ const NVEC2I vVector1, _In_ const NVEC2I vVector2)
	{
		nfInt64 v1x = vVector1.m_values.x;
		nfInt64 v1y = vVector1.m_values.y;
		nfInt64 v2x = vVector2.m_values.x;
		nfInt64 v2y = vVector2.m_values.y;
		return v1x * v2x + v1y * v2y;
	}

	nfFloat fnVEC2I_length(_In_ const NVEC2I vVector)
	{
		nfInt64 vx = vVector.m_values.x;
		nfInt64 vy = vVector.m_values.y;
		nfInt64 dotproduct = vx * vx + vy * vy;
		return sqrtf((nfFloat)dotproduct);
	}

	nfFloat fnVEC2I_distance(_In_ const NVEC2I vPoint1, _In_ const NVEC2I vPoint2)
	{
		return fnVEC2I_length(fnVEC2I_sub(vPoint1, vPoint2));
	}

	NVEC3 fnVEC3_make(_In_ nfFloat fX, _In_ nfFloat fY, _In_ nfFloat fZ)
	{
		NVEC3 vResult;
		vResult.m_values.x = fX;
		vResult.m_values.y = fY;
		vResult.m_values.z = fZ;
		return vResult;
	}

	NVEC3I fnVEC3I_floor(_In_ NVEC3 vVector, _In_ nfFloat fUnits)
	{
		if ((fUnits < NMR_VECTOR_MINUNITS) || (fUnits > NMR_VECTOR_MAXUNITS))
			throw CNMRException(NMR_ERROR_INVALIDUNITS);

		NVEC3I vResult;
		vResult.m_values.x = (nfInt32) floor(vVector.m_values.x / fUnits);
		vResult.m_values.y = (nfInt32) floor(vVector.m_values.y / fUnits);
		vResult.m_values.z = (nfInt32) floor(vVector.m_values.z / fUnits);
		return vResult;
	}

	NVEC3I fnVEC3I_round(_In_ NVEC3 vVector, _In_ nfFloat fUnits)
	{
		if ((fUnits < NMR_VECTOR_MINUNITS) || (fUnits > NMR_VECTOR_MAXUNITS))
			throw CNMRException(NMR_ERROR_INVALIDUNITS);

		NVEC3I vResult;
		vResult.m_values.x = (nfInt32)round(vVector.m_values.x / fUnits);
		vResult.m_values.y = (nfInt32)round(vVector.m_values.y / fUnits);
		vResult.m_values.z = (nfInt32)round(vVector.m_values.z / fUnits);
		return vResult;
	}

	NVEC3 fnVEC3I_uncast(_In_ NVEC3I vVector, _In_ nfFloat fUnits)
	{
		if ((fUnits < NMR_VECTOR_MINUNITS) || (fUnits > NMR_VECTOR_MAXUNITS))
			throw CNMRException(NMR_ERROR_INVALIDUNITS);

		NVEC3 vResult;
		vResult.m_values.x = (nfFloat)(vVector.m_values.x) * fUnits;
		vResult.m_values.y = (nfFloat)(vVector.m_values.y) * fUnits;
		vResult.m_values.z = (nfFloat)(vVector.m_values.z) * fUnits;
		return vResult;
	}

	NVEC3 fnVEC3_add(_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2)
	{
		nfInt32 j;
		NVEC3 vResult;

		for (j = 0; j < 3; j++)
			vResult.m_fields[j] = vVector1.m_fields[j] + vVector2.m_fields[j];

		return vResult;
	}

	NVEC3 fnVEC3_sub(_In_ const NVEC3 vMinuend, _In_ const NVEC3 vSubtrahend)
	{
		nfInt32 j;
		NVEC3 vResult;

		for (j = 0; j < 3; j++)
			vResult.m_fields[j] = vMinuend.m_fields[j] - vSubtrahend.m_fields[j];

		return vResult;
	}

	NVEC3 fnVEC3_scale(_In_ const NVEC3 vVector, _In_ nfFloat vFactor)
	{
		nfInt32 j;
		NVEC3 vResult;

		for (j = 0; j < 3; j++)
			vResult.m_fields[j] = vVector.m_fields[j] * vFactor;

		return vResult;
	}

	NVEC3 fnVEC3_combine(_In_ const NVEC3 vVector1, _In_ nfFloat vFactor1, _In_ const NVEC3 vVector2, _In_ nfFloat vFactor2)
	{
		nfInt32 j;
		NVEC3 vResult;

		for (j = 0; j < 3; j++)
			vResult.m_fields[j] = vVector1.m_fields[j] * vFactor1 + vVector2.m_fields[j] * vFactor2;

		return vResult;
	}

	nfFloat fnVEC3_dotproduct(_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2)
	{
		return vVector1.m_values.x * vVector2.m_values.x +
			vVector1.m_values.y * vVector2.m_values.y +
			vVector1.m_values.z * vVector2.m_values.z;
	}

	NVEC3 fnVEC3_crossproduct(_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2)
	{
		NVEC3 vResult;

		vResult.m_values.x = vVector1.m_values.y * vVector2.m_values.z - vVector1.m_values.z * vVector2.m_values.y;
		vResult.m_values.y = vVector1.m_values.z * vVector2.m_values.x - vVector1.m_values.x * vVector2.m_values.z;
		vResult.m_values.z = vVector1.m_values.x * vVector2.m_values.y - vVector1.m_values.y * vVector2.m_values.x;

		return vResult;
	}

	nfFloat fnVEC3_length(_In_ const NVEC3 vVector)
	{
		return sqrtf(vVector.m_values.x * vVector.m_values.x + vVector.m_values.y * vVector.m_values.y + vVector.m_values.z * vVector.m_values.z);
	}

	nfFloat fnVEC3_distance(_In_ const NVEC3 vPoint1, _In_ const NVEC3 vPoint2)
	{
		return fnVEC3_length(fnVEC3_sub(vPoint1, vPoint2));
	}

	NVEC3 fnVEC3_normalize(_In_ const NVEC3 vVector)
	{
		nfFloat fLength = fnVEC3_length(vVector);
		if (fLength < NMR_VECTOR_MINNORMALIZELENGTH)
			throw CNMRException(NMR_ERROR_NORMALIZEDZEROVECTOR);

		return fnVEC3_scale(vVector, 1.0f / fLength);
	}

	NVEC3 fnVEC3_calcTriangleNormal(_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2, _In_ const NVEC3 vVector3)
	{
		NVEC3 vVectorU = fnVEC3_sub(vVector2, vVector1);
		NVEC3 vVectorV = fnVEC3_sub(vVector3, vVector1);
		NVEC3 vResult = fnVEC3_crossproduct(vVectorU, vVectorV);
		nfFloat fLength = fnVEC3_length(vResult);

		if (fLength > NMR_VECTOR_MINNORMALIZELENGTH)
			return fnVEC3_scale(vResult, 1.0f / fLength);
		else
			return fnVEC3_make(0.0f, 0.0f, 0.0f);
	}

	NVEC3I fnVEC3I_make(_In_ nfInt32 nX, _In_ nfInt32 nY, _In_ nfInt32 nZ)
	{
		NVEC3I vResult;
		vResult.m_values.x = nX;
		vResult.m_values.y = nY;
		vResult.m_values.z = nZ;
		return vResult;
	}

	NVEC3I fnVEC3I_add(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2)
	{
		nfInt32 j;
		NVEC3I vResult;

		for (j = 0; j < 3; j++)
			vResult.m_fields[j] = vVector1.m_fields[j] + vVector2.m_fields[j];

		return vResult;
	}

	NVEC3I fnVEC3I_sub(_In_ const NVEC3I vMinuend, _In_ const NVEC3I vSubtrahend)
	{
		nfInt32 j;
		NVEC3I vResult;

		for (j = 0; j < 3; j++)
			vResult.m_fields[j] = vMinuend.m_fields[j] - vSubtrahend.m_fields[j];

		return vResult;
	}

	NVEC3I fnVEC3I_scale(_In_ const NVEC3I vVector, _In_ nfInt32 vFactor)
	{
		nfInt32 j;
		NVEC3I vResult;

		for (j = 0; j < 3; j++)
			vResult.m_fields[j] = vVector.m_fields[j] * vFactor;

		return vResult;
	}

	nfInt64 fnVEC3I_dotproduct(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2)
	{
		nfInt64 v1x = vVector1.m_values.x;
		nfInt64 v1y = vVector1.m_values.y;
		nfInt64 v1z = vVector1.m_values.z;
		nfInt64 v2x = vVector2.m_values.x;
		nfInt64 v2y = vVector2.m_values.y;
		nfInt64 v2z = vVector2.m_values.z;
		return v1x * v2x + v1y * v2y + v1z * v2z;
	}

	nfFloat fnVEC3I_length(_In_ const NVEC3I vVector)
	{
		nfInt64 vx = vVector.m_values.x;
		nfInt64 vy = vVector.m_values.y;
		nfInt64 vz = vVector.m_values.z;
		nfInt64 dotproduct = vx * vx + vy * vy + vz * vz;
		return sqrtf((nfFloat)dotproduct);
	}

	nfFloat fnVEC3I_distance(_In_ const NVEC3I vPoint1, _In_ const NVEC3I vPoint2)
	{
		return fnVEC3I_length(fnVEC3I_sub(vPoint1, vPoint2));
	}

	nfInt32 fnVEC3I_comparelexicographic(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2)
	{
		nfInt32 nResult = fnInt32_compare(vVector1.m_fields[0], vVector2.m_fields[0]);
		if (nResult == 0) {
			nResult = fnInt32_compare(vVector1.m_fields[1], vVector2.m_fields[1]);
			if (nResult == 0)
				nResult = fnInt32_compare(vVector1.m_fields[2], vVector2.m_fields[2]);
		}
		return nResult;
	}

	nfBool fnVEC3I_iszero(_In_ const NVEC3I vVector)
	{
		return ((vVector.m_fields[0] == 0) && (vVector.m_fields[1] == 0) && (vVector.m_fields[2] == 0));
	}

	NVEC3I fnVEC3I_crossproduct(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2) 
	{
		NVEC3I vResult;
		// Attention! there may be a int32 overflow
		vResult.m_values.x = vVector1.m_values.y * vVector2.m_values.z - vVector1.m_values.z * vVector2.m_values.y;
		vResult.m_values.y = vVector1.m_values.z * vVector2.m_values.x - vVector1.m_values.x * vVector2.m_values.z;
		vResult.m_values.z = vVector1.m_values.x * vVector2.m_values.y - vVector1.m_values.y * vVector2.m_values.x;

		return vResult;
	}

	NVEC3I64 fnVEC3I_crossproduct64(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2)
	{
		NVEC3I64 vResult;
		vResult.m_values.x = nfInt64(vVector1.m_values.y) * nfInt64(vVector2.m_values.z) - nfInt64(vVector1.m_values.z) * nfInt64(vVector2.m_values.y);
		vResult.m_values.y = nfInt64(vVector1.m_values.z) * nfInt64(vVector2.m_values.x) - nfInt64(vVector1.m_values.x) * nfInt64(vVector2.m_values.z);
		vResult.m_values.z = nfInt64(vVector1.m_values.x) * nfInt64(vVector2.m_values.y) - nfInt64(vVector1.m_values.y) * nfInt64(vVector2.m_values.x);

		return vResult;
	}

	nfBool fnVEC3I_triangleIsDegenerate(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2, _In_ const NVEC3I vVector3)
	{
		NVEC3I vU = fnVEC3I_sub(vVector2, vVector1);
		NVEC3I vV = fnVEC3I_sub(vVector3, vVector1);

		NVEC3I64 vNormal = fnVEC3I_crossproduct64(vU, vV);

		return ((vNormal.m_fields[0] == 0) && (vNormal.m_fields[1] == 0) && (vNormal.m_fields[2] == 0));
	}

	NVEC3I fnVEC3I_setOrderedVector(_In_ nfInt32 nValue1, _In_ nfInt32 nValue2, _In_ nfInt32 nValue3)
	{
		NVEC3I vResult;

		if ((nValue1 <= nValue2) && (nValue1 <= nValue3)) {
			vResult.m_fields[0] = nValue1;
			if (nValue2 <= nValue3) {
				vResult.m_fields[1] = nValue2;
				vResult.m_fields[2] = nValue3;
			}
			else {
				vResult.m_fields[1] = nValue3;
				vResult.m_fields[2] = nValue2;
			}
		} 
		else if ((nValue2 <= nValue1) && (nValue2 <= nValue3)) {
			vResult.m_fields[0] = nValue2;
			if (nValue1 <= nValue3) {
				vResult.m_fields[1] = nValue1;
				vResult.m_fields[2] = nValue3;
			}
			else {
				vResult.m_fields[1] = nValue3;
				vResult.m_fields[2] = nValue1;
			}
		}
		else {
			vResult.m_fields[0] = nValue3;
			if (nValue1 <= nValue2) {
				vResult.m_fields[1] = nValue1;
				vResult.m_fields[2] = nValue2;
			}
			else {
				vResult.m_fields[1] = nValue2;
				vResult.m_fields[2] = nValue1;
			}
		}

		return vResult;
	}

	nfBool fnVEC3I_checkForCollinearity(_In_ NVEC3I vVector1, _In_ NVEC3I vVector2)
	{
		if (vVector1.m_fields[1] * vVector2.m_fields[2] != vVector1.m_fields[2] * vVector2.m_fields[1])
			return false;
		if (vVector1.m_fields[2] * vVector2.m_fields[0] != vVector1.m_fields[0] * vVector2.m_fields[2])
			return false;
		if (vVector1.m_fields[0] * vVector2.m_fields[1] != vVector1.m_fields[1] * vVector2.m_fields[0])
			return false;
		// else
		return true;
	}

	NVEC2 operator+ (_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2)
	{
		return fnVEC2_add(vVector1, vVector2);
	}

	NVEC2 operator- (_In_ const NVEC2 vMinuend, _In_ const NVEC2 vSubtrahend)
	{
		return fnVEC2_sub(vMinuend, vSubtrahend);
	}

	NVEC2 operator* (_In_ const NVEC2 vVector, _In_ nfFloat vFactor)
	{
		return fnVEC2_scale(vVector, vFactor);
	}

	NVEC2 operator* (_In_ nfFloat vFactor, _In_ const NVEC2 vVector)
	{
		return fnVEC2_scale(vVector, vFactor);
	}

	nfFloat operator* (_In_ const NVEC2 vVector1, _In_ const NVEC2 vVector2)
	{
		return fnVEC2_dotproduct(vVector1, vVector2);
	}

	NVEC2I operator+ (_In_ const NVEC2I vVector1, _In_ const NVEC2I vVector2)
	{
		return fnVEC2I_add(vVector1, vVector2);
	}

	NVEC2I operator- (_In_ const NVEC2I vMinuend, _In_ const NVEC2I vSubtrahend)
	{
		return fnVEC2I_sub(vMinuend, vSubtrahend);
	}

	NVEC2I operator* (_In_ const NVEC2I vVector, _In_ nfInt32 vFactor)
	{
		return fnVEC2I_scale(vVector, vFactor);
	}

	NVEC2I operator* (_In_ nfInt32 vFactor, _In_ const NVEC2I vVector)
	{
		return fnVEC2I_scale(vVector, vFactor);
	}

	nfUint64 operator* (_In_ const NVEC2I vVector1, _In_ const NVEC2I vVector2)
	{
		return fnVEC2I_dotproduct(vVector1, vVector2);
	}

	NVEC3 operator+ (_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2)
	{
		return fnVEC3_add(vVector1, vVector2);
	}

	NVEC3 operator- (_In_ const NVEC3 vMinuend, _In_ const NVEC3 vSubtrahend)
	{
		return fnVEC3_sub(vMinuend, vSubtrahend);
	}

	NVEC3 operator* (_In_ const NVEC3 vVector, nfFloat vFactor)
	{
		return fnVEC3_scale(vVector, vFactor);
	}

	NVEC3 operator* (nfFloat vFactor, _In_ const NVEC3 vVector)
	{
		return fnVEC3_scale(vVector, vFactor);
	}

	nfFloat operator* (_In_ const NVEC3 vVector1, _In_ const NVEC3 vVector2)
	{
		return fnVEC3_dotproduct(vVector1, vVector2);
	}

	NVEC3I operator+ (_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2)
	{
		return fnVEC3I_add(vVector1, vVector2);
	}

	NVEC3I operator- (_In_ const NVEC3I vMinuend, _In_ const NVEC3I vSubtrahend)
	{
		return fnVEC3I_sub(vMinuend, vSubtrahend);
	}

	NVEC3I operator* (_In_ const NVEC3I vVector, _In_ nfInt32 vFactor)
	{
		return fnVEC3I_scale(vVector, vFactor);
	}

	NVEC3I operator* (_In_ nfInt32 vFactor, _In_ const NVEC3I vVector)
	{
		return fnVEC3I_scale(vVector, vFactor);
	}

	nfUint64 operator* (_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2)
	{
		return fnVEC3I_dotproduct(vVector1, vVector2);
	}

	nfInt32 fnInt32_sign(_In_ nfInt32 nValue)
	{
		return nValue < 0 ? -1 : nValue > 0 ? 1 : 0;
	}

	nfInt32 fnInt64_sign(_In_ nfInt64 nValue) {
		return nValue < 0 ? -1 : nValue > 0 ? 1 : 0;
	}

	nfInt32 fnInt32_compare(_In_ nfInt32 nValue1, _In_ nfInt32 nValue2)
	{
		if (nValue1 > nValue2)
			return 1;
		else if (nValue1 < nValue2)
			return -1;
		else
			return 0;
	}

	nfInt32 fnInt64_compare(_In_ nfInt64 nValue1, _In_ nfInt64 nValue2)
	{
		if (nValue1 > nValue2)
			return 1;
		else if (nValue1 < nValue2)
			return -1;
		else
			return 0;
	}

	void fnOutboxInitialize(_Out_ NOUTBOX3 &oOutbox)
	{
		for (nfInt32 i = 0; i < 3; i++){
			oOutbox.m_max.m_fields[i] = -std::numeric_limits<nfFloat>::max();
			oOutbox.m_min.m_fields[i] = std::numeric_limits<nfFloat>::max();
		}
	}

	void fnOutboxMergeVector(_Inout_ NOUTBOX3 &oOutbox, _In_ NVEC3 vVector)
	{
		for (nfUint32 i = 0; i < 3; i++){
			oOutbox.m_min.m_fields[i] = std::min(oOutbox.m_min.m_fields[i], vVector.m_fields[i]);
			oOutbox.m_max.m_fields[i] = std::max(oOutbox.m_max.m_fields[i], vVector.m_fields[i]);
		}
	}

	void fnOutboxMergeOutbox(_Inout_ NOUTBOX3 &oOutboxDest, _In_ NOUTBOX3 &oOutboxSource)
	{
		fnOutboxMergeVector(oOutboxDest, oOutboxSource.m_max);
		fnOutboxMergeVector(oOutboxDest, oOutboxSource.m_min);
	}

	nfBool fnOutboxIsValid(_In_ const NOUTBOX3 oOutbox)
	{
		return ((fabs(oOutbox.m_min.m_values.x) < NMR_OUTBOX_MAXCOORDINATE) &&
			(fabs(oOutbox.m_min.m_values.y) < NMR_OUTBOX_MAXCOORDINATE) &&
			(fabs(oOutbox.m_min.m_values.z) < NMR_OUTBOX_MAXCOORDINATE) &&
			(fabs(oOutbox.m_max.m_values.x) < NMR_OUTBOX_MAXCOORDINATE) &&
			(fabs(oOutbox.m_max.m_values.y) < NMR_OUTBOX_MAXCOORDINATE) &&
			(fabs(oOutbox.m_max.m_values.z) < NMR_OUTBOX_MAXCOORDINATE));
	}

	void fnOutbox3IMergeVector(_Inout_ NOUTBOX3I &oOutbox, _In_ NVEC3I vVector)
	{
		for (nfUint32 i = 0; i < 3; i++) {
			if (vVector.m_fields[i] < oOutbox.m_vMin.m_fields[i])
				oOutbox.m_vMin.m_fields[i] = vVector.m_fields[i];
			if (vVector.m_fields[i] > oOutbox.m_vMax.m_fields[i])
				oOutbox.m_vMax.m_fields[i] = vVector.m_fields[i];
		}
	}

	void fnOutbox3IMergeOutbox(_Inout_ NOUTBOX3I &oOutboxDest, _In_ NOUTBOX3I &oOutboxSource)
	{
		for (nfUint32 i = 0; i < 3; i++) {
			if (oOutboxSource.m_vMin.m_fields[i] < oOutboxDest.m_vMin.m_fields[i])
				oOutboxDest.m_vMin.m_fields[i] = oOutboxSource.m_vMin.m_fields[i];
			if (oOutboxSource.m_vMax.m_fields[i] > oOutboxDest.m_vMax.m_fields[i])
				oOutboxDest.m_vMax.m_fields[i] = oOutboxSource.m_vMax.m_fields[i];
		}
	}

	NOUTBOX3I fnCalcTriangleOutbox3I(_In_ const NVEC3I vVector1, _In_ const NVEC3I vVector2, _In_ const NVEC3I vVector3)
	{
		NOUTBOX3I oOutbox;
		oOutbox.m_vMin = vVector1;
		oOutbox.m_vMax = vVector1;
		fnOutbox3IMergeVector(oOutbox, vVector2);
		fnOutbox3IMergeVector(oOutbox, vVector3);

		return oOutbox;
	}

	nfBool fnOutbox3IDoIntersect(_In_ const NOUTBOX3I oOutbox1, _In_ const NOUTBOX3I oOutbox2)
	{
		nfBool bResult = true;
		nfBool bIntervalIntersects;
		for (nfUint32 i = 0; i < 3; i++) {
			bIntervalIntersects = (((oOutbox1.m_vMin.m_fields[i] >= oOutbox2.m_vMin.m_fields[i]) && (oOutbox1.m_vMin.m_fields[i] <= oOutbox2.m_vMax.m_fields[i]))
				|| ((oOutbox1.m_vMax.m_fields[i] >= oOutbox2.m_vMin.m_fields[i]) && (oOutbox1.m_vMax.m_fields[i] <= oOutbox2.m_vMax.m_fields[i]))
				|| ((oOutbox2.m_vMin.m_fields[i] >= oOutbox1.m_vMin.m_fields[i]) && (oOutbox2.m_vMin.m_fields[i] <= oOutbox1.m_vMax.m_fields[i]))
				|| ((oOutbox2.m_vMax.m_fields[i] >= oOutbox1.m_vMin.m_fields[i]) && (oOutbox2.m_vMax.m_fields[i] <= oOutbox1.m_vMax.m_fields[i])));
			
			bResult = bResult && bIntervalIntersects;
		}

		return bResult;
	}

}
