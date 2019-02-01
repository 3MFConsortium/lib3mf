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

UnitTest_TextureResources.cpp: Defines Unittests for the Texture Resource class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class Model_TextureResource : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			m_sRelationshipType = "http://schemas.autodesk.com/dmg/testattachment/2017/08";
			m_sPath = "/Resources/attachment.xml";
			m_sRelationshipType_Texture = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";
			m_sPath_Texture = "/3D/Textures/MyTexture.png";

			model = CLib3MFWrapper::CreateModel();
			otherAttachment = model->AddAttachment(m_sPath, m_sRelationshipType);
			textureAttachment = model->AddAttachment(m_sPath_Texture, m_sRelationshipType_Texture);
		}
		virtual void TearDown() {
			model.reset();
		}

		PLib3MFModel model;
		PLib3MFAttachment otherAttachment;
		PLib3MFAttachment textureAttachment;
		std::string m_sPath;
		std::string m_sRelationshipType;
		std::string m_sRelationshipType_Texture;
		std::string m_sPath_Texture;
	};

	TEST_F(Model_TextureResource, AddTexture)
	{
		ASSERT_SPECIFIC_THROW(model->AddTexture2DFromAttachment(otherAttachment.get()), ELib3MFException);
		
		auto texture = model->AddTexture2DFromAttachment(textureAttachment.get());
	}

}

