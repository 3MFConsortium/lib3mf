
CObject
====================================================================================================


.. cpp:class:: Lib3MF::CObject : public CResource 

	




	.. cpp:function:: eObjectType GetType()

		Retrieves an object's type

		:returns: returns object type enum.


	.. cpp:function:: void SetType(const eObjectType eObjectType)

		Sets an object's type

		:param eObjectType: object type enum. 


	.. cpp:function:: std::string GetName()

		Retrieves an object's name

		:returns: returns object name.


	.. cpp:function:: void SetName(const std::string & sName)

		Sets an object's name string

		:param sName: new object name. 


	.. cpp:function:: std::string GetPartNumber()

		Retrieves an object's part number

		:returns: returns object part number.


	.. cpp:function:: void SetPartNumber(const std::string & sPartNumber)

		Sets an objects partnumber string

		:param sPartNumber: new object part number. 


	.. cpp:function:: bool IsMeshObject()

		Retrieves, if an object is a mesh object

		:returns: returns, whether the object is a mesh object


	.. cpp:function:: bool IsComponentsObject()

		Retrieves, if an object is a components object

		:returns: returns, whether the object is a components object


	.. cpp:function:: bool IsValid()

		Retrieves, if the object is valid according to the core spec. For mesh objects, we distinguish between the type attribute of the object:In case of object type other, this always means false.In case of object type model or solidsupport, this means, if the mesh suffices all requirements of the core spec chapter 4.1.In case of object type support or surface, this always means true.A component objects is valid if and only if it contains at least one component and all child components are valid objects.

		:returns: returns whether the object is a valid object description


	.. cpp:function:: std::string GetUUID(bool & bHasUUID)

		Retrieves an object's uuid string (see production extension specification)

		:param bHasUUID: flag whether the build item has a UUID 
		:returns: returns object uuid.


	.. cpp:function:: void SetUUID(const std::string & sUUID)

		Sets a build object's uuid string (see production extension specification)

		:param sUUID: new object uuid string. 


	.. cpp:function:: PMetaDataGroup GetMetaDataGroup()

		Returns the metadatagroup of this object

		:returns: returns an Instance of the metadatagroup of this object


	.. cpp:function:: void SetSlicesMeshResolution(const eSlicesMeshResolution eMeshResolution)

		set the meshresolution of the mesh object

		:param eMeshResolution: meshresolution of this object 


	.. cpp:function:: eSlicesMeshResolution GetSlicesMeshResolution()

		get the meshresolution of the mesh object

		:returns: meshresolution of this object


	.. cpp:function:: bool HasSliceStack()

		returns whether the Object has a slice stack

		:returns: does the object have a slice stack


	.. cpp:function:: void ClearSliceStack()

		unlinks the attached slicestack from this object. If no slice stack is attached, do noting.



	.. cpp:function:: PSliceStack GetSliceStack()

		get the Slicestack attached to the object

		:returns: returns the slicestack instance


	.. cpp:function:: void AssignSliceStack(CSliceStack * pSliceStackInstance)

		assigns a slicestack to the object

		:param pSliceStackInstance: the new slice stack of this Object 


.. cpp:type:: std::shared_ptr<CObject> Lib3MF::PObject

	Shared pointer to CObject to easily allow reference counting.

