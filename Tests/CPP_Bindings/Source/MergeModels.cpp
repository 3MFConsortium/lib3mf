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

UnitTest_Model.cpp: Defines Unittests for the functionality to merge models

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class MergeModels : public Lib3MFTest {
	protected:
		virtual void SetUp() {
			m_pModel = wrapper->CreateModel();
			auto reader3MF = m_pModel->QueryReader("3mf");
			reader3MF->ReadFromFile(sTestFilesPath + "/Models/" + "WithSomeResources.3mf");
		}
		virtual void TearDown() {
			m_pModel.reset();
		}

		PModel m_pModel;
	};

	void ExpectEqModels(PModel p1, PModel p2)
	{
		EXPECT_EQ(p1->GetAttachmentCount(), p2->GetAttachmentCount());
		EXPECT_EQ(p1->GetBaseMaterialGroups()->Count(), p2->GetBaseMaterialGroups()->Count());
		EXPECT_EQ(p1->GetColorGroups()->Count(), p2->GetColorGroups()->Count());
		EXPECT_EQ(p1->GetCompositeMaterials()->Count(), p2->GetCompositeMaterials()->Count());
		EXPECT_EQ(p1->GetTexture2DGroups()->Count(), p2->GetTexture2DGroups()->Count());
		EXPECT_EQ(p1->GetTexture2Ds()->Count(), p2->GetTexture2Ds()->Count());
		EXPECT_EQ(p1->GetMultiPropertyGroups()->Count(), p2->GetMultiPropertyGroups()->Count());
	}

	TEST_F(MergeModels, MergeModel)
	{
		std::vector<Lib3MF_uint8> buffer;
		auto pMergedModel = m_pModel->MergeToModel();
		{
			auto writer3MF = pMergedModel->QueryWriter("3mf");
			writer3MF->WriteToBuffer(buffer);
			// writer3MF->WriteToFile(sTestFilesPath + "/Models/" + "WithSomeResources_out.3mf");
		}

		ExpectEqModels(m_pModel, pMergedModel);
		EXPECT_EQ(pMergedModel->GetMeshObjects()->Count(), 1);
		EXPECT_EQ(pMergedModel->GetComponentsObjects()->Count(), 0);
		EXPECT_EQ(pMergedModel->GetBuildItems()->Count(), 1);

		auto pReadModel = wrapper->CreateModel();
		{
			auto reader3MF = pReadModel->QueryReader("3mf");
			reader3MF->ReadFromBuffer(buffer);
		}
		ExpectEqModels(m_pModel, pReadModel);
	}

}
