
CResource
====================================================================================================


.. cpp:class:: Lib3MF::CResource : public CBase 

	




	.. cpp:function:: Lib3MF_uint32 GetResourceID()

		Retrieves the unique id of this resource within a package. This function will be removed in a later release in favor of GetUniqueResourceID

		:returns: Retrieves the unique id of this resource within a package.


	.. cpp:function:: Lib3MF_uint32 GetUniqueResourceID()

		Retrieves the unique id of this resource within a package.

		:returns: Retrieves the unique id of this resource within a package.


	.. cpp:function:: PPackagePart PackagePart()

		Returns the PackagePart within which this resource resides

		:returns: the PackagePart within which this resource resides.


	.. cpp:function:: void SetPackagePart(CPackagePart * pPackagePart)

		Sets the new PackagePart within which this resource resides

		:param pPackagePart: the new PackagePart within which this resource resides. 


	.. cpp:function:: Lib3MF_uint32 GetModelResourceID()

		Retrieves the id of this resource within a model.

		:returns: Retrieves the id of this resource within a model.


.. cpp:type:: std::shared_ptr<CResource> Lib3MF::PResource

	Shared pointer to CResource to easily allow reference counting.

