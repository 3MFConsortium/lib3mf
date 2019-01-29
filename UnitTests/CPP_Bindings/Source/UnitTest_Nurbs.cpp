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

UnitTest_Nurbs.cpp: Defines Unittests for the Nurbs Class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class Nurbs : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			mesh = model->AddMeshObject();
		}
		virtual void TearDown() {
			model.reset();
		}
	
		static PLib3MFModel model;
		static PLib3MFMeshObject mesh;

		static std::string OutFolder;
	};

	PLib3MFModel Nurbs::model;
	PLib3MFMeshObject Nurbs::mesh;

	std::string Nurbs::OutFolder(sOutFilesPath + "/Nurbs/");
	
	TEST_F(Nurbs, Export)
	{

		auto surface = Nurbs::model->AddNurbsSurface(3, 3, 4, 4);
		surface->AddKnotU(4, 0.0);
		surface->AddKnotU(4, 1.0);
		surface->AddKnotV(4, 0.0);
		surface->AddKnotV(4, 1.0);

		surface->AddUVCoordinate(0.5, 0.5);
		surface->AddUVCoordinate(0.75, 0.5);
		surface->AddUVCoordinate(0.5, 0.75);

		surface->SetControlPoint(0, 0, 0.0, 0.0, 1.0, 1.0);
		surface->SetControlPoint(1, 0, 0.0, 0.0, 1.0, 1.0 / 3.0);
		surface->SetControlPoint(2, 0, 0.0, 0.0, 1.0, 1.0 / 3.0);
		surface->SetControlPoint(3, 0, 0.0, 0.0, 1.0, 1.0);

		surface->SetControlPoint(0, 1, 2.0, 0.0, 1.0, 1.0 / 3.0);
		surface->SetControlPoint(1, 1, 2.0, 4.0, 1.0, 1.0 / 9.0);
		surface->SetControlPoint(2, 1, -2.0, 4.0, 1.0, 1.0 / 9.0);
		surface->SetControlPoint(3, 1, -2.0, 0.0, 1.0, 1.0 / 3.0);

		surface->SetControlPoint(0, 2, 2.0, 0.0, -1.0, 1.0 / 3.0);
		surface->SetControlPoint(1, 2, 2.0, 4.0, -1.0, 1.0 / 9.0);
		surface->SetControlPoint(2, 2, -2.0, 4.0, -1.0, 1.0 / 9.0);
		surface->SetControlPoint(3, 2, -2.0, 0.0, -1.0, 1.0 / 3.0);

		surface->SetControlPoint(0, 3, 0.0, 0.0, -1.0, 1.0);
		surface->SetControlPoint(1, 3, 0.0, 0.0, -1.0, 1.0 / 3.0);
		surface->SetControlPoint(2, 3, 0.0, 0.0, -1.0, 1.0 / 3.0);
		surface->SetControlPoint(3, 3, 0.0, 0.0, -1.0, 1.0);

		auto writer = Nurbs::model->QueryWriter("3mf");
		writer->WriteToFile(sOutFilesPath + "nurbstest1.3mf");
	}


}
