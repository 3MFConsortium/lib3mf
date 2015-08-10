/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
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

Abstract: COM Interface Exports for plain C DLLs

--*/

#include "Model/COM/NMR_DLLInterfaces.h"
#include "Model/COM/NMR_COMInterfaces.h"
#include "Model/COM/NMR_COMInterface_Model.h"

namespace NMR {

extern "C" {

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_getspecversion (_Out_ DWORD * pMajorVersion, _Out_ DWORD * pMinorVersion)
	{
	    if (!pMajorVersion)
            return LIB3MF_POINTER;
	    if (!pMinorVersion)
            return LIB3MF_POINTER;

        *pMajorVersion = NMR_APIVERSION_MAJOR;
        *pMinorVersion = NMR_APIVERSION_MINOR;

	    return LIB3MF_OK;
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_getinterfaceversion (_Out_ DWORD * pInterfaceVersion)
	{
	    if (!pInterfaceVersion)
            return LIB3MF_POINTER;

        *pInterfaceVersion = NMR_APIVERSION_INTERFACE;

	    return LIB3MF_OK;
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_createmodel(_Outptr_ PLib3MFModel ** ppModel)
	{
		if (ppModel == nullptr)
			return LIB3MF_POINTER;

		try {
		    #ifndef __GCC
			HRESULT hResult = CoInitialize(NULL);
			if (hResult != LIB3MF_OK)
				return hResult;
            #endif

			ILib3MFModel * pNewModel = new CCOMObject<CCOMModel>();
			*ppModel = (PLib3MFModel *) pNewModel;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
    }

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_release(_In_ PLib3MFBase * pInstance)
	{
	    if (pInstance) {
            ILib3MFBase * pBaseInstance = (ILib3MFBase *) pInstance;
            #ifndef __GCC
            pBaseInstance->Release ();
            #else
            delete pBaseInstance;
            #endif
	    }

	    return LIB3MF_OK;
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_getlasterror(_In_ PLib3MFBase * pInstance, _Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
	{
		if (!pInstance)
			return LIB3MF_POINTER;

		return ((ILib3MFBase *)pInstance)->GetLastError(pErrorCode, pErrorMessage);
	}


	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_writer_writetofile (_In_ PLib3MFModelWriter * pWriter, _In_z_ LPCWSTR pwszFilename)
	{
	    if (!pWriter)
            return LIB3MF_POINTER;

        return ((ILib3MFModelWriter *) pWriter)->WriteToFile (pwszFilename);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_readfromfile (_In_ PLib3MFModelReader * pReader, _In_z_ LPCWSTR pwszFilename)
	{
	    if (!pReader)
            return LIB3MF_POINTER;

        return ((ILib3MFModelReader *) pReader)->ReadFromFile (pwszFilename);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_getwarningcount(_In_ PLib3MFModelReader * pReader, _Out_ DWORD * pnWarningCount)
	{
	    if (!pReader)
            return LIB3MF_POINTER;

        return ((ILib3MFModelReader *) pReader)->GetWarningCount (pnWarningCount);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_getwarning(_In_ PLib3MFModelReader * pReader, _In_ DWORD nIndex, _Out_ DWORD * pErrorCode, _Out_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
	    if (!pReader)
            return LIB3MF_POINTER;

        return ((ILib3MFModelReader *) pReader)->GetWarning (nIndex, pErrorCode, pwszBuffer, cbBufferSize, pcbNeededChars);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resource_getresourceid (_In_ PLib3MFModelResource * pResource, _Out_ DWORD * pnResourceID)
	{
	    if (!pResource)
            return LIB3MF_POINTER;

        return ((ILib3MFModelResource *) pResource)->GetResourceID (pnResourceID);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resourceiterator_movenext(_In_ PLib3MFModelResourceIterator * pIterator, _Out_ BOOL * pbHasNext)
	{
	    if (!pIterator)
            return LIB3MF_POINTER;

        return ((ILib3MFModelResourceIterator *) pIterator)->MoveNext (pbHasNext);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resourceiterator_moveprevious(_In_ PLib3MFModelResourceIterator * pIterator, _Out_ BOOL * pbHasPrevious)
	{
	    if (!pIterator)
            return LIB3MF_POINTER;

        return ((ILib3MFModelResourceIterator *) pIterator)->MovePrevious (pbHasPrevious);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resourceiterator_getcurrent(_In_ PLib3MFModelResourceIterator * pIterator, _Outptr_ PLib3MFModelResource ** ppResultResource)
	{
	    if (!pIterator)
            return LIB3MF_POINTER;

        return ((ILib3MFModelResourceIterator *) pIterator)->GetCurrent ((ILib3MFModelResource **) ppResultResource);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resourceiterator_clone(_In_ PLib3MFModelResourceIterator * pIterator, _Outptr_ PLib3MFModelResourceIterator ** ppIterator)
	{
	    if (!pIterator)
            return LIB3MF_POINTER;

        return ((ILib3MFModelResourceIterator *) pIterator)->Clone ((ILib3MFModelResourceIterator **) ppIterator);
	}


	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_ismeshobject(_In_ PLib3MFModelObjectResource * pObject, _Out_ BOOL * pbIsMeshObject)
	{
		if (!pObject)
			return LIB3MF_POINTER;

		return ((ILib3MFModelObjectResource *)pObject)->IsMeshObject(pbIsMeshObject);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_iscomponentsobject(_In_ PLib3MFModelObjectResource * pObject, _Out_ BOOL * pbIsComponentObject)
	{
		if (!pObject)
			return LIB3MF_POINTER;

		return ((ILib3MFModelObjectResource *)pObject)->IsComponentsObject(pbIsComponentObject);

	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_gettype(_In_ PLib3MFModelObjectResource * pObject, _Out_ DWORD * pObjectType)
	{
	    if (!pObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelObjectResource *) pObject)->GetType (pObjectType);
	}

    LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_settype(_In_ PLib3MFModelObjectResource * pObject, _In_ DWORD ObjectType)
	{
	    if (!pObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelObjectResource *) pObject)->SetType (ObjectType);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_getname(_In_ PLib3MFModelObjectResource * pObject, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
	    if (!pObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelObjectResource *) pObject)->GetName (pwszBuffer, cbBufferSize, pcbNeededChars);
	}

    LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setname(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCWSTR pwszPartNumber)
	{
	    if (!pObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelObjectResource *) pObject)->SetName (pwszPartNumber);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_getpartnumber(_In_ PLib3MFModelObjectResource * pObject, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
	    if (!pObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelObjectResource *) pObject)->GetPartNumber (pwszBuffer, cbBufferSize, pcbNeededChars);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setpartnumber(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCWSTR pwszPartNumber)
	{
	    if (!pObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelObjectResource *) pObject)->SetPartNumber (pwszPartNumber);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_isvalidobject(_In_ PLib3MFModelObjectResource * pObject, _Out_ BOOL * pbIsValid)
	{
		if (!pObject)
			return LIB3MF_POINTER;

		return ((ILib3MFModelObjectResource *)pObject)->IsValidObject(pbIsValid);

	}


	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_createdefaultpropertyhandler(_In_ PLib3MFModelObjectResource * pObject, _Out_ PLib3MFDefaultPropertyHandler ** ppPropertyHandler)
	{
		if (!pObject)
			return LIB3MF_POINTER;

		return ((ILib3MFModelObjectResource *)pObject)->CreateDefaultPropertyHandler((ILib3MFDefaultPropertyHandler**)ppPropertyHandler);
	}



	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_createdefaultmultipropertyhandler(_In_ PLib3MFModelObjectResource * pObject, _In_ DWORD nChannel, _Out_ PLib3MFDefaultPropertyHandler ** ppPropertyHandler)
	{
		if (!pObject)
			return LIB3MF_POINTER;

		return ((ILib3MFModelObjectResource *)pObject)->CreateDefaultMultiPropertyHandler(nChannel, (ILib3MFDefaultPropertyHandler**)ppPropertyHandler);
	}



	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getvertexcount(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ DWORD * pnVertexCount)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->GetVertexCount (pnVertexCount);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_gettrianglecount(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ DWORD * pnTriangleCount)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->GetTriangleCount (pnTriangleCount);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getvertex(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _Out_ MODELMESHVERTEX * pVertex)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->GetVertex (nIndex, pVertex);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setvertex(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _In_ MODELMESHVERTEX * pVertex)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->SetVertex (nIndex, pVertex);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_addvertex(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ MODELMESHVERTEX * pVertex, _Out_opt_ DWORD * pnIndex)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->AddVertex (pVertex, pnIndex);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_gettriangle (_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _Out_ MODELMESHTRIANGLE * pTriangle)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->GetTriangle (nIndex, pTriangle);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_settriangle(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _In_ MODELMESHTRIANGLE * pTriangle)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->SetTriangle (nIndex, pTriangle);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_addtriangle(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ MODELMESHTRIANGLE * pTriangle, _Out_opt_ DWORD * pnIndex)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->AddTriangle (pTriangle, pnIndex);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getvertices(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ MODELMESHVERTEX * pVertices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnVertexCount)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->GetVertices (pVertices, nBufferSize, pnVertexCount);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_gettriangleindices(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ MODELMESHTRIANGLE * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnTriangleCount)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->GetTriangleIndices (pIndices, nBufferSize, pnTriangleCount);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setgeometry(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ MODELMESHVERTEX * pVertices, _In_ DWORD nVertexCount, _In_ MODELMESHTRIANGLE * pTriangles, _In_ DWORD nTriangleCount)
	{
	    if (!pMeshObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelMeshObject *) pMeshObject)->SetGeometry (pVertices, nVertexCount, pTriangles, nTriangleCount);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_createpropertyhandler(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ PLib3MFPropertyHandler ** ppPropertyHandler)
	{
		if (!pMeshObject)
			return LIB3MF_POINTER;

		return ((ILib3MFModelMeshObject *)pMeshObject)->CreatePropertyHandler((ILib3MFPropertyHandler **) ppPropertyHandler);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_createmultipropertyhandler(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nChannel, _In_ PLib3MFPropertyHandler ** ppPropertyHandler)
	{
		if (!pMeshObject)
			return LIB3MF_POINTER;

		return ((ILib3MFModelMeshObject *)pMeshObject)->CreateMultiPropertyHandler(nChannel, (ILib3MFPropertyHandler **)ppPropertyHandler);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_ismanifoldandoriented(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ BOOL * pbIsOrientedAndManifold)
	{
		if (!pMeshObject)
			return LIB3MF_POINTER;

		return ((ILib3MFModelMeshObject *)pMeshObject)->IsManifoldAndOriented(pbIsOrientedAndManifold);
	}


	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_getobjectresource(_In_ PLib3MFModelComponent * pComponent, _Outptr_ PLib3MFModelObjectResource ** ppObjectResource)
	{
	    if (!pComponent)
            return LIB3MF_POINTER;

        return ((ILib3MFModelComponent *) pComponent)->GetObjectResource ((ILib3MFModelObjectResource**) ppObjectResource);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_gettransform(_In_ PLib3MFModelComponent * pComponent, _Out_ MODELTRANSFORM * pTransformation)
	{
	    if (!pComponent)
            return LIB3MF_POINTER;

        return ((ILib3MFModelComponent *) pComponent)->GetTransform (pTransformation);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_settransform(_In_ PLib3MFModelComponent * pComponent, _In_ MODELTRANSFORM * pTransformation)
	{
	    if (!pComponent)
            return LIB3MF_POINTER;

        return ((ILib3MFModelComponent *) pComponent)->SetTransform (pTransformation);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_getobjectresourceid(_In_ PLib3MFModelComponent * pComponent, _Out_ DWORD * pnResourceID)
	{
	    if (!pComponent)
            return LIB3MF_POINTER;

        return ((ILib3MFModelComponent *) pComponent)->GetObjectResourceID (pnResourceID);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_hastransform(_In_ PLib3MFModelComponent * pComponent, _Out_ BOOL * pbHasTransform)
	{
	    if (!pComponent)
            return LIB3MF_POINTER;

        return ((ILib3MFModelComponent *) pComponent)->HasTransform (pbHasTransform);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_addcomponent(_In_ PLib3MFModelComponentsObject * pComponentsObject, _In_ PLib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pmTransform, _Outptr_ PLib3MFModelComponent ** ppComponent)
	{
	    if (!pComponentsObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelComponentsObject *) pComponentsObject)->AddComponent ((ILib3MFModelObjectResource*) pObject, pmTransform, (ILib3MFModelComponent**) ppComponent);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_getcomponent(_In_ PLib3MFModelComponentsObject * pComponentsObject, _In_ DWORD nIndex, _Outptr_ PLib3MFModelComponent ** ppComponent)
	{
	    if (!pComponentsObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelComponentsObject *) pComponentsObject)->GetComponent (nIndex, (ILib3MFModelComponent**) ppComponent);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_getcomponentcount(_In_ PLib3MFModelComponentsObject * pComponentsObject, _Out_ DWORD * pComponentCount)
	{
	    if (!pComponentsObject)
            return LIB3MF_POINTER;

        return ((ILib3MFModelComponentsObject *) pComponentsObject)->GetComponentCount (pComponentCount);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getobjectresource(_In_ PLib3MFModelBuildItem * pBuildItem, _Outptr_ PLib3MFModelObjectResource ** ppObject)
	{
	    if (!pBuildItem)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItem *) pBuildItem)->GetObjectResource ((ILib3MFModelObjectResource **) ppObject);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getobjectresourceid(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_ DWORD * pnID)
	{
	    if (!pBuildItem)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItem *) pBuildItem)->GetObjectResourceID (pnID);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_hasobjecttransform(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_ BOOL * pbHasTransform)
	{
	    if (!pBuildItem)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItem *) pBuildItem)->HasObjectTransform (pbHasTransform);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getobjecttransform(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_ MODELTRANSFORM * pmTransform)
	{
	    if (!pBuildItem)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItem *) pBuildItem)->GetObjectTransform (pmTransform);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_setobjecttransform(_In_ PLib3MFModelBuildItem * pBuildItem, _In_ MODELTRANSFORM * pmTransform)
	{
	    if (!pBuildItem)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItem *) pBuildItem)->SetObjectTransform (pmTransform);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getpartnumber(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
	    if (!pBuildItem)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItem *) pBuildItem)->GetPartNumber (pwszBuffer, cbBufferSize, pcbNeededChars);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_setpartnumber(_In_ PLib3MFModelBuildItem * pBuildItem, _In_z_ LPCWSTR pwszPartNumber)
	{
	    if (!pBuildItem)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItem *) pBuildItem)->SetPartNumber (pwszPartNumber);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_gethandle(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_ DWORD * pHandle)
	{
		if (!pBuildItem)
			return LIB3MF_POINTER;

		return ((ILib3MFModelBuildItem *)pBuildItem)->GetHandle(pHandle);
	}


	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditemiterator_movenext(_In_ PLib3MFModelBuildItemIterator * pIterator, _Out_ BOOL * pbHasNext)
	{
	    if (!pIterator)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItemIterator *) pIterator)->MoveNext (pbHasNext);
	}


	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditemiterator_moveprevious(_In_ PLib3MFModelBuildItemIterator * pIterator, _Out_ BOOL * pbHasPrevious)
	{
	    if (!pIterator)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItemIterator *) pIterator)->MovePrevious (pbHasPrevious);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditemiterator_getcurrent(_In_ PLib3MFModelBuildItemIterator * pIterator, _Outptr_ PLib3MFModelBuildItem ** ppResultBuildItem)
	{
	    if (!pIterator)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItemIterator *) pIterator)->GetCurrent ((ILib3MFModelBuildItem **) ppResultBuildItem);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditemiterator_clone(_In_ PLib3MFModelBuildItemIterator * pIterator, _Outptr_ PLib3MFModelBuildItemIterator ** ppIterator)
	{
	    if (!pIterator)
            return LIB3MF_POINTER;

        return ((ILib3MFModelBuildItemIterator *) pIterator)->Clone ((ILib3MFModelBuildItemIterator **) ppIterator);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_setunit(_In_ PLib3MFModel * pModel, _In_ DWORD Unit)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->SetUnit (Unit);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getunit(_In_ PLib3MFModel * pModel, _Out_ DWORD * pUnit)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->GetUnit (pUnit);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_setlanguage(_In_ PLib3MFModel * pModel, _In_z_ LPCWSTR pwszLanguage)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->SetLanguage (pwszLanguage);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getlanguage(_In_ PLib3MFModel * pModel, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->GetLanguage (pwszBuffer, cbBufferSize, pcbNeededChars);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_querywriter(_In_ PLib3MFModel * pModel, _In_z_ LPCWSTR pwszWriterClass, _Outptr_ PLib3MFModelWriter ** ppWriter)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->QueryWriter (pwszWriterClass, (ILib3MFModelWriter **) ppWriter);
	}


	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_queryreader(_In_ PLib3MFModel * pModel, _In_z_ LPCWSTR pwszReaderClass, _Outptr_ PLib3MFModelReader ** ppReader)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->QueryReader (pwszReaderClass, (ILib3MFModelReader **) ppReader);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getresourcebyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelResource ** ppResource)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->GetResourceByID (nResourceID, (ILib3MFModelResource **) ppResource);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_gettexture2dbyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelTexture2D ** ppTexture)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetTexture2DByID(nResourceID, (ILib3MFModelTexture2D **)ppTexture);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getbasematerialbyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelBaseMaterial ** ppMaterial)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetBaseMaterialByID(nResourceID, (ILib3MFModelBaseMaterial **)ppMaterial);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmeshobjectbyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelMeshObject ** ppMeshObject)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetMeshObjectByID(nResourceID, (ILib3MFModelMeshObject **)ppMeshObject);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getcomponentsobjectbyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelComponentsObject ** ppComponentsObject)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetComponentsObjectByID(nResourceID, (ILib3MFModelComponentsObject **)ppComponentsObject);
	}


	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getbuilditems(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelBuildItemIterator ** ppIterator)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->GetBuildItems ((ILib3MFModelBuildItemIterator **) ppIterator);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_resources(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->GetResources ((ILib3MFModelResourceIterator **) ppIterator);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getobjects(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->GetObjects ((ILib3MFModelResourceIterator **) ppIterator);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmeshobjects(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->GetMeshObjects ((ILib3MFModelResourceIterator **) ppIterator);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getcomponentsobjects(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->GetComponentsObjects ((ILib3MFModelResourceIterator **) ppIterator);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_get2dtextures(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->Get2DTextures((ILib3MFModelResourceIterator **)ppIterator);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getbasematerials(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->Get2DTextures((ILib3MFModelResourceIterator **)ppIterator);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getthumbnails(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelThumbnailIterator ** ppIterator)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetThumbnails((ILib3MFModelThumbnailIterator **)ppIterator);
	}


	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_mergetomodel(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModel ** ppMergedModel)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->MergeToModel ((ILib3MFModel **) ppMergedModel);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addmeshobject(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelMeshObject ** ppMeshObject)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->AddMeshObject ((ILib3MFModelMeshObject **) ppMeshObject);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addcomponentsobject(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelComponentsObject ** ppComponentsObject)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->AddComponentsObject ((ILib3MFModelComponentsObject **) ppComponentsObject);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addtexture2d(_In_ PLib3MFModel * pModel, _In_z_ LPCWSTR pwszPath, _Outptr_ PLib3MFModelTexture2D ** ppTextureInstance)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->AddTexture2D(pwszPath, (ILib3MFModelTexture2D **)ppTextureInstance);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addbasematerialgroup(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelBaseMaterial ** ppBaseMaterialInstance)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->AddBaseMaterialGroup((ILib3MFModelBaseMaterial **)ppBaseMaterialInstance);
	}



	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addbuilditem(_In_ PLib3MFModel * pModel, _In_ PLib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pTransform, _Outptr_ PLib3MFModelBuildItem ** ppBuildItem)
	{
	    if (!pModel)
            return LIB3MF_POINTER;

        return ((ILib3MFModel *) pModel)->AddBuildItem ((ILib3MFModelObjectResource *) pObject, pTransform, (ILib3MFModelBuildItem **) ppBuildItem);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_removebuilditem(_In_ PLib3MFModel * pModel, _In_ PLib3MFModelBuildItem * pBuildItem)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->RemoveBuildItem((ILib3MFModelBuildItem *)pBuildItem);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_gettexturestreamcount(_In_ PLib3MFModel * pModel, _Out_ DWORD * pnCount)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetTextureStreamCount(pnCount);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_gettexturestreamsize(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_ UINT64 * pnSize)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetTextureStreamSize(nIndex, pnSize);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_gettexturestreampath(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetTextureStreamPath(nIndex, pwszBuffer, cbBufferSize, pcbNeededChars);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmetadatacount(_In_ PLib3MFModel * pModel, _Out_ DWORD * pnCount)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetMetaDataCount(pnCount);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmetadatakey(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetMetaDataKey(nIndex, pwszBuffer, cbBufferSize, pcbNeededChars);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmetadatavalue(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->GetMetaDataValue(nIndex, pwszBuffer, cbBufferSize, pcbNeededChars);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addmetadata(_In_ PLib3MFModel * pModel, _In_ LPCWSTR pszwKey, _In_ LPCWSTR pszwValue)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->AddMetaData(pszwKey, pszwValue);
	}

	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_removemetadata(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex)
	{
		if (!pModel)
			return LIB3MF_POINTER;

		return ((ILib3MFModel *)pModel)->RemoveMetaData(nIndex);
	}

};

};
