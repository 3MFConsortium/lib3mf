
CTriangleSet
====================================================================================================


.. cpp:class:: Lib3MF::CTriangleSet : public CBase 

	




	.. cpp:function:: void SetName(const std::string & sName)

		sets the name of the triangle set

		:param sName: the new name 


	.. cpp:function:: std::string GetName()

		returns the name of the triangle set

		:returns: returns the name


	.. cpp:function:: void SetIdentifier(const std::string & sIdentifier)

		sets the identifier of the triangle set.

		:param sIdentifier: the new identifier. MUST be unique within the mesh. MUST NOT be an empty string 


	.. cpp:function:: std::string GetIdentifier()

		returns the identifier of the triangle set

		:returns: returns the identifier


	.. cpp:function:: void AddTriangle(const Lib3MF_uint32 nTriangleIndex)

		adds a triangle to the set. Does nothing if triangle is already in the set.

		:param nTriangleIndex: Triangle index to add. MUST be between 0 and TriangleCount - 1. 


	.. cpp:function:: void RemoveTriangle(const Lib3MF_uint32 nTriangleIndex)

		removes a triangle from the set

		:param nTriangleIndex: Triangle index to remove. MUST be between 0 and TriangleCount - 1. 


	.. cpp:function:: void Clear()

		clears all triangles from the list



	.. cpp:function:: void SetTriangleList(const CInputVector<Lib3MF_uint32> & TriangleIndicesBuffer)

		Sets all triangles in the list, while clearing old values. Duplicates will be merged.

		:param TriangleIndicesBuffer: Triangle indices to add. Every element MUST be between 0 and TriangleCount - 1. 


	.. cpp:function:: void GetTriangleList(std::vector<Lib3MF_uint32> & TriangleIndicesBuffer)

		Retrieves all the triangles in the TriangleSet

		:param TriangleIndicesBuffer: retrieves the indices of the triangles in this TriangleSet 


	.. cpp:function:: void AddTriangleList(const CInputVector<Lib3MF_uint32> & TriangleIndicesBuffer)

		Adds multiple triangles in the list. Duplicates will be merged.

		:param TriangleIndicesBuffer: Triangle indices to add. Every element MUST be between 0 and TriangleCount - 1. 


	.. cpp:function:: void Merge(classParam<CTriangleSet> pOtherTriangleSet, const bool bDeleteOther)

		Merges another Triangle set.

		:param pOtherTriangleSet: Other triangle set to merge. 
		:param bDeleteOther: Flag if other triangle set is getting removed. 


	.. cpp:function:: void DeleteSet()

		Deletes the whole set from the mesh.



	.. cpp:function:: PTriangleSet Duplicate(const std::string & sIdentifier)

		Duplicates the set in the mesh.

		:param sIdentifier: the new identifier. MUST be unique within the mesh. MUST NOT be an empty string 
		:returns: Copy of the triangle set.


.. cpp:type:: std::shared_ptr<CTriangleSet> Lib3MF::PTriangleSet

	Shared pointer to CTriangleSet to easily allow reference counting.

