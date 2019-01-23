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

	TEST_F(BeamLattice, ObjectType1)
	{
		sLib3MFBeam beam;
		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 1;
		beam.m_Radii[0] = 1.5;
		beam.m_Radii[1] = 1.2;
		beam.m_CapModes[0].m_enum = eBeamLatticeCapModeSphere;
		beam.m_CapModes[1].m_enum = eBeamLatticeCapModeSphere;

		mesh->BeamLattice()->AddBeam(beam);

		mesh->SetType(eLib3MFObjectType::eObjectTypeModel);
		mesh->SetType(eLib3MFObjectType::eObjectTypeSolidSupport);

		try {
			mesh->SetType(eLib3MFObjectType::eObjectTypeSupport);
			ASSERT_FALSE(true) << "Could set eObjectTypeSupport";
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
		try {
			mesh->SetType(eLib3MFObjectType::eObjectTypeOther);
			ASSERT_FALSE(true) << "Could set eObjectTypeOther";
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(BeamLattice, ObjectType2)
	{
		sLib3MFBeam beam;
		beam.m_Indices[0] = 0;
		beam.m_Indices[1] = 1;
		beam.m_Radii[0] = 1.5;
		beam.m_Radii[1] = 1.2;
		beam.m_CapModes[0].m_enum = eBeamLatticeCapModeSphere;
		beam.m_CapModes[1].m_enum = eBeamLatticeCapModeSphere;

		mesh->SetType(eLib3MFObjectType::eObjectTypeSupport);

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
		auto model = CLib3MFWrapper::CreateModel();
		{
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + "/" + "BeamLattice" + "/" + fName);
			ASSERT_EQ(reader->GetWarningCount(), 0);
		}
	}

	TEST_F(BeamLattice, Read_Box_Simple)
	{
		std::string fName("Box_Simple.3mf");
		auto model = CLib3MFWrapper::CreateModel();
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
				sLib3MFBeam beam = beamLattice->GetBeam(0);
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
			}
			nIndex++;
		}
	}

	void Read_Attributes_Negative(std::string fName)
	{
		auto model = CLib3MFWrapper::CreateModel();

		{
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + "/" + "BeamLattice" + "/" + fName);
			ASSERT_EQ(reader->GetWarningCount(), 1);
			Lib3MF_uint32 nErrorCode;
			std::string warning = reader->GetWarning(0, nErrorCode);
		}

		std::vector<eLib3MFBeamLatticeClipMode> vClipModes;
		std::vector<Lib3MF_uint32> vClipResourceIDs;
		std::vector<Lib3MF_uint32> vRepresentationResourceIDs;
		if (fName == "Box_Attributes_Negative_1.3mf") {
			vClipModes.push_back(eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeNoClipMode);
			vClipModes.push_back(eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeNoClipMode);
			vClipResourceIDs.push_back(0);
			vClipResourceIDs.push_back(0);
			vRepresentationResourceIDs.push_back(0);
			vRepresentationResourceIDs.push_back(0);
		}
		if (fName == "Box_Attributes_Negative_2.3mf") {
			vClipModes.push_back(eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeNoClipMode);
			vClipModes.push_back(eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeOutside);
			vClipModes.push_back(eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeInside);
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
			
			eLib3MFBeamLatticeClipMode eClipMode;
			Lib3MF_uint32 nResourceID;
			beamLattice->GetClipping(eClipMode, nResourceID);
			ASSERT_EQ(vClipModes[nIndex], eClipMode) << L"Clip mode does not match";
			if (eClipMode != eLib3MFBeamLatticeClipMode::eBeamLatticeClipModeNoClipMode) {
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

	INSTANTIATE_TEST_CASE_P(InstantiationName,
		BeamLattice_Attributes_Negative,
		::testing::Values("Box_Attributes_Negative_1.3mf",
			"Box_Attributes_Negative_2.3mf"));

	TEST_P(BeamLattice_Attributes_Negative, Read)
	{
		Read_Attributes_Negative(GetParam());
	}

}
