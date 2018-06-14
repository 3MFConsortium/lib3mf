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

NMR_PackageResourceID.h defines the PackageResourceID Class.

--*/

#ifndef __NMR_PACKAGERESOURCEID
#define __NMR_PACKAGERESOURCEID

#include "Model/Classes/NMR_ModelTypes.h"
#include <string>

#include <memory>
#include <map>
#include <vector>

namespace NMR {

	class CPackageResourceID {
	private:
		std::string m_path;
		ModelResourceID m_id;	// combination of those two must be unique

		PackageResourceID m_uniqueID;	// the unique Identifier
	public:
		void set(std::string p, ModelResourceID id);
		void get(std::string& p);
		void get(ModelResourceID& id);
		void setUniqueID(PackageResourceID id);
		PackageResourceID getUniqueID();
	};
	typedef std::shared_ptr<CPackageResourceID> PPackageResourceID;

	class CResourceHandler {
	private:
		// unique IDs to CPackageResourceID
		std::map<PackageResourceID, PPackageResourceID> m_resourceIDs;
	public:
		PPackageResourceID getNewRessourceID(std::wstring path, ModelResourceID id);
		PPackageResourceID getNewRessourceID(std::string path, ModelResourceID id);	// this is supposed to be the only way to generate a CPackageResourceID
		PPackageResourceID findRessourceID(PackageResourceID id);
		PPackageResourceID findRessourceID(std::wstring path, ModelResourceID id);
		PPackageResourceID findRessourceID(std::string path, ModelResourceID id);

		void FlattenIDs();

		void clear();
	};

}

#endif // __NMR_PACKAGERESOURCEID
