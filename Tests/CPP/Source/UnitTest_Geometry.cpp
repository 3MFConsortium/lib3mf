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

UnitTest_Geometry.cpp: Defines Unittests for basic linear algebraic classes and
operations

--*/

#include "UnitTest_Utilities.h"
#include "Common/Math/NMR_Vector.h"
#include "Common/Math/NMR_Matrix.h"
#include "Common/NMR_Exception.h"

namespace NMR
{
	
	TEST(Geometry, VEC2Basics)
	{
		NVEC2 vec1 = fnVEC2_make(4.0f, 5.0f);
		NVEC2 vec2 = fnVEC2_make(2.0f, 3.0f);

		ASSERT_EQ(vec1.m_fields[0], 4.0f);
		ASSERT_EQ(vec1.m_fields[1], 5.0f);
		ASSERT_EQ(vec1.m_values.x, vec1.m_fields[0]);
		ASSERT_EQ(vec1.m_values.y, vec1.m_fields[1]);

		ASSERT_EQ(vec2.m_fields[0], 2.0f);
		ASSERT_EQ(vec2.m_fields[1], 3.0f);
		ASSERT_EQ(vec2.m_values.x, vec2.m_fields[0]);
		ASSERT_EQ(vec2.m_values.y, vec2.m_fields[1]);

		NVEC2 vec3 = fnVEC2_add(vec1, vec2);
		NVEC2 vec4 = vec1 + vec2;

		ASSERT_EQ(vec3.m_values.x, 6.0f);
		ASSERT_EQ(vec3.m_values.y, 8.0f);
		ASSERT_EQ(vec4.m_values.x, 6.0f);
		ASSERT_EQ(vec4.m_values.y, 8.0f);

		NVEC2 vec5 = fnVEC2_sub(vec1, vec2);
		NVEC2 vec6 = vec1 - vec2;

		ASSERT_EQ(vec5.m_values.x, 2.0f);
		ASSERT_EQ(vec5.m_values.y, 2.0f);
		ASSERT_EQ(vec6.m_values.x, 2.0f);
		ASSERT_EQ(vec6.m_values.y, 2.0f);

		nfFloat factor = -2.0f;

		NVEC2 vec7 = fnVEC2_scale(vec1, factor);
		NVEC2 vec8 = vec1 * factor;
		NVEC2 vec9 = factor * vec1;

		ASSERT_EQ(vec7.m_values.x, -8.0f);
		ASSERT_EQ(vec7.m_values.y, -10.0f);
		ASSERT_EQ(vec8.m_values.x, -8.0f);
		ASSERT_EQ(vec8.m_values.y, -10.0f);
		ASSERT_EQ(vec9.m_values.x, -8.0f);
		ASSERT_EQ(vec9.m_values.y, -10.0f);
	}

	TEST(Geometry, VEC2DotProducts)
	{
		NVEC2 xaxis = fnVEC2_make(1.0f, 0.0f);
		NVEC2 yaxis = fnVEC2_make(0.0f, 1.0f);

		NVEC2 vec = fnVEC2_make(1.0f, 2.0f);

		nfFloat dotproduct1 = fnVEC2_dotproduct(xaxis, yaxis);
		nfFloat dotproduct2 = fnVEC2_dotproduct(xaxis, vec);
		nfFloat dotproduct3 = fnVEC2_dotproduct(vec, yaxis);
		nfFloat dotproduct4 = fnVEC2_dotproduct(yaxis, xaxis);
		nfFloat dotproduct5 = fnVEC2_dotproduct(vec, xaxis);
		nfFloat dotproduct6 = fnVEC2_dotproduct(yaxis, vec);

		ASSERT_EQ(dotproduct1, 0.0f);
		ASSERT_EQ(dotproduct2, 1.0f);
		ASSERT_EQ(dotproduct3, 2.0f);
		ASSERT_EQ(dotproduct4, 0.0f);
		ASSERT_EQ(dotproduct5, 1.0f);
		ASSERT_EQ(dotproduct6, 2.0f);
	}

	TEST(Geometry, VEC2CrossProducts)
	{
		NVEC2 xaxis = fnVEC2_make(1.0f, 0.0f);
		NVEC2 yaxis = fnVEC2_make(0.0f, 1.0f);

		NVEC2 vec = fnVEC2_make(1.0f, 2.0f);

		nfFloat crossproduct1 = fnVEC2_crossproduct(xaxis, yaxis);
		nfFloat crossproduct2 = fnVEC2_crossproduct(xaxis, vec);
		nfFloat crossproduct3 = fnVEC2_crossproduct(vec, yaxis);
		nfFloat crossproduct4 = fnVEC2_crossproduct(yaxis, xaxis);
		nfFloat crossproduct5 = fnVEC2_crossproduct(vec, xaxis);
		nfFloat crossproduct6 = fnVEC2_crossproduct(yaxis, vec);

		nfFloat crossproduct7 = fnVEC2_crossproduct(xaxis, xaxis);
		nfFloat crossproduct8 = fnVEC2_crossproduct(yaxis, yaxis);
		nfFloat crossproduct9 = fnVEC2_crossproduct(vec, vec);

		ASSERT_EQ(crossproduct1, 1.0f);
		ASSERT_EQ(crossproduct2, 2.0f);
		ASSERT_EQ(crossproduct3, 1.0f);
		ASSERT_EQ(crossproduct4, -1.0f);
		ASSERT_EQ(crossproduct5, -2.0f);
		ASSERT_EQ(crossproduct6, -1.0f);

		ASSERT_EQ(crossproduct7, 0.0f);
		ASSERT_EQ(crossproduct8, 0.0f);
		ASSERT_EQ(crossproduct9, 0.0f);
	}

	TEST(Geometry, VEC2Length)
	{
		NVEC2 xaxis = fnVEC2_make(1.0f, 0.0f);
		NVEC2 yaxis = fnVEC2_make(0.0f, 1.0f);
		NVEC2 vec = fnVEC2_make(3.0f, 4.0f);

		nfFloat length1 = fnVEC2_length(xaxis);
		nfFloat length2 = fnVEC2_length(yaxis);
		nfFloat length3 = fnVEC2_length(vec);

		ASSERT_EQ(length1, 1.0f);
		ASSERT_EQ(length2, 1.0f);
		ASSERT_EQ(length3, 5.0f);
	}

	TEST(Geometry, VEC2Normalize)
	{
		NVEC2 vec1 = fnVEC2_make(10.0f, 0.0f);
		NVEC2 vec2 = fnVEC2_make(0.0f, 11.0f);
		NVEC2 vec3 = fnVEC2_make(-3.0f, -4.0f);
		NVEC2 vec4 = fnVEC2_make(0.0f, 0.0f);

		NVEC2 norm1 = fnVEC2_normalize(vec1);
		NVEC2 norm2 = fnVEC2_normalize(vec2);
		NVEC2 norm3 = fnVEC2_normalize(vec3);

		ASSERT_EQ(norm1.m_values.x, 1.0f);
		ASSERT_EQ(norm1.m_values.y, 0.0f);
		ASSERT_EQ(norm2.m_values.x, 0.0f);
		ASSERT_EQ(norm2.m_values.y, 1.0f);
		ASSERT_EQ(norm3.m_values.x, -0.6f);
		ASSERT_EQ(norm3.m_values.y, -0.8f);

		try
		{
			fnVEC2_normalize(vec4);
			ASSERT_TRUE(false) << "This should have thrown an exception before.";
		}
		catch (CNMRException e)
		{
			ASSERT_EQ(e.getErrorCode(), NMR_ERROR_NORMALIZEDZEROVECTOR);
		}

	}

	TEST(Geometry,VEC3Basics)
	{
		NVEC3 vec1 = fnVEC3_make(4.0f, 5.0f, 6.0f);
		NVEC3 vec2 = fnVEC3_make(2.0f, 3.0f, 4.0f);

		ASSERT_EQ(vec1.m_fields[0], 4.0f);
		ASSERT_EQ(vec1.m_fields[1], 5.0f);
		ASSERT_EQ(vec1.m_fields[2], 6.0f);
		ASSERT_EQ(vec1.m_values.x, vec1.m_fields[0]);
		ASSERT_EQ(vec1.m_values.y, vec1.m_fields[1]);
		ASSERT_EQ(vec1.m_values.z, vec1.m_fields[2]);

		ASSERT_EQ(vec2.m_fields[0], 2.0f);
		ASSERT_EQ(vec2.m_fields[1], 3.0f);
		ASSERT_EQ(vec2.m_fields[2], 4.0f);
		ASSERT_EQ(vec2.m_values.x, vec2.m_fields[0]);
		ASSERT_EQ(vec2.m_values.y, vec2.m_fields[1]);
		ASSERT_EQ(vec2.m_values.z, vec2.m_fields[2]);

		NVEC3 vec3 = fnVEC3_add(vec1, vec2);
		NVEC3 vec4 = vec1 + vec2;

		ASSERT_EQ(vec3.m_values.x, 6.0f);
		ASSERT_EQ(vec3.m_values.y, 8.0f);
		ASSERT_EQ(vec3.m_values.z, 10.0f);
		ASSERT_EQ(vec4.m_values.x, 6.0f);
		ASSERT_EQ(vec4.m_values.y, 8.0f);
		ASSERT_EQ(vec4.m_values.z, 10.0f);

		NVEC3 vec5 = fnVEC3_sub(vec1, vec2);
		NVEC3 vec6 = vec1 - vec2;

		ASSERT_EQ(vec5.m_values.x, 2.0f);
		ASSERT_EQ(vec5.m_values.y, 2.0f);
		ASSERT_EQ(vec5.m_values.z, 2.0f);
		ASSERT_EQ(vec6.m_values.x, 2.0f);
		ASSERT_EQ(vec6.m_values.y, 2.0f);
		ASSERT_EQ(vec6.m_values.z, 2.0f);

		nfFloat factor = -2.0f;

		NVEC3 vec7 = fnVEC3_scale(vec1, factor);
		NVEC3 vec8 = vec1 * factor;
		NVEC3 vec9 = factor * vec1;

		ASSERT_EQ(vec7.m_values.x, -8.0f);
		ASSERT_EQ(vec7.m_values.y, -10.0f);
		ASSERT_EQ(vec7.m_values.z, -12.0f);
		ASSERT_EQ(vec8.m_values.x, -8.0f);
		ASSERT_EQ(vec8.m_values.y, -10.0f);
		ASSERT_EQ(vec8.m_values.z, -12.0f);
		ASSERT_EQ(vec9.m_values.x, -8.0f);
		ASSERT_EQ(vec9.m_values.y, -10.0f);
		ASSERT_EQ(vec9.m_values.z, -12.0f);
	}

	TEST(Geometry,VEC3DotProducts)
	{
		NVEC3 xaxis = fnVEC3_make(1.0f, 0.0f, 0.0f);
		NVEC3 yaxis = fnVEC3_make(0.0f, 1.0f, 0.0f);
		NVEC3 zaxis = fnVEC3_make(0.0f, 0.0f, 1.0f);

		NVEC3 vec = fnVEC3_make(1.0f, 2.0f, 3.0f);

		nfFloat dotproduct1 = fnVEC3_dotproduct(xaxis, yaxis);
		nfFloat dotproduct2 = fnVEC3_dotproduct(xaxis, vec);
		nfFloat dotproduct3 = fnVEC3_dotproduct(vec, yaxis);
		nfFloat dotproduct4 = fnVEC3_dotproduct(yaxis, xaxis);
		nfFloat dotproduct5 = fnVEC3_dotproduct(vec, xaxis);
		nfFloat dotproduct6 = fnVEC3_dotproduct(yaxis, vec);

		nfFloat dotproduct7 = fnVEC3_dotproduct(zaxis, xaxis);
		nfFloat dotproduct8 = fnVEC3_dotproduct(zaxis, yaxis);
		nfFloat dotproduct9 = fnVEC3_dotproduct(zaxis, vec);
		nfFloat dotproduct10 = fnVEC3_dotproduct(xaxis, zaxis);
		nfFloat dotproduct11 = fnVEC3_dotproduct(yaxis, zaxis);
		nfFloat dotproduct12 = fnVEC3_dotproduct(vec, zaxis);

		ASSERT_EQ(dotproduct1, 0.0f);
		ASSERT_EQ(dotproduct2, 1.0f);
		ASSERT_EQ(dotproduct3, 2.0f);
		ASSERT_EQ(dotproduct4, 0.0f);
		ASSERT_EQ(dotproduct5, 1.0f);
		ASSERT_EQ(dotproduct6, 2.0f);

		ASSERT_EQ(dotproduct7, 0.0f);
		ASSERT_EQ(dotproduct8, 0.0f);
		ASSERT_EQ(dotproduct9, 3.0f);
		ASSERT_EQ(dotproduct10, 0.0f);
		ASSERT_EQ(dotproduct11, 0.0f);
		ASSERT_EQ(dotproduct12, 3.0f);
	}

	TEST(Geometry,VEC3Length)
	{
		NVEC3 xaxis = fnVEC3_make(1.0f, 0.0f, 0.0f);
		NVEC3 yaxis = fnVEC3_make(0.0f, 1.0f, 0.0f);
		NVEC3 zaxis = fnVEC3_make(0.0f, 0.0f, 1.0f);
		NVEC3 vec = fnVEC3_make(8.0f, 15.0f, 144.0f);

		nfFloat length1 = fnVEC3_length(xaxis);
		nfFloat length2 = fnVEC3_length(yaxis);
		nfFloat length3 = fnVEC3_length(zaxis);
		nfFloat length4 = fnVEC3_length(vec);

		ASSERT_EQ(length1, 1.0f);
		ASSERT_EQ(length2, 1.0f);
		ASSERT_EQ(length3, 1.0f);
		ASSERT_EQ(length4, 145.0f);
	}


	TEST(Geometry,VEC3CrossProducts)
	{
		NVEC3 xaxis = fnVEC3_make(1.0f, 0.0f, 0.0f);
		NVEC3 yaxis = fnVEC3_make(0.0f, 1.0f, 0.0f);
		NVEC3 zaxis = fnVEC3_make(0.0f, 0.0f, 1.0f);

		NVEC3 vec = fnVEC3_make(1.0f, 2.0f, 3.0f);

		NVEC3 crossproduct1 = fnVEC3_crossproduct(xaxis, yaxis);
		NVEC3 crossproduct2 = fnVEC3_crossproduct(xaxis, vec);
		NVEC3 crossproduct3 = fnVEC3_crossproduct(vec, yaxis);
		NVEC3 crossproduct4 = fnVEC3_crossproduct(yaxis, xaxis);
		NVEC3 crossproduct5 = fnVEC3_crossproduct(vec, xaxis);
		NVEC3 crossproduct6 = fnVEC3_crossproduct(yaxis, vec);
		NVEC3 crossproduct7 = fnVEC3_crossproduct(xaxis, xaxis);
		NVEC3 crossproduct8 = fnVEC3_crossproduct(yaxis, yaxis);
		NVEC3 crossproduct9 = fnVEC3_crossproduct(zaxis, zaxis);
		NVEC3 crossproduct10 = fnVEC3_crossproduct(vec, vec);
		NVEC3 crossproduct11 = fnVEC3_crossproduct(xaxis, zaxis);
		NVEC3 crossproduct12 = fnVEC3_crossproduct(yaxis, zaxis);
		NVEC3 crossproduct13 = fnVEC3_crossproduct(zaxis, xaxis);
		NVEC3 crossproduct14 = fnVEC3_crossproduct(zaxis, yaxis);

		ASSERT_EQ(crossproduct1.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct1.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct1.m_values.z, 1.0f);
		ASSERT_EQ(crossproduct2.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct2.m_values.y, -3.0f);
		ASSERT_EQ(crossproduct2.m_values.z, 2.0f);
		ASSERT_EQ(crossproduct3.m_values.x, -3.0f);
		ASSERT_EQ(crossproduct3.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct3.m_values.z, 1.0f);
		ASSERT_EQ(crossproduct4.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct4.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct4.m_values.z, -1.0f);
		ASSERT_EQ(crossproduct5.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct5.m_values.y, 3.0f);
		ASSERT_EQ(crossproduct5.m_values.z, -2.0f);
		ASSERT_EQ(crossproduct6.m_values.x, 3.0f);
		ASSERT_EQ(crossproduct6.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct6.m_values.z, -1.0f);


		ASSERT_EQ(crossproduct7.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct7.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct7.m_values.z, 0.0f);
		ASSERT_EQ(crossproduct8.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct8.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct8.m_values.z, 0.0f);
		ASSERT_EQ(crossproduct9.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct9.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct9.m_values.z, 0.0f);
		ASSERT_EQ(crossproduct10.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct10.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct10.m_values.z, 0.0f);

		ASSERT_EQ(crossproduct11.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct11.m_values.y, -1.0f);
		ASSERT_EQ(crossproduct11.m_values.z, 0.0f);
		ASSERT_EQ(crossproduct12.m_values.x, 1.0f);
		ASSERT_EQ(crossproduct12.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct12.m_values.z, 0.0f);
		ASSERT_EQ(crossproduct13.m_values.x, 0.0f);
		ASSERT_EQ(crossproduct13.m_values.y, 1.0f);
		ASSERT_EQ(crossproduct13.m_values.z, 0.0f);
		ASSERT_EQ(crossproduct14.m_values.x, -1.0f);
		ASSERT_EQ(crossproduct14.m_values.y, 0.0f);
		ASSERT_EQ(crossproduct14.m_values.z, 0.0f);

	}

	TEST(Geometry,VEC3Normalize)
	{
		NVEC3 vec1 = fnVEC3_make(10.0f, 0.0f, 0.0f);
		NVEC3 vec2 = fnVEC3_make(0.0f, 11.0f, 0.0f);
		NVEC3 vec3 = fnVEC3_make(0.0f, 0.0f, 12.0f);
		NVEC3 vec4 = fnVEC3_make(3.0f, 4.0f, 0.0f);
		NVEC3 vec5 = fnVEC3_make(0.0f, -3.0f, -4.0f);
		NVEC3 vec6 = fnVEC3_make(-3.0f, 0.0f, 4.0f);
		NVEC3 vec7 = fnVEC3_make(0.0f, 0.0f, 0.0f);

		NVEC3 norm1 = fnVEC3_normalize(vec1);
		NVEC3 norm2 = fnVEC3_normalize(vec2);
		NVEC3 norm3 = fnVEC3_normalize(vec3);
		NVEC3 norm4 = fnVEC3_normalize(vec4);
		NVEC3 norm5 = fnVEC3_normalize(vec5);
		NVEC3 norm6 = fnVEC3_normalize(vec6);

		ASSERT_EQ(norm1.m_values.x, 1.0f);
		ASSERT_EQ(norm1.m_values.y, 0.0f);
		ASSERT_EQ(norm1.m_values.z, 0.0f);
		ASSERT_EQ(norm2.m_values.x, 0.0f);
		ASSERT_EQ(norm2.m_values.y, 1.0f);
		ASSERT_EQ(norm2.m_values.z, 0.0f);
		ASSERT_EQ(norm3.m_values.x, 0.0f);
		ASSERT_EQ(norm3.m_values.y, 0.0f);
		ASSERT_EQ(norm3.m_values.z, 1.0f);
		ASSERT_EQ(norm4.m_values.x, 0.6f);
		ASSERT_EQ(norm4.m_values.y, 0.8f);
		ASSERT_EQ(norm4.m_values.z, 0.0f);
		ASSERT_EQ(norm5.m_values.x, 0.0f);
		ASSERT_EQ(norm5.m_values.y, -0.6f);
		ASSERT_EQ(norm5.m_values.z, -0.8f);
		ASSERT_EQ(norm6.m_values.x, -0.6f);
		ASSERT_EQ(norm6.m_values.y, 0.0f);
		ASSERT_EQ(norm6.m_values.z, 0.8f);

		try
		{
			fnVEC3_normalize(vec7);
			ASSERT_TRUE(false) << "This should have thrown an exception before.";
		}
		catch (CNMRException e)
		{
			ASSERT_EQ(e.getErrorCode(), NMR_ERROR_NORMALIZEDZEROVECTOR);
		}

	}

	TEST(Geometry, VEC3ICast)
	{
		NVEC3 vec1 = fnVEC3_make(10.0f, 20.0f, 30.0f);
		NVEC3 vec2 = fnVEC3_make(-10.0f, -20.0f, -30.0f);
		NVEC3 vec3 = fnVEC3_make(10.1f, 20.1f, 30.1f);
		NVEC3 vec4 = fnVEC3_make(10.5f, 20.5f, 30.5f);
		NVEC3 vec5 = fnVEC3_make(10.9f, 20.9f, 30.9f);
		NVEC3 vec6 = fnVEC3_make(-10.1f, -20.1f, -30.1f);
		NVEC3 vec7 = fnVEC3_make(-10.5f, -20.5f, -30.5f);
		NVEC3 vec8 = fnVEC3_make(-10.9f, -20.9f, -30.9f);

		NVEC3I veci1 = fnVEC3I_floor(vec1, 1);
		NVEC3I veci2 = fnVEC3I_floor(vec2, 1);
		NVEC3I veci3 = fnVEC3I_floor(vec3, 1);
		NVEC3I veci4 = fnVEC3I_floor(vec4, 1);
		NVEC3I veci5 = fnVEC3I_floor(vec5, 1);
		NVEC3I veci6 = fnVEC3I_floor(vec6, 1);
		NVEC3I veci7 = fnVEC3I_floor(vec7, 1);
		NVEC3I veci8 = fnVEC3I_floor(vec8, 1);

		ASSERT_EQ(veci1.m_values.x, 10);
		ASSERT_EQ(veci1.m_values.y, 20);
		ASSERT_EQ(veci1.m_values.z, 30);
		ASSERT_EQ(veci2.m_values.x, -10);
		ASSERT_EQ(veci2.m_values.y, -20);
		ASSERT_EQ(veci2.m_values.z, -30);
		ASSERT_EQ(veci3.m_values.x, 10);
		ASSERT_EQ(veci3.m_values.y, 20);
		ASSERT_EQ(veci3.m_values.z, 30);
		ASSERT_EQ(veci4.m_values.x, 10);
		ASSERT_EQ(veci4.m_values.y, 20);
		ASSERT_EQ(veci4.m_values.z, 30);
		ASSERT_EQ(veci5.m_values.x, 10);
		ASSERT_EQ(veci5.m_values.y, 20);
		ASSERT_EQ(veci5.m_values.z, 30);
		ASSERT_EQ(veci6.m_values.x, -11);
		ASSERT_EQ(veci6.m_values.y, -21);
		ASSERT_EQ(veci6.m_values.z, -31);
		ASSERT_EQ(veci7.m_values.x, -11);
		ASSERT_EQ(veci7.m_values.y, -21);
		ASSERT_EQ(veci7.m_values.z, -31);
		ASSERT_EQ(veci8.m_values.x, -11);
		ASSERT_EQ(veci8.m_values.y, -21);
		ASSERT_EQ(veci8.m_values.z, -31);

		try
		{
			fnVEC3I_floor(vec1, 0);
			ASSERT_TRUE(false);
		}
		catch (CNMRException e)
		{
			ASSERT_EQ(e.getErrorCode(), NMR_ERROR_INVALIDUNITS);
		}

	}

}
