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

NMR_ModelImage3DChannelSelector.h defines a 3D channel selector for the volumetric extension.

--*/

#ifndef __NMR_MODELIMAGE3DCHANNELSELECTOR
#define __NMR_MODELIMAGE3DCHANNELSELECTOR

#include "Common/NMR_Types.h" 

#include "Model/Classes/NMR_ModelTexture2D.h"

#include <vector>
#include "Common/Platform/NMR_ImportStream.h"  
#include <memory>
#include <map>
#include <string>

namespace NMR {

	class CModelImage3DChannelSelector;
	typedef std::shared_ptr <CModelImage3DChannelSelector> PModelImage3DChannelSelector;

	class CModelImage3DChannelSelector {
	private:
	
		PPackageResourceID m_sImage3DID;
		std::string m_sSourceChannel;
		std::string m_sDstChannel;
		
		nfDouble m_dMinValue;
		nfDouble m_dMaxValue;
		
		eModelTextureTileStyle m_eTileStyleU;
		eModelTextureTileStyle m_eTileStyleV;
		eModelTextureTileStyle m_eTileStyleW;
		
		eModelTextureFilter m_eFilter;

		nfUint32 m_nInternalIndex;

	protected:

		CModelImage3DChannelSelector(PPackageResourceID sImage3DId, std::string sSourceChannel, std::string sDstChannel);

	public:

		~CModelImage3DChannelSelector();

		static PModelImage3DChannelSelector make (PPackageResourceID sImage3DId, std::string sSourceChannel, std::string sDstChannel);
		static PModelImage3DChannelSelector make_from (CModelImage3DChannelSelector * pChannelSelector, const std::map<PPackageResourceID, PPackageResourceID> & PackageIDMap);

		PPackageResourceID getImage3DID ();
		std::string getSourceChannel ();
		std::string getDstChannel ();

		nfDouble getMinValue ();
		nfDouble getMaxValue ();

		eModelTextureTileStyle getTileStyleU ();
		eModelTextureTileStyle getTileStyleV ();
		eModelTextureTileStyle getTileStyleW ();

		eModelTextureFilter getFilter ();


		void setImage3DID(PPackageResourceID sValue);
		void setSourceChannel(std::string sValue);
		void setDstChannel(std::string sValue);

		void setMinValue(nfDouble dValue);
		void setMaxValue(nfDouble dValue);

		void setTileStyleU(eModelTextureTileStyle eValue);
		void setTileStyleV(eModelTextureTileStyle eValue);
		void setTileStyleW(eModelTextureTileStyle eValue);

		void setFilter(eModelTextureFilter eValue);

		void setInternalIndex(nfUint32 nIndex);
		nfUint32 getInternalIndex();

	};

}

#endif // __NMR_MODELIMAGE3DCHANNELSELECTOR

