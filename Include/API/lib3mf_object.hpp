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

Abstract: This is the class declaration of CLib3MFObject

*/


#ifndef __LIB3MF_LIB3MFOBJECT
#define __LIB3MF_LIB3MFOBJECT

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_resource.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

// Include custom headers here.
#include "Model/Classes/NMR_ModelResource.h" 

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CLib3MFObject 
**************************************************************************************************************************/

class CLib3MFObject : public virtual ILib3MFObject, public virtual CLib3MFResource {
private:

	/**
	* Put private members here.
	*/

protected:
	NMR::CModelObject* object();

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CLib3MFObject(NMR::PModelResource pResource);
	CLib3MFObject() = delete;

	static ILib3MFObject* fnCreateObjectFromModelResource(NMR::PModelResource pResource, bool bFailIfUnkownClass);

	/**
	* Public member functions to implement.
	*/

	eLib3MFObjectType GetType ();

	void SetType (const eLib3MFObjectType eObjectType);

	std::string GetName ();

	void SetName (const std::string & sName);

	std::string GetPartNumber ();

	void SetPartNumber (const std::string & sPartNumber);

	virtual bool IsMeshObject ();

	virtual bool IsComponentsObject ();

	virtual ILib3MFMeshObject * AsMeshObject();

	virtual ILib3MFComponentsObject * AsComponentsObject();

	bool IsValid ();

	bool HasMetaDataGroup ();

	ILib3MFMetaDataGroup * GetMetaDataGroup ();

	ILib3MFMetaDataGroup * CreateMetaDataGroup ();

	void RemoveMetaDataGroup ();

	std::string GetUUID(bool & bHasUUID);

	void SetUUID(const std::string & sUUID);

};

}
}

#pragma warning( pop )
#endif // __LIB3MF_LIB3MFOBJECT
