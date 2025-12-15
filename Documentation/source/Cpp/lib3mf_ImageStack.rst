
CImageStack
====================================================================================================


.. cpp:class:: Lib3MF::CImageStack : public CImage3D 

	




	.. cpp:function:: Lib3MF_uint32 GetRowCount()

		Retrieves the number of rows in each image of this image3d

		:returns: number of rows


	.. cpp:function:: void SetRowCount(const Lib3MF_uint32 nRowCount)

		Sets the number of rows in each image of this image3d

		:param nRowCount: number of rows 


	.. cpp:function:: Lib3MF_uint32 GetColumnCount()

		Retrieves the number of columns in each image of this image3d

		:returns: number of columns


	.. cpp:function:: void SetColumnCount(const Lib3MF_uint32 nColumnCount)

		Sets the number of columns in each image of this image3d

		:param nColumnCount: number of columns 


	.. cpp:function:: Lib3MF_uint32 GetSheetCount()

		Retrieves the number of images in the stack.

		:returns: number of images


	.. cpp:function:: PAttachment GetSheet(const Lib3MF_uint32 nIndex)

		Retrieves a sheet of the stack. Raises an error if sheet is not set.

		:param nIndex: index of the image (0-based) 
		:returns: attachment containing the image


	.. cpp:function:: void SetSheet(const Lib3MF_uint32 nIndex, classParam<CAttachment> pSheet)

		Sets a sheet to an existing attachment.

		:param nIndex: index of the image (0-based) 
		:param pSheet: attachment containing the image 


	.. cpp:function:: PAttachment CreateEmptySheet(const Lib3MF_uint32 nIndex, const std::string & sPath)

		Creates a new sheet attachment with empty data.

		:param nIndex: index of the image (0-based) 
		:param sPath: path of part in the package 
		:returns: attachment containing the image


	.. cpp:function:: PAttachment CreateSheetFromBuffer(const Lib3MF_uint32 nIndex, const std::string & sPath, const CInputVector<Lib3MF_uint8> & DataBuffer)

		Creates a new sheet attachment from a memory buffer.

		:param nIndex: index of the image (0-based) 
		:param sPath: path of part in the package 
		:param DataBuffer: binary image data 
		:returns: attachment containing the image


	.. cpp:function:: PAttachment CreateSheetFromFile(const Lib3MF_uint32 nIndex, const std::string & sPath, const std::string & sFileName)

		Creates a new sheet attachment from a file on disk.

		:param nIndex: index of the image (0-based) 
		:param sPath: path of part in the package 
		:param sFileName: file name to read from 
		:returns: attachment containing the image


.. cpp:type:: std::shared_ptr<CImageStack> Lib3MF::PImageStack

	Shared pointer to CImageStack to easily allow reference counting.

