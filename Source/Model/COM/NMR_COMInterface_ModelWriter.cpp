/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

COM Interface Implementation for Model Writer Class

--*/

#include "Model/COM/NMR_COMInterface_ModelWriter.h" 
#include "Model/Writer/NMR_ModelWriter.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/Platform/NMR_Platform.h" 

#ifndef __GCC
#include "Common/Platform/NMR_ExportStream_COM.h" 
#endif // __GCC

namespace NMR {

	LIB3MFMETHODIMP CCOMModelWriter::WriteToFile(_In_z_ LPCWSTR pwszFilename)
	{
		if (pwszFilename == nullptr)
			return LIB3MF_POINTER;
		if (m_pModelWriter.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			PExportStream pStream = fnCreateExportStreamInstance(pwszFilename);
			m_pModelWriter->exportToStream(pStream);
			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

#ifndef __GCC
	LIB3MFMETHODIMP CCOMModelWriter::WriteToStream(_In_ IStream * pStream)
	{
		if (pStream == nullptr)
			return LIB3MF_INVALIDARG;
		if (m_pModelWriter.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(pStream);
			m_pModelWriter->exportToStream(pExportStream);
			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}

	}
#endif // __GCC

	void CCOMModelWriter::setWriter(_In_ PModelWriter pModelWriter)
	{
		m_pModelWriter = pModelWriter;
	}

}
