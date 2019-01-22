/*++

Copyright (C) 2018 3MF Consortium

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

UnitTest_BeamLattice.cpp: Defines Unittests for the BeamLattice class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class BeamLattice : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			otherMesh = model->AddMeshObject();
			mesh = model->AddMeshObject();
			beamLattice = mesh->BeamLattice();
			meshAfter = model->AddMeshObject();

			sLib3MFPosition p;
			p.m_coordinates[0] = 0;
			p.m_coordinates[1] = 0;
			p.m_coordinates[2] = 0;
			mesh->AddVertex(p);
			
			p.m_coordinates[1] = 1;
			mesh->AddVertex(p);

			p.m_coordinates[2] = 1;
			mesh->AddVertex(p);
		}
		virtual void TearDown() {
			model.reset();
		}
	
		static PLib3MFModel model;
		static PLib3MFMeshObject mesh;
		static PLib3MFMeshObject otherMesh;
		static PLib3MFMeshObject meshAfter;
		static PLib3MFBeamLattice beamLattice;
	};

	PLib3MFModel BeamLattice::model;
	PLib3MFMeshObject BeamLattice::mesh;
	PLib3MFMeshObject BeamLattice::otherMesh;
	PLib3MFMeshObject BeamLattice::meshAfter;
	PLib3MFBeamLattice BeamLattice::beamLattice;
	

	TEST_F(BeamLattice, MinLength)
	{
		double minLength = 1.2;
		beamLattice->SetMinLength(minLength);

		ASSERT_DOUBLE_EQ(beamLattice->GetMinLength(), minLength);
	}

	TEST_F(BeamLattice, Clipping)
	{
		eLib3MFBeamLatticeClipMode eClipMode;
		Lib3MF_uint32 nClippingResource;
		beamLattice->GetClipping(eClipMode, nClippingResource);
		ASSERT_EQ(eClipMode, eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeNoClipMode);

		beamLattice->SetClipping(eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeInside, otherMesh->GetResourceID());
		beamLattice->GetClipping(eClipMode, nClippingResource);
		ASSERT_EQ(eClipMode, eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeInside);
		ASSERT_EQ(nClippingResource, otherMesh->GetResourceID());

		try {
			beamLattice->SetClipping(eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeOutside, meshAfter->GetResourceID());
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException& e) {
			ASSERT_EQ(e.getErrorCode(), LIB3MF_ERROR_INVALIDPARAM);
		}
	}

	TEST_F(BeamLattice, Representation)
	{
		Lib3MF_uint32 nRepresentationResource;
		ASSERT_FALSE(beamLattice->GetRepresentation(nRepresentationResource));

		beamLattice->SetRepresentation(otherMesh->GetResourceID());
		ASSERT_TRUE(beamLattice->GetRepresentation(nRepresentationResource));
		ASSERT_EQ(nRepresentationResource, otherMesh->GetResourceID());

		try {
			beamLattice->SetRepresentation(meshAfter->GetResourceID());
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException& e) {
			ASSERT_EQ(e.getErrorCode(), LIB3MF_ERROR_INVALIDPARAM);
		}
	}


	TEST_F(BeamLattice, GeometryShouldFail)
	{
		sLib3MFBeam beam;
		beam.m_CapModes[0].m_code = eLib3MFBeamLatticeCapMode::eBeamLatticeCapModeHemiSphere;
		beam.m_CapModes[1].m_code = eLib3MFBeamLatticeCapMode::eBeamLatticeCapModeButt;

		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 0;
		beam.m_Radii[0] = 1.5;
		beam.m_Radii[1] = 1.2;
		try {
			beamLattice->AddBeam(beam);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}

		beam.m_CapModes[0].m_code = eLib3MFBeamLatticeCapMode::eBeamLatticeCapModeHemiSphere;
		beam.m_CapModes[1].m_code = eLib3MFBeamLatticeCapMode::eBeamLatticeCapModeButt;
		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 1;
		beam.m_Radii[0] = 1.5;
		beam.m_Radii[1] = 1.2;
		// That one works
		beamLattice->AddBeam(beam);

		// That one will fail again
		beam.m_Radii[1] = -1.2;
		try {
			beamLattice->SetBeam(0, beam);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(BeamLattice, Geometry)
	{
		ASSERT_EQ(beamLattice->GetBeamCount(), 0);
		sLib3MFBeam beam;
		beam.m_CapModes[0].m_code = eLib3MFBeamLatticeCapMode::eBeamLatticeCapModeHemiSphere;
		beam.m_CapModes[1].m_code = eLib3MFBeamLatticeCapMode::eBeamLatticeCapModeButt;
		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 1;
		beam.m_Radii[0] = 1.5;
		beam.m_Radii[1] = 1.2;

		beamLattice->AddBeam(beam);

		beam.m_Indices[0] = 1;
		beam.m_Indices[1] = 2;
		beamLattice->AddBeam(beam);

		beam.m_Indices[0] = 2;
		beam.m_Indices[1] = 0;
		beamLattice->AddBeam(beam);
		ASSERT_EQ(beamLattice->GetBeamCount(), 3);

		auto outBeam = beamLattice->GetBeam(2);
		for (int i = 0; i < 2; i++) {
			ASSERT_EQ(outBeam.m_CapModes[i].m_code, beam.m_CapModes[i].m_code);
			ASSERT_EQ(outBeam.m_Indices[i], beam.m_Indices[i]);
			ASSERT_DOUBLE_EQ(outBeam.m_Radii[i], beam.m_Radii[i]);
		}

		beam.m_Radii[0] = 4;
		beam.m_Radii[1] = 3.2;
		beamLattice->SetBeam(2, beam);
	}

	TEST_F(BeamLattice, GeometryBulk)
	{
		// Preparations
		sLib3MFBeam beam;
		beam.m_CapModes[0].m_code = eLib3MFBeamLatticeCapMode::eBeamLatticeCapModeHemiSphere;
		beam.m_CapModes[1].m_code = eLib3MFBeamLatticeCapMode::eBeamLatticeCapModeButt;
		beam.m_Radii[0] = 1.5;
		beam.m_Radii[1] = 1.2;
		std::vector<sLib3MFBeam> beams(3);
		for (int i = 0; i < 3; i++) {
			beam.m_Indices[0] = i;
			beam.m_Indices[1] = (i+1)%3;
			beams[i] = beam;
		}

		beamLattice->SetBeams(beams);
		std::vector<sLib3MFBeam> outBeams;

		beamLattice->GetBeams(outBeams);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 2; j++) {
				ASSERT_EQ(outBeams[i].m_CapModes[j].m_code, beams[i].m_CapModes[j].m_code);
				ASSERT_EQ(outBeams[i].m_Indices[j], beams[i].m_Indices[j]);
				ASSERT_DOUBLE_EQ(outBeams[i].m_Radii[j], beams[i].m_Radii[j]);
			}
		}

		// modify beams to fail:
		beams[0].m_Indices[0] = 12;
		try {
			beamLattice->SetBeams(beams);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(BeamLattice, BeamSet)
	{
		auto beamSet = beamLattice->AddBeamSet();

		ASSERT_EQ(beamLattice->GetBeamSetCount(), 1);

		auto beamSetOut = beamLattice->GetBeamSet(0);
	}

	TEST_F(BeamLattice, Read)
	{

	}

	TEST_F(BeamLattice, WriteRead)
	{

	}
}
