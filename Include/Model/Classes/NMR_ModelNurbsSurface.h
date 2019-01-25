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

NMR_ModelNurbsSurface.h defines the Model Nurbs Surface Class.

--*/

#ifndef __NMR_MODELNURBSSURFACE
#define __NMR_MODELNURBSSURFACE

#include "Common/NMR_Types.h" 


#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"  
#include <vector>
#include "Common/Platform/NMR_ImportStream.h"  
#include <memory>
#include <map>
#include <functional>
#include <string>

#define MAXNURBSDEGREE 32
#define MAXNURBSCONTROLPOINTS 16384
#define MAXNURBSKNOTS 65536
#define MAXNURBSMULTIPLICITY 65536

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;
	typedef std::function<void(uint32_t nID, double dU, double dV)> NurbsSurfaceUVWalker;

	typedef struct {
		nfUint32 m_Multiplicity;
		nfDouble m_Value;
	} sModelNurbsSurfaceKnot;

	typedef struct {
		nfDouble m_Position[3];
		nfDouble m_Weight;
	} sModelNurbsControlPoint;

	typedef struct {
		nfDouble m_U;
		nfDouble m_V;
	} sModelNurbsUVCoord;

	class CModelNurbsSurface : public CModelResource {
	private:
		nfUint32 m_nDegreeU;
		nfUint32 m_nDegreeV;

		nfDouble m_dMinU;
		nfDouble m_dMinV;
		nfDouble m_dMaxU;
		nfDouble m_dMaxV;

		nfUint32 m_ControlPointCountU;
		nfUint32 m_ControlPointCountV;

		std::vector<sModelNurbsSurfaceKnot> m_KnotsU;
		std::vector<sModelNurbsSurfaceKnot> m_KnotsV;
		std::vector<sModelNurbsControlPoint> m_ControlPoints;

		std::map<nfUint32, sModelNurbsUVCoord> m_UVCoords;
		nfUint32 m_nNextUVID;

	public:
		CModelNurbsSurface() = delete;
		CModelNurbsSurface(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ nfUint32 nDegreeU, _In_ nfUint32 nDegreeV, _In_ nfUint32 nControlPointCountU, _In_ nfUint32 nControlPointCountV);

		// getters/setters Degree
		nfUint32 getDegreeU();
		nfUint32 getDegreeV();

		void setDegree(_In_ nfUint32 nDegreeU, _In_ nfUint32 nDegreeV);
		void setControlPointCount(_In_ nfUint32 nControlPointCountU, _In_ nfUint32 nControlPointCountV);
							
		void copyFrom(_In_ CModelNurbsSurface * pSourceNurbs);

		nfBool CheckValidity();

		void addKnotU(_In_ const nfUint32 nMultiplicity, _In_ const nfDouble dValue);
		nfUint32 getKnotCountU();
		void getKnotU(_In_ nfUint32 nIndex, _Out_ nfUint32 & nMultiplicity, _Out_ nfDouble & dValue);
		void clearKnotsU();
		void addKnotsU(const std::vector<sModelNurbsSurfaceKnot> & Knots);

		void addKnotV(_In_ const nfUint32 nMultiplicity, _In_ const nfDouble dValue);
		nfUint32 getKnotCountV();
		void getKnotV(_In_ nfUint32 nIndex, _Out_ nfUint32 & nMultiplicity, _Out_ nfDouble & dValue);
		void clearKnotsV();
		void addKnotsV(const std::vector<sModelNurbsSurfaceKnot> & Knots);


		void setControlPoint(_In_ const nfUint32 nIndexU, _In_ const nfUint32 nIndexV, _In_ const nfDouble dX, _In_ const nfDouble dY, _In_ const nfDouble dZ, _In_ const nfDouble dW);
		nfUint32 getControlPointCountU();
		nfUint32 getControlPointCountV();
		void getControlPoint(_In_ const nfUint32 nIndexU, _In_ const nfUint32 nIndexV, _Out_ nfDouble & dX, _Out_ nfDouble & dY, _Out_ nfDouble & dZ, _Out_ nfDouble & dW);

		nfUint32 addUVCoordinate (_In_ nfDouble fU, _In_ nfDouble fV);
		nfBool getUVCoordinate (_In_ nfUint32 nID, _Out_ nfDouble & fU, _Out_ nfDouble & fV);
		nfUint32 getUVCoordinateCount();
		void walkUVCoordinates(NurbsSurfaceUVWalker Walker);

		void setUVBounds(nfDouble dMinU, nfDouble dMinV, nfDouble dMaxU, nfDouble dMaxV);
		void getUVBounds(nfDouble & dMinU, nfDouble & dMinV, nfDouble & dMaxU, nfDouble & dMaxV);



	};

	typedef std::shared_ptr <CModelNurbsSurface> PModelNurbsSurface;

}

#endif // __NMR_MODELNURBSSURFACE

