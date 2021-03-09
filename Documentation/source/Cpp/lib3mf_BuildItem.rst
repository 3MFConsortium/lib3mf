
CBuildItem
====================================================================================================


.. cpp:class:: Lib3MF::CBuildItem : public CBase 

	




	.. cpp:function:: PObject GetObjectResource()

		Retrieves the object resource associated to a build item

		:returns: returns the associated resource instance


	.. cpp:function:: std::string GetUUID(bool & bHasUUID)

		returns, whether a build item has a UUID and, if true, the build item's UUID

		:param bHasUUID: flag whether the build item has a UUID 
		:returns: the UUID as string of the form 'xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx'


	.. cpp:function:: void SetUUID(const std::string & sUUID)

		sets the build item's UUID

		:param sUUID: the UUID as string of the form 'xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx' 


	.. cpp:function:: Lib3MF_uint32 GetObjectResourceID()

		Retrieves the object UniqueResourceID associated to a build item

		:returns: returns the UniqueResourceID of the object


	.. cpp:function:: bool HasObjectTransform()

		Checks, if a build item has a non-identity transformation matrix

		:returns: returns true, if the transformation matrix is not the identity


	.. cpp:function:: sTransform GetObjectTransform()

		Retrieves a build item's transformation matrix.

		:returns: returns the transformation matrix


	.. cpp:function:: void SetObjectTransform(const sTransform & Transform)

		Sets a build item's transformation matrix.

		:param Transform: new transformation matrix 


	.. cpp:function:: std::string GetPartNumber()

		Retrieves a build item's part number string

		:returns: Returns a build item's part number string


	.. cpp:function:: void SetPartNumber(const std::string & sSetPartnumber)

		Sets a build item's part number string

		:param sSetPartnumber: new part number string for referencing parts from the outside world 


	.. cpp:function:: PMetaDataGroup GetMetaDataGroup()

		Returns the metadatagroup of this build item

		:returns: returns an Instance of the metadatagroup of this build item


	.. cpp:function:: sBox GetOutbox()

		Returns the outbox of a build item

		:returns: Outbox of this build item


.. cpp:type:: std::shared_ptr<CBuildItem> Lib3MF::PBuildItem

	Shared pointer to CBuildItem to easily allow reference counting.

