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

Abstract:

NMR_COMInterfaces.h defines the Loading and Saving COM Interfaces, which are
shown to the outside world.

--*/

#ifndef __NMR_COMINTERFACES
#define __NMR_COMINTERFACES

#include "Model/COM/NMR_COMInterfaces_Base.h"


// Define Class IDs
#define CLSID_Lib3MF_Base                   "8F1D86BF-CD19-4710-BC19-3C70D095642F"
#define CLSID_Lib3MF_ModelWriter            "58DDBE95-34D8-4F77-BFDC-1570F849BFF2"
#define CLSID_Lib3MF_ModelReader            "A7FE2C65-0121-40AA-A0AF-48C9D2287A55"
#define CLSID_Lib3MF_ModelResource          "BF38DC27-4169-4AA3-BFD1-35C8D046C0A8"
#define CLSID_Lib3MF_ModelResourceIterator  "11845233-4A6A-4B10-8B55-00A84048959C"
#define CLSID_Lib3MF_ModelObjectResource    "FB9F7E2C-D8A3-4C84-831E-76928A91CC7B"
#define CLSID_Lib3MF_ModelTexture2D         "29DD25F2-607D-4804-A737-ED2CBC7199F2"
#define CLSID_Lib3MF_ModelAttachment        "A2EBA5BC-2A64-49CE-B11B-17B134CB48B0"
#define CLSID_Lib3MF_ModelTextureAttachment "AEEEE547-C4DB-4BD0-AC28-998AFE18EEE6"
#define CLSID_Lib3MF_ModelBaseMaterial      "943648CF-B9BB-40AD-8264-79C69195A116"
#define CLSID_Lib3MF_ModelMeshObject        "8B7FE33C-8EF0-4927-A106-1C069B49B01D"
#define CLSID_Lib3MF_ModelComponent         "99F7DB2E-9A6F-4DD5-9F96-27DDAF32A0CF"
#define CLSID_Lib3MF_ModelComponentsObject  "F4125D0E-297A-41E4-84F0-9D7110C46341"
#define CLSID_Lib3MF_ModelBuildItem         "13BA279E-9CF4-41BF-B3A7-74EDB6E7A0F1"
#define CLSID_Lib3MF_ModelBuildItemIterator "65E8B244-D2D4-4363-9751-938E28B968D8"
#define CLSID_Lib3MF_Model                  "BC58EF48-22A0-4A52-94ED-566C07017C49"
#define CLSID_Lib3MF_ModelFactory           "9A41DB91-678E-4DC5-BF25-D49FE8B176CC"
#define CLSID_Lib3MF_PropertyHandler        "1092CDE7-9A6D-48BC-821B-EBADCF50BC6A"
#define CLSID_Lib3MF_DefaultPropertyHandler "A62108EE-FB26-44C2-90CB-FBEE9D2823E5"
#define CLSID_Lib3MF_ModelMeshBeamSet       "C543ADE4-5F1A-456E-B572-7CA9CE05584A"
#define CLSID_Lib3MF_SliceStack             "72F71407-0DF4-4F16-BD37-5E8094FFCFE7"
#define CLSID_Lib3MF_Slice                  "266F2E3E-2BD9-4F37-A568-01FEF32D6A4A"



#ifdef NMR_COM_NATIVE
static const IID IID_Lib3MF_ModelWriter = { 0x58ddbe95, 0x34d8, 0x4f77, { 0xbf, 0xdc, 0x15, 0x70, 0xf8, 0x49, 0xbf, 0xf2 } };
static const IID IID_Lib3MF_ModelReader = { 0xa7fe2c65, 0x121, 0x40aa, { 0xa0, 0xaf, 0x48, 0xc9, 0xd2, 0x28, 0x7a, 0x55 } };
static const IID IID_Lib3MF_ModelResource = { 0xbf38dc27, 0x4169, 0x4aa3, { 0xbf, 0xd1, 0x35, 0xc8, 0xd0, 0x46, 0xc0, 0xa8 } };
static const IID IID_Lib3MF_ModelResourceIterator = { 0x11845233, 0x4a6a, 0x4b10, { 0x8b, 0x55, 0x0, 0xa8, 0x40, 0x48, 0x95, 0x9c } };
static const IID IID_Lib3MF_ModelObjectResource = { 0xfb9f7e2c, 0xd8a3, 0x4c84, { 0x83, 0x1e, 0x76, 0x92, 0x8a, 0x91, 0xcc, 0x7b } };
static const IID IID_Lib3MF_ModelTexture2D = { 0x29dd25f2, 0x607d, 0x4804, { 0xa7, 0x37, 0xed, 0x2c, 0xbc, 0x71, 0x99, 0xf2 } };
static const IID IID_Lib3MF_ModelAttachment = { 0xa2eba5bc, 0x2a64, 0x49ce,{ 0xb1, 0x1b, 0x17, 0xb1, 0x34, 0xcb, 0x48, 0xb0 } };
static const IID IID_Lib3MF_ModelTextureAttachment = { 0xaeeee547, 0xc4db, 0x4bd0,{ 0xac, 0x28, 0x99, 0x8a, 0xfe, 0x18, 0xee, 0xe6 } };
static const IID IID_Lib3MF_ModelBaseMaterial = { 0x943648cf, 0xb9bb, 0x40ad, { 0x82, 0x64, 0x79, 0xc6, 0x91, 0x95, 0xa1, 0x16 } };
static const IID IID_Lib3MF_ModelMeshObject = { 0x8b7fe33c, 0x8ef0, 0x4927, { 0xa1, 0x6, 0x1c, 0x6, 0x9b, 0x49, 0xb0, 0x1d } };
static const IID IID_Lib3MF_ModelComponent = { 0x99f7db2e, 0x9a6f, 0x4dd5, { 0x9f, 0x96, 0x27, 0xdd, 0xaf, 0x32, 0xa0, 0xcf } };
static const IID IID_Lib3MF_ModelComponentsObject = { 0xf4125d0e, 0x297a, 0x41e4, { 0x84, 0xf0, 0x9d, 0x71, 0x10, 0xc4, 0x63, 0x41 } };
static const IID IID_Lib3MF_ModelBuildItem = { 0x13ba279e, 0x9cf4, 0x41bf, { 0xb3, 0xa7, 0x74, 0xed, 0xb6, 0xe7, 0xa0, 0xf1 } };
static const IID IID_Lib3MF_ModelBuildItemIterator = { 0x65e8b244, 0xd2d4, 0x4363, { 0x97, 0x51, 0x93, 0x8e, 0x28, 0xb9, 0x68, 0xd8 } };
static const IID IID_Lib3MF_Model = { 0xbc58ef48, 0x22a0, 0x4a52, { 0x94, 0xed, 0x56, 0x6c, 0x7, 0x1, 0x7c, 0x49 } };
static const IID IID_Lib3MF_ModelFactory = { 0x9a41db91, 0x678e, 0x4dc5, { 0xbf, 0x25, 0xd4, 0x9f, 0xe8, 0xb1, 0x76, 0xcc } };
static const IID IID_Lib3MF_PropertyHandler = { 0x1092cde7, 0x9a6d, 0x48bc, { 0x82, 0x1b, 0xeb, 0xad, 0xcf, 0x50, 0xbc, 0x6a } };
static const IID IID_Lib3MF_DefaultPropertyHandler = { 0xa62108ee, 0xfb26, 0x44c2, { 0x90, 0xcb, 0xfb, 0xee, 0x9d, 0x28, 0x23, 0xe5 } };
static const IID IID_Lib3MF_ModelMeshBeamSet = { 0xc543ade4, 0x5f1a, 0x456e, { 0xb5, 0x72, 0x7c, 0xa9, 0xce, 0x05, 0x58, 0x4a } };
#endif //NMR_COM_NATIVE

namespace NMR {

	/**********************************************************************************************************
	*  ILib3MFModelWriter encapsulates a writer class for a writing the model into a specific file type.
	*  Current implementations include (binary) STL and 3MF.
	*
	*  The file type is specified by the derived class of the instance of this interface.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelWriter, ILib3MFBase, CLSID_Lib3MF_ModelWriter) {
		LIB3MFPUBLIC(ILib3MFModelWriter)

		/**
		* Writes out the model as file. The file type is specified by the Model Writer class
		*
		* @param[in] pwszFilename Filename to write into as UTF16 string
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToFile) (_In_z_ LPCWSTR pwszFilename) LIB3MFABSTRACT;

		/**
		* Writes out the model as file. The file type is specified by the Model Writer class
		*
		* @param[in] pwszFilename Filename to write into as UTF8 string
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToFileUTF8) (_In_z_ LPCSTR pszFilename) LIB3MFABSTRACT;

		/**
		* Retrieves the size of the full 3MF file stream.
		*
		* @param[out] pcbStreamSize Returns the stream size
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetStreamSize) (_Out_ ULONG64 * pcbStreamSize) LIB3MFABSTRACT;

		/**
		* Writes out the file into a memory buffer. Buffer size must be at least the size of the stream.
		*
		* @param[out] pBuffer Buffer to write into
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToBuffer) (_Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize) LIB3MFABSTRACT;

		/**
		* Writes out the model and passes the data to a provided callback function. The file type is specified by the Model Writer class
		*
		* @param[in] pWriteCallback Callback to call for writing a data chunk.
		* @param[in] pSeekCallback Callback to call for seeking in the streamk.
		* @param[in] pUserData Userdata that is passed to the callback function
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToCallback) (_In_ void * pWriteCallback, _In_opt_ void * pSeekCallback, _In_opt_ void * pUserData) LIB3MFABSTRACT;

		/**
		* Set the progress callback for calls to this writer
		* @param[in] callback pointer to the callback function. If the callback returns
		*            "false" the original function call will be aborted and set the error to
		*            NMR_USERABORTED.
		* @param[in] userData pointer to arbitrary user data that is passed without modification
		*            to the callback.
		* @return error code or 0 (success)
		**/
		LIB3MFMETHOD(SetProgressCallback) (_In_ void * callback, _In_ void* userData) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelReader encapsulates a reader class for reading a model from a specific file type.
	*  Current implementations include (binary) STL and 3MF.
	*
	*  The file type is specified by the derived class of the instance of this interface.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelReader, ILib3MFBase, CLSID_Lib3MF_ModelReader) {
		LIB3MFPUBLIC(ILib3MFModelReader)

		/**
		* Reads a model from a file. The file type is specified by the Model Reader class
		*
		* @param[in] pwszFilename Filename to read from as UTF16 string
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromFile) (_In_z_ LPCWSTR pwszFilename) LIB3MFABSTRACT;

		/**
		* Reads a model from a file. The file type is specified by the Model Reader class
		*
		* @param[in] pszFilename Filename to read from as UTF8 string
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromFileUTF8) (_In_z_ LPCSTR pszFilename) LIB3MFABSTRACT;

		/**
		* Reads a model from a memory buffer. The file type is specified by the Model Reader class
		*
		* @param[in] pBuffer Buffer to read from
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromBuffer) (_In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize) LIB3MFABSTRACT;

		/**
		* Returns Warning and Error Count of the read process
		*
		* @param[out] pnWarningCount filled with the count of the occurred warnings.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetWarningCount) (_Out_ DWORD * pnWarningCount) LIB3MFABSTRACT;

		/**
		* Returns Warning and Error Information of the read process
		*
		* @param[in] nIndex Index of the Warning. Valid values are 0 to WarningCount - 1
		* @param[out] pErrorCode filled with the error code of the warning
		* @param[out] pwszBuffer filled with the error message, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetWarning) (_In_ DWORD nIndex, _Out_ DWORD * pErrorCode, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Returns Warning and Error Information of the read process (UTF)
		*
		* @param[in] nIndex Index of the Warning. Valid values are 0 to WarningCount - 1
		* @param[out] pErrorCode filled with the error code of the warning
		* @param[out] pszBuffer filled with the error message, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetWarningUTF8) (_In_ DWORD nIndex, _Out_ DWORD * pErrorCode, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Adds a relationship type which shall be read as attachment in memory while loading
		*
		* @param[in] pwszRelationshipType String of the relationship type (UTF16)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddRelationToRead) (_In_z_ LPCWSTR pwszRelationshipType) LIB3MFABSTRACT;

		/**
		* Adds a relationship type which shall be read as attachment in memory while loading
		*
		* @param[in] pwszRelationshipType String of the relationship type (UTF16)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveRelationToRead) (_In_z_ LPCWSTR pwszRelationshipType) LIB3MFABSTRACT;

		/**
		* Adds a relationship type which shall be read as attachment in memory while loading
		*
		* @param[in] pszRelationshipType String of the relationship type (UTF8)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddRelationToReadUTF8) (_In_z_ LPCSTR pszRelationshipType) LIB3MFABSTRACT;


		/**
		* Adds a relationship type which shall be read as attachment in memory while loading
		*
		* @param[in] pszRelationshipType String of the relationship type (UTF8)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveRelationToReadUTF8) (_In_z_ LPCSTR pszRelationshipType) LIB3MFABSTRACT;

		/**
		* Activates (deactivates) the strict mode of the reader.
		* If active, all warnings are reported as errors. Otherwise, they are reported as warnings. By default, it is deactivated.
		*
		* @param[in] bStrictModeActive flag whether strict mode is active or not
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetStrictModeActive) (_In_ BOOL bStrictModeActive) LIB3MFABSTRACT;

		/**
		* Queries whether the strict mode of the reader is active or not
		*
		* @param[out] pbStrictModeActive flag whether strict mode is active or not
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetStrictModeActive) (_Out_ BOOL * pbStrictModeActive) LIB3MFABSTRACT;

		/**
		* Reads a model and from the data provided by a callback function. The file type is specified by the Model Writer class
		*
		* @param[in] pReadCallback Callback to call for reading a data chunk.
		* @param[in] nStreamSize number of bytes the callback returns
		* @param[in] pSeekCallback Callback to call for seeking in the stream.
		* @param[in] pUserData Userdata that is passed to the callback function
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromCallback) (_In_ void * pReadCallback, _In_ ULONG64 nStreamSize,  _In_opt_ void * pSeekCallback, _In_opt_ void * pUserData) LIB3MFABSTRACT;

		/**
		* Set the progress callback for calls to this reader
		* @param[in] callback pointer to the callback function. If the callback returns
		*            "false" the original function call will be aborted and set the error to
		*            NMR_USERABORTED.
		* @param[in] userData pointer to arbitrary user data that is passed without modification
		*            to the callback.
		* @return error code or 0 (success)
		**/
		LIB3MFMETHOD(SetProgressCallback) (_In_ void * callback, _In_ void* userData) LIB3MFABSTRACT;

	};

	/**********************************************************************************************************
	*  ILib3MFModelResource is a base interface for all 3MF Resources.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelResource, ILib3MFBase, CLSID_Lib3MF_ModelResource) {
		LIB3MFPUBLIC(ILib3MFModelResource)

		/**
		* Retrieves the ID of a Model Resource Instance
		*
		* @param[out] nResourceID Filled with the ID of the Resource Instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetResourceID) (_Out_ DWORD * pnResourceID) LIB3MFABSTRACT;
	};


	/**********************************************************************************************************
	*  ILib3MFModelResourceIterator is a helper class to iterate through arbitrary lists of 3MF resources
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelResourceIterator, ILib3MFBase, CLSID_Lib3MF_ModelResourceIterator) {
		LIB3MFPUBLIC(ILib3MFModelResourceIterator)

		/**
		* Iterates to the next resource in the list.
		*
		* @param[out] pbHasNext returns, if there is a resource to use.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(MoveNext) (_Out_ BOOL * pbHasNext) LIB3MFABSTRACT;

		/**
		* Iterates to the previous resource in the list.
		*
		* @param[out] pbHasPrevious returns, if there is a resource to use.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(MovePrevious) (_Out_ BOOL * pbHasPrevious) LIB3MFABSTRACT;

		/**
		* Returns the resource the iterator points at.
		*
		* @param[out] ppResourceInstance returns the resource instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetCurrent) (_Outptr_ ILib3MFModelResource ** ppResourceInstance) LIB3MFABSTRACT;

		/**
		* Creates a new resource iterator with the same resource list.
		*
		* @param[out] ppIterator returns the cloned Iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(Clone) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator) LIB3MFABSTRACT;
	};


	/**********************************************************************************************************
	*  ILib3MFPropertyHandler encapsulates all methods for handling 3MF mesh properties.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFPropertyHandler, ILib3MFBase, CLSID_Lib3MF_PropertyHandler) {

		LIB3MFPUBLIC(ILib3MFPropertyHandler)

		/**
		* Removes all properties of a specific triangle.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveProperty) (_In_ DWORD nIndex) LIB3MFABSTRACT;

		/**
		* Removes all properties of the triangle mesh.
		*
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveAllProperties) () LIB3MFABSTRACT;

		/**
		* Returns the property type of the specific triangle
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[out] pnPropertyType Returns the property type of the triangle
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPropertyType) (_In_ DWORD nIndex, _Out_ eModelPropertyType * pnPropertyType) LIB3MFABSTRACT;

		/**
		* Returns the base material of a specific triangle.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[out] pnMaterialGroupID returns the material group id, per triangle. A return group id of 0 means either no property at all or a non-material property.
		* @param[out] pnMaterialIndex returns the material index, per triangle. Returns 0, if no base material is assigned.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBaseMaterial) (_In_ DWORD nIndex, _Out_ DWORD * pnMaterialGroupID, _Out_ DWORD * pnMaterialIndex) LIB3MFABSTRACT;

		/**
		* Returns the base materials of all triangles.
		* If a triangle property is not a material, the returned material group ID will be 0.
		*
		* @param[out] pnMaterialGroupIDs will be filled with the material group ids of the triangles. Array must have trianglecount entries. A return group id of 0 means either no property at all or a non-material property.
		* @param[out] pnMaterialIndices will be filled with the material group indices of the triangles. Array must have trianglecount entries.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBaseMaterialArray) (_Out_ DWORD * pnMaterialGroupIDs, _Out_ DWORD * pnMaterialIndices) LIB3MFABSTRACT;

		/**
		* Sets the material of a triangle to a specific single value. All other Triangle properties are removed.
		* This must be a base material .
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] nMaterialGroupID Group ID of the Material Group
		* @param[in] nMaterialIndex Index of the Material in the Group
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBaseMaterial) (_In_ DWORD nIndex, _In_ PackageResourceID nMaterialGroupID, _In_ DWORD nMaterialIndex) LIB3MFABSTRACT;

		/**
		* Sets the materials of all triangles to specific values.
		*
		* @param[in] pnMaterialGroupIDs array of the material Group IDs. Must have trianglecount entries. If a group ID of 0 is specified.
		* @param[in] pnMaterialIndices array of the corresponding material indices. Must have trianglecount entries.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBaseMaterialArray) (_In_ PackageResourceID * pnMaterialGroupIDs, _In_ DWORD * pnMaterialIndices) LIB3MFABSTRACT;


		/**
		* Returns the color of a specific triangle.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[out] pColor returns the color values of the three nodes of the triangle. (#00000000) means no property or a different kind of property!
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetColor) (_In_ DWORD nIndex, _Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColor) LIB3MFABSTRACT;

		/**
		* Returns the color array of all triangles
		*
		* @param[out] pColors returns the color values of the three nodes of each triangle. Must have at least trianglecount array entries.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetColorArray) (_Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColors) LIB3MFABSTRACT;

		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetSingleColor) (_In_ DWORD nIndex, _In_ MODELMESHCOLOR_SRGB * pColor) LIB3MFABSTRACT;

		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetSingleColorRGB) (_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue) LIB3MFABSTRACT;

		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetSingleColorRGBA) (_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha) LIB3MFABSTRACT;

		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetSingleColorFloatRGB) (_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue) LIB3MFABSTRACT;

		/**
		* Sets the specific triangle to a single color. All other properties are removed.
		* Mixing properties needs the property extension API.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color value of the triangle. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetSingleColorFloatRGBA) (_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha) LIB3MFABSTRACT;

		/**
		* Sets the (single) color of all triangles. All other properties are removed.
		*
		* @param[in] pColors new color values for the triangles. (#00000000) means no color property.. Must have at least trianglecount array entries.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetSingleColorArray) (_Out_ MODELMESHCOLOR_SRGB * pColors) LIB3MFABSTRACT;

		/**
		* Sets the specific triangle to a color per vertex. All other properties are removed.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pColor new color values of the three nodes of the triangle. (#00000000) means no color property is set.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetGradientColor) (_In_ DWORD nIndex, _Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColor) LIB3MFABSTRACT;

		/**
		* Sets the (gradient) color of all triangles. All other properties are removed.
		*
		* @param[in] pColors returns the color values of the three nodes of each triangle. Must have at least trianglecount array entries. (#00000000) means no color property is set.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetGradientColorArray) (_Out_ MODELMESH_TRIANGLECOLOR_SRGB * pColors) LIB3MFABSTRACT;


		/**
		* Returns the 2D texture information of a specific triangle.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[out] pTexture returns the UV texture values of the three nodes of the triangle. texture ID 0 means no property or a different kind of property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTexture) (_In_ DWORD nIndex, _Out_ MODELMESHTEXTURE2D * pTexture) LIB3MFABSTRACT;

		/**
		* Returns the 2D texture information of all triangles.
		*
		* @param[out] pTextures returns the UV texture values of the three nodes of all triangles. Must have at least trianglecount array entries.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTextureArray) (_Out_ MODELMESHTEXTURE2D * pTextures) LIB3MFABSTRACT;

		/**
		* Sets the 2D texture information of a specific triangle.
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pTexture new UV texture values of the three nodes of the triangle. texture ID 0 means no property or a different kind of property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetTexture) (_In_ DWORD nIndex, _In_ MODELMESHTEXTURE2D * pTexture) LIB3MFABSTRACT;

		/**
		* Sets the 2D texture information of all triangles.
		*
		* @param[in] pTexture new UV texture values of the three nodes of all triangles. Must have at least trianglecount array entries.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetTextureArray) (_In_ MODELMESHTEXTURE2D * pTextures) LIB3MFABSTRACT;

	};

	/**********************************************************************************************************
	*  ILib3MFDefaultPropertyHandler encapsulates all methods for handling 3MF default object properties.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFDefaultPropertyHandler, ILib3MFBase, CLSID_Lib3MF_DefaultPropertyHandler) {

		LIB3MFPUBLIC(ILib3MFDefaultPropertyHandler)

		/**
		* Removes the default property of the object.
		*
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveProperty) () LIB3MFABSTRACT;

		/**
		* Returns the property type of the object
		*
		* @param[out] pnPropertyType Returns the property type of the object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPropertyType) (_Out_ eModelPropertyType * pnPropertyType) LIB3MFABSTRACT;

		/**
		* Returns the base material the object
		*
		* @param[out] pnMaterialGroupID returns the material group id, per triangle. A return group id of 0 means either no property at all or a non-material property.
		* @param[out] pnMaterialIndex returns the material index, per triangle. Returns 0, if no base material is assigned.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBaseMaterial) (_Out_ DWORD * pnMaterialGroupID, _Out_ DWORD * pnMaterialIndex) LIB3MFABSTRACT;

		/**
		* Sets the material of an object to a specific single value.
		* This must be a base material.
		*
		* @param[in] nMaterialGroupID Group ID of the Material Group
		* @param[in] nMaterialIndex Index of the Material in the Group
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBaseMaterial) (_In_ DWORD nMaterialGroupID, _In_ DWORD nMaterialIndex) LIB3MFABSTRACT;


		/**
		* Returns the default property color of an object.
		*
		* @param[out] pColor returns the default color of the object. (#00000000) means no property or a different kind of property!
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetColor) (_In_ DWORD nIndex, _Out_ MODELMESHCOLOR_SRGB * pColor) LIB3MFABSTRACT;

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetColor) (_In_ MODELMESHCOLOR_SRGB * pColor) LIB3MFABSTRACT;

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetColorRGB) (_In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue) LIB3MFABSTRACT;

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetColorRGBA) (_In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha) LIB3MFABSTRACT;

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetFloatColorRGB) (_In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue) LIB3MFABSTRACT;

		/**
		* Sets the default property of an object to a single color.
		* Mixing properties needs the property extension API.
		*
		* @param[in] pColor new default color value of the object. (#00000000) means no color property.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetFloatColorRGBA) (_In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha) LIB3MFABSTRACT;

		/**
		* Returns the default 2D texture information of an object.
		*
		* @param[out] pnTextureID Returns the default Texture ID of the object. 0 means no property or a different kind of property.
		* @param[out] pfU Returns the default U value of the object.
		* @param[out] pfV Returns the default V value of the object.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTexture) (_Out_ PackageResourceID * pnTextureID, _Out_ FLOAT * pfU, _Out_ FLOAT * pfV) LIB3MFABSTRACT;

		/**
		* Sets the default 2D texture information of an object.
		*
		* @param[in] nTextureID Sets the default Texture ID of the object. 0 means not default property.
		* @param[in] fU Sets the default U value of the object.
		* @param[in] fV Sets the default V value of the object.
		*/
		LIB3MFMETHOD(SetTexture) (_In_ PackageResourceID nTextureID, _In_ FLOAT fU, _In_ FLOAT fV) LIB3MFABSTRACT;

	};

	/**********************************************************************************************************
	*  ILib3MFModelObjectResource is a base interface for all 3MF Object Resources, i.e. Mesh Objects and
	*  Components.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelObjectResource, ILib3MFModelResource, CLSID_Lib3MF_ModelObjectResource) {

		LIB3MFPUBLIC(ILib3MFModelObjectResource)

		/**
		* Retrieves a object's type
		*
		* @param[out] pObjectType returns object type constant. See ModelTypes.h
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetType) (_Out_ DWORD * pObjectType) LIB3MFABSTRACT;

		/**
		* Sets an object's type
		*
		* @param[out] ObjectType object type constant. See ModelTypes.h
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetType) (_In_ DWORD ObjectType) LIB3MFABSTRACT;

		/**
		* Retrieves a object's name string
		*
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetName) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Retrieves a object's name string (UTF8)
		*
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetNameUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Sets a object's name string
		*
		* @param[in] pwszName new name of the object as UTF16 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetName) (_In_z_ LPCWSTR pwszName) LIB3MFABSTRACT;

		/**
		* Sets a object's name string
		*
		* @param[in] pszName new name of the object as UTF8 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetNameUTF8) (_In_z_ LPCSTR pszName) LIB3MFABSTRACT;

		/**
		* Retrieves a object's part number string
		*
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPartNumber) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Retrieves a object's part number string (UTF8)
		*
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPartNumberUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;


		/**
		* Sets a object's part number string
		*
		* @param[in] pwszPartNumber new part number (UTF16) string for referencing parts from an outside context.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetPartNumber) (_In_z_ LPCWSTR pwszPartNumber) LIB3MFABSTRACT;

		/**
		* Sets a object's part number string
		*
		* @param[in] pszPartNumber new part number (UTF8) string for referencing parts from an outside context.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetPartNumberUTF8) (_In_z_ LPCSTR pszPartNumber) LIB3MFABSTRACT;


		/**
		* Retrieves, if an object is a mesh object
		*
		* @param[out] pbIsMeshObject returns, if the object is a mesh object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(IsMeshObject) (_Out_ BOOL * pbIsMeshObject) LIB3MFABSTRACT;

		/**
		* Retrieves, if an object is a component object
		*
		* @param[out] pbIsComponentObject returns, if the object is a components object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(IsComponentsObject) (_Out_ BOOL * pbIsComponentObject) LIB3MFABSTRACT;

		/**
		* Retrieves, if the object is valid according to the core spec.
		* For mesh objects, we distinguish between the type attribute of the object:
		* In case of object type "other", this always means "false"
		* In case of object type "support", this always means "true"
		* In case of object type "model", this means, if the mesh suffices all requirements of the core spec chapter 4.1
		* A component objects is valid if and only if it contains at least one component -
		* and all child components are valid objects.
		*
		* @param[out] pbIsValid returns, if the object is a valid object description.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(IsValidObject) (_Out_ BOOL * pbIsValid) LIB3MFABSTRACT;

		/**
		* Link the object to a slice stack
		*
		* @param[in] nSliceStackId id of the slice stack to link
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetSliceStackId)(_In_ DWORD nSliceStackId) LIB3MFABSTRACT;

		/**
		* get the id of the slice stack linked to the object
		* @param[out] pnSliceStackId id of the slice stack linked to the object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetSliceStackId)(_Out_ DWORD *pnSliceStackId) LIB3MFABSTRACT;

		/**
		* set the meshresolution of the object
		* @param[in] eSlicesMeshResolution meshresolution of this object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetSlicesMeshResolution)(_In_ eModelSlicesMeshResolution eSlicesMeshResolution) LIB3MFABSTRACT;

		/**
		* get the meshresolution of the object
		* @param[out] peSlicesMeshResolution meshresolution of this object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetSlicesMeshResolution)(_Out_ eModelSlicesMeshResolution *peSlicesMeshResolution) LIB3MFABSTRACT;

		/* Property handling */

		/**
		* creates a default property handler for the object
		*
		* @param[out] ppPropertyHandler returns a default property handler instance for the object.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(CreateDefaultPropertyHandler) (_Outptr_ ILib3MFDefaultPropertyHandler ** ppPropertyHandler) LIB3MFABSTRACT;

		/**
		* creates a default property handler for a specific multiproperty channel of an object
		*
		* @param[in] nChannel Channel Index
		* @param[out] ppPropertyHandler returns a default property handler instance of an object.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(CreateDefaultMultiPropertyHandler) (_In_ DWORD nChannel, _Outptr_ ILib3MFDefaultPropertyHandler ** ppPropertyHandler) LIB3MFABSTRACT;

		/**
		* Retrieves the path used as thumbnail for an object (UTF8). Returns "" if none is set
		*
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetThumbnailPathUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Sets an object's thumbnail package path (UTF8)
		*
		* @param[in] pszPath path where to look for the thumbnail (e.g. "/Textures/thumbnail.png"). Call will NULL to clear the thumbnail.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetThumbnailPathUTF8) (_In_z_ LPCSTR pszName) LIB3MFABSTRACT;

		/**
		* returns, whether a object has a UUID and, if true, the object's UUID
		*
		* @param[out] pbHasUUID flag whether the object has a UUID
		* @param[out] pszBuffer the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetUUIDUTF8) (_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer) LIB3MFABSTRACT;

		/**
		* sets the object's UUID
		*
		* @param[in] pszUUID the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetUUIDUTF8) (_In_z_ LPCSTR pszUUID) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelBaseMaterial implements the Base Material Group Resources of a 3MF model stream, and allows direct access to the
	*  base material information
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelBaseMaterial, ILib3MFModelResource, CLSID_Lib3MF_ModelBaseMaterial) {

		LIB3MFPUBLIC(ILib3MFModelBaseMaterial)

		/**
		* Retrieves the count of base materials in the material group.
		*
		* @param[out] pcbCount returns the count of base materials.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetCount) (_Out_ DWORD * pcbCount) LIB3MFABSTRACT;

		/**
		* Adds a new material to the material group
		*
		* @param[in] pwszName new name of the base material. (UTF16 String, e.g. "ABS red")
		* @param[in] bRed New red value of display color (0-255)
		* @param[in] bGreen New red value of display color (0-255)
		* @param[in] bBlue New red value of display color (0-255)
		* @param[out] pnResourceIndex returns new Index of the material in the material group
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddMaterial) (_In_z_ LPCWSTR pwszName, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ DWORD * pnResourceIndex) LIB3MFABSTRACT;

		/**
		* Adds a new material to the material group
		*
		* @param[in] pwszName new name of the base material. (UTF8 String, e.g. "ABS red")
		* @param[in] bRed New red value of display color (0-255)
		* @param[in] bGreen New red value of display color (0-255)
		* @param[in] bBlue New red value of display color (0-255)
		* @param[out] pnResourceIndex returns new Index of the material in the material group
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddMaterialUTF8) (_In_z_ LPCSTR pszName, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ DWORD * pnResourceIndex) LIB3MFABSTRACT;


		/**
		* Removes a material from the material group
		*
		* @param[in] nIndex Index of the material in the material group
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveMaterial) (_In_ DWORD nIndex) LIB3MFABSTRACT;

		/**
		* Retrieves a base material's name
		*
		* @param[in] nIndex Index of the material in the material group
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetName) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Retrieves a base material's name (UTF8)
		*
		* @param[in] nIndex Index of the material in the material group
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetNameUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Sets a base material's name (UTF16)
		*
		* @param[in] nIndex Index of the material in the material group
		* @param[in] pwszName new name of the base material. (e.g. "ABS red")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetName) (_In_ DWORD nIndex, _In_z_ LPCWSTR pwszName) LIB3MFABSTRACT;

		/**
		* Sets a base material's name (UTF8)
		*
		* @param[in] nIndex Index of the material in the material group
		* @param[in] pwszName new name of the base material. (e.g. "ABS red")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetNameUTF8) (_In_ DWORD nIndex, _In_z_ LPCSTR pszName) LIB3MFABSTRACT;

		/**
		* Sets a base material's display color. Alpha is set to 255.
		*
		* @param[in] nIndex Index of the material in the material group
		* @param[in] bRed New red value (0-255)
		* @param[in] bGreen New green value (0-255)
		* @param[in] bBlue New blue value (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetDisplayColorRGB) (_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue) LIB3MFABSTRACT;

		/**
		* Sets a base material's display color.
		*
		* @param[in] nIndex Index of the material in the material group
		* @param[in] bRed New red value (0-255)
		* @param[in] bGreen New green value (0-255)
		* @param[in] bBlue New blue value (0-255)
		* @param[in] bAlpha New alpha value (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetDisplayColorRGBA) (_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha) LIB3MFABSTRACT;

		/**
		* Sets a base material's display color. Alpha is set to 1.0.
		*
		* @param[in] nIndex Index of the material in the material group
		* @param[in] fRed New red value (0.0-1.0)
		* @param[in] fGreen New green value (0.0-1.0)
		* @param[in] fBlue New blue value (0.0-1.0)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetDisplayColorFloatRGB) (_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue) LIB3MFABSTRACT;

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
		LIB3MFMETHOD(SetDisplayColorFloatRGBA) (_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha) LIB3MFABSTRACT;

		/**
		* Returns a base material's display color.
		*
		* @param[in] nIndex Index of the material in the material group
		* @param[out] pbRed Returns red value (0-255)
		* @param[out] pbGreen Returns green value (0-255)
		* @param[out] pbBlue Returns blue value (0-255)
		* @param[out] pbAlpha Returns alpha value (0-255)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetDisplayColor) (_In_ DWORD nIndex, _Out_ BYTE* pbRed, _Out_ BYTE* pbGreen, _Out_ BYTE* pbBlue, _Out_ BYTE* pbAlpha) LIB3MFABSTRACT;

	};

	/**********************************************************************************************************
	*  ILib3MFModelAttachment implements the Model Attachments of a 3MF model stream, and allows direct access to 
	*  direct binary data.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelAttachment, ILib3MFBase, CLSID_Lib3MF_ModelAttachment) {

		LIB3MFPUBLIC(ILib3MFModelAttachment)

		/**
		* Retrieves an attachment's package path
		*
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPath) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Retrieves an attachment's package path (UTF8)
		*
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPathUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Sets an attachment's package path
		*
		* @param[in] pwszPath new path of the attachment. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetPath) (_In_z_ LPCWSTR pwszPath) LIB3MFABSTRACT;

		/**
		* Sets an attachment's package path (UTF8)
		*
		* @param[in] pszPath new path of the attachment. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetPathUTF8) (_In_z_ LPCSTR pszPath) LIB3MFABSTRACT;

		/**
		* Retrieves an attachment's package relationship type
		*
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetRelationshipType) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Retrieves an attachment's package relationship type (UTF8)
		*
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetRelationshipTypeUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Sets an attachment's package relationship type
		*
		* @param[in] pwszRelationShipType new relationship type attachment. (e.g. "/Data/data.xml")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetRelationshipType) (_In_z_ LPCWSTR pwszRelationShipType) LIB3MFABSTRACT;

		/**
		* Sets an attachment's package relationship type (UTF8)
		*
		* @param[in] pszRelationShipType new path of the attachment. (e.g. "/Data/data.png")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetRelationshipTypeUTF8) (_In_z_ LPCSTR pszRelationShipType) LIB3MFABSTRACT;

		/**
		* Retrieves the size of the attachment stream.
		*
		* @param[out] pcbStreamSize Returns the stream size
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetStreamSize) (_Out_ ULONG64 * pcbStreamSize) LIB3MFABSTRACT;

		/**
		* Writes out the attachment as file.
		*
		* @param[in] pwszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToFile) (_In_z_ LPCWSTR pwszFilename) LIB3MFABSTRACT;

		/**
		* Writes out the attachment as file. (UTF8)
		*
		* @param[in] pszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToFileUTF8) (_In_z_ LPCSTR pszFilename) LIB3MFABSTRACT;

		/**
		* Writes out the attachment into a buffer. Buffer size must be at least the size of the stream.
		*
		* @param[out] pBuffer Buffer to write into
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToBuffer) (_Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize) LIB3MFABSTRACT;

		/**
		* Writes out the attachment and passes the data to a provided callback function. The file type is specified by the Model Writer class
		*
		* @param[in] pWriteCallback Callback to call for writing a data chunk.
		* @param[in] pUserData Userdata that is passed to the callback function
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToCallback) (_In_ void * pWriteCallback, _In_opt_ void * pUserData) LIB3MFABSTRACT;


		/**
		* Reads an attachment from a file.
		*
		* @param[in] pwszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromFile) (_In_z_ LPCWSTR pwszFilename) LIB3MFABSTRACT;

		/**
		* Reads an attachment from a file.
		*
		* @param[in] pszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromFileUTF8) (_In_z_ LPCSTR pwszFilename) LIB3MFABSTRACT;

		/**
		* Reads an attachment from a memory buffer.
		*
		* @param[in] pBuffer Buffer to read from
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromBuffer) (_In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelTextureAttachment implements the TextureAttachment of a 3MF model stream, and allows direct access
	*  to the the image data.
	*
	***********************************************************************************************************/
	LIB3MFINTERFACE(ILib3MFModelTextureAttachment, ILib3MFBase, CLSID_Lib3MF_ModelTextureAttachment) {

		LIB3MFPUBLIC(ILib3MFModelTextureAttachment)

		LIB3MFMETHOD(GetPath) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelTexture2D implements the Texture2D Resources of a 3MF model stream, and allows direct access to the
	*  texture properties and the image data.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelTexture2D, ILib3MFModelResource, CLSID_Lib3MF_ModelTexture2D) {

		LIB3MFPUBLIC(ILib3MFModelTexture2D)

		/**
		* Retrieves the attachment located at the path of the texture
		*
		* @param[out] ppTextureAttachment attachment that holds the texture's image information
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetAttachment) (_Out_ ILib3MFModelAttachment ** ppTextureAttachment) LIB3MFABSTRACT;

		/**
		* Sets the texture's package path to the path of the attachment
		*
		* @param[out] pTextureAttachment attachment that holds the texture's image information
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetAttachment) (_In_ ILib3MFModelAttachment * pTextureAttachment) LIB3MFABSTRACT;

		/**
		* Retrieves a texture's package path
		*
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPath) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Retrieves a texture's package path (UTF8)
		*
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPathUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Sets a texture's package path
		*
		* @param[in] pwszPath new path of the texture resource. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetPath) (_In_z_ LPCWSTR pwszPath) LIB3MFABSTRACT;

		/**
		* Sets a texture's package path (UTF8)
		*
		* @param[in] pszPath new path of the texture resource. (e.g. "/Textures/logo.png")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetPathUTF8) (_In_z_ LPCSTR pszPath) LIB3MFABSTRACT;

		/**
		* Retrieves a texture's content type
		*
		* @param[out] peContentType returns content type enum
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetContentType) (_Out_ eModelTexture2DType * peContentType) LIB3MFABSTRACT;

		/**
		* Sets a texture's content type
		*
		* @param[in] eContentType new Content Type
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetContentType) (_In_ eModelTexture2DType eContentType) LIB3MFABSTRACT;

		/**
		* Retrieves a texture's tilestyle type
		*
		* @param[out] peTileStyleU returns tilestyle type enum
		* @param[out] peTileStyleV returns tilestyle type enum
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTileStyleUV) (_Out_ eModelTextureTileStyle * peTileStyleU, _Out_ eModelTextureTileStyle * peTileStyleV) LIB3MFABSTRACT;

		/**
		* Sets a texture's tilestyle type
		*
		* @param[out] eTileStyleU new tilestyle type enum
		* @param[out] eTileStyleV new tilestyle type enum
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetTileStyleUV) (_In_ eModelTextureTileStyle eTileStyleU, _In_ eModelTextureTileStyle eTileStyleV) LIB3MFABSTRACT;

		/**
		* Retrieves a texture's filter type
		*
		* @param[out] peFilter returns filter type enum
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetFilter) (_Out_ eModelTextureFilter * peFilter) LIB3MFABSTRACT;

		/**
		* Sets a texture's filter type
		*
		* @param[out] eFilter new filter type enum
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetFilter) (_In_ eModelTextureFilter eFilter) LIB3MFABSTRACT;

		/**
		* Retrieves a texture's box2D coordinates.
		*
		* @param[out] pfU returns the U value of the texture
		* @param[out] pfV returns the V value of the texture
		* @param[out] pfWidth returns the Width value of the texture
		* @param[out] pfHeight returns the Height value of the texture
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBox2D) (_Out_ FLOAT * pfU, _Out_ FLOAT * pfV, _Out_ FLOAT * pfWidth, _Out_ FLOAT * pfHeight) LIB3MFABSTRACT;

		/**
		* Sets a texture's box2D coordinates.
		*
		* @param[out] fU the new U value of the texture
		* @param[out] fV the new V value of the texture
		* @param[out] fWidth the new Width value of the texture
		* @param[out] fHeight the new Height value of the texture
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBox2D) (_In_ FLOAT fU, _In_ FLOAT fV, _In_ FLOAT fWidth, _In_ FLOAT fHeight) LIB3MFABSTRACT;

		/**
		* Clears a texture's box2D coordinates.
		*
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ClearBox2D) () LIB3MFABSTRACT;

		/**
		* Retrieves the size of the texture stream.
		*
		* @param[out] pcbStreamSize Returns the stream size
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetStreamSize) (_Out_ ULONG64 * pcbStreamSize) LIB3MFABSTRACT;

		/**
		* Writes out the texture as file.
		*
		* @param[in] pwszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToFile) (_In_z_ LPCWSTR pwszFilename) LIB3MFABSTRACT;

		/**
		* Writes out the texture as file. (UTF8)
		*
		* @param[in] pszFilename Filename to write into
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToFileUTF8) (_In_z_ LPCSTR pszFilename) LIB3MFABSTRACT;

		/**
		* Writes out the texture into a buffer. Buffer size must be at least the size of the stream.
		*
		* @param[out] pBuffer Buffer to write into
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToBuffer) (_Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize) LIB3MFABSTRACT;

		/**
		* Writes out the texture and passes the data to a provided callback function. The file type is specified by the Model Writer class
		*
		* @param[in] pWriteCallback Callback to call for writing a data chunk.
		* @param[in] pUserData Userdata that is passed to the callback function
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToCallback) (_In_ void * pWriteCallback, _In_opt_ void * pUserData) LIB3MFABSTRACT;


		/**
		* Reads a texture from a file.
		*
		* @param[in] pwszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromFile) (_In_z_ LPCWSTR pwszFilename) LIB3MFABSTRACT;

		/**
		* Reads a texture from a file.
		*
		* @param[in] pszFilename Filename to read from
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromFileUTF8) (_In_z_ LPCSTR pwszFilename) LIB3MFABSTRACT;

		/**
		* Reads a texture from a memory buffer.
		*
		* @param[in] pBuffer Buffer to read from
		* @param[in] cbBufferSize Size of the buffer in bytes
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromBuffer) (_In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelMeshBeamSet is a class that holds the references that contain to a beamset. It is part of the
	*  beamlattice extension to 3MF.
	*
	***********************************************************************************************************/
	LIB3MFINTERFACE(ILib3MFModelMeshBeamSet, ILib3MFBase, CLSID_Lib3MF_ModelMeshBeamSet) {
		LIB3MFPUBLIC(ILib3MFModelMeshBeamSet)

		/**
		* Sets a beamset's name string
		*
		* @param[in] pwszName new name of the BeamSet as UTF16 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetName) (_In_z_ LPCWSTR pwszName) LIB3MFABSTRACT;

		/**
		* Sets a beamset's identifier string
		*
		* @param[in] pwszIdentifier new id of the BeamSet as UTF16 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetIdentifier) (_In_z_ LPCWSTR pwszIdentifier) LIB3MFABSTRACT;

		/**
		* Sets a beamset's name string (UTF8)
		*
		* @param[in] pwszName new name of the BeamSet as UTF8 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetNameUTF8) (_In_z_ LPCSTR pszName) LIB3MFABSTRACT;

		/**
		* Sets a beamset's identifier string (UTF8)
		*
		* @param[in] pszIdentifier new identifier of the BeamSet as UTF8 string. (e.g. "Car")
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetIdentifierUTF8) (_In_z_ LPCSTR pszIdentifier) LIB3MFABSTRACT;

		/**
		* Retrieves a BeamSet's name string (UTF8)
		*
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetNameUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Retrieves a BeamSet's identifier string (UTF8)
		*
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetIdentifierUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Retrieves the reference count of a BeamSet
		*
		* @param[out] pnCount returns the reference count
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetRefCount) (_Out_ DWORD * pnCount) LIB3MFABSTRACT;

		/**
		* Sets the references of a BeamSet
		*
		* @param[in] pRefs buffer containing the indices of all beams in this beamset
		* @param[in] nRefCount number of references to be set
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetRefs) (_In_ DWORD * pRefs, _In_ DWORD nRefCount) LIB3MFABSTRACT;

		/**
		* Retrieves all references of a BeamSet
		*
		* @param[in] pRefs buffer filled with beam references (indices of beams)
		* @param[in] nBufferSize size of the buffer in elements, must be at least refcount
		* @param[out] pnRefCount returns how many references have been written
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetRefs) (_Out_ DWORD * pRefs, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnRefCount) LIB3MFABSTRACT;

	};


	/**********************************************************************************************************
	*  ILib3MFSlice encapsulates all slice functionality for handling slices in 3mf
	*
	***********************************************************************************************************/

  LIB3MFINTERFACE(ILib3MFSlice, ILib3MFBase, CLSID_Lib3MF_Slice) {
    LIB3MFPUBLIC(ILib3MFSlice)

		/**
		* Add a single vertex to a slice
		* @param[in] pVertex holds the vertex coordinates
		* @param[out] pnIndex returns the index of the vertex. Needed to reference the vertex later on in a polygon
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddVertex)(_In_ MODELSLICEVERTEX * pVertex, _Out_opt_ DWORD * pnIndex) LIB3MFABSTRACT;

		/**
		* Begin a polygon
		* @param[out] pnIndex index of the newly created polygon
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(BeginPolygon)(_Out_opt_ DWORD *pnIndex) LIB3MFABSTRACT;

		LIB3MFMETHOD(AddPolygonIndex)(_In_ DWORD nPolygonIndex, _In_ DWORD nVertexIndex) LIB3MFABSTRACT;

		/**
		* Add indices to a polygon
		* @param[in] nPolygonIndex index of the polygon to add the indices to
		* @param[in] pnVertexIndices array of the indices for the polygon
		* @param[in] nBufferSize number of elements in the vertex array
		* @param[out] nPolygonVertexindex returns the start index of the added indices
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddPolygonIndices)(_In_ DWORD nPolygonIndex, _In_ DWORD *pnVertexIndices, _In_ DWORD nBufferSize, _Out_ DWORD * nPolygonVertexIndex) LIB3MFABSTRACT;

		/**
		* Get the number of polygons in the slice
		* @param[out] pnPolygonCount returns the number of polygons in the slice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPolygonCount) (_Out_ DWORD * pnPolygonCount) LIB3MFABSTRACT;

		/**
		* Get the number of vertices in the slice
		* @param[out] pnVertexCount returns the number of vertices in the slice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetVertexCount) (_Out_ DWORD * pnVertexCount) LIB3MFABSTRACT;

		/**
		* Get the upper Z coordinate of the slice
		* @param[out] pfTopZ returns the upper Z coordinate of the slice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTopZ) (_Out_ FLOAT * pfTopZ) LIB3MFABSTRACT;

		/**
		* Get the number of indices of a polygon in the slice
		* @param[in] nPolygonIndex the index of the polygon
		* @param[out] pnPolygonCount returns the number of indices in a polygon of the slice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetIndexCountOfPolygon) (_In_ DWORD nPolygonIndex, _Out_ DWORD *pnPolygonCount) LIB3MFABSTRACT;

		/**
		* Get the indices of a polygon
		* @param[in] nPolygonIndex the index of the polygon to query
		* @param[out] nPolygonIndeces an array to be filled with the polygon indices
		* @param[in] nBufferCount number of elements in "pPolygonIndeces". Should match the number of indices in the polygon (GetIndexCountOfPolygon), if less not all indices are returned, if greater memory is wasted
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPolygonIndices) (_In_ DWORD nPolygonIndex, _Out_ DWORD *pPolygonIndices, _In_ DWORD nBufferCount) LIB3MFABSTRACT;

		/**
		* Get a vertex of the slice
		* @param[in] nIndex index of the vertex to get
		* @param[out] pVertex MODELSLICEVERTEX structure to be filled
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetVertex) (_In_ DWORD nIndex, _Out_ MODELSLICEVERTEX *pVertex) LIB3MFABSTRACT;
  };

	/**********************************************************************************************************
	*  ILib3MFSliceStack encapsulates all methods for handling slice stacks in 3MF
	*
	***********************************************************************************************************/
	LIB3MFINTERFACE(ILib3MFSliceStack, ILib3MFModelResource, CLSID_Lib3MF_SliceStack) {
    LIB3MFPUBLIC(ILib3MFSliceStack)

		/**
		* Adds a slice to the slicestack
		* @param[in] fTopZ upper Z coordinate of the slice
		* @param ppSliceObject returns the newly created slice object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddSlice) (_In_ FLOAT fTopZ, _Outptr_ ILib3MFSlice ** ppSliceObject) LIB3MFABSTRACT;

		/**
		* Query a slice from the slice stack
		* @param[in] nSliceIndex the index of the slice to query
		* @param[out] ppSliceObject returns the slice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetSlice) (_In_ DWORD nSliceIndex, _Outptr_ ILib3MFSlice **ppSliceObject) LIB3MFABSTRACT;

		/**
		* Get the number of slices on the slice stack
		* @param[out] pnSliceCount returns the number of slices on the slice stack
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetSliceCount) (_Out_ DWORD * pnSliceCount) LIB3MFABSTRACT;

		/**
		* Get the lower Z-Coordinate of the slice stack
		* @param[out] pfBottomZ the lower Z-Coordinate the slice stack
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBottomZ) (_Out_ FLOAT * pfBottomZ) LIB3MFABSTRACT;

		/**
		* Get the resource id of the slice stack
		* @param[out]pnResourceID returns the resource id
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetResourceID) (_Out_ DWORD * pnResourceID) LIB3MFABSTRACT;

		/**
		* Specify whether the slice stack should be stored in a separate model file within the 3MF-file as a slice ref
		* @param[in] bUsesSliceRef flag whether to use a slice-ref or not
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetUsesSliceRef) (_In_ BOOL bUsesSliceRef) LIB3MFABSTRACT;

		/**
		* Get the specification, whether the slice stack should be stored in a separate model file within the 3MF-file as a slice ref
		* @param[out] pbUsesSliceRef string for the slice reference
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetUsesSliceRef) (_Out_ BOOL *pbUsesSliceRef) LIB3MFABSTRACT;
		
	};

	/**********************************************************************************************************
	*  ILib3MFModelMeshObject encapsulates all methods for handling 3MF mesh objects.
	*
	***********************************************************************************************************/
	LIB3MFINTERFACE(ILib3MFModelMeshObject, ILib3MFModelObjectResource, CLSID_Lib3MF_ModelMeshObject) {
		LIB3MFPUBLIC(ILib3MFModelMeshObject)

		/**
		* Returns the vertex count of a mesh object
		*
		* @param[out] pnVertexCount filled with the vertex count
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetVertexCount) (_Out_ DWORD * pnVertexCount) LIB3MFABSTRACT;

		/**
		* Returns coordinates of a single vertex of a mesh object
		*
		* @param[in] nIndex Index of the vertex (0 to vertexcount - 1)
		* @param[out] pVertex filled with the vertex coordinates
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetVertex) (_In_ DWORD nIndex, _Out_ MODELMESHVERTEX * pVertex) LIB3MFABSTRACT;

		/**
		* Sets the coordinates of a single vertex of a mesh object
		*
		* @param[in] nIndex Index of the vertex (0 to vertexcount - 1)
		* @param[in] pVertex contains the vertex coordinates
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetVertex) (_In_ DWORD nIndex, _In_ MODELMESHVERTEX * pVertex) LIB3MFABSTRACT;

		/**
		* Adds a single vertex to a mesh object
		*
		* @param[in] pVertex contains the vertex coordinates
		* @param[out] pnIndex filled with the new Index of the vertex
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddVertex) (_In_ MODELMESHVERTEX * pVertex, _Out_opt_ DWORD * pnIndex) LIB3MFABSTRACT;

		/**
		* Retrieves all vertex coordinates of a mesh object
		*
		* @param[out] pVertices buffer filled with the vertex coordinates
		* @param[in] nBufferSize size of the buffer in elements, must be at least vertexcount
		* @param[out] pnVertexCount returns how many vertices have been written
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetVertices) (_Out_ MODELMESHVERTEX * pVertices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnVertexCount) LIB3MFABSTRACT;

		/**
		* Returns the triangle count of a mesh object
		*
		* @param[out] pnTriangleCount filled with the triangle count
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTriangleCount) (_Out_ DWORD * pnTriangleCount) LIB3MFABSTRACT;

		/**
		* Returns indices of a single triangle of a mesh object
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[out] pTriangle filled with the triangle indices
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTriangle) (_In_ DWORD nIndex, _Out_ MODELMESHTRIANGLE * pTriangle) LIB3MFABSTRACT;

		/**
		* Sets the indices of a single triangle of a mesh object
		*
		* @param[in] nIndex Index of the triangle (0 to trianglecount - 1)
		* @param[in] pTriangle contains the triangle indices
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetTriangle) (_In_ DWORD nIndex, _In_ MODELMESHTRIANGLE * pTriangle) LIB3MFABSTRACT;

		/**
		* Adds a single triangle to a mesh object
		*
		* @param[in] pTriangle contains the triangle indices
		* @param[out] pnIndex filled with the new Index of the vertex
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddTriangle) (_In_ MODELMESHTRIANGLE * pTriangle, _Out_opt_ DWORD * pnIndex) LIB3MFABSTRACT;

		/**
		* Retrieves all triangle indices of a mesh object
		*
		* @param[out] pIndices buffer filled with the triangle indices
		* @param[in] nBufferSize size of the buffer in elements, must be at least triangle count
		* @param[out] pnTriangleCount returns how many triangles have been written
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTriangleIndices) (_Out_ MODELMESHTRIANGLE * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnTriangleCount) LIB3MFABSTRACT;

		/**
		* Sets the whole geometry of a mesh object
		*
		* @param[in] pVertices Array of vertex coordinates
		* @param[in] nVertexCount Size of the vertex array
		* @param[in] pTriangles Array of triangle indices
		* @param[in] nTriangleCount Size of the triangle array
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetGeometry) (_In_ MODELMESHVERTEX * pVertices, _In_ DWORD nVertexCount, _In_ MODELMESHTRIANGLE * pTriangles, _In_ DWORD nTriangleCount) LIB3MFABSTRACT;

		// API of beam extension
		/**
		* Returns the minimal length of beams for the beamlattice
		*
		* @param[out] pdMinLength minimal length of beams for the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeamLatticeMinLength) (_Out_ DOUBLE * pdMinLength) LIB3MFABSTRACT;
		
		/**
		* Sets the minimal length of beams for the beamlattice
		*
		* @param[in] dMinLength minimal length of beams for the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBeamLatticeMinLength) (_In_ DOUBLE  dMinLength) LIB3MFABSTRACT;
		
		/**
		* Returns the default radius for the beamlattice
		*
		* @param[out] pdRadius precission of the beams in the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeamLatticeRadius) (_Out_ DOUBLE * pdRadius) LIB3MFABSTRACT;
		
		/**
		* Sets the default radius for the beamlattice
		*
		* @param[in] dRadius default radius of the beams in the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBeamLatticeRadius) (_In_ DOUBLE  dRadius) LIB3MFABSTRACT;
		
		/**
		* Returns the default capping mode for the beamlattice
		*
		* @param[out] peCapMode default eModelBeamLatticeCapMode of the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeamLatticeCapMode) (_Out_ eModelBeamLatticeCapMode * peCapMode) LIB3MFABSTRACT;
		
		/**
		* Sets the default capping mode for the beamlattice
		*
		* @param[in] eCapMode default eModelBeamLatticeCapMode of the beamlattice
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBeamLatticeCapMode) (_In_ eModelBeamLatticeCapMode eCapMode) LIB3MFABSTRACT;
		
		/**
		* Returns the clipping mode and the clipping-mesh for the beamlattice of this mesh
		*
		* @param[out] pClipMode contains the clip mode of this mesh
		* @param[out] pnResourceID filled with the resourceID of the clipping mesh-object or a undefined value if pClipMode is MODELBEAMLATTICECLIPMODE_NONE
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeamLatticeClipping) (_Out_ eModelBeamLatticeClipMode * peClipMode, _Out_ DWORD *pnResourceID) LIB3MFABSTRACT;
		
		/**
		* Sets the clipping mode and the clipping-mesh for the beamlattice of this mesh
		*
		* @param[in] pMeshObject Mesh Object Instance
		* @param[in] eClipMode contains the clip mode of this mesh
		* @param[in] nResourceID the resourceID of the clipping mesh-object. This mesh-object has to be defined before setting the Clipping
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBeamLatticeClipping) (_In_ eModelBeamLatticeClipMode eClipMode, _In_ DWORD nResourceID) LIB3MFABSTRACT;

		/**
		* Sets the representation-mesh for the beamlattice of this mesh
		*
		* @param[in] nResourceID the resourceID of the representation mesh-object. This mesh-object has to be defined before setting the representation mesh. Set "0" to unset the representation mesh.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBeamLatticeRepresentation) (_In_ DWORD nResourceID) LIB3MFABSTRACT;

		/**
		* Returns the representation-mesh for the beamlattice of this mesh
		*
		* @param[out] pbHasRepresentation flag whether the beamlattice has a representation mesh.
		* @param[out] pnResourceID filled with the resourceID of the representation mesh-object.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeamLatticeRepresentation) (_Out_ BOOL *pbHasRepresentation, _Out_ DWORD *pnResourceID) LIB3MFABSTRACT;

		/**
		* Returns the beam count of a mesh object
		*
		* @param[out] pnBeamCount filled with the beam count
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeamCount) (_Out_ DWORD * pnBeamCount) LIB3MFABSTRACT;

		/**
		* Returns indices, radii and capmodes of a single beam of a mesh object
		*
		* @param[in] nIndex Index of the beam (0 to beamcount - 1)
		* @param[out] pBeam filled with the beam indices, radii and capmodes
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeam) (_In_ DWORD nIndex, _Out_ MODELMESHBEAM * pBeam) LIB3MFABSTRACT;

		/**
		* Sets the indices, radii and capmodes of a single beam of a mesh object
		*
		* @param[in] nIndex Index of the beam (0 to beamcount - 1)
		* @param[in] pBeam contains the node indices, radii and capmodes
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBeam) (_In_ DWORD nIndex, _In_ MODELMESHBEAM * pBeam) LIB3MFABSTRACT;

		/**
		* Adds a single beam to a mesh object
		*
		* @param[in] pBeam contains the node indices, radii and capmodes
		* @param[out] pnIndex filled with the new Index of the beam
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddBeam) (_In_ MODELMESHBEAM * pBeam, _Out_opt_ DWORD * pnIndex) LIB3MFABSTRACT;

		/**
		* Sets all beam indices, raddi and capmodes of a mesh object
		*
		* @param[in] pIndices buffer with the beam indices
		* @param[in] nBufferSize size of the buffer in elements
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBeamIndices) (_In_ MODELMESHBEAM * pIndices, _In_ DWORD nBufferSize) LIB3MFABSTRACT;
		
		/**
		* Retrieves all beam indices of a mesh object
		*
		* @param[out] pIndices buffer filled with the beam indices
		* @param[in] nBufferSize size of the buffer in elements, must be at least beam count
		* @param[out] pnBeamCount returns how many beams have been written
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeamIndices) (_Out_ MODELMESHBEAM * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnBeamCount) LIB3MFABSTRACT;

		/**
		* Returns the number of beamsets of a mesh object
		*
		* @param[out] pnCount filled with the beamset count
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeamSetCount) (_Out_ DWORD * pnBeamSetCount) LIB3MFABSTRACT;
		
		/**
		* Adds an empty beamset to a mesh object
		*
		* @param[out] ppBeamSet pointer to the new beamset
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddBeamSet) (_Outptr_ ILib3MFModelMeshBeamSet ** ppBeamSet) LIB3MFABSTRACT;
		
		/**
		* Returns a beamset of a mesh object
		*
		* @param[in] nIndex index of the requested beamset (0 ... beamsetcount-1)
		* @param[out] ppBeamSet pointer to the requested beamset
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBeamSet) (_In_ DWORD nIndex, _Outptr_ ILib3MFModelMeshBeamSet ** ppBeamSet) LIB3MFABSTRACT;

		
		/* Property handling */

		/**
		* creates a property handler for the mesh
		*
		* @param[out] ppPropertyHandler returns a property handler instance for the mesh.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(CreatePropertyHandler) (_Outptr_ ILib3MFPropertyHandler ** ppPropertyHandler) LIB3MFABSTRACT;

		/**
		* creates a property handler for a specific multiproperty channel of a mesh
		*
		* @param[in] nChannel Channel Index
		* @param[out] ppPropertyHandler returns a property handler instance for the mesh.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(CreateMultiPropertyHandler) (_In_ DWORD nChannel, _Outptr_ ILib3MFPropertyHandler ** ppPropertyHandler) LIB3MFABSTRACT;

		/**
		* Retrieves, if an object describes a topologically oriented and manifold mesh, according to the core spec
		*
		* @param[out] pbIsOrientedAndManifold returns, if the object is oriented and manifold
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(IsManifoldAndOriented) (_Out_ BOOL * pbIsOrientedAndManifold) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelComponent encapsulates one component node of a 3MF component object.
	*  It links to other object resources of the same model.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelComponent, ILib3MFBase, CLSID_Lib3MF_ModelComponent) {
		LIB3MFPUBLIC(ILib3MFModelComponent)

		/**
		* Returns the associated resource Instance of the component.
		*
		* @param[out] ppResource returns the associated resource instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetObjectResource) (_Outptr_ ILib3MFModelObjectResource ** ppResource) LIB3MFABSTRACT;

		/**
		* returns, whether a component object has a UUID and, if true, the build item's UUID
		*
		* @param[out] pbHasUUID flag whether the cmponent object has a UUID
		* @param[out] pszBuffer the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetUUIDUTF8) (_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer) LIB3MFABSTRACT;

		/**
		* sets the component object's UUID
		*
		* @param[in] pszUUID the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetUUIDUTF8) (_In_z_ LPCSTR pszUUID) LIB3MFABSTRACT;

		/**
		* Returns the transformation matrix of the component.
		*
		* @param[out] pTransformation filled with the component transformation matrix
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTransform) (_Out_ MODELTRANSFORM * pTransformation) LIB3MFABSTRACT;

		/**
		* Sets the transformation matrix of the component.
		*
		* @param[in] pTransformation new transformation matrix
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetTransform) (_In_opt_ MODELTRANSFORM * pTransformation) LIB3MFABSTRACT;

		/**
		* Returns the associated Resource ID of the component.
		*
		* @param[out] pnResourceID returns the associated Resource ID
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetObjectResourceID) (_Out_ DWORD * pnResourceID) LIB3MFABSTRACT;

		/**
		* Returns, if the component has a different transformation than the identity matrix
		*
		* @param[out] pbHasTransform if true is returned, the transformation is not equal to the identity
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(HasTransform) (_Out_ BOOL * pbHasTransform) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelComponentsObject encapsulates all methods for handling 3MF component objects.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelComponentsObject, ILib3MFModelObjectResource, CLSID_Lib3MF_ModelComponentsObject) {
		LIB3MFPUBLIC(ILib3MFModelComponentsObject)

		/**
		* Adds a new component to a component object
		*
		* @param[in] pObject object to add as component. May not lead to circular references!
		* @param[in] pmTransform optional transform matrix for the component
		* @param[out] ppComponent returns new component instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddComponent) (_In_ ILib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pmTransform, _Outptr_ ILib3MFModelComponent ** ppComponent) LIB3MFABSTRACT;

		/**
		* Retrieves a component from a component object
		*
		* @param[in] nIndex index of the component to retrieve (0 to componentcount - 1)
		* @param[out] ppComponent retrieved component instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetComponent) (_In_ DWORD nIndex, _Outptr_ ILib3MFModelComponent ** ppComponent) LIB3MFABSTRACT;

		/**
		* Retrieves the component count of a component object
		*
		* @param[out] pComponentCount returns the component count
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetComponentCount) (_Out_ DWORD * pComponentCount) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelBuildItem encapsulates all methods for handling 3MF build items.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelBuildItem, ILib3MFBase, CLSID_Lib3MF_ModelBuildItem) {
		LIB3MFPUBLIC(ILib3MFModelBuildItem)

		/**
		* Retrieves the object resource associated to a build item
		*
		* @param[out] ppResource returns the associated resource instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetObjectResource) (_Outptr_ ILib3MFModelObjectResource ** ppResource) LIB3MFABSTRACT;

		/**
		* returns, whether a build item has a UUID and, if true, the build item's UUID
		*
		* @param[out] pbHasUUID flag whether the build item has a UUID
		* @param[out] pszBuffer the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetUUIDUTF8) (_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer) LIB3MFABSTRACT;

		/**
		* sets the build item's UUID
		*
		* @param[in] pszUUID the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetUUIDUTF8) (_In_z_ LPCSTR pszUUID) LIB3MFABSTRACT;

		/**
		* Retrieves the object resource id associated to a build item
		*
		* @param[out] pnID returns the ID of the object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetObjectResourceID) (_Out_ DWORD * pnID) LIB3MFABSTRACT;

		/**
		* Checks, if a build item has a non-identity transformation matrix
		*
		* @param[out] pbHasTransform returns true, if the transformation matrix is not the identity
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(HasObjectTransform) (_Out_ BOOL * pbHasTransform) LIB3MFABSTRACT;

		/**
		* Retrieves a build item's transformation matrix
		*
		* @param[out] pmTransform returns the transformation matrix
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetObjectTransform) (_Out_ MODELTRANSFORM * pmTransform) LIB3MFABSTRACT;

		/**
		* Sets a build item's transformation matrix
		*
		* @param[in] pmTransform new transformation matrix
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetObjectTransform) (_In_ MODELTRANSFORM * pmTransform) LIB3MFABSTRACT;

		/**
		* Retrieves a build item's part number string
		*
		* @param[out] pwszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPartNumber) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Retrieves a build item's part number string (UTF8)
		*
		* @param[out] pszBuffer buffer to fill
		* @param[in] cbBufferSize size of buffer to fill. needs to be at least string length + 1
		* @param[out] pcbNeededChars returns needed characters in buffer
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPartNumberUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Sets a build item's part number string
		*
		* @param[in] pwszPartNumber new part number string for referencing parts from the outside world.
		* @return error code or 0 (success)+
		*/
		LIB3MFMETHOD(SetPartNumber) (_In_z_ LPCWSTR pwszPartNumber) LIB3MFABSTRACT;

		/**
		* Sets a build item's part number string (UTF8)
		*
		* @param[in] pwszPartNumber new part number string for referencing parts from the outside world.
		* @return error code or 0 (success)+
		*/
		LIB3MFMETHOD(SetPartNumberUTF8) (_In_z_ LPCSTR pszPartNumber) LIB3MFABSTRACT;

		/**
		* Retrieves an internal handle of the build item. This 32bit number is unique throughout the model, but only valid
		* for in-memory use of this instance.
		*
		* @param[out] ppHandle returns the handle
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetHandle) (_Out_ DWORD * pHandle) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelBuildItemIterator is a helper class to iterate through arbitrary lists of 3MF build items
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelBuildItemIterator, ILib3MFBase, CLSID_Lib3MF_ModelBuildItemIterator) {
		LIB3MFPUBLIC(ILib3MFModelBuildItemIterator)

		/**
		* Iterates to the next build item in the list.
		*
		* @param[out] pbHasNext returns, if there is a build item to use.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(MoveNext) (_Out_ BOOL * pbHasNext) LIB3MFABSTRACT;

		/**
		* Iterates to the previous build item in the list.
		*
		* @param[out] pbHasPrevious returns, if there is a resource to use.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(MovePrevious) (_Out_ BOOL * pbHasPrevious) LIB3MFABSTRACT;

		/**
		* Returns the build item the iterator points at.
		*
		* @param[out] ppBuildItemInstance returns the build item instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetCurrent) (_Outptr_ ILib3MFModelBuildItem ** ppBuildItemInstance) LIB3MFABSTRACT;

		/**
		* Creates a new build item iterator with the same build item list.
		*
		* @param[out] ppIterator returns the cloned Iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(Clone) (_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModel is the basic instance owning all In-Memory elements of a 3MF file.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModel, ILib3MFBase, CLSID_Lib3MF_Model) {
		LIB3MFPUBLIC(ILib3MFModel)

		/**
		* sets the units of a model
		*
		* @param[in] Unit enum value for the model unit (see NMR_ModelTypes.h for details)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetUnit) (_In_ DWORD Unit) LIB3MFABSTRACT;

		/**
		* retrieves the units of a model
		*
		* @param[out] pUnit return enum value for of model's unit (see NMR_ModelTypes.h for details)
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetUnit) (_Out_ DWORD * pUnit) LIB3MFABSTRACT;

		/**
		* sets the language of a model
		*
		* @param[in] pwszLanguage Language string identifier
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetLanguage) (_In_z_ LPCWSTR pwszLanguage) LIB3MFABSTRACT;

		/**
		* sets the language of a model (UTF8)
		*
		* @param[in] pszLanguage Language string identifier
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetLanguageUTF8) (_In_z_ LPCSTR pszLanguage) LIB3MFABSTRACT;

		/**
		* retrieves the language of a model
		*
		* @param[out] pwszBuffer Buffer to write into
		* @param[in] cbBufferSize Buffer size
		* @param[out] pcbNeededChars returns chars which are necessary to store the language string
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetLanguage) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* retrieves the language of a model (UTF8)
		*
		* @param[out] pszBuffer Buffer to write into
		* @param[in] cbBufferSize Buffer size
		* @param[out] pcbNeededChars returns chars which are necessary to store the language string
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetLanguageUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* creates a model writer instance for a specific file type
		*
		* @param[in] pszWriterClass string identifier for the file (ASCII string, currently "stl" and "3mf")
		* @param[out] ppWriter returns the writer instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(QueryWriter) (_In_z_ LPCSTR pszWriterClass, _Outptr_ ILib3MFModelWriter ** ppWriter) LIB3MFABSTRACT;

		/**
		* creates a model reader instance for a specific file type
		*
		* @param[in] pszWriterClass string identifier for the file (ASCII string,  currently "stl" and "3mf")
		* @param[out] ppReader returns the reader instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(QueryReader) (_In_z_ LPCSTR pszReaderClass, _Outptr_ ILib3MFModelReader ** ppReader) LIB3MFABSTRACT;

		/**
		* finds a model resource by its id
		*
		* @param[in] nResourceID Resource ID
		* @param[out] ppResource returns the resource instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetResourceByID) (_In_ DWORD nResourceID, _Outptr_ ILib3MFModelResource ** ppResource) LIB3MFABSTRACT;

		/**
		* finds a model 2d texture by its id
		*
		* @param[in] nResourceID Resource ID
		* @param[out] ppTexture returns the texture resource instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTexture2DByID) (_In_ DWORD nResourceID, _Outptr_ ILib3MFModelTexture2D ** ppTexture) LIB3MFABSTRACT;

		/**
		* finds a base material by its id
		*
		* @param[in] nResourceID Resource ID
		* @param[out] ppMaterial returns the base material instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBaseMaterialByID) (_In_ DWORD nResourceID, _Outptr_ ILib3MFModelBaseMaterial ** ppMaterial) LIB3MFABSTRACT;

		/**
		* finds a mesh object resource by its id
		*
		* @param[in] nResourceID Resource ID
		* @param[out] ppMeshObject returns the resource instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMeshObjectByID) (_In_ DWORD nResourceID, _Outptr_ ILib3MFModelMeshObject ** ppMeshObject) LIB3MFABSTRACT;

		/**
		* finds a components object resource by its id
		*
		* @param[in] nResourceID Resource ID
		* @param[out] ppComponentsObject returns the resource instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetComponentsObjectByID) (_In_ DWORD nResourceID, _Outptr_ ILib3MFModelComponentsObject ** ppComponentsObject) LIB3MFABSTRACT;

		/**
		* returns, whether the build has a UUID and, if true, the build's UUID
		*
		* @param[out] pbHasUUID flag whether the build has a UUID
		* @param[out] pszBuffer the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBuildUUIDUTF8) (_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer) LIB3MFABSTRACT;

		/**
		* sets the build's UUID
		*
		* @param[in] pszUUID the UUID as string of the form "xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx"
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBuildUUIDUTF8) (_In_z_ LPCSTR pszUUID) LIB3MFABSTRACT;
		
		/**
		* creates a build item iterator instance with all build items
		*
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBuildItems) (_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator) LIB3MFABSTRACT;

		/**
		* creates a resource iterator instance with all resources
		*
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetResources) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator) LIB3MFABSTRACT;

		/**
		* creates a resource iterator instance with all object resources
		*
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetObjects) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator) LIB3MFABSTRACT;

		/**
		* creates a resource iterator instance with all mesh object resources
		*
		* @param[in] pModel Model instance
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMeshObjects) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator) LIB3MFABSTRACT;

		/**
		* creates a resource iterator instance with all component object resources
		*
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetComponentsObjects) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator) LIB3MFABSTRACT;

		/**
		* creates a resource iterator instance with all 2D texture resources
		*
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(Get2DTextures) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator) LIB3MFABSTRACT;

		/**
		* creates a resource iterator instance with all base materials
		*
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetBaseMaterials) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator) LIB3MFABSTRACT;

		/**
		* merges all components and objects which are referenced by a build item. The memory is duplicated and a
		* new model is created.
		*
		* @param[out] ppMergedModel returns the merged model instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(MergeToModel) (_Outptr_ ILib3MFModel ** ppMergedModel) LIB3MFABSTRACT;

		/**
		* adds an empty mesh object to the model
		*
		* @param[out] ppMeshObject returns the mesh object instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddMeshObject) (_Outptr_ ILib3MFModelMeshObject ** ppMeshObject) LIB3MFABSTRACT;

		/**
		* adds an empty component object to the model
		*
		* @param[out] ppComponentsObject returns the component object instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddComponentsObject) (_Outptr_ ILib3MFModelComponentsObject ** ppComponentsObject) LIB3MFABSTRACT;

		/**
		* adds a texture2d resource to the model. Its path is given by that of an existing attachment.
		*
		* @param[in] pTextureAttachment attachment containing the image data
		* @param[out] ppTextureInstance returns the new texture instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddTexture2DFromAttachment) (_In_z_ ILib3MFModelAttachment* pTextureAttachment, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance) LIB3MFABSTRACT;

		/**
		* adds a texture2d resource to the model
		*
		* @param[in] pwszPath Package path of the texture
		* @param[out] ppTextureInstance returns the new texture instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddTexture2D) (_In_z_ LPCWSTR pwszPath, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance) LIB3MFABSTRACT;

		/**
		* adds a texture2d resource to the model (UTF8)
		*
		* @param[in] pszPath Package path of the texture
		* @param[out] ppTextureInstance returns the new texture instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddTexture2DUTF8) (_In_z_ LPCSTR pszPath, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance) LIB3MFABSTRACT;

		/**
		* adds an empty basematerials resource to the model
		*
		* @param[out] ppBaseMaterialInstance returns the new base material instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddBaseMaterialGroup) (_Outptr_ ILib3MFModelBaseMaterial ** ppBaseMaterialInstance) LIB3MFABSTRACT;

		/**
		* adds a build item to the model
		*
		* @param[in] pObject Object instance
		* @param[in] pTransform Transformation matrix
		* @param[out] ppBuildItem returns the build iteminstance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddBuildItem) (_In_ ILib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pTransform, _Outptr_ ILib3MFModelBuildItem ** ppBuildItem) LIB3MFABSTRACT;

		/**
		* removes a build item from the model
		*
		* @param[in] pBuildItem Build item instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveBuildItem) (_In_ ILib3MFModelBuildItem * pBuildItem) LIB3MFABSTRACT;

		/**
		* returns the number of metadata strings of a model
		*
		* @param[out] pnCount returns the number metadata strings.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMetaDataCount) (_Out_ DWORD * pnCount) LIB3MFABSTRACT;

		/**
		* returns a metadata key of a model
		*
		* @param[in] nIndex Index of the Metadata
		* @param[out] pwszBuffer filled with the key of the Metadata
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMetaDataKey) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* returns a metadata key of a model (UTF8)
		*
		* @param[in] nIndex Index of the Metadata
		* @param[out] pszBuffer filled with the key of the Metadata
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMetaDataKeyUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* returns a metadata value of a model
		*
		* @param[in] nIndex Index of the Metadata
		* @param[out] pwszBuffer filled with the value of the Metadata
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMetaDataValue) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* returns a metadata value of a model (UTF8)
		*
		* @param[in] nIndex Index of the Metadata
		* @param[out] pszBuffer filled with the value of the Metadata
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMetaDataValueUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* adds a new metadata to the model
		*
		* @param[in] pszwKey Metadata Key.
		* @param[in] pszwValue Metadata Value.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddMetaData) (_In_ LPCWSTR pszwKey, _In_ LPCWSTR pszwValue) LIB3MFABSTRACT;

		/**
		* adds a new metadata to the model (UTF8)
		*
		* @param[in] pszKey Metadata Key.
		* @param[in] pszValue Metadata Value.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddMetaDataUTF8) (_In_ LPCSTR pszKey, _In_ LPCSTR pszValue) LIB3MFABSTRACT;

		/**
		* removes a metadata from the model
		*
		* @param[in] nIndex Index of the metadata
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveMetaData) (_In_ DWORD nIndex) LIB3MFABSTRACT;


		/**
		* adds an attachment stream to the model. The OPC part will be related to the model stream with a certain relationship type.
		*
		* @param[in] pwszURI Path of the attachment
		* @param[in] pwszRelationShipType Relationship type of the attachment
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddAttachment) (_In_z_ LPWSTR pwszURI, _In_z_ LPWSTR pwszRelationShipType, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance) LIB3MFABSTRACT;

		/**
		* adds an attachment stream to the model. The OPC part will be related to the model stream with a certain relationship type.
		*
		* @param[in] pszURI Path of the attachment (UTF8)
		* @param[in] pszRelationShipType Relationship type of the attachment (UTF8)
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddAttachmentUTF8) (_In_z_ LPSTR pszURI, _In_z_ LPSTR pszRelationShipType, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance) LIB3MFABSTRACT;


		/**
		* retrieves an attachment stream object from the model. 
		*
		* @param[in] nIndex Index of the attachment stream
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetAttachment) (_In_ DWORD nIndex, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance) LIB3MFABSTRACT;

		/**
		* retrieves an attachment stream object from the model.
		*
		* @param[in] pwszURI Path URI in the package
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(FindAttachment) (_In_z_ LPWSTR pwszURI, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance) LIB3MFABSTRACT;

		/**
		* retrieves an attachment stream object from the model.
		*
		* @param[in] pszURI Path URI in the package (UTF8)
		* @param[out] ppAttachmentInstance Instance of the attachment object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(FindAttachmentUTF8) (_In_z_ LPSTR pszURI, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance) LIB3MFABSTRACT;

		/**
		* retrieves the number of attachments of the model.
		*
		* @param[out] pnCount Returns the number of attachments
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetAttachmentCount) (_Out_ DWORD * pnCount) LIB3MFABSTRACT;

		/**
		* retrieves the size of an attachment in bytes
		*
		* @param[in] nIndex Index of the attachment stream
		* @param[out] pnSize Returns the size of the attachment
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetAttachmentSize) (_In_ DWORD nIndex, _Out_ UINT64 * pnSize) LIB3MFABSTRACT;

		/**
		* retrieves the path URI of an attachment
		*
		* @param[in] nIndex Index of the attachment stream
		* @param[out] pwszBuffer Buffer to write into, may be null to determine needed length
		* @param[in] cbBufferSize Size of the given buffer
		* @param[out] pcbNeededChars Returns number of bytes written or number of bytes needed to write.
		
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetAttachmentPath) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* retrieves the path URI of an attachment (UTF8)
		*
		* @param[in] nIndex Index of the attachment stream
		* @param[out] pszBuffer Buffer to write into, may be null to determine needed length
		* @param[in] cbBufferSize Size of the given buffer
		* @param[out] pcbNeededChars Returns number of bytes written or number of bytes needed to write.

		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetAttachmentPathUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Get the attachment to the OPC package containing the package thumbnail
		*
		* @param[in] bCreateIfNotExisting create the attachment if it does not exist
		* @param[out] ppAttachmentInstance Instance of the thumbnailattachment object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetPackageThumbnailAttachment) (_In_ BOOL bCreateIfNotExisting, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance) LIB3MFABSTRACT;

		/**
		* Remove the attachment to the OPC package containing the package thumbnail
		*
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemovePackageThumbnailAttachment) () LIB3MFABSTRACT;

		/**
		* adds a new Content Type to the model
		*
		* @param[in] pszwExtension File Extension
		* @param[in] pszwContentType Content Type Identifier
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddCustomContentType) (_In_ LPCWSTR pszwExtension, _In_ LPCWSTR pszwContentType) LIB3MFABSTRACT;

		/**
		* adds a new Content Type to the model (UTF8 version)
		*
		* @param[in] pszExtension File Extension
		* @param[in] pszContentType Content Type Identifier
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddCustomContentTypeUTF8) (_In_ LPCSTR pszExtension, _In_ LPCSTR pszContentType) LIB3MFABSTRACT;

		/**
		* removes a custom Content Type from the model
		*
		* @param[in] pszwExtension File Extension
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveCustomContentType) (_In_ LPCWSTR pszwExtension) LIB3MFABSTRACT;

		/**
		* removes a custom Content Type from the model (UTF8 version)
		*
		* @param[in] pModel Model instance
		* @param[in] pszExtension File Extension
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(RemoveCustomContentTypeUTF8) (_In_ LPCSTR pszExtension) LIB3MFABSTRACT;

		/**
		* Adds a slicestack to a model
		* @param[in] nBottomZ Bottom Z value of the slicestack
		* @param[out] ppSliceStackObject returns the new slice stack object
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddSliceStack) (_In_opt_ FLOAT nBottomZ, _Out_opt_ ILib3MFSliceStack ** ppSliceStack) LIB3MFABSTRACT;

	};

	// ILib3MFModelFactory is the global factory class for model instances.

	LIB3MFINTERFACE(ILib3MFModelFactory, ILib3MFBase, CLSID_Lib3MF_ModelFactory) {

		LIB3MFPUBLIC(ILib3MFModelFactory)

		/**
		* creates an empty model instance
		*
		* @param[out] ppModel returns created model instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(CreateModel) (_Outptr_ ILib3MFModel ** ppModel) LIB3MFABSTRACT;

		/**
		* retrieves the current version of the 3MF implementation and specification
		*
		* @param[out] pMajorVersion returns the major version of the Specification
		* @param[out] pMinorVersion returns the minor version of the Specification
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetSpecVersion) (_Out_ DWORD * pMajorVersion, _Out_ DWORD * pMinorVersion) LIB3MFABSTRACT;

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
		LIB3MFMETHOD(GetInterfaceVersion) (_Out_ DWORD * pInterfaceVersionMajor, _Out_ DWORD * pInterfaceVersionMinor, _Out_ DWORD * pInterfaceVersionMicro) LIB3MFABSTRACT;
		
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
		LIB3MFMETHOD(QueryExtension) (_In_z_ LPCWSTR pwszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion) LIB3MFABSTRACT;

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
		LIB3MFMETHOD(QueryExtensionUTF8) (_In_z_ LPCSTR pszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion) LIB3MFABSTRACT;
		
		/**
		* Return an English text for a progress identifier
		* Note: this is the only function you can call from your callback function.
		*
		* @param[in] progressIdentifier the progress identifier that is passed to the callback function
		* @param[out] progressMessage English text for the progress identifier
		* @return error code or 0 (success)
		**/
		LIB3MFMETHOD(RetrieveProgressMessage) (_In_ int progressIdentifier, _Out_ const char ** progressMessage) LIB3MFABSTRACT;
	};

}

#endif // __NMR_COMINTERFACES
