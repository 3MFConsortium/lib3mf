/*++

Copyright (C) 2018 3MF Consortium

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

#ifndef NMR_COM_NATIVE
#include "Common/Platform/NMR_COM_Emulation.h"
#else
#include "Common/Platform/NMR_COM_Native.h"
#endif

#include "Common/Platform/NMR_SAL.h"
#include "Common/Platform/NMR_WinTypes.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Common/3MF_ProgressTypes.h"
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
		typedef PLib3MFBase PLib3MFModelTextureAttachment;
		typedef PLib3MFBase PLib3MFModelTexture2D;
		typedef PLib3MFBase PLib3MFModelAttachment;
		typedef PLib3MFBase PLib3MFModelMeshBeamSet;
		typedef PLib3MFBase PLib3MFSlice;
		typedef PLib3MFBase PLib3MFSliceStack;
		typedef PLib3MFBase PLib3mfSlicePolygon;
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
		* this version will increment with each release of the library, and should
		* be used to ensure API compatibility.
		* 
		* Lib3MF's API is stable in the following way:
		* - Lib3MF-libraries with different major version are not guaranteed to be compatible
		* - Lib3MF-libraries with matching major version and different minor versions are backwards compatible:
		*   A consumer who compiled against the API of version A.B can safely use the binary of version A.C for C>=B.
		*   I.e., functions will not be removed within the range of a constant major version.
		* - Lib3MF-libraries with matching values of both major and minor version have an identical API.
		* 
		* @param[out] pInterfaceVersionMajor returns the major version of the shared library
		* @param[out] pInterfaceVersionMinor returns the minor version of the shared library
		* @param[out] pInterfaceVersionMicro returns the patch version of the shared library
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_getinterfaceversion(_Out_ DWORD * pInterfaceVersionMajor, _Out_ DWORD * pInterfaceVersionMinor, _Out_ DWORD * pInterfaceVersionMicro);

		/**
		* checks whether a extension is supported by the DLL and which version of the interface is used
		* This extension version will increment with each change of the API of the extension
		* and may be used to ensure API compatibility.
		*
		* @param[in] pwszExtensionUrl URL of extension to check
		* @param[out] pbIsSupported returns whether the extension is supported or not
		* @param[out] pExtensionInterfaceVersion returns the interface version of of the extensions (if extension is supported)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_queryextension(_In_z_ LPCWSTR pwszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion);

		/**
		* checks whether a extension is supported by the DLL and which version of the interface is used
		* This extension version will increment with each change of the API of the extension
		* and may be used to ensure API compatibility.
		*
		* @param[in] pszExtensionUrl URL of extension to check as UTF8 string
		* @param[out] pbIsSupported returns whether the extension is supported or not
		* @param[out] pExtensionInterfaceVersion returns the interface version of of the extensions (if extension is supported)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_queryextensionutf8(_In_z_ LPCSTR pszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion);

		/**
		* Return an English text for a progress identifier
		* Note: this is the only function you can call from your callback function.
		*
		* @param[in] progressIdentifier the progress identifier that is passed to the callback function
		* @param[out] progressMessage English text for the progress identifier
		* @return error code or 0 (success)
		**/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_retrieveprogressmessage(_In_ ProgressIdentifier progressIdentifier, _Out_ LPCSTR * progressMessage);

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
		* @param[in] pszFilename Filename to write into as UTF8 string
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_writer_writetofileutf8(_In_ PLib3MFModelWriter * pWriter, _In_z_ LPCSTR pszFilename);

		/**
		* Retrieves the size of the 3MF stream.
		*
		* @param[in] pWriter Writer Instance
		* @param[out] pcbStreamSize Returns the stream size
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_writer_getstreamsize(_In_ PLib3MFModelWriter * pWriter, _Out_ ULONG64 * pcbStreamSize);

		/**
		* Writes out the 3mf into a buffer. Buffer size must be at least the size of the stream.
		*
		* @param[in] pWriter 3MF Writer Instance
		* @param[out] pBuffer Buffer to write into
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_writer_writetobuffer(_In_ PLib3MFModelWriter * pWriter, _Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);

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
		* Set the progress callback for calls to this writer
		* 
		* @param[in] pWriter Writer Instance
		* @param[in] callback pointer to the callback function. If the callback returns
		*            "false" the original function call will be aborted and set the error to
		*            NMR_USERABORTED.
		* @param[in] userData pointer to arbitrary user data that is passed without modification
		*            to the callback.
		* @return error code or 0 (success)
		**/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_writer_setprogresscallback(_In_ PLib3MFModelWriter * pWriter, _In_ Lib3MFProgressCallback callback, void* userData);

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
		* @param[in] pszFilename Filename to read from
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
		* Activates (deactivates) the strict mode of the reader.
		* If active, all warnings are reported as errors. Otherwise, they are reported as warnings. By default, it is deactivated.
		*
		* @param[in] pReader Reader Instance
		* @param[in] bStrictModeActive flag whether strict mode is active or not
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_setstrictmodeactive(_In_ PLib3MFModelReader * pReader, _In_ BOOL bStrictModeActive);

		/**
		* Queries whether the strict mode of the reader is active or not
		*
		* @param[in] pReader Reader Instance
		* @param[out] pbStrictModeActive flag whether strict mode is active or not
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_getstrictmodeactive(_In_ PLib3MFModelReader * pReader, _In_ BOOL *pbStrictModeActive);

		/**
		* Reads a model from a memory buffer. The file type is specified by the Model Read class
		*
		* @param[in] pReader Reader Instance
		* @param[in] pBuffer Buffer to read from
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_readfrombuffer(_In_ PLib3MFModelReader * pReader, _In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);

		/**
		* Reads a model and from the data provided by a callback function. The file type is specified by the Model Writer class
		*
		* @param[in] pReader Reader Instance
		* @param[in] pReadCallback Callback to call for reading a data chunk.
		* @param[in] nStreamSize number of bytes the callback returns
		* @param[in] pSeekCallback Callback to call for seeking in the stream.
		* @param[in] pUserData Userdata that is passed to the callback function
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_readfromcallback(_In_ PLib3MFModelReader * pReader, _In_ ULONG64 nStreamSize, _In_ void * pReadCallback, _In_opt_ void * pSeekCallback, _In_opt_ void * pUserData);

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

		/**
		* Returns Warning and Error Information of the read process (UTF8)
		*
		* @param[in] pReader Reader Instance
		* @param[in] nIndex Index of the Warning. Valid values are 0 to WarningCount - 1
		* @param[out] pErrorCode filled with the error code of the warning
		* @param[out] pszBuffer filled with the error message, may be NULL
		* @param[in] cbBufferSize size of pszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_getwarningutf8(_In_ PLib3MFModelReader * pReader, _In_ DWORD nIndex, _Out_ DWORD * pErrorCode, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Set the progress callback for calls to this reader
		*
		* @param[in] pReader Reader Instance
		* @param[in] callback pointer to the callback function. If the callback returns
		*            "false" the original function call will be aborted and set the error to
		*            NMR_USERABORTED.
		* @param[in] userData pointer to arbitrary user data that is passed without modification
		*            to the callback.
		* @return error code or 0 (success)
		**/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_reader_setprogresscallback(_In_ PLib3MFModelReader * pReader, _In_ Lib3MFProgressCallback callback, void* userData);

		// Resources
		/**
		* Retrieves the ID of a Model Resource Instance

		*@param[in] pResource Resource Instance
		* @param[out] pnResourceID Filled with the ID of the Resource Instance
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
		* @param[in] bRed New red value of the color of the triangle (0-255)
		* @param[in] bGreen New red value of the color of the triangle (0-255)
		* @param[in] bBlue New red value of the color of the triangle (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setsinglecolorrgb(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue);

		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] bRed New red value of the color of the triangle (0-255)
		* @param[in] bGreen New red value of the color of the triangle (0-255)
		* @param[in] bBlue New red value of the color of the triangle (0-255)
		* @param[in] bAlpha New alpha value of the color of the triangle (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setsinglecolorrgba(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha);

		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] fRed New red value of the color of the triangle (0-1)
		* @param[in] fGreen New red value of the color of the triangle (0-1)
		* @param[in] fBlue New red value of the color of the triangle (0-1)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_propertyhandler_setsinglecolorfloatrgb(_In_ PLib3MFPropertyHandler * pPropertyHandler, _In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue);


		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] fRed New red value of the color of the triangle (0-1)
		* @param[in] fGreen New red value of the color of the triangle (0-1)
		* @param[in] fBlue New red value of the color of the triangle (0-1)
		* @param[in] fAlpha New alpha value of the color of the triangle (0-1)
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
		* @param[in] pTextures new UV texture values of the three nodes of all triangles. Must have at least trianglecount array entries.
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
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
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
		* @param[in] bRed New red value of the color of the object (0-255)
		* @param[in] bGreen New red value of the color of the object (0-255)
		* @param[in] bBlue New red value of the color of the object (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_setcolorrgb(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue);

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] bRed New red value of the color of the object (0-255)
		* @param[in] bGreen New red value of the color of the object (0-255)
		* @param[in] bBlue New red value of the color of the object (0-255)
		* @param[in] bAlpha New alpha value of the color of the object (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_setcolorrgba(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha);

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] fRed New red value of the color of the object (0-1)
		* @param[in] fGreen New red value of the color of the object (0-1)
		* @param[in] fBlue New red value of the color of the object (0-1)
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_defaultpropertyhandler_setfloatcolorrgb(_In_ PLib3MFDefaultPropertyHandler * pPropertyHandler, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue);

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pPropertyHandler Property Handler
		* @param[in] fRed New red value of the color of the object (0-1)
		* @param[in] fGreen New red value of the color of the object (0-1)
		* @param[in] fBlue New red value of the color of the object (0-1)
		* @param[in] fAlpha New alpha value of the color of the object (0-1)
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
		* @param[in] pszPartNumber new part number (UTF8) string for referencing parts from the outside.
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
		* Retrieves the path used as thumbnail for an object (UTF8). Returns "" if none is set
		*
		* @param[in] pObject Object Resource Instance
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_getthumbnailpathutf8(_In_ PLib3MFModelObjectResource * pObject, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		
		/**
		* Sets an object's thumbnail package path (UTF8)
		*
		* @param[in] pObject Object Instance
		* @param[in] pszPath path where to look for the thumbnail (e.g. "/Textures/thumbnail.png"). Call will NULL to clear the thumbnail.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setthumbnailpathutf8(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCSTR pszPath);

		/**
		* returns, whether an object has a UUID and, if true, the object's UUID
		*
		* @param[in] pObject object instance
		* @param[out] pbHasUUID flag whether the object has a UUID
		* @param[out] pszBuffer the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_getuuidutf8(_In_ PLib3MFModelObjectResource * pMeshObject, _Out_ BOOL *pbHasUUID, _Out_ LPSTR pszBuffer);
		
		/**
		* sets the objects's UUID
		*
		* @param[in] pObject object instance
		* @param[in] pszUUID the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_object_setuuidutf8(_In_ PLib3MFModelObjectResource * pObject, _In_z_ LPCSTR pszUUID);

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
		* @param[in] pszName new name of the base material. (UTF8 String, e.g. "ABS red")
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
		* @param[in] pszName new name of the base material. (e.g. "ABS red")
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
		* @param[in] pBaseMaterial Base Material Resource Instance
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
		* Retrieves an attachment's package path
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_getpath (_In_ PLib3MFModelAttachment * pAttachment, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Retrieves an attachment's package path (UTF8)
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_getpathutf8 (_In_ PLib3MFModelAttachment * pAttachment, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Sets an attachment's package path
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pwszPath new path of the attachment. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_setpath (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCWSTR pwszPath);

		/**
		* Sets an attachment's package path (UTF8)
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pszPath new path of the attachment. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_setpathutf8 (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCSTR pszPath);

		/**
		* Retrieves an attachment's package relationship type
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_getrelationshiptype(_In_ PLib3MFModelAttachment * pAttachment, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Retrieves an attachment's package relationship type (UTF8)
		*
		* @param[in] pAttachment Attachment Instance
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_getrelationshiptypeutf8 (_In_ PLib3MFModelAttachment * pAttachment, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);

		/**
		* Sets an attachment's package relationship type
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pwszRelationShipType new relationship type attachment. (e.g. "/Data/data.xml")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_setrelationshiptype (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCWSTR pwszRelationShipType);

		/**
		* Sets an attachment's package relationship type (UTF8)
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
		* Reads an attachment from a file.
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pwszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_readfromfile (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCWSTR pwszFilename);

		/**
		* Reads an attachment from a file.
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_readfromfileutf8 (_In_ PLib3MFModelAttachment * pAttachment, _In_z_ LPCSTR pszFilename);

		/**
		* Reads an attachment from a memory buffer.
		*
		* @param[in] pAttachment Attachment Instance
		* @param[in] pBuffer Buffer to read from
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_attachment_readfrombuffer (_In_ PLib3MFModelAttachment * pAttachment, _In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);

		/**
		* Retrieves the attachment located at the path of the texture
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] ppTextureAttachment attachment that holds the texture's image information
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_getattachment(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_ PLib3MFModelAttachment ** ppTextureAttachment);

		/**
		* Sets the texture's package path to the path of the attachment
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] pTextureAttachment attachment that holds the texture's image information
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_setattachment(_In_ PLib3MFModelTexture2D * pTexture2D, _In_ PLib3MFModelAttachment * pTextureAttachment);

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
		* Retrieves a texture's tilestyle type
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] peTileStyleU returns tilestyle type enum
		* @param[out] peTileStyleV returns tilestyle type enum
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_gettilestyleuv(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_ eModelTextureTileStyle * peTileStyleU, _Out_ eModelTextureTileStyle * peTileStyleV);

		/**
		* Sets a texture's tilestyle type
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] eTileStyleU new tilestyle type enum
		* @param[in] eTileStyleV new tilestyle type enum
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_settilestyleuv(_In_ PLib3MFModelTexture2D * pTexture2D, _In_ eModelTextureTileStyle eTileStyleU, _In_ eModelTextureTileStyle eTileStyleV);

		/**
		* Retrieves a texture's filter
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] peFilter returns filter enum
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_getfilter(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_ eModelTextureFilter * peFilter);

		/**
		* Sets a texture's filter type
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] eFilter returns filter enum
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_setfilter(_In_ PLib3MFModelTexture2D * pTexture2D, _In_ eModelTextureFilter eFilter);

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
		LIB3MFDEPRECATED( LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_getstreamsize(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_ ULONG64 * pcbStreamSize) );

		/**
		* Writes out the texture as file.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pwszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MFDEPRECATED(LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_writetofile(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCWSTR pwszFilename) );

		/**
		* Writes out the texture as file. (UTF8)
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MFDEPRECATED(LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_writetofileutf8(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCSTR pszFilename) );

		/**
		* Writes out the texture into a buffer. Buffer size must be at least the size of the stream.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[out] pBuffer Buffer to write into
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MFDEPRECATED(LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_writetobuffer(_In_ PLib3MFModelTexture2D * pTexture2D, _Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize) );

		/**
		* Writes out the texture and passes the data to a provided callback function. The file type is specified by the Model Writer class
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pWriteCallback Callback to call for writing a data chunk.
		* @param[in] pUserData Userdata that is passed to the callback function
		* @return error code or 0 (success)
		*/
		LIB3MFDEPRECATED(LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_writetocallback(_In_ PLib3MFModelTexture2D * pTexture2D, _In_ void * pWriteCallback, _In_opt_ void * pUserData) );

		/**
		* Reads a texture from a file.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pwszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MFDEPRECATED(LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_readfromfile(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCWSTR pwszFilename) );

		/**
		* Reads a texture from a file.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MFDEPRECATED(LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_readfromfileutf8(_In_ PLib3MFModelTexture2D * pTexture2D, _In_z_ LPCSTR pszFilename) );

		/**
		* Reads a texture from a memory buffer.
		*
		* @param[in] pTexture2D Texture2D Resource Instance
		* @param[in] pBuffer Buffer to read from
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MFDEPRECATED(LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_texture2d_readfrombuffer(_In_ PLib3MFModelTexture2D * pTexture2D, _In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize) );


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

		// API for beamlattice extension
		/**
		* Returns the minimal length of beams for the beamlattice
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] pdMinLength minimal length of beams for the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeamlattice_minlength(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ DOUBLE *pdMinLength);

		/**
		* Sets the minimal length of beams for the beamlattice
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] dMinLength minimal length of beams for the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setbeamlattice_minlength(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DOUBLE dMinLength);

		/**
		* Returns the default radius for the beamlattice
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] pdRadius precission of the beams in the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeamlattice_radius(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ DOUBLE *pdRadius);

		/**
		* Sets the default radius for the beamlattice
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] dRadius default radius of the beams in the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setbeamlattice_radius(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DOUBLE dRadius);

		/**
		* Returns the default capping mode for the beamlattice
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] peCapMode default eModelBeamLatticeCapMode of the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeamlattice_capmode(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ eModelBeamLatticeCapMode *peCapMode);
		
		/**
		* Sets the default capping mode for the beamlattice
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] eCapMode default eModelBeamLatticeCapMode of the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setbeamlattice_capmode(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ eModelBeamLatticeCapMode eCapMode);
		
		/**
		* Returns the clipping mode and the clipping-mesh for the beamlattice of this mesh
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] pClipMode contains the clip mode of this mesh
		* @param[out] pnResourceID filled with the resourceID of the clipping mesh-object or an undefined value if pClipMode is MODELBEAMLATTICECLIPMODE_NONE
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeamlattice_clipping(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ eModelBeamLatticeClipMode * peClipMode, _Out_ DWORD *pnResourceID);

		/**
		* Sets the clipping mode and the clipping-mesh for the beamlattice of this mesh
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] eClipMode contains the clip mode of this mesh
		* @param[in] nResourceID the resourceID of the clipping mesh-object. This mesh-object has to be defined before setting the Clipping
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setbeamlattice_clipping(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ eModelBeamLatticeClipMode eClipMode, _In_ DWORD nResourceID);

		/**
		* Returns the representation-mesh for the beamlattice of this mesh
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] pbHasRepresentation flag whether the beamlattice has a representation mesh.
		* @param[out] pnResourceID filled with the resourceID of the clipping mesh-object.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeamlattice_representation(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ BOOL *pbHasRepresentation,  _Out_ DWORD *pnResourceID);

		/**
		* Sets the representation-mesh for the beamlattice of this mesh
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] nResourceID the resourceID of the representation mesh-object. This mesh-object has to be defined before setting the representation.
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setbeamlattice_representation(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nResourceID);
		/**
		* Returns the beam count of a mesh object
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] pnBeamCount filled with the beam count
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeamcount(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ DWORD * pnBeamCount);

		/**
		* Returns indices, radii and capmodes of a single beam of a mesh object
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] nIndex Index of the beam (0 to beamcount - 1)
		* @param[out] pBeam filled with the beam indices, radii and capmodes
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeam(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _Out_ MODELMESHBEAM * pBeam);

		/**
		* Adds a single beam to a mesh object
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] pBeam contains the node indices, radii and capmodes
		* @param[out] pnIndex filled with the new Index of the beam
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_addbeam(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ MODELMESHBEAM * pBeam, _Out_opt_ DWORD * pnIndex);
		
		/**
		* Sets the indices, radii and capmodes of a single beam of a mesh object
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] nIndex Index of the beam (0 to beamcount - 1)
		* @param[in] pBeam contains the node indices, radii and capmodes
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setbeam(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _In_ MODELMESHBEAM * pBeam);
		
		/**
		* Sets all beam indices, radii and capmodes of a mesh object
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] pIndices buffer with the beam indices
		* @param[in] nBufferSize size of the buffer in elements
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setbeamindices(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ MODELMESHBEAM * pIndices, _In_ DWORD nBufferSize);

		/**
		* Retrieves all beam indices of a mesh object
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] pIndices buffer filled with the beam indices
		* @param[in] nBufferSize size of the buffer in elements, must be at least beam count
		* @param[out] pnBeamCount returns how many beams have been written
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeamindices(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ MODELMESHBEAM * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnBeamCount);

		/**
		* Returns the number of beamsets of a mesh object
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] pnCount filled with the beamset count
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeamsetcount(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ DWORD * pnCount);
		
		/**
		* Adds an empty beamset to a mesh object
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[out] ppBeamSet pointer to the new beamset
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_addbeamset(_In_ PLib3MFModelMeshObject * pMeshObject, _Out_ PLib3MFModelMeshBeamSet** ppBeamSet);
		
		/**
		* Returns a beamset of a mesh object
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] nIndex index of the requested beamset (0 ... beamsetcount-1)
		* @param[out] ppBeamSet pointer to the requested beamset
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getbeamset(_In_ PLib3MFModelMeshObject * pMeshObject, _In_ DWORD nIndex, _Out_ PLib3MFModelMeshBeamSet** ppBeamSet);

		/**
		* Sets a beamset's name string
		*
		* @param[in] pModelMeshBeamSet BeamSet Instance
		* @param[in] pwszName new name of the BeamSet as UTF16 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_beamset_setname(_In_ PLib3MFModelMeshBeamSet * pModelMeshBeamSet, _In_z_ LPCWSTR pwszName);
		
		/**
		* Sets a beamset's identifier string
		*
		* @param[in] pModelMeshBeamSet BeamSet Instance
		* @param[in] pwszIdentifier new identifier of the BeamSet as UTF16 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_beamset_setidentifier(_In_ PLib3MFModelMeshBeamSet * pModelMeshBeamSet, _In_z_ LPCWSTR pwszIdentifier);
		
		/**
		* Sets a beamset's name string
		*
		* @param[in] pModelMeshBeamSet BeamSet Instance
		* @param[in] pwszName new name of the BeamSet as UTF8 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_beamset_setnameutf8(_In_ PLib3MFModelMeshBeamSet * pModelMeshBeamSet, _In_z_ LPCSTR pszName);
		
		/**
		* Sets a beamset's identifier string
		*
		* @param[in] pModelMeshBeamSet BeamSet Instance
		* @param[in] pszIdentifier new id of the BeamSet as UTF8 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_beamset_setidentifierutf8(_In_ PLib3MFModelMeshBeamSet * pModelMeshBeamSet, _In_z_ LPCSTR pszIdentifier);
		
		/**
		* Retrieves a BeamSet's name string (UTF8)
		*
		* @param[in] pModelMeshBeamSet BeamSet Instance
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_beamset_getnameutf8(_In_ PLib3MFModelMeshBeamSet * pModelMeshBeamSet, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		
		/**
		* Retrieves a BeamSet's identifier string (UTF8)
		*
		* @param[in] pModelMeshBeamSet BeamSet Instance
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_beamset_getidentifierutf8(_In_ PLib3MFModelMeshBeamSet * pModelMeshBeamSet, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		
		/**
		* Retrieves the reference count of a BeamSet
		*
		* @param[in] pBeamSet beamset instance
		* @param[out] pnCount returns the reference count
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_beamset_getrefcount(_In_ PLib3MFModelMeshBeamSet * pBeamSet, _Out_ DWORD * pnCount);
		
		/**
		* Sets the references in a BeamSet
		*
		* @param[in] pBeamSet beamset instance
		* @param[in] pRefs buffer filled with beam references (indices of beams)
		* @param[in] nRefCount number of references to be set
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_beamset_setrefs(_In_ PLib3MFModelMeshBeamSet * pBeamSet, _In_ DWORD * pRefs, _In_ DWORD nRefCount);
		
		/**
		* Retrieves all references of a BeamSet
		*
		* @param[in] pBeamSet beamset instance
		* @param[in] pRefs buffer filled with beam references (indices of beams)
		* @param[in] nBufferSize size of the buffer in elements, must be at least refcount
		* @param[out] pnRefCount returns how many references have been written
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_beamset_getrefs(_In_ PLib3MFModelMeshBeamSet * pBeamSet, _Out_ DWORD * pRefs, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnRefCount);
		
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
		* @param[in] nChannel Channel Index
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

		/**
		* Link a slicestack to the mesh object
		*
		* @param[in] pMeshObject mesh object to link with the slicestack
		* @param[in] pSliceStack slice stack to link the meshobject to
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setslicestack(_In_ PLib3MFModelMeshObject *pMeshObject, _In_ PLib3MFSliceStack *pSliceStack);

		/**
		* Get the linked slicestack tof a the mesh object
		*
		* @param[in] pMeshObject mesh object to link with the slicestack
		* @param[out] pSliceStack slice stack to link the meshobject to
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getslicestackid(_In_ PLib3MFModelMeshObject *pMeshObject, _Out_ DWORD *pSliceStackId);

		/**
		* Set the mesh resolution of a mesh that has sliceinformation
		*
		* @param[in] pMeshObject mesh object for which the meshresolution is set
		* @param[in] eSlicesMeshResolution mesh resolution of the mesh object
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_setslicesmeshresolution(_In_ PLib3MFModelMeshObject *pMeshObject, _In_ eModelSlicesMeshResolution eSlicesMeshResolution);

		/**
		* Get the mesh resolution of a mesh that has sliceinformation
		*
		* @param[in] pMeshObject mesh object for which the meshresolution is queried
		* @param[out] peSlicesMeshResolution mesh resolution of the mesh object
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_meshobject_getslicesmeshresolution(_In_ PLib3MFModelMeshObject *pMeshObject, _Out_ eModelSlicesMeshResolution *eSlicesMeshResolution);

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
		* returns, whether a component has a UUID and, if true, the component's UUID
		*
		* @param[in] pBuildItem component instance
		* @param[out] pbHasUUID flag whether the build item has a UUID
		* @param[out] pszBuffer the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_getuuidutf8(_In_ PLib3MFModelComponent * pComponent, _Out_ BOOL *pbHasUUID, _Out_ LPSTR pszBuffer);

		/**
		* sets the component's UUID
		*
		* @param[in] pComponent component instance
		* @param[in] pszUUID the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_component_setuuidutf8(_In_ PLib3MFModelComponent * pComponent, _In_z_ LPCSTR pszUUID);

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

		/**
		* Link a slicestack to the components object
		*
		* @param[in] pComponentsObject components object to link with the slicestack
		* @param[in] pSliceStack slice stack to link the meshobject to
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_setslicestack(_In_ PLib3MFModelComponentsObject *pComponentsObject, _In_ PLib3MFSliceStack *pSliceStack);

		/**
		* Get the linked slicestack tof a the components object
		*
		* @param[in] pComponentsObject components object to link with the slicestack
		* @param[out] pSliceStack slice stack to link the meshobject to
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_getslicestackid(_In_ PLib3MFModelComponentsObject *pComponentsObject, _Out_ DWORD *pSliceStackId);

		/**
		* Set the mesh resolution of a components object that has sliceinformation
		*
		* @param[in] pComponentsObject components object for which the meshresolution is set
		* @param[in] eSlicesMeshResolution mesh resolution of the components object
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_setslicesmeshresolution(_In_ PLib3MFModelComponentsObject *pComponentsObject, _In_ eModelSlicesMeshResolution eSlicesMeshResolution);

		/**
		* Get the mesh resolution of a components object that has sliceinformation
		*
		* @param[in] pComponentsObject components object for which the meshresolution is queried
		* @param[out] peSlicesMeshResolution mesh resolution of the components object
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_componentsobject_getslicesmeshresolution(_In_ PLib3MFModelComponentsObject *pComponentsObject, _Out_ eModelSlicesMeshResolution *eSlicesMeshResolution);

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
		* returns, whether a build item has a UUID and, if true, the build item's UUID
		*
		* @param[in] pBuildItem build item instance
		* @param[out] pbHasUUID flag whether the build item has a UUID
		* @param[out] pszBuffer the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_getuuidutf8(_In_ PLib3MFModelBuildItem * pBuildItem, _Out_ BOOL *pbHasUUID, _Out_ LPSTR pszBuffer);

		/**
		* sets the build item's UUID
		*
		* @param[in] pBuildItem build item instance
		* @param[in] pszUUID the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_builditem_setuuidutf8(_In_ PLib3MFModelBuildItem * pBuildItem, _In_z_ LPCSTR pszUUID);

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
		* @param[out] pHandle returns the handle
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
		* @param[in] pszReaderClass string identifier for the file (ASCII, currently "stl" and "3mf")
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


		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getslicestackById(_In_ PLib3MFModel *pModel, _In_ DWORD nSliceStackId, _Out_ PLib3MFSliceStack **ppSliceStack);

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
		* returns, whether the build has a UUID and, if true, the build's UUID 
		*
		* @param[in] pModel Model instance
		* @param[out] pbHasUUID flag whether the build has a UUID
		* @param[out] pszBuffer the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getbuilduuidutf8(_In_ PLib3MFModel * pModel, _Out_ BOOL *pbHasUUID, _Out_ LPSTR pszBuffer);

		/**
		* sets the build's UUID
		*
		* @param[in] pModel Model instance
		* @param[in] pszBuffer the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_setbuilduuidutf8(_In_ PLib3MFModel * pModel, _In_z_ LPCSTR pszBuildUUID);

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
		* * adds a texture2d resource to the model. Its path is given by that of an existing attachment.
		*
		* @param[in] pModel Model instance
		* @param[in] pTextureAttachment attachment containing the image data
		* @param[out] ppTextureInstance returns the new texture instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addtexture2dfromattachment(_In_ PLib3MFModel * pModel, _In_ PLib3MFModelAttachment pTextureAttachment, _Outptr_ PLib3MFModelTexture2D ** ppTextureInstance);

		/**
		* adds a texture2d resource to the model
		*
		* @param[in] pModel Model instance
		* @param[in] pwszPath Package path of the texture
		* @param[out] ppTextureInstance returns the new texture instance
		* @return error code or 0 (success)
		*/
		LIB3MFDEPRECATED(LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addtexture2d(_In_ PLib3MFModel * pModel, _In_z_ LPCWSTR pwszPath, _Outptr_ PLib3MFModelTexture2D ** ppTextureInstance) );

		/**
		* adds a texture2d resource to the model (UTF8)
		*
		* @param[in] pModel Model instance
		* @param[in] pszPath Package path of the texture
		* @param[out] ppTextureInstance returns the new texture instance
		* @return error code or 0 (success)
		*/
		LIB3MFDEPRECATED(LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addtexture2dutf8(_In_ PLib3MFModel * pModel, _In_z_ LPCSTR pszPath, _Outptr_ PLib3MFModelTexture2D ** ppTextureInstance) );


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
		* @param[out] pwszBuffer filled with the key of the Metadata
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
		* @param[out] pszBuffer filled with the key of the Metadata
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
		* @param[out] pwszBuffer filled with the value of the Metadata
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
		* @param[out] pszBuffer filled with the value of the Metadata
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
		* Get the attachment to the OPC package containing the package thumbnail
		*
		* @param[in] pModel Model instance
		* @param[in] bCreateIfNotExisting create the attachment if it does not exist
		* @param[out] ppAttachmentInstance Instance of the thumbnailattachment object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_getpackagethumbnailattachment(_In_ PLib3MFModel * pModel, _In_ BOOL bCreateIfNotExisting, _Outptr_ PLib3MFModelAttachment ** ppAttachmentInstance);

		/**
		* Remove the attachment to the OPC package containing the package thumbnail
		*
		* @param[in] pModel Model instance
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_removepackagethumbnailattachment(_In_ PLib3MFModel * pModel);

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

		/**
		* Adds a slicestack to a model
		* @param[in] pModel Model instance to add slicestack to
		* @param[in] nBottomZ Bottom Z value of the slicestack
		* @param[out] ppSliceStackObject returns the new slice stack object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_model_addslicestack(_In_ PLib3MFModel * pModel, _In_ float nBottomZ, PLib3MFSliceStack **ppSliceStackObject);

		/**
		* Adds a slice to a slicestack
		* @param[in] pStack the slicestack to add the slice to
		* @param[in] nTopZ upper Z value of the slice
		* @param[out] pSliceObject returns the new slice object
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slicestack_addslice(_In_ PLib3MFSliceStack *pStack, _In_ float nTopZ, PLib3MFSlice **pSliceObject);

		/**
		* Returns the number of slices stored in a slicestack
		* @param[in] pStack slicestack to query
		* @param[out] pnSliceCount returns the number of slices on the stack
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slicestack_getslicecount(_In_ PLib3MFSliceStack *pStack, _Out_ DWORD *pnSliceCount);

		/**
		* Query a slice of a slicestack
		* @param[in] pStack slicestack to get the slice from
		* @param[in] nSliceIndex index of the slice (0 to slicecount - 1)
		* @param[out] pSlice returns the slice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slicestack_getslice(_In_ PLib3MFSliceStack *pStack, _In_ DWORD nSliceIndex, _Out_ PLib3MFSlice **pSlice);

		/**
		* Specify whether the slice stack should be stored in a separate model file within the 3MF-file as a slice ref
		* @param[in] pStack slicestack to set the reference
		* @param[in] bUsesSliceRef flag whether to use a slice-ref or not
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slicestack_setusessliceref(_In_ PLib3MFSliceStack *pStack, _In_ BOOL bUsesSliceRef);

		/**
		* Returns the slice reference of a slicestack, i.e. the slicestack is stored in a seperate entity within the 3mf file
		* Get the specification, whether the slice stack should be stored in a separate model file within the 3MF-file as a slice ref
		* @param[in] pStack the slice stack
		* @param[out] pbUsesSliceRef string for the slice reference
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slicestack_getusessliceref(_In_ PLib3MFSliceStack *pStack, _Out_ BOOL *pbUsesSliceRef);

		/**
		* Returns the top Z of a slice
		* @param[in] pSlice the slice to query
		* @param[out] nTopZ returns the upper Z value of the slice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_gettopz(_In_ PLib3MFSlice *pSlice, _Out_ float *pTopZ);

		/**
		* Add an array of vertices to a slice
		* @param[in] pSlice the slice to add vertices to
		* @param[in] pVertices array of MODELSLICEVERTEX structures
		* @param[in] nCount number of MODELSLICEVERTEX strucutres in the array
		* @param[out] returns the start index of the slice, i.e. for the first vertex array this is "0" but later calls to the function append the vertices to the internal array so the value returned here must be added as an offset if accessing the vertices
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_addvertices(_In_ PLib3MFSlice *pSlice, _In_ MODELSLICEVERTEX *pVertices, _In_ DWORD nCount, _Out_ DWORD *pStartIndex);

		/**
		* Add a polygon to a slice
		* @param[in] pSlice the slice to add the polygon to
		* @param[in] pPolygonIndices an array of indices refering the slice vertices. If more than one call to "lib3mf_addvertices" was done the offset (pStartIndex) must be considered when adding a polygon
		* @param[out] pPolygonIndex returns index of the newly created polygon
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_addpolygon(_In_ PLib3MFSlice *pSlice, _In_ DWORD *pPolygonIndices, _In_ DWORD nCount, _Out_ DWORD *pPolygonIndex);

		/**
		* Add a new (empty) polygon to a slice
		* @param[in] pSlice the slice to add the polygon to
		* @param[out] pPolygonIndex returns index of the newly created polygon
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_startpolygon(_In_ PLib3MFSlice *pSlice, _Out_ DWORD *pPolygonIndex);

		/**
		* Add a vertex index to a polygon created with lib3mf_slice_startpolygon
		* @param[in] pSlice the slice with the polygon
		* @param[in] nPolygon the polygon index
		* @param[in] nIndex the vertex index to add to the polygon
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_addindextopolygon(_In_ PLib3MFSlice *pSlice, _In_ DWORD nPolygon, _In_ DWORD nIndex);

		/**
		* Returns the number of vertices in a slice
		* @param[in] pSlice the slice to obtain the number of vertices from
		* @param[out] pVertexCount returns the number of vertices in the slice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_getvertexcount(_In_ PLib3MFSlice *pSlice, _Out_ DWORD *pVertexCount);

		/**
		* Returns the number of polgons in a slice
		* @param[in] pSlice the slice to get the number of polygons from
		* @param[out] pPolygonCount returns the number of polygons in a slice
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_getpolygoncount(_In_ PLib3MFSlice *pSlice, _Out_ DWORD *pPolygonCount);

		/**
		* Returns the vertices of a slice
		* @param[in] pSlice the slice to query the vertices from
		* @param[out] pVertices the array of MODELSLICEVERTEX to be filled
		* @param[in] nBufferSize the number of elements in the pVertices array. Should equal the number of vertices in the slice (lib3mf_slice_getvertexcount), if smaller not all vertices are returned, if bigger memory is wasted
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_getvertices(_In_ PLib3MFSlice *pSlice, _Out_ MODELSLICEVERTEX *pVertices, _In_ DWORD nBufferSize);

		/**
		* Returns the number of indices of a polygon in a slice
		* @param[in] pSlice the slice to query
		* @param[in] nPolygonIndex the index of the polygon to query
		* @param[out] npIndexCount returns the number of polygon indices of the polygon
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_getpolygonindexcount(_In_ PLib3MFSlice *pSlice, _In_ DWORD nPolygonIndex, _Out_ DWORD *npIndexCount);

		/**
		* Fills a list of polygon indices with the indices of the desired polygon
		* @param[in] pSlice the slice to query
		* @param[in] nPolygonIndex the index of the polygon
		* @param[out] pIndices an array of polygon indices to be filled
		* @param[in] nBufferSize size of the pIndices array. Should equal the number of indices in the polygon (lib3mf_slice_getpolygonindexcount), if smaller not all indices are returned, if bigger memory is wasted
		* @return error code or 0 (success)
		*/
		LIB3MF_DECLSPEC LIB3MFRESULT lib3mf_slice_getpolygonindices(_In_ PLib3MFSlice *pSlice, _In_ DWORD nPolygonIndex, _Out_ DWORD *pIndices, _In_ DWORD nBuffersize);
	};

};

#endif //__NMR_DLLINTERFACES
