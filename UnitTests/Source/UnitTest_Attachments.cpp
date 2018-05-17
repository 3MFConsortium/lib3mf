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

UnitTest_Attachments.cpp: Defines Unittests for reading and writing of attachments
in 3MF files.

--*/

#include "UnitTests/UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"

#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"

namespace NMR {
	
	class DLLInterfacesTester_Attachments
	{
	private:
		const std::string m_sFolderName;
		const std::string m_sFilenameReadWrite;
		const std::string m_sRelationShipPath;
		const std::string m_sRelationShipPath_file;
		const std::string m_sAttachmetType;
		const std::string m_sAttachmetPayload;
	public:
		
		DLLInterfacesTester_Attachments() :
			m_sFolderName("TestOutput"), m_sFilenameReadWrite("output_dllattachment.3mf"),
			m_sRelationShipPath("/Attachments/test.xml"), 
			m_sRelationShipPath_file("/Attachments/test fromfile.xml"),
			m_sAttachmetType("http://schemas.autodesk.com/dmg/testattachment/2017/08"),
			m_sAttachmetPayload("PAYLOAD")
		{
			
		}

		void Write3MF(bool writeAttachment) {
			CustomLib3MFBase pModel;

			// Create Model Instance
			ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create model";
			
			{
				CustomLib3MFBase pAttachmentInstance;
				ASSERT_EQ(NMR::lib3mf_model_addattachmentutf8(pModel.get(), const_cast<char *>(m_sRelationShipPath.c_str()), const_cast<char *>(m_sAttachmetType.c_str()), &pAttachmentInstance.get()), S_OK)
					<< L"Could not add attachment";

				ASSERT_EQ(NMR::lib3mf_attachment_readfrombuffer(pAttachmentInstance.get(), (BYTE *)(m_sAttachmetPayload.c_str()), m_sAttachmetPayload.size()), S_OK)
					<< L"Could not read attachment from buffer";
			}

			{
				CustomLib3MFBase pAttachmentInstance;
				ASSERT_EQ(NMR::lib3mf_model_addattachmentutf8(pModel.get(), const_cast<char *>(m_sRelationShipPath_file.c_str()), const_cast<char *>(m_sAttachmetType.c_str()), &pAttachmentInstance.get()), S_OK)
					<< L"Could not add attachment";
				std::string sAttachmentPath = std::string(TESTFILESPATH) + separator() + "Resources" + separator() + "attachment.xml";
				ASSERT_EQ(NMR::lib3mf_attachment_readfromfileutf8(pAttachmentInstance.get(), sAttachmentPath.c_str()), S_OK)
					<< L"Could not read attachment from file";
			}

			ASSERT_EQ(NMR::lib3mf_model_addcustomcontenttypeutf8(pModel.get(), "xml", "application/xml"), S_OK) 
				<< L"Could not add custom contenttype attachment from buffer";

			CustomLib3MFBase p3MFWriter;
			ASSERT_EQ(lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << L"Could create Model Writer";
			
			ASSERT_TRUE(CreateDir(m_sFolderName.c_str())) << L"Could not create folder.";
			ASSERT_EQ(lib3mf_writer_writetofileutf8(p3MFWriter.get(),
				(m_sFolderName + separator() + m_sFilenameReadWrite).c_str()),
				S_OK) << L"Could not write 3MF file.";
		}

		void Read3MFAttachments(CustomLib3MFBase& pModel) {
			// handle model attachments
			DWORD nCount = 0;
			ASSERT_EQ(NMR::lib3mf_model_getattachmentcount(pModel.get(), &nCount), S_OK) << L"Could not get attachment count";
			ASSERT_EQ(nCount, 2) << L"Incorrect number of Attachments";
			// 
			// iterate attachments to find known relationship types
			for (DWORD iAttachment = 0; iAttachment<nCount; iAttachment++) {
				CustomLib3MFBase pAttachment;
				ASSERT_EQ(NMR::lib3mf_model_getattachment(pModel.get(), iAttachment, &pAttachment.get()), S_OK) << L"Could not get attachment ";

				// get relationship type
				ULONG cbNeededChars;
				ASSERT_EQ(NMR::lib3mf_attachment_getrelationshiptypeutf8(pAttachment.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length relationship type";
				
				std::string strRelType;
				strRelType.resize(cbNeededChars );
				ASSERT_EQ(NMR::lib3mf_attachment_getrelationshiptypeutf8(pAttachment.get(), &strRelType[0], cbNeededChars + 1, &cbNeededChars), S_OK)
					<< L"could not get type of attachment relationship";

				// check, whether correct attachment type
				ASSERT_TRUE(strRelType == m_sAttachmetType) << L"Wrong attachment type";
				ULONG64 cbStreamSize;
				ASSERT_EQ(NMR::lib3mf_attachment_getstreamsize(pAttachment.get(), &cbStreamSize), S_OK)
					<< L"could not get attachment stream size";
					
				// get surface regions as xml
				std::string strAttachmentPayload;
				strAttachmentPayload.resize(static_cast<size_t>(cbStreamSize));
				ASSERT_EQ(NMR::lib3mf_attachment_writetobuffer(pAttachment.get(), (BYTE *)&strAttachmentPayload[0], cbStreamSize), S_OK)
					<< L"could not write model attachment to buffer";

				ASSERT_TRUE(strAttachmentPayload == m_sAttachmetPayload) << L"Attachment Payload is incorrect";

			}
		}

		void Read3MF (bool readAttachment){
			HRESULT hResult;
			CustomLib3MFBase pModel;
			ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
			{
				CustomLib3MFBase p3MFReader;
				ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
				ASSERT_EQ(NMR::lib3mf_reader_addrelationtoreadutf8(p3MFReader.get(), m_sAttachmetType.c_str()), S_OK) << L"Could not add relation to read";

				hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
					(m_sFolderName + separator() + m_sFilenameReadWrite).c_str());
				if (hResult != S_OK) {
					DWORD errorCode;
					LPCSTR errString;
					lib3mf_getlasterror(p3MFReader.get(), &errorCode, &errString);
					std::string errorMessage = std::string(errString);
				}
				ASSERT_EQ(hResult, S_OK) << L"Could not read 3MF file.";
			}
			Read3MFAttachments(pModel);
		}
	};
	
	TEST(Attachments, WriteAttachment)
	{
		DLLInterfacesTester_Attachments interfaceTester;
		interfaceTester.Write3MF(true);
	}

	TEST(Attachments, ReadAttachment)
	{
		DLLInterfacesTester_Attachments interfaceTester;
		interfaceTester.Read3MF(true);
	}
}

