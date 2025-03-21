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

--*/

#include "Model/Classes/NMR_ModelImplicitNode.h"

#include "Common/NMR_Exception.h"
#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "Model/Classes/NMR_ModelImplicitFunction.h"
#include "Model/Classes/NMR_ModelImplicitNode.h"

#include <memory>

namespace NMR
{

    CModelImplicitNode::CModelImplicitNode(Lib3MF::eImplicitNodeType type,
                                           ImplicitIdentifier const & identifier,
                                           std::string const & displayname,
                                           std::string const & tag,
                                           CModelImplicitFunction * parent)
        : m_type(type)
        , m_identifier(identifier)
        , m_displayname(displayname)
        , m_tag(tag)
        , m_parent(parent)
    {
        m_outputs = std::make_shared<Ports>();
        m_inputs = std::make_shared<Ports>();
    }

    CModelImplicitNode::CModelImplicitNode(Lib3MF::eImplicitNodeType type,
                                           CModelImplicitFunction * parent)
        : m_type(type)
        , m_parent(parent)
    {
        m_outputs = std::make_shared<Ports>();
        m_inputs = std::make_shared<Ports>();
    }

    ImplicitIdentifier const & CModelImplicitNode::getIdentifier() const
    {
        return m_identifier;
    }

    std::string const & CModelImplicitNode::getDisplayName() const
    {
        return m_displayname;
    }

    void CModelImplicitNode::setIdentifier(ImplicitIdentifier const & identifier)
    {
        m_identifier = identifier;
    }

    void CModelImplicitNode::setDisplayName(std::string const & displayname)
    {
        m_displayname = displayname;
    }

    void NMR::CModelImplicitNode::setType(Lib3MF::eImplicitNodeType type)
    {
        m_type = type;
    }

    void CModelImplicitNode::setTag(std::string const & tag)
    {
        m_tag = tag;
    }

    std::string const & CModelImplicitNode::getTag() const
    {
        return m_tag;
    }

    Lib3MF::eImplicitNodeType CModelImplicitNode::getNodeType() const
    {
        return m_type;
    }

    PModelImplicitPort CModelImplicitNode::addInput(const std::string & sIdentifier,
                                                    const std::string & sDisplayName)
    {
        auto newPort = std::make_shared<CModelImplicitPort>(this, sIdentifier, sDisplayName);
        m_inputs->push_back(newPort);
        return newPort;
    }

    PModelImplicitPort CModelImplicitNode::addOutput(const std::string & sIdentifier,
                                                     const std::string & sDisplayName)
    {
        auto newPort = std::make_shared<CModelImplicitPort>(this, sIdentifier, sDisplayName);
        m_outputs->push_back(newPort);
        return newPort;
    }

    PPorts NMR::CModelImplicitNode::getInputs() const
    {
        return m_inputs;
    }

    PPorts CModelImplicitNode::getOutputs() const
    {
        return m_outputs;
    }

    PModelImplicitPort NMR::CModelImplicitNode::findInput(const std::string & sIdentifier) const
    {
        for (auto & port : *m_inputs)
        {
            if (port->getIdentifier() == sIdentifier)
            {
                return port;
            }
        }
        return {};
    }

    PModelImplicitPort CModelImplicitNode::findOutput(const std::string & sIdentifier) const
    {
        for (auto & port : *m_outputs)
        {
            if (port->getIdentifier() == sIdentifier)
            {
                return port;
            }
        }
        return {};
    }

    void NMR::CModelImplicitNode::setConstant(double value)
    {
        if (m_type != Lib3MF::eImplicitNodeType::Constant)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        m_constant = value;
    }
    double CModelImplicitNode::getConstant() const
    {
        if (m_type != Lib3MF::eImplicitNodeType::Constant)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        return m_constant;
    }

    void NMR::CModelImplicitNode::setVector(const Lib3MF::sVector & value)
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstVec)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        m_vector = std::unique_ptr<Lib3MF::sVector>(new Lib3MF::sVector(value));
    }

    Lib3MF::sVector CModelImplicitNode::getVector() const
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstVec)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        if (!m_vector)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        return *m_vector;
    }

    void CModelImplicitNode::setMatrix(const Lib3MF::sMatrix4x4 & value)
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstMat)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        m_matrix = std::unique_ptr<Lib3MF::sMatrix4x4>(new Lib3MF::sMatrix4x4(value));
    }

    Lib3MF::sMatrix4x4 CModelImplicitNode::getMatrix() const
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstMat)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        if (!m_matrix)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        return *m_matrix;
    }

    void CModelImplicitNode::setModelResourceID(ModelResourceID resourceID)
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstResourceID)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        m_modelResourceID = resourceID;
    }

    ModelResourceID CModelImplicitNode::getModelResourceID() const
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstResourceID)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        return m_modelResourceID;
    }

    PModelResource NMR::CModelImplicitNode::getResource() const
    {
        if (m_parent == nullptr)
        {
            return nullptr;
        }

        auto model = m_parent->getModel();

        if (model == nullptr)
        {
            return nullptr;
        }

        return model->findResource(model->currentPath(), m_modelResourceID);
    }

    bool CModelImplicitNode::arePortsValid() const
    {
        return m_parent->getNodeTypes().arePortsValidForNode(*this);
    }

    void CModelImplicitNode::setGraphID(GraphID id)
    {
        m_graphId = id;
    }

    GraphID CModelImplicitNode::getGraphID() const
    {
        return m_graphId;
    }

    CModelImplicitFunction * CModelImplicitNode::getParent() const
    {
        return m_parent;
    }

    void CModelImplicitNode::setParent(CModelImplicitFunction* parent)
    {
        m_parent = parent;
    }
} // namespace NMR
