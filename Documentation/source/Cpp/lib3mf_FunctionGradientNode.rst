
CFunctionGradientNode
====================================================================================================


.. cpp:class:: Lib3MF::CFunctionGradientNode : public CImplicitNode 

	Gradient of a function




	.. cpp:function:: PImplicitPort GetInputFunctionID()

		Retrieves the input for the function id

		:returns: the input port for the function


	.. cpp:function:: PImplicitPort GetInputPos()

		Retrieves the input for the position

		:returns: the input port for the position


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


	.. cpp:function:: PImplicitPort GetOutputNormalizedGradient()

		Retrieves the normalized gradient output

		:returns: the output port for the normalized gradient


	.. cpp:function:: PImplicitPort GetOutputGradient()

		Retrieves the raw gradient output

		:returns: the output port for the raw gradient


	.. cpp:function:: PImplicitPort GetOutputMagnitude()

		Retrieves the gradient magnitude output

		:returns: the output port for the gradient magnitude


.. cpp:type:: std::shared_ptr<CFunctionGradientNode> Lib3MF::PFunctionGradientNode

	Shared pointer to CFunctionGradientNode to easily allow reference counting.

