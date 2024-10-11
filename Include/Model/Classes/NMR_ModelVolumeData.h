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

NMR_VolumeData.h defines the class CVolumeData.

--*/

#ifndef __NMR_VOLUMEDATA
#define __NMR_VOLUMEDATA

#include "Common/NMR_Types.h"
#include "Common/Math/NMR_Geometry.h"
#include "Common/Mesh/NMR_MeshTypes.h"

#include "Common/Mesh/NMR_VolumeDataColor.h"
#include "Common/Mesh/NMR_VolumeDataProperty.h"
#include "Common/Mesh/NMR_VolumeDataComposite.h"
#include "Model/Classes/NMR_ModelResource.h"

namespace NMR {
	class CModelFunction;
	typedef std::shared_ptr<CModelFunction> PModelFunction;

	class CModelVolumeData : public CModelResource {
	private:
		PVolumeDataComposite m_pComposite;
		PVolumeDataColor m_pColor;
		std::map<std::string, PVolumeDataProperty> m_mapProperties;
	public:
         CModelVolumeData() = delete;
         CModelVolumeData(ModelResourceID sResourceID,
                        CModel* pModel);

         void clear();

         nfBool hasProperty(std::string sName);
         nfUint32 getPropertyCount() const;
         PVolumeDataProperty getProperty(nfUint32 nIndex);
         PVolumeDataProperty findProperty(std::string sName);
         void addProperty(PVolumeDataProperty pProperty);
         PVolumeDataProperty addProperty(std::string sName,
                                         PModelFunction pfunction);
         void removeProperty(std::string sName);

         bool hasColor() const;
         PVolumeDataColor getColor();
         PVolumeDataColor createColor(PModelFunction pfunction);
         void setColor(PVolumeDataColor pColor);
         void removeColor();

         bool hasComposite() const;
         PVolumeDataComposite getComposite();
         PVolumeDataComposite createComposite(/* basematerialgroupd*/);
         void setComposite(PVolumeDataComposite pComposite);
         void removeComposite();

         ResourceDependencies getDependencies() override;
         
         PPackageResourceID packageResourceIDFromResourceID(
             UniqueResourceID uniqueResourceID);
    };

	typedef std::shared_ptr <CModelVolumeData> PModelVolumeData;

}

#endif // __NMR_BEAMLATTICE
