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

NMR_MeshTypes.h defines the basic datastructures for the mesh CMesh.

The  mesh has nodes and faces,  which are defined in this file.
In addition, some constants are defined here.
--*/

#ifndef __NMR_MESHTYPES
#define __NMR_MESHTYPES

#include "Common/NMR_Types.h" 
#include "Common/NMR_Local.h" 
#include "Common/Math/NMR_Geometry.h" 

#define NMR_MESH_MAXNODECOUNT 2147483646
#define NMR_MESH_MAXEDGECOUNT 2147483646
#define NMR_MESH_MAXFACECOUNT 2147483646
#define NMR_MESH_MAXCOORDINATE 1000000000.0f

#define NMR_MESH_NODEBLOCKCOUNT 256
#define NMR_MESH_EDGEBLOCKCOUNT 256
#define NMR_MESH_FACEBLOCKCOUNT 256
#define NMR_MESH_NODEEDGELINKBLOCKCOUNT 256

namespace NMR {

	typedef struct {
		nfInt32 m_index;
		NVEC3 m_position;
	} MESHNODE;

	typedef struct {
		nfInt32 m_index;
		nfInt32 m_nodeindices[3];
	} MESHFACE;

}

#endif // __NMR_MESHTYPES
