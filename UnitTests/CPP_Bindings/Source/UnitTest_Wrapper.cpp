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

UnitTest_Geometry.cpp: Defines Unittests for basic linear algebraic classes and
operations

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

#include <iostream>


namespace Lib3MF
{
	TEST(Wrapper, GetLibraryVersion)
	{
		Lib3MF_uint32 nMajor, nMinor, nMicro;
		CLib3MFWrapper::GetLibraryVersion(nMajor, nMinor, nMicro);

		ASSERT_EQ(nMajor, LIB3MF_VERSION_MAJOR);
		ASSERT_EQ(nMinor, LIB3MF_VERSION_MINOR);
		ASSERT_EQ(nMicro, LIB3MF_VERSION_MICRO);
	}
	
	TEST(Wrapper, GetSpecificationVersion)
	{
		Lib3MF_uint32 nMajor, nMinor, nMicro;
		bool bIsSupported;

		CLib3MFWrapper::GetSpecificationVersion("BogusSpecification", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_FALSE(bIsSupported);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/core/2015/02", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 2);
		ASSERT_EQ(nMicro, 3);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/material/2015/02", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 0);
		ASSERT_EQ(nMicro, 0);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/production/2015/06", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 0);
		ASSERT_EQ(nMicro, 0);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/beamlattice/2017/02", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 0);
		ASSERT_EQ(nMicro, 0);

		CLib3MFWrapper::GetSpecificationVersion("http://schemas.microsoft.com/3dmanufacturing/slice/2015/07", bIsSupported, nMajor, nMinor, nMicro);
		ASSERT_TRUE(bIsSupported);
		ASSERT_EQ(nMajor, 1);
		ASSERT_EQ(nMinor, 0);
		ASSERT_EQ(nMicro, 0);
	}

	TEST(Wrapper, CreateModel)
	{
		auto model = CLib3MFWrapper::CreateModel();
	}

}
