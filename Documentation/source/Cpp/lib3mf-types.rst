
Types used in the 3MF Library
==========================================================================================================


Simple types
--------------

	.. cpp:type:: uint8_t Lib3MF_uint8
	
	.. cpp:type:: uint16_t Lib3MF_uint16
	
	.. cpp:type:: uint32_t Lib3MF_uint32
	
	.. cpp:type:: uint64_t Lib3MF_uint64
	
	.. cpp:type:: int8_t Lib3MF_int8
	
	.. cpp:type:: int16_t Lib3MF_int16
	
	.. cpp:type:: int32_t Lib3MF_int32
	
	.. cpp:type:: int64_t Lib3MF_int64
	
	.. cpp:type:: float Lib3MF_single
	
	.. cpp:type:: double Lib3MF_double
	
	.. cpp:type:: Lib3MF_pvoid = void*
	
	.. cpp:type:: Lib3MFResult = Lib3MF_int32
	
	

Enumerations
--------------

	.. cpp:enum-class:: ePropertyType : Lib3MF_int32
	
		.. cpp:enumerator:: NoPropertyType = 0
		.. cpp:enumerator:: BaseMaterial = 1
		.. cpp:enumerator:: TexCoord = 2
		.. cpp:enumerator:: Colors = 3
		.. cpp:enumerator:: Composite = 4
		.. cpp:enumerator:: Multi = 5
	
	.. cpp:enum-class:: eSlicesMeshResolution : Lib3MF_int32
	
		.. cpp:enumerator:: Fullres = 0
		.. cpp:enumerator:: Lowres = 1
	
	.. cpp:enum-class:: eModelUnit : Lib3MF_int32
	
		.. cpp:enumerator:: MicroMeter = 0
		.. cpp:enumerator:: MilliMeter = 1
		.. cpp:enumerator:: CentiMeter = 2
		.. cpp:enumerator:: Inch = 3
		.. cpp:enumerator:: Foot = 4
		.. cpp:enumerator:: Meter = 5
	
	.. cpp:enum-class:: eObjectType : Lib3MF_int32
	
		.. cpp:enumerator:: Other = 0
		.. cpp:enumerator:: Model = 1
		.. cpp:enumerator:: Support = 2
		.. cpp:enumerator:: SolidSupport = 3
	
	.. cpp:enum-class:: eTextureType : Lib3MF_int32
	
		.. cpp:enumerator:: Unknown = 0
		.. cpp:enumerator:: PNG = 1
		.. cpp:enumerator:: JPEG = 2
	
	.. cpp:enum-class:: eTextureTileStyle : Lib3MF_int32
	
		.. cpp:enumerator:: Wrap = 0
		.. cpp:enumerator:: Mirror = 1
		.. cpp:enumerator:: Clamp = 2
		.. cpp:enumerator:: NoTileStyle = 3
	
	.. cpp:enum-class:: eTextureFilter : Lib3MF_int32
	
		.. cpp:enumerator:: Auto = 0
		.. cpp:enumerator:: Linear = 1
		.. cpp:enumerator:: Nearest = 2
	
	.. cpp:enum-class:: eBeamLatticeCapMode : Lib3MF_int32
	
		.. cpp:enumerator:: Sphere = 0
		.. cpp:enumerator:: HemiSphere = 1
		.. cpp:enumerator:: Butt = 2
	
	.. cpp:enum-class:: eBeamLatticeClipMode : Lib3MF_int32
	
		.. cpp:enumerator:: NoClipMode = 0
		.. cpp:enumerator:: Inside = 1
		.. cpp:enumerator:: Outside = 2
	
    .. cpp:enum-class:: eBeamLatticeBallMode : Lib3MF_int32
	
		.. cpp:enumerator:: None = 0
		.. cpp:enumerator:: Mixed = 1
		.. cpp:enumerator:: All = 2
    
	.. cpp:enum-class:: eProgressIdentifier : Lib3MF_int32
	
		.. cpp:enumerator:: QUERYCANCELED = 0
		.. cpp:enumerator:: DONE = 1
		.. cpp:enumerator:: CLEANUP = 2
		.. cpp:enumerator:: READSTREAM = 3
		.. cpp:enumerator:: EXTRACTOPCPACKAGE = 4
		.. cpp:enumerator:: READNONROOTMODELS = 5
		.. cpp:enumerator:: READROOTMODEL = 6
		.. cpp:enumerator:: READRESOURCES = 7
		.. cpp:enumerator:: READMESH = 8
		.. cpp:enumerator:: READSLICES = 9
		.. cpp:enumerator:: READBUILD = 10
		.. cpp:enumerator:: READCUSTOMATTACHMENT = 11
		.. cpp:enumerator:: READTEXTURETACHMENTS = 12
		.. cpp:enumerator:: CREATEOPCPACKAGE = 13
		.. cpp:enumerator:: WRITEMODELSTOSTREAM = 14
		.. cpp:enumerator:: WRITEROOTMODEL = 15
		.. cpp:enumerator:: WRITENONROOTMODELS = 16
		.. cpp:enumerator:: WRITEATTACHMENTS = 17
		.. cpp:enumerator:: WRITECONTENTTYPES = 18
		.. cpp:enumerator:: WRITENOBJECTS = 19
		.. cpp:enumerator:: WRITENODES = 20
		.. cpp:enumerator:: WRITETRIANGLES = 21
		.. cpp:enumerator:: WRITESLICES = 22
		.. cpp:enumerator:: WRITEKEYSTORE = 23
	
	.. cpp:enum-class:: eBlendMethod : Lib3MF_int32
	
		.. cpp:enumerator:: NoBlendMethod = 0
		.. cpp:enumerator:: Mix = 1
		.. cpp:enumerator:: Multiply = 2
	
	.. cpp:enum-class:: eEncryptionAlgorithm : Lib3MF_int32
	
		.. cpp:enumerator:: AES256_GCM = 1
	
	.. cpp:enum-class:: eWrappingAlgorithm : Lib3MF_int32
	
		.. cpp:enumerator:: RSA_OAEP = 0
	
	.. cpp:enum-class:: eMgfAlgorithm : Lib3MF_int32
	
		.. cpp:enumerator:: MGF1_SHA1 = 160
		.. cpp:enumerator:: MGF1_SHA224 = 224
		.. cpp:enumerator:: MGF1_SHA256 = 256
		.. cpp:enumerator:: MGF1_SHA384 = 384
		.. cpp:enumerator:: MGF1_SHA512 = 512
	
	.. cpp:enum-class:: eDigestMethod : Lib3MF_int32
	
		.. cpp:enumerator:: SHA1 = 160
		.. cpp:enumerator:: SHA256 = 256
	
	.. cpp:enum-class:: eCompression : Lib3MF_int32
	
		.. cpp:enumerator:: NoCompression = 0
		.. cpp:enumerator:: Deflate = 1
	

Structs
--------------

	All structs are defined as `packed`, i.e. with the
	
	.. code-block:: c
		
		#pragma pack (1)

	.. cpp:struct:: sTriangle
	
		.. cpp:member:: Lib3MF_uint32 m_Indices[3]
	

	.. cpp:struct:: sTriangleProperties
	
		.. cpp:member:: Lib3MF_uint32 m_ResourceID
	
		.. cpp:member:: Lib3MF_uint32 m_PropertyIDs[3]
	

	.. cpp:struct:: sPosition
	
		.. cpp:member:: Lib3MF_single m_Coordinates[3]
	

	.. cpp:struct:: sPosition2D
	
		.. cpp:member:: Lib3MF_single m_Coordinates[2]
	

	.. cpp:struct:: sCompositeConstituent
	
		.. cpp:member:: Lib3MF_uint32 m_PropertyID
	
		.. cpp:member:: Lib3MF_double m_MixingRatio
	

	.. cpp:struct:: sMultiPropertyLayer
	
		.. cpp:member:: Lib3MF_uint32 m_ResourceID
	
		.. cpp:member:: eBlendMethod m_TheBlendMethod
	

	.. cpp:struct:: sTex2Coord
	
		.. cpp:member:: Lib3MF_double m_U
	
		.. cpp:member:: Lib3MF_double m_V
	

	.. cpp:struct:: sTransform
	
		.. cpp:member:: Lib3MF_single m_Fields[4][3]
	

	.. cpp:struct:: sBox
	
		.. cpp:member:: Lib3MF_single m_MinCoordinate[3]
	
		.. cpp:member:: Lib3MF_single m_MaxCoordinate[3]
	

	.. cpp:struct:: sColor
	
		.. cpp:member:: Lib3MF_uint8 m_Red
	
		.. cpp:member:: Lib3MF_uint8 m_Green
	
		.. cpp:member:: Lib3MF_uint8 m_Blue
	
		.. cpp:member:: Lib3MF_uint8 m_Alpha
	

	.. cpp:struct:: sBeam
	
		.. cpp:member:: Lib3MF_uint32 m_Indices[2]
	
		.. cpp:member:: Lib3MF_double m_Radii[2]
	
		.. cpp:member:: eBeamLatticeCapMode m_CapModes[2]
	

	.. cpp:struct:: sBall
	
		.. cpp:member:: Lib3MF_uint32 m_Index
	
		.. cpp:member:: Lib3MF_double m_Radius


Function types
---------------


	.. cpp:type:: ProgressCallback = void(*)(bool*, Lib3MF_double, Lib3MF::eProgressIdentifier, Lib3MF_pvoid)
		
		A callback function
		
		:return: Returns whether the calculation should be aborted
		:param dProgressValue: The value of the progress function: values in the interval [0,1] are progress; < mean no progress update
		:param eProgressIdentifier: An identifier of progress
		:param pUserData: Userdata that is passed to the callback function
		
	.. cpp:type:: WriteCallback = void(*)(Lib3MF_uint64, Lib3MF_uint64, Lib3MF_pvoid)
		
		Callback to call for writing a data chunk
		
		:param nByteData: Pointer to the data to be written
		:param nNumBytes: Number of bytes to write
		:param pUserData: Userdata that is passed to the callback function
		
	.. cpp:type:: ReadCallback = void(*)(Lib3MF_uint64, Lib3MF_uint64, Lib3MF_pvoid)
		
		Callback to call for reading a data chunk
		
		:param nByteData: Pointer to a buffer to read data into
		:param nNumBytes: Number of bytes to read
		:param pUserData: Userdata that is passed to the callback function
		
	.. cpp:type:: SeekCallback = void(*)(Lib3MF_uint64, Lib3MF_pvoid)
		
		Callback to call for seeking in the stream
		
		:param nPosition: Position in the stream to move to
		:param pUserData: Userdata that is passed to the callback function
		
	.. cpp:type:: RandomNumberCallback = void(*)(Lib3MF_uint64, Lib3MF_uint64, Lib3MF_pvoid, Lib3MF_uint64*)
		
		Callback to generate random numbers
		
		:param nByteData: Pointer to a buffer to read data into
		:param nNumBytes: Size of available bytes in the buffer
		:param pUserData: Userdata that is passed to the callback function
		:return: Number of bytes generated when succeed. 0 or less if failed.
		
	.. cpp:type:: KeyWrappingCallback = void(*)(Lib3MF_AccessRight, Lib3MF_uint8 *, Lib3MF_uint8 **, Lib3MF_pvoid, Lib3MF_uint64*)
		
		A callback used to wrap (encrypt) the content key available in keystore resource group
		
		:param pKEKParams: The information about the parameters used used to wrap the key to the contents
		:param nInBufferBufferSize: Buffer to the input value. When encrypting, this should be the plain key. When decrypting, this should be the key cipher.
		:param nInBufferBufferSize: buffer of Buffer to the input value. When encrypting, this should be the plain key. When decrypting, this should be the key cipher.
		:param nOutBufferBufferSize: Number of elements in buffer
		:param pOutBufferNeededCount: will be filled with the count of the written elements, or needed buffer size.
		:param pOutBufferBuffer: buffer of Buffer where the data will be placed. When encrypting, this will be the key cipher. When decrypting, this will be the plain key. When buffer is null, neededBytes contains the required bytes to run.
		:param pUserData: Userdata that is passed to the callback function
		:return: The needed/encrypted/decrypted bytes when succeed or zero when error.
		
	.. cpp:type:: ContentEncryptionCallback = void(*)(Lib3MF_ContentEncryptionParams, Lib3MF_uint8 *, Lib3MF_uint8 **, Lib3MF_pvoid, Lib3MF_uint64*)
		
		A callback to encrypt/decrypt content called on each resource encrypted. This might be called several times depending on content size. If Input is not available(either null or size is 0), clients must return the result of authenticated tag generation/validation.
		
		:param pCEKParams: The params of the encryption process. Client must set/check AuthenticationTag when closing the encryption/decryption process.
		:param nInputBufferSize: Buffer to the original data. In encrypting, this will be the plain data. If decrypting, this will be the cipher data
		:param nInputBufferSize: buffer of Buffer to the original data. In encrypting, this will be the plain data. If decrypting, this will be the cipher data
		:param nOutputBufferSize: Number of elements in buffer
		:param pOutputNeededCount: will be filled with the count of the written elements, or needed buffer size.
		:param pOutputBuffer: buffer of Buffer to hold the transformed data. When encrypting, this will be the cipher data. When decrypting, this shall be the plain data. If buffer is null, neededBytes return the necessary amount of bytes.
		:param pUserData: Userdata that is passed to the callback function
		:return: The needed/encrypted/decrypted/verified bytes when succeed or zero when error.
		

	
ELib3MFException: The standard exception class of the 3MF Library
============================================================================================================================================================================================================
	
	Errors in the 3MF Library are reported as Exceptions. It is recommended to not throw these exceptions in your client code.
	
	
	.. cpp:class:: Lib3MF::ELib3MFException
	
		.. cpp:function:: void ELib3MFException::what() const noexcept
		
			 Returns error message
		
			 :return: the error message of this exception
		
	
		.. cpp:function:: Lib3MFResult ELib3MFException::getErrorCode() const noexcept
		
			 Returns error code
		
			 :return: the error code of this exception
		
	
CInputVector: Adapter for passing arrays as input for functions
===============================================================================================================================================================
	
	Several functions of the 3MF Library expect arrays of integral types or structs as input parameters.
	To not restrict the interface to, say, std::vector<type>,
	and to have a more abstract interface than a location in memory and the number of elements to input to a function
	the 3MF Library provides a templated adapter class to pass arrays as input for functions.
	
	Usually, instances of CInputVector are generated anonymously (or even implicitly) in the call to a function that expects an input array.
	
	
	.. cpp:class:: template<typename T> Lib3MF::CInputVector
	
		.. cpp:function:: CInputVector(const std::vector<T>& vec)
	
			Constructs of a CInputVector from a std::vector<T>
	
		.. cpp:function:: CInputVector(const T* in_data, size_t in_size)
	
			Constructs of a CInputVector from a memory address and a given number of elements
	
		.. cpp:function:: const T* CInputVector::data() const
	
			returns the start address of the data captured by this CInputVector
	
		.. cpp:function:: size_t CInputVector::size() const
	
			returns the number of elements captured by this CInputVector
	
 
