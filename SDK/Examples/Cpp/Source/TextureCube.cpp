/*++

Copyright (C) 2019 3MF Consortium

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

TextureCube.cpp : 3MF Texture Cube creation example.

--*/

#include <iostream>
#include <string>
#include <algorithm>

#include "lib3mf.hpp"

using namespace Lib3MF;


void printVersion() {
	Lib3MF_uint32 nMajor, nMinor, nMicro;
	std::string sReleaseInfo, sBuildInfo;
	CLib3MFWrapper::GetLibraryVersion(nMajor, nMinor, nMicro, sReleaseInfo, sBuildInfo);
	std::cout << "Lib3MF version = " << nMajor << "." << nMinor << "." << nMicro;
	if (!sReleaseInfo.empty()) {
		std::cout << "-" << sReleaseInfo;
	}
	if (!sBuildInfo.empty()) {
		std::cout << "+" << sBuildInfo;
	}
	std::cout << std::endl;
}

// Utility functions to create vertices and triangles
sLib3MFPosition fnCreateVertex(float x, float y, float z)
{
	sLib3MFPosition result;
	result.m_Coordinates[0] = x;
	result.m_Coordinates[1] = y;
	result.m_Coordinates[2] = z;
	return result;
}

sLib3MFTriangle fnCreateTriangle(int v0, int v1, int v2)
{
	sLib3MFTriangle result;
	result.m_Indices[0] = v0;
	result.m_Indices[1] = v1;
	result.m_Indices[2] = v2;
	return result;
}

PLib3MFTexture2DGroup fnLoadModelTexture(PLib3MFModel model, const std::string sOPCPath, const std::string sFilePath, eLib3MFTextureType eType, eLib3MFTextureTileStyle eTileStyleU, eLib3MFTextureTileStyle eTileStyleV)
{
	std::string sRelationshipType_Texture = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";
	PLib3MFAttachment attachment = model->AddAttachment(sOPCPath, sRelationshipType_Texture);
	attachment->ReadFromFile(sFilePath);

	PLib3MFTexture2D texture2D = model->AddTexture2DFromAttachment(attachment.get());

	texture2D->SetContentType(eType);
	texture2D->SetTileStyleUV(eTileStyleU, eTileStyleV);

	PLib3MFTexture2DGroup textureGroup = model->AddTexture2DGroup(texture2D.get());
	return textureGroup;
}


sLib3MFTriangleProperties fnCreateTexture(PLib3MFTexture2DGroup textureGroup, double u1, double v1, double u2, double v2, double u3, double v3)
{
	sLib3MFTriangleProperties property;
	property.m_ResourceID = textureGroup->GetResourceID();
	property.m_PropertyIDs[0] = textureGroup->AddTex2Coord(sLib3MFTex2Coord({ u1, v1 }));
	property.m_PropertyIDs[1] = textureGroup->AddTex2Coord(sLib3MFTex2Coord({ u2, v2 }));
	property.m_PropertyIDs[2] = textureGroup->AddTex2Coord(sLib3MFTex2Coord({ u3, v3 }));
	return property;
}

void TextureExample() {
	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Texture Cube example" << std::endl;
	printVersion();
	std::cout << "------------------------------------------------------------------" << std::endl;

	PLib3MFModel model = CLib3MFWrapper::CreateModel();

	PLib3MFMeshObject meshObject = model->AddMeshObject();
	meshObject->SetName("Textured Box");

	// Create mesh structure of a cube
	std::vector<sLib3MFPosition> vertices(8);
	std::vector<sLib3MFTriangle> triangles(12);

	float fSizeX = 100.0f;
	float fSizeY = 100.0f;
	float fSizeZ = 100.0f;

	// Manually create vertices
	vertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
	vertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
	vertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
	vertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
	vertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
	vertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
	vertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
	vertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);

	// Manually create triangles
	triangles[0] = fnCreateTriangle(2, 1, 0);
	triangles[1] = fnCreateTriangle(0, 3, 2);
	triangles[2] = fnCreateTriangle(4, 5, 6);
	triangles[3] = fnCreateTriangle(6, 7, 4);
	triangles[4] = fnCreateTriangle(0, 1, 5);
	triangles[5] = fnCreateTriangle(5, 4, 0);
	triangles[6] = fnCreateTriangle(2, 3, 7);
	triangles[7] = fnCreateTriangle(7, 6, 2);
	triangles[8] = fnCreateTriangle(1, 2, 6);
	triangles[9] = fnCreateTriangle(6, 5, 1);
	triangles[10] = fnCreateTriangle(3, 0, 4);
	triangles[11] = fnCreateTriangle(4, 7, 3);

	meshObject->SetGeometry(vertices, triangles);

	std::string sTextureFolder = TEXTURESPATH;
	// add textures to 3mf package
	std::cout << "sTextureFolder=\"" << sTextureFolder << "\"\n";
	auto textureGroup1 = fnLoadModelTexture(model, "/3D/Textures/tex1.png", sTextureFolder + "tex1.png", eLib3MFTextureType::eTextureTypePNG, eLib3MFTextureTileStyle::eTextureTileStyleWrap, eLib3MFTextureTileStyle::eTextureTileStyleWrap);
	auto textureGroup2 = fnLoadModelTexture(model, "/3D/Textures/tex2.png", sTextureFolder + "tex2.png", eLib3MFTextureType::eTextureTypePNG, eLib3MFTextureTileStyle::eTextureTileStyleMirror, eLib3MFTextureTileStyle::eTextureTileStyleWrap);
	auto textureGroup3 = fnLoadModelTexture(model, "/3D/Textures/tex3.png", sTextureFolder + "tex3.png", eLib3MFTextureType::eTextureTypePNG, eLib3MFTextureTileStyle::eTextureTileStyleWrap, eLib3MFTextureTileStyle::eTextureTileStyleMirror);
	auto textureGroup4 = fnLoadModelTexture(model, "/3D/Textures/tex4.png", sTextureFolder + "tex4.png", eLib3MFTextureType::eTextureTypePNG, eLib3MFTextureTileStyle::eTextureTileStyleClamp, eLib3MFTextureTileStyle::eTextureTileStyleWrap);
	auto textureGroup5 = fnLoadModelTexture(model, "/3D/Textures/tex5.png", sTextureFolder + "tex5.png", eLib3MFTextureType::eTextureTypePNG, eLib3MFTextureTileStyle::eTextureTileStyleWrap, eLib3MFTextureTileStyle::eTextureTileStyleClamp);
	auto textureGroup6 = fnLoadModelTexture(model, "/3D/Textures/tex6.png", sTextureFolder + "tex6.png", eLib3MFTextureType::eTextureTypePNG, eLib3MFTextureTileStyle::eTextureTileStyleClamp, eLib3MFTextureTileStyle::eTextureTileStyleMirror);

	// Side 1
	meshObject->SetTriangleProperties(0, fnCreateTexture(textureGroup1, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0));
	meshObject->SetTriangleProperties(1, fnCreateTexture(textureGroup1, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0));

	// Side 2
	meshObject->SetTriangleProperties(2, fnCreateTexture(textureGroup2, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0));
	meshObject->SetTriangleProperties(3, fnCreateTexture(textureGroup2, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0));

	// Side 3
	// Go outside of bounds on this side
	meshObject->SetTriangleProperties(4, fnCreateTexture(textureGroup3, -1.0, -1.0, 2.0, -1.0, 2.0, 2.0));
	meshObject->SetTriangleProperties(5, fnCreateTexture(textureGroup3, 2.0, 2.0, -1.0, 2.0, -1.0, -1.0));

	// Side 4
	meshObject->SetTriangleProperties(6, fnCreateTexture(textureGroup4, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0));
	meshObject->SetTriangleProperties(7, fnCreateTexture(textureGroup4, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0));

	// Side 5
	meshObject->SetTriangleProperties(8, fnCreateTexture(textureGroup5, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0));
	meshObject->SetTriangleProperties(9, fnCreateTexture(textureGroup5, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0));

	// Side 6
	meshObject->SetTriangleProperties(10, fnCreateTexture(textureGroup6, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0));
	meshObject->SetTriangleProperties(11, fnCreateTexture(textureGroup6, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0));

	// Add build item
	model->AddBuildItem(meshObject.get(), CLib3MFWrapper::GetIdentityTransform());

	PLib3MFWriter writer = model->QueryWriter("3mf");
	writer->WriteToFile("texturegroup.3mf");

	std::cout << "done" << std::endl;
}

int main() {
	try {
		TextureExample();
	}
	catch (ELib3MFException &e) {
		std::cout << e.what() << std::endl;
		return e.getErrorCode();
	}
	return 0;
}


