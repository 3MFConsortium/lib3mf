
CResourceData
====================================================================================================


.. cpp:class:: Lib3MF::CResourceData : public CBase 

	




	.. cpp:function:: PPackagePart GetPath()

		Gets the encrypted part path

		:returns: The part path


	.. cpp:function:: eEncryptionAlgorithm GetEncryptionAlgorithm()

		Gets the encryption algorithm used to encrypt this ResourceData

		:returns: The encryption algorithm


	.. cpp:function:: eCompression GetCompression()

		Tells whether this ResourceData is compressed or not

		:returns: The compression method


	.. cpp:function:: void GetAdditionalAuthenticationData(std::vector<Lib3MF_uint8> & ByteDataBuffer)

		Tells whether this ResourceData is compressed or not

		:param ByteDataBuffer: The compression method 


.. cpp:type:: std::shared_ptr<CResourceData> Lib3MF::PResourceData

	Shared pointer to CResourceData to easily allow reference counting.

