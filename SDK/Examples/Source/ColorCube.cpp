/*++

© 2017 Autodesk Inc
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

ColorCube.cpp : 3MF Color Cube creation example

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

MODELMESHCOLOR_SRGB fnCreateColor(unsigned char red, unsigned char green, unsigned char blue)
{
	MODELMESHCOLOR_SRGB result;
	result.m_Red = red;
	result.m_Green = green;
	result.m_Blue = blue;
	result.m_Alpha = 255;
	return result;
	

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
	std::cout << "3MF Cube example" << std::endl;
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
	float fSizeY = 200.0f;
	float fSizeZ = 300.0f;

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

	// Create color entries for cube
	hResult = lib3mf_meshobject_createpropertyhandler(pMeshObject, &pPropertyHandler);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create property handler: " << std::hex << hResult << std::endl;
		lib3mf_getlasterror(pMeshObject, &nErrorMessage, &pszErrorMessage);
		std::cout << "error #" << std::hex << nErrorMessage << ": " << pszErrorMessage << std::endl;
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// define colors
	MODELMESHCOLOR_SRGB sColorRed = fnCreateColor (255, 0, 0);
	MODELMESHCOLOR_SRGB sColorGreen = fnCreateColor(0, 255, 0);
	MODELMESHCOLOR_SRGB sColorBlue = fnCreateColor(0, 0, 255);
	MODELMESHCOLOR_SRGB sColorOrange = fnCreateColor(255, 128, 0);
	MODELMESHCOLOR_SRGB sColorYellow = fnCreateColor(255, 255, 0);

	MODELMESH_TRIANGLECOLOR_SRGB sTriangleColor1;
	sTriangleColor1.m_Colors[0] = sColorOrange;
	sTriangleColor1.m_Colors[1] = sColorRed;
	sTriangleColor1.m_Colors[2] = sColorYellow;

	MODELMESH_TRIANGLECOLOR_SRGB sTriangleColor2;
	sTriangleColor2.m_Colors[0] = sColorYellow;
	sTriangleColor2.m_Colors[1] = sColorGreen;
	sTriangleColor2.m_Colors[2] = sColorOrange;


	// One-colored Triangles
	lib3mf_propertyhandler_setsinglecolor(pPropertyHandler, 0, &sColorRed);
	lib3mf_propertyhandler_setsinglecolor(pPropertyHandler, 1, &sColorRed);
	lib3mf_propertyhandler_setsinglecolor(pPropertyHandler, 2, &sColorGreen);
	lib3mf_propertyhandler_setsinglecolor(pPropertyHandler, 3, &sColorGreen);
	lib3mf_propertyhandler_setsinglecolor(pPropertyHandler, 4, &sColorBlue);
	lib3mf_propertyhandler_setsinglecolor(pPropertyHandler, 5, &sColorBlue);

	// Gradient-colored Triangles
	lib3mf_propertyhandler_setgradientcolor(pPropertyHandler, 6, &sTriangleColor1);
	lib3mf_propertyhandler_setgradientcolor(pPropertyHandler, 7, &sTriangleColor2);
	lib3mf_propertyhandler_setgradientcolor(pPropertyHandler, 8, &sTriangleColor1);
	lib3mf_propertyhandler_setgradientcolor(pPropertyHandler, 9, &sTriangleColor2);
	lib3mf_propertyhandler_setgradientcolor(pPropertyHandler, 10, &sTriangleColor1);
	lib3mf_propertyhandler_setgradientcolor(pPropertyHandler, 11, &sTriangleColor2);

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
	lib3mf_defaultpropertyhandler_setcolor(pDefaultPropertyHandler, &sColorRed);

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
	std::cout << "writing colorcube.3mf..." << std::endl;
	hResult = lib3mf_writer_writetofile(p3MFWriter, L"colorcube.3mf");
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

