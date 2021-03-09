
CContentEncryptionParams
====================================================================================================


.. cpp:class:: Lib3MF::CContentEncryptionParams : public CBase 

	




	.. cpp:function:: eEncryptionAlgorithm GetEncryptionAlgorithm()

		Returns the encryption method to be used in this encryption process

		:returns: 


	.. cpp:function:: void GetKey(std::vector<Lib3MF_uint8> & ByteDataBuffer)

		Gets the key for the resource associated

		:param ByteDataBuffer: Pointer to a buffer where to place the key. 


	.. cpp:function:: void GetInitializationVector(std::vector<Lib3MF_uint8> & ByteDataBuffer)

		Gets the IV data

		:param ByteDataBuffer: Pointer to a buffer where to place the data. 


	.. cpp:function:: void GetAuthenticationTag(std::vector<Lib3MF_uint8> & ByteDataBuffer)

		A handler descriptor that uniquely identifies the context of the resource. Each resource will be assigned a different value

		:param ByteDataBuffer: Pointer to a buffer where to place the data. 


	.. cpp:function:: void SetAuthenticationTag(const CInputVector<Lib3MF_uint8> & ByteDataBuffer)

		Sets the authentication tag

		:param ByteDataBuffer: The authentication tag size 


	.. cpp:function:: void GetAdditionalAuthenticationData(std::vector<Lib3MF_uint8> & ByteDataBuffer)

		A handler descriptor that uniquely identifies the context of the resource. Each resource will be assigned a different value

		:param ByteDataBuffer: Buffer where the data will be placed 


	.. cpp:function:: Lib3MF_uint64 GetDescriptor()

		A handler descriptor that uniquely identifies the context of the resource. Each resource will be assigned a different value

		:returns: 


	.. cpp:function:: std::string GetKeyUUID()

		Gets the resourcedatagroup keyuuid

		:returns: The resourcedatagroup keyuuid that may be use to reference an external key


.. cpp:type:: std::shared_ptr<CContentEncryptionParams> Lib3MF::PContentEncryptionParams

	Shared pointer to CContentEncryptionParams to easily allow reference counting.

