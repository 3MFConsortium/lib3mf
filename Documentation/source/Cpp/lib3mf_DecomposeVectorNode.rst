
CDecomposeVectorNode
====================================================================================================


.. cpp:class:: Lib3MF::CDecomposeVectorNode : public CImplicitNode 

	Decomposes a vector into three scalar values




	.. cpp:function:: PImplicitPort GetInputA()

		Retrieves the input

		:returns: the input port for the vector to decompose


	.. cpp:function:: PImplicitPort GetOutputX()

		Retrieves the output for the x component

		:returns: the output for the x component


	.. cpp:function:: PImplicitPort GetOutputY()

		Retrieves the output for the y component

		:returns: the output for the y component


	.. cpp:function:: PImplicitPort GetOutputZ()

		Retrieves the output for the z component

		:returns: the output for the z component


.. cpp:type:: std::shared_ptr<CDecomposeVectorNode> Lib3MF::PDecomposeVectorNode

	Shared pointer to CDecomposeVectorNode to easily allow reference counting.

