
CConsumer
====================================================================================================


.. cpp:class:: Lib3MF::CConsumer : public CBase 

	




	.. cpp:function:: std::string GetConsumerID()

		Gets the consumerid

		:returns: A unique identifier for the consumers


	.. cpp:function:: std::string GetKeyID()

		Getts the keyid

		:returns: The identifier for the key of this consumer


	.. cpp:function:: std::string GetKeyValue()

		Gets the keyvalue associated with this consumer

		:returns: The public key, when available, of this consumer


.. cpp:type:: std::shared_ptr<CConsumer> Lib3MF::PConsumer

	Shared pointer to CConsumer to easily allow reference counting.

