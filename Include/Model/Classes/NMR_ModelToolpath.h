/*++

Copyright (C) 2018 3MF Consortium

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

NMR_ModelToolpath.h defines the Model Toolpath.

--*/

#ifndef __NMR_MODELTOOLPATH
#define __NMR_MODELTOOLPATH

#include "Common/NMR_Types.h" 


#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_ModelToolpathLayer.h" 
#include "Model/Classes/NMR_CustomXMLTree.h"  
#include "Model/Classes/NMR_ModelToolpathProfile.h" 
#include "Model/Classes/NMR_Model.h"  
#include <vector>
#include "Common/Platform/NMR_ImportStream.h"  
#include <memory>
#include <map>
#include <string>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelToolpath;
	typedef std::shared_ptr <CModelToolpath> PModelToolpath;

	class CModelToolpath : public CModelResource {
	private:
		CUUID m_UUID;

		nfUint32 m_nBottomZ;
		double m_dUnitFactor;
		std::vector<PModelToolpathLayer> m_Layers;
		std::vector<PModelToolpathProfile> m_Profiles;
		std::map<std::string, PModelToolpathProfile> m_ProfileMap;

		std::vector<PCustomXMLTree> m_CustomXMLData;

	public:
		CModelToolpath() = delete;
		CModelToolpath(_In_ const ModelResourceID sID, _In_ CModel * pModel, double dUnitFactor, CUUID uuid, nfUint32 nBottomZ);

		static PModelToolpath make(_In_ const ModelResourceID sID, _In_ CModel * pModel, double dUnitFactor, CUUID uuid, nfUint32 nBottomZ);

		nfUint32 getBottomZ();
		void setBottomZ(nfUint32 nBottomZ);

		PModelToolpathLayer addLayer (const std::string & sPath, nfUint32 nMaxZ);
		nfUint32 getLayerCount();
		PModelToolpathLayer getLayer(nfUint32 nIndex);

		PModelToolpathProfile addProfile(const std::string & sName);
		PModelToolpathProfile addExistingProfile(const std::string & sUUID, const std::string & sName);

		nfUint32 getProfileCount();
		PModelToolpathProfile getProfile(nfUint32 nIndex);
		PModelToolpathProfile getProfileByUUID(std::string sUUID);

		double getUnitFactor ();
		CUUID getUUID ();
		void resetUUID();

		uint32_t getCustomXMLDataCount();
		PCustomXMLTree getCustomXMLData(uint32_t nIndex);
		void addCustomXMLData(PCustomXMLTree pCustomXMLTree);
		uint32_t clearCustomXMLData();
		bool deleteCustomXMLData(CCustomXMLTree * pCustomXMLTree);

	};


}

#endif // __NMR_MODELTOOLPATH

