
CMatrixFromColumnsNode
====================================================================================================


.. cpp:class:: Lib3MF::CMatrixFromColumnsNode : public CImplicitNode 

	Composes a 4x4 matrix from four column vectors A,B,C,D




	.. cpp:function:: PImplicitPort GetInputA()

		Retrieves the input for the first column

		:returns: the input for the first column


	.. cpp:function:: PImplicitPort GetInputB()

		Retrieves the input for the second column

		:returns: the input for the second column


	.. cpp:function:: PImplicitPort GetInputC()

		Retrieves the input for the third column

		:returns: the input for the third column


	.. cpp:function:: PImplicitPort GetInputD()

		Retrieves the input for the fourth column

		:returns: the input for the fourth column


	.. cpp:function:: PImplicitPort GetOutputResult()

		Retrieves the output

		:returns: the output


.. cpp:type:: std::shared_ptr<CMatrixFromColumnsNode> Lib3MF::PMatrixFromColumnsNode

	Shared pointer to CMatrixFromColumnsNode to easily allow reference counting.

