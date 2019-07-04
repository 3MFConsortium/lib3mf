/*++

Copyright (C) 2019 3MF Consortium

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

Abstract:

NMR_ModelVolumetricLayer.h defines a volumetric 3D layer for the volumetric extension

--*/

#ifndef __NMR_MODELVOLUMETRICLAYER
#define __NMR_MODELVOLUMETRICLAYER


#include "Common/NMR_Types.h" 
#include "Model/Classes/NMR_ModelTypes.h" 

#include "Model/Classes/NMR_ModelImage3D.h"
#include "Model/Classes/NMR_ModelImage3DChannelSelector.h"

#include <vector>
#include <memory>
#include <map>
#include <string>

#define MAX_VOLUMETRIC_CHANNELS (1024*1024)

namespace NMR {

	class CModelVolumetricLayer;
	typedef std::shared_ptr <CModelVolumetricLayer> PModelVolumetricLayer;

	class CModelVolumetricLayer {
	private:
		MODELTRANSFORM m_Transform;
		eModelBlendMethod m_BlendMethod;
		nfDouble m_dSourceAlpha;
		nfDouble m_dDstAlpha;

		std::vector<PModelImage3DChannelSelector> m_ChannelSelectors;
		PModelImage3DChannelSelector m_MaskChannelSelector;

	protected:
		CModelVolumetricLayer(MODELTRANSFORM Transform, eModelBlendMethod BlendMethod);

	public:

		~CModelVolumetricLayer();

		static PModelVolumetricLayer make(MODELTRANSFORM Transform, eModelBlendMethod BlendMethod);
		static PModelVolumetricLayer make_from(CModelVolumetricLayer * pVolumetricLayer, const std::map<PPackageResourceID, PPackageResourceID> & PackageIDMap);

		MODELTRANSFORM getTransform ();
		eModelBlendMethod getBlendMethod ();
		nfDouble getSourceAlpha ();
		nfDouble getDstAlpha ();

		void setTransform(MODELTRANSFORM Transform);
		void setBlendMethod(eModelBlendMethod BlendMethod);
		void setSourceAlpha(nfDouble srcAlpha);
		void setDstAlpha(nfDouble dstAlpha);

		PModelImage3DChannelSelector getMaskChannelSelector ();
		void setMaskChannelSelector (PModelImage3DChannelSelector ChannelSelector);

		void addChannelSelector (PModelImage3DChannelSelector ChannelSelector);
		nfUint32 getChannelSelectorCount ();
		PModelImage3DChannelSelector getChannelSelector (nfUint32 nIndex);
		void removeChannelSelector (nfUint32 nIndex);
		void clearChannelSelectors();


	};


}

#endif // __NMR_MODELVOLUMETRICLAYER

