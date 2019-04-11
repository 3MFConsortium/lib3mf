
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


.. cpp:type:: std::shared_ptr<CWriter> Lib3MF::PWriter

	Shared pointer to CWriter to easily allow reference counting.

