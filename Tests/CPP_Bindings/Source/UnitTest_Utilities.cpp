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

UnitTest_Utilities.cpp: Implementtion of Utilities for UnitTests

--*/


#include "UnitTest_Utilities.h"

void fnCreateBox(std::vector<Lib3MF::sPosition> &vctVertices, std::vector<Lib3MF::sTriangle> &vctTriangles) {
	float fSizeX = 100.0f;
	float fSizeY = 100.0f;
	float fSizeZ = 100.0f;

	vctVertices.resize(8);
	vctTriangles.resize(12);

	// Manually create vertices
	vctVertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
	vctVertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
	vctVertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
	vctVertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
	vctVertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
	vctVertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
	vctVertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
	vctVertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);


	// Manually create triangles
	vctTriangles[0] = fnCreateTriangle(2, 1, 0);
	vctTriangles[1] = fnCreateTriangle(0, 3, 2);
	vctTriangles[2] = fnCreateTriangle(4, 5, 6);
	vctTriangles[3] = fnCreateTriangle(6, 7, 4);
	vctTriangles[4] = fnCreateTriangle(0, 1, 5);
	vctTriangles[5] = fnCreateTriangle(5, 4, 0);
	vctTriangles[6] = fnCreateTriangle(2, 3, 7);
	vctTriangles[7] = fnCreateTriangle(7, 6, 2);
	vctTriangles[8] = fnCreateTriangle(1, 2, 6);
	vctTriangles[9] = fnCreateTriangle(6, 5, 1);
	vctTriangles[10] = fnCreateTriangle(3, 0, 4);
	vctTriangles[11] = fnCreateTriangle(4, 7, 3);
}

