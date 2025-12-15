
CNormalizeDistanceNode
====================================================================================================


.. cpp:class:: Lib3MF::CNormalizeDistanceNode : public CImplicitNode 

	Normalized signed distance from referenced function




	.. cpp:function:: PImplicitPort GetInputFunctionID()

		Retrieves the input for the function id

		:returns: the input port for the function


	.. cpp:function:: PImplicitPort GetInputPos()

		Retrieves the input for the position

		:returns: the input port for the position (vector)


	.. cpp:function:: PImplicitPort GetInputStep()

		Retrieves the input for the finite difference step

		:returns: the input port for the finite difference step


	.. cpp:function:: void SetScalarOutputName(const std::string & sScalarOutputName)

		Sets the name of the referenced scalar output

		:param sScalarOutputName: the name of the scalar output of the referenced function 


	.. cpp:function:: std::string GetScalarOutputName()

		Retrieves the name of the referenced scalar output

		:returns: the name of the scalar output of the referenced function


	.. cpp:function:: void SetVectorInputName(const std::string & sVectorInputName)

		Sets the name of the referenced vector input

		:param sVectorInputName: the name of the vector input (float3) of the referenced function 


	.. cpp:function:: std::string GetVectorInputName()

		Retrieves the name of the referenced vector input

		:returns: the name of the vector input (float3) of the referenced function


	.. cpp:function:: PImplicitPort GetOutputResult()

		Retrieves the normalized result output

		:returns: the output port for the normalized distance


.. cpp:type:: std::shared_ptr<CNormalizeDistanceNode> Lib3MF::PNormalizeDistanceNode

	Shared pointer to CNormalizeDistanceNode to easily allow reference counting.

