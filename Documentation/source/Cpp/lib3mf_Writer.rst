
CWriter
====================================================================================================


.. cpp:class:: Lib3MF::CWriter : public CBase 

	




	.. cpp:function:: void WriteToFile(const std::string & sFilename)

		Writes out the model as file. The file type is specified by the Model Writer class.

		:param sFilename: Filename to write into 


	.. cpp:function:: Lib3MF_uint64 GetStreamSize()

		Retrieves the size of the full 3MF file stream.

		:returns: the stream size


	.. cpp:function:: void WriteToBuffer(std::vector<Lib3MF_uint8> & BufferBuffer)

		Writes out the 3MF file into a memory buffer

		:param BufferBuffer: buffer to write into 


	.. cpp:function:: void WriteToCallback(const WriteCallback pTheWriteCallback, const SeekCallback pTheSeekCallback, const Lib3MF_pvoid pUserData)

		Writes out the model and passes the data to a provided callback function. The file type is specified by the Model Writer class.

		:param pTheWriteCallback: Callback to call for writing a data chunk 
		:param pTheSeekCallback: Callback to call for seeking in the stream 
		:param pUserData: Userdata that is passed to the callback function 


	.. cpp:function:: void SetProgressCallback(const ProgressCallback pProgressCallback, const Lib3MF_pvoid pUserData)

		Set the progress callback for calls to this writer

		:param pProgressCallback: pointer to the callback function. 
		:param pUserData: pointer to arbitrary user data that is passed without modification to the callback. 


	.. cpp:function:: Lib3MF_uint32 GetDecimalPrecision()

		Returns the number of digits after the decimal point to be written in each vertex coordinate-value.

		:returns: The number of digits to be written in each vertex coordinate-value after the decimal point.


	.. cpp:function:: void SetDecimalPrecision(const Lib3MF_uint32 nDecimalPrecision)

		Sets the number of digits after the decimal point to be written in each vertex coordinate-value.

		:param nDecimalPrecision: The number of digits to be written in each vertex coordinate-value after the decimal point. 


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

		Registers a callback to deal with data key encryption/decryption from keystore

		:param sConsumerID: The ConsumerID to register for 
		:param pTheCallback: The callback to be callede for wrapping and encryption key 
		:param pUserData: Userdata that is passed to the callback function 


	.. cpp:function:: void SetContentEncryptionCallback(const ContentEncryptionCallback pTheCallback, const Lib3MF_pvoid pUserData)

		Registers a callback to deal with encryption of content

		:param pTheCallback: The callback used to encrypt content 
		:param pUserData: Userdata that is passed to the callback function 


.. cpp:type:: std::shared_ptr<CWriter> Lib3MF::PWriter

	Shared pointer to CWriter to easily allow reference counting.

