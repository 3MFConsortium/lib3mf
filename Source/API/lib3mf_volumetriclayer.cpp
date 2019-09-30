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
#include "lib3mf_image3d.hpp"
#include "lib3mf_image3dchannelselector.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVolumetricLayer 
**************************************************************************************************************************/

CVolumetricLayer::CVolumetricLayer(NMR::PModelVolumetricLayer pLayer, NMR::CModel *pModel)
	: m_pLayer(pLayer), m_pModel(pModel)
{
	if (pLayer.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	if (pModel == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}


Lib3MF::sTransform CVolumetricLayer::GetTransform()
{
	return Lib3MF::MatrixToTransform(m_pLayer->getTransform());
}

void CVolumetricLayer::SetTransform(const Lib3MF::sTransform Transform)
{
	m_pLayer->setTransform(Lib3MF::TransformToMatrix (Transform));
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
	sTransform = Lib3MF::MatrixToTransform(m_pLayer->getTransform());
	eBlendMethod =(Lib3MF::eBlendMethod) m_pLayer->getBlendMethod();
	dSourceAlpha = m_pLayer->getSourceAlpha();
	dDestinationAlpha = m_pLayer->getDstAlpha();
}

void CVolumetricLayer::SetInformation(const Lib3MF::sTransform Transform, const Lib3MF::eBlendMethod eBlendMethod, const Lib3MF_double dSourceAlpha, const Lib3MF_double dDestinationAlpha)
{
	m_pLayer->setTransform(Lib3MF::TransformToMatrix(Transform));
	m_pLayer->setBlendMethod((NMR::eModelBlendMethod) eBlendMethod);
	m_pLayer->setSourceAlpha(dSourceAlpha);
	m_pLayer->setDstAlpha(dDestinationAlpha);
}

IImage3DChannelSelector * CVolumetricLayer::CreateMaskChannelSelector(IImage3D* pImage3D, const std::string & sSourceChannel, const std::string & sDestinationChannel)
{
	if (pImage3D == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CImage3D * pImage3DClass = dynamic_cast<CImage3D *> (pImage3D);
	if (pImage3DClass == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);
	
	NMR::PModelImage3DChannelSelector pChannelSelector = NMR::CModelImage3DChannelSelector::make (pImage3DClass->getModelImage3D ()->getResourceID(), sSourceChannel, sDestinationChannel );
	m_pLayer->setMaskChannelSelector(pChannelSelector);
	return new CImage3DChannelSelector(m_pModel, pChannelSelector);
}

bool CVolumetricLayer::HasMaskChannelSelector()
{
	return m_pLayer->hasMaskChannelSelector();
}

void CVolumetricLayer::ClearMaskChannelSelector()
{
	m_pLayer->setMaskChannelSelector (nullptr);
}

IImage3DChannelSelector * CVolumetricLayer::GetMaskChannelSelector()
{
	if (!m_pLayer->hasMaskChannelSelector())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_NOMASKCHANNELSELECTOR);

	NMR::PModelImage3DChannelSelector pChannelSelector = m_pLayer->getMaskChannelSelector();
	return new CImage3DChannelSelector(m_pModel, pChannelSelector);
}

Lib3MF_uint32 CVolumetricLayer::GetChannelSelectorCount()
{
	return m_pLayer->getChannelSelectorCount();
}

IImage3DChannelSelector * CVolumetricLayer::GetChannelSelector(const Lib3MF_uint32 nIndex)
{
	NMR::PModelImage3DChannelSelector pChannelSelector = m_pLayer->getChannelSelector(nIndex);
	return new CImage3DChannelSelector(m_pModel, pChannelSelector);
}

IImage3DChannelSelector * CVolumetricLayer::AddChannelSelector(IImage3D* pImage3D, const std::string & sSourceChannel, const std::string & sDestinationChannel)
{
	if (pImage3D == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CImage3D * pImage3DClass = dynamic_cast<CImage3D *> (pImage3D);
	if (pImage3DClass == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	NMR::PModelImage3DChannelSelector pChannelSelector = NMR::CModelImage3DChannelSelector::make(pImage3DClass->getModelImage3D()->getResourceID(), sSourceChannel, sDestinationChannel);
	m_pLayer->addChannelSelector(pChannelSelector);
	return new CImage3DChannelSelector(m_pModel, pChannelSelector);

}

void CVolumetricLayer::ClearChannelSelectors()
{
	m_pLayer->clearChannelSelectors();
}

void CVolumetricLayer::ReindexChannelSelector(IImage3DChannelSelector* pChannelSelector, const Lib3MF_uint32 nIndex)
{
	if (pChannelSelector == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CImage3DChannelSelector * pChannelSelectorClass = dynamic_cast<CImage3DChannelSelector *> (pChannelSelector);
	if (pChannelSelectorClass == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	m_pLayer->reIndexChannelSelector(pChannelSelectorClass->getModelSelector().get(), nIndex);

}

void CVolumetricLayer::RemoveChannelSelector(IImage3DChannelSelector* pChannelSelector)
{
	if (pChannelSelector == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CImage3DChannelSelector * pChannelSelectorClass = dynamic_cast<CImage3DChannelSelector *> (pChannelSelector);
	if (pChannelSelectorClass == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	m_pLayer->removeChannelSelector(pChannelSelectorClass->getModelSelector ().get());

}

void CVolumetricLayer::RemoveChannelSelectorByIndex(const Lib3MF_uint32 nIndex)
{
	m_pLayer->removeChannelSelectorByIndex(nIndex);
}


NMR::PModelVolumetricLayer CVolumetricLayer::getModelLayer()
{
	return m_pLayer;
}

