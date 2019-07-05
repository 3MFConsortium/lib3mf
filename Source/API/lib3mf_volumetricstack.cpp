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

Abstract: This is a stub class definition of CVolumetricStack

*/

#include "lib3mf_volumetricstack.hpp"
#include "lib3mf_volumetriclayer.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_utils.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVolumetricStack 
**************************************************************************************************************************/

CVolumetricStack::CVolumetricStack(NMR::PModelVolumetricStack pVolumetricStack)
	:	CResource (pVolumetricStack), m_pVolumetricStack (pVolumetricStack)
{
	
}


Lib3MF_uint32 CVolumetricStack::GetDestinationChannelCount()
{
	return m_pVolumetricStack->getDstChannelCount();
}

Lib3MF_uint32 CVolumetricStack::AddDestinationChannel(const std::string & sName, const Lib3MF_double dBackground)
{
	return m_pVolumetricStack->addDstChannel(sName, dBackground);
}

void CVolumetricStack::UpdateDestinationChannel(const Lib3MF_uint32 nIndex, const Lib3MF_double dBackground)
{
	auto pChannel = m_pVolumetricStack->getDstChannel(nIndex);
	pChannel->setBackground(dBackground);
}

void CVolumetricStack::UpdateDestinationChannelByName(const std::string & sName, const Lib3MF_double dBackground)
{
	unsigned int nIndex = m_pVolumetricStack->findDstChannel(sName);
	if (nIndex >= m_pVolumetricStack->getDstChannelCount ())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	auto pChannel = m_pVolumetricStack->getDstChannel(nIndex);
	pChannel->setBackground(dBackground);
}

void CVolumetricStack::RemoveDestinationChannel(const Lib3MF_uint32 nIndex)
{
	m_pVolumetricStack->removeDstChannel(nIndex);
}

void CVolumetricStack::RemoveDestinationChannelByName(const std::string & sName)
{
	unsigned int nIndex = m_pVolumetricStack->findDstChannel(sName);
	if (nIndex >= m_pVolumetricStack->getDstChannelCount())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	m_pVolumetricStack->removeDstChannel(nIndex);
}

Lib3MF_uint32 CVolumetricStack::GetLayerCount()
{
	return m_pVolumetricStack->getLayerCount();
}

IVolumetricLayer * CVolumetricStack::GetLayer(const Lib3MF_uint32 nIndex)
{
	return new CVolumetricLayer (m_pVolumetricStack->getLayer (nIndex));
}

IVolumetricLayer * CVolumetricStack::AddLayer(const Lib3MF::sTransform Transform, const Lib3MF::eBlendMethod eBlendMethod)
{
	return new CVolumetricLayer(m_pVolumetricStack->addLayer (Lib3MF::TransformToModelTransform (Transform), NMR::eModelBlendMethod (eBlendMethod)));
}

void CVolumetricStack::ReindexLayer(IVolumetricLayer* pLayer, const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CVolumetricStack::RemoveLayer(IVolumetricLayer* pLayer)
{
	if (pLayer == nullptr)
	  throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CVolumetricLayer * pLayerClass = dynamic_cast<CVolumetricLayer *> (pLayer);

	if (pLayerClass == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	m_pVolumetricStack->removeLayer(pLayerClass->getModelLayer().get());
}

void CVolumetricStack::RemoveLayerByIndex(const Lib3MF_uint32 nIndex)
{
	m_pVolumetricStack->removeLayerByIndex(nIndex);
}

