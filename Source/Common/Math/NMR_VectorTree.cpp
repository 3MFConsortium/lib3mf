/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_VectorTree.cpp implements a n*log(n) lookup tree class to identify vectors
by their position.

--*/

#include "Common/Math/NMR_VectorTree.h" 
#include "Common/Math/NMR_Vector.h" 
#include "Common/NMR_Exception.h" 
#include <math.h>
#include <map>

namespace NMR {

	bool operator< (_In_ const VECTORTREEENTRY & entry1, _In_ const VECTORTREEENTRY & entry2)
	{
		for (int i = 0; i < 3; i++) {
			if (entry1.m_position.m_fields[i] < entry2.m_position.m_fields[i])
				return true;
			if (entry1.m_position.m_fields[i] > entry2.m_position.m_fields[i])
				return false;
		}

		return false;
	}

	CVectorTree::CVectorTree()
	{
		setUnits(NMR_VECTOR_DEFAULTUNITS);
	}

	CVectorTree::CVectorTree(_In_ nfFloat fUnits)
	{
		setUnits(fUnits);
	}

	nfFloat CVectorTree::getUnits()
	{
		return m_fUnits;
	}

	void CVectorTree::setUnits(_In_ nfFloat fUnits)
	{
		if ((fUnits < NMR_VECTOR_MINUNITS) || (fUnits > NMR_VECTOR_MAXUNITS))
			throw CNMRException(NMR_ERROR_INVALIDUNITS);
		if (m_entries.size() > 0)
			throw CNMRException(NMR_ERROR_COULDNOTSETUNITS);

		m_fUnits = fUnits;
	}

	_Success_(return) nfBool CVectorTree::findVector2(_In_ NVEC2 vVector, _Out_opt_ nfUint32 & value)
	{
		NVEC3 vVector3;
		vVector3.m_fields[0] = vVector.m_fields[0];
		vVector3.m_fields[1] = vVector.m_fields[1];
		vVector3.m_fields[2] = 0.0f;
		return findVector3(vVector3, value);
	}

	_Success_(return) nfBool CVectorTree::findVector3(_In_ NVEC3 vVector, _Out_opt_ nfUint32 & value)
	{
		VECTORTREEENTRY entry;
		entry.m_position = fnVEC3I_floor(vVector, m_fUnits);

		std::map <VECTORTREEENTRY, nfUint32>::iterator iter = m_entries.find(entry);

		if (iter != m_entries.end()) {
			value = iter->second;
			return true;
		}
		else
			return false;
	}

	_Success_(return) nfBool CVectorTree::findIntVector2(_In_ NVEC2I vVector, _Out_opt_ nfUint32 & value)
	{
		NVEC3I vVector3;
		vVector3.m_fields[0] = vVector.m_fields[0];
		vVector3.m_fields[1] = vVector.m_fields[1];
		vVector3.m_fields[2] = 0;
		return findIntVector3(vVector3, value);
	}

	_Success_(return) nfBool CVectorTree::findIntVector3(_In_ NVEC3I vVector, _Out_opt_ nfUint32 & value)
	{
		VECTORTREEENTRY entry;
		entry.m_position = vVector;

		std::map <VECTORTREEENTRY, nfUint32>::iterator iter = m_entries.find(entry);

		if (iter != m_entries.end()) {
			value = iter->second;
			return true;
		}
		else
			return false;
	}

	void CVectorTree::addVector2(_In_ NVEC2 vVector, _In_ nfUint32 value)
	{
		NVEC3 vVector3;
		vVector3.m_fields[0] = vVector.m_fields[0];
		vVector3.m_fields[1] = vVector.m_fields[1];
		vVector3.m_fields[2] = 0.0f;
		addVector3(vVector3, value);
	}

	void CVectorTree::addVector3(_In_ NVEC3 vVector, _In_ nfUint32 value)
	{
		VECTORTREEENTRY entry;
		entry.m_position = fnVEC3I_floor(vVector, m_fUnits);
		m_entries.insert(std::make_pair(entry, value));
	}

	void CVectorTree::addIntVector2(_In_ NVEC2I vVector, _In_ nfUint32 value)
	{
		NVEC3I vVector3;
		vVector3.m_fields[0] = vVector.m_fields[0];
		vVector3.m_fields[1] = vVector.m_fields[1];
		vVector3.m_fields[2] = 0;
		addIntVector3(vVector3, value);
	}

	void CVectorTree::addIntVector3(_In_ NVEC3I vVector, _In_ nfUint32 value)
	{
		VECTORTREEENTRY entry;
		entry.m_position = vVector;
		m_entries.insert(std::make_pair(entry, value));
	}

	void CVectorTree::removeVector2(_In_ NVEC2 vVector)
	{
		NVEC3 vVector3;
		vVector3.m_fields[0] = vVector.m_fields[0];
		vVector3.m_fields[1] = vVector.m_fields[1];
		vVector3.m_fields[2] = 0.0f;
		removeVector3(vVector3);
	}

	void CVectorTree::removeVector3(_In_ NVEC3 vVector)
	{
		VECTORTREEENTRY entry;
		entry.m_position = fnVEC3I_floor(vVector, m_fUnits);
		m_entries.erase(entry);
	}

	void CVectorTree::removeIntVector2(_In_ NVEC2I vVector)
	{
		NVEC3I vVector3;
		vVector3.m_fields[0] = vVector.m_fields[0];
		vVector3.m_fields[1] = vVector.m_fields[1];
		vVector3.m_fields[2] = 0;
		removeIntVector3(vVector3);
	}

	void CVectorTree::removeIntVector3(_In_ NVEC3I vVector)
	{
		VECTORTREEENTRY entry;
		entry.m_position = vVector;
		m_entries.erase(entry);
	}

}
