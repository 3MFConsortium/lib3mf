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

Cube.cpp : 3MF beamlattice extension example

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

class CustomLib3MFBase {
public:
	CustomLib3MFBase() : m_pLib3MFBase(NULL)
	{
	}
	~CustomLib3MFBase()
	{
		if (m_pLib3MFBase) {
			NMR::lib3mf_release(m_pLib3MFBase);
			m_pLib3MFBase = NULL;
		}
	}
	NMR::PLib3MFBase* & get() {
		return m_pLib3MFBase;
	}
private:
	NMR::PLib3MFBase* m_pLib3MFBase;
};


#ifndef __GNUC__
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif // __GNUC__
{
	// Vertices for the first 3d model
	NMR::MODELMESHVERTEX a_cVerties[8] = {
		{ 0,   0, 0 },
		{ 100,   0, 0 },
		{ 100, 100, 0 },
		{ 0, 100, 0 },
		{ 0,   0, 100 },
		{ 100,   0, 100 },
		{ 100, 100, 100 },
		{ 0, 100, 100 }
	};

	// Vertices for the second 3d model
	NMR::MODELMESHVERTEX a_cVerties_CutOut[8] = {
		{ 0,   0, 0 },
		{ 50,   0, 0 },
		{ 50, 50, 0 },
		{ 0, 50, 0 },
		{ 0,   0, 50 },
		{ 50,   0, 50 },
		{ 50, 50, 50 },
		{ 0, 50, 50 }
	};

	// Indices for the 3d model defined
	NMR::MODELMESHTRIANGLE a_cTris[12] = {
		{ 2, 1, 0 },
		{ 3, 2, 0 },
		{ 4, 5, 6 },
		{ 4, 6, 7 },
		{ 0, 1, 4 },
		{ 5, 4, 1 },
		{ 1, 2, 5 },
		{ 6, 5, 2 },
		{ 2, 3, 6 },
		{ 7, 6, 3 },
		{ 7, 0, 4 },
		{ 0, 7, 3 }
	};

	CustomLib3MFBase pModel;
	CustomLib3MFBase pBuildItem;
	// Create a lib3mf model
	if (NMR::lib3mf_createmodel(&pModel.get()) != LIB3MF_OK) {
		std::cout << "Unable to create model" << std::endl;
		return 1;
	}

	CustomLib3MFBase pMeshObject;
	CustomLib3MFBase pMeshObjectCutOut;
	
	// Add a meshobject and set it's size unit to millimeter
	if (NMR::lib3mf_model_addmeshobject(pModel.get(), &pMeshObjectCutOut.get()) != LIB3MF_OK) { std::cout << "Unable to add mesh object to model" << std::endl; return 1; }
	if (NMR::lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()) != LIB3MF_OK) { std::cout << "Unable to add mesh object to model" << std::endl; return 1; }
	if (NMR::lib3mf_model_setunit(pModel.get(), NMR::MODELUNIT_MILLIMETER) != LIB3MF_OK) { std::cout << "Cannot set model units" << std::endl; return 1; }

	// Set mesh geometries
	if (NMR::lib3mf_meshobject_setgeometry(pMeshObjectCutOut.get(), a_cVerties_CutOut, 8, a_cTris, 12) != LIB3MF_OK) {
		std::cout << "Unable to set mesh geometry on object" << std::endl;
		return 1;
	}
	if (NMR::lib3mf_meshobject_setgeometry(pMeshObject.get(), a_cVerties, 8, a_cTris, 12) != LIB3MF_OK) {
		std::cout << "Unable to set mesh geometry on object" << std::endl;
		return 1;
	}
	DWORD nResourceID;
	if (NMR::lib3mf_resource_getresourceid(pMeshObjectCutOut.get(), &nResourceID) != LIB3MF_OK) {
		std::cout << "Unable to get resource ID" << std::endl;
		return 1;
	}
	if (NMR::lib3mf_meshobject_setbeamlattice_clipping(pMeshObject.get(), MODELBEAMLATTICECLIPMODE_OUTSIDE, nResourceID) != LIB3MF_OK) {
		std::cout << "Unable to set beamlattice clipping on object" << std::endl;
		return 1;
	}

	NMR::MODELMESHBEAM beam;
	DWORD nIndex;
	beam.m_dRadius[0] = 1;
	beam.m_dRadius[1] = 2;
	beam.m_nIndices[0] = 0;
	beam.m_nIndices[1] = 1;
	if (NMR::lib3mf_meshobject_addbeam(pMeshObject.get(), &beam, &nIndex) != LIB3MF_OK) {
		std::cout << "Unable to add beam to meshobject" << std::endl;
		return 1;
	}
	beam.m_dRadius[0] = 2;
	beam.m_dRadius[1] = 1.5;
	beam.m_nIndices[0] = 1;
	beam.m_nIndices[1] = 2;
	beam.m_eCapMode[1] = MODELBEAMLATTICECAPMODE_BUTT;
	if (NMR::lib3mf_meshobject_addbeam(pMeshObject.get(), &beam, &nIndex) != LIB3MF_OK) {
		std::cout << "Unable to add beam to meshobject" << std::endl;
		return 1;
	}

	if (NMR::lib3mf_model_addbuilditem(pModel.get(), pMeshObject.get(), NULL, &pBuildItem.get()) != LIB3MF_OK) {
		std::cout << "Unable to add build item" << std::endl;
		return 1;
	}

	CustomLib3MFBase p3MFWriter;

	// create a 3mf writer
	if (NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), LIB3MF_OK) { std::cout << "Cannot create model writer"; return 1; }
	// Export the model with the beam to a 3mf file
	if (NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), "beamlattice.3mf"), LIB3MF_OK) { std::cout << "Cannot write 3mf"; return 1; }

	return 0;
}
