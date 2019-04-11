
CTexture2D
====================================================================================================


.. cpp:class:: Lib3MF::CTexture2D : public CResource 

	




	.. cpp:function:: PAttachment GetAttachment()

		Retrieves the attachment located at the path of the texture.

		:returns: attachment that holds the texture's image information.


	.. cpp:function:: void SetAttachment(CAttachment * pAttachment)

		Sets the texture's package path to the path of the attachment.

		:param pAttachment: attachment that holds the texture's image information. 


	.. cpp:function:: eTextureType GetContentType()

		Retrieves a texture's content type.

		:returns: returns content type enum.


	.. cpp:function:: void SetContentType(const eTextureType eContentType)

		Retrieves a texture's content type.

		:param eContentType: new Content Type 


	.. cpp:function:: void GetTileStyleUV(eTextureTileStyle & eTileStyleU, eTextureTileStyle & eTileStyleV)

		Retrieves a texture's tilestyle type.

		:param eTileStyleU: returns tilestyle type enum. 
		:param eTileStyleV: returns tilestyle type enum. 


	.. cpp:function:: void SetTileStyleUV(const eTextureTileStyle eTileStyleU, const eTextureTileStyle eTileStyleV)

		Sets a texture's tilestyle type.

		:param eTileStyleU: new tilestyle type enum. 
		:param eTileStyleV: new tilestyle type enum. 


	.. cpp:function:: eTextureFilter GetFilter()

		Retrieves a texture's filter type.

		:returns: returns filter type enum.


	.. cpp:function:: void SetFilter(const eTextureFilter eFilter)

		Sets a texture's filter type.

		:param eFilter: sets new filter type enum. 


.. cpp:type:: std::shared_ptr<CTexture2D> Lib3MF::PTexture2D

	Shared pointer to CTexture2D to easily allow reference counting.

