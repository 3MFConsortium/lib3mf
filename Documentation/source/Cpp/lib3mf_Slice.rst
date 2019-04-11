
CSlice
====================================================================================================


.. cpp:class:: Lib3MF::CSlice : public CBase 

	




	.. cpp:function:: void SetVertices(const CInputVector<sPosition2D> & VerticesBuffer)

		Set all vertices of a slice. All polygons will be cleared.

		:param VerticesBuffer: contains the positions. 


	.. cpp:function:: void GetVertices(std::vector<sPosition2D> & VerticesBuffer)

		Get all vertices of a slice

		:param VerticesBuffer: contains the positions. 


	.. cpp:function:: Lib3MF_uint64 GetVertexCount()

		Get the number of vertices in a slice

		:returns: the number of vertices in the slice


	.. cpp:function:: Lib3MF_uint64 AddPolygon(const CInputVector<Lib3MF_uint32> & IndicesBuffer)

		Add a new polygon to this slice

		:param IndicesBuffer: the new indices of the new polygon 
		:returns: the index of the new polygon


	.. cpp:function:: Lib3MF_uint64 GetPolygonCount()

		Get the number of polygons in the slice

		:returns: the number of polygons in the slice


	.. cpp:function:: void SetPolygonIndices(const Lib3MF_uint64 nIndex, const CInputVector<Lib3MF_uint32> & IndicesBuffer)

		Set all indices of a polygon

		:param nIndex: the index of the polygon to manipulate 
		:param IndicesBuffer: the new indices of the index-th polygon 


	.. cpp:function:: void GetPolygonIndices(const Lib3MF_uint64 nIndex, std::vector<Lib3MF_uint32> & IndicesBuffer)

		Get all vertices of a slice

		:param nIndex: the index of the polygon to manipulate 
		:param IndicesBuffer: the indices of the index-th polygon  


	.. cpp:function:: Lib3MF_uint64 GetPolygonIndexCount(const Lib3MF_uint64 nIndex)

		Get the number of vertices in a slice

		:param nIndex: the index of the polygon to manipulate 
		:returns: the number of indices of the index-th polygon


	.. cpp:function:: Lib3MF_double GetZTop()

		Get the upper Z-Coordinate of this slice.

		:returns: the upper Z-Coordinate of this slice


.. cpp:type:: std::shared_ptr<CSlice> Lib3MF::PSlice

	Shared pointer to CSlice to easily allow reference counting.

