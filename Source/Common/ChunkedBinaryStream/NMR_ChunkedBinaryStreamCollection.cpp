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

#include "Common/ChunkedBinaryStream/NMR_ChunkedBinaryStreamCollection.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 

#include <vector>


namespace NMR {

	CChunkedBinaryStreamCollection::CChunkedBinaryStreamCollection()
	{

	}

	void CChunkedBinaryStreamCollection::registerReader(const std::string & sPath, PChunkedBinaryStreamReader pReader)
	{
		if (pReader.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::string sAbsolutePath = fnRemoveLeadingPathDelimiter(sPath);
		m_ReaderMap.insert(std::make_pair (sAbsolutePath, pReader));
	}

	CChunkedBinaryStreamReader * CChunkedBinaryStreamCollection::findReader(const std::string & sPath)
	{
		std::string sAbsolutePath = fnRemoveLeadingPathDelimiter(sPath);

		auto iIter = m_ReaderMap.find (sAbsolutePath);
		if (iIter != m_ReaderMap.end())
			return iIter->second.get();

		return nullptr;
	}


}

