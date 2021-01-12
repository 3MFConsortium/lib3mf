
CModel
====================================================================================================


.. cpp:class:: Lib3MF::CModel : public CBase 

	




	.. cpp:function:: PPackagePart RootModelPart()

		Returns the PackagePart within the OPC package that holds the root model.

		:returns: the PackagePart within the OPC package that holds the model-file


	.. cpp:function:: PPackagePart FindOrCreatePackagePart(const std::string & sAbsolutePath)

		Returns a new PackagePart for use within the OPC package.

		:param sAbsolutePath: the absolute Path (physical location) within the OPC package 
		:returns: the new PackagePart within the OPC package


	.. cpp:function:: void SetUnit(const eModelUnit eUnit)

		sets the units of a model.

		:param eUnit: Unit enum value for the model unit 


	.. cpp:function:: eModelUnit GetUnit()

		returns the units of a model.

		:returns: Unit enum value for the model unit


	.. cpp:function:: std::string GetLanguage()

		retrieves the language of a model

		:returns: language identifier


	.. cpp:function:: void SetLanguage(const std::string & sLanguage)

		sets the language of a model

		:param sLanguage: language identifier 


	.. cpp:function:: PWriter QueryWriter(const std::string & sWriterClass)

		creates a model writer instance for a specific file type

		:param sWriterClass:  string identifier for the file type 
		:returns:  string identifier for the file type


	.. cpp:function:: PReader QueryReader(const std::string & sReaderClass)

		creates a model reader instance for a specific file type

		:param sReaderClass:  string identifier for the file type 
		:returns:  string identifier for the file type


	.. cpp:function:: PTexture2D GetTexture2DByID(const Lib3MF_uint32 nUniqueResourceID)

		finds a model texture by its UniqueResourceID

		:param nUniqueResourceID: UniqueResourceID 
		:returns: returns the texture2d instance


	.. cpp:function:: ePropertyType GetPropertyTypeByID(const Lib3MF_uint32 nUniqueResourceID)

		returns a Property's type

		:param nUniqueResourceID: Resource ID of the Property to Query 
		:returns: returns a Property's type


	.. cpp:function:: PBaseMaterialGroup GetBaseMaterialGroupByID(const Lib3MF_uint32 nUniqueResourceID)

		finds a model base material group by its UniqueResourceID

		:param nUniqueResourceID: UniqueResourceID 
		:returns: returns the BaseMaterialGroup instance


	.. cpp:function:: PTexture2DGroup GetTexture2DGroupByID(const Lib3MF_uint32 nUniqueResourceID)

		finds a model texture2d group by its UniqueResourceID

		:param nUniqueResourceID: UniqueResourceID 
		:returns: returns the Texture2DGroup instance


	.. cpp:function:: PCompositeMaterials GetCompositeMaterialsByID(const Lib3MF_uint32 nUniqueResourceID)

		finds a model CompositeMaterials by its UniqueResourceID

		:param nUniqueResourceID: UniqueResourceID 
		:returns: returns the CompositeMaterials instance


	.. cpp:function:: PMultiPropertyGroup GetMultiPropertyGroupByID(const Lib3MF_uint32 nUniqueResourceID)

		finds a model MultiPropertyGroup by its UniqueResourceID

		:param nUniqueResourceID: UniqueResourceID 
		:returns: returns the MultiPropertyGroup instance


	.. cpp:function:: PMeshObject GetMeshObjectByID(const Lib3MF_uint32 nUniqueResourceID)

		finds a mesh object by its UniqueResourceID

		:param nUniqueResourceID: UniqueResourceID 
		:returns: returns the mesh object instance


	.. cpp:function:: PComponentsObject GetComponentsObjectByID(const Lib3MF_uint32 nUniqueResourceID)

		finds a components object by its UniqueResourceID

		:param nUniqueResourceID: UniqueResourceID 
		:returns: returns the components object instance


	.. cpp:function:: PColorGroup GetColorGroupByID(const Lib3MF_uint32 nUniqueResourceID)

		finds a model color group by its UniqueResourceID

		:param nUniqueResourceID: UniqueResourceID 
		:returns: returns the ColorGroup instance


	.. cpp:function:: PSliceStack GetSliceStackByID(const Lib3MF_uint32 nUniqueResourceID)

		finds a model slicestack by its UniqueResourceID

		:param nUniqueResourceID: UniqueResourceID 
		:returns: returns the slicestack instance


	.. cpp:function:: std::string GetBuildUUID(bool & bHasUUID)

		returns, whether a build has a UUID and, if true, the build's UUID

		:param bHasUUID: flag whether the build has a UUID 
		:returns: the UUID as string of the form 'xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx'


	.. cpp:function:: void SetBuildUUID(const std::string & sUUID)

		sets the build's UUID

		:param sUUID: the UUID as string of the form 'xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx' 


	.. cpp:function:: PBuildItemIterator GetBuildItems()

		creates a build item iterator instance with all build items.

		:returns: returns the iterator instance.


	.. cpp:function:: sBox GetOutbox()

		Returns the outbox of a Model

		:returns: Outbox of this Model


	.. cpp:function:: PResourceIterator GetResources()

		creates a resource iterator instance with all resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PObjectIterator GetObjects()

		creates a resource iterator instance with all object resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PMeshObjectIterator GetMeshObjects()

		creates a resource iterator instance with all mesh object resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PComponentsObjectIterator GetComponentsObjects()

		creates a resource iterator instance with all components object resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PTexture2DIterator GetTexture2Ds()

		creates a Texture2DIterator instance with all texture2d resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PBaseMaterialGroupIterator GetBaseMaterialGroups()

		creates a BaseMaterialGroupIterator instance with all base material resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PColorGroupIterator GetColorGroups()

		creates a ColorGroupIterator instance with all ColorGroup resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PTexture2DGroupIterator GetTexture2DGroups()

		creates a Texture2DGroupIterator instance with all base material resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PCompositeMaterialsIterator GetCompositeMaterials()

		creates a CompositeMaterialsIterator instance with all CompositeMaterials resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PMultiPropertyGroupIterator GetMultiPropertyGroups()

		creates a MultiPropertyGroupsIterator instance with all MultiPropertyGroup resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PSliceStackIterator GetSliceStacks()

		creates a resource iterator instance with all slice stack resources.

		:returns: returns the iterator instance.


	.. cpp:function:: PModel MergeToModel()

		Merges all components and objects which are referenced by a build item into a mesh. The memory is duplicated and a new model is created.

		:returns: returns the merged model instance


	.. cpp:function:: PMeshObject AddMeshObject()

		adds an empty mesh object to the model.

		:returns:  returns the mesh object instance


	.. cpp:function:: PComponentsObject AddComponentsObject()

		adds an empty component object to the model.

		:returns:  returns the components object instance


	.. cpp:function:: PSliceStack AddSliceStack(const Lib3MF_double dZBottom)

		creates a new model slicestack by its id

		:param dZBottom: Bottom Z value of the slicestack 
		:returns: returns the new slicestack instance


	.. cpp:function:: PTexture2D AddTexture2DFromAttachment(CAttachment * pTextureAttachment)

		adds a texture2d resource to the model. Its path is given by that of an existing attachment.

		:param pTextureAttachment: attachment containing the image data. 
		:returns: returns the new texture instance.


	.. cpp:function:: PBaseMaterialGroup AddBaseMaterialGroup()

		adds an empty BaseMaterialGroup resource to the model.

		:returns: returns the new base material instance.


	.. cpp:function:: PColorGroup AddColorGroup()

		adds an empty ColorGroup resource to the model.

		:returns: returns the new ColorGroup instance.


	.. cpp:function:: PTexture2DGroup AddTexture2DGroup(CTexture2D * pTexture2DInstance)

		adds an empty Texture2DGroup resource to the model.

		:param pTexture2DInstance: The texture2D instance of the created Texture2DGroup. 
		:returns: returns the new Texture2DGroup instance.


	.. cpp:function:: PCompositeMaterials AddCompositeMaterials(CBaseMaterialGroup * pBaseMaterialGroupInstance)

		adds an empty CompositeMaterials resource to the model.

		:param pBaseMaterialGroupInstance: The BaseMaterialGroup instance of the created CompositeMaterials. 
		:returns: returns the new CompositeMaterials instance.


	.. cpp:function:: PMultiPropertyGroup AddMultiPropertyGroup()

		adds an empty MultiPropertyGroup resource to the model.

		:returns: returns the new MultiPropertyGroup instance.


	.. cpp:function:: PBuildItem AddBuildItem(CObject * pObject, const sTransform & Transform)

		adds a build item to the model.

		:param pObject: Object instance. 
		:param Transform: Transformation matrix. 
		:returns: returns the build item instance.


	.. cpp:function:: void RemoveBuildItem(CBuildItem * pBuildItemInstance)

		removes a build item from the model

		:param pBuildItemInstance: Build item to remove. 


	.. cpp:function:: PMetaDataGroup GetMetaDataGroup()

		Returns the metadata of the model as MetaDataGroup

		:returns: returns an Instance of the metadatagroup of the model


	.. cpp:function:: PAttachment AddAttachment(const std::string & sURI, const std::string & sRelationShipType)

		adds an attachment stream to the model. The OPC part will be related to the model stream with a certain relationship type.

		:param sURI: Path of the attachment 
		:param sRelationShipType: Relationship type of the attachment 
		:returns: Instance of the attachment object


	.. cpp:function:: void RemoveAttachment(CAttachment * pAttachmentInstance)

		Removes attachment from the model.

		:param pAttachmentInstance: Attachment instance to remove 


	.. cpp:function:: PAttachment GetAttachment(const Lib3MF_uint32 nIndex)

		retrieves an attachment stream object from the model..

		:param nIndex: Index of the attachment stream 
		:returns: Instance of the attachment object


	.. cpp:function:: PAttachment FindAttachment(const std::string & sURI)

		retrieves an attachment stream object from the model.

		:param sURI: Path URI in the package 
		:returns: Instance of the attachment object


	.. cpp:function:: Lib3MF_uint32 GetAttachmentCount()

		retrieves the number of attachments of the model.

		:returns: Returns the number of attachments.


	.. cpp:function:: bool HasPackageThumbnailAttachment()

		Retrieve whether the OPC package contains a package thumbnail.

		:returns: returns whether the OPC package contains a package thumbnail


	.. cpp:function:: PAttachment CreatePackageThumbnailAttachment()

		Create a new or the existing package thumbnail for the OPC package.

		:returns: Instance of a new or the existing thumbnailattachment object.


	.. cpp:function:: PAttachment GetPackageThumbnailAttachment()

		Get the attachment to the OPC package containing the package thumbnail.

		:returns: Instance of the thumbnailattachment object or NULL.


	.. cpp:function:: void RemovePackageThumbnailAttachment()

		Remove the attachment to the OPC package containing the package thumbnail.



	.. cpp:function:: void AddCustomContentType(const std::string & sExtension, const std::string & sContentType)

		Adds a new Content Type to the model.

		:param sExtension: File Extension 
		:param sContentType: Content Type Identifier 


	.. cpp:function:: void RemoveCustomContentType(const std::string & sExtension)

		Removes a custom Content Type from the model (UTF8 version).

		:param sExtension: File Extension 


	.. cpp:function:: void SetRandomNumberCallback(const RandomNumberCallback pTheCallback, const Lib3MF_pvoid pUserData)

		Sets the random number generator callback for use in the library

		:param pTheCallback: The callback used to generate random numbers 
		:param pUserData: Userdata to be passed to the callback function 


	.. cpp:function:: PKeyStore GetKeyStore()

		Gets the keystore associated with this model

		:returns: The package keystore


.. cpp:type:: std::shared_ptr<CModel> Lib3MF::PModel

	Shared pointer to CModel to easily allow reference counting.

