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

#ifndef __NMR_MODELTRIANGLESET
#define __NMR_MODELTRIANGLESET

#include "Common/NMR_Types.h" 

#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"
#include "Common/Platform/NMR_ImportStream.h"  
#include <set>


namespace NMR {

	class CModelTriangleSet;
	typedef std::shared_ptr <CModelTriangleSet> PModelTriangleSet;

	class CModelTriangleSet {
	private:
	
		std::string m_sName;
		std::string m_sIdentifier;

		std::set<uint32_t> m_sTriangles;
	
	protected:
		

	public:
		CModelTriangleSet(_In_ const std::string& sName, _In_ const std::string& sIdentifier);

		CModelTriangleSet() = delete;
		
		static PModelTriangleSet make(_In_ const std::string & sName, _In_ const std::string & sIdentifier);
		
		std::string getName ();
		std::string getIdentifier ();
		
		void setName (_In_ const std::string & sName);
		void setIdentifier (_In_ const std::string & sIdentifier);

		void clear();
		void addTriangle(const uint32_t nTriangleIndex);
		void removeTriangle(const uint32_t nTriangleIndex);

		PModelTriangleSet duplicate(const std::string & sIdentifier);
		void merge (PModelTriangleSet pTriangleSet);

	};

}

#endif // __NMR_MODELTRIANGLESET


