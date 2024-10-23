/*++

Copyright (C) 2023 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CFunction

*/

#include "lib3mf_function.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_implicitport.hpp"
#include "lib3mf_implicitportiterator.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CFunction 
**************************************************************************************************************************/

NMR::CModelFunction* Lib3MF::Impl::CFunction::function()
{
	NMR::CModelFunction* pFunction = dynamic_cast<NMR::CModelFunction*>(resource().get());
	if (pFunction == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);

	return pFunction;
}

Lib3MF::Impl::CFunction::CFunction(NMR::PModelResource pResource)
	: CResource(pResource)
{
}

std::string CFunction::GetDisplayName()
{
	return function()->getDisplayName();
}

void CFunction::SetDisplayName(const std::string& sDisplayName)
{
	function()->setDisplayName(sDisplayName);
}

IImplicitPort* CFunction::AddInput(const std::string& sIdentifier,
	const std::string& sDisplayName,
	const Lib3MF::eImplicitPortType eType)
{

	return new CImplicitPort(function()->addInput(sIdentifier, sDisplayName, eType));
}

IImplicitPortIterator* CFunction::GetInputs()
{
	return new CImplicitPortIterator(function()->getInputs());
}

void CFunction::RemoveInput(IImplicitPort* pInput)
{
	auto inputs = function()->getInputs();
	auto it = std::find_if(inputs->begin(), inputs->end(), [pInput](const NMR::PModelImplicitPort& port) {
		return port->getIdentifier() == pInput->GetIdentifier();
	});
	if (it != inputs->end())
		inputs->erase(it);
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

IImplicitPort* CFunction::AddOutput(const std::string& sIdentifier,
	const std::string& sDisplayName,
	const Lib3MF::eImplicitPortType eType)
{
	return new CImplicitPort(function()->addOutput(sIdentifier, sDisplayName, eType));
}

IImplicitPortIterator* CFunction::GetOutputs()
{
	return new CImplicitPortIterator(function()->getOutputs());
}

void CFunction::RemoveOutput(IImplicitPort* pOutput)
{
	auto outputs = function()->getOutputs();
	auto it = std::find_if(outputs->begin(), outputs->end(), [pOutput](const NMR::PModelImplicitPort& port) {
		return port->getIdentifier() == pOutput->GetIdentifier();
	});
	if (it != outputs->end())
		outputs->erase(it);
	else
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

IImplicitPort* CFunction::FindInput(const std::string& sIdentifier)
{
	return new CImplicitPort(function()->findInput(sIdentifier));
}

IImplicitPort* CFunction::FindOutput(const std::string& sIdentifier)
{
	return new CImplicitPort(function()->findOutput(sIdentifier));
}
