/*++

Copyright (C) 2015 Microsoft Corporation 
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_Matrix.cpp implements all needed matrix calculations and transforms for
2D and 3D vectors.

--*/

#include "Common/Math/NMR_Geometry.h" 
#include "Common/Math/NMR_Matrix.h" 
#include "Common/Math/NMR_Vector.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 
#include <math.h>
#include <string>
#include <sstream>

namespace NMR {

	NMATRIX2 fnMATRIX2_identity()
	{
		NMATRIX2 mResult;
		nfInt32 i, j;
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				mResult.m_fields[i][j] = (i == j) ? 1.0f : 0.0f;

		return mResult;
	}

	NMATRIX2 fnMATRIX2_rotation(_In_ const nfFloat fAngle)
	{
		NMATRIX2 mResult;
		nfFloat s = sinf(fAngle);
		nfFloat c = cosf(fAngle);

		mResult.m_fields[0][0] = c;
		mResult.m_fields[0][1] = -s;
		mResult.m_fields[0][2] = 0.0f;
		mResult.m_fields[1][0] = s;
		mResult.m_fields[1][1] = c;
		mResult.m_fields[1][2] = 0.0f;
		mResult.m_fields[2][0] = 0.0f;
		mResult.m_fields[2][1] = 0.0f;
		mResult.m_fields[2][2] = 1.0f;

		return mResult;
	}

	NMATRIX2 fnMATRIX2_translation(_In_ const NVEC2 vTranslation)
	{
		NMATRIX2 mResult;

		mResult.m_fields[0][0] = 1.0f;
		mResult.m_fields[0][1] = 0.0f;
		mResult.m_fields[0][2] = vTranslation.m_fields[0];
		mResult.m_fields[1][0] = 0.0f;
		mResult.m_fields[1][1] = 1.0f;
		mResult.m_fields[1][2] = vTranslation.m_fields[1];
		mResult.m_fields[2][0] = 0.0f;
		mResult.m_fields[2][1] = 0.0f;
		mResult.m_fields[2][2] = 1.0f;

		return mResult;
	}

	NMATRIX2 fnMATRIX2_transformation(_In_ const nfFloat fAngle, _In_ const NVEC2 vTranslation)
	{
		NMATRIX2 mResult;
		nfFloat s = sinf(fAngle);
		nfFloat c = cosf(fAngle);

		mResult.m_fields[0][0] = c;
		mResult.m_fields[0][1] = -s;
		mResult.m_fields[0][2] = vTranslation.m_fields[0];
		mResult.m_fields[1][0] = s;
		mResult.m_fields[1][1] = c;
		mResult.m_fields[1][2] = vTranslation.m_fields[1];
		mResult.m_fields[2][0] = 0.0f;
		mResult.m_fields[2][1] = 0.0f;
		mResult.m_fields[2][2] = 1.0f;

		return mResult;
	}

	NMATRIX2 fnMATRIX2_uniformscale(_In_ nfFloat fScale)
	{
		NMATRIX2 mResult;

		mResult.m_fields[0][0] = fScale;
		mResult.m_fields[0][1] = 0.0f;
		mResult.m_fields[0][2] = 0.0f;
		mResult.m_fields[1][0] = 0.0f;
		mResult.m_fields[1][1] = fScale;
		mResult.m_fields[1][2] = 0.0f;
		mResult.m_fields[2][0] = 0.0f;
		mResult.m_fields[2][1] = 0.0f;
		mResult.m_fields[2][2] = 1.0f;

		return mResult;
	}

	NMATRIX2 fnMATRIX2_scale(_In_ nfFloat fScaleX, _In_ nfFloat fScaleY)
	{
		NMATRIX2 mResult;

		mResult.m_fields[0][0] = fScaleX;
		mResult.m_fields[0][1] = 0.0f;
		mResult.m_fields[0][2] = 0.0f;
		mResult.m_fields[1][0] = 0.0f;
		mResult.m_fields[1][1] = fScaleY;
		mResult.m_fields[1][2] = 0.0f;
		mResult.m_fields[2][0] = 0.0f;
		mResult.m_fields[2][1] = 0.0f;
		mResult.m_fields[2][2] = 1.0f;

		return mResult;
	}

	NMATRIX2 fnMATRIX2_multiply(_In_ const NMATRIX2 mMatrix1, _In_  const NMATRIX2 mMatrix2)
	{
		NMATRIX2 mResult;
		nfInt32 i, j;
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				mResult.m_fields[i][j] = mMatrix1.m_fields[i][0] * mMatrix2.m_fields[0][j] +
				mMatrix1.m_fields[i][1] * mMatrix2.m_fields[1][j] +
				mMatrix1.m_fields[i][2] * mMatrix2.m_fields[2][j];

		return mResult;
	}

	NVEC2 fnMATRIX2_apply(_In_ const NMATRIX2 mMatrix, _In_ const NVEC2 vVector)
	{
		NVEC2 vResult;
		nfInt32 i;
		for (i = 0; i < 2; i++)
			vResult.m_fields[i] = mMatrix.m_fields[i][0] * vVector.m_fields[0] + mMatrix.m_fields[i][1] * vVector.m_fields[1] + mMatrix.m_fields[i][2];

		return vResult;
	}

	NLINMATRIX2 fnLINMATRIX2_identity()
	{
		NLINMATRIX2 mResult;
		nfInt32 i, j;
		for (i = 0; i < 2; i++)
			for (j = 0; j < 2; j++)
				mResult.m_fields[i][j] = (i == j) ? 1.0f : 0.0f;

		return mResult;
	}

	NLINMATRIX2 fnLINMATRIX2_rotation(_In_ const nfFloat fAngle)
	{
		NLINMATRIX2 mResult;
		nfFloat s = sinf(fAngle);
		nfFloat c = cosf(fAngle);

		mResult.m_fields[0][0] = c;
		mResult.m_fields[0][1] = -s;
		mResult.m_fields[1][0] = s;
		mResult.m_fields[1][1] = c;

		return mResult;
	}

	NLINMATRIX2 fnLINMATRIX2_uniformscale(_In_ nfFloat fScale)
	{
		NLINMATRIX2 mResult;

		mResult.m_fields[0][0] = fScale;
		mResult.m_fields[0][1] = 0.0f;
		mResult.m_fields[1][0] = 0.0f;
		mResult.m_fields[1][1] = fScale;

		return mResult;
	}

	NLINMATRIX2 fnLINMATRIX2_scale(_In_ nfFloat fScaleX, _In_ nfFloat fScaleY)
	{
		NLINMATRIX2 mResult;

		mResult.m_fields[0][0] = fScaleX;
		mResult.m_fields[0][1] = 0.0f;
		mResult.m_fields[1][0] = 0.0f;
		mResult.m_fields[1][1] = fScaleY;

		return mResult;
	}

	NLINMATRIX2 fnLINMATRIX2_multiply(_In_ const NLINMATRIX2 mMatrix1, _In_  const NLINMATRIX2 mMatrix2)
	{
		NLINMATRIX2 mResult;
		nfInt32 i, j;
		for (i = 0; i < 2; i++)
			for (j = 0; j < 2; j++)
				mResult.m_fields[i][j] = mMatrix1.m_fields[i][0] * mMatrix2.m_fields[0][j] +
				mMatrix1.m_fields[i][1] * mMatrix2.m_fields[1][j];

		return mResult;
	}

	NVEC2 fnLINMATRIX2_apply(_In_ const NLINMATRIX2 mMatrix, _In_ const NVEC2 vVector)
	{
		NVEC2 vResult;
		nfInt32 i;
		for (i = 0; i < 2; i++)
			vResult.m_fields[i] = mMatrix.m_fields[i][0] * vVector.m_fields[0] + mMatrix.m_fields[i][1] * vVector.m_fields[1];

		return vResult;
	}

	NLINMATRIX2 fnLINMATRIX2_invert(_In_ const NLINMATRIX2 mMatrix, _In_ nfBool bFailIfSingular)
	{
		nfFloat fDet = mMatrix.m_fields[0][0] * mMatrix.m_fields[1][1] - mMatrix.m_fields[0][1] * mMatrix.m_fields[1][0];
		if (fabs(fDet) < NMR_MATRIX_MINDETERMINANT) {
			if (bFailIfSingular)
				throw CNMRException(NMR_ERROR_SINGULARMATRIX);
			return fnLINMATRIX2_identity();
		}

		NLINMATRIX2 mResult;

		mResult.m_fields[0][0] = mMatrix.m_fields[1][1] / fDet;
		mResult.m_fields[0][1] = -mMatrix.m_fields[0][1] / fDet;
		mResult.m_fields[1][0] = -mMatrix.m_fields[1][0] / fDet;
		mResult.m_fields[1][1] = mMatrix.m_fields[0][0] / fDet;

		return mResult;
	}

	NMATRIX3 fnMATRIX3_identity()
	{
		NMATRIX3 mResult;
		nfInt32 i, j;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				mResult.m_fields[i][j] = (i == j) ? 1.0f : 0.0f;

		return mResult;
	}

	nfBool fnMATRIX3_isIdentity(_In_ const NMATRIX3 mMatrix)
	{
		nfDouble dDelta = 0.0;
		nfInt32 i, j;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++) {
				nfDouble dId = (i == j) ? 1.0f : 0.0f;
				nfDouble dM = mMatrix.m_fields[i][j];
				nfDouble dX = dM - dId;
				dDelta += dX * dX;
			}

		return dDelta < NMR_MATRIX_IDENTITYTHRESHOLD;
	}

	NMATRIX3 fnMATRIX3_rotation(_In_ const NVEC3 vAxis, _In_ const nfFloat fAngle)
	{
		NVEC3 vNormalAxis = fnVEC3_normalize(vAxis);

		NMATRIX3 mResult;
		nfFloat s = sinf(fAngle);
		nfFloat c = cosf(fAngle);
		nfFloat c1 = 1 - c;

		mResult.m_fields[0][0] = vNormalAxis.m_fields[0] * vNormalAxis.m_fields[0] * c1 + c;
		mResult.m_fields[0][1] = vNormalAxis.m_fields[1] * vNormalAxis.m_fields[0] * c1 + vNormalAxis.m_fields[2] * s;
		mResult.m_fields[0][2] = vNormalAxis.m_fields[2] * vNormalAxis.m_fields[0] * c1 - vNormalAxis.m_fields[1] * s;
		mResult.m_fields[0][3] = 0.0f;
		mResult.m_fields[1][0] = vNormalAxis.m_fields[0] * vNormalAxis.m_fields[1] * c1 - vNormalAxis.m_fields[2] * s;
		mResult.m_fields[1][1] = vNormalAxis.m_fields[1] * vNormalAxis.m_fields[1] * c1 + c;
		mResult.m_fields[1][2] = vNormalAxis.m_fields[2] * vNormalAxis.m_fields[1] * c1 + vNormalAxis.m_fields[0] * s;
		mResult.m_fields[1][3] = 0.0f;
		mResult.m_fields[2][0] = vNormalAxis.m_fields[0] * vNormalAxis.m_fields[2] * c1 + vNormalAxis.m_fields[1] * s;
		mResult.m_fields[2][1] = vNormalAxis.m_fields[1] * vNormalAxis.m_fields[2] * c1 - vNormalAxis.m_fields[0] * s;
		mResult.m_fields[2][2] = vNormalAxis.m_fields[2] * vNormalAxis.m_fields[2] * c1 + c;
		mResult.m_fields[2][3] = 0.0f;
		mResult.m_fields[3][0] = 0.0f;
		mResult.m_fields[3][1] = 0.0f;
		mResult.m_fields[3][2] = 0.0f;
		mResult.m_fields[3][3] = 1.0f;

		return mResult;
	}

	NMATRIX3 fnMATRIX3_translation(_In_ const NVEC3 vTranslation)
	{
		NMATRIX3 mResult;

		mResult.m_fields[0][0] = 1.0f;
		mResult.m_fields[0][1] = 0.0f;
		mResult.m_fields[0][2] = 0.0f;
		mResult.m_fields[0][3] = vTranslation.m_fields[0];
		mResult.m_fields[1][0] = 0.0f;
		mResult.m_fields[1][1] = 1.0f;
		mResult.m_fields[1][2] = 0.0f;
		mResult.m_fields[1][3] = vTranslation.m_fields[1];
		mResult.m_fields[2][0] = 0.0f;
		mResult.m_fields[2][1] = 0.0f;
		mResult.m_fields[2][2] = 1.0f;
		mResult.m_fields[2][3] = vTranslation.m_fields[2];
		mResult.m_fields[3][0] = 0.0f;
		mResult.m_fields[3][1] = 0.0f;
		mResult.m_fields[3][2] = 0.0f;
		mResult.m_fields[3][3] = 1.0f;

		return mResult;
	}

	NMATRIX3 fnMATRIX3_transformation(_In_ const NVEC3 vAxis, _In_ const nfFloat fAngle, _In_ const NVEC3 vTranslation)
	{
		NMATRIX3 mResult = fnMATRIX3_rotation(vAxis, fAngle);

		mResult.m_fields[0][3] = vTranslation.m_fields[0];
		mResult.m_fields[1][3] = vTranslation.m_fields[1];
		mResult.m_fields[2][3] = vTranslation.m_fields[2];

		return mResult;
	}

	NMATRIX3 fnMATRIX3_multiply(_In_ const NMATRIX3 mMatrix1, _In_ const NMATRIX3 mMatrix2)
	{
		NMATRIX3 mResult;
		nfInt32 i, j;
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				mResult.m_fields[i][j] = mMatrix1.m_fields[i][0] * mMatrix2.m_fields[0][j] +
				mMatrix1.m_fields[i][1] * mMatrix2.m_fields[1][j] +
				mMatrix1.m_fields[i][2] * mMatrix2.m_fields[2][j] +
				mMatrix1.m_fields[i][3] * mMatrix2.m_fields[3][j];

		return mResult;

	}

	NMATRIX3 fnMATRIX3_uniformscale(_In_ nfFloat fScale)
	{
		NMATRIX3 mResult;

		mResult.m_fields[0][0] = fScale;
		mResult.m_fields[0][1] = 0.0f;
		mResult.m_fields[0][2] = 0.0f;
		mResult.m_fields[0][3] = 0.0f;
		mResult.m_fields[1][0] = 0.0f;
		mResult.m_fields[1][1] = fScale;
		mResult.m_fields[1][2] = 0.0f;
		mResult.m_fields[1][3] = 0.0f;
		mResult.m_fields[2][0] = 0.0f;
		mResult.m_fields[2][1] = 0.0f;
		mResult.m_fields[2][2] = fScale;
		mResult.m_fields[2][3] = 0.0f;
		mResult.m_fields[3][0] = 0.0f;
		mResult.m_fields[3][1] = 0.0f;
		mResult.m_fields[3][2] = 0.0f;
		mResult.m_fields[3][3] = 1.0f;

		return mResult;
	}

	NMATRIX3 fnMATRIX3_scale(_In_ nfFloat fScaleX, _In_ nfFloat fScaleY, _In_ nfFloat fScaleZ)
	{
		NMATRIX3 mResult;

		mResult.m_fields[0][0] = fScaleX;
		mResult.m_fields[0][1] = 0.0f;
		mResult.m_fields[0][2] = 0.0f;
		mResult.m_fields[0][3] = 0.0f;
		mResult.m_fields[1][0] = 0.0f;
		mResult.m_fields[1][1] = fScaleY;
		mResult.m_fields[1][2] = 0.0f;
		mResult.m_fields[1][3] = 0.0f;
		mResult.m_fields[2][0] = 0.0f;
		mResult.m_fields[2][1] = 0.0f;
		mResult.m_fields[2][2] = fScaleZ;
		mResult.m_fields[2][3] = 0.0f;
		mResult.m_fields[3][0] = 0.0f;
		mResult.m_fields[3][1] = 0.0f;
		mResult.m_fields[3][2] = 0.0f;
		mResult.m_fields[3][3] = 1.0f;

		return mResult;
	}

	NVEC3 fnMATRIX3_apply(_In_ const NMATRIX3 mMatrix, _In_ const NVEC3 vVector)
	{
		NVEC3 vResult;
		nfInt32 i;
		for (i = 0; i < 3; i++)
			vResult.m_fields[i] = mMatrix.m_fields[i][0] * vVector.m_fields[0] + mMatrix.m_fields[i][1] * vVector.m_fields[1] + mMatrix.m_fields[i][2] * vVector.m_fields[2] + mMatrix.m_fields[i][3];

		return vResult;
	}

	std::wstring fnMATRIX3_toWideString(_In_ const NMATRIX3 mMatrix) {
		std::wstringstream sStream;
		nfInt32 i, j;

		for (j = 0; j < 4; j++) {
			for (i = 0; i < 3; i++) {
				sStream << mMatrix.m_fields[i][j];
				if ((i != 2) || (j != 3))
					sStream << L" ";
			}
		}

		return sStream.str();
	}

	NMATRIX3 fnMATRIX3_fromWideString(_In_ const std::wstring sString)
	{
		nfUint32 nValueCount = 0;
		nfFloat nValues[12];

		const nfWChar * pwszString = sString.c_str();
		const nfWChar * pCurrent = pwszString;

		nfBool bFinished = false;
		while (!bFinished) {
			// Find next space
			const nfWChar * pBegin = pCurrent;
			while ((*pCurrent != L' ') && (*pCurrent))
				pCurrent++;

			// If we have not found a space, convert value to double
			if (pBegin != pCurrent) {
				if (nValueCount >= 12)
					throw CNMRException(NMR_ERROR_TOOMANYVALUESINMATRIXSTRING);
				nValues[nValueCount] = fnWStringToFloat(pBegin);
				nValueCount++;
			}

			// If we are finished, break, otherwise skip space!
			if (!*pCurrent)
				bFinished = true;
			else
				pCurrent++;
		}

		if (nValueCount < 12)
			throw CNMRException(NMR_ERROR_NOTENOUGHVALUESINMATRIXSTRING);

		NMATRIX3 mResult;

		nfInt32 i, j;
		for (j = 0; j < 4; j++) {
			for (i = 0; i < 3; i++) {
				mResult.m_fields[i][j] = nValues[i + j * 3];
			}
		}

		mResult.m_fields[3][0] = 0.0f;
		mResult.m_fields[3][1] = 0.0f;
		mResult.m_fields[3][2] = 0.0f;
		mResult.m_fields[3][3] = 1.0f;

		return mResult;
	}

}
