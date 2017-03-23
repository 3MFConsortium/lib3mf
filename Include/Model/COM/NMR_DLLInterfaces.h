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

#ifdef __GNUC__
#ifndef NMR_COM_EMULATION
#define NMR_COM_EMULATION
#endif //NMR_COM_EMULATION
#endif //__GNUC__

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
		typedef PLib3MFBase PLib3MFDefaultPropertyHandler;
		typedef PLib3MFBase PLib3MFPropertyHandler;
		typedef PLib3MFBase PLib3MFModelBaseMaterial;
		typedef PLib3MFBase PLib3MFModelTexture2D;
		typedef PLib3MFBase PLib3MFModelThumbnailIterator;
		typedef PLib3MFBase PLib3MFModelThumbnail;
		typedef PLib3MFBase PLib3MFModelAttachment;

		typedef BOOL(*PLib3MFModelWriterCallback)(_In_ const BYTE * pData, _In_ const DWORD cbBytes, _In_ const void * pUserData);

		// Base functions
		/**
		* retrieves the current version of the 3MF implementation and specification
		*
		* @param[out] pMajorVersion returns the major version of the Specification
		* @param[out] pMinorVersion returns the minor version of the Specification
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_getspecversion(_Out_ DWORD * pMajorVersion, _Out_ DWORD * pMinorVersion);

		/**
		* retrieves the current interface version of the library (build version)
		* this version will increment with each official release of the library,
		* and may be used to ensure API compatibility.
		*
		* @param[out] pInterfaceVersion returns the interface version of the DLL
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_getinterfaceversion(_Out_ DWORD * pInterfaceVersion);

		/**
		* checks whether a extension is supported by the DLL and which version of the interface is used
		* This extension version will increment with each change of the API of the extension
		* and may be used to ensure API compatibility.
		*
		* @param[in] pwszExtensionUrl URL of extension to check
		* @param[out] pbIsSupported returns whether the extension is supported or not
		* @param[out] pInterfaceVersion returns the interface version of of the extensions (if extension is supported)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_queryextension(_In_z_ LPCWSTR pwszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion);

		/**
		* checks whether a extension is supported by the DLL and which version of the interface is used
		* This extension version will increment with each change of the API of the extension
		* and may be used to ensure API compatibility.
		*
		* @param[in] pwszExtensionUrl URL of extension to check as UTF8 string
		* @param[out] pbIsSupported returns whether the extension is supported or not
		* @param[out] pInterfaceVersion returns the interface version of of the extensions (if extension is supported)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_queryextensionutf8(_In_z_ LPCSTR pszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion);

		/**
		* creates an empty model instance
		*
		* @param[out] ppModel returns created model instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_createmodel(_Outptr_ PLib3MFModel ** ppModel, _In_ BOOL bInitialize);

		/**
		* Frees all memory of any object instance.
		*
		* @param[out] pInstance Object to free memory of.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_release(_In_ PLib3MFBase * pInstance);

		/**
		* Returns detailed information of the last known error an object method.
		* The error information is available for every method returning a LIB3MF_FAILED
		* constant.
		*
		* @param[out] pInstance Object instance.
		* @param[out] pErrorCode Error Code
		* @param[out] pErrorMessage Returns pointer to the error message string, NULL if no error.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_getlasterror(_In_ PLib3MFBase * pInstance, _Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);


		// Reader/Writer
		/**
		* Writes out the model as file. The file type is specified by the Model Writer class
		*
		* @param[in] pWriter Writer Instance
		* @param[in] pwszFilename Filename to write into as UTF16 string
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_writer_writetofile(_In_ PLib3MFModelWriter * pWriter, _In_z_ LPCWSTR pwszFilename);

		/**
		* Writes out the model as file. The file type is specified by the Model Writer class
		*
		* @param[in] pWriter Writer Instance
		* @param[in] pwszFilename Filename to write into as UTF8 string
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_writer_writetofileutf8(_In_ PLib3MFModelWriter * pWriter, _In_z_ LPCSTR pszFilename);

		/**
		* Writes out the model and passes the data to a provided callback function. The file type is specified by the Model Writer class
		*
		* @param[in] pWriter Writer Instance
		* @param[in] pWriteCallback Callback to call for writing a data chunk.
		* @param[in] pSeekCallback Callback to call for seeking in the streamk.
		* @param[in] pUserData Userdata that is passed to the callback function
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_writer_writetocallback(_In_ PLib3MFModelWriter * pWriter, _In_ void * pWriteCallback, _In_ void * pSeekCallback, _In_opt_ void * pUserData);

		/**
		* Reads a model from a file. The file type is specified by the Model Read class
		*
		* @param[in] pReader Reader Instance
		* @param[in] pwszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_readfromfile(_In_ PLib3MFModelReader * pReader, _In_z_ LPCWSTR pwszFilename);

		/**
		* Reads a model from a file. The file type is specified by the Model Read class
		*
		* @param[in] pReader Reader Instance
		* @param[in] pwszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_readfromfileutf8(_In_ PLib3MFModelReader * pReader, _In_z_ LPCSTR pszFilename);


		/**
		* Adds a relationship type which shall be read as attachment in memory while loading
		*
		* @param[in] pReader Reader Instance
		* @param[in] pwszRelationshipType String of the relationship type (UTF16)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_addrelationtoread (_In_ PLib3MFModelReader * pReader,  _In_z_ LPCWSTR pwszRelationshipType);

		/**
		* Removes a relationship type which shall be read as attachment in memory while loading
		*
		* @param[in] pReader Reader Instance
		* @param[in] pwszRelationshipType String of the relationship type (UTF16)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_removerelationtoread (_In_ PLib3MFModelReader * pReader,  _In_z_ LPCWSTR pwszRelationshipType);

		/**
		* Adds a relationship type which shall be read as attachment in memory while loading
		*
		* @param[in] pReader Reader Instance
		* @param[in] pszRelationshipType String of the relationship type (UTF8)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_addrelationtoreadutf8(_In_ PLib3MFModelReader * pReader,  _In_z_ LPCSTR pszRelationshipType);


		/**
		* Remove a relationship type which shall be read as attachment in memory while loading
		*
		* @param[in] pReader Reader Instance
		* @param[in] pszRelationshipType String of the relationship type (UTF8)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_removerelationtoreadutf8(_In_ PLib3MFModelReader * pReader,  _In_z_ LPCSTR pszRelationshipType);


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
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_resource_getresourceid(_In_ PLib3MFModelResource * pResource, _Out_ DWORD * pnResourceID);

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
		* Iterates to the next thumbnail in the list.
		*
		* @param[in] pIterator Iterator Instance
		* @param[out] pbHasNext returns, if there is a thumbnail to use.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_thumbnailiterator_movenext(_In_ PLib3MFModelThumbnailIterator * pIterator, _Out_ BOOL * pbHasNext);

		/**
		* Iterates to the previous thumbnail in the list.
		*
		* @param[in] pIterator Iterator Instance
		* @param[out] pbHasPrevious returns, if there is a thumbnail to use.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_thumbnailiterator_moveprevious(_In_ PLib3MFModelThumbnailIterator * pIterator, _Out_ BOOL * pbHasPrevious);

		/**
		* Returns the resource the iterator points at.
		*
		* @param[in] pIterator Iterator Instance
		* @param[out] ppThumbnailInstance returns the thumbnail instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_thumbnailiterator_getcurrent(_In_ PLib3MFModelThumbnailIterator * pIterator, _Outptr_ PLib3MFModelThumbnail ** ppThumbnailInstance);

		/**
		* Creates a new resource iterator with the same resource list.
		*
		* @param[in] pIterator Iterator Instance
		* @param[out] ppIterator returns the cloned Iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_thumbnailiterator_clone(_In_ PLib3MFModelThumbnailIterator * pIterator, _Outptr_ PLib3MFModelThumbnailIterator ** ppIterator);


		/**
		* Removes all properties of a specific triangle.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_removeproperty(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex);

		/**
		* Removes all properties of the triangle mesh.
		*
		* @param[in] pPropertyHandler Property Handler
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_removeallproperties(_In_ PLib3MFPropertyHandler * pPropertyHandler);

		/**
		* Returns the property type of the specific triangle
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[out] pnPropertyType Returns the property type of the triangle
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_getpropertytype(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _Out_ eModelPropertyType * pnPropertyType);

		/**
		* Returns the base material of a specific triangle.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[out] pnMaterialGroupID returns the material group id, per triangle. A return group id of 0 means either no property at all or a non-material property.
		* @param[out] pnMaterialIndex returns the material index, per triangle. Returns 0, if no base material is assigned.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_getbasematerial(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _Out_ DWORD * pnMaterialGroupID, _Out_ DWORD * pnMaterialIndex);

		/**
		* Returns the base materials of all triangles.
		* If a triangle property is not a material, the returned material group ID will be 0.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[out] pnMaterialGroupIDs will be filled with the material group ids of the triangles. Array must have trianglecount entries. A return group id of 0 means either no property at all or a non-material property.
		* @param[out] pnMaterialIndices will be filled with the material group indices of the triangles. Array must have trianglecount entries.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_getbasematerialarray(_In_ PLib3MFPropertyHandler * pPropertyHandler, _Out_ DWORD * pnMaterialGroupIDs, _Out_ DWORD * pnMaterialIndices);

		/**
		* Sets the material of a triangle to a specific single value. All other Triangle properties are removed.
		* This must be a base material .
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] nMaterialGroupID Group ID of the Material Group
		* @param[in] nMaterialIndex Index of the Material in the Group
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setbasematerial(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ ModelResourceID nMaterialGroupID, _In_ DWORD nMaterialIndex);

		/**
		* Sets the materials of all triangles to specific values.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] pnMaterialGroupIDs array of the material Group IDs. Must have trianglecount entries. If a group ID of 0 is specified.
		* @param[in] pnMaterialIndices array of the corresponding material indices. Must have trianglecount entries.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setbasematerialarray(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ ModelResourceID * pnMaterialGroupIDs, _In_ DWORD * pnMaterialIndices);


		/**
		* Returns the color of a specific triangle.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[out] pColor returns the color values of the three nodes of the triangle. (#00000000) means no property or a different kind of property!
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_getcolor(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColor);

		/**
		* Returns the color array of all triangles
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[out] pColors returns the color values of the three nodes of each triangle. Must have at least trianglecount array entries.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_getcolorarray(_In_ PLib3MFPropertyHandler * pPropertyHandler, _Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColors);


		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setsinglecolor(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ MODELMESHCOLOR_SRGB * pColor);


		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setsinglecolorrgb(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue);

		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setsinglecolorrgba(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha);

		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setsinglecolorfloatrgb(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue);


		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setsinglecolorfloatrgba(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha);

		/**
		* Sets the (single) color of all triangles. All other properties are removed.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] pColors new color values for the triangles. (#00000000) means no color property.. Must have at least trianglecount array entries.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setsinglecolorarray(_In_ PLib3MFPropertyHandler * pPropertyHandler, _Out_ MODELMESHCOLOR_SRGB * pColors);

		/**
		* Sets the specific triangle to a color per vertex. All other properties are removed.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color values of the three nodes of the triangle. (#00000000) means no color property is set.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setgradientcolor(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColor);

		/**
		* Sets the (gradient) color of all triangles. All other properties are removed.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] pColors returns the color values of the three nodes of each triangle. Must have at least trianglecount array entries. (#00000000) means no color property is set.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setgradientcolorarray(_In_ PLib3MFPropertyHandler * pPropertyHandler, _Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColors);


		/**
		* Returns the 2D texture information of a specific triangle.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[out] pTexture returns the UV texture values of the three nodes of the triangle. texture ID 0 means no property or a different kind of property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_gettexture(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _Out_ MODELMESHTEXTURE2D * pTexture);

		/**
		* Returns the 2D texture information of all triangles.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[out] pTextures returns the UV texture values of the three nodes of all triangles. Must have at least trianglecount array entries.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_gettexturearray(_In_ PLib3MFPropertyHandler * pPropertyHandler, _Out_ MODELMESHTEXTURE2D * pTextures);

		/**
		* Sets the 2D texture information of a specific triangle.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pTexture new UV texture values of the three nodes of the triangle. texture ID 0 means no property or a different kind of property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_settexture(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ MODELMESHTEXTURE2D * pTexture);

		/**
		* Sets the 2D texture information of all triangles.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] pTexture new UV texture values of the three nodes of all triangles. Must have at least trianglecount array entries.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_settexturearray(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ MODELMESHTEXTURE2D * pTextures);


		/**
		* Removes the default property of the object.
		*
		* @param[in] pPropertyHandler Property Handler
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_removeproperty(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler);

		/**
		* Returns the property type of the object
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[out] pnPropertyType Returns the property type of the object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_getpropertytype(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _Out_ eModelPropertyType * pnPropertyType);

		/**
		* Returns the base material the object
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[out] pnMaterialGroupID returns the material group id, per triangle. A return group id of 0 means either no property at all or a non-material property.
		* @param[out] pnMaterialIndex returns the material index, per triangle. Returns 0, if no base material is assigned.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_getbasematerial(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _Out_ DWORD * pnMaterialGroupID, _Out_ DWORD * pnMaterialIndex);

		/**
		* Sets the material of an object to a specific single value.
		* This must be a base material .
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nMaterialGroupID Group ID of the Material Group
		* @param[in] nMaterialIndex Index of the Material in the Group
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_setbasematerial(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ DWORD nMaterialGroupID, _In_ DWORD nMaterialIndex);


		/**
		* Returns the default property color of an object.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[out] pColor returns the default color of the object. (#00000000) means no property or a different kind of property!
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_getcolor(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _Out_ MODELMESHCOLOR_SRGB * pColor);

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_setcolor(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ MODELMESHCOLOR_SRGB * pColor);

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_setcolorrgb(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue);

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_setcolorrgba(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha);

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_setfloatcolorrgb(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue);

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_setfloatcolorrgba(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha);

		/**
		* Returns the default 2D texture information of an object.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[out] pnTextureID Returns the default Texture ID of the object. 0 means no property or a different kind of property.
		* @param[out] pfU Returns the default U value of the object.
		* @param[out] pfV Returns the default V value of the object.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_gettexture(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _Out_ ModelResourceID * pnTextureID, _Out_ FLOAT * pfU, _Out_ FLOAT * pfV);

		/**
		* Sets the default 2D texture information of an object.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nTextureID Sets the default Texture ID of the object. 0 means not default property.
		* @param[in] fU Sets the default U value of the object.
		* @param[in] fV Sets the default V value of the object.
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_settexture(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ ModelResourceID nTextureID, _In_ FLOAT fU, _In_ FLOAT fV);


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
		* Retrieves a object's name string (UTF8)
		*
		* @param[in] pObject Object Resource Instance
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_getnameutf8(_In_ PLib3MFModelObjectResource * pObject, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Sets a build object's name string
		*
		* @param[in] pObject Object Resource Instance
		* @param[in] pwszName new name of the object as UTF16 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setname(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCWSTR pwszName);

		/**
		* Sets a build object's name string
		*
		* @param[in] pObject Object Resource Instance
		* @param[in] pszName new name of the object as UTF8 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setnameutf8(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCSTR pszName);

		/**
		* Retrieves a object's part number string
		*
		* @param[in] pObject Object Resource Instance
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_getpartnumber(_In_ PLib3MFModelObjectResource * pObject, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Retrieves a object's part number string (UTF8)
		*
		* @param[in] pObject Object Resource Instance
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_getpartnumberutf8(_In_ PLib3MFModelObjectResource * pObject, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Sets a build object's part number string
		*
		* @param[in] pObject Object Resource Instance
		* @param[in] pwszPartNumber new part number (UTF16) string for referencing parts from the outside.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setpartnumber(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCWSTR pwszPartNumber);

		/**
		* Sets a build object's part number string
		*
		* @param[in] pObject Object Resource Instance
		* @param[in] pwszPartNumber new part number (UTF8) string for referencing parts from the outside.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setpartnumberutf8(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCSTR pszPartNumber);

		/**
		* Retrieves, if an object is a mesh object
		*
		* @param[in] pObject Object Resource Instance
		* @param[out] pbIsMeshObject returns, if the object is a mesh object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_ismeshobject(_In_ PLib3MFModelObjectResource * pObject, _Out_ BOOL * pbIsMeshObject);

		/**
		* Retrieves, if an object is a component object
		*
		* @param[in] pObject Object Resource Instance
		* @param[out] pbIsComponentObject returns, if the object is a mesh object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_iscomponentsobject(_In_ PLib3MFModelObjectResource * pObject, _Out_ BOOL * pbIsComponentObject);

		/**
		* Retrieves, if the object is valid according to the core spec.
		* For mesh objects, we distinguish between the type attribute of the object:
		* In case of object type "other", this always means "false"
		* In case of object type "support", this always means "true"
		* In case of object type "model", this means, if the mesh suffices all requirements of the core spec chapter 4.1
		* A component objects is valid if and only if it contains at least one component -
		* and all child components are valid objects.
		*
		* @param[in] pObject Object Resource Instance
		* @param[out] pbIsValid returns, if the object is a valid object description.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_isvalidobject(_In_ PLib3MFModelObjectResource * pObject, _Out_ BOOL * pbIsValid);

		/**
		* creates a default property handler for the object
		*
		* @param[in] pObject Object Resource Instance
		* @param[out] ppPropertyHandler returns a default property handler instance for the object.
		* @return error code or 0 (success)f
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_createdefaultpropertyhandler(_In_ PLib3MFModelObjectResource * pObject, _Out_ PLib3MFDefaultPropertyHandler ** ppPropertyHandler);

		/**
		* creates a default property handler for a specific multiproperty channel of an object
		*
		* @param[in] pObject Object Resource Instance
		* @param[in] nChannel Channel Index
		* @param[out] ppPropertyHandler returns a default property handler instance of an object.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_createdefaultmultipropertyhandler(_In_ PLib3MFModelObjectResource * pObject, _In_ DWORD nChannel, _Out_ PLib3MFDefaultPropertyHandler ** ppPropertyHandler);

		/**
		* Retrieves the count of base materials in the material group.
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[out] pcbCount returns the count of base materials.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_getcount(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _Out_ DWORD * pcbCount);

		/**
		* Retrieves the resource id of the material group.
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[out] pnResourceID returns the id of the material group.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_getresourceid(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _Out_ DWORD * pnResourceID);

		/**
		* Adds a new material to the material group
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] pwszName new name of the base material. (UTF16 String, e.g. "ABS red")
		* @param[in] bRed New red value of display color (0-255)
		* @param[in] bGreen New red value of display color (0-255)
		* @param[in] bBlue New red value of display color (0-255)
		* @param[out] pnResourceIndex returns new Index of the material in the material group
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_addmaterial(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_z_ LPCWSTR pwszName, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ DWORD * pnResourceIndex);

		/**
		* Adds a new material to the material group
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] pwszName new name of the base material. (UTF8 String, e.g. "ABS red")
		* @param[in] bRed New red value of display color (0-255)
		* @param[in] bGreen New red value of display color (0-255)
		* @param[in] bBlue New red value of display color (0-255)
		* @param[out] pnResourceIndex returns new Index of the material in the material group
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_addmaterialutf8(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_z_ LPCSTR pszName, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ DWORD * pnResourceIndex);


		/**
		* Removes a material from the material group
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] nIndex Index of the material in the material group
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_removematerial(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex);

		/**
		* Retrieves a base material's name
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] nIndex Index of the material in the material group
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_getname(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Retrieves a base material's name (UTF8)
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] nIndex Index of the material in the material group
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_getnameutf8(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Sets a base material's name (UTF16)
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] nIndex Index of the material in the material group
		* @param[in] pwszName new name of the base material. (e.g. "ABS red")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_setname(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex, _In_z_ LPCWSTR pwszName);

		/**
		* Sets a base material's name (UTF8)
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] nIndex Index of the material in the material group
		* @param[in] pwszName new name of the base material. (e.g. "ABS red")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_setnameutf8(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex, _In_z_ LPCSTR pszName);

		/**
		* Sets a base material's display color. Alpha is set to 255.
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] nIndex Index of the material in the material group
		* @param[in] bRed New red value (0-255)
		* @param[in] bGreen New green value (0-255)
		* @param[in] bBlue New blue value (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_setdisplaycolorrgb(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue);

		/**
		* Sets a base material's display color.
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] nIndex Index of the material in the material group
		* @param[in] bRed New red value (0-255)
		* @param[in] bGreen New green value (0-255)
		* @param[in] bBlue New blue value (0-255)
		* @param[in] bAlpha New alpha value (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_setdisplaycolorrgba(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha);

		/**
		* Sets a base material's display color. Alpha is set to 1.0.
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] nIndex Index of the material in the material group
		* @param[in] fRed New red value (0.0-1.0)
		* @param[in] fGreen New green value (0.0-1.0)
		* @param[in] fBlue New blue value (0.0-1.0)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_setdisplaycolorfloatrgb(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue);

		/**
		* Sets a base material's display color.
		*
		* @param[in] nIndex Index of the material in the material group
		* @param[in] fRed New red value (0.0-1.0)
		* @param[in] fGreen New green value (0.0-1.0)
		* @param[in] fBlue New blue value (0.0-1.0)
		* @param[in] fAlpha New alpha value (0.0-1.0)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_setdisplaycolorfloatrgba(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha);

		/**
		* Returns a base material's display color.
		*
		* @param[in] pBaseMaterial Base Material Resource Instance
		* @param[in] nIndex Index of the material in the material group
		* @param[out] pbRed Returns red value (0-255)
		* @param[out] pbGreen Returns green value (0-255)
		* @param[out] pbBlue Returns blue value (0-255)
		* @param[out] pbAlpha Returns alpha value (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_basematerial_getdisplaycolor(_In_ PLib3MFModelBaseMaterial * pBaseMaterial, _In_ DWORD nIndex, _Out_ BYTE* pbRed, _Out_ BYTE* pbGreen, _Out_ BYTE* pbBlue, _Out_ BYTE* pbAlpha);


		/**
		* Retrieves a attachment's package path
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_getpath (_In_ PLib3MFModelAttachment * pAttachment, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Retrieves a attachment's package path (UTF8)
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_getpathutf8 (_In_ PLib3MFModelAttachment * pAttachment, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Sets a attachment's package path
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pwszPath new path of the attachment. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_setpath (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCWSTR pwszPath);

		/**
		* Sets a attachment's package path (UTF8)
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pszPath new path of the attachment. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_setpathutf8 (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCSTR pszPath);

		/**
		* Retrieves a attachment's package relationship type
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_getrelationshiptype(_In_ PLib3MFModelAttachment * pAttachment, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Retrieves a attachment's package relationship type (UTF8)
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_getrelationshiptypeutf8 (_In_ PLib3MFModelAttachment * pAttachment, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Sets a attachment's package relationship type
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pwszRelationShipType new relationship type attachment. (e.g. "/Data/data.xml")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_setrelationshiptype (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCWSTR pwszRelationShipType);

		/**
		* Sets a attachment's package relationship type (UTF8)
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pszRelationShipType new path of the attachment. (e.g. "/Data/data.png")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_setrelationshiptypeutf8 (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCSTR pszRelationShipType);

		/**
		* Retrieves the size of the attachment stream.
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pcbStreamSize Returns the stream size
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_getstreamsize (_In_ PLib3MFModelAttachment * pAttachment, _Out_ ULONG64 * pcbStreamSize);

		/**
		* Writes out the attachment as file.
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pwszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_writetofile (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCWSTR pwszFilename);

		/**
		* Writes out the attachment as file. (UTF8)
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_writetofileutf8 (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCSTR pszFilename);

		/**
		* Writes out the attachment into a buffer. Buffer size must be at least the size of the stream.
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pBuffer Buffer to write into
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_writetobuffer (_In_ PLib3MFModelAttachment * pAttachment, _Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);

		/**
		* Writes out the attachment and passes the data to a provided callback function. The file type is specified by the Model Writer class
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pWriteCallback Callback to call for writing a data chunk.
		* @param[in] pUserData Userdata that is passed to the callback function
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_writetocallback (_In_ PLib3MFModelAttachment * pAttachment, _In_ void * pWriteCallback, _In_opt_ void * pUserData);


		/**
		* Reads a attachment from a file.
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pwszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_readfromfile (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCWSTR pwszFilename);

		/**
		* Reads a attachment from a file.
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_readfromfileutf8 (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCSTR pszFilename);

		/**
		* Reads a attachment from a memory buffer.
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pBuffer Buffer to read from
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_readfrombuffer (_In_ PLib3MFModelAttachment * pAttachment, _In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);


		/**
		* Retrieves a texture's package path
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_getpath(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Retrieves a texture's package path (UTF8)
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_getpathutf8(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Sets a texture's package path
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pwszPath new path of the texture resource. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_setpath(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCWSTR pwszPath);

		/**
		* Sets a texture's package path (UTF8)
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pszPath new path of the texture resource. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_setpathutf8(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCSTR pszPath);

		/**
		* Retrieves a texture's content type
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] peContentType returns content type enum
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_getcontenttype(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_ eModelTexture2DType * peContentType);

		/**
		* Sets a texture's content type
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] eContentType new Content Type
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_setcontenttype(_In_ PLib3MFModelTexture2D * pTexture2D, _In_ eModelTexture2DType eContentType);

		/**
		* Retrieves a texture's box2D coordinates.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] pfU returns the U value of the texture
		* @param[out] pfV returns the V value of the texture
		* @param[out] pfWidth returns the Width value of the texture
		* @param[out] pfHeight returns the Height value of the texture
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_getbox2d(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_ FLOAT * pfU, _Out_ FLOAT * pfV, _Out_ FLOAT * pfWidth, _Out_ FLOAT * pfHeight);

		/**
		* Sets a texture's box2D coordinates.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] fU the new U value of the texture
		* @param[out] fV the new V value of the texture
		* @param[out] fWidth the new Width value of the texture
		* @param[out] fHeight the new Height value of the texture
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_setbox2d(_In_ PLib3MFModelTexture2D * pTexture2D, _In_ FLOAT fU, _In_ FLOAT fV, _In_ FLOAT fWidth, _In_ FLOAT fHeight);

		/**
		* Clears a texture's box2D coordinates.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_clearbox2d(_In_ PLib3MFModelTexture2D * pTexture2D);

		/**
		* Retrieves the size of the texture stream.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] pcbStreamSize Returns the stream size
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_getstreamsize(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_ ULONG64 * pcbStreamSize);

		/**
		* Writes out the texture as file.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pwszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_writetofile(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCWSTR pwszFilename);

		/**
		* Writes out the texture as file. (UTF8)
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_writetofileutf8(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCSTR pszFilename);

		/**
		* Writes out the texture into a buffer. Buffer size must be at least the size of the stream.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] pBuffer Buffer to write into
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_writetobuffer(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);

		/**
		* Writes out the texture and passes the data to a provided callback function. The file type is specified by the Model Writer class
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pWriteCallback Callback to call for writing a data chunk.
		* @param[in] pUserData Userdata that is passed to the callback function
		* @return error code or 0 (success)
		*/ 
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_writetocallback(_In_ PLib3MFModelTexture2D * pTexture2D, _In_ void * pWriteCallback, _In_opt_ void * pUserData);

		/**
		* Reads a texture from a file.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pwszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_readfromfile(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCWSTR pwszFilename);

		/**
		* Reads a texture from a file.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_readfromfileutf8(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCSTR pszFilename);

		/**
		* Reads a texture from a memory buffer.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pBuffer Buffer to read from
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_readfrombuffer(_In_ PLib3MFModelTexture2D * pTexture2D, _In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);


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
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_gettriangle(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _Out_ MODELMESHTRIANGLE * pTriangle);

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


		/**
		* creates a property handler for the mesh
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] ppPropertyHandler returns a property handler instance for the mesh.
		* @param[in] pMeshObject Mesh Object Instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_createpropertyhandler(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ PLib3MFPropertyHandler ** ppPropertyHandler);

		/**
		* creates a property handler for a specific multiproperty channel of a mesh
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] ppPropertyHandler returns a property handler instance for the mesh.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_createmultipropertyhandler(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nChannel, _In_ PLib3MFPropertyHandler ** ppPropertyHandler);


		/**
		* Retrieves, if an object describes a topologically oriented and manifold mesh, according to the core spec
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] pbIsOrientedAndManifold returns, if the object is oriented and manifold
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_ismanifoldandoriented(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ BOOL * pbIsOrientedAndManifold);

		// Component
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
		* Retrieves a build item's part number string (UTF8)
		*
		* @param[in] pBuildItem build item instance
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getpartnumberutf8(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);

		/**
		* Sets a build item's part number string
		*
		* @param[in] pBuildItem build item instance
		* @param[in] pwszPartNumber new part number string for referencing parts from the outside.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_setpartnumber(_In_ PLib3MFModelBuildItem * pBuildItem, _In_z_ LPCWSTR pwszPartNumber);

		/**
		* Sets a build item's part number string (UTF8)
		*
		* @param[in] pBuildItem build item instance
		* @param[in] pszPartNumber new part number string for referencing parts from the outside.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_setpartnumberutf8(_In_ PLib3MFModelBuildItem * pBuildItem, _In_z_ LPCSTR pszPartNumber);

		/**
		* Retrieves an internal handle of the build item. This 32bit number is unique throughout the model, but only valid
		* for in-memory use of this instance.
		*
		* @param[in] pBuildItem build item instance
		* @param[out] ppHandle returns the handle
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_gethandle(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_ DWORD * pHandle);

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
		* sets the language of a model (UTF8)
		*
		* @param[in] pModel Model instance
		* @param[in] pwszLanguage Language string identifier
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_setlanguageutf8(_In_ PLib3MFModel * pModel, _In_z_ LPCSTR pwszLanguage);

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
		* retrieves the language of a model (UTF8)
		*
		* @param[in] pModel Model instance
		* @param[out] pszBuffer Buffer to write into
		* @param[in] cbBufferSize Buffer size
		* @param[out] pcbNeededChars returns chars which are necessary to store the language string
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getlanguageutf8(_In_ PLib3MFModel * pModel, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);

		/**
		* creates a model writer instance for a specific file type
		*
		* @param[in] pModel Model instance
		* @param[in] pszWriterClass string identifier for the file (ASCII, currently "stl" and "3mf")
		* @param[out] ppWriter returns the writer instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_querywriter(_In_ PLib3MFModel * pModel, _In_z_ LPCSTR pszWriterClass, _Outptr_ PLib3MFModelWriter ** ppWriter);

		/**
		* creates a model reader instance for a specific file type
		*
		* @param[in] pModel Model instance
		* @param[in] pszWriterClass string identifier for the file (ASCII, currently "stl" and "3mf")
		* @param[out] ppReader returns the reader instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_queryreader(_In_ PLib3MFModel * pModel, _In_z_ LPCSTR pszReaderClass, _Outptr_ PLib3MFModelReader ** ppReader);

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
		* finds a model 2d texture by its id
		*
		* @param[in] pModel Model instance
		* @param[in] nResourceID Resource ID
		* @param[out] ppTexture returns the texture resource instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_gettexture2dbyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelTexture2D ** ppTexture);

		/**
		* finds a base material by its id
		*
		* @param[in] pModel Model instance
		* @param[in] nResourceID Resource ID
		* @param[out] ppMaterial returns the base material resource instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getbasematerialbyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelBaseMaterial ** ppMaterial);

		/**
		* finds a mesh object resource by its id
		*
		* @param[in] pModel Model instance
		* @param[in] nResourceID Resource ID
		* @param[out] ppMeshObject returns the resource instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmeshobjectbyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelMeshObject ** ppMeshObject);

		/**
		* finds a components object resource by its id
		*
		* @param[in] pModel Model instance
		* @param[in] nResourceID Resource ID
		* @param[out] ppComponentsObject returns the resource instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getcomponentsobjectbyid(_In_ PLib3MFModel * pModel, _In_ DWORD nResourceID, _Outptr_ PLib3MFModelComponentsObject ** ppComponentsObject);


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
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getresources(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

		/**
		* creates a resource iterator instance with all object resources
		*
		* @param[in] pModel Model instance
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getobjects(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

		/**
		* creates a resource iterator instance with all mesh object resources
		*
		* @param[in] pModel Model instance
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmeshobjects(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

		/**
		* creates a resource iterator instance with all component object resources
		*
		* @param[in] pModel Model instance
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getcomponentsobjects(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

		/**
		* creates a resource iterator instance with all 2D texture resources
		*
		* @param[in] pModel Model instance
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_get2dtextures(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

		/**
		* creates a resource iterator instance with all base materials
		*
		* @param[in] pModel Model instance
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getbasematerials(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelResourceIterator ** ppIterator);

		/**
		* creates a thumbnail iterator instance with all thumbnails
		*
		* @param[in] pModel Model instance
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getthumbnails(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelThumbnailIterator ** ppIterator);

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
		* adds an empty texture2d resource to the model
		*
		* @param[in] pModel Model instance
		* @param[in] pwszPath Package path of the texture
		* @param[out] ppTextureInstance returns the new texture instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addtexture2d(_In_ PLib3MFModel * pModel, _In_z_ LPCWSTR pwszPath, _Outptr_ PLib3MFModelTexture2D ** ppTextureInstance);

		/**
		* adds an empty texture2d resource to the model (UTF8)
		*
		* @param[in] pModel Model instance
		* @param[in] pszPath Package path of the texture
		* @param[out] ppTextureInstance returns the new texture instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addtexture2dutf8(_In_ PLib3MFModel * pModel, _In_z_ LPCSTR pszPath, _Outptr_ PLib3MFModelTexture2D ** ppTextureInstance);


		/**
		* adds an empty basematerials resource to the model
		*
		* @param[in] pModel Model instance
		* @param[out] ppBaseMaterialInstance returns the new base material instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addbasematerialgroup(_In_ PLib3MFModel * pModel, _Outptr_ PLib3MFModelBaseMaterial ** ppBaseMaterialInstance);


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


		/**
		* removes a build item from the model
		*
		* @param[in] pModel Model instance
		* @param[in] pBuildItem Build item instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_removebuilditem(_In_ PLib3MFModel * pModel, _In_ PLib3MFModelBuildItem * pBuildItem);

		/**
		* returns the number of texture streams of a model
		*
		* @param[in] pModel Model instance
		* @param[out] pnCount returns the number of texture streams.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_gettexturestreamcount(_In_ PLib3MFModel * pModel, _Out_ DWORD * pnCount);


		/**
		* returns the size of a texture stream
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex index of the texture stream
		* @param[out] pnSize returns the size of a texture stream in bytes.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_gettexturestreamsize(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_ UINT64 * pnSize);

		/**
		* Returns the path of a texture stream in the 3mf package.
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the Texture Stream
		* @param[out] pwszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_gettexturestreampath(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Returns the path of a texture stream in the 3mf package. (UTF8)
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the Texture Stream
		* @param[out] pszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_gettexturestreampathutf8(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* returns the number of metadata strings of a model
		*
		* @param[in] pModel Model instance
		* @param[out] pnCount returns the number metadata strings.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmetadatacount(_In_ PLib3MFModel * pModel, _Out_ DWORD * pnCount);

		/**
		* returns a metadata key of a model
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the Metadata
		* @param[out] pwszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmetadatakey(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* returns a metadata key of a model (UTF8)
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the Metadata
		* @param[out] pszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmetadatakeyutf8(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* returns a metadata value of a model
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the Metadata
		* @param[out] pwszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmetadatavalue(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* returns a metadata value of a model (UTF8)
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the Metadata
		* @param[out] pszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getmetadatavalueutf8(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* adds a new metadata to the model
		*
		* @param[in] pModel Model instance
		* @param[in] pszwKey Metadata Key.
		* @param[in] pszwValue Metadata Value.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addmetadata(_In_ PLib3MFModel * pModel, _In_ LPCWSTR pszwKey, _In_ LPCWSTR pszwValue);

		/**
		* adds a new metadata to the model
		*
		* @param[in] pModel Model instance
		* @param[in] pszKey Metadata Key.
		* @param[in] pszValue Metadata Value.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addmetadatautf8(_In_ PLib3MFModel * pModel, _In_ LPCSTR pszKey, _In_ LPCSTR pszValue);

		/**
		* removes a metadata from the model
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the metadata
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_removemetadata(_In_ PLib3MFModel * pModel, _In_ DWORD nIndex);


		/**
		* adds an attachment stream to the model. The OPC part will be related to the model stream with a certain relationship type.
		*
		* @param[in] pModel Model instance
		* @param[in] pwszURI Path of the attachment
		* @param[in] pwszRelationShipType Relationship type of the attachment
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addattachment (_In_ PLib3MFModel * pModel, _In_z_ LPWSTR pwszURI, _In_z_ LPWSTR pwszRelationShipType, _Outptr_ PLib3MFModelAttachment ** ppAttachmentInstance);

		/**
		* adds an attachment stream to the model. The OPC part will be related to the model stream with a certain relationship type.
		*
		* @param[in] pModel Model instance
		* @param[in] pszURI Path of the attachment (UTF8)
		* @param[in] pszRelationShipType Relationship type of the attachment (UTF8)
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addattachmentutf8(_In_ PLib3MFModel * pModel, _In_z_ LPSTR pszURI, _In_z_ LPSTR pszRelationShipType, _Outptr_ PLib3MFModelAttachment ** ppAttachmentInstance);


		/**
		* retrieves an attachment stream object from the model.
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the attachment stream
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
	    LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getattachment (_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Outptr_ PLib3MFModelAttachment ** ppAttachmentInstance);

		/**
		* retrieves an attachment stream object from the model.
		*
		* @param[in] pModel Model instance
		* @param[in] pwszURI Path URI in the package
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_findattachment (_In_ PLib3MFModel * pModel, _In_z_ LPWSTR pwszURI, _Outptr_ PLib3MFModelAttachment ** ppAttachmentInstance);

		/**
		* retrieves an attachment stream object from the model.
		*
		* @param[in] pModel Model instance
		* @param[in] pszURI Path URI in the package (UTF8)
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_findattachmentutf8 (_In_ PLib3MFModel * pModel, _In_z_ LPSTR pszURI, _Outptr_ PLib3MFModelAttachment ** ppAttachmentInstance);

		/**
		* retrieves the number of attachments of the model.
		*
		* @param[in] pModel Model instance
		* @param[out] pnCount Returns the number of attachments
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getattachmentcount (_In_ PLib3MFModel * pModel, _Out_ DWORD * pnCount);

		/**
		* retrieves the size of an attachment in bytes
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the attachment stream
		* @param[out] pnSize Returns the size of the attachment
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getattachmentsize (_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_ UINT64 * pnSize);

		/**
		* retrieves the path URI of an attachment
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the attachment stream
		* @param[out] pwszBuffer Buffer to write into, may be null to determine needed length
		* @param[in] cbBufferSize Size of the given buffer
		* @param[out] pcbNeededChars Returns number of bytes written or number of bytes needed to write.

		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getattachmentpath (_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* retrieves the path URI of an attachment (UTF8)
		*
		* @param[in] pModel Model instance
		* @param[in] nIndex Index of the attachment stream
		* @param[out] pszBuffer Buffer to write into, may be null to determine needed length
		* @param[in] cbBufferSize Size of the given buffer
		* @param[out] pcbNeededChars Returns number of bytes written or number of bytes needed to write.

		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getattachmentpathutf8 (_In_ PLib3MFModel * pModel, _In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* adds a new Content Type to the model
		*
		* @param[in] pModel Model instance
		* @param[in] pszwExtension File Extension
		* @param[in] pszwContentType Content Type Identifier
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addcustomcontenttype(_In_ PLib3MFModel * pModel, _In_ LPCWSTR pszwExtension, _In_ LPCWSTR pszwContentType);

		/**
		* adds a new Content Type to the model (UTF8 version)
		*
		* @param[in] pModel Model instance
		* @param[in] pszExtension File Extension
		* @param[in] pszContentType Content Type Identifier
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addcustomcontenttypeutf8(_In_ PLib3MFModel * pModel, _In_ LPCSTR pszExtension, _In_ LPCSTR pszContentType);

		/**
		* removes a custom Content Type from the model
		*
		* @param[in] pModel Model instance
		* @param[in] pszwExtension File Extension
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_removecustomcontenttype(_In_ PLib3MFModel * pModel, _In_ LPCWSTR pszwExtension);

		/**
		* removes a custom Content Type from the model (UTF8 version)
		*
		* @param[in] pModel Model instance
		* @param[in] pszExtension File Extension
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_removecustomcontenttypeutf8(_In_ PLib3MFModel * pModel, _In_ LPCSTR pszExtension);

	};

};

#endif //__NMR_DLLINTERFACES
