/*++

Copyright (C) 2019 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MICROSOFT AND/OR NETFABB BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

ColorCube.cpp : 3MF Color Cube creation example

--*/

#include <iostream>
#include <string>
#include <algorithm>

#include "lib3mf.hpp"

using namespace Lib3MF;


void printVersion() {
	Lib3MF_uint32 nMajor, nMinor, nMicro;
	std::string sReleaseInfo, sBuildInfo;
	CLib3MFWrapper::GetLibraryVersion(nMajor, nMinor, nMicro, sReleaseInfo, sBuildInfo);
	std::cout << "Lib3MF version = " << nMajor << "." << nMinor << "." << nMicro;
	if (!sReleaseInfo.empty()) {
		std::cout << "-" << sReleaseInfo;
	}
	if (!sBuildInfo.empty()) {
		std::cout << "+" << sBuildInfo;
	}
	std::cout << std::endl;
}

// Utility functions to create vertices and triangles
sLib3MFPosition fnCreateVertex(float x, float y, float z)
{
	sLib3MFPosition result;
	result.m_Coordinates[0] = x;
	result.m_Coordinates[1] = y;
	result.m_Coordinates[2] = z;
	return result;
}

sLib3MFTriangle fnCreateTriangle(int v0, int v1, int v2)
{
	sLib3MFTriangle result;
	result.m_Indices[0] = v0;
	result.m_Indices[1] = v1;
	result.m_Indices[2] = v2;
	return result;
}

sLib3MFTriangleProperties fnCreateTriangleColor(PLib3MFColorGroup colorGroup, Lib3MF_uint32 colorID1, Lib3MF_uint32 colorID2, Lib3MF_uint32 colorID3)
{
	sLib3MFTriangleProperties sTriangleProperty;
	sTriangleProperty.m_ResourceID = colorGroup->GetResourceID();
	sTriangleProperty.m_PropertyIDs[0] = colorID1;
	sTriangleProperty.m_PropertyIDs[1] = colorID2;
	sTriangleProperty.m_PropertyIDs[2] = colorID3;
	return sTriangleProperty;
}


void CubeExample() {
	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Color Cube example" << std::endl;
	printVersion();
	std::cout << "------------------------------------------------------------------" << std::endl;

	PLib3MFModel model = CLib3MFWrapper::CreateModel();

	PLib3MFMeshObject meshObject = model->AddMeshObject();
	meshObject->SetName("Colored Box");

	// Create mesh structure of a cube
	std::vector<sLib3MFPosition> vertices(8);
	std::vector<sLib3MFTriangle> triangles(12);

	float fSizeX = 100.0f;
	float fSizeY = 200.0f;
	float fSizeZ = 300.0f;

	// Manually create vertices
	vertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
	vertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
	vertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
	vertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
	vertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
	vertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
	vertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
	vertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);

	// Manually create triangles
	triangles[0] = fnCreateTriangle(2, 1, 0);
	triangles[1] = fnCreateTriangle(0, 3, 2);
	triangles[2] = fnCreateTriangle(4, 5, 6);
	triangles[3] = fnCreateTriangle(6, 7, 4);
	triangles[4] = fnCreateTriangle(0, 1, 5);
	triangles[5] = fnCreateTriangle(5, 4, 0);
	triangles[6] = fnCreateTriangle(2, 3, 7);
	triangles[7] = fnCreateTriangle(7, 6, 2);
	triangles[8] = fnCreateTriangle(1, 2, 6);
	triangles[9] = fnCreateTriangle(6, 5, 1);
	triangles[10] = fnCreateTriangle(3, 0, 4);
	triangles[11] = fnCreateTriangle(4, 7, 3);

	meshObject->SetGeometry(vertices, triangles);

	// define colors
	PLib3MFColorGroup colorGroup = model->AddColorGroup();
	Lib3MF_uint32 idRed = colorGroup->AddColor(CLib3MFWrapper::RGBAToColor(255, 0, 0, 255));
	Lib3MF_uint32 idGreen = colorGroup->AddColor(CLib3MFWrapper::RGBAToColor(0, 255, 0, 255));
	Lib3MF_uint32 idBlue = colorGroup->AddColor(CLib3MFWrapper::RGBAToColor(0, 0, 255, 255));
	Lib3MF_uint32 idOrange = colorGroup->AddColor(CLib3MFWrapper::RGBAToColor(255, 128, 0, 255));
	Lib3MF_uint32 idYellow = colorGroup->AddColor(CLib3MFWrapper::RGBAToColor(255, 255, 0, 255));

	sLib3MFTriangleProperties sTriangleColorRed = fnCreateTriangleColor(colorGroup, idRed, idRed, idRed);
	sLib3MFTriangleProperties sTriangleColorGreen = fnCreateTriangleColor(colorGroup, idGreen, idGreen, idGreen);
	sLib3MFTriangleProperties sTriangleColorBlue = fnCreateTriangleColor(colorGroup, idBlue, idBlue, idBlue);
	
	sLib3MFTriangleProperties sTriangleColor1 = fnCreateTriangleColor(colorGroup, idOrange, idRed, idYellow);
	sLib3MFTriangleProperties sTriangleColor2 = fnCreateTriangleColor(colorGroup, idYellow, idGreen, idOrange);
	
	// One-colored Triangles
	meshObject->SetTriangleProperties(0, sTriangleColorRed);
	meshObject->SetTriangleProperties(1, sTriangleColorRed);
	meshObject->SetTriangleProperties(2, sTriangleColorGreen);
	meshObject->SetTriangleProperties(3, sTriangleColorGreen);
	meshObject->SetTriangleProperties(4, sTriangleColorBlue);
	meshObject->SetTriangleProperties(5, sTriangleColorBlue);
	// Gradient-colored Triangles
	meshObject->SetTriangleProperties(6, sTriangleColor1);
	meshObject->SetTriangleProperties(7, sTriangleColor2);
	meshObject->SetTriangleProperties(8, sTriangleColor1);
	meshObject->SetTriangleProperties(9, sTriangleColor2);
	meshObject->SetTriangleProperties(10, sTriangleColor1);
	meshObject->SetTriangleProperties(11, sTriangleColor2);

	// Add build item
	model->AddBuildItem(meshObject.get(), CLib3MFWrapper::GetIdentityTransform());

	PLib3MFWriter writer = model->QueryWriter("3mf");
	writer->WriteToFile("colorcube.3mf");

	std::cout << "done" << std::endl;
}

int main() {
	
	try {
		CubeExample();
	}
	catch (ELib3MFException &e) {
		std::cout << e.what() << std::endl;
		return e.getErrorCode();
	}
	return 0;
}
