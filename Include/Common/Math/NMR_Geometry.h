/*++

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

NMR_Geometry.h defines all basic structures for vector and matrix calculations.

--*/

#ifndef __NMR_GEOMETRY
#define __NMR_GEOMETRY

#include "Common/NMR_Types.h" 
#include "Common/NMR_Local.h" 

#define NMR_VECTOR_MINNORMALIZELENGTH 1.0e-10f
#define NMR_VECTOR_DEFAULTUNITS 0.001f
#define NMR_VECTOR_MINUNITS 0.00001f
#define NMR_VECTOR_MAXUNITS 1000.0f
#define NMR_OUTBOX_MAXCOORDINATE 10000000000.0f

namespace NMR {

	typedef union {
		nfFloat m_fields[2];
		struct { nfFloat x; nfFloat y; } m_values;
	} NVEC2;

	typedef union {
		nfFloat m_fields[3];
		struct { nfFloat x; nfFloat y; nfFloat z; } m_values;
	} NVEC3;

	typedef union {
		nfDouble m_fields[3];
		struct { nfDouble x; nfDouble y; nfDouble z; } m_values;
	} NVEC3D;

	typedef union {
		nfInt32 m_fields[2];
		struct { nfInt32 x; nfInt32 y; } m_values;
	} NVEC2I;

	typedef union {
		nfInt32 m_fields[3];
		struct { nfInt32 x; nfInt32 y; nfInt32 z; } m_values;
	} NVEC3I;

	typedef union {
		nfInt64 m_fields[3];
		struct { nfInt64 x; nfInt64 y; nfInt64 z; } m_values;
	} NVEC3I64;

	typedef struct {
		nfFloat m_fields[3][3];
	} NMATRIX2;

	typedef struct {
		nfFloat m_fields[2][2];
	} NLINMATRIX2;

	typedef struct {
		nfFloat m_fields[4][4];
	} NMATRIX3;

	typedef struct {
		NVEC2 m_min;
		NVEC2 m_max;
	} NOUTBOX2;

	typedef struct {
		NVEC3 m_min;
		NVEC3 m_max;
	} NOUTBOX3;

	typedef struct {
		NVEC2I m_vMin;
		NVEC2I m_vMax;
	} NOUTBOX2I;

	typedef struct {
		NVEC3I m_vMin;
		NVEC3I m_vMax;
	} NOUTBOX3I;

	typedef struct {
		NVEC2 m_vPoints[3];
		NLINMATRIX2 m_vMatrix;
		NLINMATRIX2 m_vInvMatrix;
	} NBARYCENTRICTRIANGLE;

}

#endif // __NMR_GEOMETRY
