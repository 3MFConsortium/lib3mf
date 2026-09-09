/*++

Copyright (C) 2026 3MF Consortium

All rights reserved.

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"
#include <cmath>

namespace {

constexpr const char * TextureRelationship = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";

Lib3MF::PAttachment addPNGAttachment(const Lib3MF::PModel & model, const std::string & path = "/3D/Textures/displacement.png")
{
	auto attachment = model->AddAttachment(path, TextureRelationship);
	const std::vector<Lib3MF_uint8> pngSignature{ 137, 80, 78, 71, 13, 10, 26, 10 };
	attachment->ReadFromBuffer(pngSignature);
	return attachment;
}

Lib3MF::PDisplacementMeshObject addTetrahedron(const Lib3MF::PModel & model)
{
	std::vector<Lib3MF::sPosition> vertices{
		{ { 0.0f, 0.0f, 0.0f } },
		{ { 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, 0.0f, 1.0f } }
	};
	std::vector<Lib3MF::sTriangle> triangles{
		{ { 0, 2, 1 } },
		{ { 0, 1, 3 } },
		{ { 0, 3, 2 } },
		{ { 1, 2, 3 } }
	};
	auto mesh = model->AddDisplacementMeshObject();
	mesh->SetGeometry(vertices, triangles);
	return mesh;
}

}

class Displacement : public Lib3MFTest {
};

TEST_F(Displacement, CreateQueryAndRoundTrip)
{
	bool supported = false;
	Lib3MF_uint32 major = 0, minor = 0, micro = 0;
	wrapper->GetSpecificationVersion("http://schemas.3mf.io/3dmanufacturing/displacement/2023/10", supported, major, minor, micro);
	ASSERT_TRUE(supported);
	ASSERT_EQ(major, 1u);
	ASSERT_EQ(minor, 0u);
	ASSERT_EQ(micro, 0u);

	auto model = wrapper->CreateModel();
	auto texture = model->AddDisplacement2D(addPNGAttachment(model).get());
	texture->SetChannel(Lib3MF::eChannelName::Blue);
	texture->SetTileStyleUV(Lib3MF::eTextureTileStyle::Mirror, Lib3MF::eTextureTileStyle::Clamp);
	texture->SetFilter(Lib3MF::eTextureFilter::Nearest);

	auto normals = model->AddNormVectorGroup();
	Lib3MF::sVector vector{ { 2.0, 2.0, 2.0 } };
	ASSERT_EQ(normals->AddVector(vector), 0u);
	auto normalized = normals->GetVector(0);
	ASSERT_NEAR(normalized.m_Coordinates[0], 1.0 / std::sqrt(3.0), 1.0e-12);

	auto coordinates = model->AddDisp2DGroup(texture.get(), normals.get(), 2.5, -0.25);
	for (Lib3MF_double factor : { 0.0, 0.5, 1.0 }) {
		Lib3MF::sDisplacement2DCoordinate coordinate{ factor, 1.0 - factor, 0, factor };
		coordinates->AddCoordinate(coordinate);
	}

	auto mesh = addTetrahedron(model);
	Lib3MF::sTriangleDisplacement triangleDisplacement{ { 0, 1, 2 } };
	mesh->SetTriangleDisplacement(3, coordinates.get(), triangleDisplacement);
	model->AddBuildItem(mesh.get(), wrapper->GetIdentityTransform());

	ASSERT_TRUE(mesh->IsDisplacementMeshObject());
	ASSERT_TRUE(mesh->HasTriangleDisplacement(3));
	ASSERT_FALSE(mesh->HasTriangleDisplacement(0));
	ASSERT_EQ(model->GetMeshObjectByID(mesh->GetResourceID())->IsDisplacementMeshObject(), true);
	ASSERT_EQ(model->GetDisplacementMeshObjects()->Count(), 1u);
	ASSERT_EQ(model->GetDisplacement2Ds()->Count(), 1u);
	ASSERT_EQ(model->GetNormVectorGroups()->Count(), 1u);
	ASSERT_EQ(model->GetDisp2DGroups()->Count(), 1u);

	Lib3MF::PDisp2DGroup returnedGroup;
	auto returned = mesh->GetTriangleDisplacement(3, returnedGroup);
	ASSERT_EQ(returnedGroup->GetResourceID(), coordinates->GetResourceID());
	ASSERT_EQ(returned.m_DisplacementIndices[1], 1u);

	std::vector<Lib3MF_uint8> buffer;
	model->QueryWriter("3mf")->WriteToBuffer(buffer);
	auto readModel = wrapper->CreateModel();
	auto reader = readModel->QueryReader("3mf");
	reader->ReadFromBuffer(buffer);
	CheckReaderWarnings(reader, 0);

	auto meshes = readModel->GetDisplacementMeshObjects();
	ASSERT_EQ(meshes->Count(), 1u);
	ASSERT_TRUE(meshes->MoveNext());
	auto readMesh = meshes->GetCurrentDisplacementMeshObject();
	ASSERT_EQ(readMesh->GetTriangleCount(), 4u);
	ASSERT_TRUE(readMesh->HasTriangleDisplacement(3));
	ASSERT_EQ(readModel->GetDisplacement2DByID(texture->GetModelResourceID())->GetChannel(), Lib3MF::eChannelName::Blue);
	ASSERT_EQ(readModel->GetNormVectorGroupByID(normals->GetModelResourceID())->GetCount(), 1u);
	ASSERT_EQ(readModel->GetDisp2DGroupByID(coordinates->GetModelResourceID())->GetCount(), 3u);
	ASSERT_SPECIFIC_THROW(model->MergeToModel(), Lib3MF::ELib3MFException);
}

TEST_F(Displacement, RejectInvalidApiState)
{
	auto model = wrapper->CreateModel();
	auto jpg = model->AddAttachment("/3D/Textures/displacement.jpg", TextureRelationship);
	ASSERT_SPECIFIC_THROW(model->AddDisplacement2D(jpg.get()), Lib3MF::ELib3MFException);

	auto texture = model->AddDisplacement2D(addPNGAttachment(model).get());
	auto normals = model->AddNormVectorGroup();
	Lib3MF::sVector zero{ { 0.0, 0.0, 0.0 } };
	ASSERT_SPECIFIC_THROW(normals->AddVector(zero), Lib3MF::ELib3MFException);
	Lib3MF::sVector outward{ { 1.0, 1.0, 1.0 } };
	normals->AddVector(outward);
	auto coordinates = model->AddDisp2DGroup(texture.get(), normals.get(), 1.0, 0.0);
	Lib3MF::sDisplacement2DCoordinate negativeFactor{ 0.0, 0.0, 0, -1.0 };
	ASSERT_SPECIFIC_THROW(coordinates->AddCoordinate(negativeFactor), Lib3MF::ELib3MFException);
	Lib3MF::sDisplacement2DCoordinate invalidNormal{ 0.0, 0.0, 1, 1.0 };
	ASSERT_SPECIFIC_THROW(coordinates->AddCoordinate(invalidNormal), Lib3MF::ELib3MFException);

	Lib3MF::sDisplacement2DCoordinate valid{ 0.0, 0.0, 0, 1.0 };
	coordinates->AddCoordinate(valid);
	auto mesh = addTetrahedron(model);
	Lib3MF::sTriangleDisplacement displacement{ { 0, 0, 0 } };
	ASSERT_SPECIFIC_THROW(mesh->SetTriangleDisplacement(0, coordinates.get(), displacement), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(mesh->SetTriangleDisplacement(4, coordinates.get(), displacement), Lib3MF::ELib3MFException);
	auto mirrored = wrapper->GetIdentityTransform();
	mirrored.m_Fields[0][0] = -1.0f;
	ASSERT_SPECIFIC_THROW(model->AddBuildItem(mesh.get(), mirrored), Lib3MF::ELib3MFException);

	auto incompleteModel = wrapper->CreateModel();
	incompleteModel->AddDisplacementMeshObject();
	std::vector<Lib3MF_uint8> buffer;
	ASSERT_SPECIFIC_THROW(incompleteModel->QueryWriter("3mf")->WriteToBuffer(buffer), Lib3MF::ELib3MFException);

	auto incompleteResources = wrapper->CreateModel();
	incompleteResources->AddNormVectorGroup();
	ASSERT_SPECIFIC_THROW(incompleteResources->QueryWriter("3mf")->WriteToBuffer(buffer), Lib3MF::ELib3MFException);
}

TEST_F(Displacement, MergeFromModelCopiesResourceGraph)
{
	auto source = wrapper->CreateModel();
	auto texture = source->AddDisplacement2D(addPNGAttachment(source).get());
	auto normals = source->AddNormVectorGroup();
	Lib3MF::sVector vector{ { 0.0, 0.0, 1.0 } };
	normals->AddVector(vector);
	auto coordinates = source->AddDisp2DGroup(texture.get(), normals.get(), 3.0, -1.0);
	Lib3MF::sDisplacement2DCoordinate coordinate{ 0.25, 0.75, 0, 0.5 };
	coordinates->AddCoordinate(coordinate);

	auto target = wrapper->CreateModel();
	target->MergeFromModel(source.get());
	ASSERT_EQ(target->GetDisplacement2Ds()->Count(), 1u);
	ASSERT_EQ(target->GetNormVectorGroups()->Count(), 1u);
	auto groups = target->GetDisp2DGroups();
	ASSERT_EQ(groups->Count(), 1u);
	ASSERT_TRUE(groups->MoveNext());
	auto copied = groups->GetCurrentDisp2DGroup();
	ASSERT_DOUBLE_EQ(copied->GetHeight(), 3.0);
	ASSERT_DOUBLE_EQ(copied->GetOffset(), -1.0);
	ASSERT_EQ(copied->GetCount(), 1u);
	ASSERT_EQ(copied->GetNormalVectorGroup()->GetCount(), 1u);
	ASSERT_EQ(copied->GetDisplacement2D()->GetAttachment()->GetPath(), "/3D/Textures/displacement.png");
}

TEST_F(Displacement, RemoveResourcesUpdatesGenericLookup)
{
	auto model = wrapper->CreateModel();
	auto texture = model->AddDisplacement2D(addPNGAttachment(model).get());
	auto normals = model->AddNormVectorGroup();
	Lib3MF::sVector vector{ { 0.0, 0.0, 1.0 } };
	normals->AddVector(vector);
	auto coordinates = model->AddDisp2DGroup(texture.get(), normals.get(), 1.0, 0.0);
	Lib3MF::sDisplacement2DCoordinate coordinate{ 0.0, 0.0, 0, 1.0 };
	coordinates->AddCoordinate(coordinate);

	model->RemoveResource(coordinates.get());
	model->RemoveResource(normals.get());
	model->RemoveResource(texture.get());
	ASSERT_EQ(model->GetDisp2DGroups()->Count(), 0u);
	ASSERT_EQ(model->GetNormVectorGroups()->Count(), 0u);
	ASSERT_EQ(model->GetDisplacement2Ds()->Count(), 0u);
}
