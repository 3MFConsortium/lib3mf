
CComponent
====================================================================================================


.. cpp:class:: Lib3MF::CComponent : public CBase 

	




	.. cpp:function:: PObject GetObjectResource()

		Returns the Resource Instance of the component.

		:returns: filled with the Resource Instance.


	.. cpp:function:: Lib3MF_uint32 GetObjectResourceID()

		Returns the UniqueResourceID of the component.

		:returns: returns the UniqueResourceID.


	.. cpp:function:: std::string GetUUID(bool & bHasUUID)

		returns, whether a component has a UUID and, if true, the component's UUID

		:param bHasUUID: flag whether the component has a UUID 
		:returns: the UUID as string of the form 'xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx'


	.. cpp:function:: void SetUUID(const std::string & sUUID)

		sets the component's UUID

		:param sUUID: the UUID as string of the form 'xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx' 


	.. cpp:function:: bool HasTransform()

		Returns, if the component has a different transformation than the identity matrix

		:returns: if true is returned, the transformation is not equal than the identity


	.. cpp:function:: sTransform GetTransform()

		Returns the transformation matrix of the component.

		:returns: filled with the component transformation matrix


	.. cpp:function:: void SetTransform(const sTransform & Transform)

		Sets the transformation matrix of the component.

		:param Transform: new transformation matrix 


.. cpp:type:: std::shared_ptr<CComponent> Lib3MF::PComponent

	Shared pointer to CComponent to easily allow reference counting.

