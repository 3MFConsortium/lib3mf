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

UnitTest_BeamSets.cpp: Defines Unittests for the BeamSet class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class BeamSet : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			mesh = model->AddMeshObject();
			beamLattice = mesh->BeamLattice();

			sPosition p;
			p.m_Coordinates[0] = 0;
			p.m_Coordinates[1] = 0;
			p.m_Coordinates[2] = 0;
			mesh->AddVertex(p);
			
			p.m_Coordinates[1] = 1;
			mesh->AddVertex(p);

			p.m_Coordinates[2] = 1;
			mesh->AddVertex(p);

			// Preparations
			sBeam beam;
			beam.m_CapModes[0] = eBeamLatticeCapMode::HemiSphere;
			beam.m_CapModes[1] = eBeamLatticeCapMode::Butt;
			beam.m_Radii[0] = 1.5;
			beam.m_Radii[1] = 1.2;
			std::vector<sBeam> beams(3);
			for (int i = 0; i < 3; i++) {
				beam.m_Indices[0] = i;
				beam.m_Indices[1] = (i + 1) % 3;
				beams[i] = beam;
			}

			beamLattice->SetBeams(beams);

			beamLattice->SetBallOptions(eBeamLatticeBallMode::Mixed, 1.8);
			sBall ball;
			ball.m_Radius = 1.8;
			std::vector<sBall> balls(3);
			for (int i = 0; i < 3; i++) {
				ball.m_Index = i;
				balls[i] = ball;
			}

			beamLattice->SetBalls(balls);
		}
		virtual void TearDown() {
			model.reset();
		}
	
		PModel model;
		PMeshObject mesh;
		PBeamLattice beamLattice;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper BeamSet::wrapper;
	
	TEST_F(BeamSet, Name)
	{

		auto beamSet = beamLattice->AddBeamSet();
		std::string name = "ASDF";
		beamSet->SetName(name);
		ASSERT_EQ(beamSet->GetName(), name);
	}

	TEST_F(BeamSet, Identifier)
	{
		auto beamSet = beamLattice->AddBeamSet();
		std::string ident = "ASDF";
		beamSet->SetIdentifier(ident);
		ASSERT_EQ(beamSet->GetIdentifier(), ident);
	}

	TEST_F(BeamSet, UniqueIdentifier)
	{
		auto beamSet = beamLattice->AddBeamSet();
		std::string ident1 = "ASDF";
		std::string ident2 = "GHJKL";
		beamSet->SetIdentifier(ident1);

		beamSet = beamLattice->AddBeamSet();
		beamSet->SetIdentifier(ident2);

		beamSet = beamLattice->AddBeamSet();
		try {
			beamSet->SetIdentifier(ident1);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(BeamSet, ReadUniqueIdentifier)
	{
		std::string fName("BeamSet_Unique.3mf");
		auto model = wrapper->CreateModel();
		{
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + "/" + "BeamLattice" + "/" + fName);
			ASSERT_EQ(reader->GetWarningCount(), 0);
		}

		fName = "BeamSet_Not_Unique.3mf";
		model = wrapper->CreateModel();
		{
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + "/" + "BeamLattice" + "/" + fName);
			ASSERT_NE(reader->GetWarningCount(), 0);
		}
	}

	TEST_F(BeamSet, References)
	{
		auto beamSet = beamLattice->AddBeamSet();
		
		ASSERT_EQ(beamSet->GetReferenceCount(), 0);

		Lib3MF_uint32 nReferences = 2;
		std::vector<Lib3MF_uint32> references(nReferences);
		references[0] = 2;
		references[1] = 5;
		try {
			beamSet->SetReferences(references);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}

		references[1] = 1;
		beamSet->SetReferences(references);

		ASSERT_EQ(beamSet->GetReferenceCount(), nReferences);

		std::vector<Lib3MF_uint32> referencesOut;
		beamSet->GetReferences(referencesOut);
		ASSERT_EQ(beamSet->GetReferenceCount(), referencesOut.size());
		for (size_t i = 0; i < referencesOut.size(); i++) {
			ASSERT_EQ(referencesOut[i], references[i]);
		}

	}

	TEST_F(BeamSet, BallReferences)
	{
		auto beamSet = beamLattice->AddBeamSet();

		ASSERT_EQ(beamSet->GetBallReferenceCount(), 0);

		Lib3MF_uint32 nBallReferences = 2;
		std::vector<Lib3MF_uint32> ballReferences(nBallReferences);
		ballReferences[0] = 2;
		ballReferences[1] = 5;
		try {
			beamSet->SetBallReferences(ballReferences);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}

		ballReferences[1] = 1;
		beamSet->SetBallReferences(ballReferences);

		ASSERT_EQ(beamSet->GetBallReferenceCount(), nBallReferences);

		std::vector<Lib3MF_uint32> ballReferencesOut;
		beamSet->GetBallReferences(ballReferencesOut);
		ASSERT_EQ(beamSet->GetBallReferenceCount(), ballReferencesOut.size());
		for (size_t i = 0; i < ballReferencesOut.size(); i++) {
			ASSERT_EQ(ballReferencesOut[i], ballReferences[i]);
		}
	}

}
