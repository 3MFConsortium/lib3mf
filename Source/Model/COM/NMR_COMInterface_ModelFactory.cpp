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

NMR_COMFactory.h implements a custom COM creation export to link the COM interface without
bothering about global registration of a COM Server.

--*/

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/COM/NMR_COMInterface_Model.h" 
#include "Model/COM/NMR_COMInterface_ModelFactory.h" 
#include "Model/COM/NMR_COMVersion.h" 
#include "Common/NMR_Exception_Windows.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CCOMModelFactory::CCOMModelFactory()
	{
		m_nErrorCode = NMR_SUCCESS;
	}

	LIB3MFRESULT CCOMModelFactory::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelFactory::handleNMRException(_In_ CNMRException * pException)
	{
		__NMRASSERT(pException);

		m_nErrorCode = pException->getErrorCode();
		m_sErrorMessage = std::string(pException->what());

		CNMRException_Windows * pWinException = dynamic_cast<CNMRException_Windows *> (pException);
		if (pWinException != nullptr) {
			return pWinException->getHResult();
		}
		else {
			if (m_nErrorCode == NMR_ERROR_INVALIDPOINTER)
				return LIB3MF_POINTER;
			if (m_nErrorCode == NMR_ERROR_INVALIDPARAM)
				return LIB3MF_INVALIDARG;

			return LIB3MF_FAIL;
		}
	}

	LIB3MFRESULT CCOMModelFactory::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelFactory::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
	{
		if (!pErrorCode)
			return LIB3MF_POINTER;

		*pErrorCode = m_nErrorCode;
		if (pErrorMessage) {
			if (m_nErrorCode != NMR_SUCCESS) {
				*pErrorMessage = m_sErrorMessage.c_str();
			}
			else {
				*pErrorMessage = nullptr;
			}
		}

		return LIB3MF_OK;
	}

	LIB3MFMETHODIMP CCOMModelFactory::CreateModel(_Outptr_ ILib3MFModel ** ppModel)
	{

		try
		{
			if (ppModel == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*ppModel = new CCOMObject<CCOMModel>();
			( (CCOMObject<CCOMModel>*) (*ppModel) )->AddRef();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModelFactory::GetSpecVersion(_Out_ DWORD * pMajorVersion, _Out_ DWORD * pMinorVersion)
	{
		try
		{
			if (pMajorVersion == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (pMinorVersion == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*pMajorVersion = NMR_APIVERSION_MAJOR;
			*pMinorVersion = NMR_APIVERSION_MINOR;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModelFactory::GetInterfaceVersion(_Out_ DWORD * pInterfaceVersionMajor, _Out_ DWORD * pInterfaceVersionMinor, _Out_ DWORD * pInterfaceVersionMicro)
	{
		try
		{
			if ((pInterfaceVersionMajor == nullptr) || (pInterfaceVersionMinor == nullptr) || (pInterfaceVersionMicro == nullptr))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*pInterfaceVersionMajor = NMR_APIVERSION_INTERFACE_MAJOR;
			*pInterfaceVersionMinor = NMR_APIVERSION_INTERFACE_MINOR;
			*pInterfaceVersionMicro = NMR_APIVERSION_INTERFACE_MICRO;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModelFactory::QueryExtension(_In_z_ LPCWSTR pwszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion)
	{
		try
		{
			if (pwszExtensionUrl == nullptr || pbIsSupported == nullptr || pExtensionInterfaceVersion == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (wcscmp(pwszExtensionUrl, XML_3MF_NAMESPACE_MATERIALSPEC) == 0) {
				*pbIsSupported = true;
				*pExtensionInterfaceVersion = NMR_APIVERSION_INTERFACE_MATERIALSPEC;
			}
			else if (wcscmp(pwszExtensionUrl, XML_3MF_NAMESPACE_PRODUCTIONSPEC) == 0) {
				*pbIsSupported = true;
				*pExtensionInterfaceVersion = NMR_APIVERSION_INTERFACE_PRODUCTIONSEPC;
			}
			else if (wcscmp(pwszExtensionUrl, XML_3MF_NAMESPACE_BEAMLATTICESPEC) == 0) {
				*pbIsSupported = true;
				*pExtensionInterfaceVersion = NMR_APIVERSION_INTERFACE_BEAMLATTICESPEC;
			}
			else if (wcscmp(pwszExtensionUrl, XML_3MF_NAMESPACE_SLICESPEC) == 0) {
				*pbIsSupported = true;
				*pExtensionInterfaceVersion = NMR_APIVERSION_INTERFACE_SLICESPEC;
			}
			else {
				*pbIsSupported = false;
			}
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelFactory::QueryExtensionUTF8(_In_z_ LPCSTR pszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion) {
		try {
			if (!pszExtensionUrl)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			std::string sUTF8ExtensionUrl(pszExtensionUrl);
			std::wstring sUTF16Language = fnUTF8toUTF16(sUTF8ExtensionUrl);
			QueryExtension(sUTF16Language.c_str(), pbIsSupported, pExtensionInterfaceVersion);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


}


