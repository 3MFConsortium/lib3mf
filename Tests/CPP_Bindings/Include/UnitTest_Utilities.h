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


class Lib3MFTest : public ::testing::Test {
	public:
	static void SetUpTestCase() {
		wrapper = Lib3MF::CWrapper::loadLibrary();
		#ifdef CREATEJOURNAL
		wrapper->SetJournal("journal.txt");
		#endif
	}
	static Lib3MF::PWrapper wrapper;
};

namespace Lib3MF
{
        namespace helper
        {
                bool directoryExists(std::string& path);

                void comparePorts(
                    Lib3MF::PImplicitPortIterator const& portIterator1,
                    Lib3MF::PImplicitPortIterator const& portIterator2,
                    bool ignoreReference = false);

                void compareImplicitFunctions(
                    PImplicitFunction const& function1,
                    PImplicitFunction const& function2);

                void compareFunctionsFromImage3D(Lib3MF::PModel model1,
                                         PFunctionFromImage3D const& function1,
                                         Lib3MF::PModel model2,
                                         PFunctionFromImage3D const& function2);

                void compareFunctions(Lib3MF::PModel model1,
                                      PFunction const& function1,
                                      Lib3MF::PModel model2,
                                      PFunction const& function2);

                void CompareColors(Lib3MF::sColor c1, Lib3MF::sColor c2);

                void CompareTransforms(Lib3MF::sTransform t1,
                                       Lib3MF::sTransform t2);

                void CompareFunctionReferences(Lib3MF::PModel modelA,
                                               Lib3MF::PFunctionReference A,
                                               Lib3MF::PModel modelB,
                                               Lib3MF::PFunctionReference B);

                void CompareImage3Ds(Lib3MF::PModel modelA, Lib3MF::PImage3D i1,
                                     Lib3MF::PModel modelB,
                                     Lib3MF::PImage3D i2);
                void CompareImageStacks(Lib3MF::PImageStack i1,
                                        Lib3MF::PImageStack i2);

                void CompareVolumeData(Lib3MF::PModel modelA,
                                       Lib3MF::PVolumeData A,
                                       Lib3MF::PModel modelB,
                                       Lib3MF::PVolumeData B);


				/**
				 * @brief Calcutes the transform from the mesh coordinates to the normalized UVW space
				 * @param mesh Mesh to calculate the transform for
				 * @return Transform from mesh coordinates to UVW space
				*/
				Lib3MF::sTransform ComputeTransformFromMeshCoordinatesToUVW(Lib3MF::PMeshObject mesh);

				Lib3MF::PLevelSet getFirstLevelSet(Lib3MF::PModel model);

        }  // namespace helper
}  // namespace Lib3MF

std::vector<Lib3MF_uint8> ReadFileIntoBuffer(std::string sFileName);
void WriteBufferToFile(std::vector<Lib3MF_uint8> const & buffer, std::string sFileName);

sLib3MFTransform getIdentityTransform();

template<typename T>
struct PositionedVector
{
	std::vector<T> vec;
	Lib3MF_uint64 pos;
	PositionedVector() { pos = 0; };

	static void writeCallback(Lib3MF_uint64 nByteData, Lib3MF_uint64 nNumBytes, Lib3MF_pvoid pUserData) {
		PositionedVector<T>* buffer = reinterpret_cast<PositionedVector<T>*> ((void*)(pUserData));
		T* pData = (T*)(nByteData);
		for (Lib3MF_uint64 i = 0; i < nNumBytes; i++) {
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
		for (Lib3MF_uint64 i = 0u; i < nNumBytes; i++) {
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

sLib3MFPosition fnCreateVertex(float x, float y, float z);
sLib3MFTriangle fnCreateTriangle(int v0, int v1, int v2);
void fnCreateBox(std::vector<sLib3MFPosition> &vctVertices, std::vector<sLib3MFTriangle> &vctTriangles);

void CheckReaderWarnings(Lib3MF::PReader reader, Lib3MF_uint32 nWarnings);

#endif //__NMR_UNITTEST_UTILITIES
