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

	enum eModelBeamLatticeClipMode {
		MODELBEAMLATTICECLIPMODE_NONE = 0,
		MODELBEAMLATTICECLIPMODE_INSIDE = 1,
		MODELBEAMLATTICECLIPMODE_OUTSIDE = 2
	};

	enum eModelSlicesMeshResolution {
		MODELSLICESMESHRESOLUTION_FULL = 0,
		MODELSLICESMESHRESOLUTION_LOW = 1
	};

	enum eModelBeamLatticeCapMode {
		MODELBEAMLATTICECAPMODE_SPHERE = 0,
		MODELBEAMLATTICECAPMODE_HEMISPHERE = 1,
		MODELBEAMLATTICECAPMODE_BUTT = 2
	};

	enum eModelBeamLatticeBallMode {
		MODELBEAMLATTICEBALLMODE_NONE = 0,
		MODELBEAMLATTICEBALLMODE_MIXED = 1,
		MODELBEAMLATTICEBALLMODE_ALL = 2
	};

	typedef struct MODELMESHBEAM {
		DWORD m_nIndices[2] = {0,0};
		DOUBLE m_dRadius[2] = { 1.,1. };	// the radius of end 0 or 1 of this beam
		eModelBeamLatticeCapMode m_eCapMode[2];
		MODELMESHBEAM() {
			m_eCapMode[0] = MODELBEAMLATTICECAPMODE_SPHERE;
			m_eCapMode[1] = MODELBEAMLATTICECAPMODE_SPHERE;
		}
	} MODELMESHBEAM;

	typedef struct MODELMESHBALL {
		DWORD m_nIndex = 0;
		DOUBLE m_dRadius = 1.;	// the radius of this ball
		MODELMESHBALL() {
		}
	} MODELMESHBALL;

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
		DOUBLE m_dU;
		DOUBLE m_dV;
	} MODELTEXTURE2DCOORDINATE;

	typedef struct {
		MODELMESHCOLOR_SRGB m_Colors[3];
	} MODELMESH_TRIANGLECOLOR_SRGB;

#pragma pack()

	typedef DWORD ModelPropertyID;
	typedef DWORD ModelResourceID;	// Resource ID unique within a model, the id that you actually see as id in the xml
	typedef DWORD UniqueResourceID;	// Resource ID unique within a package, handled internally to allow access to resources across models
	typedef DWORD ModelResourceIndex;	//TODO: Add explanation

	enum eModelUnit {
		MODELUNIT_MICROMETER = 0,
		MODELUNIT_MILLIMETER = 1,
		MODELUNIT_CENTIMETER = 2,
		MODELUNIT_INCH = 3,
		MODELUNIT_FOOT = 4,
		MODELUNIT_METER = 5
	};

	enum eModelObjectType {
		MODELOBJECTTYPE_OTHER = 0,
		MODELOBJECTTYPE_MODEL = 1,
		MODELOBJECTTYPE_SUPPORT = 2,
		MODELOBJECTTYPE_SOLIDSUPPORT = 3,
		MODELOBJECTTYPE_SURFACE = 4
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

	enum eModelTextureTileStyle {
		MODELTEXTURETILESTYLE_WRAP = 0,
		MODELTEXTURETILESTYLE_MIRROR = 1,
		MODELTEXTURETILESTYLE_CLAMP = 2,
		MODELTEXTURETILESTYLE_NONE = 3
	};

	enum eModelTextureFilter {
		MODELTEXTUREFILTER_AUTO = 0,
		MODELTEXTUREFILTER_LINEAR = 1,
		MODELTEXTUREFILTER_NEAREST = 2
	};

	enum eModelBlendMethod {
		MODELBLENDMETHOD_MIX = 0,
		MODELBLENDMETHOD_MULTIPLY = 1,
		MODELBLENDMETHOD_MASK = 2
	};

	enum eModelColorChannel {
		MODELCOLORCHANNEL_RED = 0,
		MODELCOLORCHANNEL_GREEN = 1,
		MODELCOLORCHANNEL_BLUE = 2,
		MODELCOLORCHANNEL_ALPHA = 3
	};

	enum class eModelCompositionMethod {
		MODELCOMPOSITIONMETHOD_WEIGHTEDSUM = 0,
		MODELCOMPOSITIONMETHOD_MULTIPLY= 1,
		MODELCOMPOSITIONMETHOD_MIN = 2,
		MODELCOMPOSITIONMETHOD_MAX = 3,
		MODELCOMPOSITIONMETHOD_MASK = 4
	};

	enum class eModelCompositionSpace {
		MODELCOMPOSITIONSPACE_RAW = 0,
		MODELCOMPOSITIONSPACE_LINEARCOLOR = 1
	};

	typedef struct {
		FLOAT m_fPosition[2];
	} MODELSLICEVERTEX;

	typedef struct {
		DOUBLE m_dMixingRatio;
		ModelPropertyID m_nPropertyID;
	} MODELCOMPOSITECONSTITUENT;

	typedef struct {
		UniqueResourceID m_nUniqueResourceID;
		eModelBlendMethod m_nMethod;
	} MODELMULTIPROPERTYLAYER;
}

#endif // __NMR_MODELTYPES
