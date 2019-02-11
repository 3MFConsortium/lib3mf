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

Abstract: This is the class declaration of CLib3MFMetaDataGroup

*/


#ifndef __LIB3MF_LIB3MFMETADATAGROUP
#define __LIB3MF_LIB3MFMETADATAGROUP

#include "lib3mf_interfaces.hpp"
#include "lib3mf_baseclass.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

#include "Model/Classes/NMR_ModelMetaDataGroup.h"

// Include custom headers here.


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CLib3MFMetaDataGroup 
**************************************************************************************************************************/

class CLib3MFMetaDataGroup : public virtual ILib3MFMetaDataGroup, public virtual CLib3MFBaseClass {
private:

	/**
	* Put private members here.
	*/
	NMR::PModelMetaDataGroup m_pModelMetaDataGroup;

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CLib3MFMetaDataGroup(NMR::PModelMetaDataGroup pMetaDataGroup);

	/**
	* Public member functions to implement.
	*/

	virtual Lib3MF_uint32 GetMetaDataCount();

	ILib3MFMetaData * GetMetaData(const Lib3MF_uint32 nIndex);

	ILib3MFMetaData * GetMetaDataByKey(const std::string & sNameSpace, const std::string & sName);

	void RemoveMetaDataByIndex(const Lib3MF_uint32 nIndex);

	void RemoveMetaData(ILib3MFMetaData* pTheMetaData);

	ILib3MFMetaData * AddMetaData(const std::string & sNameSpace, const std::string & sName, const std::string & sValue, const std::string & sType, const bool bMustPreserve);


};

}
}

#pragma warning( pop )
#endif // __LIB3MF_LIB3MFMETADATAGROUP
