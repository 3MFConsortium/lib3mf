
CFunctionFromImage3D
====================================================================================================


.. cpp:class:: Lib3MF::CFunctionFromImage3D : public CFunction 

	




	.. cpp:function:: PImage3D GetImage3D()

		Returns the selected 3D image.

		:returns: image instance


	.. cpp:function:: void SetImage3D(classParam<CImage3D> pImage3D)

		Sets the 3D image of the selector.

		:param pImage3D: image instance 


	.. cpp:function:: void SetFilter(const eTextureFilter eFilter)

		Sets the texture filter of the selector.

		:param eFilter: texture filter 


	.. cpp:function:: eTextureFilter GetFilter()

		Returns the texture filter of the selector.

		:returns: texture filter


	.. cpp:function:: void SetTileStyles(const eTextureTileStyle eTileStyleU, const eTextureTileStyle eTileStyleV, const eTextureTileStyle eTileStyleW)

		Sets the tile styles of the selector.

		:param eTileStyleU: tile style in U 
		:param eTileStyleV: tile style in V 
		:param eTileStyleW: tile style in W 


	.. cpp:function:: void GetTileStyles(eTextureTileStyle & eTileStyleU, eTextureTileStyle & eTileStyleV, eTextureTileStyle & eTileStyleW)

		Retrieves the tile styles of the selector.

		:param eTileStyleU: tile style in U 
		:param eTileStyleV: tile style in V 
		:param eTileStyleW: tile style in W 


	.. cpp:function:: Lib3MF_double GetOffset()

		returns the offset value for the pixel values in the Image3D

		:returns: the offset value for the pixel values in the Image3D


	.. cpp:function:: void SetOffset(const Lib3MF_double dOffset)

		Sets the offset value for the pixel values in the Image3D

		:param dOffset: the offset value for the pixel values in the Image3D 


	.. cpp:function:: Lib3MF_double GetScale()

		returns the scale value for the pixel values in the Image3D

		:returns: the scale value for the pixel values in the Image3D


	.. cpp:function:: void SetScale(const Lib3MF_double dScale)

		Sets the scale value for the pixel values in the Image3D

		:param dScale: the scale value for the pixel values in the Image3D 


.. cpp:type:: std::shared_ptr<CFunctionFromImage3D> Lib3MF::PFunctionFromImage3D

	Shared pointer to CFunctionFromImage3D to easily allow reference counting.

