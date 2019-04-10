
CSliceStack
====================================================================================================


.. cpp:class:: Lib3MF::CSliceStack : public CResource 

	




	.. cpp:function:: Lib3MF_double GetBottomZ()

		Get the lower Z-Coordinate of the slice stack.

		:returns: the lower Z-Coordinate the slice stack


	.. cpp:function:: Lib3MF_uint64 GetSliceCount()

		Returns the number of slices

		:returns: the number of slices


	.. cpp:function:: PSlice GetSlice(const Lib3MF_uint64 nSliceIndex)

		Query a slice from the slice stack

		:param nSliceIndex: the index of the slice 
		:returns: the Slice instance


	.. cpp:function:: PSlice AddSlice(const Lib3MF_double dZTop)

		Returns the number of slices

		:param dZTop: upper Z coordinate of the slice 
		:returns: a new Slice instance


	.. cpp:function:: Lib3MF_uint64 GetSliceRefCount()

		Returns the number of slice refs

		:returns: the number of slicereferences


	.. cpp:function:: void AddSliceStackReference(CSliceStack * pTheSliceStack)

		Adds another existing slicestack as sliceref in this slicestack

		:param pTheSliceStack: the slicestack to use as sliceref 


	.. cpp:function:: PSliceStack GetSliceStackReference(const Lib3MF_uint64 nSliceRefIndex)

		Adds another existing slicestack as sliceref in this slicestack

		:param nSliceRefIndex: the index of the slice ref 
		:returns: the slicestack that is used as sliceref


	.. cpp:function:: void CollapseSliceReferences()

		Removes the indirection of slices via slice-refs, i.e. creates the slices of all slice refs of this SliceStack as actual slices of this SliceStack. All previously existing slices or slicerefs will be removed.



	.. cpp:function:: void SetOwnPath(const std::string & sPath)

		Sets the package path where this Slice should be stored. Input an empty string to reset the path

		:param sPath: the package path where this Slice should be stored 


	.. cpp:function:: std::string GetOwnPath()

		Obtains the package path where this Slice should be stored. Returns an empty string if the slicestack is stored within the root model.

		:returns: the package path where this Slice will be stored


.. cpp:type:: std::shared_ptr<CSliceStack> Lib3MF::PSliceStack

	Shared pointer to CSliceStack to easily allow reference counting.

