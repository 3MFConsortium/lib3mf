
CConstVecNode
====================================================================================================


.. cpp:class:: Lib3MF::CConstVecNode : public CImplicitNode 

	Constant vector




	.. cpp:function:: void SetVector(const sVector & Value)

		Sets the vector value of the node

		:param Value: the value 


	.. cpp:function:: sVector GetVector()

		Retrieves the vector value of the node

		:returns: the value


	.. cpp:function:: PImplicitPort GetOutputVector()

		Retrieves the output

		:returns: the output


.. cpp:type:: std::shared_ptr<CConstVecNode> Lib3MF::PConstVecNode

	Shared pointer to CConstVecNode to easily allow reference counting.

