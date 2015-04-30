/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

Abstract: Interface Exports for plain C DLLs.

--*/

#ifndef __NMR_DLLINTERFACES
#define __NMR_DLLINTERFACES

#ifdef NMR_COM_EMULATION
#include "Common/Platform/NMR_COM_Emulation.h"
#else
#include "Common/Platform/NMR_COM_Native.h"
#endif

#include "Common/Platform/NMR_SAL.h"
#include "Common/Platform/NMR_WinTypes.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Model/COM/NMR_COMVersion.h"


namespace NMR {

extern "C" {

	// Define void "class pointers" for piping through the plain C interface
	typedef void * PLib3MFBase;
	typedef PLib3MFBase PLib3MFModel;
	typedef PLib3MFBase PLib3MFModelWriter;
	typedef PLib3MFBase PLib3MFModelReader;
	typedef PLib3MFBase PLib3MFModelResource;
	typedef PLib3MFBase PLib3MFModelResourceIterator;
	typedef PLib3MFBase PLib3MFModelObjectResource;
	typedef PLib3MFBase PLib3MFModelMeshObject;
	typedef PLib3MFBase PLib3MFModelComponentsObject;
	typedef PLib3MFBase PLib3MFModelComponent;
	typedef PLib3MFBase PLib3MFModelBuildItem;
	typedef PLib3MFBase PLib3MFModelBuildItemIterator;

	// Base functions
    /**
    * retrieves the current version of the 3MF implementation and specification
    *
    * @param[out] pMajorVersion returns the major version of the Specification
    * @param[out] pMinorVersion returns the minor version of the Specification
    * @return error code or 0 (success)
    */
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_getspecversion (_Out_ DWORD * pMajorVersion, _Out_ DWORD * pMinorVersion);

    /**
    * retrieves the current interface version of the library (build version)
    * this version will increment with each official release of the library,
    * and may be used to ensure API compatibility.
    *
    * @param[out] pInterfaceVersion returns the interface version of the DLL
    * @return error code or 0 (success)
    */
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_getinterfaceversion (_Out_ DWORD * pInterfaceVersion);

    /**
    * creates an empty model instance
    *
    * @param[out] ppModel returns created model instance
    * @return error code or 0 (success)
    */
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_createmodel(_Outptr_ PLib3MFModel ** ppModel);

	/**
	* Frees all memory of any object instance.
	*
	* @param[out] pObject Object to free memory of.
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_free(_In_ PLib3MFBase * pInstance);

	// Reader/Writer
	/**
	* Writes out the model as file. The file type is specified by the Model Writer class
	*
	* @param[in] pWriter Writer Instance
	* @param[in] pwszFilename Filename to write into
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_writer_writetofile (_In_ PLib3MFModelWriter * pWriter, _In_z_ LPCWSTR pwszFilename);

	/**
	* Reads a model from a file. The file type is specified by the Model Read class
	*
	* @param[in] pReader Reader Instance
	* @param[in] pwszFilename Filename to read from
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_readfromfile (_In_ PLib3MFModelReader * pReader, _In_z_ LPCWSTR pwszFilename);

	/**
	* Returns Warning and Error Count of the read process
	*
	* @param[in] pReader Reader Instance
	* @param[out] pnWarningCount filled with the count of the occurred warnings.
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_getwarningcount(_In_ PLib3MFModelReader * pReader, _Out_ DWORD * pnWarningCount);

	/**
	* Returns Warning and Error Information of the read process
	*
	* @param[in] pReader Reader Instance
	* @param[in] nIndex Index of the Warning. Valid values are 0 to WarningCount - 1
	* @param[out] pErrorCode filled with the error code of the warning
	* @param[out] pwszBuffer filled with the error message, may be NULL
	* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
	* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_getwarning(_In_ PLib3MFModelReader * pReader, _In_ DWORD nIndex, _Out_ DWORD * pErrorCode, _Out_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

	// Resources
	/**
	* Retrieves the ID of a Model Resource Instance

	*@param[in] pResource Resource Instance
	* @param[out] nResourceID Filled with the ID of the Resource Instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resource_getresourceid (_In_ PLib3MFModelResource * pResource, _Out_ DWORD * pnResourceID);

	/**
	* Iterates to the next resource in the list.
	*
	* @param[in] pIterator Iterator Instance
	* @param[out] pbHasNext returns, if there is a resource to use.
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resourceiterator_movenext(_In_ PLib3MFModelResourceIterator * pIterator, _Out_ BOOL * pbHasNext);

	/**
	* Iterates to the previous resource in the list.
	*
	* @param[in] pIterator Iterator Instance
	* @param[out] pbHasPrevious returns, if there is a resource to use.
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resourceiterator_moveprevious(_In_ PLib3MFModelResourceIterator * pIterator, _Out_ BOOL * pbHasPrevious);

	/**
	* Returns the resource the iterator points at.
	*
	* @param[in] pIterator Iterator Instance
	* @param[out] ppResourceInstance returns the resource instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resourceiterator_getcurrent(_In_ PLib3MFModelResourceIterator * pIterator, _Outptr_ PLib3MFModelResource ** ppResourceInstance);

	/**
	* Creates a new resource iterator with the same resource list.
	*
	* @param[in] pIterator Iterator Instance
	* @param[out] ppIterator returns the cloned Iterator instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resourceiterator_clone(_In_ PLib3MFModelResourceIterator * pIterator, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

    /**
    * Retrieves a object's type
    *
	* @param[in] pObject Object Resource Instance
    * @param[out] pObjectType returns object type constant. See ModelTypes.h
    * @return error code or 0 (success)
    */
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_gettype(_In_ PLib3MFModelObjectResource * pObject, _Out_ DWORD * pObjectType);

    /**
    * Sets an object's type
    *
	* @param[in] pObject Object Resource Instance
    * @param[out] ObjectType object type constant. See ModelTypes.h
    * @return error code or 0 (success)
    */
    LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_settype(_In_ PLib3MFModelObjectResource * pObject, _In_ DWORD ObjectType);

    /**
    * Retrieves a object's name string
    *
	* @param[in] pObject Object Resource Instance
    * @param[out] pwszBuffer buffer to fill
    * @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
    * @param[out] pcbNeededChars returns needed characters in buffer
    * @return error code or 0 (success)
    */
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_getname(_In_ PLib3MFModelObjectResource * pObject, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

    /**
    * Sets a build object's name string
    *
    * @param[in] pwszPartNumber new part number string for referencing parts from the outside.
    * @return error code or 0 (success)
    */
    LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setname(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCWSTR pwszPartNumber);

    /**
    * Retrieves a object's part number string
    *
    * @param[out] pwszBuffer buffer to fill
    * @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
    * @param[out] pcbNeededChars returns needed characters in buffer
    * @return error code or 0 (success)
    */
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_getpartnumber(_In_ PLib3MFModelObjectResource * pObject, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

    /**
    * Sets a build object's part number string
    *
    * @param[in] pwszPartNumber new part number string for referencing parts from the outside.
    * @return error code or 0 (success)
    */
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setpartnumber(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCWSTR pwszPartNumber);

	// Mesh Object
	/**
	* Returns the vertex count of a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[out] pnVertexCount filled with the vertex count
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getvertexcount(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ DWORD * pnVertexCount);

	/**
	* Returns the triangle count of a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[out] pnTriangleCount filled with the triangle count
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_gettrianglecount(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ DWORD * pnTriangleCount);

	/**
	* Returns coordinates of a single vertex of a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[in] nIndex Index of the vertex (0 to vertexcount - 1)
	* @param[out] pVertex filled with the vertex coordinates
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getvertex(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _Out_ MODELMESHVERTEX * pVertex);

	/**
	* Sets the coordinates of a single vertex of a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[in] nIndex Index of the vertex (0 to vertexcount - 1)
	* @param[in] pVertex contains the vertex coordinates
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setvertex(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _In_ MODELMESHVERTEX * pVertex);

	/**
	* Adds a single vertex to a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[in] pVertex contains the vertex coordinates
	* @param[out] pnIndex filled with the new Index of the vertex
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_addvertex(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ MODELMESHVERTEX * pVertex, _Out_opt_ DWORD * pnIndex);

	/**
	* Returns indices of a single triangle of a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
	* @param[out] pTriangle filled with the triangle indices
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_gettriangle (_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _Out_ MODELMESHTRIANGLE * pTriangle);

	/**
	* Sets the indices of a single triangle of a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
	* @param[in] pTriangle contains the triangle indices
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_settriangle(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _In_ MODELMESHTRIANGLE * pTriangle);

	/**
	* Adds a single triangle to a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[in] pTriangle contains the triangle indices
	* @param[out] pnIndex filled with the new Index of the vertex
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_addtriangle(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ MODELMESHTRIANGLE * pTriangle, _Out_opt_ DWORD * pnIndex);

	/**
	* Retrieves all vertex coordinates of a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[out] pVertices buffer filled with the vertex coordinates
	* @param[in] nBufferSize size of the buffer in elements, must be at least vertexcount
	* @param[out] pnVertexCount returns how many vertices have been written
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getvertices(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ MODELMESHVERTEX * pVertices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnVertexCount);

	/**
	* Retrieves all triangle indices of a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[out] pIndices buffer filled with the triangle indices
	* @param[in] nBufferSize size of the buffer in elements, must be at least triangle count
	* @param[out] pnTriangleCount returns how many triangles have been written
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_gettriangleindices(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ MODELMESHTRIANGLE * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnTriangleCount);

	/**
	* Sets the whole geometry of a mesh object
	*
	* @param[in] pMeshObject Mesh Object Instance
	* @param[in] pVertices Array of vertex coordinates
	* @param[in] nVertexCount Size of the vertex array
	* @param[in] pTriangles Array of triangle indices
	* @param[in] nTriangleCount Size of the triangle array
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setgeometry(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ MODELMESHVERTEX * pVertices, _In_ DWORD nVertexCount, _In_ MODELMESHTRIANGLE * pTriangles, _In_ DWORD nTriangleCount);

	// Components
	/**
	* Returns the Resource Instance of the component.
	*
	* @param[in] pComponent component instance
	* @param[out] ppObjectResource filled with the Resource Instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_getobjectresource(_In_ PLib3MFModelComponent * pComponent, _Outptr_ PLib3MFModelObjectResource ** ppObjectResource);

	/**
	* Returns the transformation matrix of the component.
	*
	* @param[in] pComponent component instance
	* @param[out] pTransformation filled with the component transformation matrix
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_gettransform(_In_ PLib3MFModelComponent * pComponent, _Out_ MODELTRANSFORM * pTransformation);

	/**
	* Sets the transformation matrix of the component.
	*
	* @param[in] pComponent component instance
	* @param[in] pTransformation new transformation matrix
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_settransform(_In_ PLib3MFModelComponent * pComponent, _In_ MODELTRANSFORM * pTransformation);

	/**
	* Returns the Resource ID of the component.
	*
	* @param[in] pComponent component instance
	* @param[out] pnResourceID returns the Resource ID
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_getobjectresourceid(_In_ PLib3MFModelComponent * pComponent, _Out_ DWORD * pnResourceID);

	/**
	* Returns, if the component has a different transformation than the identity matrix
	*
	* @param[in] pComponent component instance
	* @param[out] pbHasTransform if true is returned, the transformation is not equal than the identity
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_hastransform(_In_ PLib3MFModelComponent * pComponent, _Out_ BOOL * pbHasTransform);

	// Component Object
	/**
	* Adds a new component to a component object
	*
	* @param[in] pComponentsObject component object instance
	* @param[in] pObject object to add as component. May not lead to circular references!
	* @param[in] pmTransform optional transform matrix for the component
	* @param[out] ppComponent new component instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_addcomponent(_In_ PLib3MFModelComponentsObject * pComponentsObject, _In_ PLib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pmTransform, _Outptr_ PLib3MFModelComponent ** ppComponent);

	/**
	* Retrieves a component from a component object
	*
	* @param[in] pComponentsObject component object instance
	* @param[in] nIndex index of the component to retrieve (0 to componentcount - 1)
	* @param[out] ppComponent retrieved component instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_getcomponent(_In_ PLib3MFModelComponentsObject * pComponentsObject, _In_ DWORD nIndex, _Outptr_ PLib3MFModelComponent ** ppComponent);

	/**
	* Retrieves a component count of a component object
	*
	* @param[in] pComponentsObject component object instance
	* @param[out] pComponentCount returns the component count
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_getcomponentcount(_In_ PLib3MFModelComponentsObject * pComponentsObject, _Out_ DWORD * pComponentCount);

	// Build Item
	/**
	* Retrieves the object resource associated to a build item
	*
	* @param[in] pBuildItem build item instance
	* @param[out] ppObject returns the object resource instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getobjectresource(_In_ PLib3MFModelBuildItem * pBuildItem, _Outptr_ PLib3MFModelObjectResource ** ppObject);

	/**
	* Retrieves the object resource id associated to a build item
	*
	* @param[in] pBuildItem build item instance
	* @param[out] pnID returns the ID of the object
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getobjectresourceid(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_ DWORD * pnID);

	/**
	* Checks, if a build item has a non-identity transformation matrix
	*
	* @param[in] pBuildItem build item instance
	* @param[out] pbHasTransform returns true, if the transformation matrix is not the identity
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_hasobjecttransform(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_ BOOL * pbHasTransform);

	/**
	* Retrieves a build item's transformation matrix
	*
	* @param[in] pBuildItem build item instance
	* @param[out] pmTransform returns the transformation matrix
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getobjecttransform(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_ MODELTRANSFORM * pmTransform);

	/**
	* Sets a build item's transformation matrix
	*
	* @param[in] pBuildItem build item instance
	* @param[in] pmTransform new transformation matrix
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_setobjecttransform(_In_ PLib3MFModelBuildItem * pBuildItem, _In_ MODELTRANSFORM * pmTransform);

	/**
	* Retrieves a build item's part number string
	*
	* @param[in] pBuildItem build item instance
	* @param[out] pwszBuffer buffer to fill
	* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
	* @param[out] pcbNeededChars returns needed characters in buffer
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getpartnumber(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);

	/**
	* Sets a build item's part number string
	*
	* @param[in] pBuildItem build item instance
	* @param[in] pwszPartNumber new part number string for referencing parts from the outside.
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_setpartnumber(_In_ PLib3MFModelBuildItem * pBuildItem, _In_z_ LPCWSTR pwszPartNumber);

	// Build Item Iterator
	/**
	* Iterates to the next build item in the list.
	*
	* @param[in] pIterator Iterator Instance
	* @param[out] pbHasNext returns, if there is a build item to use.
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditemiterator_movenext(_In_ PLib3MFModelBuildItemIterator * pIterator, _Out_ BOOL * pbHasNext);

	/**
	* Iterates to the previous build item in the list.
	*
	* @param[in] pIterator Iterator Instance
	* @param[out] pbHasPrevious returns, if there is a resource to use.
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditemiterator_moveprevious(_In_ PLib3MFModelBuildItemIterator * pIterator, _Out_ BOOL * pbHasPrevious);

	/**
	* Returns the build item the iterator points at.
	*
	* @param[in] pIterator Iterator Instance
	* @param[out] ppResultBuildItem returns the build item instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditemiterator_getcurrent(_In_ PLib3MFModelBuildItemIterator * pIterator, _Outptr_ PLib3MFModelBuildItem ** ppResultBuildItem);

	/**
	* Creates a new build item iterator with the same build item list.
	*
	* @param[in] pIterator Iterator Instance
	* @param[out] ppIterator returns the cloned Iterator instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditemiterator_clone(_In_ PLib3MFModelBuildItemIterator * pIterator, _Outptr_ PLib3MFModelBuildItemIterator ** ppIterator);

	// Model
	/**
	* sets the units of a model
	*
	* @param[in] pModel Model instance
	* @param[in] Unit enum value for the model unit (see NMR_ModelTypes.h for details)
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_setunit(_In_ PLib3MFModel * pModel, _In_ DWORD Unit);

	/**
	* sets the units of a model
	*
	* @param[in] pModel Model instance
	* @param[out] pUnit return enum value for of model's unit (see NMR_ModelTypes.h for details)
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getunit(_In_ PLib3MFModel * pModel, _Out_ DWORD * pUnit);

	/**
	* sets the language of a model
	*
	* @param[in] pModel Model instance
	* @param[in] pwszLanguage Language string identifier
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_setlanguage(_In_ PLib3MFModel * pModel, _In_z_ LPCWSTR pwszLanguage);

	/**
	* retrieves the language of a model
	*
	* @param[in] pModel Model instance
	* @param[out] pwszBuffer Buffer to write into
	* @param[in] cbBufferSize Buffer size
	* @param[out] pcbNeededChars returns chars which are necessary to store the language string
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getlanguage(_In_ PLib3MFModel * pModel, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);

	/**
	* creates a model writer instance for a specific file type
	*
	* @param[in] pModel Model instance
	* @param[in] pwszWriterClass string identifier for the file (currently "stl" and "3mf")
	* @param[out] ppWriter returns the writer instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_querywriter(_In_ PLib3MFModel * pModel, _In_z_ LPCWSTR pwszWriterClass, _Outptr_ PLib3MFModelWriter ** ppWriter);

	/**
	* creates a model reader instance for a specific file type
	*
	* @param[in] pModel Model instance
	* @param[in] pwszWriterClass string identifier for the file (currently "stl" and "3mf")
	* @param[out] ppReader returns the reader instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_queryreader(_In_ PLib3MFModel * pModel, _In_z_ LPCWSTR pwszReaderClass, _Outptr_ PLib3MFModelReader ** ppReader);

	/**
	* finds a model resource by its id
	*
	* @param[in] pModel Model instance
	* @param[in] nResourceID Resource ID
	* @param[out] ppResource returns the resource instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getresourcebyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelResource ** ppResource);

	/**
	* creates a build item iterator instance with all build items
	*
	* @param[in] pModel Model instance
	* @param[out] ppIterator returns the iterator instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getbuilditems(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelBuildItemIterator ** ppIterator);

	/**
	* creates a resource iterator instance with all resources
	*
	* @param[in] pModel Model instance
	* @param[out] ppIterator returns the iterator instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_resources(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

	/**
	* creates a resource iterator instance with all object resources
	*
	* @param[in] pModel Model instance
	* @param[out] ppIterator returns the iterator instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_objects(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

	/**
	* creates a resource iterator instance with all mesh object resources
	*
	* @param[in] pModel Model instance
	* @param[out] ppIterator returns the iterator instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_meshobjects(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

	/**
	* creates a resource iterator instance with all component object resources
	*
	* @param[in] pModel Model instance
	* @param[out] ppIterator returns the iterator instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_componentsobjects(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

	/**
	* merges all components and objects which are referenced by a build item. The memory is duplicated and a
	* new model is created.
	*
	* @param[in] pModel Model instance
	* @param[out] ppMergedModel returns the merged model instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_mergetomodel(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModel ** ppMergedModel);

	/**
	* adds an empty mesh object to the model
	*
	* @param[in] pModel Model instance
	* @param[out] ppMeshObject returns the mesh object instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addmeshobject(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelMeshObject ** ppMeshObject);

	/**
	* adds an empty component object to the model
	*
	* @param[in] pModel Model instance
	* @param[out] ppComponentsObject returns the component object instance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addcomponentsobject(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelComponentsObject ** ppComponentsObject);

	/**
	* adds a build item to the model
	*
	* @param[in] pModel Model instance
	* @param[in] pObject Object instance
	* @param[in] pTransform Transformation matrix
	* @param[out] ppBuildItem returns the build iteminstance
	* @return error code or 0 (success)
	*/
	LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addbuilditem(_In_ PLib3MFModel * pModel, _In_ PLib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pTransform, _Outptr_ PLib3MFModelBuildItem ** ppBuildItem);

};

};

#endif //__NMR_DLLINTERFACES
