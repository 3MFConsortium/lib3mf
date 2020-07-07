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

UnitTest_BeamLattice.cpp: Defines Unittests for the BeamLattice class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class BeamLattice : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();
			otherMesh = model->AddMeshObject();
			mesh = model->AddMeshObject();
			beamLattice = mesh->BeamLattice();
			meshAfter = model->AddMeshObject();

			sPosition p;
			p.m_Coordinates[0] = 0;
			p.m_Coordinates[1] = 0;
			p.m_Coordinates[2] = 0;
			mesh->AddVertex(p);
			
			p.m_Coordinates[1] = 1;
			mesh->AddVertex(p);

			p.m_Coordinates[2] = 1;
			mesh->AddVertex(p);
		}
		virtual void TearDown() {
			model.reset();
		}
	
		PModel model;
		PMeshObject mesh;
		PMeshObject otherMesh;
		PMeshObject meshAfter;
		PBeamLattice beamLattice;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
	public:
		static PWrapper wrapper;
	};
	PWrapper BeamLattice::wrapper;
	

	TEST_F(BeamLattice, MinLength)
	{
		double minLength = 1.2;
		beamLattice->SetMinLength(minLength);

		ASSERT_DOUBLE_EQ(beamLattice->GetMinLength(), minLength);
	}

	TEST_F(BeamLattice, Clipping)
	{
		eBeamLatticeClipMode eClipMode;
		Lib3MF_uint32 nClippingResource;
		beamLattice->GetClipping(eClipMode, nClippingResource);
		ASSERT_EQ(eClipMode, eBeamLatticeClipMode::NoClipMode);

		beamLattice->SetClipping(eBeamLatticeClipMode::Inside, otherMesh->GetResourceID());
		beamLattice->GetClipping(eClipMode, nClippingResource);
		ASSERT_EQ(eClipMode, eBeamLatticeClipMode::Inside);
		ASSERT_EQ(nClippingResource, otherMesh->GetResourceID());

		try {
			beamLattice->SetClipping(eBeamLatticeClipMode::Outside, meshAfter->GetResourceID());
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


		sBeam beam;
		beam.m_Radii[0] = 1.0;
		beam.m_Radii[1] = 1.0;
		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 1;
		beamLattice->AddBeam(beam);

		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->SetStrictModeActive(true);
		reader->ReadFromBuffer(buffer);

		auto meshObjects = readModel->GetMeshObjects();
		PLib3MFMeshObject firstMesh;
		while (meshObjects->MoveNext()) {
			if (!firstMesh.get()) {
				firstMesh = meshObjects->GetCurrentMeshObject();
			}
			else {
				auto meshObject = meshObjects->GetCurrentMeshObject();
				auto readBeamLattice = meshObject->BeamLattice();
				Lib3MF_uint32 nRepID = 0;
				if (readBeamLattice->GetRepresentation(nRepID)) {
					ASSERT_EQ(firstMesh->GetResourceID(), nRepID);
					return;
				}
			}
		}
		ASSERT_FALSE(true);
	}

	TEST_F(BeamLattice, BallMode)
	{
		beamLattice->SetBallOptions(eBeamLatticeBallMode::None, 1.2);

		sBeam beam;
		beam.m_Radii[0] = 1.0;
		beam.m_Radii[1] = 1.0;
		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 1;
		beamLattice->AddBeam(beam);
		beam.m_Indices[0] = 1;
		beam.m_Indices[1] = 2;
		beamLattice->AddBeam(beam);
		ASSERT_EQ(beamLattice->GetBallCount(), 0);

		beamLattice->SetBallOptions(eBeamLatticeBallMode::Mixed, 1.2);
		sBall ball;

		ball.m_Index = 1;
		ball.m_Radius = 1.5;
		beamLattice->AddBall(ball);
		ASSERT_EQ(beamLattice->GetBallCount(), 1);

		beamLattice->SetBallOptions(eBeamLatticeBallMode::All, 1.2);
		ASSERT_EQ(beamLattice->GetBallCount(), mesh->GetVertexCount());

		// Ball order can change between platforms
		std::map<int, double> ballMap;

		for (Lib3MF_uint32 iBall = 0; iBall < beamLattice->GetBallCount(); iBall++) {
			auto outBall = beamLattice->GetBall(iBall);
			ballMap[outBall.m_Index] = outBall.m_Radius;
		}

		ASSERT_DOUBLE_EQ(ballMap[0], 1.2);
		ASSERT_DOUBLE_EQ(ballMap[1], ball.m_Radius);
		ASSERT_DOUBLE_EQ(ballMap[2], 1.2);

		ball.m_Index = 2;
		ball.m_Radius = 3.1;
		beamLattice->AddBall(ball);

		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->SetStrictModeActive(true);
		reader->ReadFromBuffer(buffer);

		auto meshObjects = readModel->GetMeshObjects();
		PLib3MFMeshObject firstMesh;
		while (meshObjects->MoveNext()) {
			if (!firstMesh.get()) {
				firstMesh = meshObjects->GetCurrentMeshObject();
			}
			else {
				auto meshObject = meshObjects->GetCurrentMeshObject();
				auto readBeamLattice = meshObject->BeamLattice();
				eBeamLatticeBallMode ballMode;
				Lib3MF_double defaultBallRadius;
				readBeamLattice->GetBallOptions(ballMode, defaultBallRadius);
				ASSERT_EQ(ballMode, eBeamLatticeBallMode::All);

				ballMap.clear();
				for (Lib3MF_uint32 iBall = 0; iBall < beamLattice->GetBallCount(); iBall++) {
					auto outBall = beamLattice->GetBall(iBall);
					ballMap[outBall.m_Index] = outBall.m_Radius;
				}

				ASSERT_DOUBLE_EQ(ballMap[0], 1.2);
				ASSERT_DOUBLE_EQ(ballMap[1], 1.5);
				ASSERT_DOUBLE_EQ(ballMap[2], ball.m_Radius);
				return;
			}
		}

	}

	TEST_F(BeamLattice, BallModeAll)
	{
		beamLattice->SetBallOptions(eBeamLatticeBallMode::All, 1.2);

		sBeam beam;

		beam.m_Radii[0] = 1.0;
		beam.m_Radii[1] = 1.0;
		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 2;
		beamLattice->AddBeam(beam);

		ASSERT_EQ(beamLattice->GetBallCount(), 2);

		std::vector<sBall> outBalls;
		beamLattice->GetBalls(outBalls);

		// Ball order can change between platforms
		std::map<int, double> ballMap;

		for (auto outBall : outBalls) {
			ballMap[outBall.m_Index] = outBall.m_Radius;
		}

		ASSERT_EQ(ballMap.size(), beamLattice->GetBallCount());
		ASSERT_DOUBLE_EQ(ballMap[2], 1.2);
	}

	TEST_F(BeamLattice, GeometryShouldFail)
	{
		sBeam beam;
		beam.m_CapModes[0] = eBeamLatticeCapMode::HemiSphere;
		beam.m_CapModes[1] = eBeamLatticeCapMode::Butt;

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

		beam.m_CapModes[0] = eBeamLatticeCapMode::HemiSphere;
		beam.m_CapModes[1] = eBeamLatticeCapMode::Butt;
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

		//Invalid ball options, should fail
		try {
			beamLattice->SetBallOptions(eBeamLatticeBallMode::All, 0.0);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}

		beamLattice->SetBallOptions(eBeamLatticeBallMode::All, 1.6);

		sBall ball;
		ball.m_Index = 0;
		ball.m_Radius = 2.2;
		// This one works
		beamLattice->AddBall(ball);

		ball.m_Radius = -2.2;
		try {
			beamLattice->SetBall(0, ball);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}

		ball.m_Index = 2;
		ball.m_Radius = 2.2;
		try {
			beamLattice->AddBall(ball);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(BeamLattice, Geometry)
	{
		ASSERT_EQ(beamLattice->GetBeamCount(), 0);
		sBeam beam;
		beam.m_CapModes[0] = eBeamLatticeCapMode::HemiSphere;
		beam.m_CapModes[1] = eBeamLatticeCapMode::Butt;
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
			ASSERT_EQ(outBeam.m_CapModes[i], beam.m_CapModes[i]);
			ASSERT_EQ(outBeam.m_Indices[i], beam.m_Indices[i]);
			ASSERT_DOUBLE_EQ(outBeam.m_Radii[i], beam.m_Radii[i]);
		}

		beam.m_Radii[0] = 4;
		beam.m_Radii[1] = 3.2;
		beamLattice->SetBeam(2, beam);

		beamLattice->SetBallOptions(eBeamLatticeBallMode::Mixed, 1.2);

		ASSERT_EQ(beamLattice->GetBallCount(), 0);
		sBall ball;

		ball.m_Index = 0;
		ball.m_Radius = 2.0;
		beamLattice->AddBall(ball);

		ball.m_Index = 1;
		ball.m_Radius = 2.4;
		beamLattice->AddBall(ball);
		
		ball.m_Index = 2;
		ball.m_Radius = 1.8;
		beamLattice->AddBall(ball);
		ASSERT_EQ(beamLattice->GetBallCount(), 3);

		auto outBall = beamLattice->GetBall(2);
		ASSERT_EQ(outBall.m_Index, ball.m_Index);
		ASSERT_DOUBLE_EQ(outBall.m_Radius, ball.m_Radius);
	}

	TEST_F(BeamLattice, GeometryBulk)
	{
		// Preparations
		sBeam beam;
		beam.m_CapModes[0] = eBeamLatticeCapMode::HemiSphere;
		beam.m_CapModes[1] = eBeamLatticeCapMode::Butt;
		beam.m_Radii[0] = 1.5;
		beam.m_Radii[1] = 1.2;
		std::vector<sBeam> beams(3);
		for (int i = 0; i < 3; i++) {
			beam.m_Indices[0] = i;
			beam.m_Indices[1] = (i+1)%3;
			beams[i] = beam;
		}

		beamLattice->SetBeams(beams);
		std::vector<sBeam> outBeams;

		beamLattice->GetBeams(outBeams);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 2; j++) {
				ASSERT_EQ(outBeams[i].m_CapModes[j], beams[i].m_CapModes[j]);
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

		// fix beams for adding balls
		beams[0].m_Indices[0] = 0;
		beamLattice->SetBeams(beams);

		beamLattice->SetBallOptions(eBeamLatticeBallMode::All, 1.8);

		sBall ball;
		std::vector<sBall> balls(2);
		ball.m_Index = 1;
		ball.m_Radius = 1.9;
		balls[0] = ball;
		ball.m_Index = 2;
		ball.m_Radius = 2.1;
		balls[1] = ball;

		beamLattice->SetBalls(balls);

		std::vector<sBall> outBalls;
		beamLattice->GetBalls(outBalls);

		// Ball order can change between platforms
		std::map<int, double> ballMap;

		for (auto outBall : outBalls) {
			ballMap[outBall.m_Index] = outBall.m_Radius;
		}

		ASSERT_DOUBLE_EQ(ballMap[0], 1.8);

		for (int i = 1; i < 3; i++) {
			ASSERT_DOUBLE_EQ(ballMap[i], balls[i-1].m_Radius);
		}

		// modify balls to fail:
		balls[0].m_Index = 12;
		try {
			beamLattice->SetBalls(balls);
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

	TEST_F(BeamLattice, ObjectType1)
	{
		sBeam beam;
		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 1;
		beam.m_Radii[0] = 1.5;
		beam.m_Radii[1] = 1.2;
		beam.m_CapModes[0] = eBeamLatticeCapMode::Sphere;
		beam.m_CapModes[1] = eBeamLatticeCapMode::Sphere;

		mesh->BeamLattice()->AddBeam(beam);

		mesh->SetType(eObjectType::Model);
		mesh->SetType(eObjectType::SolidSupport);

		try {
			mesh->SetType(eObjectType::Support);
			ASSERT_FALSE(true) << "Could set eObjectTypeSupport";
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
		try {
			mesh->SetType(eObjectType::Other);
			ASSERT_FALSE(true) << "Could set eObjectTypeOther";
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(BeamLattice, ObjectType2)
	{
		sBeam beam;
		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 1;
		beam.m_Radii[0] = 1.5;
		beam.m_Radii[1] = 1.2;
		beam.m_CapModes[0] = eBeamLatticeCapMode::Sphere;
		beam.m_CapModes[1] = eBeamLatticeCapMode::Sphere;

		mesh->SetType(eObjectType::Support);

		auto beamLattice = mesh->BeamLattice();
		try {
			beamLattice->AddBeam(beam);
			ASSERT_FALSE(true) << "Could add beam on eObjectTypeSupport";
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(BeamLattice, Read_Attributes_Positive)
	{
		std::string fName("Box_Attributes_Positive.3mf");
		auto model = wrapper->CreateModel();
		{
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + "/" + "BeamLattice" + "/" + fName);
			ASSERT_EQ(reader->GetWarningCount(), 0);
		}
	}

	TEST_F(BeamLattice, Read_Box_Simple)
	{
		std::string fName("Box_Simple.3mf");
		auto model = wrapper->CreateModel();
		{
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + "/" + "BeamLattice" + "/" + fName);
			ASSERT_EQ(reader->GetWarningCount(), 0);
		}

		auto meshObjects = model->GetMeshObjects();
		int nIndex = 0;
		while (meshObjects->MoveNext()) {
			auto modelResource = meshObjects->GetCurrent();
			auto meshObject = model->GetMeshObjectByID(modelResource->GetResourceID());
			auto beamLattice = meshObject->BeamLattice();
			if (beamLattice->GetBeamCount() > 0) // that's the mesh with the beams
			{
				const double dEps = 1e-7;
				ASSERT_EQ(beamLattice->GetBeamCount(), 12);
				sBeam beam = beamLattice->GetBeam(0);
				ASSERT_EQ(beam.m_Indices[0], 0);
				ASSERT_EQ(beam.m_Indices[1], 1);
				EXPECT_NEAR(beam.m_Radii[0], 1., dEps);
				EXPECT_NEAR(beam.m_Radii[1], 1., dEps);

				beam = beamLattice->GetBeam(1);
				ASSERT_EQ(beam.m_Indices[0], 0);
				ASSERT_EQ(beam.m_Indices[1], 3);
				EXPECT_NEAR(beam.m_Radii[0], 3.1, dEps);
				EXPECT_NEAR(beam.m_Radii[1], 3.2, dEps);

				beam = beamLattice->GetBeam(10);
				ASSERT_EQ(beam.m_Indices[0], 5);
				ASSERT_EQ(beam.m_Indices[1], 6);
				EXPECT_NEAR(beam.m_Radii[0], 10., dEps);
				EXPECT_NEAR(beam.m_Radii[1], 15., dEps);

				ASSERT_DOUBLE_EQ(beamLattice->GetMinLength(), 0.01);
			}
			nIndex++;
		}
	}

	void Read_Attributes_Negative(std::string fName)
	{
		auto model = BeamLattice::wrapper->CreateModel();

		{
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + "/" + "BeamLattice" + "/" + fName);
			ASSERT_EQ(reader->GetWarningCount(), 1);
			Lib3MF_uint32 nErrorCode;
			std::string warning = reader->GetWarning(0, nErrorCode);
		}

		std::vector<eBeamLatticeClipMode> vClipModes;
		std::vector<Lib3MF_uint32> vClipResourceIDs;
		std::vector<Lib3MF_uint32> vRepresentationResourceIDs;
		if (fName == "Box_Attributes_Negative_1.3mf") {
			vClipModes.push_back(eBeamLatticeClipMode::NoClipMode);
			vClipModes.push_back(eBeamLatticeClipMode::NoClipMode);
			vClipResourceIDs.push_back(0);
			vClipResourceIDs.push_back(0);
			vRepresentationResourceIDs.push_back(0);
			vRepresentationResourceIDs.push_back(0);
		}
		if (fName == "Box_Attributes_Negative_2.3mf") {
			vClipModes.push_back(eBeamLatticeClipMode::NoClipMode);
			vClipModes.push_back(eBeamLatticeClipMode::Outside);
			vClipModes.push_back(eBeamLatticeClipMode::Inside);
			vClipResourceIDs.push_back(0);
			vClipResourceIDs.push_back(1);
			vClipResourceIDs.push_back(2);
			vRepresentationResourceIDs.push_back(0);
			vRepresentationResourceIDs.push_back(0);
			vRepresentationResourceIDs.push_back(1);
		}

		auto meshObjects = model->GetMeshObjects();
		int nIndex = 0;
		while (meshObjects->MoveNext()) {
			auto modelResource = meshObjects->GetCurrent();
			auto meshObject = model->GetMeshObjectByID(modelResource->GetResourceID());
			auto beamLattice = meshObject->BeamLattice();
			
			eBeamLatticeClipMode eClipMode;
			Lib3MF_uint32 nResourceID;
			beamLattice->GetClipping(eClipMode, nResourceID);
			ASSERT_EQ(vClipModes[nIndex], eClipMode) << L"Clip mode does not match";
			if (eClipMode != eBeamLatticeClipMode::NoClipMode) {
				ASSERT_EQ(vClipResourceIDs[nIndex], nResourceID) << L"Clipping Resource ID does not match";
			}

			if (beamLattice->GetRepresentation(nResourceID)) {
				ASSERT_EQ(vRepresentationResourceIDs[nIndex], nResourceID) << L"Representation Resource ID does not match";
			}
			nIndex++;
		}
	}

	// A new one of these is created for each test
	class BeamLattice_Attributes_Negative : public testing::TestWithParam<const char*>
	{
	public:
		virtual void SetUp() {}
		virtual void TearDown() {}
	};

	INSTANTIATE_TEST_SUITE_P(InstantiationName,
		BeamLattice_Attributes_Negative,
		::testing::Values("Box_Attributes_Negative_1.3mf",
			"Box_Attributes_Negative_2.3mf"));

	TEST_P(BeamLattice_Attributes_Negative, Read)
	{
		Read_Attributes_Negative(GetParam());
	}

}
