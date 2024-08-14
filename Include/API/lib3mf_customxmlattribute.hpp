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

Abstract: This is the class declaration of CCustomXMLAttribute

*/


#ifndef __LIB3MF_CUSTOMXMLATTRIBUTE
#define __LIB3MF_CUSTOMXMLATTRIBUTE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_CustomXMLAttribute.h"
#include "Model/Classes/NMR_CustomXMLNode.h"


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CCustomXMLAttribute 
**************************************************************************************************************************/

class CCustomXMLAttribute : public virtual ICustomXMLAttribute, public virtual CBase {
private:

private:

	NMR::PCustomXMLNode m_pXMLNode;
	NMR::PCustomXMLAttribute m_pXMLAttribute;

public:

	CCustomXMLAttribute(NMR::PCustomXMLNode pXMLNode, NMR::PCustomXMLAttribute pXMLAttribute);

	virtual ~CCustomXMLAttribute();


	/**
	* ICustomXMLAttribute::GetName - Retrieves name of the attribute.
	* @return returns the name of the attribute.
	*/
	std::string GetName() override;

	/**
	* ICustomXMLAttribute::GetValue - Retrieves value of the attribute as string.
	* @return returns the value of the attribute.
	*/
	std::string GetValue() override;

	/**
	* ICustomXMLAttribute::IsValidInteger - Checks if the value is a valid integer in the given range.
	* @param[in] nMinValue - Minimum allowed value
	* @param[in] nMaxValue - Maximum allowed value
	* @return returns if the value is a valid integer.
	*/
	bool IsValidInteger(const Lib3MF_int64 nMinValue, const Lib3MF_int64 nMaxValue) override;

	/**
	* ICustomXMLAttribute::GetIntegerValue - Returns the value as integer. Fails if the value is not a valid integer in the given range.
	* @param[in] nMinValue - Minimum allowed value
	* @param[in] nMaxValue - Maximum allowed value
	* @return returns the value.
	*/
	Lib3MF_int64 GetIntegerValue(const Lib3MF_int64 nMinValue, const Lib3MF_int64 nMaxValue) override;

	/**
	* ICustomXMLAttribute::IsValidDouble - Checks if the value is a valid double in the given range.
	* @param[in] dMinValue - Minimum allowed value
	* @param[in] dMaxValue - Maximum allowed value
	* @return returns if the value is a valid double.
	*/
	bool IsValidDouble(const Lib3MF_double dMinValue, const Lib3MF_double dMaxValue) override;

	/**
	* ICustomXMLAttribute::GetDoubleValue - Returns the value as double. Fails if the value is not a valid double in the given range.
	* @param[in] dMinValue - Minimum allowed value
	* @param[in] dMaxValue - Maximum allowed value
	* @return returns the value .
	*/
	Lib3MF_double GetDoubleValue(const Lib3MF_double dMinValue, const Lib3MF_double dMaxValue) override;

	/**
	* ICustomXMLAttribute::IsValidBool - Checks if the value is a valid boolean value, meaning an integer or true or false as string. The value will be trimmed and any character will be converted to lowercase.
	* @return returns if the value is a valid bool.
	*/
	bool IsValidBool() override;

	/**
	* ICustomXMLAttribute::GetBoolValue - Returns the value as bool. Fails if the value is not a valid boolean value, meaning an integer or true or false as string. The value will be trimmed and any character will be converted to lowercase.
	* @param[in] dMinValue - Minimum allowed value
	* @param[in] dMaxValue - Maximum allowed value
	* @return returns the value .
	*/
	bool GetBoolValue(const Lib3MF_double dMinValue, const Lib3MF_double dMaxValue) override;

	/**
	* ICustomXMLAttribute::SetValue - Sets the value of the attribute as string.
	* @param[in] sValue - new value of the attribute.
	*/
	void SetValue(const std::string & sValue) override;

	/**
	* ICustomXMLAttribute::SetIntegerValue - Sets the value of the attribute as integer.
	* @param[in] nValue - new value of the attribute.
	*/
	void SetIntegerValue(const Lib3MF_int64 nValue) override;

	/**
	* ICustomXMLAttribute::SetDoubleValue - Sets the value of the attribute as double.
	* @param[in] dValue - new value of the attribute.
	*/
	void SetDoubleValue(const Lib3MF_double dValue) override;

	/**
	* ICustomXMLAttribute::SetBoolValue - Sets the value of the attribute as bool.
	* @param[in] bValue - new value of the attribute.
	*/
	void SetBoolValue(const bool bValue) override;

	/**
	* ICustomXMLAttribute::Remove - Removes the attribute from its parent node. All subsequent calls to the class will fail.
	*/
	void Remove() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_CUSTOMXMLATTRIBUTE
