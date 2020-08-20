
CResourceDataGroup
====================================================================================================


.. cpp:class:: Lib3MF::CResourceDataGroup : public CBase 

	




	.. cpp:function:: std::string GetKeyUUID()

		Sets the resourcedatagroup keyuuid

		:returns: The new resourcedatagroup keyuuid.


	.. cpp:function:: PAccessRight AddAccessRight(CConsumer * pConsumer, const eWrappingAlgorithm eWrappingAlgorithm, const eMgfAlgorithm eMgfAlgorithm, const eDigestMethod eDigestMethod)

		Add accessright to resourcedatagroup element

		:param pConsumer: The Consumer reference 
		:param eWrappingAlgorithm: The key wrapping algorithm to be used 
		:param eMgfAlgorithm: The mask generation function to be used 
		:param eDigestMethod: The digest mechanism to be used 
		:returns: The acess right instance


	.. cpp:function:: PAccessRight FindAccessRightByConsumer(CConsumer * pConsumer)

		Finds the AccessRight associated with a Consumer

		:param pConsumer: The Consumer instance 
		:returns: The AcessRight instance


	.. cpp:function:: void RemoveAccessRight(CConsumer * pConsumer)

		Removes access from a Consumer on this resource data group

		:param pConsumer: The Consumer instance 


.. cpp:type:: std::shared_ptr<CResourceDataGroup> Lib3MF::PResourceDataGroup

	Shared pointer to CResourceDataGroup to easily allow reference counting.

