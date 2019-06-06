
CComponentsObject
====================================================================================================


.. cpp:class:: Lib3MF::CComponentsObject : public CObject 

	




	.. cpp:function:: PComponent AddComponent(CObject * pObjectResource, const sTransform & Transform)

		Adds a new component to a components object.

		:param pObjectResource: object to add as component. Must not lead to circular references! 
		:param Transform: optional transform matrix for the component. 
		:returns: new component instance


	.. cpp:function:: PComponent GetComponent(const Lib3MF_uint32 nIndex)

		Retrieves a component from a component object.

		:param nIndex: index of the component to retrieve (0 to componentcount - 1) 
		:returns: component instance


	.. cpp:function:: Lib3MF_uint32 GetComponentCount()

		Retrieves a component count of a component object.

		:returns: returns the component count


.. cpp:type:: std::shared_ptr<CComponentsObject> Lib3MF::PComponentsObject

	Shared pointer to CComponentsObject to easily allow reference counting.

