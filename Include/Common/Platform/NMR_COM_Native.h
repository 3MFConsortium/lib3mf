/*++

Copyright (C) 2018 3MF Consortium

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

NMR_COM_Native.h defines Macros for an easy definition of COM Interfaces

--*/

#ifndef __NMR_COM_NATIVE
#define __NMR_COM_NATIVE

#include <comutil.h>

#ifdef LIB3MF_EXPORTS
#define LIB3MF_DECLSPEC __declspec(dllexport)
#else
#define LIB3MF_DECLSPEC 
#endif

#ifndef __INTERFACE__
#define __INTERFACE__ __interface
#endif

#ifndef BEGIN_INTERFACE_MAP
#define BEGIN_INTERFACE_MAP(x)  \
    typedef x* Interface;   \
    __inline HRESULT InternalQueryInterface(_In_ REFIID riid, _COM_Outptr_ void **ppvObject)  \
{   \
    void* pResult; \
if (riid == __uuidof(x)) \
{ \
    pResult = (x*)this; \
}
#endif

#ifndef END_INTERFACE_MAP
#define END_INTERFACE_MAP   \
			    else if (riid == __uuidof(IUnknown)) \
{   \
    pResult = (Interface)this;   \
}   \
			    else  \
{ \
    *ppvObject = NULL;    \
    return E_NOINTERFACE; \
}; \
if (ppvObject) \
{\
    ((IUnknown*)pResult)->AddRef(); \
    *ppvObject = pResult; \
}; \
    return S_OK; \
};
#endif

#define LIB3MFINTERFACE_DECL(x) \
 BEGIN_INTERFACE_MAP(x) \
END_INTERFACE_MAP

#define LIB3MFINTERFACE(iface, baseiface, iid) __INTERFACE__ __declspec(uuid(iid)) __declspec(novtable)  iface : baseiface 
#define LIB3MFABSTRACT /* */
#define LIB3MFPUBLIC(X) /* */
#define LIB3MFMETHOD(x) _Success_(return == 0) STDMETHOD(x)
#define LIB3MFRESULT HRESULT
#define LIB3MFMETHODIMP _Success_(return == 0) STDMETHODIMP

#define LIB3MF_OK S_OK
#define LIB3MF_FAIL E_FAIL
#define LIB3MF_POINTER E_POINTER
#define LIB3MF_INVALIDARG E_INVALIDARG

#ifdef __GNUC__
#define LIB3MFDEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define LIB3MFDEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define LIB3MFDEPRECATED(func) func
#endif

namespace NMR {
	
	typedef IUnknown ILib3MFUnknown;

}

template <class BASE>
class CCOMObject : public BASE {
private:
	volatile LONG m_RefCount;

public:
	CCOMObject() {
		m_RefCount = 0;
	}

	STDMETHOD_(ULONG, AddRef)() {
		return InterlockedIncrement(&m_RefCount);
	}

	STDMETHOD_(ULONG, Release)() {
		ULONG refCnt = InterlockedDecrement(&m_RefCount);
		if (!refCnt)
			delete this;

		return refCnt;
	}

	STDMETHOD(QueryInterface)(_In_ REFIID riid, _COM_Outptr_ void **ppvObject) {
		return InternalQueryInterface(riid, ppvObject);
	};

	_Ret_maybenull_ static inline CCOMObject<BASE>* CreateInstance() {
		return new CCOMObject<BASE>();
	}
};

#endif // __NMR_COM_NATIVE
