
CConstantNode
====================================================================================================


.. cpp:class:: Lib3MF::CConstantNode : public CImplicitNode 

	Constant value




	.. cpp:function:: void SetConstant(const Lib3MF_double dValue)

		Sets the constant value of the node

		:param dValue: the value 


	.. cpp:function:: Lib3MF_double GetConstant()

		Retrieves the constant value of the node

		:returns: the value


	.. cpp:function:: PImplicitPort GetOutputValue()

		Retrieves the output

		:returns: the output


.. cpp:type:: std::shared_ptr<CConstantNode> Lib3MF::PConstantNode

	Shared pointer to CConstantNode to easily allow reference counting.

