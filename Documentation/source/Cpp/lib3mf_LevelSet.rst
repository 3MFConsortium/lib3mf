
CLevelSet
====================================================================================================


.. cpp:class:: Lib3MF::CLevelSet : public CObject 

	




	.. cpp:function:: PFunction GetFunction()

		Returns the function that is used as boundary shape.

		:returns: the function to use as boundary shape


	.. cpp:function:: void SetFunction(classParam<CFunction> pTheFunction)

		Sets the function to use as boundary shape.

		:param pTheFunction: the function to use as boundary shape 


	.. cpp:function:: sTransform GetTransform()

		Returns the transformation matrix into the coordinate system of the referenced Function.

		:returns: the transformation matrix


	.. cpp:function:: void SetTransform(const sTransform & Transform)

		Sets the transformation matrix into the coordinate system of the referenced Function.

		:param Transform: new transformation matrix 


	.. cpp:function:: std::string GetChannelName()

		Returns the name of the function output channel to use.

		:returns: the name of the function output channel


	.. cpp:function:: void SetChannelName(const std::string & sChannelName)

		Sets the name of the function output channel to use.

		:param sChannelName: new name of the function output channel 


	.. cpp:function:: void SetMinFeatureSize(const Lib3MF_double dMinFeatureSize)

		Sets the minimal feature size as a hint for the function evaluator

		:param dMinFeatureSize: minimal feature size 


	.. cpp:function:: Lib3MF_double GetMinFeatureSize()

		Returns the minimal feature size as a hint for the function evaluator

		:returns: minimal feature size


	.. cpp:function:: void SetFallBackValue(const Lib3MF_double dFallBackValue)

		Sets the fallback value to use if the function evaluation fails (e.g. evaluates to NaN or Inf).

		:param dFallBackValue: fallback value 


	.. cpp:function:: Lib3MF_double GetFallBackValue()

		Returns the fallback value to use if the function evaluation fails (e.g. evaluates to NaN or Inf).

		:returns: fallback value


	.. cpp:function:: void SetMeshBBoxOnly(const bool bMeshBBoxOnly)

		If set only the bounding box of the mesh is intersected with the boundary

		:param bMeshBBoxOnly: If set only the bounding box of the mesh is intersected with the boundary 


	.. cpp:function:: bool GetMeshBBoxOnly()

		If set only the bounding box of the mesh is intersected with the boundary

		:returns: If set only the bounding box of the mesh is intersected with the boundary


	.. cpp:function:: void SetMesh(classParam<CMeshObject> pTheMesh)

		Sets the mesh to use as evaluation domain

		:param pTheMesh: The mesh 


	.. cpp:function:: PMeshObject GetMesh()

		Returns the mesh that is used as evaluation domain

		:returns: The mesh


	.. cpp:function:: PVolumeData GetVolumeData()

		Retrieves the VolumeData this Object.

		:returns: the VolumeData of this Object


	.. cpp:function:: void SetVolumeData(classParam<CVolumeData> pTheVolumeData)

		Sets the VolumeData of this LevelSet.

		:param pTheVolumeData: the VolumeData of this MeshObject 


.. cpp:type:: std::shared_ptr<CLevelSet> Lib3MF::PLevelSet

	Shared pointer to CLevelSet to easily allow reference counting.

