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

// Include custom headers here.
#include "lib3mf_object.hpp"

using namespace Lib3MF;

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
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFBuildItem::SetUUID (const std::string & sUUID)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CLib3MFBuildItem::GetObjectResourceID ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CLib3MFBuildItem::HasObjectTransform ()
{
	return buildItem().hasTransform();
}

NMR::NMATRIX3 TransformToMatrix(const sLib3MFTransform Transform)
{
	NMR::NMATRIX3 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
			matrix.m_fields[i][j] = Transform.m_Fields[i][j];
		matrix.m_fields[i][3] = 0 + 1.0f*(i == 3);
	}
	return matrix;
}

sLib3MFTransform MatrixToTransform(const NMR::NMATRIX3 matrix)
{
	sLib3MFTransform transform;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
			transform.m_Fields[i][j] = matrix.m_fields[i][j];
	}
	return transform;
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

bool CLib3MFBuildItem::HasMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILib3MFMetaDataGroup * CLib3MFBuildItem::GetMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILib3MFMetaDataGroup * CLib3MFBuildItem::CreateMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFBuildItem::RemoveMetaDataGroup ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

