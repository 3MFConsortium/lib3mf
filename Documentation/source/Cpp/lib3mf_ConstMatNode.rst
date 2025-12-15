
CConstMatNode
====================================================================================================


.. cpp:class:: Lib3MF::CConstMatNode : public CImplicitNode 

	Constant matrix




	.. cpp:function:: void SetMatrix(const sMatrix4x4 & Value)

		Sets the matrix value of the node

		:param Value: the value 


	.. cpp:function:: sMatrix4x4 GetMatrix()

		Retrieves the matrix value of the node

		:returns: the matrix


	.. cpp:function:: PImplicitPort GetOutputMatrix()

		Retrieves the output

		:returns: the output


.. cpp:type:: std::shared_ptr<CConstMatNode> Lib3MF::PConstMatNode

	Shared pointer to CConstMatNode to easily allow reference counting.

