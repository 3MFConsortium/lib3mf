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

Abstract:

NMR_COMInterfaces.h defines the Loading and Saving COM Interfaces, which are
shown to the outside world.

--*/

#ifndef __NMR_COMINTERFACES
#define __NMR_COMINTERFACES

#ifdef NMR_COM_EMULATION
#include "Common/Platform/NMR_COM_Emulation.h"
#else
#include "Common/Platform/NMR_COM_Native.h"
#endif

#include "Common/Platform/NMR_SAL.h"
#include "Common/Platform/NMR_WinTypes.h"
#include "Model/Classes/NMR_ModelTypes.h"


#ifndef __GCC
#include "Common/Platform/NMR_IStream.h"
#endif




// Define Class IDs
#define CLSID_Lib3MF_Base                   "8F1D86BF-CD19-4710-BC19-3C70D095642F"
#define CLSID_Lib3MF_ModelWriter            "58DDBE95-34D8-4F77-BFDC-1570F849BFF2"
#define CLSID_Lib3MF_ModelReader            "A7FE2C65-0121-40AA-A0AF-48C9D2287A55"
#define CLSID_Lib3MF_ModelResource          "BF38DC27-4169-4AA3-BFD1-35C8D046C0A8"
#define CLSID_Lib3MF_ModelResourceIterator  "11845233-4A6A-4B10-8B55-00A84048959C"
#define CLSID_Lib3MF_ModelObjectResource    "FB9F7E2C-D8A3-4C84-831E-76928A91CC7B"
#define CLSID_Lib3MF_ModelTexture2D         "29DD25F2-607D-4804-A737-ED2CBC7199F2"
#define CLSID_Lib3MF_ModelBaseMaterial      "943648CF-B9BB-40AD-8264-79C69195A116"
#define CLSID_Lib3MF_ModelMeshObject        "8B7FE33C-8EF0-4927-A106-1C069B49B01D"
#define CLSID_Lib3MF_ModelComponent         "99F7DB2E-9A6F-4DD5-9F96-27DDAF32A0CF"
#define CLSID_Lib3MF_ModelComponentsObject  "F4125D0E-297A-41E4-84F0-9D7110C46341"
#define CLSID_Lib3MF_ModelBuildItem         "13BA279E-9CF4-41BF-B3A7-74EDB6E7A0F1"
#define CLSID_Lib3MF_ModelBuildItemIterator "65E8B244-D2D4-4363-9751-938E28B968D8"
#define CLSID_Lib3MF_Model                  "BC58EF48-22A0-4A52-94ED-566C07017C49"
#define CLSID_Lib3MF_ModelFactory           "9A41DB91-678E-4DC5-BF25-D49FE8B176CC"
#define CLSID_Lib3MF_ModelThumbnail         "35F56BCA-784E-46D5-BC33-4608B405D99B"
#define CLSID_Lib3MF_ModelThumbnailIterator "26E42A63-BE7D-402B-A638-175A5CCED4A8"
#define CLSID_Lib3MF_PropertyHandler        "1092CDE7-9A6D-48BC-821B-EBADCF50BC6A"
#define CLSID_Lib3MF_DefaultPropertyHandler "A62108EE-FB26-44C2-90CB-FBEE9D2823E5"


#ifndef __GCC
static const IID IID_Lib3MF_Base = { 0x8f1d86bf, 0xcd19, 0x4710, { 0xbc, 0x19, 0x3c, 0x70, 0xd0, 0x95, 0x64, 0x2f } };
static const IID IID_Lib3MF_ModelWriter = { 0x58ddbe95, 0x34d8, 0x4f77, { 0xbf, 0xdc, 0x15, 0x70, 0xf8, 0x49, 0xbf, 0xf2 } };
static const IID IID_Lib3MF_ModelReader = { 0xa7fe2c65, 0x121, 0x40aa, { 0xa0, 0xaf, 0x48, 0xc9, 0xd2, 0x28, 0x7a, 0x55 } };
static const IID IID_Lib3MF_ModelResource = { 0xbf38dc27, 0x4169, 0x4aa3, { 0xbf, 0xd1, 0x35, 0xc8, 0xd0, 0x46, 0xc0, 0xa8 } };
static const IID IID_Lib3MF_ModelResourceIterator = { 0x11845233, 0x4a6a, 0x4b10, { 0x8b, 0x55, 0x0, 0xa8, 0x40, 0x48, 0x95, 0x9c } };
static const IID IID_Lib3MF_ModelObjectResource = { 0xfb9f7e2c, 0xd8a3, 0x4c84, { 0x83, 0x1e, 0x76, 0x92, 0x8a, 0x91, 0xcc, 0x7b } };
static const IID IID_Lib3MF_ModelTexture2D = { 0x29dd25f2, 0x607d, 0x4804, { 0xa7, 0x37, 0xed, 0x2c, 0xbc, 0x71, 0x99, 0xf2 } };
static const IID IID_Lib3MF_ModelBaseMaterial = { 0x943648cf, 0xb9bb, 0x40ad, { 0x82, 0x64, 0x79, 0xc6, 0x91, 0x95, 0xa1, 0x16 } };
static const IID IID_Lib3MF_ModelMeshObject = { 0x8b7fe33c, 0x8ef0, 0x4927, { 0xa1, 0x6, 0x1c, 0x6, 0x9b, 0x49, 0xb0, 0x1d } };
static const IID IID_Lib3MF_ModelComponent = { 0x99f7db2e, 0x9a6f, 0x4dd5, { 0x9f, 0x96, 0x27, 0xdd, 0xaf, 0x32, 0xa0, 0xcf } };
static const IID IID_Lib3MF_ModelComponentsObject = { 0xf4125d0e, 0x297a, 0x41e4, { 0x84, 0xf0, 0x9d, 0x71, 0x10, 0xc4, 0x63, 0x41 } };
static const IID IID_Lib3MF_ModelBuildItem = { 0x13ba279e, 0x9cf4, 0x41bf, { 0xb3, 0xa7, 0x74, 0xed, 0xb6, 0xe7, 0xa0, 0xf1 } };
static const IID IID_Lib3MF_ModelBuildItemIterator = { 0x65e8b244, 0xd2d4, 0x4363, { 0x97, 0x51, 0x93, 0x8e, 0x28, 0xb9, 0x68, 0xd8 } };
static const IID IID_Lib3MF_Model = { 0xbc58ef48, 0x22a0, 0x4a52, { 0x94, 0xed, 0x56, 0x6c, 0x7, 0x1, 0x7c, 0x49 } };
static const IID IID_Lib3MF_ModelFactory = { 0x9a41db91, 0x678e, 0x4dc5, { 0xbf, 0x25, 0xd4, 0x9f, 0xe8, 0xb1, 0x76, 0xcc } };
static const IID IID_Lib3MF_ModelThumbnail = { 0x35f56bca, 0x784e, 0x46d5, { 0xbc, 0x33, 0x46, 0x8, 0xb4, 0x5, 0xd9, 0x9b } };
static const IID IID_Lib3MF_ModelThumbnailIterator = { 0x26e42a63, 0xbe7d, 0x402b, { 0xa6, 0x38, 0x17, 0x5a, 0x5c, 0xce, 0xd4, 0xa8 } };
static const IID IID_Lib3MF_PropertyHandler = { 0x1092cde7, 0x9a6d, 0x48bc, { 0x82, 0x1b, 0xeb, 0xad, 0xcf, 0x50, 0xbc, 0x6a } };
static const IID IID_Lib3MF_DefaultPropertyHandler = { 0xa62108ee, 0xfb26, 0x44c2, { 0x90, 0xcb, 0xfb, 0xee, 0x9d, 0x28, 0x23, 0xe5 } };



#endif //__GCC

namespace NMR {

	/**********************************************************************************************************
	*  ILib3MFBase is a base interface, which serves as parent for all interfaces
	*  related to the 3MF Library
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFBase, ILib3MFUnknown, CLSID_Lib3MF_Base) {
		LIB3MFPUBLIC(ILib3MFBase)

		/**
		* Returns detailed information of the last known error an object method.
		* The error information is available for every method returning a LIB3MF_FAILED
		* constant.
		*
		* @param[out] pErrorCode Error Code
		* @param[out] pErrorMessage Returns pointer to the error message string, NULL if no error.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR *	pErrorMessage) LIB3MFABSTRACT;
	};

	/**********************************************************************************************************
	*  ILib3MFModelWriter encapsulates an writer class for a writing the model into a specific file type.
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
		LIB3MFMETHOD(WriteToFileUTF8) (_In_z_ LPCSTR pwszFilename) LIB3MFABSTRACT;

#ifndef __GCC
		/**
		* Writes out the model into a COM IStream. Only available on Windows.
		*
		* @param[in] pStream IStream to write into.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToStream) (_In_ IStream * pStream) LIB3MFABSTRACT;
#endif// __GCC
	};

	/**********************************************************************************************************
	*  ILib3MFModelReader encapsulates an reader class for reading a model from a specific file type.
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
		* @param[in] pwszFilename Filename to read from as UTF8 string
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromFileUTF8) (_In_z_ LPCSTR pwszFilename) LIB3MFABSTRACT;

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

#ifndef __GCC
		/**
		* Reads a model from a COM IStream. Only available on Windows.
		*
		* @param[in] pStream IStream to read from
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromStream) (_In_ IStream * pStream) LIB3MFABSTRACT;
#endif //__GCC
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
	*  ILib3MFModelThumbnail encapsulates a thumbnail for the whole 3mf or singular objects
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelThumbnail, ILib3MFBase, CLSID_Lib3MF_ModelThumbnail) {
		LIB3MFPUBLIC(ILib3MFModelThumbnail)

	};




	/**********************************************************************************************************
	*  ILib3MFModelThumbnailIterator is a helper class to iterate through arbitrary lists of 3MF thumbnails
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelThumbnailIterator, ILib3MFBase, CLSID_Lib3MF_ModelThumbnailIterator) {
		LIB3MFPUBLIC(ILib3MFModelThumbnailIterator)

		/**
		* Iterates to the next thumbnail in the list.
		*
		* @param[out] pbHasNext returns, if there is a thumbnail to use.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(MoveNext) (_Out_ BOOL * pbHasNext) LIB3MFABSTRACT;

		/**
		* Iterates to the previous thumbnail in the list.
		*
		* @param[out] pbHasPrevious returns, if there is a thumbnail to use.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(MovePrevious) (_Out_ BOOL * pbHasPrevious) LIB3MFABSTRACT;

		/**
		* Returns the resource the iterator points at.
		*
		* @param[out] ppThumbnailInstance returns the thumbnail instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetCurrent) (_Outptr_ ILib3MFModelThumbnail ** ppThumbnailInstance) LIB3MFABSTRACT;

		/**
		* Creates a new resource iterator with the same resource list.
		*
		* @param[out] ppIterator returns the cloned Iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(Clone) (_Outptr_ ILib3MFModelThumbnailIterator ** ppIterator) LIB3MFABSTRACT;
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
		LIB3MFMETHOD(SetBaseMaterial) (_In_ DWORD nIndex, _In_ ModelResourceID nMaterialGroupID, _In_ DWORD nMaterialIndex) LIB3MFABSTRACT;

		/**
		* Sets the materials of all triangles to specific values.
		*
		* @param[in] pnMaterialGroupIDs array of the material Group IDs. Must have trianglecount entries. If a group ID of 0 is specified.
		* @param[in] pnMaterialIndices array of the corresponding material indices. Must have trianglecount entries.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(SetBaseMaterialArray) (_In_ ModelResourceID * pnMaterialGroupIDs, _In_ DWORD * pnMaterialIndices) LIB3MFABSTRACT;


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
		* This must be a base material .
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
		LIB3MFMETHOD(GetTexture) (_Out_ ModelResourceID * pnTextureID, _Out_ FLOAT * pfU, _Out_ FLOAT * pfV) LIB3MFABSTRACT;

		/**
		* Sets the default 2D texture information of an object.
		*
		* @param[in] nTextureID Sets the default Texture ID of the object. 0 means not default property.
		* @param[in] fU Sets the default U value of the object.
		* @param[in] fV Sets the default V value of the object.
		*/
		LIB3MFMETHOD(SetTexture) (_In_ ModelResourceID nTextureID, _In_ FLOAT fU, _In_ FLOAT fV) LIB3MFABSTRACT;

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
	*  ILib3MFModelTexture2D implements the Texture2D Resources of a 3MF model stream, and allows direct access to the
	*  texture properties and the image data.
	*
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFModelTexture2D, ILib3MFModelResource, CLSID_Lib3MF_ModelTexture2D) {

		LIB3MFPUBLIC(ILib3MFModelTexture2D)

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

#ifndef __GCC
		/**
		* Writes out the texture into a COM IStream. Only available on Windows.
		*
		* @param[in] pStream IStream to write into.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(WriteToStream) (_In_ IStream * pStream) LIB3MFABSTRACT;
#endif// __GCC

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

#ifndef __GCC
		/**
		* Reads a texture from a COM IStream. Only available on Windows.
		*
		* @param[in] pStream IStream to read from
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(ReadFromStream) (_In_ IStream * pStream) LIB3MFABSTRACT;
#endif //__GCC

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
		* Returns the triangle count of a mesh object
		*
		* @param[out] pnTriangleCount filled with the triangle count
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTriangleCount) (_Out_ DWORD * pnTriangleCount) LIB3MFABSTRACT;

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
		* Retrieves all vertex coordinates of a mesh object
		*
		* @param[out] pVertices buffer filled with the vertex coordinates
		* @param[in] nBufferSize size of the buffer in elements, must be at least vertexcount
		* @param[out] pnVertexCount returns how many vertices have been written
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetVertices) (_Out_ MODELMESHVERTEX * pVertices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnVertexCount) LIB3MFABSTRACT;

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
		* creates a thumbnail iterator instance with all thumbnails
		*
		* @param[out] ppIterator returns the iterator instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetThumbnails) (_Outptr_ ILib3MFModelThumbnailIterator ** ppIterator) LIB3MFABSTRACT;


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
		* adds an empty texture2d resource to the model
		*
		* @param[in] pwszPath Package path of the texture
		* @param[out] ppTextureInstance returns the new texture instance
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(AddTexture2D) (_In_z_ LPCWSTR pwszPath, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance) LIB3MFABSTRACT;

		/**
		* adds an empty texture2d resource to the model (UTF8)
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
		* returns the number of texture streams of a model
		*
		* @param[out] pnCount returns the number of texture streams.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTextureStreamCount) (_Out_ DWORD * pnCount) LIB3MFABSTRACT;

		/**
		* returns the size of a texture stream
		*
		* @param[in] nIndex index of the texture stream
		* @param[out] pnSize returns the size of a texture stream in bytes.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTextureStreamSize) (_In_ DWORD nIndex, _Out_ UINT64 * pnSize) LIB3MFABSTRACT;

		/**
		* Returns the path of a texture stream in the 3mf package.
		*
		* @param[in] nIndex Index of the Texture Stream
		* @param[out] pwszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTextureStreamPath) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* Returns the path of a texture stream in the 3mf package. (UTF8)
		*
		* @param[in] nIndex Index of the Texture Stream
		* @param[out] pszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetTextureStreamPathUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

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
		* @param[out] pwszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMetaDataKey) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* returns a metadata key of a model (UTF8)
		*
		* @param[in] nIndex Index of the Metadata
		* @param[out] pszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMetaDataKeyUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* returns a metadata value of a model
		*
		* @param[in] nIndex Index of the Metadata
		* @param[out] pwszBuffer filled with the texture stream path, may be NULL
		* @param[in] cbBufferSize size of pwszBuffer (including trailing 0).
		* @param[out] pcbNeededChars filled with the count of the written bytes, or needed buffer size.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetMetaDataValue) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) LIB3MFABSTRACT;

		/**
		* returns a metadata value of a model (UTF8)
		*
		* @param[in] nIndex Index of the Metadata
		* @param[out] pszBuffer filled with the texture stream path, may be NULL
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
		* this version will increment with each official release of the library,
		* and may be used to ensure API compatibility.
		*
		* @param[out] pInterfaceVersion returns the interface version of the DLL
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetInterfaceVersion) (_Out_ DWORD * pInterfaceVersion) LIB3MFABSTRACT;
	};

}

#endif // __NMR_COMINTERFACES
