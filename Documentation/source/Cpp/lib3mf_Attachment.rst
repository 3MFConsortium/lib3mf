
CAttachment
====================================================================================================


.. cpp:class:: Lib3MF::CAttachment : public CBase 

	




	.. cpp:function:: std::string GetPath()

		Retrieves an attachment's package path. This function will be removed in a later release.

		:returns: returns the attachment's package path string


	.. cpp:function:: void SetPath(const std::string & sPath)

		Sets an attachment's package path. This function will be removed in a later release.

		:param sPath: new path of the attachment. 


	.. cpp:function:: PPackagePart PackagePart()

		Returns the PackagePart that is this attachment.

		:returns: The PackagePart of this attachment.


	.. cpp:function:: std::string GetRelationShipType()

		Retrieves an attachment's relationship type

		:returns: returns the attachment's package relationship type string


	.. cpp:function:: void SetRelationShipType(const std::string & sPath)

		Sets an attachment's relationship type.

		:param sPath: new relationship type string. 


	.. cpp:function:: void WriteToFile(const std::string & sFileName)

		Writes out the attachment as file.

		:param sFileName: file to write into. 


	.. cpp:function:: void ReadFromFile(const std::string & sFileName)

		Reads an attachment from a file. The path of this file is only read when this attachment is being written as part of the 3MF package, or via the WriteToFile or WriteToBuffer methods.

		:param sFileName: file to read from. 


	.. cpp:function:: void ReadFromCallback(const ReadCallback pTheReadCallback, const Lib3MF_uint64 nStreamSize, const SeekCallback pTheSeekCallback, const Lib3MF_pvoid pUserData)

		Reads an attachment from the data provided by a callback function. This callback function is only invoked when this attachment is being written as part of the 3MF package, or via the WriteToFile or WriteToBuffer methods.

		:param pTheReadCallback: Callback to call for reading a data chunk 
		:param nStreamSize: number of bytes the callback returns 
		:param pTheSeekCallback: Callback to call for seeking in the stream. 
		:param pUserData: Userdata that is passed to the callback function 


	.. cpp:function:: Lib3MF_uint64 GetStreamSize()

		Retrieves the size of the attachment stream

		:returns: the stream size


	.. cpp:function:: void WriteToBuffer(std::vector<Lib3MF_uint8> & BufferBuffer)

		Writes out the attachment into a buffer

		:param BufferBuffer: Buffer to write into 


	.. cpp:function:: void ReadFromBuffer(const CInputVector<Lib3MF_uint8> & BufferBuffer)

		Reads an attachment from a memory buffer. This buffer is immediately read (in contrast to the ReadFromCallback and ReadFromFile-methods).

		:param BufferBuffer: Buffer to read from 


.. cpp:type:: std::shared_ptr<CAttachment> Lib3MF::PAttachment

	Shared pointer to CAttachment to easily allow reference counting.

