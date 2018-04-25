/*++

Copyright (C) 2017 Autodesk, Inc.
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

UnitTest_Textures.cpp: Defines Unittests for reading and writing textures from/to
3mf files and models.

--*/

#include "UnitTests/UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"

#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"


namespace NMR {

	LIB3MFRESULT fnLoadModelTexture(CustomLib3MFBase& pModel, const char * pszPath, const char * pszFile, eModelTexture2DType eType, eModelTextureTileStyle eTileStyleU, eModelTextureTileStyle eTileStyleV, DWORD & nTextureID, int useAttachmentMode = 0)
	{
		CustomLib3MFBase pTexture2D;
		if (useAttachmentMode ==0) {
			CustomLib3MFBase pAttachment;
			EXPECT_EQ(lib3mf_model_addattachmentutf8(pModel.get(), const_cast<char *>(pszPath), const_cast<char *>("http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture"), &pAttachment.get()) , S_OK) << L"Could not add attachment";
			EXPECT_EQ(lib3mf_model_addtexture2dfromattachment(pModel.get(), pAttachment.get(), &pTexture2D.get()), S_OK) << L"Could not add texture from attachment";
			EXPECT_EQ(lib3mf_attachment_readfromfileutf8(pAttachment.get(), pszFile), S_OK) << L"Could not read textureattachment file";
		}
		else if (useAttachmentMode == 1)
		{
			CustomLib3MFBase pAttachment;
			EXPECT_EQ(lib3mf_model_addtexture2dutf8(pModel.get(), pszPath, &pTexture2D.get()), S_OK) << L"Could not add texture";
			EXPECT_EQ(lib3mf_texture2d_readfromfileutf8(pTexture2D.get(), pszFile), S_OK) << L"Could not load texture";
		}
		else
		{
			CustomLib3MFBase pAttachment;
			EXPECT_EQ(lib3mf_model_addattachmentutf8(pModel.get(), const_cast<char *>(pszPath), const_cast<char *>("http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture"), &pAttachment.get()), S_OK) << L"Could not add attachment";
			EXPECT_EQ(lib3mf_model_addtexture2dutf8(pModel.get(), pszPath, &pTexture2D.get()), S_OK) << L"Could not add texture";
			EXPECT_EQ(lib3mf_attachment_readfromfileutf8(pAttachment.get(), pszFile), S_OK) << L"Could not read textureattachment file";
			EXPECT_EQ(lib3mf_texture2d_setattachment(pTexture2D.get(), pAttachment.get()), S_OK) << L"Could not set attachment for texture";
		}
		CustomLib3MFBase pAttachment;
		EXPECT_EQ(lib3mf_texture2d_getattachment(pTexture2D.get(), &pAttachment.get()), S_OK) << L"Could not get texture attachment";
		ULONG cbNeededChars;
		EXPECT_EQ(lib3mf_attachment_getpathutf8(pAttachment.get(), nullptr, 0, &cbNeededChars), S_OK) << L"Could not get attachment path";
		std::string sAttachmentPath;
		sAttachmentPath.resize(cbNeededChars);
		EXPECT_EQ(lib3mf_attachment_getpathutf8(pAttachment.get(), &(sAttachmentPath[0]), cbNeededChars+1, &cbNeededChars), S_OK) << L"Could not get attachment path";
		EXPECT_TRUE(sAttachmentPath == std::string(pszPath)) << L"Attachment path";

		EXPECT_EQ(lib3mf_resource_getresourceid(pTexture2D.get(), &nTextureID), S_OK) << L"Could not load texture";
		EXPECT_EQ(lib3mf_texture2d_setcontenttype(pTexture2D.get(), eType), S_OK) << L"Could not set content type";
		EXPECT_EQ(lib3mf_texture2d_clearbox2d(pTexture2D.get()), S_OK) << L"Could not clear box2d";
		EXPECT_EQ(lib3mf_texture2d_setbox2d(pTexture2D.get(), 0.1f, 0.2f, 0.8f, 0.9f), S_OK) << L"Could not set box2d";

		EXPECT_EQ(lib3mf_texture2d_settilestyleuv(pTexture2D.get(), eTileStyleU, eTileStyleV), S_OK) << L"Could not set tilestyle";
		return 0;
	}

	MODELMESHTEXTURE2D fnCreateTexture(float u1, float v1, float u2, float v2, float u3, float v3, unsigned int textureid)
	{
		MODELMESHTEXTURE2D result;
		result.m_fU[0] = u1;
		result.m_fV[0] = v1;
		result.m_fU[1] = u2;
		result.m_fV[1] = v2;
		result.m_fU[2] = u3;
		result.m_fV[2] = v3;
		result.m_nTextureID = textureid;
		return result;
	}
	
	void CreateBox(std::vector<MODELMESHVERTEX> &vctVertices, std::vector<MODELMESHTRIANGLE> &vctTriangles) {
		float fSizeX = 100.0f;
		float fSizeY = 100.0f;
		float fSizeZ = 100.0f;

		vctVertices.resize(8);
		vctTriangles.resize(12);

		// Manually create vertices
		vctVertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
		vctVertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
		vctVertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
		vctVertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
		vctVertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
		vctVertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
		vctVertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
		vctVertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);


		// Manually create triangles
		vctTriangles[0] = fnCreateTriangle(2, 1, 0);
		vctTriangles[1] = fnCreateTriangle(0, 3, 2);
		vctTriangles[2] = fnCreateTriangle(4, 5, 6);
		vctTriangles[3] = fnCreateTriangle(6, 7, 4);
		vctTriangles[4] = fnCreateTriangle(0, 1, 5);
		vctTriangles[5] = fnCreateTriangle(5, 4, 0);
		vctTriangles[6] = fnCreateTriangle(2, 3, 7);
		vctTriangles[7] = fnCreateTriangle(7, 6, 2);
		vctTriangles[8] = fnCreateTriangle(1, 2, 6);
		vctTriangles[9] = fnCreateTriangle(6, 5, 1);
		vctTriangles[10] = fnCreateTriangle(3, 0, 4);
		vctTriangles[11] = fnCreateTriangle(4, 7, 3);
	}

	bool CreateTexturedModel(CustomLib3MFBase &pModel) {
		// Create Model Instance
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";

		// Create Mesh Object
		CustomLib3MFBase pMeshObject;
		EXPECT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << L"Could not add 3MF model";

		// Create mesh structure of a cube
		std::vector<MODELMESHVERTEX> vctVertices;
		std::vector<MODELMESHTRIANGLE> vctTriangles;
		CreateBox(vctVertices, vctTriangles);

		EXPECT_EQ(lib3mf_meshobject_setgeometry(pMeshObject.get(), &vctVertices[0], (DWORD)vctVertices.size(), &vctTriangles[0], (DWORD)vctTriangles.size()), S_OK) << L"Could not set mesh geometry";

		CustomLib3MFBase pPropertyHandler;
		// Create texture entries for cube
		EXPECT_EQ(lib3mf_meshobject_createpropertyhandler(pMeshObject.get(), &pPropertyHandler.get()), S_OK) << L"Could not create property handler";

		DWORD nTextureIDA, nTextureIDB, nTextureIDC;
		std::string sAttachmentPath = std::string(TESTFILESPATH) + separator() + "Textures" + separator() + "tex1.png";
		fnLoadModelTexture(pModel, "/3D/Textures/tex1.png", sAttachmentPath.c_str(), MODELTEXTURETYPE_PNG, MODELTEXTURETILESTYLE_WRAP, MODELTEXTURETILESTYLE_WRAP, nTextureIDA, 0);
		sAttachmentPath = std::string(TESTFILESPATH) + separator() + "Textures" + separator() + "tex2.png";
		fnLoadModelTexture(pModel, "/3D/Textures/tex2.png", sAttachmentPath.c_str(), MODELTEXTURETYPE_PNG, MODELTEXTURETILESTYLE_MIRROR, MODELTEXTURETILESTYLE_WRAP, nTextureIDB, 1);
		sAttachmentPath = std::string(TESTFILESPATH) + separator() + "Textures" + separator() + "tex3.png";
		fnLoadModelTexture(pModel, "/3D/Textures/tex3.png", sAttachmentPath.c_str(), MODELTEXTURETYPE_PNG, MODELTEXTURETILESTYLE_CLAMP, MODELTEXTURETILESTYLE_MIRROR, nTextureIDC, 2);

		MODELMESHTEXTURE2D sTexture1, sTexture2;
		// Side 1
		float fLow = -0.5f;
		float fHigh = 1.5f;
		sTexture1 = fnCreateTexture(fLow, fLow, fHigh, fLow, fHigh, fHigh, nTextureIDA);
		sTexture2 = fnCreateTexture(fHigh, fHigh, fLow, fHigh, fLow, fLow, nTextureIDA);
		lib3mf_propertyhandler_settexture(pPropertyHandler.get(), 0, &sTexture1);
		lib3mf_propertyhandler_settexture(pPropertyHandler.get(), 1, &sTexture2);

		// Side 2
		fLow = -1.5f;
		fHigh = 2.5f;
		sTexture1 = fnCreateTexture(fLow, fLow, fHigh, fLow, fHigh, fHigh, nTextureIDB);
		sTexture2 = fnCreateTexture(fHigh, fHigh, fLow, 1.0f, fLow, fLow, nTextureIDB);
		lib3mf_propertyhandler_settexture(pPropertyHandler.get(), 2, &sTexture1);
		lib3mf_propertyhandler_settexture(pPropertyHandler.get(), 3, &sTexture2);

		// Side 3
		sTexture1 = fnCreateTexture(fLow, fLow, fHigh, 0.0f, fHigh, fHigh, nTextureIDC);
		sTexture2 = fnCreateTexture(fHigh, fHigh, fLow, fHigh, fLow, fLow, nTextureIDC);
		lib3mf_propertyhandler_settexture(pPropertyHandler.get(), 4, &sTexture1);
		lib3mf_propertyhandler_settexture(pPropertyHandler.get(), 5, &sTexture2);

		CustomLib3MFBase pDefaultPropertyHandler;
		EXPECT_EQ(lib3mf_object_createdefaultpropertyhandler(pMeshObject.get(), &pDefaultPropertyHandler.get()), S_OK) << L"Could not create default property handler";
		EXPECT_EQ(lib3mf_defaultpropertyhandler_settexture(pDefaultPropertyHandler.get(), nTextureIDB, 0.0, 0.0), S_OK) << L"Could not set default property";

		CustomLib3MFBase pBuildItem;
		EXPECT_EQ(lib3mf_model_addbuilditem(pModel.get(), pMeshObject.get(), NULL, &pBuildItem.get()), S_OK) << L"Could not create build item.";

		return true;
	}

	bool CreateMultiTexturedModel(CustomLib3MFBase &pModel) {
		// Create Model Instance
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";

		// Create Mesh Object
		CustomLib3MFBase pMeshObject;
		EXPECT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << L"Could not add 3MF model";

		// Create mesh structure of a cube
		std::vector<MODELMESHVERTEX> vctVertices;
		std::vector<MODELMESHTRIANGLE> vctTriangles;
		CreateBox(vctVertices, vctTriangles);
		
		EXPECT_EQ(lib3mf_meshobject_setgeometry(pMeshObject.get(), &vctVertices[0], (DWORD)vctVertices.size(), &vctTriangles[0], (DWORD)vctTriangles.size()), S_OK) << L"Could not set mesh geometry";

		CustomLib3MFBase pPropertyHandlerA;
		CustomLib3MFBase pPropertyHandlerB;
		// Create texture entries for cube
		EXPECT_EQ(lib3mf_meshobject_createmultipropertyhandler(pMeshObject.get(), 0, &pPropertyHandlerA.get()), S_OK) << L"Could not create multi-property handler A";
		EXPECT_EQ(lib3mf_meshobject_createmultipropertyhandler(pMeshObject.get(), 1, &pPropertyHandlerB.get()), S_OK) << L"Could not create multi-property handler B";

		DWORD nTextureIDA, nTextureIDB, nTextureIDC;
		std::string sAttachmentPath = std::string(TESTFILESPATH) + separator() + "Textures" + separator() + "tex1.png";
		fnLoadModelTexture(pModel, "/3D/Textures/tex1.png", sAttachmentPath.c_str(), MODELTEXTURETYPE_PNG, MODELTEXTURETILESTYLE_WRAP, MODELTEXTURETILESTYLE_WRAP ,nTextureIDA, 0);
		sAttachmentPath = std::string(TESTFILESPATH) + separator() + "Textures" + separator() + "tex2.png";
		fnLoadModelTexture(pModel, "/3D/Textures/tex2.png", sAttachmentPath.c_str(), MODELTEXTURETYPE_PNG, MODELTEXTURETILESTYLE_MIRROR, MODELTEXTURETILESTYLE_WRAP, nTextureIDB, 1);
		sAttachmentPath = std::string(TESTFILESPATH) + separator() + "Textures" + separator() + "tex3.png";
		fnLoadModelTexture(pModel, "/3D/Textures/tex3.png", sAttachmentPath.c_str(), MODELTEXTURETYPE_PNG, MODELTEXTURETILESTYLE_CLAMP, MODELTEXTURETILESTYLE_MIRROR, nTextureIDC, 2);

		MODELMESHTEXTURE2D sTexture1, sTexture2;
		// Side 1
		float fLow = -0.5f;
		float fHigh = 1.5f;
		sTexture1 = fnCreateTexture(fLow, fLow, fHigh, fLow, fHigh, fHigh, nTextureIDA);
		sTexture2 = fnCreateTexture(fHigh, fHigh, fLow, fHigh, fLow, fLow, nTextureIDA);
		lib3mf_propertyhandler_settexture(pPropertyHandlerA.get(), 0, &sTexture1);
		lib3mf_propertyhandler_settexture(pPropertyHandlerA.get(), 1, &sTexture2);
		lib3mf_propertyhandler_settexture(pPropertyHandlerB.get(), 0, &sTexture2);
		lib3mf_propertyhandler_settexture(pPropertyHandlerB.get(), 1, &sTexture1);

		// Side 2
		fLow = -1.5f;
		fHigh = 2.5f;
		sTexture1 = fnCreateTexture(fLow, fLow, fHigh, fLow, fHigh, fHigh, nTextureIDB);
		sTexture2 = fnCreateTexture(fHigh, fHigh, fLow, 1.0f, fLow, fLow, nTextureIDB);
		lib3mf_propertyhandler_settexture(pPropertyHandlerA.get(), 2, &sTexture1);
		lib3mf_propertyhandler_settexture(pPropertyHandlerA.get(), 3, &sTexture2);
		lib3mf_propertyhandler_settexture(pPropertyHandlerB.get(), 2, &sTexture2);
		lib3mf_propertyhandler_settexture(pPropertyHandlerB.get(), 3, &sTexture1);

		// Side 3
		sTexture1 = fnCreateTexture(fLow, fLow, fHigh, 0.0f, fHigh, fHigh, nTextureIDC);
		sTexture2 = fnCreateTexture(fHigh, fHigh, fLow, fHigh, fLow, fLow, nTextureIDC);
		lib3mf_propertyhandler_settexture(pPropertyHandlerA.get(), 4, &sTexture1);
		lib3mf_propertyhandler_settexture(pPropertyHandlerA.get(), 5, &sTexture2);
		lib3mf_propertyhandler_settexture(pPropertyHandlerB.get(), 4, &sTexture2);
		lib3mf_propertyhandler_settexture(pPropertyHandlerB.get(), 5, &sTexture1);
		
		CustomLib3MFBase pDefaultPropertyHandlerA;
		CustomLib3MFBase pDefaultPropertyHandlerB;
		EXPECT_EQ(lib3mf_object_createdefaultmultipropertyhandler(pMeshObject.get(), 0, &pDefaultPropertyHandlerA.get()), S_OK) << L"Could not create default property handler A";
		EXPECT_EQ(lib3mf_defaultpropertyhandler_settexture(pDefaultPropertyHandlerA.get(), nTextureIDB, 0.0, 0.0), S_OK) << L"Could not set default property";
		EXPECT_EQ(lib3mf_object_createdefaultmultipropertyhandler(pMeshObject.get(), 1, &pDefaultPropertyHandlerB.get()), S_OK) << L"Could not create default property handler B";
		EXPECT_EQ(lib3mf_defaultpropertyhandler_settexture(pDefaultPropertyHandlerB.get(), nTextureIDB, 0.0, 0.0), S_OK) << L"Could not set default property";

		CustomLib3MFBase pBuildItem;
		EXPECT_EQ(lib3mf_model_addbuilditem(pModel.get(), pMeshObject.get(), NULL, &pBuildItem.get()), S_OK) << L"Could not create build item.";

		return true;
	}

	void Investigate2DTexture(CustomLib3MFBase &pTexture2D)
	{
		FLOAT fU, fV, fwU, fwV;
		ASSERT_EQ(lib3mf_texture2d_getbox2d(pTexture2D.get(), &fU, &fV, &fwU, &fwV), S_OK) << L"Could not get the box2d";
		ASSERT_FLOAT_EQ(fU,  0.1f) << L"The coordinates of the box are not as expected";
		ASSERT_FLOAT_EQ(fV,  0.2f) << L"The coordinates of the box are not as expected";
		ASSERT_FLOAT_EQ(fwU, 0.8f) << L"The coordinates of the box are not as expected";
		ASSERT_FLOAT_EQ(fwV, 0.9f) << L"The coordinates of the box are not as expected";

		eModelTexture2DType eContentType;
		ASSERT_EQ(lib3mf_texture2d_getcontenttype(pTexture2D.get(), &eContentType), S_OK) << L"Could not get contenttype";
		EXPECT_TRUE((eContentType == MODELTEXTURETYPE_UNKNOWN) || (eContentType == MODELTEXTURETYPE_PNG) || (eContentType == MODELTEXTURETYPE_JPEG)) << L"Invalid content type";

		eModelTextureTileStyle eTileStyleU, eTileStyleV;
		ASSERT_EQ(lib3mf_texture2d_gettilestyleuv(pTexture2D.get(), &eTileStyleU, &eTileStyleV), S_OK) << L"Could not get tilestyle";
		EXPECT_TRUE((eTileStyleU == MODELTEXTURETILESTYLE_MIRROR) || (eTileStyleU == MODELTEXTURETILESTYLE_WRAP) || (eTileStyleU == MODELTEXTURETILESTYLE_CLAMP)) << L"Invalid tilestyle";
		EXPECT_TRUE((eTileStyleV == MODELTEXTURETILESTYLE_MIRROR) || (eTileStyleV == MODELTEXTURETILESTYLE_WRAP) || (eTileStyleV == MODELTEXTURETILESTYLE_CLAMP)) << L"Invalid tilestyle";

		CustomLib3MFBase pAttachment;
		ASSERT_EQ(lib3mf_texture2d_getattachment(pTexture2D.get(), &pAttachment.get()), S_OK) << L"Could not get texture2D attachment";

		ULONG64 cbStreamSize;
		ASSERT_EQ(lib3mf_attachment_getstreamsize(pAttachment.get(), &cbStreamSize), S_OK) << L"Could not stream size";
	}

	void InvestigateTexturedModel(CustomLib3MFBase &pModel)
	{
		DWORD nCount;
		ASSERT_EQ(lib3mf_model_getattachmentcount(pModel.get(), &nCount), S_OK) << L"Could not get number of attachments";
		ASSERT_GE(nCount, DWORD(1)) << "Expected at least one texture stream in the file.";

		CustomLib3MFBase pTexture2DResources;
		ASSERT_EQ(lib3mf_model_get2dtextures(pModel.get(), &pTexture2DResources.get()), S_OK) << L"Could not get texture2dresourcess";

		BOOL bHasNext;
		ASSERT_EQ(lib3mf_resourceiterator_movenext(pTexture2DResources.get(), &bHasNext), S_OK) << L"Could not move texture2dresources";
		while (bHasNext)
		{
			CustomLib3MFBase pTexture2DResource;
			ASSERT_EQ(lib3mf_resourceiterator_getcurrent(pTexture2DResources.get(), &pTexture2DResource.get()), S_OK) << L"Could not get texture2dresources";
			DWORD nResourceID;
			ASSERT_EQ(lib3mf_resource_getresourceid(pTexture2DResource.get(), &nResourceID), S_OK) << L"Could not get texture2dresrouce ID";
			CustomLib3MFBase pTexture2D;
			ASSERT_EQ(lib3mf_model_gettexture2dbyid(pModel.get(), nResourceID, &pTexture2D.get()), S_OK) << L"Could not get texture2d ";

			Investigate2DTexture(pTexture2D);

			ASSERT_EQ(lib3mf_resourceiterator_movenext(pTexture2DResources.get(), &bHasNext), S_OK) << L"Could not move 2dtexturesresources";
		}


		// TODO: investigate file further
		// checkout stream, checkout individual triangles

	}

	TEST(Textures, Textures_CreateTexturedFile)
	{
		CustomLib3MFBase pModel;
		CreateTexturedModel(pModel);
		CustomLib3MFBase p3MFWriter;
		EXPECT_EQ(lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << L"Could not create modelwriter";

		EXPECT_EQ(lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput")+separator()+"textures.3mf").c_str()), S_OK) << L"Could not write file";
	}

	/*
	TEST(Textures, Textures_CreateMultiTexturedFile)
	{
		CustomLib3MFBase pModel;
		CreateMultiTexturedModel(pModel);
		CustomLib3MFBase p3MFWriter;
		EXPECT_EQ(lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << L"Could not create modelwriter";

		EXPECT_EQ(lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + "multitextures.3mf").c_str()), S_OK) << L"Could not write file";
	}
	*/

	TEST(Textures, Textures_ReadTexturedFile)
	{
		CustomLib3MFBase pModel;
		CustomLib3MFBase p3MFReader;
		ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could not create modelreader";
		ASSERT_EQ(lib3mf_reader_readfromfileutf8(p3MFReader.get(), (std::string("TestOutput") + separator() + "textures.3mf").c_str()), S_OK) << L"Could not read file";

		InvestigateTexturedModel(pModel);
	}

}

