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

UnitTest_Thumbnails.cpp: Defines Unittests for reading and writing thumbnails of
a 3MF package or individual objects within a model.

--*/

#include "UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"

#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"


namespace NMR {

	bool LoadFile(const char* fileName) {
		using namespace NMR;
		CustomLib3MFBase pModel;
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		CustomLib3MFBase p3MFReader;
		EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
			(sTestFilesPath + separator() + "Mixed" + separator() + fileName).c_str());
		DWORD nWarningCount;
		lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount);
		EXPECT_EQ(nWarningCount, 0) << L"Warnings in the 3MF file : " << nWarningCount;
		for (DWORD iWarning = 0; iWarning < nWarningCount; iWarning++) {
			DWORD nErrorCode;
			wchar_t sWarning[4096];
			EXPECT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, sWarning, 4096, NULL), S_OK) << L"Could not get warning " << iWarning;
			EXPECT_TRUE(true) << iWarning << L": " << nErrorCode << ", " << sWarning;
		}

		DWORD nErrorCode;
		LPCSTR sErrorMessage;
		lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
		EXPECT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << hResult << ", " << nErrorCode << ", " << sErrorMessage;

		// investigate packagethumbnail
		{
			CustomLib3MFBase pAttachmentInstance;
			EXPECT_EQ(lib3mf_model_getpackagethumbnailattachment(pModel.get(), false, &pAttachmentInstance.get()), S_OK) << L"Could not determine whether package has thumbnail attachment";
			EXPECT_TRUE(pAttachmentInstance.get() != nullptr) << L"Package does not have thumbnail.";
			EXPECT_EQ(lib3mf_attachment_writetofileutf8(pAttachmentInstance.get(), (std::string("TestOutput") + separator() + "thumbnail.png").c_str()), S_OK) << L"Could not read thumbnail.";

			EXPECT_EQ(lib3mf_attachment_readfromfileutf8(pAttachmentInstance.get(), (sTestFilesPath + separator() + "Resources" + separator() + "thumbnail.png").c_str()), S_OK) << L"Could not read thumbnail.";
			EXPECT_EQ(lib3mf_attachment_setpathutf8(pAttachmentInstance.get(), "/Metadata/newthumbnail.png"), S_OK) << L"Could not set thumbnail path.";
		}

		// iterate build items
		CustomLib3MFBase pBuildItemIterator;
		EXPECT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

		BOOL bHasNext;
		EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		while (bHasNext) {
			CustomLib3MFBase pBuildItem;
			EXPECT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

			CustomLib3MFBase pObjectResource;
			EXPECT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Ccould not get build item resource";

			//BOOL bIsMeshObject;
			//EXPECT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";
			//if (bIsMeshObject) {
			//	investigateMeshObject(pModel, pObjectResource);
			//}
			//BOOL bIsComponentObject;
			//EXPECT_EQ(NMR::lib3mf_object_iscomponentsobject(pObjectResource.get(), &bIsComponentObject), S_OK) << L"Could not determine whether build item is components object";
			//if (bIsComponentObject) {
			//	investigateComponentsObject(pModel, pObjectResource);
			//}

			EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		}

		std::string sFileName = fileName;
		{
			// Re-Output file
			CustomLib3MFBase p3MFWriter;
			std::string sFileName = fileName;
			EXPECT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
			EXPECT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + +"ChangedPNG_" + fileName).c_str()), S_OK) << "Cannot write 3mf";
		}

		{
			// Change thumbnail and re-output file
			CustomLib3MFBase pAttachmentInstance;
			CustomLib3MFBase p3MFWriter;
			EXPECT_EQ(lib3mf_model_getpackagethumbnailattachment(pModel.get(), false, &pAttachmentInstance.get()), S_OK) << L"Could not determine whether package has thumbnail attachment";
			EXPECT_TRUE(pAttachmentInstance.get() != nullptr) << L"Package does not have thumbnail.";
			EXPECT_EQ(lib3mf_attachment_readfromfileutf8(pAttachmentInstance.get(), (sTestFilesPath + separator() + "Resources" + separator() + "thumbnail.jpg").c_str()), S_OK) << L"Could not read thumbnail.";
			EXPECT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
			EXPECT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + +"ChangedJPG_" + fileName).c_str()), S_OK) << "Cannot write 3mf";
		}

		{
			// Remove thumbnail and re-output file
			CustomLib3MFBase p3MFWriter;
			EXPECT_EQ(lib3mf_model_removepackagethumbnailattachment(pModel.get()), S_OK) << L"Could not remove package thumbnail.";
			EXPECT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
			EXPECT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + +"Removed_" + fileName).c_str()), S_OK) << "Cannot write 3mf";
		}

		return true;
	}

	bool LoadFileWithObjectThumbnail(std::string sPath, std::string sFileName, DWORD nExpectedWarnings) {
		using namespace NMR;
		CustomLib3MFBase pModel;
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		CustomLib3MFBase p3MFReader;
		EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
			(sPath + separator() + sFileName).c_str());
		DWORD nWarningCount;
		lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount);
		EXPECT_EQ(nWarningCount, nExpectedWarnings) << L"Warnings in the 3MF file : " << nWarningCount; // because of composite materials
		for (DWORD iWarning = 0; iWarning < nWarningCount; iWarning++) {
			DWORD nErrorCode;
			wchar_t sWarning[4096];
			EXPECT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, sWarning, 4096, NULL), S_OK) << L"Could not get warning " << iWarning;
			EXPECT_TRUE(true) << iWarning << L": " << nErrorCode << ", " << sWarning;
		}

		DWORD nErrorCode;
		LPCSTR sErrorMessage;
		lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
		EXPECT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << hResult << ", " << nErrorCode << ", " << sErrorMessage;

		// iterate build items
		CustomLib3MFBase pBuildItemIterator;
		EXPECT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

		BOOL bHasNext;
		EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		while (bHasNext) {
			CustomLib3MFBase pBuildItem;
			EXPECT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

			CustomLib3MFBase pObjectResource;
			EXPECT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Could not get build item resource";

			std::string sThumbnailPath;
			ULONG cbNeededChars;
			EXPECT_EQ(NMR::lib3mf_object_getthumbnailpathutf8(pObjectResource.get(), nullptr, 0, &cbNeededChars), S_OK) << L"Could not get length of thumbnail path";
			if (cbNeededChars > 0)
			{
				sThumbnailPath.resize(cbNeededChars);
				EXPECT_EQ(NMR::lib3mf_object_getthumbnailpathutf8(pObjectResource.get(), &sThumbnailPath[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get thumbnail path";

				CustomLib3MFBase pAttachment;
				EXPECT_EQ(NMR::lib3mf_model_findattachmentutf8(pModel.get(), &sThumbnailPath[0], &pAttachment.get()), S_OK) << L"Could not find thumbnail-attachment";
			}


			//EXPECT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";
			//if (bIsMeshObject) {
			//	investigateMeshObject(pModel, pObjectResource);
			//}
			//BOOL bIsComponentObject;
			//EXPECT_EQ(NMR::lib3mf_object_iscomponentsobject(pObjectResource.get(), &bIsComponentObject), S_OK) << L"Could not determine whether build item is components object";
			//if (bIsComponentObject) {
			//	investigateComponentsObject(pModel, pObjectResource);
			//}

			EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		}

		{
			// Re-Output file
			CustomLib3MFBase p3MFWriter;
			EXPECT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
			EXPECT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + +"ReOut_" + sFileName).c_str()), S_OK) << "Cannot write 3mf";
		}

		return true;
	}

	void CreateThumbnailedModel(CustomLib3MFBase &pModel) {
		// Create Model Instance
		ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";

		CustomLib3MFBase pAttachmentOtherType;
		ASSERT_EQ(lib3mf_model_addattachmentutf8(pModel.get(), "/3D/Textures/Other.xml", "OTHERRELATIONSHIP", &pAttachmentOtherType.get()), S_OK) << L"Could not add attachment.";

		std::vector<CustomLib3MFBase> vctAttachments(3);
		for (size_t i=0; i<vctAttachments.size(); i++)
		{
			std::string sAttachmentPath = std::string(TESTFILESPATH) + separator() + "Textures" + separator() + "tex" + std::to_string(i + 1) +".png";
			ASSERT_EQ(lib3mf_model_addattachmentutf8(pModel.get(), &("/3D/Textures/tex" + std::to_string(i + 1) + ".png")[0], "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture", &vctAttachments[i].get()), S_OK) << L"Could not add attachment.";
			ASSERT_EQ(lib3mf_attachment_readfromfileutf8(vctAttachments[i].get(), &sAttachmentPath[0]), S_OK) << L"Could not read attachment.";
		}

		CustomLib3MFBase pMeshObject;
		ASSERT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << L"Could not add mesh object.";
		FillMeshObject(pMeshObject);
		CustomLib3MFBase pMeshObjectInComponent;
		ASSERT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObjectInComponent.get()), S_OK) << L"Could not add mesh object.";
		FillMeshObject(pMeshObjectInComponent);
		CustomLib3MFBase pComponentsObject;
		ASSERT_EQ(lib3mf_model_addcomponentsobject(pModel.get(), &pComponentsObject.get()), S_OK) << L"Could not add components object.";
		
		CustomLib3MFBase pComponentObject;
		ASSERT_EQ(lib3mf_componentsobject_addcomponent(pComponentsObject.get(), pMeshObjectInComponent.get(), nullptr, &pComponentObject.get()), S_OK) << L"Could not add cpmponent";
		
		ASSERT_EQ(lib3mf_object_setthumbnailpathutf8(pMeshObject.get(), &("/3D/Textures/tex" + std::to_string(0 + 1) + ".png")[0]), S_OK) << L"Could not set thumbnail path";
		ASSERT_EQ(lib3mf_object_setthumbnailpathutf8(pMeshObjectInComponent.get(), &("/3D/Textures/tex" + std::to_string(1 + 1) + ".png")[0]), S_OK) << L"Could not set thumbnail path";
		ASSERT_EQ(lib3mf_object_setthumbnailpathutf8(pComponentsObject.get(), &("/3D/Textures/tex" + std::to_string(2 + 1) + ".png")[0]), S_OK) << L"Could not set thumbnail path";
		{
			CustomLib3MFBase pBuildItem;
			ASSERT_EQ(lib3mf_model_addbuilditem(pModel.get(), pMeshObject.get(), nullptr, &pBuildItem.get()), S_OK) << L"Could not add build item";
		}
		{
			CustomLib3MFBase pBuildItem;
			ASSERT_EQ(lib3mf_model_addbuilditem(pModel.get(), pComponentsObject.get(), nullptr, &pBuildItem.get()), S_OK) << L"Could not add build item";
		}
	}

	TEST(Thumbnails, PackageThumbnail_ReadWrite)
	{
		LoadFile("Thumbnail_Package.3mf");
	}

	TEST(Thumbnails, ObjectThumbnail_Read)
	{
		LoadFileWithObjectThumbnail(sTestFilesPath + separator() + "Mixed" + separator(), "texcube.3mf", 2);
	}

	TEST(Thumbnails, ObjectThumbnail_ReadWrite)
	{
		CustomLib3MFBase pModel;
		CreateThumbnailedModel(pModel);
		CustomLib3MFBase p3MFWriter;
		// std::string sFileName = std::string("TestOutput") + separator() + "objectThumbnails.3mf";
		std::string sPath = std::string("TestOutput") + separator();
		std::string sFileName = "objectThumbnails.3mf";
		EXPECT_EQ(lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << L"Could not create modelwriter";
		EXPECT_EQ(lib3mf_writer_writetofileutf8(p3MFWriter.get(), (sPath+sFileName).c_str() ), S_OK) << L"Could not write file";
		
		LoadFileWithObjectThumbnail(sPath, sFileName, 0);
	}
	
}

