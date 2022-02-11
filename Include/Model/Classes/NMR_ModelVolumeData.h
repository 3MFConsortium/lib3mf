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
#include "Common/Mesh/NMR_VolumeDataBoundary.h"
#include "Common/Mesh/NMR_VolumeDataComposite.h"

namespace NMR {

	class CModelScalarField;
	typedef std::shared_ptr<CModelScalarField> PModelScalarField;
	class CModelVector3DField;
	typedef std::shared_ptr<CModelVector3DField> PModelVector3DField;

	class CModelVolumeData {
	private:
		PVolumeDataBoundary m_pBoundary;
		PVolumeDataComposite m_pComposite;
		PVolumeDataColor m_pColor;
		std::map<std::string, PVolumeDataProperty> m_mapProperties;
	public:
		CModelVolumeData();

		void clear();

		bool HasBoundary() const;
		PVolumeDataBoundary GetBoundary();
		PVolumeDataBoundary CreateBoundary(PModelScalarField);
		void SetBoundary(PVolumeDataBoundary pLevelset);

		nfBool hasProperty(std::string sName);
		nfUint32 GetPropertyCount() const;
		PVolumeDataProperty GetProperty(nfUint32 nIndex);
		PVolumeDataProperty FindProperty(std::string sName);
		void AddProperty(PVolumeDataProperty pProperty);
		PVolumeDataProperty AddProperty(std::string sName, PModelScalarField pScalarField);
		PVolumeDataProperty AddProperty(std::string sName, PModelVector3DField pVector3DField);
		void RemoveProperty(std::string sName);

		bool HasColor() const;
		PVolumeDataColor GetColor();
		PVolumeDataColor CreateColor(PModelVector3DField pVector3DField);
		void SetColor(PVolumeDataColor pColor);
	};

	typedef std::shared_ptr <CModelVolumeData> PModelVolumeData;

}

#endif // __NMR_BEAMLATTICE
