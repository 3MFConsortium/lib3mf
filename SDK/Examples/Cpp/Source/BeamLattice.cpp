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

BeamLattice.cpp : 3MF beamlattice creation example

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

// Utility functions to create vertices and beams
sLib3MFPosition fnCreateVertex(float x, float y, float z)
{
	sLib3MFPosition result;
	result.m_Coordinates[0] = x;
	result.m_Coordinates[1] = y;
	result.m_Coordinates[2] = z;
	return result;
}

sLib3MFBeam fnCreateBeam(int v0, int v1, double r0, double r1, eLib3MFBeamLatticeCapMode c0, eLib3MFBeamLatticeCapMode c1)
{
	sLib3MFBeam result;
	result.m_Indices[0] = v0;
	result.m_Indices[1] = v1;
	result.m_Radii[0] = r0;
	result.m_Radii[1] = r1;
	result.m_CapModes[0].m_enum = c0;
	result.m_CapModes[1].m_enum = c1;
	return result;
}


void BeamLatticeExample() {
	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Beamlattice example" << std::endl;
	printVersion();
	std::cout << "------------------------------------------------------------------" << std::endl;

	PLib3MFModel model = CLib3MFWrapper::CreateModel();

	PLib3MFMeshObject meshObject = model->AddMeshObject();
	meshObject->SetName("Beamlattice");

	// Create mesh structure of a cube
	std::vector<sLib3MFPosition> vertices(8);
	std::vector<sLib3MFTriangle> triangles(0);
	std::vector<sLib3MFBeam> beams(12);

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

	// Manually create beams
	double r0 = 1.0;
	double r1 = 1.5;
	double r2 = 2.0;
	double r3 = 2.5;
	beams[0] = fnCreateBeam(2, 1, r0, r0, eBeamLatticeCapModeButt, eBeamLatticeCapModeButt);
	beams[1] = fnCreateBeam(0, 3, r0, r1, eBeamLatticeCapModeSphere, eBeamLatticeCapModeButt);
	beams[2] = fnCreateBeam(4, 5, r0, r2, eBeamLatticeCapModeSphere, eBeamLatticeCapModeButt);
	beams[3] = fnCreateBeam(6, 7, r0, r3, eBeamLatticeCapModeHemiSphere, eBeamLatticeCapModeButt);
	beams[4] = fnCreateBeam(0, 1, r1, r0, eBeamLatticeCapModeHemiSphere, eBeamLatticeCapModeButt);
	beams[5] = fnCreateBeam(5, 4, r1, r1, eBeamLatticeCapModeSphere, eBeamLatticeCapModeHemiSphere);
	beams[6] = fnCreateBeam(2, 3, r1, r2, eBeamLatticeCapModeSphere, eBeamLatticeCapModeSphere);
	beams[7] = fnCreateBeam(7, 6, r1, r3, eBeamLatticeCapModeButt, eBeamLatticeCapModeButt);
	beams[8] = fnCreateBeam(1, 2, r2, r2, eBeamLatticeCapModeButt, eBeamLatticeCapModeButt);
	beams[9] = fnCreateBeam(6, 5, r2, r3, eBeamLatticeCapModeHemiSphere, eBeamLatticeCapModeButt);
	beams[10] = fnCreateBeam(3, 0, r3, r0, eBeamLatticeCapModeButt, eBeamLatticeCapModeSphere);
	beams[11] = fnCreateBeam(4, 7, r3, r1, eBeamLatticeCapModeHemiSphere, eBeamLatticeCapModeHemiSphere);
	meshObject->SetGeometry(vertices, triangles);

	// Set beamlattice geometry and metadata
	PLib3MFBeamLattice beamLattice = meshObject->BeamLattice();
	beamLattice->SetBeams(beams);
	beamLattice->SetMinLength(0.005);

	PLib3MFBeamSet set = beamLattice->AddBeamSet();
	set->SetName("Special Beams");
	set->SetIdentifier("bs1");
	std::vector<Lib3MF_uint32> references = { 2,0,5 };
	set->SetReferences(references);

	// Add build item
	model->AddBuildItem(meshObject.get(), CLib3MFWrapper::GetIdentityTransform());

	// Write file
	PLib3MFWriter writer = model->QueryWriter("3mf");
	writer->WriteToFile("beamlattice.3mf");

	std::cout << "done" << std::endl;
}

int main() {
	try {
		BeamLatticeExample();
	}
	catch (ELib3MFException &e) {
		std::cout << e.what() << std::endl;
		return e.getErrorCode();
	}
	return 0;
}
