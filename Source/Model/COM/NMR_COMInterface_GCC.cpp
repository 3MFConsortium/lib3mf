/*++

Copyright (C) 2015 netfabb GmbH

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

COM Interface Implementation for GCC

--*/

#include "Model/COM/NMR_COMInterfaces.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

#ifdef __GNUC__

namespace NMR {

/*------------------------------------------------------------------*/

    ILib3MFBase::ILib3MFBase ()
    {
    }

    ILib3MFBase::~ILib3MFBase ()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelWriter::ILib3MFModelWriter()
    {
    }

    ILib3MFModelWriter::~ILib3MFModelWriter()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelReader::ILib3MFModelReader()
    {
    }

    ILib3MFModelReader::~ILib3MFModelReader()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelResource::ILib3MFModelResource()
    {
    }

    ILib3MFModelResource::~ILib3MFModelResource()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelResourceIterator::ILib3MFModelResourceIterator()
    {
    }

    ILib3MFModelResourceIterator::~ILib3MFModelResourceIterator()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelThumbnail::ILib3MFModelThumbnail()
    {
    }

    ILib3MFModelThumbnail::~ILib3MFModelThumbnail()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelThumbnailIterator::ILib3MFModelThumbnailIterator()
    {
    }

    ILib3MFModelThumbnailIterator::~ILib3MFModelThumbnailIterator()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFPropertyHandler::ILib3MFPropertyHandler()
    {
    }

    ILib3MFPropertyHandler::~ILib3MFPropertyHandler()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFDefaultPropertyHandler::ILib3MFDefaultPropertyHandler()
    {
    }

    ILib3MFDefaultPropertyHandler::~ILib3MFDefaultPropertyHandler()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelObjectResource::ILib3MFModelObjectResource()
    {
    }

    ILib3MFModelObjectResource::~ILib3MFModelObjectResource()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelBaseMaterial::ILib3MFModelBaseMaterial()
    {
    }

    ILib3MFModelBaseMaterial::~ILib3MFModelBaseMaterial()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelTexture2D::ILib3MFModelTexture2D()
    {
    }

    ILib3MFModelTexture2D::~ILib3MFModelTexture2D()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelMeshObject::ILib3MFModelMeshObject()
    {
    }

    ILib3MFModelMeshObject::~ILib3MFModelMeshObject()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelComponent::ILib3MFModelComponent()
    {
    }

    ILib3MFModelComponent::~ILib3MFModelComponent()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelComponentsObject::ILib3MFModelComponentsObject()
    {
    }

    ILib3MFModelComponentsObject::~ILib3MFModelComponentsObject()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelBuildItem::ILib3MFModelBuildItem()
    {
    }


    ILib3MFModelBuildItem::~ILib3MFModelBuildItem()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelBuildItemIterator::ILib3MFModelBuildItemIterator()
    {
    }

    ILib3MFModelBuildItemIterator::~ILib3MFModelBuildItemIterator()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModel::ILib3MFModel()
    {
    }

    ILib3MFModel::~ILib3MFModel()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelFactory::ILib3MFModelFactory()
    {
    }

    ILib3MFModelFactory::~ILib3MFModelFactory()
    {
    }

/*------------------------------------------------------------------*/

    ILib3MFModelAttachment::ILib3MFModelAttachment()
    {
    }

    ILib3MFModelAttachment::~ILib3MFModelAttachment()
    {
    }

}

#endif // __GNUC__



