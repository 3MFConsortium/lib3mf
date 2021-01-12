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

NMR_PackageResourceID.h defines the UniqueResourceID Class.

--*/

#ifndef __NMR_PACKAGERESOURCEID
#define __NMR_PACKAGERESOURCEID

#include "Model/Classes/NMR_ModelTypes.h"
#include <string>

#include <memory>
#include <map>
#include <unordered_map>
#include <vector>

namespace NMR {

	class CResourceHandler;


	class CPackageModelPath {
	private:
		CResourceHandler* m_pResourceHandler;
		std::string m_sPath;

	public:
		CPackageModelPath(CResourceHandler* pResourceHandler, std::string sPath);

		friend class CResourceHandler;

		std::string getPath();
		void setPath(std::string sPath);
	};
	typedef std::shared_ptr<CPackageModelPath> PPackageModelPath;


	class CPackageResourceID {
	private:
		CResourceHandler* m_pResourceHandler;
		// combination of those two must be unique
		PPackageModelPath m_pModelPath;
		ModelResourceID m_id;

		// the unique Identifier that MUST not be persistent into a .3mf-file.
		// it is only valid thoughout the lifetime of a NMR::CModel
		UniqueResourceID m_uniqueID;

		void setUniqueID(UniqueResourceID id);

	public:
		CPackageResourceID(CResourceHandler* pResourceHandler, PPackageModelPath pModelPath, ModelResourceID nID);

		friend class CResourceHandler;

		std::string getPath();
		PPackageModelPath getPackageModelPath();
		ModelResourceID getModelResourceID();
		UniqueResourceID getUniqueID();

		static void setModelPath(std::shared_ptr<CPackageResourceID> pPackageResourceID, PPackageModelPath pPath);
		CResourceHandler * getResourceHandler();
	};
	typedef std::shared_ptr<CPackageResourceID> PPackageResourceID;


	using UniqueIDPackageIdMap = std::unordered_map<UniqueResourceID, PPackageResourceID>;
	using UniqueIdPackageIdPair = std::pair<UniqueResourceID, PPackageResourceID>;

	class CResourceHandler {
	private:
		// getPath-strings to ModelPaths
		std::unordered_map<std::string, PPackageModelPath> m_PathToModelPath;

		// unique IDs to CPackageResourceID
		UniqueIDPackageIdMap m_resourceIDs;
		std::map<std::pair<ModelResourceID, PPackageModelPath>, PPackageResourceID> m_IdAndPathToPackageResourceIDs;
	public:
		PPackageResourceID makePackageResourceID(std::string path, ModelResourceID id);	// this is supposed to be the only way to generate a CPackageResourceID
		
		PPackageResourceID findResourceIDByUniqueID(UniqueResourceID id);
		PPackageResourceID findResourceIDByPair(std::string path, ModelResourceID id);

		PPackageModelPath findPackageModelPath(std::string sPath);
		PPackageModelPath makePackageModelPath(std::string sPath);

		void updateModelPath(PPackageResourceID pPackageResourceID, PPackageModelPath pNewPath);
		std::vector<PPackageModelPath> retrieveAllModelPaths();

		void removePackageResourceID(PPackageResourceID pPackageResourceID);

		void clear();
	};

}

#endif // __NMR_PACKAGERESOURCEID
