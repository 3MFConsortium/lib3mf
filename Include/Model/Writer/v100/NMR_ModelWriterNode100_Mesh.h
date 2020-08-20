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

NMR_ModelWriterNode_Mesh.h defines the Model Writer Mesh Node Class.
This is the class for exporting the 3mf mesh node.

--*/

#ifndef __NMR_MODELWRITERNODE100_MESH
#define __NMR_MODELWRITERNODE100_MESH

#include "Model/Writer/NMR_ModelWriterNode_ModelBase.h" 
#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 

#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"

#include "Common/Platform/NMR_XmlWriter.h"
#include <array>

#define MODELWRITERMESH100_LINEBUFFERSIZE 1024
#define MODELWRITERMESH100_VERTEXLINESTART "<vertex x=\""
#define MODELWRITERMESH100_TRIANGLELINESTART "<triangle v1=\""
#define MODELWRITERMESH100_VERTEXLINESTARTLENGTH 11
#define MODELWRITERMESH100_TRIANGLELINESTARTLENGTH 14
#define MODELWRITERMESH100_BEAMLATTICE_BEAMLINESTART "<b:beam v1=\""
#define MODELWRITERMESH100_BEAMLATTICE_BEAMSTARTLENGTH 12
#define MODELWRITERMESH100_BEAMLATTICE_BALLLINESTART "<b:ball vindex=\""
#define MODELWRITERMESH100_BEAMLATTICE_BALLSTARTLENGTH 16
#define MODELWRITERMESH100_BEAMLATTICE_REFLINESTART  "<b:ref index=\""
#define MODELWRITERMESH100_BEAMLATTICE_REFSTARTLENGTH 14
#define MODELWRITERMESH100_BEAMLATTICE_BALLREFLINESTART  "<b:ballref index=\""
#define MODELWRITERMESH100_BEAMLATTICE_BALLREFSTARTLENGTH 18


namespace NMR {

	class CModelWriterNode100_Mesh : public CModelWriterNode_ModelBase {
	protected:
		CModelMeshObject * m_pModelMeshObject;

		PMeshInformation_PropertyIndexMapping m_pPropertyIndexMapping;

		nfBool m_bWriteMaterialExtension;
		nfBool m_bWriteBeamLatticeExtension;

		// Internal functions for an efficient and buffered output of raw XML data
		std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> m_VertexLine;
		std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> m_TriangleLine;
		std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> m_BeamLine;
		std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> m_BallLine;
		std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> m_BeamRefLine;
		std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> m_BallRefLine;
		nfUint32 m_nVertexBufferPos;
		nfUint32 m_nTriangleBufferPos;
		nfUint32 m_nBeamBufferPos;
		nfUint32 m_nBallBufferPos;
		nfUint32 m_nBeamRefBufferPos;
		nfUint32 m_nBallRefBufferPos;
	private:
		const int m_nPosAfterDecPoint;
		const int m_nPutDoubleFactor;
		__NMR_INLINE void putFloat(_In_ const nfFloat fValue, _In_ std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> & line, _In_ nfUint32 & nBufferPos);
		__NMR_INLINE void putDouble(_In_ const nfDouble dValue, _In_ std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> & line, _In_ nfUint32 & nBufferPos);

	protected:
		__NMR_INLINE void putVertexString(_In_ const nfChar * pszString);
		__NMR_INLINE void putVertexFloat(_In_ const nfFloat fValue);

		__NMR_INLINE void putTriangleString(_In_ const nfChar * pszString);
		__NMR_INLINE void putTriangleUInt32(_In_ const nfUint32 nValue);

		__NMR_INLINE void putBeamString(_In_ const nfChar * pszString);
		__NMR_INLINE void putBeamUInt32(_In_ const nfUint32 nValue);
		__NMR_INLINE void putBeamDouble(_In_ const nfDouble dValue);

		__NMR_INLINE void putBallString(_In_ const nfChar * pszString);
		__NMR_INLINE void putBallUInt32(_In_ const nfUint32 nValue);
		__NMR_INLINE void putBallDouble(_In_ const nfDouble dValue);

		__NMR_INLINE void putBeamRefString(_In_ const nfChar * pszString);
		__NMR_INLINE void putBeamRefUInt32(_In_ const nfUint32 nValue);

		__NMR_INLINE void putBallRefString(_In_ const nfChar * pszString);
		__NMR_INLINE void putBallRefUInt32(_In_ const nfUint32 nValue);

		__NMR_INLINE void writeVertexData(_In_ MESHNODE * pNode);
		__NMR_INLINE void writeFaceData_Plain(_In_ MESHFACE * pFace, _In_opt_ const nfChar * pszAdditionalString);
		__NMR_INLINE void writeFaceData_OneProperty(_In_ MESHFACE * pFace, _In_ const ModelResourceID nPropertyID, _In_ const ModelResourceIndex nPropertyIndex, _In_opt_ const nfChar * pszAdditionalString);
		__NMR_INLINE void writeFaceData_ThreeProperties(_In_ MESHFACE * pFace, _In_ const ModelResourceID nPropertyID, _In_ const ModelResourceIndex nPropertyIndex1, _In_ const ModelResourceIndex nPropertyIndex2, _In_ const ModelResourceIndex nPropertyIndex3, _In_opt_ const nfChar * pszAdditionalString);
		__NMR_INLINE void writeBeamData(_In_ MESHBEAM * pBeam, _In_ nfDouble dRadius, _In_ eModelBeamLatticeCapMode eDefaultCapMode);
		__NMR_INLINE void writeBallData(_In_ MESHBALL * pBall, _In_ eModelBeamLatticeBallMode eBallMode, _In_ nfDouble dRadius);
		__NMR_INLINE void writeRefData(_In_ INT nRefID);
		__NMR_INLINE void writeBallRefData(_In_ INT nRefID);
	public:
		CModelWriterNode100_Mesh() = delete;
		CModelWriterNode100_Mesh(_In_ CModelMeshObject * pModelMeshObject, _In_ CXmlWriter * pXMLWriter, _In_ PProgressMonitor pProgressMonitor,
			_In_ PMeshInformation_PropertyIndexMapping pPropertyIndexMapping, _In_ int nPosAfterDecPoint, _In_ nfBool bWriteMaterialExtension, _In_ nfBool m_bWriteBeamLatticeExtension);
		virtual void writeToXML();
	};

}

#endif // __NMR_MODELWRITERNODE100_MESH
