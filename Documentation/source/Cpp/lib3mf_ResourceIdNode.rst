
CResourceIdNode
====================================================================================================


.. cpp:class:: Lib3MF::CResourceIdNode : public CImplicitNode 

	Resource ID




	.. cpp:function:: void SetResource(classParam<CResource> pResource)

		Sets the Resource that the resourceid attribute of the node will point to

		:param pResource: the resource 


	.. cpp:function:: PResource GetResource()

		Retrieves the resource of the node

		:returns: the resource


	.. cpp:function:: PImplicitPort GetOutputValue()

		Retrieves the output

		:returns: the output


.. cpp:type:: std::shared_ptr<CResourceIdNode> Lib3MF::PResourceIdNode

	Shared pointer to CResourceIdNode to easily allow reference counting.

