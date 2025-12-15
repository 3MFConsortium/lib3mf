
CMeshObject
====================================================================================================


.. cpp:class:: Lib3MF::CMeshObject : public CObject 

	




	.. cpp:function:: Lib3MF_uint32 GetVertexCount()

		Returns the vertex count of a mesh object.

		:returns: filled with the vertex count.


	.. cpp:function:: Lib3MF_uint32 GetTriangleCount()

		Returns the triangle count of a mesh object.

		:returns: filled with the triangle count.


	.. cpp:function:: sPosition GetVertex(const Lib3MF_uint32 nIndex)

		Returns the vertex count of a mesh object.

		:param nIndex: Index of the vertex (0 to vertexcount - 1) 
		:returns: filled with the vertex coordinates.


	.. cpp:function:: void SetVertex(const Lib3MF_uint32 nIndex, const sPosition & Coordinates)

		Sets the coordinates of a single vertex of a mesh object

		:param nIndex: Index of the vertex (0 to vertexcount - 1) 
		:param Coordinates: contains the vertex coordinates. 


	.. cpp:function:: Lib3MF_uint32 AddVertex(const sPosition & Coordinates)

		Adds a single vertex to a mesh object

		:param Coordinates: contains the vertex coordinates. 
		:returns: Index of the new vertex


	.. cpp:function:: void GetVertices(std::vector<sPosition> & VerticesBuffer)

		Obtains all vertex positions of a mesh object

		:param VerticesBuffer: contains the vertex coordinates. 


	.. cpp:function:: sTriangle GetTriangle(const Lib3MF_uint32 nIndex)

		Returns indices of a single triangle of a mesh object.

		:param nIndex: Index of the triangle (0 to trianglecount - 1) 
		:returns: filled with the triangle indices.


	.. cpp:function:: void SetTriangle(const Lib3MF_uint32 nIndex, const sTriangle & Indices)

		Sets the indices of a single triangle of a mesh object.

		:param nIndex: Index of the triangle (0 to trianglecount - 1) 
		:param Indices: contains the triangle indices. 


	.. cpp:function:: Lib3MF_uint32 AddTriangle(const sTriangle & Indices)

		Adds a single triangle to a mesh object

		:param Indices: contains the triangle indices. 
		:returns: Index of the new triangle


	.. cpp:function:: void GetTriangleIndices(std::vector<sTriangle> & IndicesBuffer)

		Get all triangles of a mesh object

		:param IndicesBuffer: contains the triangle indices. 


	.. cpp:function:: void SetObjectLevelProperty(const Lib3MF_uint32 nUniqueResourceID, const Lib3MF_uint32 nPropertyID)

		Sets the property at the object-level of the mesh object.

		:param nUniqueResourceID: the object-level Property UniqueResourceID. 
		:param nPropertyID: the object-level PropertyID. 


	.. cpp:function:: bool GetObjectLevelProperty(Lib3MF_uint32 & nUniqueResourceID, Lib3MF_uint32 & nPropertyID)

		Gets the property at the object-level of the mesh object.

		:param nUniqueResourceID: the object-level Property UniqueResourceID. 
		:param nPropertyID: the object-level PropertyID. 
		:returns: Has an object-level property been specified?


	.. cpp:function:: void SetTriangleProperties(const Lib3MF_uint32 nIndex, const sTriangleProperties & Properties)

		Sets the properties of a single triangle of a mesh object.

		:param nIndex: Index of the triangle (0 to trianglecount - 1) 
		:param Properties: contains the triangle properties. 


	.. cpp:function:: void GetTriangleProperties(const Lib3MF_uint32 nIndex, sTriangleProperties & Property)

		Gets the properties of a single triangle of a mesh object.

		:param nIndex: Index of the triangle (0 to trianglecount - 1) 
		:param Property: returns the triangle properties. 


	.. cpp:function:: void SetAllTriangleProperties(const CInputVector<sTriangleProperties> & PropertiesArrayBuffer)

		Sets the properties of all triangles of a mesh object. Sets the object level property to the first entry of the passed triangle properties, if not yet specified.

		:param PropertiesArrayBuffer: contains the triangle properties array. Must have trianglecount elements. 


	.. cpp:function:: void GetAllTriangleProperties(std::vector<sTriangleProperties> & PropertiesArrayBuffer)

		Gets the properties of all triangles of a mesh object.

		:param PropertiesArrayBuffer: returns the triangle properties array. Must have trianglecount elements. 


	.. cpp:function:: void ClearAllProperties()

		Clears all properties of this mesh object (triangle and object-level).



	.. cpp:function:: void SetGeometry(const CInputVector<sPosition> & VerticesBuffer, const CInputVector<sTriangle> & IndicesBuffer)

		Set all triangles of a mesh object

		:param VerticesBuffer: contains the positions. 
		:param IndicesBuffer: contains the triangle indices. 


	.. cpp:function:: bool IsManifoldAndOriented()

		Retrieves, if an object describes a topologically oriented and manifold mesh, according to the core spec.

		:returns: returns, if the object is oriented and manifold.


	.. cpp:function:: PBeamLattice BeamLattice()

		Retrieves the BeamLattice within this MeshObject.

		:returns: the BeamLattice within this MeshObject


	.. cpp:function:: PVolumeData GetVolumeData()

		Retrieves the VolumeData of this MeshObject.

		:returns: the VolumeData of this MeshObject


	.. cpp:function:: void SetVolumeData(classParam<CVolumeData> pTheVolumeData)

		Sets the VolumeData of this MeshObject.

		:param pTheVolumeData: the VolumeData of this MeshObject 


	.. cpp:function:: PTriangleSet AddTriangleSet(const std::string & sIdentifier, const std::string & sName)

		Adds a new triangle set.

		:param sIdentifier: the new identifier. MUST be unique within the mesh. MUST NOT be an empty string 
		:param sName: the human readable name. MUST NOT be an empty string 
		:returns: the new Triangle Set Instance.


	.. cpp:function:: bool HasTriangleSet(const std::string & sIdentifier)

		Checks if a triangle set exists.

		:param sIdentifier: the identifier to be found. 
		:returns: flag if the triangles set exists.


	.. cpp:function:: PTriangleSet FindTriangleSet(const std::string & sIdentifier)

		Finds a new triangle set by identifier. Fails if not existing.

		:param sIdentifier: the identifier to be found. 
		:returns: the triangle Set Instance.


	.. cpp:function:: Lib3MF_uint32 GetTriangleSetCount()

		Returns number of triangle sets.

		:returns: the number of triangle sets of this mesh.


	.. cpp:function:: PTriangleSet GetTriangleSet(const Lib3MF_uint32 nIndex)

		Returns a specific triangle set by index.

		:param nIndex: the index of the triangle set. 
		:returns: the triangle Set Instance.


.. cpp:type:: std::shared_ptr<CMeshObject> Lib3MF::PMeshObject

	Shared pointer to CMeshObject to easily allow reference counting.

