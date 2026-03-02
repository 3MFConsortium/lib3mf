
CImplicitNode
====================================================================================================


.. cpp:class:: Lib3MF::CImplicitNode : public CBase 

	




	.. cpp:function:: std::string GetIdentifier()

		Retrieves the identifier of the node

		:returns: the identifier


	.. cpp:function:: void SetIdentifier(const std::string & sIdentifier)

		Sets the identifier of the node

		:param sIdentifier: the identifier 


	.. cpp:function:: std::string GetDisplayName()

		Retrieves the display name of the node

		:returns: the display name


	.. cpp:function:: void SetDisplayName(const std::string & sDisplayName)

		Sets the display name of the node

		:param sDisplayName: the display name 


	.. cpp:function:: std::string GetTag()

		Retrieves the tag of the node

		:returns: the tag


	.. cpp:function:: void SetTag(const std::string & sTag)

		Sets the tag of the node

		:param sTag: the tag 


	.. cpp:function:: eImplicitNodeType GetNodeType()

		Retrieves the type of the node

		:returns: the type of the node


	.. cpp:function:: PImplicitPort AddInput(const std::string & sIdentifier, const std::string & sDisplayName)

		Add an input

		:param sIdentifier: the identifier of the input 
		:param sDisplayName: the display name of the input 
		:returns: 


	.. cpp:function:: PImplicitPortIterator GetInputs()

		Retrieves the inputs

		:returns: the iterator for the inputs


	.. cpp:function:: PImplicitPort AddOutput(const std::string & sIdentifier, const std::string & sDisplayName)

		Add an output

		:param sIdentifier: the identifier of the output 
		:param sDisplayName: the display name of the output 
		:returns: 


	.. cpp:function:: PImplicitPortIterator GetOutputs()

		Retrieves the outputs

		:returns: the iterator the outputs


	.. cpp:function:: PImplicitPort FindInput(const std::string & sIdentifier)

		Retrieves an input

		:param sIdentifier: the identifier of the input 
		:returns: the input port


	.. cpp:function:: PImplicitPort FindOutput(const std::string & sIdentifier)

		Retrieves an output

		:param sIdentifier: the identifier of the output 
		:returns: the output port


	.. cpp:function:: bool AreTypesValid()

		Checks if the types of the input and output ports are valid for the node type

		:returns: true, if the types are valid


.. cpp:type:: std::shared_ptr<CImplicitNode> Lib3MF::PImplicitNode

	Shared pointer to CImplicitNode to easily allow reference counting.

