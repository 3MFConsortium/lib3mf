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

NMR_ModelTypes.h defines 3MF Model Type Definitions

--*/

#ifndef __NMR_MODELTYPES
#define __NMR_MODELTYPES

#include "Common/Platform/NMR_WinTypes.h"

#define MODELREADERCLASS_3MF "3mf"
#define MODELWRITERCLASS_3MF "3mf"

#define MODELREADERCLASS_STL "stl"
#define MODELWRITERCLASS_STL "stl"

#define MODEL_MAXSTRINGBUFFERLENGTH 1073741823 // (Safe margin for buffer overflows: 2^30 - 1)

namespace NMR {

#pragma pack (1)

	typedef struct {
		FLOAT m_fPosition[3];
	} MODELMESHVERTEX;

	typedef struct {
		DWORD m_nIndices[3];
	} MODELMESHTRIANGLE;

	typedef struct {
		FLOAT m_fFields[3][4];
	} MODELTRANSFORM;

	typedef struct {
		BYTE m_Red;
		BYTE m_Green;
		BYTE m_Blue;
		BYTE m_Alpha;
	} MODELMESHCOLOR_SRGB;

	typedef struct {
		DWORD m_nTextureID;
		FLOAT m_fU[3];
		FLOAT m_fV[3];
	} MODELMESHTEXTURE2D;

	typedef struct {
		MODELMESHCOLOR_SRGB m_Colors[3];
	} MODELMESH_TRIANGLECOLOR_SRGB;

#pragma pack()

	typedef DWORD ModelResourceID;
	typedef DWORD ModelResourceIndex;

	enum eModelUnit {
		MODELUNIT_MICROMETER = 0,
		MODELUNIT_MILLIMETER = 1,
		MODELUNIT_CENTIMETER = 2,
		MODELUNIT_INCH = 3,
		MODELUNIT_FOOT = 4,
		MODELUNIT_METER = 5
	};

	enum eModelExtension {
		MODELEXTENSION_CUSTOM = 0,
		MODELEXTENSION_PROPERTIES = 1,
		MODELEXTENSION_SLICES = 2,
		MODELEXTENSION_MULTIPART = 3,
		MODELEXTENSION_VOLUMETRIC = 4
	};

	enum eModelObjectType {
		MODELOBJECTTYPE_OTHER = 0,
		MODELOBJECTTYPE_MODEL = 1,
		MODELOBJECTTYPE_SUPPORT = 2
	};

	enum eModelPropertyType {
		MODELPROPERTYTYPE_NONE = 0,
		MODELPROPERTYTYPE_BASEMATERIALS = 1,
		MODELPROPERTYTYPE_COLOR = 2,
		MODELPROPERTYTYPE_TEXCOORD2D = 3,
		MODELPROPERTYTYPE_COMPOSITE = 4,
	};

	enum eModelTexture2DType {
		MODELTEXTURETYPE_UNKNOWN = 0,
		MODELTEXTURETYPE_PNG = 1,
		MODELTEXTURETYPE_JPEG = 2,
	};


}

#endif // __NMR_MODELTYPES
