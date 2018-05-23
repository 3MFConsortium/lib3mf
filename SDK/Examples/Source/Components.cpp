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

Components.cpp : 3MF Components example

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

MODELTRANSFORM createTranslationMatrix(float x, float y, float z)
{
	MODELTRANSFORM mMatrix;
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			mMatrix.m_fFields[j][i] = (i == j) ? 1.0f : 0.0f;
		}
	}

	mMatrix.m_fFields[0][3] = x;
	mMatrix.m_fFields[1][3] = y;
	mMatrix.m_fFields[2][3] = z;

	return mMatrix;
}


#ifndef __GNUC__
int _tmain(int argc, _TCHAR* argv[])
#else
int main (int argc, char* argv[])
#endif // __GNUC__
{
	// General Variables
	HRESULT hResult;
	DWORD nInterfaceVersionMajor, nInterfaceVersionMinor, nInterfaceVersionMicro;
	MODELTRANSFORM mTransform;

	// Objects
	PLib3MFModel * pModel;
	PLib3MFModelWriter * pSTLWriter;
	PLib3MFModelWriter * p3MFWriter;
	PLib3MFModelMeshObject * pMeshObject;
	PLib3MFModelComponentsObject * pComponentsObject;
	PLib3MFModelBuildItem * pBuildItem;
	PLib3MFModelComponent * pComponent1;
	PLib3MFModelComponent * pComponent2;
	PLib3MFModelComponent * pComponent3;


	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Components example" << std::endl;
	std::cout << "------------------------------------------------------------------" << std::endl;

	// Check 3MF Library Version
	hResult = lib3mf_getinterfaceversion(&nInterfaceVersionMajor, &nInterfaceVersionMinor, &nInterfaceVersionMicro);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not get 3MF Library version: " << std::hex << hResult << std::endl;
		return -1;
	}

	if ((nInterfaceVersionMajor != NMR_APIVERSION_INTERFACE_MAJOR)) {
		std::cout << "invalid 3MF Library major version: " << NMR_APIVERSION_INTERFACE_MAJOR << std::endl;
		return -1;
	}
	if (!(nInterfaceVersionMinor >= NMR_APIVERSION_INTERFACE_MINOR)) {
		std::cout << "invalid 3MF Library minor version: " << NMR_APIVERSION_INTERFACE_MINOR << std::endl;
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
		lib3mf_release(pModel);
		return -1;
	}

	// Create mesh structure of a cube
	MODELMESHVERTEX pVertices[8];
	MODELMESHTRIANGLE pTriangles[12];
	float fSizeX = 10.0f;
	float fSizeY = 20.0f;
	float fSizeZ = 30.0f;

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
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Create Component Object
	hResult = lib3mf_model_addcomponentsobject(pModel, &pComponentsObject);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not add mesh object: " << std::hex << hResult << std::endl;
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Create model transform for second cube
	mTransform = createTranslationMatrix(0.0f, 0.0f, 0.0f);

	hResult = lib3mf_componentsobject_addcomponent(pComponentsObject, pMeshObject, &mTransform, &pComponent1);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not add component 1: " << std::hex << hResult << std::endl;
		lib3mf_release(pComponentsObject);
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Release Component 1
	lib3mf_release(pComponent1);

	// Create model transform for second cube
	mTransform = createTranslationMatrix(40.0f, 60.0f, 80.0f);

	// Add Build Item for Mesh
	hResult = lib3mf_componentsobject_addcomponent(pComponentsObject, pMeshObject, &mTransform, &pComponent2);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not add component 2: " << std::hex << hResult << std::endl;
		lib3mf_release(pComponentsObject);
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Release Component 2
	lib3mf_release(pComponent2);

	// Create model transform for third cube
	mTransform = createTranslationMatrix(120.0f, 30.0f, 70.0f);

	// Add Build Item for Mesh
	hResult = lib3mf_componentsobject_addcomponent(pComponentsObject, pMeshObject, &mTransform, &pComponent3);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not add component 3: " << std::hex << hResult << std::endl;
		lib3mf_release(pComponentsObject);
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Release Component 3
	lib3mf_release(pComponent3);

	// Add Build Item for Component
	mTransform = createTranslationMatrix(0.0f, 0.0f, 0.0f);
	hResult = lib3mf_model_addbuilditem(pModel, pComponentsObject, &mTransform, &pBuildItem);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create build item: " << std::hex << hResult << std::endl;
		lib3mf_release(pComponentsObject);
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Release Build Item
	lib3mf_release(pBuildItem);

	// Add second Build Item for Component
	mTransform = createTranslationMatrix(200.0f, 40.0f, 10.0f);
	hResult = lib3mf_model_addbuilditem(pModel, pComponentsObject, &mTransform, &pBuildItem);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create build item: " << std::hex << hResult << std::endl;
		lib3mf_release(pComponentsObject);
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Add third Build Item for Mesh Object
	mTransform = createTranslationMatrix(-40.0f, 0.0f, 20.0f);
	hResult = lib3mf_model_addbuilditem(pModel, pMeshObject, &mTransform, &pBuildItem);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create build item: " << std::hex << hResult << std::endl;
		lib3mf_release(pComponentsObject);
		lib3mf_release(pMeshObject);
		lib3mf_release(pModel);
		return -1;
	}

	// Release Build Item
	lib3mf_release(pBuildItem);

	// Release Objects
	lib3mf_release(pComponentsObject);
	lib3mf_release(pMeshObject);


	// Output scene as STL and 3MF

	// Create Model Writer
	hResult = lib3mf_model_querywriter(pModel, "stl", &pSTLWriter);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create model reader: " << std::hex << hResult << std::endl;
		lib3mf_release(pModel);
		return -1;
	}

	// Export Model into File
	std::cout << "writing components.stl..." << std::endl;
	hResult = lib3mf_writer_writetofile(pSTLWriter, L"components.stl");
	if (hResult != LIB3MF_OK) {
		std::cout << "could not write file: " << std::hex << hResult << std::endl;
		lib3mf_release(pSTLWriter);
		lib3mf_release(pModel);
		return -1;
	}

	// Create Model Writer
	hResult = lib3mf_model_querywriter(pModel, "3mf", &p3MFWriter);
	if (hResult != LIB3MF_OK) {
		std::cout << "could not create model writer: " << std::hex << hResult << std::endl;
		lib3mf_release(pModel);
		return -1;
	}

	// Release Writer
	lib3mf_release(pSTLWriter);

	// Export Model into File
	std::cout << "writing components.3mf..." << std::endl;
	hResult = lib3mf_writer_writetofile(p3MFWriter, L"components.3mf");
	if (hResult != LIB3MF_OK) {
		std::cout << "could not write file: " << std::hex << hResult << std::endl;
		lib3mf_release(p3MFWriter);
		lib3mf_release(pModel);
		return -1;
	}

	// Release Writer
	lib3mf_release(p3MFWriter);

	std::cout << "done" << std::endl;

	// Release Model
	lib3mf_release(pModel);

	return 0;
}

