/*++

Copyright (C) 2019 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MICROSOFT AND/OR NETFABB BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

Converter.cpp : Can convert 3MFs to STL and back

--*/

#include <iostream>
#include <string>
#include <algorithm>

#ifndef __GNUC__
#include <Windows.h>
#endif

#include "lib3mf.hpp"

using namespace Lib3MF;


void printVersion() {
	Lib3MF_uint32 nMajor, nMinor, nMicro;
	std::string sReleaseInfo, sBuildInfo;
	CLib3MFWrapper::GetLibraryVersion(nMajor, nMinor, nMicro, sReleaseInfo, sBuildInfo);
	std::cout << "Lib3MF version = " << nMajor << "." << nMinor << "." << nMicro;
	if (!sReleaseInfo.empty()) {
		std::cout << "-" << sReleaseInfo;
	}
	if (!sBuildInfo.empty()) {
		std::cout << "+" << sBuildInfo;
	}
	std::cout << std::endl;
}

std::string FindExtension(std::string filename) {
	// this emulates Windows' PathFindExtension
	std::string::size_type idx;
	idx = filename.rfind('.');

	if (idx != std::string::npos)
	{
		return filename.substr(idx);
	}
	else
	{
		return "";
	}
}


int convert(std::string sFilename) {
	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Model Converter" << std::endl;
	printVersion();
	std::cout << "------------------------------------------------------------------" << std::endl;

	// Extract Extension of filename
	std::string sReaderName;
	std::string sWriterName;
	std::string sNewExtension;
	std::string sExtension = FindExtension(sFilename);
	std::transform(sExtension.begin(), sExtension.end(), sExtension.begin(), ::tolower);

	// Which Reader and Writer classes do we need?
	if (sExtension == ".stl") {
		sReaderName = "stl";
		sWriterName = "3mf";
		sNewExtension = ".3mf";
	}
	if (sExtension == ".3mf") {
		sReaderName = "3mf";
		sWriterName = "stl";
		sNewExtension = ".stl";
	}
	if (sReaderName.length() == 0) {
		std::cout << "unknown input file extension:" << sExtension << std::endl;
		return -1;
	}

	// Create new filename
	std::string sOutputFilename = sFilename;
	sOutputFilename.erase(sOutputFilename.length() - sExtension.length());
	sOutputFilename += sNewExtension;

	PLib3MFModel model = CLib3MFWrapper::CreateModel();
	PLib3MFReader reader = model->QueryReader(sReaderName);

	// Import Model from File
	std::cout << "reading " << sFilename << "..." << std::endl;
#ifndef __GNUC__
	ULONGLONG nStartTicks = GetTickCount64();
#endif
	reader->ReadFromFile(sFilename);
#ifndef __GNUC__
	std::cout << "elapsed time: " << (GetTickCount64() - nStartTicks) << "ms" << std::endl;
#endif

	PLib3MFWriter writer = model->QueryWriter(sWriterName);
	std::cout << "writing " << sOutputFilename << "..." << std::endl;
#ifndef __GNUC__
	nStartTicks = GetTickCount64();
#endif
	writer->WriteToFile(sOutputFilename);
#ifndef __GNUC__
	std::cout << "elapsed time: " << (GetTickCount64() - nStartTicks) << "ms" << std::endl;
#endif
	std::cout << "done" << std::endl;
	return 0;
}

int main(int argc, char** argv) {
	// Parse Arguments
	if (argc != 2) {
		std::cout << "Usage: " << std::endl;
		std::cout << "Convert 3MF to STL: Converter.exe model.3mf" << std::endl;
		std::cout << "Convert STL to 3MF: Converter.exe model.stl" << std::endl;
		return 0;
	}
	
	try {
		return convert(argv[1]);
	}
	catch (ELib3MFException &e) {
		std::cout << e.what() << std::endl;
		return e.getErrorCode();
	}
	return 0;
}
