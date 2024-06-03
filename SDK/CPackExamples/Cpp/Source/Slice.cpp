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

Slice.cpp : 3MF slice extension example

--*/

#include <iostream>
#include <string>
#include <cmath>
double const_pi() { return std::atan(1) * 4; }
#include <algorithm>

#include "lib3mf_implicit.hpp"

using namespace Lib3MF;


void printVersion(PWrapper wrapper) {
	Lib3MF_uint32 nMajor, nMinor, nMicro;
	wrapper->GetLibraryVersion(nMajor, nMinor, nMicro);
	std::cout << "lib3mf version = " << nMajor << "." << nMinor << "." << nMicro;
	std::string sReleaseInfo, sBuildInfo;
	if (wrapper->GetPrereleaseInformation(sReleaseInfo)) {
		std::cout << "-" << sReleaseInfo;
	}
	if (wrapper->GetBuildInformation(sBuildInfo)) {
		std::cout << "+" << sBuildInfo;
	}
	std::cout << std::endl;
}

// Utility functions to create vertices and beams
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


void SliceExample() {
	PWrapper wrapper = CWrapper::loadLibrary();
	
	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Slice example" << std::endl;
	printVersion(wrapper);
	std::cout << "------------------------------------------------------------------" << std::endl;

	PModel model = wrapper->CreateModel();

	PMeshObject meshObject = model->AddMeshObject();
	meshObject->SetName("Sliced Object [outbox]");

	// Create mesh structure of a cube
	std::vector<sLib3MFPosition> vertices(8);
	std::vector<sLib3MFTriangle> triangles(12);

	float fSizeX = 100.0f;
	float fSizeY = 100.0f;
	float fSizeZ = 300.0f;

	// Manually create vertices
	vertices[0] = fnCreateVertex(-fSizeX, -fSizeY, 0.0f);
	vertices[1] = fnCreateVertex(fSizeX, -fSizeY, 0.0f);
	vertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
	vertices[3] = fnCreateVertex(-fSizeX, fSizeY, 0.0f);
	vertices[4] = fnCreateVertex(-fSizeX, -fSizeY, fSizeZ);
	vertices[5] = fnCreateVertex(fSizeX, -fSizeY, fSizeZ);
	vertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
	vertices[7] = fnCreateVertex(-fSizeX, fSizeY, fSizeZ);

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

	// Set Geometry
	meshObject->SetGeometry(vertices, triangles);


	PSliceStack sliceStack = model->AddSliceStack(0.0);





	// Define an ellipse
	Lib3MF_uint32 nSliceVertices = 20;
	std::vector<sLib3MFPosition2D> origSliceVertices(nSliceVertices);
	for (Lib3MF_uint32 iSliceVertex = 0; iSliceVertex < nSliceVertices; iSliceVertex++) {
		double angle = 2 * (const_pi()* iSliceVertex) / nSliceVertices;
		origSliceVertices[iSliceVertex].m_Coordinates[0] = (Lib3MF_single)(fSizeX / 2 * std::cos(angle));
		origSliceVertices[iSliceVertex].m_Coordinates[1] = (Lib3MF_single)(fSizeY  *std::sin(angle));
	}

	std::vector<Lib3MF_uint32> polygonIndices(nSliceVertices + 1);
	for (Lib3MF_uint32 iPolygonIndex = 0; iPolygonIndex < polygonIndices.size(); iPolygonIndex++) {
		polygonIndices[iPolygonIndex] = iPolygonIndex % nSliceVertices;
	}

	Lib3MF_uint32 nSlices = 10;

	for (Lib3MF_uint32 iSlice = 0; iSlice < nSlices; iSlice++) {
		PSlice slice = sliceStack->AddSlice((iSlice + 1.0)*fSizeZ / nSlices);

		// Rotate the ellpise as we move up z
		double angle = 2 * (const_pi()*iSlice) / nSlices;
		std::vector<sLib3MFPosition2D> sliceVertices(nSliceVertices);
		for (Lib3MF_uint32 iSliceVertex = 0; iSliceVertex < nSliceVertices; iSliceVertex++) {
			double x = origSliceVertices[iSliceVertex].m_Coordinates[0];
			double y = origSliceVertices[iSliceVertex].m_Coordinates[1];
			sliceVertices[iSliceVertex].m_Coordinates[0] = (Lib3MF_single)(std::cos(angle)*x - std::sin(angle)*y);
			sliceVertices[iSliceVertex].m_Coordinates[1] = (Lib3MF_single)(std::sin(angle)*x + std::cos(angle)*y);
		}

		slice->SetVertices(sliceVertices);
		slice->AddPolygon(polygonIndices);
	}


	// Assign this slice stack to mesh..., the exact geometry of the part
	meshObject->AssignSliceStack(sliceStack.get());
	// which is not an exact representation of the sliced geometry
	meshObject->SetSlicesMeshResolution(eSlicesMeshResolution::Lowres);


	// Add build item
	model->AddBuildItem(meshObject.get(), wrapper->GetIdentityTransform());

	// Write file
	PWriter writer = model->QueryWriter("3mf");
	writer->WriteToFile("slice.3mf");

	std::cout << "done" << std::endl;
}

int main() {
	try {
		SliceExample();
	}
	catch (ELib3MFException &e) {
		std::cout << e.what() << std::endl;
		return e.getErrorCode();
	}
	return 0;
}
