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

UnitTest_Utilities.h: Utilities for UnitTests

--*/

#ifndef __NMR_UNITTEST_UTILITIES
#define __NMR_UNITTEST_UTILITIES

#include "lib3mf_implicit.hpp"
#include "gtest/gtest.h"
#include <fstream>

#ifdef LTESTFILESPATH
const std::string sTestFilesPath = TESTFILESPATH;
#else
const std::string sTestFilesPath = "TestFiles";
#endif

#ifdef LOUTFILESPATH
const std::string sOutFilesPath = OUTFILESPATH;
#else
const std::string sOutFilesPath = "..\TestOutput";
#endif

inline bool CreateDir(std::string sPath) {
	return (system((std::string("mkdir \"") + sPath + "\"").c_str()) != -1);
}

#define ASSERT_SPECIFIC_THROW(statement, type) {\
try {\
  statement;\
  ASSERT_FALSE(true);\
}\
catch (const type &) {\
  ASSERT_TRUE(true);\
}\
}


inline std::vector<Lib3MF_uint8> ReadFileIntoBuffer(std::string sFileName)
{
	// Read the file fully into a memory buffer
	std::ifstream file(sFileName, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	// Memory buffer
	std::vector<Lib3MF_uint8> buffer(static_cast<size_t>(size));
	file.read((char*)buffer.data(), size);
	return buffer;
}

inline void WriteBufferToFile(std::vector<Lib3MF_uint8> const & buffer, std::string sFileName) 
{
	std::ofstream file(sFileName, std::ios::binary);
	for (Lib3MF_uint8 cByte: buffer) {
		file.put(cByte);
	}
	file.close();
}

inline sLib3MFTransform getIdentityTransform()
{
	sLib3MFTransform t;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
			t.m_Fields[i][j] = 0 + 1.0f * (i==j);
	}
	return t;
}

inline void CompareColors(Lib3MF::sColor c1, Lib3MF::sColor c2)
{
	EXPECT_EQ(c1.m_Alpha, c2.m_Alpha);
	EXPECT_EQ(c1.m_Red, c2.m_Red);
	EXPECT_EQ(c1.m_Green, c2.m_Green);
	EXPECT_EQ(c1.m_Blue, c2.m_Blue);
}

template<typename T>
struct PositionedVector
{
	std::vector<T> vec;
	Lib3MF_uint64 pos;
	PositionedVector() { pos = 0; };

	static void writeCallback(Lib3MF_uint64 nByteData, Lib3MF_uint64 nNumBytes, Lib3MF_pvoid pUserData) {
		PositionedVector<T>* buffer = reinterpret_cast<PositionedVector<T>*> ((void*)(pUserData));
		T* pData = (T*)(nByteData);
		for (int i = 0; i < nNumBytes; i++) {
			if (buffer->pos == buffer->vec.size()) {
				buffer->vec.push_back(*pData);
			}
			else  if (buffer->pos < buffer->vec.size()) {
				buffer->vec[(size_t)buffer->pos] = *pData;
			}
			else {
				ASSERT_TRUE(false);
			}
			buffer->pos++;
			pData++;
		}
	};

	static void readCallback(Lib3MF_uint64 nByteData, Lib3MF_uint64 nNumBytes, Lib3MF_pvoid pUserData) {
		PositionedVector<T>* buffer = reinterpret_cast<PositionedVector<T>*> ((void*)(pUserData));
		T* pData = (T*)(nByteData);
		for (int i = 0; i < nNumBytes; i++) {
			if (buffer->pos < buffer->vec.size()) {
				*pData = buffer->vec[(size_t)buffer->pos];
			}
			else {
				ASSERT_TRUE(false);
			}
			buffer->pos++;
			pData++;
		}
	};

	static void seekCallback(Lib3MF_uint64 nPosition, Lib3MF_pvoid pUserData) {
		PositionedVector<T>* buffer = reinterpret_cast<PositionedVector<T>*> ((void*)(pUserData));
		buffer->pos = nPosition;
	};

};

inline sLib3MFPosition fnCreateVertex(float x, float y, float z)
{
	sLib3MFPosition result;
	result.m_Coordinates[0] = x;
	result.m_Coordinates[1] = y;
	result.m_Coordinates[2] = z;
	return result;
}

inline sLib3MFTriangle fnCreateTriangle(int v0, int v1, int v2)
{
	sLib3MFTriangle result;
	result.m_Indices[0] = v0;
	result.m_Indices[1] = v1;
	result.m_Indices[2] = v2;
	return result;
}


void fnCreateBox(std::vector<sLib3MFPosition> &vctVertices, std::vector<sLib3MFTriangle> &vctTriangles);


inline void CheckReaderWarnings(Lib3MF::PReader reader, Lib3MF_uint32 nWarnings)
{
	EXPECT_EQ(reader->GetWarningCount(), nWarnings);
	for (Lib3MF_uint32 iWarning = 0; iWarning < reader->GetWarningCount(); iWarning++)
	{
		Lib3MF_uint32 nErrorCode;
		std::string sWarning = reader->GetWarning(iWarning, nErrorCode);
		EXPECT_TRUE(true) << iWarning << ": " << nErrorCode << ", " << sWarning;
	}
}


#endif //__NMR_UNITTEST_UTILITIES
