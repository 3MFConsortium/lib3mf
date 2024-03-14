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

Abstract: This is the class declaration of CNameSpaceIterator

*/


#ifndef __LIB3MF_NAMESPACEITERATOR
#define __LIB3MF_NAMESPACEITERATOR

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CNameSpaceIterator 
**************************************************************************************************************************/

class CNameSpaceIterator : public virtual INameSpaceIterator, public virtual CBase {
private:

	/**
	* Put private members here.
	*/
	std::vector<std::string> m_namespaces;
	Lib3MF_int32 m_nCurrentIndex = -1;

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/

	void setNameSpaces(std::vector<std::string>&& namespaces);

	/**
	* Public member functions to implement.
	*/

	/**
	* INameSpaceIterator::MoveNext - Iterates to the next namespace in the list.
	* @return Iterates to the namespace in the list.
	*/
	bool MoveNext() override;

	/**
	* INameSpaceIterator::MovePrevious - Iterates to the previous namespace in the list.
	* @return Iterates to the previous required namespace in the list.
	*/
	bool MovePrevious() override;

	/**
	* INameSpaceIterator::GetCurrent - Returns the required namespace the iterator points at.
	* @return returns the namespace.
	*/
	std::string GetCurrent() override;

	/**
	* INameSpaceIterator::Count - Returns the number of namespaces the iterator captures.
	* @return returns the number of namspaces the iterator captures.
	*/
	Lib3MF_uint64 Count() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_NAMESPACEITERATOR
