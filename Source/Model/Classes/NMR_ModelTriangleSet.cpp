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


--*/

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelTriangleSet.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {


	CModelTriangleSet::CModelTriangleSet(_In_ const std::string& sName, _In_ const std::string& sIdentifier)
		: m_sName (sName), m_sIdentifier (sIdentifier)
	{

	}

	PModelTriangleSet CModelTriangleSet::make(_In_ const std::string& sName, _In_ const std::string& sIdentifier)
	{
		return std::make_shared<CModelTriangleSet>(sName, sIdentifier);
	}

	std::string CModelTriangleSet::getName()
	{
		return m_sName;
	}

	std::string CModelTriangleSet::getIdentifier()
	{
		return m_sIdentifier;
	}

	void CModelTriangleSet::setName(_In_ const std::string& sName)
	{
		m_sName = sName;
	}

	void CModelTriangleSet::setIdentifier(_In_ const std::string& sIdentifier)
	{
		m_sIdentifier = sIdentifier;
	}

	void CModelTriangleSet::clear()
	{
		m_sTriangles.clear();
	}

	void CModelTriangleSet::addTriangle(const uint32_t nTriangleIndex)
	{
		m_sTriangles.insert(nTriangleIndex);
	}

	void CModelTriangleSet::removeTriangle(const uint32_t nTriangleIndex)
	{
		m_sTriangles.erase(nTriangleIndex);
	}

	PModelTriangleSet CModelTriangleSet::duplicate(const std::string& sIdentifier)
	{
		auto pTriangleSet = std::make_shared<CModelTriangleSet>(m_sName, sIdentifier);

		for (auto nTriangleIndex : m_sTriangles)
			pTriangleSet->m_sTriangles.insert(nTriangleIndex);

		return pTriangleSet;
	}

	void CModelTriangleSet::merge(PModelTriangleSet pTriangleSet)
	{
		if (pTriangleSet.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		for (auto nTriangleIndex : pTriangleSet->m_sTriangles)
			m_sTriangles.insert(nTriangleIndex);

	}

	std::set<uint32_t>& CModelTriangleSet::getTriangles()
	{
		return m_sTriangles;

	}
}
