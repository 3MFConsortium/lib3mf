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

NMR_ModelConstants_Slices.h defines the 3MF Model Constants of the slice extension.

--*/

#ifndef __NMR_MESHINFORMATION_SLICES
#define __NMR_MESHINFORMATION_SLICES

#define XML_3MF_ELEMENT_SLICESTACKRESOURCE "slicestack"
#define XML_3MF_ELEMENT_SLICEREFRESOURCE "sliceref"
#define XML_3MF_ELEMENT_SLICE "slice"
#define XML_3MF_ELEMENT_SLICEVERTICES "vertices"
#define XML_3MF_ELEMENT_SLICEVERTEX "vertex"
#define XML_3MF_ELEMENT_SLICEPOLYGON "polygon"
#define XML_3MF_ELEMENT_SLICESEGMENT "segment"

#define XML_3MF_ATTRIBUTE_SLICESTACKZBOTTOM "zbottom"
#define XML_3MF_ATTRIBUTE_SLICESTACKID "id"
#define XML_3MF_ATTRIBUTE_SLICEZTOP "ztop"
#define XML_3MF_ATTRIBUTE_SLICEVERTEX_X "x"
#define XML_3MF_ATTRIBUTE_SLICEVERTEX_Y "y"
#define XML_3MF_ATTRIBUTE_SLICEPOLYGON_STARTV "startv"
#define XML_3MF_ATTRIBUTE_SLICESEGMENT_V2 "v2"

#define XML_3MF_ATTRIBUTE_SLICEREF_ID "slicestackid"
#define XML_3MF_ATTRIBUTE_SLICEREF_PATH "slicepath"

#endif

