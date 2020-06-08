
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

		Reads an attachment from a file.

		:param sFileName: file to read from. 


	.. cpp:function:: Lib3MF_uint64 GetStreamSize()

		Retrieves the size of the attachment stream

		:returns: the stream size


	.. cpp:function:: void WriteToBuffer(std::vector<Lib3MF_uint8> & BufferBuffer)

		Writes out the attachment into a buffer

		:param BufferBuffer: Buffer to write into 


	.. cpp:function:: void ReadFromBuffer(const CInputVector<Lib3MF_uint8> & BufferBuffer)

		Reads an attachment from a memory buffer

		:param BufferBuffer: Buffer to read from 


.. cpp:type:: std::shared_ptr<CAttachment> Lib3MF::PAttachment

	Shared pointer to CAttachment to easily allow reference counting.

