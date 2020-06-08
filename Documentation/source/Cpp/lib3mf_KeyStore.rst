
CKeyStore
====================================================================================================


.. cpp:class:: Lib3MF::CKeyStore : public CBase 

	




	.. cpp:function:: PConsumer AddConsumer(const std::string & sConsumerID, const std::string & sKeyID, const std::string & sKeyValue)

		Adds a consumer to the keystore

		:param sConsumerID: A unique identifier for the consumer 
		:param sKeyID: The id of the key of the consumer 
		:param sKeyValue: The public key for this consumer in PEM format 
		:returns: The consumer instance


	.. cpp:function:: Lib3MF_uint64 GetConsumerCount()

		Gets the number of consumers in the keystore

		:returns: The consumer count


	.. cpp:function:: PConsumer GetConsumer(const Lib3MF_uint64 nConsumerIndex)

		Get a consumer from the keystore

		:param nConsumerIndex: The index of the consumer 
		:returns: The consumer instance


	.. cpp:function:: void RemoveConsumer(CConsumer * pConsumer)

		Removes a consumer from the keystore

		:param pConsumer: The consumer instance to remove 


	.. cpp:function:: PConsumer FindConsumer(const std::string & sConsumerID)

		Finds a consumer by ID

		:param sConsumerID: The ID of the consumer 
		:returns: The consumer instance


	.. cpp:function:: Lib3MF_uint64 GetResourceDataGroupCount()

		Gets the number of resource data group in the keysore

		:returns: The number of resource data available


	.. cpp:function:: PResourceDataGroup AddResourceDataGroup()

		Adds a resource data group into the keystore.

		:returns: The resource data group instance


	.. cpp:function:: PResourceDataGroup GetResourceDataGroup(const Lib3MF_uint64 nResourceDataIndex)

		Gets a resource data group

		:param nResourceDataIndex: The index of the resource data 
		:returns: The resource data group instance


	.. cpp:function:: void RemoveResourceDataGroup(CResourceDataGroup * pResourceDataGroup)

		Removes a resource data group

		:param pResourceDataGroup: The resource data group instance 


	.. cpp:function:: PResourceDataGroup FindResourceDataGroup(CPackagePart * pPartPath)

		Finds a resource data group that contains a particular resourcedata

		:param pPartPath: The target path for the resourcedata hold by the resource data group 
		:returns: The data resource instance


	.. cpp:function:: PResourceData AddResourceData(CResourceDataGroup * pResourceDataGroup, CPackagePart * pPartPath, const eEncryptionAlgorithm eAlgorithm, const eCompression eCompression, const CInputVector<Lib3MF_uint8> & AdditionalAuthenticationDataBuffer)

		Add resourcedata to resourcedatagroup element

		:param pResourceDataGroup: The resource data group where to add this resource data 
		:param pPartPath: The path of the part to be encrypted 
		:param eAlgorithm: The encryption algorithm to be used to encrypt this resource 
		:param eCompression: Whether compression should be used prior to encryption 
		:param AdditionalAuthenticationDataBuffer: Additional data to be encrypted along the contents for better security 
		:returns: The data resource instance


	.. cpp:function:: void RemoveResourceData(CResourceData * pResourceData)

		Removes a resource data

		:param pResourceData: The resource data to be removed 


	.. cpp:function:: PResourceData FindResourceData(CPackagePart * pResourcePath)

		Finds a resource data on this resource group

		:param pResourcePath: The target path for the resourcedata 
		:returns: The resource data instance


	.. cpp:function:: Lib3MF_uint64 GetResourceDataCount()

		Gets the number of resource data in the keysore

		:returns: The number of resource data available


	.. cpp:function:: PResourceData GetResourceData(const Lib3MF_uint64 nResourceDataIndex)

		Gets a resource data

		:param nResourceDataIndex: The index of the resource data 
		:returns: The data resource instance


	.. cpp:function:: std::string GetUUID(bool & bHasUUID)

		Gets the keystore UUID

		:param bHasUUID: flag whether the keystore has a UUID 
		:returns: returns the keystore uuid.


	.. cpp:function:: void SetUUID(const std::string & sUUID)

		Sets the keystore UUID

		:param sUUID: The new keystore uuid. 


.. cpp:type:: std::shared_ptr<CKeyStore> Lib3MF::PKeyStore

	Shared pointer to CKeyStore to easily allow reference counting.

