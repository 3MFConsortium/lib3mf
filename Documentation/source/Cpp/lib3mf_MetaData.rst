
CMetaData
====================================================================================================


.. cpp:class:: Lib3MF::CMetaData : public CBase 

	




	.. cpp:function:: std::string GetNameSpace()

		returns the namespace URL of the metadata

		:returns: the namespace URL of the metadata


	.. cpp:function:: void SetNameSpace(const std::string & sNameSpace)

		sets a new namespace URL of the metadata

		:param sNameSpace: the new namespace URL of the metadata 


	.. cpp:function:: std::string GetName()

		returns the name of a metadata

		:returns: the name of the metadata


	.. cpp:function:: void SetName(const std::string & sName)

		sets a new name of a metadata

		:param sName: the new name of the metadata 


	.. cpp:function:: std::string GetKey()

		returns the (namespace+name) of a metadata

		:returns: the key (namespace+name) of the metadata


	.. cpp:function:: bool GetMustPreserve()

		returns, whether a metadata must be preserved

		:returns: returns, whether a metadata must be preserved


	.. cpp:function:: void SetMustPreserve(const bool bMustPreserve)

		sets whether a metadata must be preserved

		:param bMustPreserve: a new value whether a metadata must be preserved 


	.. cpp:function:: std::string GetType()

		returns the type of a metadata

		:returns: the type of the metadata


	.. cpp:function:: void SetType(const std::string & sType)

		sets a new type of a metadata. This must be a simple XML type

		:param sType: a new type of the metadata 


	.. cpp:function:: std::string GetValue()

		returns the value of the metadata

		:returns: the value of the metadata


	.. cpp:function:: void SetValue(const std::string & sValue)

		sets a new value of the metadata

		:param sValue: a new value of the metadata 


.. cpp:type:: std::shared_ptr<CMetaData> Lib3MF::PMetaData

	Shared pointer to CMetaData to easily allow reference counting.

