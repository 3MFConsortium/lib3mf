
CMatrixFromRowsNode
====================================================================================================


.. cpp:class:: Lib3MF::CMatrixFromRowsNode : public CImplicitNode 

	Composes a 4x4 matrix from four row vectors A,B,C,D




	.. cpp:function:: PImplicitPort GetInputA()

		Retrieves the input for the first row

		:returns: the input for the first row


	.. cpp:function:: PImplicitPort GetInputB()

		Retrieves the input for the second row

		:returns: the input for the second row


	.. cpp:function:: PImplicitPort GetInputC()

		Retrieves the input for the third row

		:returns: the input for the third row


	.. cpp:function:: PImplicitPort GetInputD()

		Retrieves the input for the fourth row

		:returns: the input for the fourth row


	.. cpp:function:: PImplicitPort GetOutputResult()

		Retrieves the output

		:returns: the output


.. cpp:type:: std::shared_ptr<CMatrixFromRowsNode> Lib3MF::PMatrixFromRowsNode

	Shared pointer to CMatrixFromRowsNode to easily allow reference counting.

