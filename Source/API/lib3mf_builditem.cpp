/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CBuildItem

*/

#include "lib3mf_builditem.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_utils.hpp"
#include "lib3mf_object.hpp"
#include "lib3mf_metadatagroup.hpp"
// Include custom headers here.

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CBuildItem 
**************************************************************************************************************************/

NMR::CModelBuildItem& CBuildItem::buildItem()
{
	return *m_pBuildItem.get();
}

CBuildItem::CBuildItem(NMR::PModelBuildItem pBuildItem)
{
	m_pBuildItem = pBuildItem;
}

Lib3MF_uint32 CBuildItem::GetHandle()
{
	return buildItem().getHandle();
}

IObject * CBuildItem::GetObjectResource ()
{
	NMR::PModelResource pResource = buildItem().getModel()->findResource(buildItem().getObject()->getPackageResourceID());
	if (!pResource.get())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMODELRESOURCE);
	
	std::unique_ptr<IObject> pResourceInterface(CObject::fnCreateObjectFromModelResource(pResource, true));
	if (pResourceInterface == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	return pResourceInterface.release();
}

std::string CBuildItem::GetUUID (bool & bHasUUID)
{
	bHasUUID = buildItem().uuid() != nullptr;
	if (bHasUUID)
		return buildItem().uuid()->toString();
	return "";
}

void CBuildItem::SetUUID (const std::string & sUUID)
{
	NMR::PUUID pUUID = std::make_shared<NMR::CUUID>(sUUID);
	buildItem().setUUID(pUUID);
}

Lib3MF_uint32 CBuildItem::GetObjectResourceID()
{
	return buildItem().getObject()->getPackageResourceID()->getUniqueID();
}

bool CBuildItem::HasObjectTransform ()
{
	return buildItem().hasTransform();
}

sLib3MFTransform CBuildItem::GetObjectTransform ()
{
	const NMR::NMATRIX3 matrix = buildItem().getTransform();
	return MatrixToTransform(matrix);
}

void CBuildItem::SetObjectTransform (const sLib3MFTransform Transform)
{
	buildItem().setTransform(TransformToMatrix(Transform));
}

std::string CBuildItem::GetPartNumber ()
{
	return buildItem().getPartNumber();
}

void CBuildItem::SetPartNumber (const std::string & sSetPartnumber)
{
	buildItem().setPartNumber(sSetPartnumber);
}

IMetaDataGroup * CBuildItem::GetMetaDataGroup ()
{
	return new CMetaDataGroup(buildItem().metaDataGroup());
}

Lib3MF::sBox CBuildItem::GetOutbox()
{
	NMR::NOUTBOX3 sOutbox;
	NMR::fnOutboxInitialize(sOutbox);

	buildItem().getObject()->extendOutbox(sOutbox, buildItem().getTransform());

	sBox s;
	s.m_MinCoordinate[0] = sOutbox.m_min.m_fields[0];
	s.m_MinCoordinate[1] = sOutbox.m_min.m_fields[1];
	s.m_MinCoordinate[2] = sOutbox.m_min.m_fields[2];

	s.m_MaxCoordinate[0] = sOutbox.m_max.m_fields[0];
	s.m_MaxCoordinate[1] = sOutbox.m_max.m_fields[1];
	s.m_MaxCoordinate[2] = sOutbox.m_max.m_fields[2];
	return s;
}
