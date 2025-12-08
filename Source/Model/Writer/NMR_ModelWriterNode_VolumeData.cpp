/*++

Copyright (C) 2024 3MF Consortium

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
Writer node for VolumeData resources

--*/
#include "Model/Writer/NMR_ModelWriterNode_VolumeData.h"
#include <sstream>

#include "Common/NMR_Exception.h"
#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "lib3mf_types.hpp"

namespace NMR
{
    CModelWriterNode_VolumeData::CModelWriterNode_VolumeData(
        CModel* pModel, CXmlWriter* pXMLWriter,
        PProgressMonitor pProgressMonitor)
        : CModelWriterNode_ModelBase(pModel, pXMLWriter, pProgressMonitor)
    {
    }

    void CModelWriterNode_VolumeData::writeVolumeDataResources()
    {
        nfUint32 const nCount = m_pModel->getVolumeDataCount();

        for(nfUint32 nIndex = 0; nIndex < nCount; nIndex++)
        {
            m_pProgressMonitor->IncrementProgress(1);
            CModelVolumeData * pVolumeData = m_pModel->getVolumeData(nIndex);
            if(!pVolumeData)
            {
                continue;
            }
            writeVolumeDataResource(*pVolumeData);
        }
    }

    void CModelWriterNode_VolumeData::writeToXML()
    {
        writeVolumeDataResources();   
    }

    void CModelWriterNode_VolumeData::writeVolumeDataResource(
        CModelVolumeData& volumeData)
    {
        writeStartElementWithPrefix(XML_3MF_ELEMENT_VOLUMEDATA,
                                    XML_3MF_NAMESPACEPREFIX_VOLUMETRIC);

        writeIntAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_ID, volumeData.getPackageResourceID()->getModelResourceID());
        
        if(volumeData.getPropertyCount())
        {
            nfUint32 count = volumeData.getPropertyCount();

            for(nfUint32 i = 0; i < count; i++)
            {
                PVolumeDataProperty pProperty = volumeData.getProperty(i);
                if(pProperty)
                {
                    writeProperty(*pProperty);
                }
            }
        }

        if(volumeData.hasColor())
        {
            PVolumeDataColor pColor = volumeData.getColor();
            if(pColor)
            {
                writeColor(*pColor);
            }
        }

        writeFullEndElement();
    }

    void CModelWriterNode_VolumeData::writeProperty(
        CVolumeDataProperty& property)
    {
        writeStartElementWithPrefix(XML_3MF_ELEMENT_VOLUMETRIC_PROPERTY,
                                    XML_3MF_NAMESPACEPREFIX_VOLUMETRIC);
        writeStringAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_PROPERTY,
                             property.getName());
        if(!fnMATRIX3_isIdentity(property.getTransform()))
        {
            writeStringAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_TRANSFORM,
                                 fnMATRIX3_toString(property.getTransform()));
        }
        PPackageResourceID pID =
            m_pModel->findPackageResourceID(property.getFunctionResourceID());
        if(!pID) throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
        writeIntAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_FUNCTIONID,
                          pID->getModelResourceID());
        if(!property.isRequired())
            writeStringAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_PROPERTY_REQUIRED,
                                 "false");

        if(!property.getChannelName().empty())
        {
            writeStringAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_CHANNEL,
                                 property.getChannelName());
        }

        if(property.getMinFeatureSize() != 0.0)
        {
            writeFloatAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_MINFEATURESIZE,
                                float(property.getMinFeatureSize()));
        }

        if(property.getFallBackValue() != 0.0)
        {
            writeFloatAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_FALLBACKVALUE,
                                float(property.getFallBackValue()));
        }

        writeEndElement();
    }

    void CModelWriterNode_VolumeData::writeColor(CVolumeDataColor& color)
    {
        writeStartElementWithPrefix(XML_3MF_ELEMENT_VOLUMETRIC_COLOR,
                                    XML_3MF_NAMESPACEPREFIX_VOLUMETRIC);

        PPackageResourceID pID =
            m_pModel->findPackageResourceID(color.getFunctionResourceID());
        if(!pID) throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
        writeIntAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_FUNCTIONID,
                          pID->getModelResourceID());

        if(!fnMATRIX3_isIdentity(color.getTransform()))
        {
            writeStringAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_TRANSFORM,
                                 fnMATRIX3_toString(color.getTransform()));
        }

        if(!color.getChannelName().empty())
        {
            writeStringAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_CHANNEL,
                                 color.getChannelName());
        }

        if(color.getMinFeatureSize() != 0.0)
        {
            writeFloatAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_MINFEATURESIZE,
                                float(color.getMinFeatureSize()));
        }

        if(color.getFallBackValue() != 0.0)
        {
            writeFloatAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_FALLBACKVALUE,
                                float(color.getFallBackValue()));
        }

        writeEndElement();
    }
}  // namespace NMR
