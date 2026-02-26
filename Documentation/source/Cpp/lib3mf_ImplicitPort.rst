
CImplicitPort
====================================================================================================


.. cpp:class:: Lib3MF::CImplicitPort : public CBase 

	




	.. cpp:function:: std::string GetIdentifier()

		Retrieves the identifier of the port

		:returns: the identifier


	.. cpp:function:: void SetIdentifier(const std::string & sIdentifier)

		Sets the identifier of the port

		:param sIdentifier: the identifier 


	.. cpp:function:: std::string GetDisplayName()

		Retrieves the display name of the port

		:returns: the display name


	.. cpp:function:: void SetDisplayName(const std::string & sDisplayName)

		Sets the display name of the port

		:param sDisplayName: the display name 


	.. cpp:function:: void SetType(const eImplicitPortType eImplicitPortType)

		Sets the type of the port

		:param eImplicitPortType: the type 


	.. cpp:function:: eImplicitPortType GetType()

		Retrieves the type of the port

		:returns: the type


	.. cpp:function:: std::string GetReference()

		Retrieves the reference of the port, only used for input ports

		:returns: the reference


	.. cpp:function:: void SetReference(const std::string & sReference)

		Sets the reference of the port, only used for input ports

		:param sReference: the reference 


.. cpp:type:: std::shared_ptr<CImplicitPort> Lib3MF::PImplicitPort

	Shared pointer to CImplicitPort to easily allow reference counting.

