/*++
--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

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
