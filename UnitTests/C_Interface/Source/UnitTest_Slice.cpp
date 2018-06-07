/*++

Copyright (C) 2017 Autodesk, Inc.
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

UnitTest_Slice.cpp: Defines Unittests for the slice extension to 3MF

--*/

#include "UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"

#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"

namespace NMR {

	void generateSliceStack(CustomLib3MFBase &pSliceStack) {
		NMR::MODELSLICEVERTEX aVertices[] = {
			{ { 1.01f, 1.02f } },
			{ { 9.03f, 1.04f } },
			{ { 9.05f, 9.06f } },
			{ { 1.07f, 9.08f } }
		};
		DWORD aIndices[] = { 0, 1, 2, 3, 0 };

		ASSERT_EQ(NMR::lib3mf_slicestack_setusessliceref(pSliceStack.get(), true), S_OK) << L"Could not set slice ref";
		for (int i = 0; i < 10; i++) {
			float fTopZ = ((float)(i + 1)) / 10.0f;
			CustomLib3MFBase pSlice;
			ASSERT_EQ(NMR::lib3mf_slicestack_addslice(pSliceStack.get(), fTopZ, &pSlice.get()), S_OK) << L"Could not add slice to slicestack";
			DWORD nVertexIndex;
			ASSERT_EQ(NMR::lib3mf_slice_addvertices(pSlice.get(), aVertices, 4, &nVertexIndex), S_OK) << L"Could not add vertices to slice";

			DWORD nPolyIndex;
			ASSERT_EQ(NMR::lib3mf_slice_addpolygon(pSlice.get(), aIndices, 5, &nPolyIndex), S_OK) << L"Unable to add slice polygon";
		}

		DWORD nSliceCount;
		ASSERT_EQ(NMR::lib3mf_slicestack_getslicecount(pSliceStack.get(), &nSliceCount), S_OK) << L"Unable to retrieve number of slices";
		ASSERT_EQ(nSliceCount, 10) << L"Incorrect number of slices found";
		for (DWORD i = 0; i < nSliceCount; i++) {

			CustomLib3MFBase pSlice;
			ASSERT_EQ(NMR::lib3mf_slicestack_getslice(pSliceStack.get(), i, &pSlice.get()), S_OK) << L"Unable to retrieve slice";

			DWORD nVertexCount;
			ASSERT_EQ(NMR::lib3mf_slice_getvertexcount(pSlice.get(), &nVertexCount), S_OK) << L"Unable to retrieve number of vertices in slice";
			ASSERT_EQ(nVertexCount, 4) << L"Retrieved wrong number of vertices in slice";

			NMR::MODELSLICEVERTEX aQueryVertices[4];
			ASSERT_EQ(NMR::lib3mf_slice_getvertices(pSlice.get(), aQueryVertices, 4), S_OK) << L"Unable to retrieve vertices of slice";

			for (int j = 0; j < 4; j++) {
				ASSERT_EQ(aQueryVertices[j].m_fPosition[0], aVertices[j].m_fPosition[0]) << L"X-Coordinate of slice vertex does not match";
				ASSERT_EQ(aQueryVertices[j].m_fPosition[1], aVertices[j].m_fPosition[1]) << L"Y-Coordinate of slice vertex does not match";
			}

			DWORD nPolygonCount;
			ASSERT_EQ(NMR::lib3mf_slice_getpolygoncount(pSlice.get(), &nPolygonCount), S_OK) << L"Unable to retrieve number of slice polygons";
			ASSERT_EQ(nPolygonCount, 1) << L"Incorrect number of slice polygon indices retrieved";

			DWORD nIndexCount;
			ASSERT_EQ(NMR::lib3mf_slice_getpolygonindexcount(pSlice.get(), 0, &nIndexCount), S_OK) << L"Unable to retrieve number of slice polygon indices";
			ASSERT_EQ(nIndexCount, 5) << L"Incorrect number of slice polygon indices retrieved";

			DWORD aQueryIndices[5];
			ASSERT_EQ(NMR::lib3mf_slice_getpolygonindices(pSlice.get(), 0, aQueryIndices, 5), S_OK) << L"Unable to retrieve slice polygon indices";

			for (int j = 0; j < 5; j++) {
				ASSERT_EQ(aQueryIndices[j], aIndices[j]) << L"Slice polygon index does not match";
			}
		}
	}

	TEST(Test_Slice, SlicesOnComponents)
	{
		CustomLib3MFBase pModel;

		// Create Model Instance
		ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";

		CustomLib3MFBase pSliceStack;
		ASSERT_EQ(NMR::lib3mf_model_addslicestack(pModel.get(), 0.005f, &pSliceStack.get()), S_OK) << L"Could not add slice stack";
		generateSliceStack(pSliceStack);

		CustomLib3MFBase pComponetsObject;
		ASSERT_EQ(NMR::lib3mf_model_addcomponentsobject(pModel.get(), &pComponetsObject.get()), S_OK) << "Unable to add componets object to model";

		ASSERT_EQ(NMR::lib3mf_componentsobject_setslicestack(pComponetsObject.get(), pSliceStack.get()), S_OK) << "Unable to set slicestak for componets object";
		
		eModelSlicesMeshResolution eMeshResolution;
		ASSERT_EQ(NMR::lib3mf_componentsobject_setslicesmeshresolution(pComponetsObject.get(), MODELSLICESMESHRESOLUTION_LOW), S_OK) << "Cannot set meshresolution";
		ASSERT_EQ(NMR::lib3mf_componentsobject_getslicesmeshresolution(pComponetsObject.get(), &eMeshResolution), S_OK) << "Cannot get meshresolution";
		ASSERT_EQ(eMeshResolution, MODELSLICESMESHRESOLUTION_LOW) << "Meshresolution is not \"lowres\"";

		CustomLib3MFBase p3MFWriter;

		ASSERT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
		ASSERT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + "Slice_SlicesOnComponents.3mf").c_str()), S_OK) << "Cannot write 3mf";

		{
			CustomLib3MFBase p3mfReader, pModel;
			ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << "Could not create model";
			ASSERT_EQ(NMR::lib3mf_model_queryreader(pModel.get(), "3mf", &p3mfReader.get()), S_OK) << "Could not query reader";
			ASSERT_EQ(NMR::lib3mf_reader_readfromfileutf8(p3mfReader.get(), (std::string("TestOutput") + separator() + "Slice_SlicesOnComponents.3mf").c_str()), S_OK) << "Unable to load file";

			CustomLib3MFBase pComponentsIterator;

			ASSERT_EQ(NMR::lib3mf_model_getcomponentsobjects(pModel.get(), &pComponentsIterator.get()), S_OK) << "Could not get components object";

			BOOL bHasNext;

			ASSERT_EQ(NMR::lib3mf_resourceiterator_movenext(pComponentsIterator.get(), &bHasNext), S_OK) << "Could not retrieve components object";

			CustomLib3MFBase pComponentsObject;

			ASSERT_EQ(NMR::lib3mf_resourceiterator_getcurrent(pComponentsIterator.get(), &pComponentsObject.get()), S_OK) << "Could not get componentsobject from iterator";

			DWORD nSliceStackId;

			ASSERT_EQ(NMR::lib3mf_meshobject_getslicestackid(pComponentsObject.get(), &nSliceStackId), S_OK) << "Could not get slice stack id";
			ASSERT_NE(nSliceStackId, 0) << "Invalid slice stack id";

			CustomLib3MFBase pSliceStack;

			ASSERT_EQ(NMR::lib3mf_model_getslicestackById(pModel.get(), nSliceStackId, &pSliceStack.get()), S_OK) << "Could not read slice stack";
		}
	}

	TEST(Test_Slice, ModelOnly)
	{
		CustomLib3MFBase pModel;

		// Create Model Instance
		ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		{
			CustomLib3MFBase pSliceStack;
			ASSERT_EQ(NMR::lib3mf_model_addslicestack(pModel.get(), 0.005f, &pSliceStack.get()), S_OK) << L"Could not add slice stack";
			{
				generateSliceStack(pSliceStack);
			}
		}
	}


	TEST(Test_Slice, ReadWriteSliceFile)
	{
		NMR::MODELSLICEVERTEX aSliceVertices[] = {
			{ { 1.01f, 1.02f } },
			{ { 9.03f, 1.04f } },
			{ { 9.05f, 9.06f } },
			{ { 1.07f, 9.08f } }
		};

		DWORD aSliceIndices[] = { 0, 1, 2, 3, 0 };

		{
			CustomLib3MFBase pModel;

			ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << "Unable to create model";

			CustomLib3MFBase pMeshObject;

			ASSERT_EQ(NMR::lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << "Unable to add mesh object to model";
			ASSERT_EQ(NMR::lib3mf_model_setunit(pModel.get(), NMR::MODELUNIT_MILLIMETER), S_OK) << "Cannot set model units";

			eModelSlicesMeshResolution eMeshResolution;
			ASSERT_EQ(NMR::lib3mf_meshobject_getslicesmeshresolution(pMeshObject.get(), &eMeshResolution), S_OK) << "Cannot get meshresolution";
			ASSERT_EQ(eMeshResolution, MODELSLICESMESHRESOLUTION_FULL) << "Default meshresolution is not \"fullres\"";

			ASSERT_EQ(NMR::lib3mf_meshobject_setslicesmeshresolution(pMeshObject.get(), MODELSLICESMESHRESOLUTION_LOW), S_OK) << "Cannot set meshresolution";
			ASSERT_EQ(NMR::lib3mf_meshobject_getslicesmeshresolution(pMeshObject.get(), &eMeshResolution), S_OK) << "Cannot get meshresolution";
			ASSERT_EQ(eMeshResolution, MODELSLICESMESHRESOLUTION_LOW) << "Meshresolution is not \"lowres\"";

			NMR::MODELMESHVERTEX a_cVerties[8] = {
				{ { 0,   0, 0 } },
				{ { 100,   0, 0 } },
				{ { 100, 100, 0 } },
				{ { 0, 100, 0 } },

				{ { 0,   0, 100 } },
				{ { 100,   0, 100 } },
				{ { 100, 100, 100 } },
				{ { 0, 100, 100 } }
			};

			for (int i = 0; i < 8; i++)
				ASSERT_EQ(NMR::lib3mf_meshobject_addvertex(pMeshObject.get(), &a_cVerties[i], nullptr), S_OK) << "Unable to add vertex to object";

			NMR::MODELMESHTRIANGLE a_cTris[] = {
				{ { 2, 1, 0 } },
				{ { 3, 2, 0 } },
				{ { 4, 5, 6 } },
				{ { 4, 6, 7 } },
				{ { 0, 1, 4 } },
				{ { 5, 4, 1 } },
				{ { 1, 2, 5 } },
				{ { 6, 5, 2 } },
				{ { 2, 3, 6 } },
				{ { 7, 6, 3 } },
				{ { 7, 0, 4 } },
				{ { 0, 7, 3 } },

				{ { 0xFFFF, 0xFFFF, 0xFFFF } }

			};

			for (int i = 0; a_cTris[i].m_nIndices[0] != 0xFFFF; i++)
				ASSERT_EQ(NMR::lib3mf_meshobject_addtriangle(pMeshObject.get(), &a_cTris[i], nullptr), S_OK) << "Unable to add triangle to mesh object";

			CustomLib3MFBase pSliceStack;

			ASSERT_EQ(NMR::lib3mf_model_addslicestack(pModel.get(), 0.005f, &pSliceStack.get()), S_OK) << "Unable to add slice stack to object";
			ASSERT_EQ(NMR::lib3mf_slicestack_setusessliceref(pSliceStack.get(), true), S_OK) << L"Could not set slice ref";
			ASSERT_EQ(NMR::lib3mf_meshobject_setslicestack(pMeshObject.get(), pSliceStack.get()), S_OK) << "Unable to link slice stack to mesh";

			for (int i = 0; i < 10; i++) {
				float fTopZ = ((float)(i+1)) / 10.0f;
				CustomLib3MFBase pSlice;
				DWORD nPolyIndex, nVertexIndex;

				ASSERT_EQ(NMR::lib3mf_slicestack_addslice(pSliceStack.get(), fTopZ, &pSlice.get()), S_OK) << "Unable to add slice to slicestack";
				ASSERT_EQ(NMR::lib3mf_slice_addvertices(pSlice.get(), aSliceVertices, 4, &nVertexIndex), S_OK) << "Unable to add vertices to slice";
				ASSERT_EQ(NMR::lib3mf_slice_addpolygon(pSlice.get(), aSliceIndices, 5, &nPolyIndex), S_OK) << "Unable to add indices to slice";
			}

			CustomLib3MFBase p3MFWriter;

			ASSERT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
			ASSERT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + "Slice_TestFile_1.3mf").c_str()), S_OK) << "Cannot write 3mf";
		}

		{
			CustomLib3MFBase p3mfReader, pModel;
			ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << "Could not create model";
			ASSERT_EQ(NMR::lib3mf_model_queryreader(pModel.get(), "3mf", &p3mfReader.get()), S_OK) << "Could not query reader";
			ASSERT_EQ(NMR::lib3mf_reader_readfromfileutf8(p3mfReader.get(), (std::string("TestOutput") + separator() + "Slice_TestFile_1.3mf").c_str()), S_OK) << "Unable to load file";

			CustomLib3MFBase pMeshIterator;

			ASSERT_EQ(NMR::lib3mf_model_getmeshobjects(pModel.get(), &pMeshIterator.get()), S_OK) << "Could not get mesh object";

			BOOL bHasNext;

			ASSERT_EQ(NMR::lib3mf_resourceiterator_movenext(pMeshIterator.get(), &bHasNext), S_OK) << "Could not retrieve mesh object";

			CustomLib3MFBase pMeshObject;

			ASSERT_EQ(NMR::lib3mf_resourceiterator_getcurrent(pMeshIterator.get(), &pMeshObject.get()), S_OK) << "Could not get mesh from iterator";

			DWORD nSliceStackId;

			ASSERT_EQ(NMR::lib3mf_meshobject_getslicestackid(pMeshObject.get(), &nSliceStackId), S_OK) << "Could not get slice stack id";
			ASSERT_NE(nSliceStackId, 0) << "Invalid slice stack id";

			CustomLib3MFBase pSliceStack;

			ASSERT_EQ(NMR::lib3mf_model_getslicestackById(pModel.get(), nSliceStackId, &pSliceStack.get()), S_OK) << "Could not read slice stack";

			DWORD nSliceCount;

			ASSERT_EQ(NMR::lib3mf_slicestack_getslicecount(pSliceStack.get(), &nSliceCount), S_OK) << "Cannot get number of slices";

			for (DWORD i = 0; i < nSliceCount; i++) {
				CustomLib3MFBase pSlice;
				ASSERT_EQ(lib3mf_slicestack_getslice(pSliceStack.get(), i, &pSlice.get()), S_OK) << "unable to retrieve slice " << i;

				DWORD nVertexCount;

				ASSERT_EQ(NMR::lib3mf_slice_getvertexcount(pSlice.get(), &nVertexCount), S_OK) << "unable to retrieve vertex count of slice";
				ASSERT_EQ(nVertexCount, 4);

				NMR::MODELSLICEVERTEX aVertices[4];

				ASSERT_EQ(NMR::lib3mf_slice_getvertices(pSlice.get(), aVertices, 4), S_OK) << "unable to retrieve slice vertices.";

				for (int j = 0; j < 4; j++) {
					ASSERT_EQ(aVertices[j].m_fPosition[0], aSliceVertices[j].m_fPosition[0]) << "Slice X-Coodrdinate does not match after loading";
					ASSERT_EQ(aVertices[j].m_fPosition[1], aSliceVertices[j].m_fPosition[1]) << "Slice Y-Coodrdinate does not match after loading";
				}

				DWORD nPolygonCount;

				ASSERT_EQ(NMR::lib3mf_slice_getpolygoncount(pSlice.get(), &nPolygonCount), S_OK) << "Unable to retrieve polygon count";
				ASSERT_EQ(nPolygonCount, 1) << "Polygon count does not match";

				DWORD nIndexCount;

				ASSERT_EQ(NMR::lib3mf_slice_getpolygonindexcount(pSlice.get(), 0, &nIndexCount), S_OK) << "Unable to retrieve index count";
				ASSERT_EQ(nIndexCount, 5) << "Index count not correct";

				DWORD aIndices[5];

				ASSERT_EQ(NMR::lib3mf_slice_getpolygonindices(pSlice.get(), 0, aIndices, 5), S_OK) << "Unable to query polygon indices.";

				for (int j = 0; j < 5; j++)
					ASSERT_EQ(aIndices[j], aSliceIndices[j]) << "Slice index does not match";
			}
		}
	}
	TEST(Test_Slice, ReadWriteMultipleSliceRefs)
	{
		const int nModels = 3;
		NMR::MODELSLICEVERTEX aSliceVertices[] = {
			{ { 1.01f, 1.02f } },
			{ { 9.03f, 1.04f } },
			{ { 9.05f, 9.06f } },
			{ { 1.07f, 9.08f } }
		};

		DWORD aSliceIndices[] = { 0, 1, 2, 3, 0 };

		{
			CustomLib3MFBase pModel;

			ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << "Unable to create model";

			CustomLib3MFBase pMeshObject;

			for (int iModel = 0; iModel < nModels; iModel++) {
				ASSERT_EQ(NMR::lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << "Unable to add mesh object to model";
				ASSERT_EQ(NMR::lib3mf_model_setunit(pModel.get(), NMR::MODELUNIT_MILLIMETER), S_OK) << "Cannot set model units";

				NMR::MODELMESHVERTEX a_cVerties[8] = {
					{ { 0,   0, 0 } },
					{ { 100,   0, 0 } },
					{ { 100, 100, 0 } },
					{ { 0, 100, 0 } },

					{ { 0,   0, 100 } },
					{ { 100,   0, 100 } },
					{ { 100, 100, 100 } },
					{ { 0, 100, 100 } }
				};

				for (int i = 0; i < 8; i++)
					ASSERT_EQ(NMR::lib3mf_meshobject_addvertex(pMeshObject.get(), &a_cVerties[i], nullptr), S_OK) << "Unable to add vertex to object";

				NMR::MODELMESHTRIANGLE a_cTris[]{
					{ { 2, 1, 0 } },
					{ { 3, 2, 0 } },
					{ { 4, 5, 6 } },
					{ { 4, 6, 7 } },
					{ { 0, 1, 4 } },
					{ { 5, 4, 1 } },
					{ { 1, 2, 5 } },
					{ { 6, 5, 2 } },
					{ { 2, 3, 6 } },
					{ { 7, 6, 3 } },
					{ { 7, 0, 4 } },
					{ { 0, 7, 3 } },

					{ { 0xFFFF, 0xFFFF, 0xFFFF } }

				};

				for (int i = 0; a_cTris[i].m_nIndices[0] != 0xFFFF; i++)
					ASSERT_EQ(NMR::lib3mf_meshobject_addtriangle(pMeshObject.get(), &a_cTris[i], nullptr), S_OK) << "Unable to add triangle to mesh object";

				CustomLib3MFBase pSliceStack;

				ASSERT_EQ(NMR::lib3mf_model_addslicestack(pModel.get(), 0.005f, &pSliceStack.get()), S_OK) << "Unable to add slice stack to object";
				ASSERT_EQ(NMR::lib3mf_meshobject_setslicestack(pMeshObject.get(), pSliceStack.get()), S_OK) << "Unable to link slice stack to mesh";

				ASSERT_EQ(NMR::lib3mf_slicestack_setusessliceref(pSliceStack.get(), iModel!=1), S_OK) << "Unable to set slice ref";

				BOOL bUsesSliceRef;
				ASSERT_EQ(NMR::lib3mf_slicestack_getusessliceref(pSliceStack.get(), &bUsesSliceRef), S_OK) << "Unable to get slice ref";
				ASSERT_TRUE(bUsesSliceRef == (iModel != 1)) << "Slice ref does not match after write / read";

				for (int i = 0; i < 10-iModel; i++) {
					float fTopZ = ((float)(i+1)) / 10.0f + iModel;
					CustomLib3MFBase pSlice;
					DWORD nPolyIndex, nVertexIndex;

					ASSERT_EQ(NMR::lib3mf_slicestack_addslice(pSliceStack.get(), fTopZ, &pSlice.get()), S_OK) << "Unable to add slice to slicestack";
					ASSERT_EQ(NMR::lib3mf_slice_addvertices(pSlice.get(), aSliceVertices, 4, &nVertexIndex), S_OK) << "Unable to add vertices to slice";
					ASSERT_EQ(NMR::lib3mf_slice_addpolygon(pSlice.get(), aSliceIndices, 5, &nPolyIndex), S_OK) << "Unable to add indices to slice";
				}
			}

			CustomLib3MFBase p3MFWriter;

			ASSERT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
			ASSERT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + "Slice_TestFile_2.3mf").c_str()), S_OK) << "Cannot write 3mf";
		}

		{
			CustomLib3MFBase p3mfReader, pModel;
			ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << "Could not create model";
			ASSERT_EQ(NMR::lib3mf_model_queryreader(pModel.get(), "3mf", &p3mfReader.get()), S_OK) << "Could not query reader";
			ASSERT_EQ(NMR::lib3mf_reader_readfromfileutf8(p3mfReader.get(), (std::string("TestOutput") + separator() + "Slice_TestFile_2.3mf").c_str()), S_OK) << "Unable to load file";

			CustomLib3MFBase pMeshIterator;

			ASSERT_EQ(NMR::lib3mf_model_getmeshobjects(pModel.get(), &pMeshIterator.get()), S_OK) << "Could not get mesh object";
			for (int iModel = 0; iModel < nModels; iModel++) {
				BOOL bHasNext;
				ASSERT_EQ(NMR::lib3mf_resourceiterator_movenext(pMeshIterator.get(), &bHasNext), S_OK) << "Could not retrieve mesh object";

				CustomLib3MFBase pMeshObject;
				ASSERT_EQ(NMR::lib3mf_resourceiterator_getcurrent(pMeshIterator.get(), &pMeshObject.get()), S_OK) << "Could not get mesh from iterator";

				DWORD nSliceStackId;
				ASSERT_EQ(NMR::lib3mf_meshobject_getslicestackid(pMeshObject.get(), &nSliceStackId), S_OK) << "Could not get slice stack id";
				ASSERT_NE(nSliceStackId, 0) << "Invalid slice stack id";

				CustomLib3MFBase pSliceStack;

				ASSERT_EQ(NMR::lib3mf_model_getslicestackById(pModel.get(), nSliceStackId, &pSliceStack.get()), S_OK) << "Could not read slice stack";
				BOOL bUsesSliceRef;
				ASSERT_EQ(NMR::lib3mf_slicestack_getusessliceref(pSliceStack.get(), &bUsesSliceRef), S_OK) << "Unable to get slice ref";
				ASSERT_TRUE(bUsesSliceRef == (iModel != 1)) << "Slice ref does not match after write / read";

				DWORD nSliceCount;
				ASSERT_EQ(NMR::lib3mf_slicestack_getslicecount(pSliceStack.get(), &nSliceCount), S_OK) << "Cannot get number of slices";
				ASSERT_TRUE(nSliceCount == DWORD(10 - iModel));

				for (DWORD i = 0; i < nSliceCount; i++) {
					CustomLib3MFBase pSlice;
					ASSERT_EQ(lib3mf_slicestack_getslice(pSliceStack.get(), i, &pSlice.get()), S_OK) << "unable to retrieve slice " << i;

					DWORD nVertexCount;

					ASSERT_EQ(NMR::lib3mf_slice_getvertexcount(pSlice.get(), &nVertexCount), S_OK) << "unable to retrieve vertex count of slice";
					ASSERT_EQ(nVertexCount, 4);

					NMR::MODELSLICEVERTEX aVertices[4];

					ASSERT_EQ(NMR::lib3mf_slice_getvertices(pSlice.get(), aVertices, 4), S_OK) << "unable to retrieve slice vertices.";

					for (int j = 0; j < 4; j++) {
						ASSERT_EQ(aVertices[j].m_fPosition[0], aSliceVertices[j].m_fPosition[0]) << "Slice X-Coodrdinate does not match after loading";
						ASSERT_EQ(aVertices[j].m_fPosition[1], aSliceVertices[j].m_fPosition[1]) << "Slice Y-Coodrdinate does not match after loading";
					}

					DWORD nPolygonCount;

					ASSERT_EQ(NMR::lib3mf_slice_getpolygoncount(pSlice.get(), &nPolygonCount), S_OK) << "Unable to retrieve polygon count";
					ASSERT_EQ(nPolygonCount, 1) << "Polygon count does not match";

					DWORD nIndexCount;

					ASSERT_EQ(NMR::lib3mf_slice_getpolygonindexcount(pSlice.get(), 0, &nIndexCount), S_OK) << "Unable to retrieve index count";
					ASSERT_EQ(nIndexCount, 5) << "Index count not correct";

					DWORD aIndices[5];

					ASSERT_EQ(NMR::lib3mf_slice_getpolygonindices(pSlice.get(), 0, aIndices, 5), S_OK) << "Unable to query polygon indices.";

					for (int j = 0; j < 5; j++)
						ASSERT_EQ(aIndices[j], aSliceIndices[j]) << "Slice index does not match";
				}
			}
		}
	}

	bool LoadSliceFile(const char * pFileName){
		CustomLib3MFBase p3mfReader, pModel;
		EXPECT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << "Could not create model";
		EXPECT_EQ(NMR::lib3mf_model_queryreader(pModel.get(), "3mf", &p3mfReader.get()), S_OK) << "Could not query reader";
		EXPECT_EQ(NMR::lib3mf_reader_readfromfileutf8(p3mfReader.get(), (sTestFilesPath + separator() + "Slice" + separator() + pFileName).c_str()), S_OK) << "Unable to load file";

		CustomLib3MFBase pBuildItemIterator;
		EXPECT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

		// iterate build items
		BOOL bHasNext;
		EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		while (bHasNext) {
			CustomLib3MFBase pBuildItem;
			EXPECT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

			CustomLib3MFBase pObjectResource;
			EXPECT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Ccould not get build item resource";

			BOOL bIsMeshObject;
			EXPECT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";
			EXPECT_TRUE(bIsMeshObject) << L"Build item is not a mesh object";

			DWORD nSliceStackId;
			EXPECT_EQ(NMR::lib3mf_meshobject_getslicestackid(pObjectResource.get(), &nSliceStackId), S_OK) << "Could not get slice stack id";
			EXPECT_NE(nSliceStackId, 0) << "Invalid slice stack id";

			CustomLib3MFBase pSliceStack;

			EXPECT_EQ(NMR::lib3mf_model_getslicestackById(pModel.get(), nSliceStackId, &pSliceStack.get()), S_OK) << "Could not read slice stack";

			DWORD nSliceCount;
			EXPECT_EQ(NMR::lib3mf_slicestack_getslicecount(pSliceStack.get(), &nSliceCount), S_OK) << "Cannot get number of slices";
			EXPECT_GE(nSliceCount, DWORD(10)) << "Invalid slice stack id";

			EXPECT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
		}
		return true;
	}

	class Slice_LoadFileTest : public testing::TestWithParam<const char*>
	{
	public:
		virtual void SetUp() {}
		virtual void TearDown() {}
	};

	INSTANTIATE_TEST_CASE_P(InstantiationName,
		Slice_LoadFileTest,
		::testing::Values("MultiSliceStack_NoFile.3mf", "MultiSliceStack_OneFile.3mf", "MultiSliceStack_TwoFiles.3mf", "UniSliceStack_TwoFiles.3mf"));

	TEST_P(Slice_LoadFileTest, Slice_ReadFiles)
	{
		return;
		ASSERT_TRUE(LoadSliceFile(GetParam()));
	}



	bool LoadSliceFile_MustFail(const char * pFileName) {
		CustomLib3MFBase p3mfReader, pModel;
		EXPECT_EQ(NMR::lib3mf_createmodel(&pModel.get()), S_OK) << "Could not create model";
		EXPECT_EQ(NMR::lib3mf_model_queryreader(pModel.get(), "3mf", &p3mfReader.get()), S_OK) << "Could not query reader";
		EXPECT_NE(NMR::lib3mf_reader_readfromfileutf8(p3mfReader.get(), (sTestFilesPath + separator() + "Slice" + separator() + pFileName).c_str()), S_OK) << "Able to laod file allthough we should not be";
		return true;
	}

	class Slice_LoadFileTest_MustFail : public testing::TestWithParam<const char*>
	{
	public:
		virtual void SetUp() {}
		virtual void TearDown() {}
	};

	INSTANTIATE_TEST_CASE_P(InstantiationName,
		Slice_LoadFileTest_MustFail,
		::testing::Values("Slice_MustFail_TooRefsTooDeep.3mf"));

	TEST_P(Slice_LoadFileTest_MustFail, LoadFileTest_MustFail)
	{
		return;
		ASSERT_TRUE(LoadSliceFile_MustFail(GetParam()));
	}

}



