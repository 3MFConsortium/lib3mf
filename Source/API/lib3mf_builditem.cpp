/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CLib3MFBuildItem

*/

#include "lib3mf_builditem.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_utils.hpp"
#include "lib3mf_object.hpp"
#include "lib3mf_metadatagroup.hpp"
// Include custom headers here.

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFBuildItem 
**************************************************************************************************************************/

NMR::CModelBuildItem& CLib3MFBuildItem::buildItem()
{
	return *m_pBuildItem.get();
}

CLib3MFBuildItem::CLib3MFBuildItem(NMR::PModelBuildItem pBuildItem)
{
	m_pBuildItem = pBuildItem;
}

Lib3MF_uint32 CLib3MFBuildItem::GetHandle()
{
	return buildItem().getHandle();
}

ILib3MFObject * CLib3MFBuildItem::GetObjectResource ()
{
	NMR::PModelResource pResource = buildItem().getModel()->findResource(buildItem().getObjectID());
	if (!pResource.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMODELRESOURCE);
	
	std::unique_ptr<ILib3MFObject> pResourceInterface(CLib3MFObject::fnCreateObjectFromModelResource(pResource, true));
	if (pResourceInterface == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	return pResourceInterface.release();
}

std::string CLib3MFBuildItem::GetUUID (bool & bHasUUID)
{
	bHasUUID = buildItem().uuid() != nullptr;
	if (bHasUUID)
		return buildItem().uuid()->toString();
	return "";
}

void CLib3MFBuildItem::SetUUID (const std::string & sUUID)
{
	NMR::PUUID pUUID = std::make_shared<NMR::CUUID>(sUUID);
	buildItem().setUUID(pUUID);
}

Lib3MF_uint32 CLib3MFBuildItem::GetObjectResourceID ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CLib3MFBuildItem::HasObjectTransform ()
{
	return buildItem().hasTransform();
}

sLib3MFTransform CLib3MFBuildItem::GetObjectTransform ()
{
	const NMR::NMATRIX3 matrix = buildItem().getTransform();
	return MatrixToTransform(matrix);
}

void CLib3MFBuildItem::SetObjectTransform (const sLib3MFTransform Transform)
{
	buildItem().setTransform(TransformToMatrix(Transform));
}

std::string CLib3MFBuildItem::GetPartNumber ()
{
	return buildItem().getPartNumber();
}

void CLib3MFBuildItem::SetPartNumber (const std::string & sSetPartnumber)
{
	buildItem().setPartNumber(sSetPartnumber);
}

ILib3MFMetaDataGroup * CLib3MFBuildItem::GetMetaDataGroup ()
{
	return new CLib3MFMetaDataGroup(buildItem().metaDataGroup());
}

