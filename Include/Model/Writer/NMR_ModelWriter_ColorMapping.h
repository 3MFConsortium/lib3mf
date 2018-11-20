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

NMR_ModelWriter.h defines the Model Writer Class.
A model writer exports the in memory represenation into the 3MF file.

--*/

#ifndef __NMR_MODELWRITER_COLORMAPPING
#define __NMR_MODELWRITER_COLORMAPPING

#include "Model/Classes/NMR_ModelTypes.h" 
#include "Common/NMR_Types.h" 
#include "Common/NMR_PagedVector.h" 

#include <map>
#include <memory>

namespace NMR {

	class CModelWriter_ColorMapping {
	private:
	protected:
		ModelResourceIndex m_nCurrentIndex;
		std::map<nfColor, ModelResourceIndex> m_IndexMap;
		std::vector<nfColor> m_ColorVector;
		ModelResourceID m_ResourceID;

	public:
		CModelWriter_ColorMapping() = delete;
		CModelWriter_ColorMapping(_In_ ModelResourceID ResourceID);

		void registerColor(_In_ nfColor cColor);
		nfBool findColor(_In_ nfColor cColor, _Out_ ModelResourceIndex & nResourceIndex);

		nfUint32 getCount();
		nfColor getColor(_In_ nfUint32 nIndex);

		ModelResourceID getResourceID();
	};

	typedef std::shared_ptr <CModelWriter_ColorMapping> PModelWriter_ColorMapping;

}

#endif // __NMR_MODELWRITER
