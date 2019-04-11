
CMetaDataGroup
====================================================================================================


.. cpp:class:: Lib3MF::CMetaDataGroup : public CBase 

	




	.. cpp:function:: Lib3MF_uint32 GetMetaDataCount()

		returns the number of metadata in this metadatagroup

		:returns: returns the number metadata


	.. cpp:function:: PMetaData GetMetaData(const Lib3MF_uint32 nIndex)

		returns a metadata value within this metadatagroup

		:param nIndex: Index of the Metadata. 
		:returns: an instance of the metadata


	.. cpp:function:: PMetaData GetMetaDataByKey(const std::string & sNameSpace, const std::string & sName)

		returns a metadata value within this metadatagroup

		:param sNameSpace: the namespace of the metadata 
		:param sName: the name of the Metadata 
		:returns: an instance of the metadata


	.. cpp:function:: void RemoveMetaDataByIndex(const Lib3MF_uint32 nIndex)

		removes metadata by index from the model.

		:param nIndex:  Index of the metadata to remove 


	.. cpp:function:: void RemoveMetaData(CMetaData * pTheMetaData)

		removes metadata from the model.

		:param pTheMetaData: The metadata to remove 


	.. cpp:function:: PMetaData AddMetaData(const std::string & sNameSpace, const std::string & sName, const std::string & sValue, const std::string & sType, const bool bMustPreserve)

		adds a new metadata to this metadatagroup

		:param sNameSpace: the namespace of the metadata 
		:param sName: the name of the metadata 
		:param sValue: the value of the metadata 
		:param sType: the type of the metadata 
		:param bMustPreserve: shuold the metadata be preserved 
		:returns: a new instance of the metadata


.. cpp:type:: std::shared_ptr<CMetaDataGroup> Lib3MF::PMetaDataGroup

	Shared pointer to CMetaDataGroup to easily allow reference counting.

