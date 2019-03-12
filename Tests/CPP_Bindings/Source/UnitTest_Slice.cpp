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

UnitTest_Slice.cpp: Defines Unittests for the Slice class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class Slice : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			sliceStack = model->AddSliceStack(.0);
			slice = sliceStack->AddSlice(1.0);

			sLib3MFPosition2D pos;
			pos.m_coordinates[0] = 0;
			pos.m_coordinates[1] = 0;
			vVertices.push_back(pos);

			pos.m_coordinates[0] = 0;
			pos.m_coordinates[1] = 1;
			vVertices.push_back(pos);

			pos.m_coordinates[0] = 1;
			pos.m_coordinates[1] = 1;
			vVertices.push_back(pos);

			pos.m_coordinates[0] = 1;
			pos.m_coordinates[1] = 0;
			vVertices.push_back(pos);

			vOpenPolygon.push_back(0);
			vOpenPolygon.push_back(1);
			vOpenPolygon.push_back(2);

			vClosedPolygon.push_back(0);
			vClosedPolygon.push_back(1);
			vClosedPolygon.push_back(2);
			vClosedPolygon.push_back(3);
			vClosedPolygon.push_back(0);

			vInvalidPolygon.push_back(7);
		}
		virtual void TearDown() {
			model.reset();
		}

		PLib3MFModel model;
		PLib3MFSliceStack sliceStack;
		PLib3MFSlice slice;
		std::vector<sLib3MFPosition2D> vVertices;
		std::vector<Lib3MF_uint32> vOpenPolygon;
		std::vector<Lib3MF_uint32> vClosedPolygon;
		std::vector<Lib3MF_uint32> vEmptyPolygon;

		std::vector<Lib3MF_uint32> vInvalidPolygon;
	};


	TEST_F(Slice, Properties)
	{
		ASSERT_DOUBLE_EQ(slice->GetZTop(), 1.0);
		ASSERT_EQ(slice->GetVertexCount(), 0);
		ASSERT_EQ(slice->GetPolygonCount(), 0);
	}

	TEST_F(Slice, Vertices)
	{
		slice->SetVertices(vVertices);
		ASSERT_EQ(slice->GetVertexCount(), vVertices.size());

		std::vector<sLib3MFPosition2D> vNewVertices;
		slice->GetVertices(vNewVertices);
		ASSERT_EQ(vVertices.size(), vNewVertices.size());
		for (int i = 0; i < vVertices.size(); i++) {
			ASSERT_FLOAT_EQ(vVertices[i].m_coordinates[0], vNewVertices[i].m_coordinates[0]);
			ASSERT_FLOAT_EQ(vVertices[i].m_coordinates[1], vNewVertices[i].m_coordinates[1]);
		}
	}

	TEST_F(Slice, Polygons)
	{
		slice->SetVertices(vVertices);

		slice->AddPolygon(vOpenPolygon);
		ASSERT_EQ(slice->GetPolygonCount(), 1);

		ASSERT_EQ(slice->GetPolygonIndexCount(0), vOpenPolygon.size());

		std::vector<Lib3MF_uint32> vIndices;
		slice->GetPolygonIndices(0, vIndices);
		ASSERT_TRUE(vOpenPolygon == vIndices);

		slice->SetPolygonIndices(0, vClosedPolygon);
		slice->GetPolygonIndices(0, vIndices);
		ASSERT_TRUE(vClosedPolygon == vIndices);
	}

	TEST_F(Slice, PolygonsFail)
	{
		slice->SetVertices(vVertices);

		ASSERT_SPECIFIC_THROW(slice->AddPolygon(vInvalidPolygon), ELib3MFException);
	}

	TEST_F(Slice, ChangeVertices)
	{
		slice->SetVertices(vVertices);

		slice->AddPolygon(vOpenPolygon);
		ASSERT_EQ(slice->GetPolygonCount(), 1);

		slice->SetVertices(vVertices);
		ASSERT_EQ(slice->GetPolygonCount(), 0);
	}

}


