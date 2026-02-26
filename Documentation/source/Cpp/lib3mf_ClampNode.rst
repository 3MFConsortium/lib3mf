
CClampNode
====================================================================================================


.. cpp:class:: Lib3MF::CClampNode : public COneInputNode 

	Clamps the input value to min and max




	.. cpp:function:: PImplicitPort GetInputMin()

		Retrieves the input for the lower limit

		:returns: the input for the lower limit


	.. cpp:function:: PImplicitPort GetInputMax()

		Retrieves the input for the upper limit

		:returns: the input for the upper limit


.. cpp:type:: std::shared_ptr<CClampNode> Lib3MF::PClampNode

	Shared pointer to CClampNode to easily allow reference counting.

