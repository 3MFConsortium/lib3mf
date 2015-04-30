/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of Microsoft Corporation nor netfabb GmbH nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MICROSOFT OR NETFABB BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

NMR_ModelReader_GCC_OPC.cpp implements the Model Reader Class.
A model reader reads in a 3MF file and generates an in-memory representation of it.
It uses a native ZIP implementation.

--*/

#include "Model/Reader/NMR_ModelReader_3MF_GCC.h" 
#include "Model/Reader/NMR_ModelReaderNode_Model.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 

namespace NMR {

	CModelReader_3MF_GCC::CModelReader_3MF_GCC(_In_ PModel pModel)
		: CModelReader_3MF(pModel)
	{
		// empty on purpose
	}

	PImportStream CModelReader_3MF_GCC::extract3MFOPCPackage(_In_ PImportStream pPackageStream)
	{
		throw CNMRException (NMR_ERROR_NOTIMPLEMENTED);
	}
	
	void CModelReader_3MF_GCC::release3MFOPCPackage()
	{
		throw CNMRException (NMR_ERROR_NOTIMPLEMENTED);
	}

}
