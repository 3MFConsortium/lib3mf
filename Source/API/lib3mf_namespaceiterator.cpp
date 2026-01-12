/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

Abstract: Iterator for namespaces

*/

#include "lib3mf_namespaceiterator.hpp"
#include "lib3mf_interfaceexception.hpp"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CNameSpaceIterator 
**************************************************************************************************************************/

bool CNameSpaceIterator::MoveNext()
{
	Lib3MF_int32 count = (Lib3MF_int32)m_namespaces.size();
	m_nCurrentIndex++;

	// Check new Index
	if (m_nCurrentIndex >= count) {
		m_nCurrentIndex = count;
		return false;
	}
	else {
		return true;
	}
}

bool CNameSpaceIterator::MovePrevious()
{
	m_nCurrentIndex--;

	// Check new Index
	if (m_nCurrentIndex < 0) {
		m_nCurrentIndex = -1;
		return false;
	}
	else {
		return true;
	}
}

std::string CNameSpaceIterator::GetCurrent()
{
	if ((m_nCurrentIndex < 0) || (m_nCurrentIndex >= (Lib3MF_int32)m_namespaces.size()))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ITERATORINVALIDINDEX);

	return m_namespaces[m_nCurrentIndex];
}

Lib3MF_uint64 CNameSpaceIterator::Count()
{
	return (Lib3MF_uint64)m_namespaces.size();
}

void Lib3MF::Impl::CNameSpaceIterator::setNameSpaces(std::vector<std::string> &&nameSpaces)
{
	m_namespaces = std::move(nameSpaces);
}
