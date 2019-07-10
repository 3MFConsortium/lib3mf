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

#ifndef __NMR_MODELVOLUMETRICSTACK
#define __NMR_MODELVOLUMETRICSTACK


#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"  
#include "Model/Classes/NMR_ModelVolumetricDstChannel.h"  
#include "Model/Classes/NMR_ModelVolumetricLayer.h"  
#include <vector>
#include <memory>
#include <map>
#include <string>


namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	typedef std::shared_ptr <CModelVolumetricStack> PModelVolumetricStack;

	class CModelVolumetricStack : public CModelResource {
	private:

		std::vector<PModelVolumetricDstChannel> m_DstChannels;
		std::map<std::string, nfUint32> m_DstChannelNameMap;
		

		std::vector<PModelVolumetricLayer> m_Layers;

	protected:
		CModelVolumetricStack(_In_ const ModelResourceID sID, _In_ CModel * pModel);

	public:
		CModelVolumetricStack() = delete;
		
		static PModelVolumetricStack make(_In_ const ModelResourceID sID, _In_ CModel * pModel);
		static PModelVolumetricStack make_from(_In_ CModelVolumetricStack * pVolumetricStack, _In_ const ModelResourceID sID, _In_ CModel * pNewModel, _In_ const std::map<PPackageResourceID, PPackageResourceID> & PackageIDMap);

		nfUint32 getDstChannelCount();
		PModelVolumetricDstChannel getDstChannel(nfUint32 nIndex);
		nfUint32 addDstChannel(const std::string & sName, nfDouble dBackground);
		void removeDstChannel(nfUint32 nIndex);
		nfUint32 findDstChannel(const std::string & sName);
		bool hasDstChannel(const std::string & sName);
		void clearDstChannels();
		void clearUnusedDstChannels();


		nfUint32 getLayerCount();
		PModelVolumetricLayer getLayer(nfUint32 nIndex);
		PModelVolumetricLayer addLayer(NMATRIX3 Transform, eModelBlendMethod BlendMethod);
		PModelVolumetricLayer addLayerFrom(CModelVolumetricLayer * pSourceLayer, const std::map<PPackageResourceID, PPackageResourceID> & PackageIDMap);
		void removeLayerByIndex(nfUint32 nIndex);
 		void removeLayer(CModelVolumetricLayer * pLayer);

		void reIndexLayer(CModelVolumetricLayer * pLayer, nfUint32 nNewIndex);

		void clear();
	};

}

#endif // __NMR_MODELVOLUMETRICSTACK

