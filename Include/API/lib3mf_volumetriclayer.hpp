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

Abstract: This is the class declaration of CVolumetricLayer

*/


#ifndef __LIB3MF_VOLUMETRICLAYER
#define __LIB3MF_VOLUMETRICLAYER

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelVolumetricLayer.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CVolumetricLayer 
**************************************************************************************************************************/

class CVolumetricLayer : public virtual IVolumetricLayer, public virtual CBase {
private:

protected:

	NMR::PModelVolumetricLayer m_pLayer;

public:


	CVolumetricLayer (NMR::PModelVolumetricLayer pLayer);

	Lib3MF::sTransform GetTransform();

	void SetTransform(const Lib3MF::sTransform Transform);

	Lib3MF::eBlendMethod GetBlendMethod();

	void SetBlendMethod(const Lib3MF::eBlendMethod eBlendMethod);

	Lib3MF_double GetSourceAlpha();

	void SetSourceAlpha(const Lib3MF_double dSourceAlpha);

	Lib3MF_double GetDestinationAlpha();

	void SetDestinationAlpha(const Lib3MF_double dDestinationAlpha);

	void GetInformation(Lib3MF::sTransform & sTransform, Lib3MF::eBlendMethod & eBlendMethod, Lib3MF_double & dSourceAlpha, Lib3MF_double & dDestinationAlpha);

	void SetInformation(const Lib3MF::sTransform Transform, const Lib3MF::eBlendMethod eBlendMethod, const Lib3MF_double dSourceAlpha, const Lib3MF_double dDestinationAlpha);

	NMR::PModelVolumetricLayer getModelLayer ();

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_VOLUMETRICLAYER
