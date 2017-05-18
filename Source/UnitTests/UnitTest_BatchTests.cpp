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

UnitTest_BatchTests.cpp: Defines Entry point for the BatchTests of the shared
library interface

--*/


#include <iostream>
#include <vector>
#include <string>

#ifdef _WIN32

#include <windows.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::vector<std::string> Get3MFFiles(const std::string sDir) {
	HANDLE hFind;
	WIN32_FIND_DATA data;

	std::vector<std::string> fileList;

	hFind = FindFirstFile((sDir+"\\*.3mf").c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			fileList.push_back(sDir + data.cFileName);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	return fileList;
}

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
			result += buffer.data();
	}
	return result;
}

int main(int argc, char **argv)
{
	std::string testFiles;

	bool doPositives = true;
	if (doPositives)
		testFiles = "D:/Parts/3MF_repo/Positive/";
	else
		testFiles = "D:/Parts/3MF_repo/Negative/";

	std::vector<std::string> fileList = Get3MFFiles(testFiles);
	std::vector<std::string> failures;
	std::vector<std::string> successes;

	std::string command = "C:\\Users\\weismam\\Documents\\Code\\lib3MF\\Tools\\ExtractInfo\\Example_ExtractInfo.exe";
	for (auto fileName : fileList) {
		std::cout << fileName << std::endl;
		std::string  res = exec((command + " \"" + fileName+"\"").c_str());
		int errorPos = (int)res.find("error");
		if (errorPos < 0) {
			errorPos = (int)res.find("warning");
		}
		if (errorPos >= 0) {
			failures.push_back(fileName);
			if (doPositives)
				std::cout << res << std::endl;
			std::cout << "Failure" << std::endl;
		}
		else {
			successes.push_back(fileName);
			if (!doPositives)
				std::cout << res << std::endl;
			std::cout << "Success" << std::endl;
		}
		std::cout << "========================" << std::endl;
	}

	std::cout << "========================" << std::endl;

	if (doPositives) {
		std::cout << "Tested " << fileList.size() << " files; " << failures.size() << " of which failed." << std::endl;
		for (auto fn : failures) {
			std::cout << fn << std::endl;
		}
	}
	else
	{
		std::cout << "Tested " << fileList.size() << " files; " << successes.size() << " of which succeded although they should have failed." << std::endl;
		for (auto fn : successes) {
			std::cout << fn << std::endl;
		}
	}

    return 0;
}

#else
int main(int argc, char **argv)
{
	std::cout << "This test only runs on windows" << std::endl;
	return 0;
}
#endif
