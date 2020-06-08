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

Abstract: This is a stub class definition of CSliceStack

*/

#include "lib3mf_slicestack.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_slice.hpp"
// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CSliceStack 
**************************************************************************************************************************/

CSliceStack::CSliceStack(NMR::PModelSliceStack pSliceStack):
	CResource(pSliceStack)
{
}

NMR::PModelSliceStack CSliceStack::sliceStack()
{
	return std::dynamic_pointer_cast<NMR::CModelSliceStack>(resource());
}

double CSliceStack::GetBottomZ()
{
	return sliceStack()->getZBottom();
}

Lib3MF_uint64 CSliceStack::GetSliceCount ()
{
	return sliceStack()->getSliceCount();
}

ISlice * CSliceStack::GetSlice (const Lib3MF_uint64 nSliceIndex)
{
	NMR::PSlice pSlice = sliceStack()->getSlice(Lib3MF_uint32(nSliceIndex));
	return new CSlice(pSlice);
}

ISlice * CSliceStack::AddSlice (const double fZTop)
{
	NMR::PSlice pSlice = sliceStack()->AddSlice(fZTop);
	return new CSlice(pSlice);
}

Lib3MF_uint64 CSliceStack::GetSliceRefCount()
{
	return sliceStack()->getSliceRefCount();
}

void CSliceStack::AddSliceStackReference(ISliceStack* pTheSliceStack)
{
	NMR::ModelResourceID nID = pTheSliceStack->GetResourceID();
	
	std::shared_ptr<IPackagePart> pPackagePart(pTheSliceStack->PackagePart());
	std::string sPath = pPackagePart->GetPath();

	NMR::PModelResource pResource = sliceStack()->getModel()->findResource(sPath, nID);
	NMR::PModelSliceStack pModelSliceStack = std::dynamic_pointer_cast<NMR::CModelSliceStack>(pResource);
	if (!pModelSliceStack)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDSLICESTACKRESOURCE);

	sliceStack()->AddSliceRef(pModelSliceStack);
}

ISliceStack * CSliceStack::GetSliceStackReference(const Lib3MF_uint64 nSliceRefIndex)
{
	NMR::PModelSliceStack pModelSliceStack = sliceStack()->getSliceRef(Lib3MF_uint32(nSliceRefIndex));
	return new CSliceStack(pModelSliceStack);
}

void CSliceStack::CollapseSliceReferences()
{
	sliceStack()->CollapseSliceReferences();
}

void CSliceStack::SetOwnPath(const std::string & sPath)
{
	sliceStack()->SetOwnPath(sPath);
}

std::string CSliceStack::GetOwnPath()
{
	return sliceStack()->OwnPath();
}

