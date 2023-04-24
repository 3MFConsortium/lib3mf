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

Abstract: This is a stub class definition of CImplicitFunction

*/

#include "lib3mf_implicitfunction.hpp"
#include "lib3mf_implicitnode.hpp"
#include "lib3mf_implicitport.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_nodeiterator.hpp"
#include "lib3mf_implicitportiterator.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CImplicitFunction 
**************************************************************************************************************************/

NMR::PModelImplicitFunction Lib3MF::Impl::CImplicitFunction::function() 
{
	if (m_function.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);

	return m_function;
}

Lib3MF::Impl::CImplicitFunction::CImplicitFunction(NMR::PModelImplicitFunction pResource)
    : CResource(pResource), m_function(pResource)
{
}

std::string CImplicitFunction::GetIdentifier()
{
	return function()->getIdentifier();
}

void CImplicitFunction::SetIdentifier(const std::string & sIdentifier)
{
	function()->setIdentifier(sIdentifier);
}

std::string CImplicitFunction::GetDisplayName()
{
	return function()->getDisplayName();
}

void CImplicitFunction::SetDisplayName(const std::string & sDisplayName)
{
	function()->setDisplayName(sDisplayName);
}

IImplicitNode * CImplicitFunction::AddNode(const Lib3MF::eImplicitNodeType eNodeType, const std::string & sIdentifier, const std::string & sDisplayName)
{
	auto newNode = function()->addNode(eNodeType, sIdentifier, sDisplayName);
	return new CImplicitNode(newNode);
}

INodeIterator * CImplicitFunction::GetNodes()
{
	return new CNodeIterator(function()->getNodes());
}

void CImplicitFunction::RemoveNode(IImplicitNode* pNode)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IImplicitPort *  CImplicitFunction::AddInput(const std::string & sIdentifier,
                                     const std::string & sDisplayName,
                                     const Lib3MF::eImplicitPortType eType)
{
	
	return new CImplicitPort(function()->addInput(sIdentifier, sDisplayName, eType));
}

IImplicitPortIterator * CImplicitFunction::GetInputs()
{
	 return new CImplicitPortIterator(m_function->getInputs());
}

void CImplicitFunction::RemoveInput(IImplicitPort* pInput)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IImplicitPort * CImplicitFunction::AddOutput(const std::string & sIdentifier,
                                              const std::string & sDisplayName,
                                              const Lib3MF::eImplicitPortType eType)
{
	return new CImplicitPort(function()->addOutput(sIdentifier, sDisplayName, eType));
}

IImplicitPortIterator * CImplicitFunction::GetOutputs()
{
	return new CImplicitPortIterator(m_function->getOutputs());
}

void CImplicitFunction::RemoveOutput(IImplicitPort* pOutput)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CImplicitFunction::AddLink(IImplicitPort* pSource, IImplicitPort* pTarget)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CImplicitFunction::AddLinkByNames(const std::string & sSource, const std::string & sTarget)
{
	m_function->addLink(sSource, sTarget);
}