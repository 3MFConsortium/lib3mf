/*++

Copyright (C) 2023 3MF Consortium

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

NMR_FunctionReference.h defines the class CFunctionReference.

--*/

#pragma once

#include "Common/NMR_Types.h"
#include "Common/Math/NMR_Geometry.h"
#include "Common/Math/NMR_Matrix.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Model/Classes/NMR_PackageResourceID.h"

namespace NMR {
	class CModelFunction;
	typedef std::shared_ptr<CModelFunction> PModelFunction;

	class CFunctionReference {
	private:
		UniqueResourceID m_pFunctionResourceID;
		std::string m_outputPortName;

		NMR::NMATRIX3 m_transform = NMR::fnMATRIX3_identity();
		bool m_bHasTransform = false;
	public:
		CFunctionReference() = delete;
		explicit CFunctionReference(UniqueResourceID functionResourceId);
		explicit CFunctionReference(PModelFunction pFunction);

		virtual void setFunctionResourceID(UniqueResourceID functionID);
		virtual UniqueResourceID getFunctionResourceID() const;

		void setOutputPortName(std::string outputPortName);
		std::string getOutputPortName() const;

		void setTransform(NMR::NMATRIX3 transform);
		NMR::NMATRIX3 getTransform() const;

		bool hasTransform() const;
	};

	typedef std::shared_ptr <CFunctionReference> PFunctionReference;
}
