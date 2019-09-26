/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CBuildItemIterator

*/

#include "lib3mf_builditemiterator.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_builditem.hpp"
// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CBuildItemIterator 
**************************************************************************************************************************/

CBuildItemIterator::CBuildItemIterator()
{
	m_nCurrentIndex = -1;
}


void CBuildItemIterator::addBuildItem(NMR::PModelBuildItem pBuildItem)
{
	m_pBuildItems.push_back(pBuildItem);
}

bool CBuildItemIterator::MoveNext ()
{
	// Get Resource Count
	Lib3MF_int32 nBuildItemCount = (Lib3MF_int32)m_pBuildItems.size();
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

bool CBuildItemIterator::MovePrevious ()
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

IBuildItem * CBuildItemIterator::GetCurrent ()
{
	// Get Resource Count
	Lib3MF_int32 nBuildItemCount = (Lib3MF_int32)m_pBuildItems.size();
	if ((m_nCurrentIndex < 0) || (m_nCurrentIndex >= nBuildItemCount))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ITERATORINVALIDINDEX);

	// Create specific API class
	NMR::PModelBuildItem pBuildItem = m_pBuildItems[m_nCurrentIndex];
	
	return new CBuildItem(pBuildItem);
}

IBuildItemIterator * CBuildItemIterator::Clone ()
{
	auto pBuildItems = std::unique_ptr<CBuildItemIterator>(new CBuildItemIterator());
	
	for (auto iIterator = m_pBuildItems.begin(); iIterator != m_pBuildItems.end(); iIterator++)
		pBuildItems->addBuildItem(*iIterator);

	return pBuildItems.release();
}

Lib3MF_uint64 CBuildItemIterator::Count()
{
	return m_pBuildItems.size();
}

