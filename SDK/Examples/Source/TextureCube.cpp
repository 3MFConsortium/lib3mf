/*++

© 2017 Autodesk Inc.
© 2015 netfabb GmbH (Original Author)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MICROSOFT AND/OR NETFABB BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

TextureCube.cpp : 3MF Texture Cube creation example

--*/

#ifndef __GNUC__
#include <tchar.h>
#include <Windows.h>
#endif // __GNUC__

#include <iostream>
#include <string>
#include <algorithm>

// Plain C Includes of 3MF Library
#include "NMR_DLLInterfaces.h"

// Use NMR namespace for the interfaces
using namespace NMR;

// Utility functions to create vertices and triangles
MODELMESHVERTEX fnCreateVertex(float x, float y, float z)
{
	MODELMESHVERTEX result;
	result.m_fPosition[0] = x;
	result.m_fPosition[1] = y;
	result.m_fPosition[2] = z;
	return result;
}

MODELMESHTRIANGLE fnCreateTriangle(int v0, int v1, int v2)
{
	MODELMESHTRIANGLE result;
	result.m_nIndices[0] = v0;
	result.m_nIndices[1] = v1;
	result.m_nIndices[2] = v2;
	return result;
}

MODELMESHTEXTURE2D fnCreateTexture(float u1, float v1, float u2, float v2, float u3, float v3, unsigned int textureid)
{
	MODELMESHTEXTURE2D result;
	result.m_fU[0] = u1;
	result.m_fV[0] = v1;
	result.m_fU[1] = u2;
	result.m_fV[1] = v2;
	result.m_fU[2] = u3;
	result.m_fV[2] = v3;
	result.m_nTextureID = textureid;
	return result;
}

LIB3MFRESULT fnLoadModelTexture(PLib3MFModel * pModel, const char * pszPath, const char * pszFile, eModelTexture2DType eType, DWORD * pTextureID)
{
	LIB3MFRESULT hResult = 0;
	PLib3MFModelTexture2D * pTexture2D;

	hResult = lib3mf_model_addtexture2dutf8(pModel, pszPath, &pTexture2D);
	if (hResult != 0)
		return hResult;

	hResult = lib3mf_texture2d_setcontenttype(pTexture2D, eType);
	if (hResult != 0) {
		lib3mf_release(pTexture2D);
		return hResult;
	}

	// Read Texture From File
	hResult = lib3mf_texture2d_readfromfileutf8(pTexture2D, pszFile);
	if (hResult != 0) {
		lib3mf_release(pTexture2D);
		return hResult;
	}

	// Retrieve Texture ID
	hResult = lib3mf_resource_getresourceid(pTexture2D, pTextureID);
	if (hResult != 0) {
		lib3mf_release(pTexture2D);
		return hResult;
	}

	// Release Texture Object from memory (does not delete texture from 3mf!)
	lib3mf_release(pTexture2D);
	return hResult;

}


#ifndef __GNUC__
int _tmain(int argc, _TCHAR* argv[])
#else
int main ()
#endif // __GNUC__
{
	// General Variables
	HRESULT hResult;
	DWORD nInterfaceVersion;
	DWORD nErrorMessage;
	LPCSTR pszErrorMessage;

	// Objects
	PLib3MFModel * pModel;
	PLib3MFModelWriter * p3MFWriter;
	PLib3MFModelMeshObject * pMeshObject;
	PLib3MFModelBuildItem * pBuildItem;
	PLib3MFPropertyHandler * pPropertyHandler;
	PLib3MFDefaultPropertyHandler * pDefaultPropertyHandler;

	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Texture Cube example" << std::endl;
	std::cout << "------------------------------------------------------------------" << std::endl;

	// Check 3MF Library Version
	hResult = lib3mf_getinterfaceversion (&nInterfaceVersion);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not get 3MF Library version: " << std::hex << hResult << std::endl;
		return -1;
	}

	if ((nInterfaceVersion != NMR_APIVERSION_INTERFACE)) {
		std::cout << "invalid 3MF Library version: " << NMR_APIVERSION_INTERFACE << std::endl;
		return -1;
	}

	// Create Model Instance
	hResult = lib3mf_createmodel(&pModel);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create model: " << std::hex << hResult << std::endl;
		return -1;
	}

	// Create Mesh Object
	hResult = lib3mf_model_addmeshobject(pModel, &pMeshObject);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not add mesh object: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pModel, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pModel);
		return -1;
	}

	hResult = lib3mf_object_setnameutf8(pMeshObject, "Colored Box");
	if (hResult != LIB3MF_OK) {
		std::cout << "could not set object name: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pMeshObject, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Create mesh structure of a cube
	MODELMESHVERTEX pVertices[8];
	MODELMESHTRIANGLE pTriangles[12];
	float fSizeX = 100.0f;
	float fSizeY = 100.0f;
	float fSizeZ = 100.0f;

	// Manually create vertices
	pVertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
	pVertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
	pVertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
	pVertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
	pVertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
	pVertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
	pVertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
	pVertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);


	// Manually create triangles
	pTriangles[0] = fnCreateTriangle(2, 1, 0);
	pTriangles[1] = fnCreateTriangle(0, 3, 2);
	pTriangles[2] = fnCreateTriangle(4, 5, 6);
	pTriangles[3] = fnCreateTriangle(6, 7, 4);
	pTriangles[4] = fnCreateTriangle(0, 1, 5);
	pTriangles[5] = fnCreateTriangle(5, 4, 0);
	pTriangles[6] = fnCreateTriangle(2, 3, 7);
	pTriangles[7] = fnCreateTriangle(7, 6, 2);
	pTriangles[8] = fnCreateTriangle(1, 2, 6);
	pTriangles[9] = fnCreateTriangle(6, 5, 1);
	pTriangles[10] = fnCreateTriangle(3, 0, 4);
	pTriangles[11] = fnCreateTriangle(4, 7, 3);

	hResult = lib3mf_meshobject_setgeometry(pMeshObject, pVertices, 8, pTriangles, 12);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not set mesh geometry: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pMeshObject, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Create texture entries for cube
	hResult = lib3mf_meshobject_createpropertyhandler(pMeshObject, &pPropertyHandler);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create property handler: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pMeshObject, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Create Texture Parts
	DWORD nTextureID1 = 0;
	DWORD nTextureID2 = 0;
	DWORD nTextureID3 = 0;
	DWORD nTextureID4 = 0;
	DWORD nTextureID5 = 0;
	DWORD nTextureID6 = 0;

	// add textures to 3mf package
#ifndef TEXTURESPATH
	std::string sFolder = "../Files/";
#else
	std::string sFolder = TEXTURESPATH;
#endif
	std::cout<<"sFolder=\""<<sFolder<<"\"\n";
	hResult = fnLoadModelTexture(pModel, "/3D/Textures/tex1.png", (sFolder+"tex1.png").c_str(), MODELTEXTURETYPE_PNG, &nTextureID1);
	if (hResult == 0) 
		hResult = fnLoadModelTexture(pModel, "/3D/Textures/tex2.png", (sFolder + "tex2.png").c_str(), MODELTEXTURETYPE_PNG, &nTextureID2);
	if (hResult == 0)
		hResult = fnLoadModelTexture(pModel, "/3D/Textures/tex3.png", (sFolder + "tex3.png").c_str(), MODELTEXTURETYPE_PNG, &nTextureID3);
	if (hResult == 0)
		hResult = fnLoadModelTexture(pModel, "/3D/Textures/tex4.png", (sFolder + "tex4.png").c_str(), MODELTEXTURETYPE_PNG, &nTextureID4);
	if (hResult == 0)
		hResult = fnLoadModelTexture(pModel, "/3D/Textures/tex5.png", (sFolder + "tex5.png").c_str(), MODELTEXTURETYPE_PNG, &nTextureID5);
	if (hResult == 0)
		hResult = fnLoadModelTexture(pModel, "/3D/Textures/tex6.png", (sFolder + "tex6.png").c_str(), MODELTEXTURETYPE_PNG, &nTextureID6);

	if (hResult != LIB3MF_OK) {
		std::cout << "could not load model texture: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pMeshObject, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		lib3mf_release(pPropertyHandler);
		return -1;
	}

	// Set Texture Parameters of triangle
	MODELMESHTEXTURE2D sTexture1;
	MODELMESHTEXTURE2D sTexture2;

	// Side 1
	sTexture1 = fnCreateTexture(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, nTextureID1);
	sTexture2 = fnCreateTexture(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, nTextureID1);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 0, &sTexture1);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 1, &sTexture2);

	// Side 2
	sTexture1 = fnCreateTexture(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, nTextureID2);
	sTexture2 = fnCreateTexture(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, nTextureID2);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 2, &sTexture1);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 3, &sTexture2);

	// Side 3
	sTexture1 = fnCreateTexture(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, nTextureID3);
	sTexture2 = fnCreateTexture(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, nTextureID3);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 4, &sTexture1);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 5, &sTexture2);

	// Side 4
	sTexture1 = fnCreateTexture(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, nTextureID4);
	sTexture2 = fnCreateTexture(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, nTextureID4);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 6, &sTexture1);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 7, &sTexture2);

	// Side 5
	sTexture1 = fnCreateTexture(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, nTextureID5);
	sTexture2 = fnCreateTexture(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, nTextureID5);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 8, &sTexture1);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 9, &sTexture2);

	// Side 6
	sTexture1 = fnCreateTexture(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, nTextureID6);
	sTexture2 = fnCreateTexture(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, nTextureID6);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 10, &sTexture1);
	lib3mf_propertyhandler_settexture(pPropertyHandler, 11, &sTexture2);

	// release property handler
	lib3mf_release(pPropertyHandler);

	// make sure to define a default property
	hResult = lib3mf_object_createdefaultpropertyhandler(pMeshObject, &pDefaultPropertyHandler);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create default property handler: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pMeshObject, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}
	lib3mf_defaultpropertyhandler_settexture(pDefaultPropertyHandler, nTextureID1, 1.0, 1.0);

	// release default property handler
	lib3mf_release(pDefaultPropertyHandler);

	// Add Build Item for Mesh
	hResult = lib3mf_model_addbuilditem(pModel, pMeshObject, NULL, &pBuildItem);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create build item: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pModel, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Output cube as STL and 3MF
	lib3mf_release(pMeshObject);
	lib3mf_release(pBuildItem);


	// Create Model Writer
	hResult = lib3mf_model_querywriter(pModel, "3mf", &p3MFWriter);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create model reader: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pModel, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pModel);
		return -1;
	}

	// Export Model into File
	std::cout << "writing texturecube.3mf..." << std::endl;
	hResult = lib3mf_writer_writetofileutf8(p3MFWriter, "texturecube.3mf");
	if (hResult != LIB3MF_OK) {
		std::cout << "could not write file: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(p3MFWriter, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pModel);
		lib3mf_release(p3MFWriter);
		return -1;
	}

	// Release Model Writer
	lib3mf_release(p3MFWriter);

	// Release Model
	lib3mf_release(pModel);

	std::cout << "done" << std::endl;


	return 0;
}

