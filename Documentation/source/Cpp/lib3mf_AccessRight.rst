
CAccessRight
====================================================================================================


.. cpp:class:: Lib3MF::CAccessRight : public CBase 

	




	.. cpp:function:: PConsumer GetConsumer()

		Gets the consumer associated with this access right

		:returns: The consumer instance


	.. cpp:function:: eWrappingAlgorithm GetWrappingAlgorithm()

		Gets the associated encryption algorithm

		:returns: The algorithm used for the key in this accessright


	.. cpp:function:: eMgfAlgorithm GetMgfAlgorithm()

		Gets the associated mask generation function algorithm

		:returns: The MFG1 algorithm


	.. cpp:function:: eDigestMethod GetDigestMethod()

		Gets the digest method assoicated

		:returns: The digest method for this accessright


.. cpp:type:: std::shared_ptr<CAccessRight> Lib3MF::PAccessRight

	Shared pointer to CAccessRight to easily allow reference counting.

