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

ExtractInfo.cpp : 3MF Read Example

--*/

#include <iostream>
#include <string>
#include <algorithm>

#include "lib3mf_dynamic.hpp"

using namespace Lib3MF;


void printVersion(PWrapper wrapper) {
	Lib3MF_uint32 nMajor, nMinor, nMicro;
	wrapper->GetLibraryVersion(nMajor, nMinor, nMicro);
	std::cout << "lib3mf version = " << nMajor << "." << nMinor << "." << nMicro;
	std::string sReleaseInfo, sBuildInfo;
	if (wrapper->GetPrereleaseInformation(sReleaseInfo)) {
		std::cout << "-" << sReleaseInfo;
	}
	if (wrapper->GetBuildInformation(sBuildInfo)) {
		std::cout << "+" << sBuildInfo;
	}
	std::cout << std::endl;
}

void ShowThumbnailInformation(PModel model)
{
	/*
	// TODO: this is not yet implemented in lib3mf
	*/
}


void ShowMetaDataInformation(PMetaDataGroup metaDataGroup)
{
	Lib3MF_uint32 nMetaDataCount = metaDataGroup->GetMetaDataCount();

	for (Lib3MF_uint32 iMeta = 0; iMeta < nMetaDataCount; iMeta++) {

		PMetaData metaData = metaDataGroup->GetMetaData(iMeta);
		std::string sMetaDataValue = metaData->GetValue();
		std::string sMetaDataName = metaData->GetName();
		std::cout << "Metadatum: " << iMeta << ":" << std::endl;
		std::cout << "Name  = \"" << sMetaDataName << "\"" << std::endl;
		std::cout << "Value = \"" << sMetaDataValue << "\"" << std::endl;
	}
}

void ShowSliceStack(PSliceStack sliceStack, std::string indent)
{
	std::cout << indent << "SliceStackID:  " << sliceStack->GetResourceID() << std::endl;
	if (sliceStack->GetSliceCount() > 0) {
		std::cout << indent << "  Slice count:  " << sliceStack->GetSliceCount() << std::endl;
	}
	if (sliceStack->GetSliceRefCount() > 0) {
		std::cout << indent << "  Slice ref count:  " << sliceStack->GetSliceRefCount() << std::endl;
		for (Lib3MF_uint64 iSliceRef = 0; iSliceRef < sliceStack->GetSliceRefCount(); iSliceRef++) {
			std::cout << indent << "  Slice ref :  " << sliceStack->GetSliceStackReference(iSliceRef)->GetResourceID() << std::endl;
		}
	}
}

void ShowObjectProperties(PObject object)
{
	std::cout << "   Name:            \"" << object->GetName() << "\"" << std::endl;
	std::cout << "   PartNumber:      \"" << object->GetPartNumber() << "\"" << std::endl;

	switch (object->GetType()) {
	case eObjectType::Model:
		std::cout << "   Object type:     model" << std::endl;
		break;
	case eObjectType::Support:
		std::cout << "   Object type:     support" << std::endl;
		break;
	case eObjectType::SolidSupport:
		std::cout << "   Object type:     solidsupport" << std::endl;
		break;
	case eObjectType::Other:
		std::cout << "   Object type:     other" << std::endl;
		break;
	default:
		std::cout << "   Object type:     invalid" << std::endl;
		break;
	}

	if (object->HasSlices(false)) {
		PSliceStack sliceStack = object->GetSliceStack();
		ShowSliceStack(sliceStack, "   ");
	}

	if (object->GetMetaDataGroup()->GetMetaDataCount() > 0) {
		ShowMetaDataInformation(object->GetMetaDataGroup());
	}
}

void ShowMeshObjectInformation(PMeshObject meshObject)
{
	std::cout << "mesh object #" << meshObject->GetResourceID() << ": " << std::endl;

	ShowObjectProperties(meshObject);

	Lib3MF_uint64 nVertexCount = meshObject->GetVertexCount();
	Lib3MF_uint64 nTriangleCount = meshObject->GetTriangleCount();
	PBeamLattice beamLattice = meshObject->BeamLattice();

	// Output data
	std::cout << "   Vertex count:    " << nVertexCount << std::endl;
	std::cout << "   Triangle count:  " << nTriangleCount << std::endl;

	Lib3MF_uint64 nBeamCount = beamLattice->GetBeamCount();
	if (nBeamCount > 0) {
		std::cout << "   Beam count:  " << nBeamCount << std::endl;
		Lib3MF_uint32 nRepresentationMesh;
		if (beamLattice->GetRepresentation(nRepresentationMesh))
			std::cout << "   |_Representation Mesh ID:  " << nRepresentationMesh << std::endl;
		eLib3MFBeamLatticeClipMode eClipMode;
		Lib3MF_uint32 nClippingMesh;
		beamLattice->GetClipping(eClipMode, nClippingMesh);
		if (eClipMode != eBeamLatticeClipMode::NoClipMode)
			std::cout << "   |_Clipping Mesh ID:  " << nClippingMesh << "(mode=" << (int)eClipMode << ")" << std::endl;
		if (beamLattice->GetBeamSetCount() > 0) {
			std::cout << "   |_BeamSet count:  " << beamLattice->GetBeamSetCount() << std::endl;
		}
	}

}

void ShowTransform(sLib3MFTransform transform, std::string indent) {
	std::cout << indent << "Transformation:  [ " << transform.m_Fields[0][0] << " " << transform.m_Fields[1][0] << " " << transform.m_Fields[2][0] << " " << transform.m_Fields[3][0] << " ]" << std::endl;
	std::cout << indent << "                 [ " << transform.m_Fields[0][1] << " " << transform.m_Fields[1][1] << " " << transform.m_Fields[2][1] << " " << transform.m_Fields[3][1] << " ]" << std::endl;
	std::cout << indent << "                 [ " << transform.m_Fields[0][2] << " " << transform.m_Fields[1][2] << " " << transform.m_Fields[2][2] << " " << transform.m_Fields[3][2] << " ]" << std::endl;
}

void ShowComponentsObjectInformation(PComponentsObject componentsObject)
{
	std::cout << "components object #" << componentsObject->GetResourceID() << ": " << std::endl;

	ShowObjectProperties(componentsObject);
	std::cout << "   Component count:    " << componentsObject->GetComponentCount() << std::endl;
	for (Lib3MF_uint32 nIndex = 0; nIndex < componentsObject->GetComponentCount(); nIndex++) {
		PComponent component = componentsObject->GetComponent(nIndex);

		std::cout << "   Component " << nIndex << ":    Object ID:   " << component->GetObjectResourceID() << std::endl;
		if (component->HasTransform()) {
			ShowTransform(component->GetTransform(), "                   ");
		}
		else {
			std::cout << "                   Transformation:  none" << std::endl;
		}
	}
}


void ExtractInfoExample(std::string sFileName) {
	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF Read example" << std::endl;
	
	std::string libpath = ("."); // TODO: put the location of the Lib3MF-library file here.
	auto wrapper = Lib3MF::CWrapper::loadLibrary(LIB3MF_LIBRARY_LOCATION); // TODO: add correct suffix of the library

	printVersion(wrapper);
	std::cout << "------------------------------------------------------------------" << std::endl;

	PModel model = wrapper->CreateModel();

	// Import Model from 3MF File
	{
		PReader reader = model->QueryReader("3mf");
		// And deactivate the strict mode (default is "false", anyway. This just demonstrates where/how to use it).
		reader->SetStrictModeActive(false);
		reader->ReadFromFile(sFileName);

		for (Lib3MF_uint32 iWarning = 0; iWarning < reader->GetWarningCount(); iWarning++) {
			Lib3MF_uint32 nErrorCode;
			std::string sWarningMessage = reader->GetWarning(iWarning, nErrorCode);
			std::cout << "Encountered warning #" << nErrorCode << " : " << sWarningMessage << std::endl;
		}
	}
	ShowThumbnailInformation(model);

	ShowMetaDataInformation(model->GetMetaDataGroup());

	PSliceStackIterator sliceStacks = model->GetSliceStacks();
	while (sliceStacks->MoveNext()) {
		PSliceStack sliceStack = sliceStacks->GetCurrentSliceStack();
		ShowSliceStack(sliceStack, "");
	}

	PObjectIterator objectIterator = model->GetObjects();
	while (objectIterator->MoveNext()) {
		PObject object = objectIterator->GetCurrentObject();
		if (object->IsMeshObject()) {
			ShowMeshObjectInformation(model->GetMeshObjectByID(object->GetResourceID()));
		}
		else if (object->IsComponentsObject()) {
			ShowComponentsObjectInformation(model->GetComponentsObjectByID(object->GetResourceID()));
		}
		else {
			std::cout << "unknown object #" << object->GetResourceID() << ": " << std::endl;
		}
	}


	PBuildItemIterator buildItemIterator = model->GetBuildItems();
	while (buildItemIterator->MoveNext()) {
		PBuildItem buildItem = buildItemIterator->GetCurrent();

		std::cout << "Build item (Object #" << buildItem->GetObjectResourceID() << "): " << std::endl;

		if (buildItem->HasObjectTransform()) {
			ShowTransform(buildItem->GetObjectTransform(), "   ");
		}
		else {
			std::cout << "   Transformation:  none" << std::endl;
		}
		std::cout << "   Part number:     \"" << buildItem->GetPartNumber() << "\"" << std::endl;
		if (buildItem->GetMetaDataGroup()->GetMetaDataCount() > 0) {
			ShowMetaDataInformation(buildItem->GetMetaDataGroup());
		}
	}

	std::cout << "done" << std::endl;
}


int main(int argc, char** argv) {
	// Parse Arguments
	if (argc != 2) {
		std::cout << "Usage: " << std::endl;
		std::cout << "ExtractInfo.exe model.3mf" << std::endl;
		return 0;
	}
	
	try {
		ExtractInfoExample(argv[1]);
	}
	catch (ELib3MFException &e) {
		std::cout << e.what() << std::endl;
		return e.getErrorCode();
	}
	return 0;
}
