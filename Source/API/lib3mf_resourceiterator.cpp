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

Abstract: This is a stub class definition of CLib3MFResourceIterator

*/

#include "lib3mf_resource.hpp"
#include "lib3mf_resourceiterator.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFResourceIterator 
**************************************************************************************************************************/

CLib3MFResourceIterator::CLib3MFResourceIterator()
{
	m_nCurrentIndex = -1;
}

void CLib3MFResourceIterator::addResource(NMR::PModelResource pResource)
{
	if (pResource.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	m_pResources.push_back(pResource);
}

bool CLib3MFResourceIterator::MoveNext ()
{
	// Get Resource Count
	Lib3MF_int32 nBuildItemCount = (Lib3MF_int32)m_pResources.size();
	m_nCurrentIndex++;

	// Check new Index
	if (m_nCurrentIndex >= nBuildItemCount) {
		m_nCurrentIndex = nBuildItemCount;
		return false;
	}
	else {
		return true;
	}
}

bool CLib3MFResourceIterator::MovePrevious ()
{
	// Get Resource Count
	m_nCurrentIndex--;

	// Check new Index
	if (m_nCurrentIndex <= -1) {
		m_nCurrentIndex = -1;
		return false;
	}
	else {
		return true;
	}
}

inline NMR::PModelResource CLib3MFResourceIterator::GetCurrentResource()
{
	// Get Resource Count
	Lib3MF_int32 nBuildItemCount = (Lib3MF_int32)m_pResources.size();
	if ((m_nCurrentIndex < 0) || (m_nCurrentIndex >= nBuildItemCount))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ITERATORINVALIDINDEX);
	
	return m_pResources[m_nCurrentIndex];
}

ILib3MFResource * CLib3MFResourceIterator::GetCurrent ()
{
	// Create specific API class
	auto pACTResource = std::make_unique<CLib3MFResource>(GetCurrentResource());
	return pACTResource.release();
}

ILib3MFResourceIterator * CLib3MFResourceIterator::Clone ()
{
	auto pResources = std::make_unique<CLib3MFResourceIterator>();

	for (auto iIterator = m_pResources.begin(); iIterator != m_pResources.end(); iIterator++)
		pResources->addResource(*iIterator);

	return pResources.release();
}

