/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

This file has been generated by the Automatic Component Toolkit (ACT) version 1.8.0-develop.

Abstract: This is an autogenerated C++ Implementation file with the basic internal
 exception type in order to allow an easy use of the 3MF Library

Interface version: 2.3.1

*/


#include <string>

#include "lib3mf_interfaceexception.hpp"

/*************************************************************************************************************************
 Class ELib3MFInterfaceException
**************************************************************************************************************************/
ELib3MFInterfaceException::ELib3MFInterfaceException(Lib3MFResult errorCode)
	: m_errorMessage(LIB3MF_GETERRORSTRING (errorCode))
{
	m_errorCode = errorCode;
}

ELib3MFInterfaceException::ELib3MFInterfaceException(Lib3MFResult errorCode, std::string errorMessage)
	: m_errorMessage(errorMessage + " (" + std::to_string (errorCode) + ")")
{
	m_errorCode = errorCode;
}

Lib3MFResult ELib3MFInterfaceException::getErrorCode ()
{
	return m_errorCode;
}

const char * ELib3MFInterfaceException::what () const noexcept
{
	return m_errorMessage.c_str();
}

