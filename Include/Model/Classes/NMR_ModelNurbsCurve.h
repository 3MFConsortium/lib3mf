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

NMR_ModelNurbsCurve.h defines the Model Nurbs Curve Class.

--*/

#ifndef __NMR_MODELNURBSCURVE
#define __NMR_MODELNURBSCURVE

#include "Common/NMR_Types.h" 


#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"  
#include "Model/Classes/NMR_ModelNurbsSurface.h"  
#include <vector>
#include "Common/Platform/NMR_ImportStream.h"  
#include <memory>
#include <map>
#include <functional>
#include <string>

#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;
	
	class CModelNurbsCurve : public CModelResource {
	private:
		nfUint32 m_nDegree;

		nfDouble m_dMinT;
		nfDouble m_dMaxT;

		std::vector<sModelNurbsSurfaceKnot> m_Knots;
		std::vector<sModelNurbsControlPoint> m_ControlPoints;

	public:
		CModelNurbsCurve() = delete;
		CModelNurbsCurve(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ nfUint32 nDegree, _In_ nfUint32 nControlPointCount);

		// getters/setters Degree
		nfUint32 getDegree();

		void setDegree(_In_ nfUint32 nDegree);
		void setControlPointCount(_In_ nfUint32 nControlPointCount);
							
		nfBool CheckValidity();

		void addKnot(_In_ const nfUint32 nMultiplicity, _In_ const nfDouble dValue);
		nfUint32 getKnotCount();
		void getKnot(_In_ nfUint32 nIndex, _Out_ nfUint32 & nMultiplicity, _Out_ nfDouble & dValue);
		void clearKnots();
		void addKnots(const std::vector<sModelNurbsSurfaceKnot> & Knots);

		void setControlPoint(_In_ const nfUint32 nIndex,_In_ const nfDouble dX, _In_ const nfDouble dY, _In_ const nfDouble dZ, _In_ const nfDouble dW);
		nfUint32 getControlPointCount();
		void getControlPoint(_In_ const nfUint32 nIndex, _Out_ nfDouble & dX, _Out_ nfDouble & dY, _Out_ nfDouble & dZ, _Out_ nfDouble & dW);

		void setTBounds(nfDouble dMinT, nfDouble dMaxT);
		void getTBounds(nfDouble & dMinT, nfDouble & dMaxT);

	};

	typedef std::shared_ptr <CModelNurbsCurve> PModelNurbsCurve;

}

#endif // __NMR_MODELNURBSCURVE


