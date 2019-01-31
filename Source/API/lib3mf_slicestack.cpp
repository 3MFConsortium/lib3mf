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

Abstract: This is a stub class definition of CLib3MFSliceStack

*/

#include "lib3mf_slicestack.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_slice.hpp"
// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFSliceStack 
**************************************************************************************************************************/

CLib3MFSliceStack::CLib3MFSliceStack(NMR::PModelSliceStack pSliceStack):
	CLib3MFResource(pSliceStack)
{
}

NMR::PModelSliceStack CLib3MFSliceStack::sliceStack()
{
	return std::dynamic_pointer_cast<NMR::CModelSliceStack>(resource());
}

double CLib3MFSliceStack::GetBottomZ()
{
	return sliceStack()->getZBottom();
}

Lib3MF_uint64 CLib3MFSliceStack::GetSliceCount ()
{
	return sliceStack()->getSliceCount();
}

ILib3MFSlice * CLib3MFSliceStack::GetSlice (const Lib3MF_uint64 nSliceIndex)
{
	NMR::PSlice pSlice = sliceStack()->getSlice(Lib3MF_uint32(nSliceIndex));
	return new CLib3MFSlice(pSlice);
}

ILib3MFSlice * CLib3MFSliceStack::AddSlice (const double fZTop)
{
	NMR::PSlice pSlice = sliceStack()->AddSlice(fZTop);
	return new CLib3MFSlice(pSlice);
}

Lib3MF_uint64 CLib3MFSliceStack::GetSliceRefCount()
{
	return sliceStack()->getSliceRefCount();
}

void CLib3MFSliceStack::AddSliceStackReference(ILib3MFSliceStack* pTheSliceStack)
{
	Lib3MF_uint32 nID = pTheSliceStack->GetResourceID();
	NMR::PModelResource pResource = sliceStack()->getModel()->findResource(nID);
	NMR::PModelSliceStack pModelSliceStack = std::dynamic_pointer_cast<NMR::CModelSliceStack>(pResource);
	if (!pModelSliceStack)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDSLICESTACKRESOURCE);

	sliceStack()->AddSliceRef(pModelSliceStack);
}

ILib3MFSliceStack * CLib3MFSliceStack::GetSliceStackReference(const Lib3MF_uint64 nSliceRefIndex)
{
	NMR::PModelSliceStack pModelSliceStack = sliceStack()->getSliceRef(Lib3MF_uint32(nSliceRefIndex));
	return new CLib3MFSliceStack(pModelSliceStack);
}

void CLib3MFSliceStack::CollapseSliceReferences()
{
	sliceStack()->CollapseSliceReferences();
}

void CLib3MFSliceStack::SetOwnPath(const std::string & sPath)
{
	sliceStack()->SetOwnPath(sPath);
}

std::string CLib3MFSliceStack::GetOwnPath()
{
	return sliceStack()->OwnPath();
}

