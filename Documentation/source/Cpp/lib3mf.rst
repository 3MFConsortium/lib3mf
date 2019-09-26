
The wrapper class CWrapper
===================================================================================


.. cpp:class:: Lib3MF::CWrapper

	All types of the 3MF Library reside in the namespace Lib3MF and all
	functionality of the 3MF Library resides in Lib3MF::CWrapper.

	A suitable way to use Lib3MF::CWrapper is as a singleton.

	.. cpp:function:: void GetLibraryVersion(Lib3MF_uint32 & nMajor, Lib3MF_uint32 & nMinor, Lib3MF_uint32 & nMicro)
	
		retrieves the binary version of this library.
	
		:param nMajor: returns the major version of this library 
		:param nMinor: returns the minor version of this library 
		:param nMicro: returns the micro version of this library 

	
	.. cpp:function:: bool GetPrereleaseInformation(std::string & sPrereleaseInfo)
	
		retrieves prerelease information of this library.
	
		:returns: Does the library provide prerelease version?
		:param sPrereleaseInfo: retrieves prerelease information of this library. 

	
	.. cpp:function:: bool GetBuildInformation(std::string & sBuildInformation)
	
		retrieves build information of this library.
	
		:returns: Does the library provide build version?
		:param sBuildInformation: retrieves build information of this library. 

	
	.. cpp:function:: void GetSpecificationVersion(const std::string & sSpecificationURL, bool & bIsSupported, Lib3MF_uint32 & nMajor, Lib3MF_uint32 & nMinor, Lib3MF_uint32 & nMicro)
	
		retrieves whether a specification is supported, and if so, which version.
	
		:param sSpecificationURL: URL of extension to check 
		:param bIsSupported: returns whether this specification is supported 
		:param nMajor: returns the major version of the extension (if IsSupported) 
		:param nMinor: returns the minor version of the extension (if IsSupported) 
		:param nMicro: returns the micro version of the extension (if IsSupported) 

	
	.. cpp:function:: PModel CreateModel()
	
		creates an empty model instance.
	
		:returns: returns an empty model instance

	
	.. cpp:function:: void Release(CBase * pInstance)
	
		releases shared ownership of an object instance
	
		:param pInstance: the object instance to release 

	
	.. cpp:function:: void Acquire(CBase * pInstance)
	
		acquires shared ownership of an object instance
	
		:param pInstance: the object instance to acquire 

	
	.. cpp:function:: void SetJournal(const std::string & sJournalPath)
	
		Sets the journal file path
	
		:param sJournalPath: File name of the journal file 

	
	.. cpp:function:: bool GetLastError(CBase * pInstance, std::string & sLastErrorString)
	
		Retrieves the last error string of an instance
	
		:param pInstance: Object where the error occured. 
		:param sLastErrorString: Last Error String 
		:returns: Returns if the instance has a last error.

	
	.. cpp:function:: void RetrieveProgressMessage(const eProgressIdentifier eTheProgressIdentifier, std::string & sProgressMessage)
	
		Return an English text for a progress identifier.|Note: this is the only function you can call from your callback function.
	
		:param eTheProgressIdentifier: the progress identifier that is passed to the callback function 
		:param sProgressMessage: English text for the progress identifier 

	
	.. cpp:function:: sColor RGBAToColor(const Lib3MF_uint8 nRed, const Lib3MF_uint8 nGreen, const Lib3MF_uint8 nBlue, const Lib3MF_uint8 nAlpha)
	
		Creates a Color from uint8 RGBA values
	
		:param nRed: Red value of color (0-255) 
		:param nGreen: Green value of color (0-255) 
		:param nBlue: Blue value of color (0-255) 
		:param nAlpha: Alpha value of color (0-255) 
		:returns: Assembled color

	
	.. cpp:function:: sColor FloatRGBAToColor(const Lib3MF_single fRed, const Lib3MF_single fGreen, const Lib3MF_single fBlue, const Lib3MF_single fAlpha)
	
		Creates a Color from uint8 RGBA values
	
		:param fRed: Red value of color (0-1) 
		:param fGreen: Green value of color (0-1) 
		:param fBlue: Blue value of color (0-1) 
		:param fAlpha: Alpha value of color (0-1) 
		:returns: Assembled color

	
	.. cpp:function:: void ColorToRGBA(const sColor & TheColor, Lib3MF_uint8 & nRed, Lib3MF_uint8 & nGreen, Lib3MF_uint8 & nBlue, Lib3MF_uint8 & nAlpha)
	
		Calculates uint8-RGBA-values from a Color
	
		:param TheColor: Color to handle 
		:param nRed: Red value of color (0-255) 
		:param nGreen: Green value of color (0-255) 
		:param nBlue: Blue value of color (0-255) 
		:param nAlpha: Alpha value of color (0-255) 

	
	.. cpp:function:: void ColorToFloatRGBA(const sColor & TheColor, Lib3MF_single & fRed, Lib3MF_single & fGreen, Lib3MF_single & fBlue, Lib3MF_single & fAlpha)
	
		Calculates float-RGBA-values from a Color
	
		:param TheColor: Color to handle 
		:param fRed: Red value of color (0-1) 
		:param fGreen: Green value of color (0-1) 
		:param fBlue: Blue value of color (0-1) 
		:param fAlpha: Alpha value of color (0-1) 

	
	.. cpp:function:: sTransform GetIdentityTransform()
	
		Creates an identity transform
	
		:returns: Transformation matrix.

	
	.. cpp:function:: sTransform GetUniformScaleTransform(const Lib3MF_single fFactor)
	
		Creates a uniform scale transform
	
		:param fFactor: Factor in X, Y and Z 
		:returns: Transformation matrix.

	
	.. cpp:function:: sTransform GetScaleTransform(const Lib3MF_single fFactorX, const Lib3MF_single fFactorY, const Lib3MF_single fFactorZ)
	
		Creates a scale transform
	
		:param fFactorX: Factor in X 
		:param fFactorY: Factor in Y 
		:param fFactorZ: Factor in Z 
		:returns: Transformation matrix.

	
	.. cpp:function:: sTransform GetTranslationTransform(const Lib3MF_single fVectorX, const Lib3MF_single fVectorY, const Lib3MF_single fVectorZ)
	
		Creates an translation transform
	
		:param fVectorX: Translation in X 
		:param fVectorY: Translation in Y 
		:param fVectorZ: Translation in Z 
		:returns: Transformation matrix.

	
.. cpp:type:: std::shared_ptr<CWrapper> Lib3MF::PWrapper
	
