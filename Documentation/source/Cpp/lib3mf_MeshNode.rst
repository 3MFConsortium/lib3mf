
CMeshNode
====================================================================================================


.. cpp:class:: Lib3MF::CMeshNode : public CImplicitNode 

	Signed distance to mesh




	.. cpp:function:: PImplicitPort GetInputMesh()

		Retrieves the input for the model resource id of the mesh

		:returns: the input port for the model resource id of the mesh


	.. cpp:function:: PImplicitPort GetInputPos()

		Retrieves the input for the position

		:returns: the input port for the position


	.. cpp:function:: PImplicitPort GetOutputDistance()

		Retrieves the output

		:returns: the output port for the signed distance to the mesh


.. cpp:type:: std::shared_ptr<CMeshNode> Lib3MF::PMeshNode

	Shared pointer to CMeshNode to easily allow reference counting.

