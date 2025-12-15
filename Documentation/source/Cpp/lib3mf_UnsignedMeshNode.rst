
CUnsignedMeshNode
====================================================================================================


.. cpp:class:: Lib3MF::CUnsignedMeshNode : public CImplicitNode 

	Unsigned distance to mesh




	.. cpp:function:: PImplicitPort GetInputMesh()

		Retrieves the input for the model resource id of the mesh

		:returns: the input port for the model resource id of the mesh


	.. cpp:function:: PImplicitPort GetInputPos()

		Retrieves the input for the position

		:returns: the input port for the position


	.. cpp:function:: PImplicitPort GetOutputDistance()

		Retrieves the output

		:returns: the output port for the unsigned distance to the mesh


.. cpp:type:: std::shared_ptr<CUnsignedMeshNode> Lib3MF::PUnsignedMeshNode

	Shared pointer to CUnsignedMeshNode to easily allow reference counting.

