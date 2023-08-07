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
Reader for FunctionFormImage3D

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_FunctionFromImage3D.h"

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelFunctionFromImage3D.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR
{
    NMR::CModelReaderNode_FunctionFromImage3D::CModelReaderNode_FunctionFromImage3D(
      CModel * pModel,

      PModelWarnings pWarnings)
        : CModelReaderNode(pWarnings)
        , m_pModel(pModel)
    {
        __NMRASSERT(pModel);
    }

    void CModelReaderNode_FunctionFromImage3D::parseXML(CXmlReader * pXMLReader)
    {
        __NMRASSERT(pXMLReader);

        // Parse name
        parseName(pXMLReader);

        // Parse Attributes
        parseAttributes(pXMLReader);

        // Create Implicit Function
        m_pFunctionFromImage3D = std::make_shared<CModelFunctionFromImage3D>(m_imageID, m_pModel);
        m_pFunctionFromImage3D->setDisplayName(m_displayName);

        // Parse Content
        parseContent(pXMLReader);

        m_pModel->addResource(m_pFunctionFromImage3D);
    }

    eModelTextureTileStyle fnStringToModelTextureTileStyle(const std::string & sValue)
    {
        if (sValue == XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_WRAP)
        {
            return eModelTextureTileStyle::MODELTEXTURETILESTYLE_WRAP;
        }
        else if (sValue == XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_MIRROR)
        {
            return eModelTextureTileStyle::MODELTEXTURETILESTYLE_MIRROR;
        }
        else if (sValue == XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_CLAMP)
        {
            return eModelTextureTileStyle::MODELTEXTURETILESTYLE_CLAMP;
        }
        else if (sValue == XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_NONE)
        {
            return eModelTextureTileStyle::MODELTEXTURETILESTYLE_NONE;
        }
        else
        {
            throw CNMRException(NMR_ERROR_INVALIDTILESTYLE);
        }
    }

    eModelTextureFilter fnStringToModelTextureFilter(const std::string & sValue)
    {
        if (sValue == XML_3MF_ATTRIBUTE_TEXTURE2D_FILTER_AUTO)
        {
            return eModelTextureFilter::MODELTEXTUREFILTER_AUTO;
        }
        else if (sValue == XML_3MF_ATTRIBUTE_TEXTURE2D_FILTER_LINEAR)
        {
            return eModelTextureFilter::MODELTEXTUREFILTER_LINEAR;
        }
        else if (sValue == XML_3MF_ATTRIBUTE_TEXTURE2D_FILTER_NEAREST)
        {
            return eModelTextureFilter::MODELTEXTUREFILTER_NEAREST;
        }
        else
        {
            throw CNMRException(NMR_ERROR_INVALIDFILTER);
        }
    }

    void CModelReaderNode_FunctionFromImage3D::OnAttribute(const nfChar * pAttributeName,
                                                           const nfChar * pAttributeValue)
    {

        if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_FUNTCTION_FROM_IMAGE3D_IMAGE3DID) == 0)
        {
            m_imageID = fnStringToUint32(pAttributeValue);
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_FUNCTION_DISPLAY_NAME) == 0)
        {
            m_displayName = std::string{pAttributeValue};
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_FUNTCTION_FROM_IMAGE3D_OFFSET) == 0)
        {
            m_offset = fnStringToDouble(pAttributeValue);
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_FUNTCTION_FROM_IMAGE3D_SCALE) == 0)
        {
            m_scale = fnStringToDouble(pAttributeValue);
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_FUNTCTION_FROM_IMAGE3D_TILESTYLEU) == 0)
        {
            m_tileStyleU = fnStringToModelTextureTileStyle(pAttributeValue);
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_FUNTCTION_FROM_IMAGE3D_TILESTYLEV) == 0)
        {
            m_tileStyleV = fnStringToModelTextureTileStyle(pAttributeValue);
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_FUNTCTION_FROM_IMAGE3D_TILESTYLEW) == 0)
        {
            m_tileStyleW = fnStringToModelTextureTileStyle(pAttributeValue);
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_FUNTCTION_FROM_IMAGE3D_FILTER) == 0)
        {
            m_filter = fnStringToModelTextureFilter(pAttributeValue);
        }
        else
        {
            m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE),
                                      mrwInvalidOptionalValue);
        }
    }
}
