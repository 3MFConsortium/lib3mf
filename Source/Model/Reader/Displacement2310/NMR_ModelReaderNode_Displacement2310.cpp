/*++

Copyright (C) 2026 3MF Consortium

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

In-memory representation and serialization support for the 3MF Displacement extension.

--*/

#include "Model/Reader/Displacement2310/NMR_ModelReaderNode_Displacement2310.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include <cmath>

namespace NMR {

	class CDisplacementItemNode : public CModelReaderNode {
	public:
		enum class Kind { Normal, Coordinate };
		Kind kind;
		nfDouble x = 0, y = 0, z = 0, u = 0, v = 0, factor = 1;
		nfInt32 normal = -1;
		bool hasX = false, hasY = false, hasZ = false, hasU = false, hasV = false, hasNormal = false;
		std::set<std::string> attributes;
		CDisplacementItemNode(PModelWarnings warnings, Kind itemKind) : CModelReaderNode(warnings), kind(itemKind) {}
		void parseXML(CXmlReader * reader) override { parseName(reader); parseAttributes(reader); parseContent(reader); }
		void OnAttribute(const nfChar * name, const nfChar * value) override {
			if (!attributes.insert(name).second)
				throw CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE);
			if (kind == Kind::Normal) {
				if (!strcmp(name, "x")) { x = fnStringToDouble(value); hasX = true; }
				else if (!strcmp(name, "y")) { y = fnStringToDouble(value); hasY = true; }
				else if (!strcmp(name, "z")) { z = fnStringToDouble(value); hasZ = true; }
				else throw CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE);
			} else {
				if (!strcmp(name, "u")) { u = fnStringToDouble(value); hasU = true; }
				else if (!strcmp(name, "v")) { v = fnStringToDouble(value); hasV = true; }
				else if (!strcmp(name, XML_3MF_ATTRIBUTE_DISPLACEMENT_FACTOR)) factor = fnStringToDouble(value);
				else if (!strcmp(name, XML_3MF_ATTRIBUTE_DISPLACEMENT_NORMAL)) { normal = fnStringToInt32(value); hasNormal = true; }
				else throw CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE);
			}
		}
	};

	CModelReaderNode_Displacement2310_Resource::CModelReaderNode_Displacement2310_Resource(CModel * pModel, PModelWarnings warnings, eDisplacementResourceKind kind)
		: CModelReaderNode(warnings), m_pModel(pModel), m_Kind(kind), m_nID(0), m_nDispID(0), m_nNormID(0), m_dHeight(0), m_dOffset(0), m_bHasHeight(false) {}

	void CModelReaderNode_Displacement2310_Resource::OnAttribute(const nfChar * name, const nfChar * value) {
		if (!m_Attributes.insert(name).second)
			throw CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE);
		if (!strcmp(name, "id")) m_nID = fnStringToUint32(value);
		else if (m_Kind == eDisplacementResourceKind::Displacement2D && !strcmp(name, "path")) m_sPath = value;
		else if (m_Kind == eDisplacementResourceKind::Displacement2D && !strcmp(name, "channel")) m_sChannel = value;
		else if (m_Kind == eDisplacementResourceKind::Displacement2D && !strcmp(name, "tilestyleu")) m_sTileStyleU = value;
		else if (m_Kind == eDisplacementResourceKind::Displacement2D && !strcmp(name, "tilestylev")) m_sTileStyleV = value;
		else if (m_Kind == eDisplacementResourceKind::Displacement2D && !strcmp(name, "filter")) m_sFilter = value;
		else if (m_Kind == eDisplacementResourceKind::Disp2DGroup && !strcmp(name, "dispid")) m_nDispID = fnStringToUint32(value);
		else if (m_Kind == eDisplacementResourceKind::Disp2DGroup && !strcmp(name, "nid")) m_nNormID = fnStringToUint32(value);
		else if (m_Kind == eDisplacementResourceKind::Disp2DGroup && !strcmp(name, "height")) { m_dHeight = fnStringToDouble(value); m_bHasHeight = true; }
		else if (m_Kind == eDisplacementResourceKind::Disp2DGroup && !strcmp(name, "offset")) m_dOffset = fnStringToDouble(value);
		else throw CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE);
	}

	void CModelReaderNode_Displacement2310_Resource::OnNSChildElement(const nfChar * name, const nfChar * ns, CXmlReader * reader) {
		if (strcmp(ns, XML_3MF_NAMESPACE_DISPLACEMENTSPEC)) { m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue); return; }
		if (m_Kind == eDisplacementResourceKind::NormVectorGroup && !strcmp(name, "normvector")) {
			auto node = std::make_shared<CDisplacementItemNode>(m_pWarnings, CDisplacementItemNode::Kind::Normal); node->parseXML(reader);
			if (!node->hasX || !node->hasY || !node->hasZ) throw CNMRException(NMR_ERROR_MODELCOORDINATEMISSING);
			m_pNormGroup->addVector({ node->x, node->y, node->z });
		} else if (m_Kind == eDisplacementResourceKind::Disp2DGroup && !strcmp(name, "disp2dcoord")) {
			auto node = std::make_shared<CDisplacementItemNode>(m_pWarnings, CDisplacementItemNode::Kind::Coordinate); node->parseXML(reader);
			if (!node->hasU || !node->hasV || !node->hasNormal || node->normal < 0) throw CNMRException(NMR_ERROR_INVALIDINDEX);
			m_pDispGroup->addCoordinate({ node->u, node->v, (nfUint32)node->normal, node->factor });
		} else m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_Displacement2310_Resource::parseXML(CXmlReader * reader) {
		parseName(reader); parseAttributes(reader);
		if (!m_nID) throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);
		if (m_Kind == eDisplacementResourceKind::Displacement2D) {
			auto attachment = m_pModel->findModelAttachment(m_sPath);
			auto resource = std::make_shared<CModelDisplacement2DResource>(m_nID, m_pModel, attachment);
			if (!m_sChannel.empty()) resource->setChannelString(m_sChannel);
			if (!m_sTileStyleU.empty()) resource->setTileStyleUString(m_sTileStyleU);
			if (!m_sTileStyleV.empty()) resource->setTileStyleVString(m_sTileStyleV);
			if (!m_sFilter.empty()) resource->setFilterString(m_sFilter);
			m_pModel->addResource(resource);
		} else if (m_Kind == eDisplacementResourceKind::NormVectorGroup) {
			m_pNormGroup = std::make_shared<CModelNormVectorGroupResource>(m_nID, m_pModel); m_pModel->addResource(m_pNormGroup);
		} else {
			if (!m_bHasHeight || !m_nDispID || !m_nNormID) throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
			auto displacement = std::dynamic_pointer_cast<CModelDisplacement2DResource>(m_pModel->findResource(m_pModel->currentPath(), m_nDispID));
			auto normals = std::dynamic_pointer_cast<CModelNormVectorGroupResource>(m_pModel->findResource(m_pModel->currentPath(), m_nNormID));
			if (!displacement || !normals) throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
			m_pDispGroup = std::make_shared<CModelDisp2DGroupResource>(m_nID, m_pModel, displacement, normals, m_dHeight, m_dOffset); m_pModel->addResource(m_pDispGroup);
		}
		parseContent(reader);
		if (m_Kind == eDisplacementResourceKind::NormVectorGroup && m_pNormGroup->getCount() == 0)
			throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
		if (m_Kind == eDisplacementResourceKind::Disp2DGroup && m_pDispGroup->getCount() == 0)
			throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
	}
}
