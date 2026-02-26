
CFunction
====================================================================================================


.. cpp:class:: Lib3MF::CFunction : public CResource 

	




	.. cpp:function:: std::string GetDisplayName()

		Retrieves the display name of the function

		:returns: the display name


	.. cpp:function:: void SetDisplayName(const std::string & sDisplayName)

		Sets the display name of the function

		:param sDisplayName: the display name 


	.. cpp:function:: PImplicitPort AddInput(const std::string & sIdentifier, const std::string & sDisplayName, const eImplicitPortType eType)

		Add an input

		:param sIdentifier: the identifier of the input 
		:param sDisplayName: the display name of the input 
		:param eType: the type of the input 
		:returns: The added input port


	.. cpp:function:: PImplicitPortIterator GetInputs()

		Retrieves the inputs

		:returns: iterator for the list of inputs


	.. cpp:function:: void RemoveInput(classParam<CImplicitPort> pInput)

		Removes an input

		:param pInput: The input to be removed 


	.. cpp:function:: PImplicitPort AddOutput(const std::string & sIdentifier, const std::string & sDisplayName, const eImplicitPortType eType)

		Add an output

		:param sIdentifier: the identifier of the output 
		:param sDisplayName: the display name of the output 
		:param eType: the type of the input 
		:returns: The added input port


	.. cpp:function:: PImplicitPortIterator GetOutputs()

		Retrieves the outputs

		:returns: iterator for the outputs


	.. cpp:function:: void RemoveOutput(classParam<CImplicitPort> pOutput)

		Removes an output

		:param pOutput: The output to be removed 


	.. cpp:function:: PImplicitPort FindInput(const std::string & sIdentifier)

		Retrieves an input

		:param sIdentifier: the identifier of the input 
		:returns: the input port


	.. cpp:function:: PImplicitPort FindOutput(const std::string & sIdentifier)

		Retrieves an output

		:param sIdentifier: the identifier of the output 
		:returns: the output port


.. cpp:type:: std::shared_ptr<CFunction> Lib3MF::PFunction

	Shared pointer to CFunction to easily allow reference counting.

