/*++
--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace {

Lib3MF::PMeshObject addBoxMesh(const Lib3MF::PModel & model)
{
	std::vector<Lib3MF::sPosition> vertices;
	std::vector<Lib3MF::sTriangle> triangles;
	fnCreateBox(vertices, triangles);

	auto mesh = model->AddMeshObject();
	mesh->SetGeometry(vertices, triangles);
	return mesh;
}

void addSingleBeam(const Lib3MF::PMeshObject & mesh)
{
	Lib3MF::sBeam beam;
	beam.m_Indices[0] = 0;
	beam.m_Indices[1] = 1;
	beam.m_Radii[0] = 0.5;
	beam.m_Radii[1] = 0.5;
	mesh->BeamLattice()->AddBeam(beam);
}

Lib3MF::sTransform translatedIdentity(Lib3MF::PWrapper wrapper, float x, float y, float z)
{
	auto transform = wrapper->GetIdentityTransform();
	transform.m_Fields[3][0] = x;
	transform.m_Fields[3][1] = y;
	transform.m_Fields[3][2] = z;
	return transform;
}

}

class BooleanRead : public Lib3MFTest {
};

TEST_F(BooleanRead, ReadAndIterateBooleanObjects)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "booleans_operations.3mf");

	auto booleanObjects = model->GetBooleanObjects();
	ASSERT_EQ(booleanObjects->Count(), 2);
	ASSERT_TRUE(booleanObjects->MoveNext());

	auto booleanObjectA = booleanObjects->GetCurrentBooleanObject();
	ASSERT_TRUE(booleanObjectA->IsBooleanObject());
	ASSERT_EQ(booleanObjectA->GetOperation(), Lib3MF::eBooleanOperation::Intersection);
	ASSERT_EQ(booleanObjectA->GetBaseObject()->GetModelResourceID(), 4u);
	ASSERT_EQ(booleanObjectA->GetOperandCount(), 1u);
	Lib3MF::PMeshObject operandA;
	auto transformA = booleanObjectA->GetOperand(0, operandA);
	(void)transformA;
	ASSERT_EQ(operandA->GetModelResourceID(), 5u);

	ASSERT_TRUE(booleanObjects->MoveNext());
	auto booleanObjectB = booleanObjects->GetCurrentBooleanObject();
	ASSERT_TRUE(booleanObjectB->IsBooleanObject());
	ASSERT_EQ(booleanObjectB->GetOperation(), Lib3MF::eBooleanOperation::Difference);
	ASSERT_EQ(booleanObjectB->GetBaseObject()->GetModelResourceID(), 6u);
	ASSERT_EQ(booleanObjectB->GetOperandCount(), 3u);
	Lib3MF::PMeshObject operandB;
	booleanObjectB->GetOperand(0, operandB);
	ASSERT_EQ(operandB->GetModelResourceID(), 3u);

	auto resourceByID = model->GetBooleanObjectByID(booleanObjectB->GetResourceID());
	ASSERT_EQ(resourceByID->GetOperandCount(), 3u);
}

TEST_F(BooleanRead, ReadWriteReadBooleanObjects)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "booleans_operations.3mf");

	std::vector<Lib3MF_uint8> buffer;
	auto writer = model->QueryWriter("3mf");
	writer->WriteToBuffer(buffer);

	auto roundTripModel = wrapper->CreateModel();
	auto roundTripReader = roundTripModel->QueryReader("3mf");
	roundTripReader->ReadFromBuffer(buffer);

	auto booleanObjects = roundTripModel->GetBooleanObjects();
	ASSERT_EQ(booleanObjects->Count(), 2);
	ASSERT_TRUE(booleanObjects->MoveNext());

	auto booleanObjectA = booleanObjects->GetCurrentBooleanObject();
	ASSERT_EQ(booleanObjectA->GetOperation(), Lib3MF::eBooleanOperation::Intersection);
	ASSERT_EQ(booleanObjectA->GetBaseObject()->GetModelResourceID(), 4u);
	ASSERT_EQ(booleanObjectA->GetOperandCount(), 1u);

	ASSERT_TRUE(booleanObjects->MoveNext());
	auto booleanObjectB = booleanObjects->GetCurrentBooleanObject();
	ASSERT_EQ(booleanObjectB->GetOperation(), Lib3MF::eBooleanOperation::Difference);
	ASSERT_EQ(booleanObjectB->GetBaseObject()->GetModelResourceID(), 6u);
	ASSERT_EQ(booleanObjectB->GetOperandCount(), 3u);
}

TEST_F(BooleanRead, CreateAndRoundTripBooleanObject)
{
	auto model = wrapper->CreateModel();
	auto baseMesh = addBoxMesh(model);
	auto operandMesh = addBoxMesh(model);
	auto booleanObject = model->AddBooleanObject();

	auto baseTransform = translatedIdentity(wrapper, 1.0f, 2.0f, 3.0f);
	auto operandTransform = translatedIdentity(wrapper, -4.0f, 5.0f, -6.0f);

	booleanObject->SetBaseObject(baseMesh.get(), baseTransform);
	booleanObject->SetOperation(Lib3MF::eBooleanOperation::Difference);
	booleanObject->AddOperand(operandMesh.get(), operandTransform);
	model->AddBuildItem(booleanObject.get(), wrapper->GetIdentityTransform());

	ASSERT_TRUE(booleanObject->IsBooleanObject());
	ASSERT_EQ(booleanObject->GetBaseObject()->GetResourceID(), baseMesh->GetResourceID());
	ASSERT_EQ(booleanObject->GetOperation(), Lib3MF::eBooleanOperation::Difference);
	ASSERT_EQ(booleanObject->GetOperandCount(), 1u);
	Lib3MF::helper::CompareTransforms(booleanObject->GetBaseTransform(), baseTransform);

	Lib3MF::PMeshObject readOperand;
	auto roundTripOperandTransform = booleanObject->GetOperand(0, readOperand);
	ASSERT_EQ(readOperand->GetResourceID(), operandMesh->GetResourceID());
	Lib3MF::helper::CompareTransforms(roundTripOperandTransform, operandTransform);

	std::vector<Lib3MF_uint8> buffer;
	model->QueryWriter("3mf")->WriteToBuffer(buffer);

	auto roundTripModel = wrapper->CreateModel();
	auto reader = roundTripModel->QueryReader("3mf");
	reader->ReadFromBuffer(buffer);
	CheckReaderWarnings(reader, 0);

	auto booleanObjects = roundTripModel->GetBooleanObjects();
	ASSERT_EQ(booleanObjects->Count(), 1);
	ASSERT_TRUE(booleanObjects->MoveNext());
	auto roundTripBoolean = booleanObjects->GetCurrentBooleanObject();
	ASSERT_EQ(roundTripBoolean->GetOperation(), Lib3MF::eBooleanOperation::Difference);
	ASSERT_EQ(roundTripBoolean->GetBaseObject()->GetModelResourceID(), baseMesh->GetModelResourceID());
	ASSERT_EQ(roundTripBoolean->GetOperandCount(), 1u);
	Lib3MF::helper::CompareTransforms(roundTripBoolean->GetBaseTransform(), baseTransform);

	Lib3MF::PMeshObject roundTripOperand;
	auto roundTripTransform = roundTripBoolean->GetOperand(0, roundTripOperand);
	ASSERT_EQ(roundTripOperand->GetModelResourceID(), operandMesh->GetModelResourceID());
	Lib3MF::helper::CompareTransforms(roundTripTransform, operandTransform);
}

TEST_F(BooleanRead, ApiRejectsInvalidStateAndAccess)
{
	auto model = wrapper->CreateModel();
	auto baseMesh = addBoxMesh(model);
	auto operandMesh = addBoxMesh(model);
	auto supportMesh = addBoxMesh(model);
	auto booleanObject = model->AddBooleanObject();
	auto componentsObject = model->AddComponentsObject();
	supportMesh->SetType(Lib3MF::eObjectType::Support);

	ASSERT_SPECIFIC_THROW(booleanObject->GetBaseObject(), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(booleanObject->SetBaseTransform(wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(booleanObject->SetBaseObject(nullptr, wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(booleanObject->SetBaseObject(componentsObject.get(), wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(booleanObject->SetBaseObject(supportMesh.get(), wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);

	booleanObject->SetBaseObject(baseMesh.get(), wrapper->GetIdentityTransform());
	booleanObject->AddOperand(operandMesh.get(), wrapper->GetIdentityTransform());

	ASSERT_SPECIFIC_THROW(booleanObject->AddOperand(nullptr, wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(booleanObject->AddOperand(supportMesh.get(), wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW({
		Lib3MF::PMeshObject invalidOperand;
		booleanObject->GetOperand(1, invalidOperand);
	}, Lib3MF::ELib3MFException);
	ASSERT_SPECIFIC_THROW(model->GetBooleanObjectByID(baseMesh->GetResourceID()), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, ApiRejectsBeamLatticeMeshes)
{
	auto model = wrapper->CreateModel();
	auto regularMesh = addBoxMesh(model);
	auto beamMesh = addBoxMesh(model);
	auto booleanObject = model->AddBooleanObject();
	addSingleBeam(beamMesh);

	ASSERT_SPECIFIC_THROW(booleanObject->SetBaseObject(beamMesh.get(), wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);

	booleanObject->SetBaseObject(regularMesh.get(), wrapper->GetIdentityTransform());
	ASSERT_SPECIFIC_THROW(booleanObject->AddOperand(beamMesh.get(), wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, ApiRejectsLevelSetAsBaseObject)
{
	auto model = wrapper->CreateModel();
	auto levelSet = model->AddLevelSet();
	auto booleanObject = model->AddBooleanObject();

	ASSERT_SPECIFIC_THROW(booleanObject->SetBaseObject(levelSet.get(), wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, ApiRejectsBooleanBaseReferenceCycles)
{
	auto model = wrapper->CreateModel();
	auto mesh = addBoxMesh(model);
	auto booleanA = model->AddBooleanObject();
	auto booleanB = model->AddBooleanObject();

	booleanA->SetBaseObject(booleanB.get(), wrapper->GetIdentityTransform());
	ASSERT_SPECIFIC_THROW(booleanB->SetBaseObject(booleanA.get(), wrapper->GetIdentityTransform()), Lib3MF::ELib3MFException);

	// Keep both objects otherwise structurally valid to ensure failure is from cycle rejection.
	booleanA->AddOperand(mesh.get(), wrapper->GetIdentityTransform());
	booleanB->AddOperand(mesh.get(), wrapper->GetIdentityTransform());
}

TEST_F(BooleanRead, ApiControlsCSGModeAndExtractionResolution)
{
	auto model = wrapper->CreateModel();
	auto baseMesh = addBoxMesh(model);
	auto operandMesh = addBoxMesh(model);
	auto booleanObject = model->AddBooleanObject();

	booleanObject->SetBaseObject(baseMesh.get(), wrapper->GetIdentityTransform());
	booleanObject->SetOperation(Lib3MF::eBooleanOperation::Difference);
	booleanObject->AddOperand(operandMesh.get(), wrapper->GetIdentityTransform());

	ASSERT_FALSE(booleanObject->GetCSGModeEnabled());
	booleanObject->SetCSGModeEnabled(true);
	ASSERT_TRUE(booleanObject->GetCSGModeEnabled());

	booleanObject->SetExtractionGridResolution(144);
	ASSERT_EQ(booleanObject->GetExtractionGridResolution(), 144u);
	ASSERT_SPECIFIC_THROW(booleanObject->SetExtractionGridResolution(0), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, CreateAndRoundTripBooleanObjectWithExternalReferences)
{
	auto model = wrapper->CreateModel();
	auto baseMesh = addBoxMesh(model);
	auto operandMesh = addBoxMesh(model);
	auto booleanObject = model->AddBooleanObject();
	auto basePart = model->FindOrCreatePackagePart("/3D/boolean_base.model");
	auto operandPart = model->FindOrCreatePackagePart("/3D/boolean_operand.model");

	booleanObject->SetBaseObject(baseMesh.get(), wrapper->GetIdentityTransform());
	booleanObject->SetOperation(Lib3MF::eBooleanOperation::Union);
	booleanObject->AddOperand(operandMesh.get(), wrapper->GetTranslationTransform(2.0, 0.0, 0.0));
	baseMesh->SetPackagePart(basePart.get());
	operandMesh->SetPackagePart(operandPart.get());
	model->AddBuildItem(booleanObject.get(), wrapper->GetIdentityTransform());

	std::vector<Lib3MF_uint8> buffer;
	model->QueryWriter("3mf")->WriteToBuffer(buffer);

	auto roundTripModel = wrapper->CreateModel();
	auto reader = roundTripModel->QueryReader("3mf");
	reader->ReadFromBuffer(buffer);
	CheckReaderWarnings(reader, 0);

	auto booleanObjects = roundTripModel->GetBooleanObjects();
	ASSERT_EQ(booleanObjects->Count(), 1);
	ASSERT_TRUE(booleanObjects->MoveNext());

	auto roundTripBoolean = booleanObjects->GetCurrentBooleanObject();
	ASSERT_EQ(roundTripBoolean->GetBaseObject()->PackagePart()->GetPath(), "/3D/boolean_base.model");
	ASSERT_EQ(roundTripBoolean->GetOperandCount(), 1u);

	Lib3MF::PMeshObject operand;
	roundTripBoolean->GetOperand(0, operand);
	ASSERT_EQ(operand->PackagePart()->GetPath(), "/3D/boolean_operand.model");
}

TEST_F(BooleanRead, WriterRejectsIncompleteBooleanObjects)
{
	{
		auto model = wrapper->CreateModel();
		auto booleanObject = model->AddBooleanObject();
		model->AddBuildItem(booleanObject.get(), wrapper->GetIdentityTransform());

		std::vector<Lib3MF_uint8> buffer;
		ASSERT_SPECIFIC_THROW(model->QueryWriter("3mf")->WriteToBuffer(buffer), Lib3MF::ELib3MFException);
	}

	{
		auto model = wrapper->CreateModel();
		auto baseMesh = addBoxMesh(model);
		auto booleanObject = model->AddBooleanObject();
		booleanObject->SetBaseObject(baseMesh.get(), wrapper->GetIdentityTransform());
		model->AddBuildItem(booleanObject.get(), wrapper->GetIdentityTransform());

		std::vector<Lib3MF_uint8> buffer;
		ASSERT_SPECIFIC_THROW(model->QueryWriter("3mf")->WriteToBuffer(buffer), Lib3MF::ELib3MFException);
	}
}

TEST_F(BooleanRead, WriterRejectsDeepBooleanReferences)
{
	auto model = wrapper->CreateModel();
	auto baseMesh = addBoxMesh(model);
	auto operandMesh = addBoxMesh(model);
	auto booleanObject = model->AddBooleanObject();
	auto nonRootPart = model->FindOrCreatePackagePart("/3D/nonroot_boolean.model");

	booleanObject->SetBaseObject(baseMesh.get(), wrapper->GetIdentityTransform());
	booleanObject->SetOperation(Lib3MF::eBooleanOperation::Difference);
	booleanObject->AddOperand(operandMesh.get(), wrapper->GetIdentityTransform());
	booleanObject->SetPackagePart(nonRootPart.get());
	model->AddBuildItem(booleanObject.get(), wrapper->GetIdentityTransform());

	std::vector<Lib3MF_uint8> buffer;
	ASSERT_SPECIFIC_THROW(model->QueryWriter("3mf")->WriteToBuffer(buffer), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, InvalidOperationYieldsWarningAndDefaultsToUnion)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "invalid_operation_warning.3mf");
	CheckReaderWarnings(reader, 1);

	auto booleanObjects = model->GetBooleanObjects();
	ASSERT_EQ(booleanObjects->Count(), 1);
	ASSERT_TRUE(booleanObjects->MoveNext());
	ASSERT_EQ(booleanObjects->GetCurrentBooleanObject()->GetOperation(), Lib3MF::eBooleanOperation::Union);
}

TEST_F(BooleanRead, MissingBaseObjectIDThrows)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "missing_base_objectid.3mf"), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, MissingOperandsThrow)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "missing_operands.3mf"), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, MissingPathTargetThrows)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "missing_path_target.3mf"), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, PathWithoutProductionExtensionThrows)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "path_without_production_extension.3mf"), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, ComponentsBaseObjectThrows)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "components_base_object.3mf"), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, NonMeshOperandThrows)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "components_operand_object.3mf"), Lib3MF::ELib3MFException);
}

TEST_F(BooleanRead, UnknownBooleanAttributeYieldsWarning)
{
	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "unknown_attribute_warning.3mf");
	CheckReaderWarnings(reader, 1);

	auto booleanObjects = model->GetBooleanObjects();
	ASSERT_EQ(booleanObjects->Count(), 1);
	ASSERT_TRUE(booleanObjects->MoveNext());
	ASSERT_EQ(booleanObjects->GetCurrentBooleanObject()->GetOperandCount(), 1u);
}

TEST_F(BooleanRead, STLWriterMaterializesBooleanObject)
{
	auto model = wrapper->CreateModel();
	auto baseMesh = addBoxMesh(model);
	auto operandMesh = addBoxMesh(model);
	auto booleanObject = model->AddBooleanObject();

	booleanObject->SetBaseObject(baseMesh.get(), wrapper->GetIdentityTransform());
	booleanObject->SetOperation(Lib3MF::eBooleanOperation::Intersection);
	booleanObject->AddOperand(operandMesh.get(), wrapper->GetTranslationTransform(1.0, 0.0, 0.0));
	model->AddBuildItem(booleanObject.get(), wrapper->GetIdentityTransform());

	std::vector<Lib3MF_uint8> buffer;
	model->QueryWriter("stl")->WriteToBuffer(buffer);
	ASSERT_FALSE(buffer.empty());
}

TEST_F(BooleanRead, STLWriterMaterializesBooleanOperations)
{
	auto testOperation = [&](Lib3MF::eBooleanOperation operation) {
		auto model = wrapper->CreateModel();
		auto baseMesh = addBoxMesh(model);
		auto operandMesh = addBoxMesh(model);
		auto booleanObject = model->AddBooleanObject();

		booleanObject->SetBaseObject(baseMesh.get(), wrapper->GetIdentityTransform());
		booleanObject->SetOperation(operation);
		booleanObject->AddOperand(operandMesh.get(), wrapper->GetTranslationTransform(2.0, 0.0, 0.0));
		model->AddBuildItem(booleanObject.get(), wrapper->GetIdentityTransform());

		std::vector<Lib3MF_uint8> buffer;
		model->QueryWriter("stl")->WriteToBuffer(buffer);
		ASSERT_FALSE(buffer.empty());
	};

	testOperation(Lib3MF::eBooleanOperation::Union);
	testOperation(Lib3MF::eBooleanOperation::Difference);
	testOperation(Lib3MF::eBooleanOperation::Intersection);
}

TEST_F(BooleanRead, ObjectTypeIntrospectionIsConsistent)
{
	auto model = wrapper->CreateModel();
	auto mesh = addBoxMesh(model);
	auto components = model->AddComponentsObject();
	auto booleanObject = model->AddBooleanObject();
	auto levelSet = model->AddLevelSet();

	booleanObject->SetBaseObject(mesh.get(), wrapper->GetIdentityTransform());
	booleanObject->AddOperand(mesh.get(), wrapper->GetIdentityTransform());

	ASSERT_TRUE(booleanObject->IsBooleanObject());
	ASSERT_FALSE(booleanObject->IsMeshObject());
	ASSERT_FALSE(booleanObject->IsComponentsObject());
	ASSERT_FALSE(booleanObject->IsLevelSetObject());

	ASSERT_TRUE(mesh->IsMeshObject());
	ASSERT_FALSE(mesh->IsBooleanObject());

	ASSERT_TRUE(components->IsComponentsObject());
	ASSERT_FALSE(components->IsBooleanObject());

	ASSERT_TRUE(levelSet->IsLevelSetObject());
	ASSERT_FALSE(levelSet->IsBooleanObject());
}

TEST_F(BooleanRead, WriteBooleanWithHigherResourceDependencies)
{
	auto model = wrapper->CreateModel();
	auto booleanObject = model->AddBooleanObject();
	auto baseMesh = addBoxMesh(model);
	auto operandMesh = addBoxMesh(model);

	booleanObject->SetBaseObject(baseMesh.get(), wrapper->GetIdentityTransform());
	booleanObject->SetOperation(Lib3MF::eBooleanOperation::Union);
	booleanObject->AddOperand(operandMesh.get(), wrapper->GetIdentityTransform());
	model->AddBuildItem(booleanObject.get(), wrapper->GetIdentityTransform());

	std::vector<Lib3MF_uint8> buffer;
	model->QueryWriter("3mf")->WriteToBuffer(buffer);
	ASSERT_FALSE(buffer.empty());
}

TEST_F(BooleanRead, ExperimentalCSGPathMaterializesSurface)
{
#if !defined(LIB3MF_ENABLE_EXPENSIVE_TESTS) || (LIB3MF_ENABLE_EXPENSIVE_TESTS == 0)
	GTEST_SKIP() << "Disabled by default. Reconfigure with -DLIB3MF_ENABLE_EXPENSIVE_TESTS=ON to run this test.";
#endif

	auto model = wrapper->CreateModel();
	auto baseMesh = addBoxMesh(model);
	auto operandMesh = addBoxMesh(model);
	auto booleanObject = model->AddBooleanObject();

	booleanObject->SetBaseObject(baseMesh.get(), wrapper->GetIdentityTransform());
	booleanObject->SetOperation(Lib3MF::eBooleanOperation::Difference);
	booleanObject->AddOperand(operandMesh.get(), wrapper->GetTranslationTransform(0.25, 0.0, 0.0));
	booleanObject->SetCSGModeEnabled(true);
	booleanObject->SetExtractionGridResolution(160);
	model->AddBuildItem(booleanObject.get(), wrapper->GetIdentityTransform());

	std::vector<Lib3MF_uint8> buffer;
	model->QueryWriter("stl")->WriteToBuffer(buffer);
	ASSERT_FALSE(buffer.empty());
}

TEST_F(BooleanRead, ReadBooleanFileAndExtractSTLToDisk)
{
#if !defined(LIB3MF_ENABLE_EXPENSIVE_TESTS) || (LIB3MF_ENABLE_EXPENSIVE_TESTS == 0)
	GTEST_SKIP() << "Disabled by default. Reconfigure with -DLIB3MF_ENABLE_EXPENSIVE_TESTS=ON to run this test.";
#endif

	auto model = wrapper->CreateModel();
	auto reader = model->QueryReader("3mf");
	reader->ReadFromFile(sTestFilesPath + "/" + "Boolean" + "/" + "booleans_operations.3mf");
	auto booleanObjects = model->GetBooleanObjects();
	while (booleanObjects->MoveNext()) {
		auto booleanObject = booleanObjects->GetCurrentBooleanObject();
		booleanObject->SetCSGModeEnabled(true);
		booleanObject->SetExtractionGridResolution(160);
	}

	const std::string outputPath = "boolean_from_file_extracted.stl";
	auto stlWriter = model->QueryWriter("stl");
	stlWriter->WriteToFile(outputPath);

	std::ifstream file(outputPath, std::ios::binary | std::ios::ate);
	ASSERT_TRUE(file.good());
	ASSERT_GT(file.tellg(), 0);
}
