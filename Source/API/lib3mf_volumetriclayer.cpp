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

Abstract: This is a stub class definition of CVolumetricLayer

*/

#include "lib3mf_volumetriclayer.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_utils.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVolumetricLayer 
**************************************************************************************************************************/

CVolumetricLayer::CVolumetricLayer(NMR::PModelVolumetricLayer pLayer)
	: m_pLayer (pLayer)
{
	if (pLayer.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

}


Lib3MF::sTransform CVolumetricLayer::GetTransform()
{
	return Lib3MF::ModelTransformToTransform(m_pLayer->getTransform());
}

void CVolumetricLayer::SetTransform(const Lib3MF::sTransform Transform)
{
	m_pLayer->setTransform(Lib3MF::TransformToModelTransform (Transform));
}

Lib3MF::eBlendMethod CVolumetricLayer::GetBlendMethod()
{
	return (Lib3MF::eBlendMethod) m_pLayer->getBlendMethod();
}

void CVolumetricLayer::SetBlendMethod(const Lib3MF::eBlendMethod eBlendMethod)
{
	m_pLayer->setBlendMethod((NMR::eModelBlendMethod) eBlendMethod);
}

Lib3MF_double CVolumetricLayer::GetSourceAlpha()
{
	return m_pLayer->getSourceAlpha();
}

void CVolumetricLayer::SetSourceAlpha(const Lib3MF_double dSourceAlpha)
{
	m_pLayer->setSourceAlpha(dSourceAlpha);
}

Lib3MF_double CVolumetricLayer::GetDestinationAlpha()
{
	return m_pLayer->getDstAlpha();
}

void CVolumetricLayer::SetDestinationAlpha(const Lib3MF_double dDestinationAlpha)
{
	m_pLayer->setDstAlpha(dDestinationAlpha);
}

void CVolumetricLayer::GetInformation(Lib3MF::sTransform & sTransform, Lib3MF::eBlendMethod & eBlendMethod, Lib3MF_double & dSourceAlpha, Lib3MF_double & dDestinationAlpha)
{
	sTransform = Lib3MF::ModelTransformToTransform(m_pLayer->getTransform());
	eBlendMethod =(Lib3MF::eBlendMethod) m_pLayer->getBlendMethod();
	dSourceAlpha = m_pLayer->getSourceAlpha();
	dDestinationAlpha = m_pLayer->getDstAlpha();
}

void CVolumetricLayer::SetInformation(const Lib3MF::sTransform Transform, const Lib3MF::eBlendMethod eBlendMethod, const Lib3MF_double dSourceAlpha, const Lib3MF_double dDestinationAlpha)
{
	m_pLayer->setTransform(Lib3MF::TransformToModelTransform(Transform));
	m_pLayer->setBlendMethod((NMR::eModelBlendMethod) eBlendMethod);
	m_pLayer->setSourceAlpha(dSourceAlpha);
	m_pLayer->setDstAlpha(dDestinationAlpha);
}

NMR::PModelVolumetricLayer CVolumetricLayer::getModelLayer()
{
	return m_pLayer;
}

