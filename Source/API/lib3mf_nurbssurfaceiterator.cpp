/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CLib3MFNurbsSurfaceIterator

*/

#include "lib3mf_nurbssurfaceiterator.hpp"
#include "lib3mf_nurbssurface.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFNurbsSurfaceIterator 
**************************************************************************************************************************/

ILib3MFNurbsSurface * CLib3MFNurbsSurfaceIterator::GetCurrentNurbsSurface()
{
	// Get Resource Count
	Lib3MF_int32 nBuildItemCount = (Lib3MF_int32)m_pResources.size();
	if ((m_nCurrentIndex < 0) || (m_nCurrentIndex >= nBuildItemCount))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ITERATORINVALIDINDEX);

	// Create specific API class
	NMR::PModelResource pResource = m_pResources[m_nCurrentIndex];

	auto pNurbsSurface = std::dynamic_pointer_cast<NMR::CModelNurbsSurface> (pResource);

	auto pACTBuildItem = std::make_unique<CLib3MFNurbsSurface>(pNurbsSurface);

	return pACTBuildItem.release();

}
