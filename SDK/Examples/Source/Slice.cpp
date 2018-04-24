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

Cube.cpp : 3MF slice extension example

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
	// Vertices for the 3d model defined
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

	// Vertices for the slices defined
	NMR::MODELSLICEVERTEX aSliceVertices[4] = {
		{ 1.01f, 1.02f },
		{ 9.03f, 1.04f },
		{ 9.05f, 9.06f },
		{ 1.07f, 9.08f }
	};

	// Indices for the slices defined
	DWORD aSliceIndices[5] = { 0, 1, 2, 3, 0 };

	CustomLib3MFBase pModel;

	// Create a lib3mf model
	if (NMR::lib3mf_createmodel(&pModel.get()) != LIB3MF_OK) {
		std::cout << "Unable to create model" << std::endl;
		return 1;
	}

	CustomLib3MFBase pMeshObject;

	// Add a meshobject and set it's size unit to millimeter
	if (NMR::lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()) != LIB3MF_OK) { std::cout << "Unable to add mesh object to model" << std::endl; return 1; }
	if (NMR::lib3mf_model_setunit(pModel.get(), NMR::MODELUNIT_MILLIMETER) != LIB3MF_OK) { std::cout << "Cannot set model units" << std::endl; return 1; }

	// Add 3d vertices to the mesh model
	for (int i = 0; i < 8; i++)
		if (NMR::lib3mf_meshobject_addvertex(pMeshObject.get(), &a_cVerties[i], NULL) != LIB3MF_OK) {
			std::cout << "Unable to add vertex to object" << std::endl;
			return 1;
		}

	// Add indices to the mesh model
	for (int i = 0; i < 12; i++)
		if (NMR::lib3mf_meshobject_addtriangle(pMeshObject.get(), &a_cTris[i], NULL) != LIB3MF_OK) {
			std::cout << "Unable to add triangle to mesh object" << std::endl;
			return 1;
		}

	CustomLib3MFBase pSliceStack;

	// create a slicestack and assign it to the meshobject
	if (NMR::lib3mf_model_addslicestack(pModel.get(), 0.005f, &pSliceStack.get()) != LIB3MF_OK) { std::cout << "Unable to add slice stack to object" << std::endl; return 1; }
	if (NMR::lib3mf_meshobject_setslicestack(pMeshObject.get(), pSliceStack.get()) != LIB3MF_OK) { std::cout << "Unable to link slice stack to mesh" << std::endl; return 1; }

	// Now add 10 slices ...
	for (int i = 0; i < 10; i++) {
		float fTopZ = ((float)(i+1)) / 10.0f;
		CustomLib3MFBase pSlice;
		DWORD nPolyIndex, nVertexIndex;

		if (NMR::lib3mf_slicestack_addslice(pSliceStack.get(), fTopZ, &pSlice.get()) != LIB3MF_OK) { std::cout << "Unable to add slice to slicestack"; return 1; }
		// ... with the slice data defined above
		if (NMR::lib3mf_slice_addvertices(pSlice.get(), aSliceVertices, 4, &nVertexIndex) != LIB3MF_OK) { std::cout << "Unable to add vertices to slice"; return 1; }
		if (NMR::lib3mf_slice_addpolygon(pSlice.get(), aSliceIndices, 5, &nPolyIndex) != LIB3MF_OK) { std::cout << "Unable to add indices to slice"; return 1; }
	}

	CustomLib3MFBase p3MFWriter;

	// create a 3mf writer
	if (NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), LIB3MF_OK) { std::cout << "Cannot create model writer"; return 1; }
	// Export the model with the slices to a 3mf file
	if (NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), "slices.3mf"), LIB3MF_OK) { std::cout << "Cannot write 3mf"; return 1; }

	return 0;
}
