
CReader
====================================================================================================


.. cpp:class:: Lib3MF::CReader : public CBase 

	




	.. cpp:function:: void ReadFromFile(const std::string & sFilename)

		Reads a model from a file. The file type is specified by the Model Reader class

		:param sFilename: Filename to read from 


	.. cpp:function:: void ReadFromBuffer(const CInputVector<Lib3MF_uint8> & BufferBuffer)

		Reads a model from a memory buffer.

		:param BufferBuffer: Buffer to read from 


	.. cpp:function:: void ReadFromCallback(const ReadCallback pTheReadCallback, const Lib3MF_uint64 nStreamSize, const SeekCallback pTheSeekCallback, const Lib3MF_pvoid pUserData)

		Reads a model and from the data provided by a callback function

		:param pTheReadCallback: Callback to call for reading a data chunk 
		:param nStreamSize: number of bytes the callback returns 
		:param pTheSeekCallback: Callback to call for seeking in the stream. 
		:param pUserData: Userdata that is passed to the callback function 


	.. cpp:function:: void SetProgressCallback(const ProgressCallback pProgressCallback, const Lib3MF_pvoid pUserData)

		Set the progress callback for calls to this writer

		:param pProgressCallback: pointer to the callback function. 
		:param pUserData: pointer to arbitrary user data that is passed without modification to the callback. 


	.. cpp:function:: void AddRelationToRead(const std::string & sRelationShipType)

		Adds a relationship type which shall be read as attachment in memory while loading

		:param sRelationShipType: String of the relationship type 


	.. cpp:function:: void RemoveRelationToRead(const std::string & sRelationShipType)

		Removes a relationship type which shall be read as attachment in memory while loading

		:param sRelationShipType: String of the relationship type 


	.. cpp:function:: void SetStrictModeActive(const bool bStrictModeActive)

		Activates (deactivates) the strict mode of the reader.

		:param bStrictModeActive: flag whether strict mode is active or not. 


	.. cpp:function:: bool GetStrictModeActive()

		Queries whether the strict mode of the reader is active or not

		:returns: returns flag whether strict mode is active or not.


	.. cpp:function:: std::string GetWarning(const Lib3MF_uint32 nIndex, Lib3MF_uint32 & nErrorCode)

		Returns Warning and Error Information of the read process

		:param nIndex: Index of the Warning. Valid values are 0 to WarningCount - 1 
		:param nErrorCode: filled with the error code of the warning 
		:returns: the message of the warning


	.. cpp:function:: Lib3MF_uint32 GetWarningCount()

		Returns Warning and Error Count of the read process

		:returns: filled with the count of the occurred warnings.


	.. cpp:function:: void AddKeyWrappingCallback(const std::string & sConsumerID, const KeyWrappingCallback pTheCallback, const Lib3MF_pvoid pUserData)

		Registers a callback to deal with key wrapping mechanism from keystore

		:param sConsumerID: The ConsumerID to register for 
		:param pTheCallback: The callback used to decrypt data key 
		:param pUserData: Userdata that is passed to the callback function 


	.. cpp:function:: void SetContentEncryptionCallback(const ContentEncryptionCallback pTheCallback, const Lib3MF_pvoid pUserData)

		Registers a callback to deal with encryption of content

		:param pTheCallback: The callback used to encrypt content 
		:param pUserData: Userdata that is passed to the callback function 


.. cpp:type:: std::shared_ptr<CReader> Lib3MF::PReader

	Shared pointer to CReader to easily allow reference counting.

