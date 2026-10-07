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
	// The spec does not restrict build item transforms of displacement meshes.
	model->AddBuildItem(mesh.get(), mirrored);

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

TEST_F(Displacement, RejectForeignModelHandlesWithMatchingIDsAndPaths)
{
	auto model = wrapper->CreateModel();
	auto otherModel = wrapper->CreateModel();
	auto attachment = addPNGAttachment(model);
	auto otherAttachment = addPNGAttachment(otherModel);
	auto texture = model->AddDisplacement2D(attachment.get());
	auto otherTexture = otherModel->AddDisplacement2D(otherAttachment.get());
	auto normals = model->AddNormVectorGroup();
	auto otherNormals = otherModel->AddNormVectorGroup();
	Lib3MF::sVector vector{ { 1.0, 1.0, 1.0 } };
	normals->AddVector(vector);
	otherNormals->AddVector(vector);
	auto group = model->AddDisp2DGroup(texture.get(), normals.get(), 1.0, 0.0);
	auto otherGroup = otherModel->AddDisp2DGroup(otherTexture.get(), otherNormals.get(), 1.0, 0.0);
	Lib3MF::sDisplacement2DCoordinate coordinate{ 0.0, 0.0, 0, 1.0 };
	group->AddCoordinate(coordinate);
	otherGroup->AddCoordinate(coordinate);
	ASSERT_EQ(texture->GetResourceID(), otherTexture->GetResourceID());
	ASSERT_EQ(normals->GetResourceID(), otherNormals->GetResourceID());
	ASSERT_EQ(group->GetResourceID(), otherGroup->GetResourceID());
	ASSERT_EQ(attachment->GetPath(), otherAttachment->GetPath());

	ASSERT_SPECIFIC_THROW(model->AddDisplacement2D(otherAttachment.get()), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(texture->SetAttachment(otherAttachment.get()), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(model->AddDisp2DGroup(otherTexture.get(), normals.get(), 1.0, 0.0), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(model->AddDisp2DGroup(texture.get(), otherNormals.get(), 1.0, 0.0), Lib3MF::ELib3MFException);

	auto mesh = addTetrahedron(model);
	Lib3MF::sTriangleDisplacement displacement{ { 0, 0, 0 } };
	ASSERT_SPECIFIC_THROW(mesh->SetTriangleDisplacement(3, otherGroup.get(), displacement), Lib3MF::ELib3MFException);
	ASSERT_FALSE(mesh->HasTriangleDisplacement(3));
	mesh->SetTriangleDisplacement(3, group.get(), displacement);
	ASSERT_TRUE(mesh->HasTriangleDisplacement(3));
	texture->SetAttachment(attachment.get());
	ASSERT_EQ(model->GetDisplacement2Ds()->Count(), 1u);
	ASSERT_EQ(model->GetDisp2DGroups()->Count(), 1u);

	model->RemoveResource(group.get());
	ASSERT_SPECIFIC_THROW(mesh->SetTriangleDisplacement(3, group.get(), displacement), Lib3MF::ELib3MFException);
	model->RemoveResource(texture.get());
	ASSERT_SPECIFIC_THROW(model->AddDisp2DGroup(texture.get(), normals.get(), 1.0, 0.0), Lib3MF::ELib3MFException);
}

TEST_F(Displacement, RejectInvalidTextureEnumsWithoutChangingState)
{
	auto model = wrapper->CreateModel();
	auto texture = model->AddDisplacement2D(addPNGAttachment(model).get());
	texture->SetTileStyleUV(Lib3MF::eTextureTileStyle::Mirror, Lib3MF::eTextureTileStyle::Clamp);
	texture->SetFilter(Lib3MF::eTextureFilter::Nearest);
	for (int invalid : { -1, 999 }) {
		ASSERT_SPECIFIC_THROW(texture->SetFilter(static_cast<Lib3MF::eTextureFilter>(invalid)), Lib3MF::ELib3MFException);
		ASSERT_SPECIFIC_THROW(texture->SetTileStyleUV(static_cast<Lib3MF::eTextureTileStyle>(invalid), Lib3MF::eTextureTileStyle::Wrap), Lib3MF::ELib3MFException);
		ASSERT_SPECIFIC_THROW(texture->SetTileStyleUV(Lib3MF::eTextureTileStyle::Wrap, static_cast<Lib3MF::eTextureTileStyle>(invalid)), Lib3MF::ELib3MFException);
		Lib3MF::eTextureTileStyle u, v;
		texture->GetTileStyleUV(u, v);
		ASSERT_EQ(u, Lib3MF::eTextureTileStyle::Mirror);
		ASSERT_EQ(v, Lib3MF::eTextureTileStyle::Clamp);
		ASSERT_EQ(texture->GetFilter(), Lib3MF::eTextureFilter::Nearest);
	}
}

TEST_F(Displacement, RejectNormalPerpendicularToTriangle)
{
	auto model = wrapper->CreateModel();
	auto texture = model->AddDisplacement2D(addPNGAttachment(model).get());
	auto normals = model->AddNormVectorGroup();
	// Triangle 3 of the tetrahedron faces (1, 1, 1); (1, -1, 0) lies in its plane.
	Lib3MF::sVector perpendicular{ { 1.0, -1.0, 0.0 } };
	normals->AddVector(perpendicular);
	auto coordinates = model->AddDisp2DGroup(texture.get(), normals.get(), 1.0, 0.0);
	Lib3MF::sDisplacement2DCoordinate coordinate{ 0.0, 0.0, 0, 1.0 };
	coordinates->AddCoordinate(coordinate);
	auto mesh = addTetrahedron(model);
	Lib3MF::sTriangleDisplacement displacement{ { 0, 0, 0 } };
	ASSERT_SPECIFIC_THROW(mesh->SetTriangleDisplacement(3, coordinates.get(), displacement), Lib3MF::ELib3MFException);
}

TEST_F(Displacement, ReplacingTrianglesClearsTheirDisplacement)
{
	auto model = wrapper->CreateModel();
	auto texture = model->AddDisplacement2D(addPNGAttachment(model).get());
	auto normals = model->AddNormVectorGroup();
	Lib3MF::sVector outward{ { 1.0, 1.0, 1.0 } };
	normals->AddVector(outward);
	auto coordinates = model->AddDisp2DGroup(texture.get(), normals.get(), 1.0, 0.0);
	Lib3MF::sDisplacement2DCoordinate coordinate{ 0.0, 0.0, 0, 1.0 };
	coordinates->AddCoordinate(coordinate);
	auto mesh = addTetrahedron(model);
	Lib3MF::sTriangleDisplacement displacement{ { 0, 0, 0 } };

	mesh->SetTriangleDisplacement(3, coordinates.get(), displacement);
	mesh->SetTriangle(3, mesh->GetTriangle(3));
	ASSERT_FALSE(mesh->HasTriangleDisplacement(3));

	mesh->SetTriangleDisplacement(3, coordinates.get(), displacement);
	std::vector<Lib3MF::sPosition> vertices;
	std::vector<Lib3MF::sTriangle> triangles;
	mesh->GetVertices(vertices);
	mesh->GetTriangleIndices(triangles);
	mesh->SetGeometry(vertices, triangles);
	ASSERT_FALSE(mesh->HasTriangleDisplacement(3));
}

TEST_F(Displacement, MergeFromModelIntoItselfTerminates)
{
	// Without attachments, merging a model into itself reaches the displacement merge loops.
	auto model = wrapper->CreateModel();
	auto normals = model->AddNormVectorGroup();
	Lib3MF::sVector vector{ { 0.0, 0.0, 1.0 } };
	normals->AddVector(vector);

	model->MergeFromModel(model.get());
	ASSERT_EQ(model->GetNormVectorGroups()->Count(), 2u);
}

class DisplacementFiles : public Lib3MFTest, public ::testing::WithParamInterface<std::pair<std::string, Lib3MF_uint32>> {
};

// Spec-conformant files, with the number of displaced triangles each one contains
TEST_P(DisplacementFiles, ReadValidFile)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	reader->ReadFromFile(sTestFilesPath + "/Displacement/" + GetParam().first + ".3mf");
	CheckReaderWarnings(reader, 0);
	auto meshes = model->GetDisplacementMeshObjects();
	ASSERT_EQ(meshes->Count(), 1u);
	ASSERT_TRUE(meshes->MoveNext());
	auto mesh = meshes->GetCurrentDisplacementMeshObject();
	Lib3MF_uint32 nDisplaced = 0;
	for (Lib3MF_uint32 i = 0; i < mesh->GetTriangleCount(); ++i)
		if (mesh->HasTriangleDisplacement(i))
			++nDisplaced;
	ASSERT_EQ(nDisplaced, GetParam().second);
}

INSTANTIATE_TEST_SUITE_P(Displacement, DisplacementFiles, ::testing::Values(
	std::make_pair(std::string("displacement_valid"), 2u),
	std::make_pair(std::string("displacement_triangles_level_did"), 2u),
	std::make_pair(std::string("displacement_d1_omitted"), 1u),
	std::make_pair(std::string("displacement_p1_omitted"), 2u),
	std::make_pair(std::string("displacement_mirrored_build_item"), 2u),
	std::make_pair(std::string("displacement_channel_alpha"), 2u),
	std::make_pair(std::string("displacement_unused_did_ignored"), 0u)
));

class InvalidDisplacementFiles : public Lib3MFTest, public ::testing::WithParamInterface<std::string> {
};

TEST_P(InvalidDisplacementFiles, RejectInvalidFile)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/Displacement/" + GetParam() + ".3mf"), Lib3MF::ELib3MFException);
}

INSTANTIATE_TEST_SUITE_P(Displacement, InvalidDisplacementFiles, ::testing::Values(
	std::string("displacement_missing_did"),
	std::string("displacement_normal_perpendicular"),
	std::string("displacement_normal_inward"),
	std::string("displacement_negative_factor"),
	std::string("displacement_core_namespace_vertices"),
	std::string("displacement_missing_texture_relationship")
));

TEST_F(Displacement, CoreTrianglesIgnoreUnknownAttributes)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	reader->ReadFromFile(sTestFilesPath + "/Reader/core_triangles_unknown_attribute.3mf");
	CheckReaderWarnings(reader, 0);
}

TEST_F(Displacement, WritingAfterRemovingReferencedResourcesReportsMissingResource)
{
	// Removing a resource that is still referenced is allowed, as for other resource types; the write reports it.
	auto expectResourceNotFound = [](const Lib3MF::PModel & model) {
		std::vector<Lib3MF_uint8> buffer;
		try {
			model->QueryWriter("3mf")->WriteToBuffer(buffer);
			FAIL() << "Write should fail";
		}
		catch (Lib3MF::ELib3MFException & e) {
			ASSERT_NE(std::string(e.what()).find("Resource not found"), std::string::npos) << e.what();
		}
	};

	for (int removeGroup = 0; removeGroup < 2; ++removeGroup) {
		auto model = wrapper->CreateModel();
		auto texture = model->AddDisplacement2D(addPNGAttachment(model).get());
		auto normals = model->AddNormVectorGroup();
		Lib3MF::sVector outward{ { 1.0, 1.0, 1.0 } };
		normals->AddVector(outward);
		auto coordinates = model->AddDisp2DGroup(texture.get(), normals.get(), 1.0, 0.0);
		Lib3MF::sDisplacement2DCoordinate coordinate{ 0.0, 0.0, 0, 1.0 };
		coordinates->AddCoordinate(coordinate);
		auto mesh = addTetrahedron(model);
		mesh->SetTriangleDisplacement(3, coordinates.get(), Lib3MF::sTriangleDisplacement{ { 0, 0, 0 } });
		model->AddBuildItem(mesh.get(), wrapper->GetIdentityTransform());

		if (removeGroup)
			model->RemoveResource(coordinates.get());
		else
			model->RemoveResource(texture.get());
		expectResourceNotFound(model);
	}
}
