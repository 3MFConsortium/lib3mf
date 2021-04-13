using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Lib3MF {

	public enum ePropertyType {
		NoPropertyType = 0,
		BaseMaterial = 1,
		TexCoord = 2,
		Colors = 3,
		Composite = 4,
		Multi = 5
	};

	public enum eSlicesMeshResolution {
		Fullres = 0,
		Lowres = 1
	};

	public enum eModelUnit {
		MicroMeter = 0,
		MilliMeter = 1,
		CentiMeter = 2,
		Inch = 3,
		Foot = 4,
		Meter = 5
	};

	public enum eObjectType {
		Other = 0,
		Model = 1,
		Support = 2,
		SolidSupport = 3
	};

	public enum eTextureType {
		Unknown = 0,
		PNG = 1,
		JPEG = 2
	};

	public enum eTextureTileStyle {
		Wrap = 0,
		Mirror = 1,
		Clamp = 2,
		NoTileStyle = 3
	};

	public enum eTextureFilter {
		Auto = 0,
		Linear = 1,
		Nearest = 2
	};

	public enum eBeamLatticeCapMode {
		Sphere = 0,
		HemiSphere = 1,
		Butt = 2
	};

	public enum eBeamLatticeClipMode {
		NoClipMode = 0,
		Inside = 1,
		Outside = 2
	};

	public enum eBeamLatticeBallMode {
		None = 0,
		Mixed = 1,
		All = 2
	};

	public enum eProgressIdentifier {
		QUERYCANCELED = 0,
		DONE = 1,
		CLEANUP = 2,
		READSTREAM = 3,
		EXTRACTOPCPACKAGE = 4,
		READNONROOTMODELS = 5,
		READROOTMODEL = 6,
		READRESOURCES = 7,
		READMESH = 8,
		READSLICES = 9,
		READBUILD = 10,
		READCUSTOMATTACHMENT = 11,
		READTEXTURETACHMENTS = 12,
		CREATEOPCPACKAGE = 13,
		WRITEMODELSTOSTREAM = 14,
		WRITEROOTMODEL = 15,
		WRITENONROOTMODELS = 16,
		WRITEATTACHMENTS = 17,
		WRITECONTENTTYPES = 18,
		WRITENOBJECTS = 19,
		WRITENODES = 20,
		WRITETRIANGLES = 21,
		WRITESLICES = 22,
		WRITEKEYSTORE = 23
	};

	public enum eBlendMethod {
		Mix = 0,
		Multiply = 1,
		Mask = 2
	};

	public enum eColorChannel {
		Red = 0,
		Green = 1,
		Blue = 2
	};

	public enum eEncryptionAlgorithm {
		AES256_GCM = 1
	};

	public enum eWrappingAlgorithm {
		RSA_OAEP = 0
	};

	public enum eMgfAlgorithm {
		MGF1_SHA1 = 160,
		MGF1_SHA224 = 224,
		MGF1_SHA256 = 256,
		MGF1_SHA384 = 384,
		MGF1_SHA512 = 512
	};

	public enum eDigestMethod {
		SHA1 = 160,
		SHA256 = 256
	};

	public enum eCompression {
		NoCompression = 0,
		Deflate = 1
	};

	public struct sTriangle
	{
		public UInt32[] Indices;
	}

	public struct sTriangleProperties
	{
		public UInt32 ResourceID;
		public UInt32[] PropertyIDs;
	}

	public struct sPosition
	{
		public Single[] Coordinates;
	}

	public struct sPosition2D
	{
		public Single[] Coordinates;
	}

	public struct sCompositeConstituent
	{
		public UInt32 PropertyID;
		public Double MixingRatio;
	}

	public struct sMultiPropertyLayer
	{
		public UInt32 ResourceID;
		public eBlendMethod TheBlendMethod;
	}

	public struct sTex2Coord
	{
		public Double U;
		public Double V;
	}

	public struct sTransform
	{
		public Single[][] Fields;
	}

	public struct sBox
	{
		public Single[] MinCoordinate;
		public Single[] MaxCoordinate;
	}

	public struct sColor
	{
		public Byte Red;
		public Byte Green;
		public Byte Blue;
		public Byte Alpha;
	}

	public struct sBeam
	{
		public UInt32[] Indices;
		public Double[] Radii;
		public eBeamLatticeCapMode[] CapModes;
	}

	public struct sBall
	{
		public UInt32 Index;
		public Double Radius;
	}


	namespace Internal {

		[StructLayout(LayoutKind.Explicit, Size=12)]
		public unsafe struct InternalTriangle
		{
			[FieldOffset(0)] public fixed UInt32 Indices[3];
		}

		[StructLayout(LayoutKind.Explicit, Size=16)]
		public unsafe struct InternalTriangleProperties
		{
			[FieldOffset(0)] public UInt32 ResourceID;
			[FieldOffset(4)] public fixed UInt32 PropertyIDs[3];
		}

		[StructLayout(LayoutKind.Explicit, Size=12)]
		public unsafe struct InternalPosition
		{
			[FieldOffset(0)] public fixed Single Coordinates[3];
		}

		[StructLayout(LayoutKind.Explicit, Size=8)]
		public unsafe struct InternalPosition2D
		{
			[FieldOffset(0)] public fixed Single Coordinates[2];
		}

		[StructLayout(LayoutKind.Explicit, Size=12)]
		public unsafe struct InternalCompositeConstituent
		{
			[FieldOffset(0)] public UInt32 PropertyID;
			[FieldOffset(4)] public Double MixingRatio;
		}

		[StructLayout(LayoutKind.Explicit, Size=8)]
		public unsafe struct InternalMultiPropertyLayer
		{
			[FieldOffset(0)] public UInt32 ResourceID;
			[FieldOffset(4)] public Int32 TheBlendMethod;
		}

		[StructLayout(LayoutKind.Explicit, Size=16)]
		public unsafe struct InternalTex2Coord
		{
			[FieldOffset(0)] public Double U;
			[FieldOffset(8)] public Double V;
		}

		[StructLayout(LayoutKind.Explicit, Size=48)]
		public unsafe struct InternalTransform
		{
			[FieldOffset(0)] public fixed Single Fields[12];
		}

		[StructLayout(LayoutKind.Explicit, Size=24)]
		public unsafe struct InternalBox
		{
			[FieldOffset(0)] public fixed Single MinCoordinate[3];
			[FieldOffset(12)] public fixed Single MaxCoordinate[3];
		}

		[StructLayout(LayoutKind.Explicit, Size=4)]
		public unsafe struct InternalColor
		{
			[FieldOffset(0)] public Byte Red;
			[FieldOffset(1)] public Byte Green;
			[FieldOffset(2)] public Byte Blue;
			[FieldOffset(3)] public Byte Alpha;
		}

		[StructLayout(LayoutKind.Explicit, Size=32)]
		public unsafe struct InternalBeam
		{
			[FieldOffset(0)] public fixed UInt32 Indices[2];
			[FieldOffset(8)] public fixed Double Radii[2];
			[FieldOffset(24)] public fixed Int32 CapModes[2];
		}

		[StructLayout(LayoutKind.Explicit, Size=12)]
		public unsafe struct InternalBall
		{
			[FieldOffset(0)] public UInt32 Index;
			[FieldOffset(4)] public Double Radius;
		}


		public class Lib3MFWrapper
		{
			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_writetofile", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_WriteToFile (IntPtr Handle, byte[] AFilename);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_getstreamsize", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_GetStreamSize (IntPtr Handle, out UInt64 AStreamSize);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_writetobuffer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_WriteToBuffer (IntPtr Handle, UInt64 sizeBuffer, out UInt64 neededBuffer, IntPtr dataBuffer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_writetocallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_WriteToCallback (IntPtr Handle, IntPtr ATheWriteCallback, IntPtr ATheSeekCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_setprogresscallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_SetProgressCallback (IntPtr Handle, IntPtr AProgressCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_getdecimalprecision", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_GetDecimalPrecision (IntPtr Handle, out UInt32 ADecimalPrecision);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_setdecimalprecision", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_SetDecimalPrecision (IntPtr Handle, UInt32 ADecimalPrecision);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_setstrictmodeactive", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_SetStrictModeActive (IntPtr Handle, Byte AStrictModeActive);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_getstrictmodeactive", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_GetStrictModeActive (IntPtr Handle, out Byte AStrictModeActive);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_getwarning", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_GetWarning (IntPtr Handle, UInt32 AIndex, out UInt32 AErrorCode, UInt32 sizeWarning, out UInt32 neededWarning, IntPtr dataWarning);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_getwarningcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_GetWarningCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_addkeywrappingcallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_AddKeyWrappingCallback (IntPtr Handle, byte[] AConsumerID, IntPtr ATheCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_writer_setcontentencryptioncallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Writer_SetContentEncryptionCallback (IntPtr Handle, IntPtr ATheCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_readfromfile", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_ReadFromFile (IntPtr Handle, byte[] AFilename);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_readfrombuffer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_ReadFromBuffer (IntPtr Handle, UInt64 sizeBuffer, IntPtr dataBuffer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_readfromcallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_ReadFromCallback (IntPtr Handle, IntPtr ATheReadCallback, UInt64 AStreamSize, IntPtr ATheSeekCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_setprogresscallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_SetProgressCallback (IntPtr Handle, IntPtr AProgressCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_addrelationtoread", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_AddRelationToRead (IntPtr Handle, byte[] ARelationShipType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_removerelationtoread", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_RemoveRelationToRead (IntPtr Handle, byte[] ARelationShipType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_setstrictmodeactive", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_SetStrictModeActive (IntPtr Handle, Byte AStrictModeActive);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_getstrictmodeactive", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_GetStrictModeActive (IntPtr Handle, out Byte AStrictModeActive);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_getwarning", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_GetWarning (IntPtr Handle, UInt32 AIndex, out UInt32 AErrorCode, UInt32 sizeWarning, out UInt32 neededWarning, IntPtr dataWarning);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_getwarningcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_GetWarningCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_addkeywrappingcallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_AddKeyWrappingCallback (IntPtr Handle, byte[] AConsumerID, IntPtr ATheCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_reader_setcontentencryptioncallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Reader_SetContentEncryptionCallback (IntPtr Handle, IntPtr ATheCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_packagepart_getpath", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 PackagePart_GetPath (IntPtr Handle, UInt32 sizePath, out UInt32 neededPath, IntPtr dataPath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_packagepart_setpath", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 PackagePart_SetPath (IntPtr Handle, byte[] APath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resource_getresourceid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Resource_GetResourceID (IntPtr Handle, out UInt32 AUniqueResourceID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resource_getuniqueresourceid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Resource_GetUniqueResourceID (IntPtr Handle, out UInt32 AUniqueResourceID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resource_packagepart", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Resource_PackagePart (IntPtr Handle, out IntPtr APackagePart);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resource_setpackagepart", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Resource_SetPackagePart (IntPtr Handle, IntPtr APackagePart);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resource_getmodelresourceid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Resource_GetModelResourceID (IntPtr Handle, out UInt32 AModelResourceId);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourceiterator_movenext", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceIterator_MoveNext (IntPtr Handle, out Byte AHasNext);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourceiterator_moveprevious", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceIterator_MovePrevious (IntPtr Handle, out Byte AHasPrevious);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourceiterator_getcurrent", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceIterator_GetCurrent (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourceiterator_clone", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceIterator_Clone (IntPtr Handle, out IntPtr AOutResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourceiterator_count", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceIterator_Count (IntPtr Handle, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestackiterator_getcurrentslicestack", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStackIterator_GetCurrentSliceStack (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_objectiterator_getcurrentobject", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ObjectIterator_GetCurrentObject (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobjectiterator_getcurrentmeshobject", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObjectIterator_GetCurrentMeshObject (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_componentsobjectiterator_getcurrentcomponentsobject", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ComponentsObjectIterator_GetCurrentComponentsObject (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2diterator_getcurrenttexture2d", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2DIterator_GetCurrentTexture2D (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_basematerialgroupiterator_getcurrentbasematerialgroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BaseMaterialGroupIterator_GetCurrentBaseMaterialGroup (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_colorgroupiterator_getcurrentcolorgroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ColorGroupIterator_GetCurrentColorGroup (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2dgroupiterator_getcurrenttexture2dgroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2DGroupIterator_GetCurrentTexture2DGroup (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_compositematerialsiterator_getcurrentcompositematerials", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 CompositeMaterialsIterator_GetCurrentCompositeMaterials (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroupiterator_getcurrentmultipropertygroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroupIterator_GetCurrentMultiPropertyGroup (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3diterator_getcurrentimage3d", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DIterator_GetCurrentImage3D (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstackiterator_getcurrentvolumetricstack", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStackIterator_GetCurrentVolumetricStack (IntPtr Handle, out IntPtr AResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_getnamespace", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_GetNameSpace (IntPtr Handle, UInt32 sizeNameSpace, out UInt32 neededNameSpace, IntPtr dataNameSpace);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_setnamespace", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_SetNameSpace (IntPtr Handle, byte[] ANameSpace);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_getname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_GetName (IntPtr Handle, UInt32 sizeName, out UInt32 neededName, IntPtr dataName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_setname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_SetName (IntPtr Handle, byte[] AName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_getkey", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_GetKey (IntPtr Handle, UInt32 sizeKey, out UInt32 neededKey, IntPtr dataKey);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_getmustpreserve", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_GetMustPreserve (IntPtr Handle, out Byte AMustPreserve);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_setmustpreserve", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_SetMustPreserve (IntPtr Handle, Byte AMustPreserve);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_gettype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_GetType (IntPtr Handle, UInt32 sizeType, out UInt32 neededType, IntPtr dataType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_settype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_SetType (IntPtr Handle, byte[] AType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_getvalue", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_GetValue (IntPtr Handle, UInt32 sizeValue, out UInt32 neededValue, IntPtr dataValue);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadata_setvalue", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaData_SetValue (IntPtr Handle, byte[] AValue);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadatagroup_getmetadatacount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaDataGroup_GetMetaDataCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadatagroup_getmetadata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaDataGroup_GetMetaData (IntPtr Handle, UInt32 AIndex, out IntPtr AMetaData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadatagroup_getmetadatabykey", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaDataGroup_GetMetaDataByKey (IntPtr Handle, byte[] ANameSpace, byte[] AName, out IntPtr AMetaData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadatagroup_removemetadatabyindex", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaDataGroup_RemoveMetaDataByIndex (IntPtr Handle, UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadatagroup_removemetadata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaDataGroup_RemoveMetaData (IntPtr Handle, IntPtr ATheMetaData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_metadatagroup_addmetadata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MetaDataGroup_AddMetaData (IntPtr Handle, byte[] ANameSpace, byte[] AName, byte[] AValue, byte[] AType, Byte AMustPreserve, out IntPtr AMetaData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_gettype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_GetType (IntPtr Handle, out Int32 AObjectType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_settype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_SetType (IntPtr Handle, Int32 AObjectType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_getname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_GetName (IntPtr Handle, UInt32 sizeName, out UInt32 neededName, IntPtr dataName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_setname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_SetName (IntPtr Handle, byte[] AName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_getpartnumber", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_GetPartNumber (IntPtr Handle, UInt32 sizePartNumber, out UInt32 neededPartNumber, IntPtr dataPartNumber);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_setpartnumber", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_SetPartNumber (IntPtr Handle, byte[] APartNumber);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_ismeshobject", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_IsMeshObject (IntPtr Handle, out Byte AIsMeshObject);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_iscomponentsobject", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_IsComponentsObject (IntPtr Handle, out Byte AIsComponentsObject);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_isvalid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_IsValid (IntPtr Handle, out Byte AIsValid);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_setattachmentasthumbnail", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_SetAttachmentAsThumbnail (IntPtr Handle, IntPtr AAttachment);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_getthumbnailattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_GetThumbnailAttachment (IntPtr Handle, out IntPtr AAttachment);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_clearthumbnailattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_ClearThumbnailAttachment (IntPtr Handle);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_getoutbox", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_GetOutbox (IntPtr Handle, out InternalBox AOutbox);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_getuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_GetUUID (IntPtr Handle, out Byte AHasUUID, UInt32 sizeUUID, out UInt32 neededUUID, IntPtr dataUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_setuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_SetUUID (IntPtr Handle, byte[] AUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_getmetadatagroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_GetMetaDataGroup (IntPtr Handle, out IntPtr AMetaDataGroup);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_setslicesmeshresolution", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_SetSlicesMeshResolution (IntPtr Handle, Int32 AMeshResolution);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_getslicesmeshresolution", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_GetSlicesMeshResolution (IntPtr Handle, out Int32 AMeshResolution);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_hasslices", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_HasSlices (IntPtr Handle, Byte ARecursive, out Byte AHasSlices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_clearslicestack", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_ClearSliceStack (IntPtr Handle);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_getslicestack", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_GetSliceStack (IntPtr Handle, out IntPtr ASliceStackInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_object_assignslicestack", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Object_AssignSliceStack (IntPtr Handle, IntPtr ASliceStackInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_getvertexcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_GetVertexCount (IntPtr Handle, out UInt32 AVertexCount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_gettrianglecount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_GetTriangleCount (IntPtr Handle, out UInt32 AVertexCount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_getvertex", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_GetVertex (IntPtr Handle, UInt32 AIndex, out InternalPosition ACoordinates);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_setvertex", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_SetVertex (IntPtr Handle, UInt32 AIndex, InternalPosition ACoordinates);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_addvertex", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_AddVertex (IntPtr Handle, InternalPosition ACoordinates, out UInt32 ANewIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_getvertices", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_GetVertices (IntPtr Handle, UInt64 sizeVertices, out UInt64 neededVertices, IntPtr dataVertices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_gettriangle", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_GetTriangle (IntPtr Handle, UInt32 AIndex, out InternalTriangle AIndices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_settriangle", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_SetTriangle (IntPtr Handle, UInt32 AIndex, InternalTriangle AIndices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_addtriangle", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_AddTriangle (IntPtr Handle, InternalTriangle AIndices, out UInt32 ANewIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_gettriangleindices", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_GetTriangleIndices (IntPtr Handle, UInt64 sizeIndices, out UInt64 neededIndices, IntPtr dataIndices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_setobjectlevelproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_SetObjectLevelProperty (IntPtr Handle, UInt32 AUniqueResourceID, UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_getobjectlevelproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_GetObjectLevelProperty (IntPtr Handle, out UInt32 AUniqueResourceID, out UInt32 APropertyID, out Byte AHasObjectLevelProperty);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_settriangleproperties", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_SetTriangleProperties (IntPtr Handle, UInt32 AIndex, InternalTriangleProperties AProperties);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_gettriangleproperties", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_GetTriangleProperties (IntPtr Handle, UInt32 AIndex, out InternalTriangleProperties AProperty);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_setalltriangleproperties", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_SetAllTriangleProperties (IntPtr Handle, UInt64 sizePropertiesArray, IntPtr dataPropertiesArray);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_getalltriangleproperties", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_GetAllTriangleProperties (IntPtr Handle, UInt64 sizePropertiesArray, out UInt64 neededPropertiesArray, IntPtr dataPropertiesArray);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_clearallproperties", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_ClearAllProperties (IntPtr Handle);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_setgeometry", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_SetGeometry (IntPtr Handle, UInt64 sizeVertices, IntPtr dataVertices, UInt64 sizeIndices, IntPtr dataIndices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_ismanifoldandoriented", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_IsManifoldAndOriented (IntPtr Handle, out Byte AIsManifoldAndOriented);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_beamlattice", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_BeamLattice (IntPtr Handle, out IntPtr ATheBeamLattice);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_meshobject_volumedata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MeshObject_VolumeData (IntPtr Handle, out IntPtr ATheVolumeData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getminlength", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetMinLength (IntPtr Handle, out Double AMinLength);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_setminlength", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_SetMinLength (IntPtr Handle, Double AMinLength);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getclipping", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetClipping (IntPtr Handle, out Int32 AClipMode, out UInt32 AUniqueResourceID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_setclipping", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_SetClipping (IntPtr Handle, Int32 AClipMode, UInt32 AUniqueResourceID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getrepresentation", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetRepresentation (IntPtr Handle, out Byte AHasRepresentation, out UInt32 AUniqueResourceID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_setrepresentation", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_SetRepresentation (IntPtr Handle, UInt32 AUniqueResourceID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getballoptions", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetBallOptions (IntPtr Handle, out Int32 ABallMode, out Double ABallRadius);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_setballoptions", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_SetBallOptions (IntPtr Handle, Int32 ABallMode, Double ABallRadius);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getbeamcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetBeamCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getbeam", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetBeam (IntPtr Handle, UInt32 AIndex, out InternalBeam ABeamInfo);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_addbeam", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_AddBeam (IntPtr Handle, InternalBeam ABeamInfo, out UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_setbeam", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_SetBeam (IntPtr Handle, UInt32 AIndex, InternalBeam ABeamInfo);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_setbeams", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_SetBeams (IntPtr Handle, UInt64 sizeBeamInfo, IntPtr dataBeamInfo);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getbeams", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetBeams (IntPtr Handle, UInt64 sizeBeamInfo, out UInt64 neededBeamInfo, IntPtr dataBeamInfo);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getballcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetBallCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getball", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetBall (IntPtr Handle, UInt32 AIndex, out InternalBall ABallInfo);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_addball", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_AddBall (IntPtr Handle, InternalBall ABallInfo, out UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_setball", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_SetBall (IntPtr Handle, UInt32 AIndex, InternalBall ABallInfo);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_setballs", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_SetBalls (IntPtr Handle, UInt64 sizeBallInfo, IntPtr dataBallInfo);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getballs", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetBalls (IntPtr Handle, UInt64 sizeBallInfo, out UInt64 neededBallInfo, IntPtr dataBallInfo);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getbeamsetcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetBeamSetCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_addbeamset", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_AddBeamSet (IntPtr Handle, out IntPtr ABeamSet);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamlattice_getbeamset", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamLattice_GetBeamSet (IntPtr Handle, UInt32 AIndex, out IntPtr ABeamSet);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataitem_getvolumetricstack", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataItem_GetVolumetricStack (IntPtr Handle, out IntPtr ATheVolumetricStack);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataitem_setvolumetricstack", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataItem_SetVolumetricStack (IntPtr Handle, IntPtr ATheVolumetricStack);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataitem_gettransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataItem_GetTransform (IntPtr Handle, out InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataitem_settransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataItem_SetTransform (IntPtr Handle, InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatalevelset_getsolidthreshold", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataLevelset_GetSolidThreshold (IntPtr Handle, out Double ATheSolidThreshold);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatalevelset_setsolidthreshold", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataLevelset_SetSolidThreshold (IntPtr Handle, Double ATheSolidThreshold);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatalevelset_setchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataLevelset_SetChannel (IntPtr Handle, byte[] AChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatalevelset_getchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataLevelset_GetChannel (IntPtr Handle, UInt32 sizeChannelName, out UInt32 neededChannelName, IntPtr dataChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatacolor_setchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataColor_SetChannel (IntPtr Handle, Int32 ATheColorChannel, byte[] AChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatacolor_getchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataColor_GetChannel (IntPtr Handle, Int32 ATheColorChannel, UInt32 sizeChannelName, out UInt32 neededChannelName, IntPtr dataChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatacomposite_getbasematerialgroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataComposite_GetBaseMaterialGroup (IntPtr Handle, out IntPtr ABaseMaterialGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatacomposite_setbasematerialgroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataComposite_SetBaseMaterialGroup (IntPtr Handle, IntPtr ABaseMaterialGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatacomposite_getmaterialmappingcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataComposite_GetMaterialMappingCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatacomposite_getmaterialmapping", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataComposite_GetMaterialMapping (IntPtr Handle, UInt32 AIndex, out UInt32 APropertyID, UInt32 sizeChannelName, out UInt32 neededChannelName, IntPtr dataChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatacomposite_setmaterialmapping", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataComposite_SetMaterialMapping (IntPtr Handle, UInt32 AIndex, out UInt32 APropertyID, UInt32 sizeChannelName, out UInt32 neededChannelName, IntPtr dataChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatacomposite_addmaterialmapping", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataComposite_AddMaterialMapping (IntPtr Handle, UInt32 APropertyID, byte[] AChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedatacomposite_removematerialmapping", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataComposite_RemoveMaterialMapping (IntPtr Handle, UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataproperty_setchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataProperty_SetChannel (IntPtr Handle, byte[] AChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataproperty_getchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataProperty_GetChannel (IntPtr Handle, UInt32 sizeChannelName, out UInt32 neededChannelName, IntPtr dataChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataproperty_setname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataProperty_SetName (IntPtr Handle, byte[] APropertyName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataproperty_getname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataProperty_GetName (IntPtr Handle, UInt32 sizePropertyName, out UInt32 neededPropertyName, IntPtr dataPropertyName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataproperty_setisrequired", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataProperty_SetIsRequired (IntPtr Handle, Byte AIsRequired);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedataproperty_isrequired", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeDataProperty_IsRequired (IntPtr Handle, out Byte AIsRequired);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_getlevelset", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_GetLevelset (IntPtr Handle, out IntPtr ATheLevelsetData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_createnewlevelset", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_CreateNewLevelset (IntPtr Handle, IntPtr ATheVolumetricStack, out IntPtr ATheLevelsetData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_getcomposite", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_GetComposite (IntPtr Handle, out IntPtr ATheCompositeData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_createnewcomposite", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_CreateNewComposite (IntPtr Handle, IntPtr ATheVolumetricStack, out IntPtr ATheCompositeData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_getcolor", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_GetColor (IntPtr Handle, out IntPtr ATheColorData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_createnewcolor", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_CreateNewColor (IntPtr Handle, IntPtr ATheVolumetricStack, out IntPtr ATheColorData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_getpropertycount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_GetPropertyCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_getproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_GetProperty (IntPtr Handle, UInt32 AIndex, out IntPtr AThePropertyData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_findproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_FindProperty (IntPtr Handle, byte[] AName, out IntPtr AThePropertyData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_addproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_AddProperty (IntPtr Handle, byte[] AName, IntPtr ATheVolumetricStack, out IntPtr AThePropertyData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumedata_removeproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumeData_RemoveProperty (IntPtr Handle, byte[] AName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_component_getobjectresource", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Component_GetObjectResource (IntPtr Handle, out IntPtr AObjectResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_component_getobjectresourceid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Component_GetObjectResourceID (IntPtr Handle, out UInt32 AUniqueResourceID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_component_getuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Component_GetUUID (IntPtr Handle, out Byte AHasUUID, UInt32 sizeUUID, out UInt32 neededUUID, IntPtr dataUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_component_setuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Component_SetUUID (IntPtr Handle, byte[] AUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_component_hastransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Component_HasTransform (IntPtr Handle, out Byte AHasTransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_component_gettransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Component_GetTransform (IntPtr Handle, out InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_component_settransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Component_SetTransform (IntPtr Handle, InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_componentsobject_addcomponent", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ComponentsObject_AddComponent (IntPtr Handle, IntPtr AObjectResource, InternalTransform ATransform, out IntPtr AComponentInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_componentsobject_getcomponent", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ComponentsObject_GetComponent (IntPtr Handle, UInt32 AIndex, out IntPtr AComponentInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_componentsobject_getcomponentcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ComponentsObject_GetComponentCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_setname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_SetName (IntPtr Handle, byte[] AName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_getname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_GetName (IntPtr Handle, UInt32 sizeName, out UInt32 neededName, IntPtr dataName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_setidentifier", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_SetIdentifier (IntPtr Handle, byte[] AIdentifier);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_getidentifier", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_GetIdentifier (IntPtr Handle, UInt32 sizeIdentifier, out UInt32 neededIdentifier, IntPtr dataIdentifier);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_getreferencecount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_GetReferenceCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_setreferences", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_SetReferences (IntPtr Handle, UInt64 sizeReferences, IntPtr dataReferences);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_getreferences", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_GetReferences (IntPtr Handle, UInt64 sizeReferences, out UInt64 neededReferences, IntPtr dataReferences);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_getballreferencecount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_GetBallReferenceCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_setballreferences", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_SetBallReferences (IntPtr Handle, UInt64 sizeBallReferences, IntPtr dataBallReferences);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_beamset_getballreferences", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BeamSet_GetBallReferences (IntPtr Handle, UInt64 sizeBallReferences, out UInt64 neededBallReferences, IntPtr dataBallReferences);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_basematerialgroup_getcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BaseMaterialGroup_GetCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_basematerialgroup_getallpropertyids", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BaseMaterialGroup_GetAllPropertyIDs (IntPtr Handle, UInt64 sizePropertyIDs, out UInt64 neededPropertyIDs, IntPtr dataPropertyIDs);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_basematerialgroup_addmaterial", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BaseMaterialGroup_AddMaterial (IntPtr Handle, byte[] AName, InternalColor ADisplayColor, out UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_basematerialgroup_removematerial", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BaseMaterialGroup_RemoveMaterial (IntPtr Handle, UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_basematerialgroup_getname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BaseMaterialGroup_GetName (IntPtr Handle, UInt32 APropertyID, UInt32 sizeName, out UInt32 neededName, IntPtr dataName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_basematerialgroup_setname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BaseMaterialGroup_SetName (IntPtr Handle, UInt32 APropertyID, byte[] AName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_basematerialgroup_setdisplaycolor", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BaseMaterialGroup_SetDisplayColor (IntPtr Handle, UInt32 APropertyID, InternalColor ATheColor);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_basematerialgroup_getdisplaycolor", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BaseMaterialGroup_GetDisplayColor (IntPtr Handle, UInt32 APropertyID, out InternalColor ATheColor);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_colorgroup_getcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ColorGroup_GetCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_colorgroup_getallpropertyids", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ColorGroup_GetAllPropertyIDs (IntPtr Handle, UInt64 sizePropertyIDs, out UInt64 neededPropertyIDs, IntPtr dataPropertyIDs);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_colorgroup_addcolor", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ColorGroup_AddColor (IntPtr Handle, InternalColor ATheColor, out UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_colorgroup_removecolor", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ColorGroup_RemoveColor (IntPtr Handle, UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_colorgroup_setcolor", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ColorGroup_SetColor (IntPtr Handle, UInt32 APropertyID, InternalColor ATheColor);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_colorgroup_getcolor", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ColorGroup_GetColor (IntPtr Handle, UInt32 APropertyID, out InternalColor ATheColor);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2dgroup_getcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2DGroup_GetCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2dgroup_getallpropertyids", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2DGroup_GetAllPropertyIDs (IntPtr Handle, UInt64 sizePropertyIDs, out UInt64 neededPropertyIDs, IntPtr dataPropertyIDs);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2dgroup_addtex2coord", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2DGroup_AddTex2Coord (IntPtr Handle, InternalTex2Coord AUVCoordinate, out UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2dgroup_gettex2coord", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2DGroup_GetTex2Coord (IntPtr Handle, UInt32 APropertyID, out InternalTex2Coord AUVCoordinate);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2dgroup_removetex2coord", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2DGroup_RemoveTex2Coord (IntPtr Handle, UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2dgroup_gettexture2d", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2DGroup_GetTexture2D (IntPtr Handle, out IntPtr ATexture2DInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_compositematerials_getcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 CompositeMaterials_GetCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_compositematerials_getallpropertyids", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 CompositeMaterials_GetAllPropertyIDs (IntPtr Handle, UInt64 sizePropertyIDs, out UInt64 neededPropertyIDs, IntPtr dataPropertyIDs);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_compositematerials_getbasematerialgroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 CompositeMaterials_GetBaseMaterialGroup (IntPtr Handle, out IntPtr ABaseMaterialGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_compositematerials_addcomposite", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 CompositeMaterials_AddComposite (IntPtr Handle, UInt64 sizeComposite, IntPtr dataComposite, out UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_compositematerials_removecomposite", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 CompositeMaterials_RemoveComposite (IntPtr Handle, UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_compositematerials_getcomposite", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 CompositeMaterials_GetComposite (IntPtr Handle, UInt32 APropertyID, UInt64 sizeComposite, out UInt64 neededComposite, IntPtr dataComposite);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_getcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_GetCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_getallpropertyids", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_GetAllPropertyIDs (IntPtr Handle, UInt64 sizePropertyIDs, out UInt64 neededPropertyIDs, IntPtr dataPropertyIDs);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_addmultiproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_AddMultiProperty (IntPtr Handle, UInt64 sizePropertyIDs, IntPtr dataPropertyIDs, out UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_setmultiproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_SetMultiProperty (IntPtr Handle, UInt32 APropertyID, UInt64 sizePropertyIDs, IntPtr dataPropertyIDs);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_getmultiproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_GetMultiProperty (IntPtr Handle, UInt32 APropertyID, UInt64 sizePropertyIDs, out UInt64 neededPropertyIDs, IntPtr dataPropertyIDs);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_removemultiproperty", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_RemoveMultiProperty (IntPtr Handle, UInt32 APropertyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_getlayercount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_GetLayerCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_addlayer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_AddLayer (IntPtr Handle, InternalMultiPropertyLayer ATheLayer, out UInt32 ALayerIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_getlayer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_GetLayer (IntPtr Handle, UInt32 ALayerIndex, out InternalMultiPropertyLayer ATheLayer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_multipropertygroup_removelayer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 MultiPropertyGroup_RemoveLayer (IntPtr Handle, UInt32 ALayerIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_getsizex", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_GetSizeX (IntPtr Handle, out UInt32 ASizeX);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_getsizey", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_GetSizeY (IntPtr Handle, out UInt32 ASizeY);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_getsheetcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_GetSheetCount (IntPtr Handle, out UInt32 ASheetCount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_getsheet", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_GetSheet (IntPtr Handle, UInt32 AIndex, out IntPtr ASheet);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_getsheetminvalue", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_GetSheetMinValue (IntPtr Handle, UInt32 AIndex, out Double AMinVal);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_getsheetmaxvalue", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_GetSheetMaxValue (IntPtr Handle, UInt32 AIndex, out Double AMaxVal);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_createemptysheet", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_CreateEmptySheet (IntPtr Handle, UInt32 AIndex, byte[] APath, Double AMin, Double AMax, out IntPtr ASheet);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_createsheetfrombuffer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_CreateSheetFromBuffer (IntPtr Handle, UInt32 AIndex, byte[] APath, UInt64 sizeData, IntPtr dataData, Double AMin, Double AMax, out IntPtr ASheet);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_createsheetfromfile", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_CreateSheetFromFile (IntPtr Handle, UInt32 AIndex, byte[] APath, byte[] AFileName, Double AMin, Double AMax, out IntPtr ASheet);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_setsheet", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_SetSheet (IntPtr Handle, UInt32 AIndex, IntPtr ASheet);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_setsheetminvalue", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_SetSheetMinValue (IntPtr Handle, UInt32 AIndex, Double AMinVal);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3d_setsheetmaxvalue", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3D_SetSheetMaxValue (IntPtr Handle, UInt32 AIndex, Double AMaxVal);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_getimage", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_GetImage (IntPtr Handle, out IntPtr AImage3D);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_setimage", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_SetImage (IntPtr Handle, IntPtr AImage3D);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_setsourcechannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_SetSourceChannel (IntPtr Handle, byte[] AChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_getsourcechannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_GetSourceChannel (IntPtr Handle, UInt32 sizeChannelName, out UInt32 neededChannelName, IntPtr dataChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_setdestinationchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_SetDestinationChannel (IntPtr Handle, byte[] AChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_getdestinationchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_GetDestinationChannel (IntPtr Handle, UInt32 sizeChannelName, out UInt32 neededChannelName, IntPtr dataChannelName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_setfilter", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_SetFilter (IntPtr Handle, Int32 AFilter);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_getfilter", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_GetFilter (IntPtr Handle, out Int32 AFilter);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_settilestyles", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_SetTileStyles (IntPtr Handle, Int32 ATileStyleU, Int32 ATileStyleV, Int32 ATileStyleW);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_image3dchannelselector_gettilestyles", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Image3DChannelSelector_GetTileStyles (IntPtr Handle, out Int32 ATileStyleU, out Int32 ATileStyleV, out Int32 ATileStyleW);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_gettransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_GetTransform (IntPtr Handle, out InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_settransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_SetTransform (IntPtr Handle, InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_getblendmethod", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_GetBlendMethod (IntPtr Handle, out Int32 ABlendMethod);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_setblendmethod", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_SetBlendMethod (IntPtr Handle, Int32 ABlendMethod);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_getsourcealpha", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_GetSourceAlpha (IntPtr Handle, out Double ASourceAlpha);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_setsourcealpha", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_SetSourceAlpha (IntPtr Handle, Double ASourceAlpha);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_getdestinationalpha", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_GetDestinationAlpha (IntPtr Handle, out Double ADestinationAlpha);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_setdestinationalpha", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_SetDestinationAlpha (IntPtr Handle, Double ADestinationAlpha);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_getinformation", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_GetInformation (IntPtr Handle, out InternalTransform ATransform, out Int32 ABlendMethod, out Double ASourceAlpha, out Double ADestinationAlpha);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_setinformation", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_SetInformation (IntPtr Handle, InternalTransform ATransform, Int32 ABlendMethod, Double ASourceAlpha, Double ADestinationAlpha);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_createmaskchannelselector", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_CreateMaskChannelSelector (IntPtr Handle, IntPtr AImage3D, byte[] ASourceChannel, byte[] ADestinationChannel, out IntPtr AChannelSelector);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_hasmaskchannelselector", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_HasMaskChannelSelector (IntPtr Handle, out Byte ASelectorExists);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_clearmaskchannelselector", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_ClearMaskChannelSelector (IntPtr Handle);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_getmaskchannelselector", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_GetMaskChannelSelector (IntPtr Handle, out IntPtr AChannelSelector);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_getchannelselectorcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_GetChannelSelectorCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_getchannelselector", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_GetChannelSelector (IntPtr Handle, UInt32 AIndex, out IntPtr AChannelSelector);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_addchannelselector", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_AddChannelSelector (IntPtr Handle, IntPtr AImage3D, byte[] ASourceChannel, byte[] ADestinationChannel, out IntPtr AChannelSelector);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_clearchannelselectors", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_ClearChannelSelectors (IntPtr Handle);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_reindexchannelselector", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_ReindexChannelSelector (IntPtr Handle, IntPtr AChannelSelector, UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_removechannelselector", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_RemoveChannelSelector (IntPtr Handle, IntPtr AChannelSelector);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetriclayer_removechannelselectorbyindex", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricLayer_RemoveChannelSelectorByIndex (IntPtr Handle, UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_clear", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_Clear (IntPtr Handle);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_clearunuseddestinationchannels", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_ClearUnusedDestinationChannels (IntPtr Handle);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_getdestinationchannelcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_GetDestinationChannelCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_getdestinationchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_GetDestinationChannel (IntPtr Handle, UInt32 AIndex, UInt32 sizeName, out UInt32 neededName, IntPtr dataName, out Double ABackground);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_adddestinationchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_AddDestinationChannel (IntPtr Handle, byte[] AName, Double ABackground, out UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_updatedestinationchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_UpdateDestinationChannel (IntPtr Handle, UInt32 AIndex, Double ABackground);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_updatedestinationchannelbyname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_UpdateDestinationChannelByName (IntPtr Handle, byte[] AName, Double ABackground);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_removedestinationchannel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_RemoveDestinationChannel (IntPtr Handle, UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_removedestinationchannelbyname", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_RemoveDestinationChannelByName (IntPtr Handle, byte[] AName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_getlayercount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_GetLayerCount (IntPtr Handle, out UInt32 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_getlayer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_GetLayer (IntPtr Handle, UInt32 AIndex, out IntPtr ALayer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_addlayer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_AddLayer (IntPtr Handle, InternalTransform ATransform, Int32 ABlendMethod, out IntPtr ALayer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_reindexlayer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_ReindexLayer (IntPtr Handle, IntPtr ALayer, UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_removelayer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_RemoveLayer (IntPtr Handle, IntPtr ALayer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_volumetricstack_removelayerbyindex", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 VolumetricStack_RemoveLayerByIndex (IntPtr Handle, UInt32 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_getpath", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_GetPath (IntPtr Handle, UInt32 sizePath, out UInt32 neededPath, IntPtr dataPath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_setpath", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_SetPath (IntPtr Handle, byte[] APath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_packagepart", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_PackagePart (IntPtr Handle, out IntPtr APackagePart);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_getrelationshiptype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_GetRelationShipType (IntPtr Handle, UInt32 sizePath, out UInt32 neededPath, IntPtr dataPath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_setrelationshiptype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_SetRelationShipType (IntPtr Handle, byte[] APath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_writetofile", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_WriteToFile (IntPtr Handle, byte[] AFileName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_readfromfile", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_ReadFromFile (IntPtr Handle, byte[] AFileName);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_readfromcallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_ReadFromCallback (IntPtr Handle, IntPtr ATheReadCallback, UInt64 AStreamSize, IntPtr ATheSeekCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_getstreamsize", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_GetStreamSize (IntPtr Handle, out UInt64 AStreamSize);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_writetobuffer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_WriteToBuffer (IntPtr Handle, UInt64 sizeBuffer, out UInt64 neededBuffer, IntPtr dataBuffer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_attachment_readfrombuffer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Attachment_ReadFromBuffer (IntPtr Handle, UInt64 sizeBuffer, IntPtr dataBuffer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2d_getattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2D_GetAttachment (IntPtr Handle, out IntPtr AAttachment);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2d_setattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2D_SetAttachment (IntPtr Handle, IntPtr AAttachment);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2d_getcontenttype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2D_GetContentType (IntPtr Handle, out Int32 AContentType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2d_setcontenttype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2D_SetContentType (IntPtr Handle, Int32 AContentType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2d_gettilestyleuv", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2D_GetTileStyleUV (IntPtr Handle, out Int32 ATileStyleU, out Int32 ATileStyleV);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2d_settilestyleuv", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2D_SetTileStyleUV (IntPtr Handle, Int32 ATileStyleU, Int32 ATileStyleV);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2d_getfilter", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2D_GetFilter (IntPtr Handle, out Int32 AFilter);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_texture2d_setfilter", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Texture2D_SetFilter (IntPtr Handle, Int32 AFilter);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_getobjectresource", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_GetObjectResource (IntPtr Handle, out IntPtr AObjectResource);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_getuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_GetUUID (IntPtr Handle, out Byte AHasUUID, UInt32 sizeUUID, out UInt32 neededUUID, IntPtr dataUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_setuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_SetUUID (IntPtr Handle, byte[] AUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_getobjectresourceid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_GetObjectResourceID (IntPtr Handle, out UInt32 AUniqueResourceID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_hasobjecttransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_HasObjectTransform (IntPtr Handle, out Byte AHasTransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_getobjecttransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_GetObjectTransform (IntPtr Handle, out InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_setobjecttransform", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_SetObjectTransform (IntPtr Handle, InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_getpartnumber", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_GetPartNumber (IntPtr Handle, UInt32 sizePartNumber, out UInt32 neededPartNumber, IntPtr dataPartNumber);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_setpartnumber", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_SetPartNumber (IntPtr Handle, byte[] ASetPartnumber);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_getmetadatagroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_GetMetaDataGroup (IntPtr Handle, out IntPtr AMetaDataGroup);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditem_getoutbox", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItem_GetOutbox (IntPtr Handle, out InternalBox AOutbox);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditemiterator_movenext", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItemIterator_MoveNext (IntPtr Handle, out Byte AHasNext);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditemiterator_moveprevious", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItemIterator_MovePrevious (IntPtr Handle, out Byte AHasPrevious);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditemiterator_getcurrent", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItemIterator_GetCurrent (IntPtr Handle, out IntPtr ABuildItem);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditemiterator_clone", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItemIterator_Clone (IntPtr Handle, out IntPtr AOutBuildItemIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_builditemiterator_count", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 BuildItemIterator_Count (IntPtr Handle, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slice_setvertices", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Slice_SetVertices (IntPtr Handle, UInt64 sizeVertices, IntPtr dataVertices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slice_getvertices", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Slice_GetVertices (IntPtr Handle, UInt64 sizeVertices, out UInt64 neededVertices, IntPtr dataVertices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slice_getvertexcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Slice_GetVertexCount (IntPtr Handle, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slice_addpolygon", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Slice_AddPolygon (IntPtr Handle, UInt64 sizeIndices, IntPtr dataIndices, out UInt64 AIndex);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slice_getpolygoncount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Slice_GetPolygonCount (IntPtr Handle, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slice_setpolygonindices", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Slice_SetPolygonIndices (IntPtr Handle, UInt64 AIndex, UInt64 sizeIndices, IntPtr dataIndices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slice_getpolygonindices", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Slice_GetPolygonIndices (IntPtr Handle, UInt64 AIndex, UInt64 sizeIndices, out UInt64 neededIndices, IntPtr dataIndices);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slice_getpolygonindexcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Slice_GetPolygonIndexCount (IntPtr Handle, UInt64 AIndex, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slice_getztop", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Slice_GetZTop (IntPtr Handle, out Double AZTop);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_getbottomz", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_GetBottomZ (IntPtr Handle, out Double AZBottom);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_getslicecount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_GetSliceCount (IntPtr Handle, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_getslice", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_GetSlice (IntPtr Handle, UInt64 ASliceIndex, out IntPtr ATheSlice);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_addslice", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_AddSlice (IntPtr Handle, Double AZTop, out IntPtr ATheSlice);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_getslicerefcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_GetSliceRefCount (IntPtr Handle, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_addslicestackreference", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_AddSliceStackReference (IntPtr Handle, IntPtr ATheSliceStack);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_getslicestackreference", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_GetSliceStackReference (IntPtr Handle, UInt64 ASliceRefIndex, out IntPtr ATheSliceStack);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_collapseslicereferences", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_CollapseSliceReferences (IntPtr Handle);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_setownpath", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_SetOwnPath (IntPtr Handle, byte[] APath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_slicestack_getownpath", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 SliceStack_GetOwnPath (IntPtr Handle, UInt32 sizePath, out UInt32 neededPath, IntPtr dataPath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_consumer_getconsumerid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Consumer_GetConsumerID (IntPtr Handle, UInt32 sizeConsumerID, out UInt32 neededConsumerID, IntPtr dataConsumerID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_consumer_getkeyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Consumer_GetKeyID (IntPtr Handle, UInt32 sizeKeyID, out UInt32 neededKeyID, IntPtr dataKeyID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_consumer_getkeyvalue", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Consumer_GetKeyValue (IntPtr Handle, UInt32 sizeKeyValue, out UInt32 neededKeyValue, IntPtr dataKeyValue);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_accessright_getconsumer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 AccessRight_GetConsumer (IntPtr Handle, out IntPtr AConsumer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_accessright_getwrappingalgorithm", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 AccessRight_GetWrappingAlgorithm (IntPtr Handle, out Int32 AAlgorithm);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_accessright_getmgfalgorithm", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 AccessRight_GetMgfAlgorithm (IntPtr Handle, out Int32 AAlgorithm);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_accessright_getdigestmethod", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 AccessRight_GetDigestMethod (IntPtr Handle, out Int32 AAlgorithm);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_contentencryptionparams_getencryptionalgorithm", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ContentEncryptionParams_GetEncryptionAlgorithm (IntPtr Handle, out Int32 AAlgorithm);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_contentencryptionparams_getkey", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ContentEncryptionParams_GetKey (IntPtr Handle, UInt64 sizeByteData, out UInt64 neededByteData, IntPtr dataByteData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_contentencryptionparams_getinitializationvector", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ContentEncryptionParams_GetInitializationVector (IntPtr Handle, UInt64 sizeByteData, out UInt64 neededByteData, IntPtr dataByteData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_contentencryptionparams_getauthenticationtag", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ContentEncryptionParams_GetAuthenticationTag (IntPtr Handle, UInt64 sizeByteData, out UInt64 neededByteData, IntPtr dataByteData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_contentencryptionparams_setauthenticationtag", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ContentEncryptionParams_SetAuthenticationTag (IntPtr Handle, UInt64 sizeByteData, IntPtr dataByteData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_contentencryptionparams_getadditionalauthenticationdata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ContentEncryptionParams_GetAdditionalAuthenticationData (IntPtr Handle, UInt64 sizeByteData, out UInt64 neededByteData, IntPtr dataByteData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_contentencryptionparams_getdescriptor", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ContentEncryptionParams_GetDescriptor (IntPtr Handle, out UInt64 ADescriptor);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_contentencryptionparams_getkeyuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ContentEncryptionParams_GetKeyUUID (IntPtr Handle, UInt32 sizeUUID, out UInt32 neededUUID, IntPtr dataUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourcedata_getpath", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceData_GetPath (IntPtr Handle, out IntPtr APath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourcedata_getencryptionalgorithm", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceData_GetEncryptionAlgorithm (IntPtr Handle, out Int32 AEncryptionAlgorithm);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourcedata_getcompression", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceData_GetCompression (IntPtr Handle, out Int32 ACompression);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourcedata_getadditionalauthenticationdata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceData_GetAdditionalAuthenticationData (IntPtr Handle, UInt64 sizeByteData, out UInt64 neededByteData, IntPtr dataByteData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourcedatagroup_getkeyuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceDataGroup_GetKeyUUID (IntPtr Handle, UInt32 sizeUUID, out UInt32 neededUUID, IntPtr dataUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourcedatagroup_addaccessright", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceDataGroup_AddAccessRight (IntPtr Handle, IntPtr AConsumer, Int32 AWrappingAlgorithm, Int32 AMgfAlgorithm, Int32 ADigestMethod, out IntPtr ATheAccessRight);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourcedatagroup_findaccessrightbyconsumer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceDataGroup_FindAccessRightByConsumer (IntPtr Handle, IntPtr AConsumer, out IntPtr ATheAccessRight);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_resourcedatagroup_removeaccessright", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 ResourceDataGroup_RemoveAccessRight (IntPtr Handle, IntPtr AConsumer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_addconsumer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_AddConsumer (IntPtr Handle, byte[] AConsumerID, byte[] AKeyID, byte[] AKeyValue, out IntPtr AConsumer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_getconsumercount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_GetConsumerCount (IntPtr Handle, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_getconsumer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_GetConsumer (IntPtr Handle, UInt64 AConsumerIndex, out IntPtr AConsumer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_removeconsumer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_RemoveConsumer (IntPtr Handle, IntPtr AConsumer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_findconsumer", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_FindConsumer (IntPtr Handle, byte[] AConsumerID, out IntPtr AConsumer);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_getresourcedatagroupcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_GetResourceDataGroupCount (IntPtr Handle, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_addresourcedatagroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_AddResourceDataGroup (IntPtr Handle, out IntPtr AResourceDataGroup);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_getresourcedatagroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_GetResourceDataGroup (IntPtr Handle, UInt64 AResourceDataIndex, out IntPtr AResourceDataGroup);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_removeresourcedatagroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_RemoveResourceDataGroup (IntPtr Handle, IntPtr AResourceDataGroup);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_findresourcedatagroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_FindResourceDataGroup (IntPtr Handle, IntPtr APartPath, out IntPtr AResourceDataGroup);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_addresourcedata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_AddResourceData (IntPtr Handle, IntPtr AResourceDataGroup, IntPtr APartPath, Int32 AAlgorithm, Int32 ACompression, UInt64 sizeAdditionalAuthenticationData, IntPtr dataAdditionalAuthenticationData, out IntPtr AResourceData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_removeresourcedata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_RemoveResourceData (IntPtr Handle, IntPtr AResourceData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_findresourcedata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_FindResourceData (IntPtr Handle, IntPtr AResourcePath, out IntPtr AResourceData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_getresourcedatacount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_GetResourceDataCount (IntPtr Handle, out UInt64 ACount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_getresourcedata", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_GetResourceData (IntPtr Handle, UInt64 AResourceDataIndex, out IntPtr AResourceData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_getuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_GetUUID (IntPtr Handle, out Byte AHasUUID, UInt32 sizeUUID, out UInt32 neededUUID, IntPtr dataUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_keystore_setuuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 KeyStore_SetUUID (IntPtr Handle, byte[] AUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_rootmodelpart", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_RootModelPart (IntPtr Handle, out IntPtr ARootModelPart);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_findorcreatepackagepart", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_FindOrCreatePackagePart (IntPtr Handle, byte[] AAbsolutePath, out IntPtr AModelPart);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_setunit", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_SetUnit (IntPtr Handle, Int32 AUnit);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getunit", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetUnit (IntPtr Handle, out Int32 AUnit);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getlanguage", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetLanguage (IntPtr Handle, UInt32 sizeLanguage, out UInt32 neededLanguage, IntPtr dataLanguage);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_setlanguage", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_SetLanguage (IntPtr Handle, byte[] ALanguage);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_querywriter", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_QueryWriter (IntPtr Handle, byte[] AWriterClass, out IntPtr AWriterInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_queryreader", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_QueryReader (IntPtr Handle, byte[] AReaderClass, out IntPtr AReaderInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_gettexture2dbyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetTexture2DByID (IntPtr Handle, UInt32 AUniqueResourceID, out IntPtr ATextureInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getpropertytypebyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetPropertyTypeByID (IntPtr Handle, UInt32 AUniqueResourceID, out Int32 AThePropertyType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getbasematerialgroupbyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetBaseMaterialGroupByID (IntPtr Handle, UInt32 AUniqueResourceID, out IntPtr ABaseMaterialGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_gettexture2dgroupbyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetTexture2DGroupByID (IntPtr Handle, UInt32 AUniqueResourceID, out IntPtr ATexture2DGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getcompositematerialsbyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetCompositeMaterialsByID (IntPtr Handle, UInt32 AUniqueResourceID, out IntPtr ACompositeMaterialsInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getmultipropertygroupbyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetMultiPropertyGroupByID (IntPtr Handle, UInt32 AUniqueResourceID, out IntPtr AMultiPropertyGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getmeshobjectbyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetMeshObjectByID (IntPtr Handle, UInt32 AUniqueResourceID, out IntPtr AMeshObjectInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getcomponentsobjectbyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetComponentsObjectByID (IntPtr Handle, UInt32 AUniqueResourceID, out IntPtr AComponentsObjectInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getcolorgroupbyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetColorGroupByID (IntPtr Handle, UInt32 AUniqueResourceID, out IntPtr AColorGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getslicestackbyid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetSliceStackByID (IntPtr Handle, UInt32 AUniqueResourceID, out IntPtr ASliceStacInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getbuilduuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetBuildUUID (IntPtr Handle, out Byte AHasUUID, UInt32 sizeUUID, out UInt32 neededUUID, IntPtr dataUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_setbuilduuid", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_SetBuildUUID (IntPtr Handle, byte[] AUUID);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getbuilditems", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetBuildItems (IntPtr Handle, out IntPtr ABuildItemIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getoutbox", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetOutbox (IntPtr Handle, out InternalBox AOutbox);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getresources", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetResources (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getobjects", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetObjects (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getmeshobjects", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetMeshObjects (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getcomponentsobjects", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetComponentsObjects (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_gettexture2ds", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetTexture2Ds (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getbasematerialgroups", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetBaseMaterialGroups (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getcolorgroups", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetColorGroups (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_gettexture2dgroups", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetTexture2DGroups (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getcompositematerials", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetCompositeMaterials (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getmultipropertygroups", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetMultiPropertyGroups (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getslicestacks", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetSliceStacks (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getimage3ds", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetImage3Ds (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_mergetomodel", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_MergeToModel (IntPtr Handle, out IntPtr AMergedModelInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getvolumetricstacks", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetVolumetricStacks (IntPtr Handle, out IntPtr AResourceIterator);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addmeshobject", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddMeshObject (IntPtr Handle, out IntPtr AMeshObjectInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addcomponentsobject", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddComponentsObject (IntPtr Handle, out IntPtr AComponentsObjectInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addslicestack", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddSliceStack (IntPtr Handle, Double AZBottom, out IntPtr ASliceStackInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addtexture2dfromattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddTexture2DFromAttachment (IntPtr Handle, IntPtr ATextureAttachment, out IntPtr ATexture2DInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addbasematerialgroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddBaseMaterialGroup (IntPtr Handle, out IntPtr ABaseMaterialGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addcolorgroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddColorGroup (IntPtr Handle, out IntPtr AColorGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addtexture2dgroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddTexture2DGroup (IntPtr Handle, IntPtr ATexture2DInstance, out IntPtr ATexture2DGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addcompositematerials", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddCompositeMaterials (IntPtr Handle, IntPtr ABaseMaterialGroupInstance, out IntPtr ACompositeMaterialsInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addmultipropertygroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddMultiPropertyGroup (IntPtr Handle, out IntPtr AMultiPropertyGroupInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addimage3d", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddImage3D (IntPtr Handle, UInt32 ASizeX, UInt32 ASizeY, UInt32 ASheetCount, out IntPtr AInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addvolumetricstack", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddVolumetricStack (IntPtr Handle, out IntPtr AInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addbuilditem", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddBuildItem (IntPtr Handle, IntPtr AObject, InternalTransform ATransform, out IntPtr ABuildItemInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_removebuilditem", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_RemoveBuildItem (IntPtr Handle, IntPtr ABuildItemInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getmetadatagroup", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetMetaDataGroup (IntPtr Handle, out IntPtr ATheMetaDataGroup);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddAttachment (IntPtr Handle, byte[] AURI, byte[] ARelationShipType, out IntPtr AAttachmentInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_removeattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_RemoveAttachment (IntPtr Handle, IntPtr AAttachmentInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetAttachment (IntPtr Handle, UInt32 AIndex, out IntPtr AAttachmentInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_findattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_FindAttachment (IntPtr Handle, byte[] AURI, out IntPtr AAttachmentInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getattachmentcount", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetAttachmentCount (IntPtr Handle, out UInt32 AAttachmentCount);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_haspackagethumbnailattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_HasPackageThumbnailAttachment (IntPtr Handle, out Byte AHasThumbnail);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_createpackagethumbnailattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_CreatePackageThumbnailAttachment (IntPtr Handle, out IntPtr AAttachment);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getpackagethumbnailattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetPackageThumbnailAttachment (IntPtr Handle, out IntPtr AAttachment);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_removepackagethumbnailattachment", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_RemovePackageThumbnailAttachment (IntPtr Handle);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_addcustomcontenttype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_AddCustomContentType (IntPtr Handle, byte[] AExtension, byte[] AContentType);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_removecustomcontenttype", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_RemoveCustomContentType (IntPtr Handle, byte[] AExtension);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_setrandomnumbercallback", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_SetRandomNumberCallback (IntPtr Handle, IntPtr ATheCallback, UInt64 AUserData);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_model_getkeystore", CallingConvention=CallingConvention.Cdecl)]
			public unsafe extern static Int32 Model_GetKeyStore (IntPtr Handle, out IntPtr AKeyStore);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_getlibraryversion", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetLibraryVersion (out UInt32 AMajor, out UInt32 AMinor, out UInt32 AMicro);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_getprereleaseinformation", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetPrereleaseInformation (out Byte AHasPrereleaseInfo, UInt32 sizePrereleaseInfo, out UInt32 neededPrereleaseInfo, IntPtr dataPrereleaseInfo);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_getbuildinformation", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetBuildInformation (out Byte AHasBuildInfo, UInt32 sizeBuildInformation, out UInt32 neededBuildInformation, IntPtr dataBuildInformation);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_getspecificationversion", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetSpecificationVersion (byte[] ASpecificationURL, out Byte AIsSupported, out UInt32 AMajor, out UInt32 AMinor, out UInt32 AMicro);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_createmodel", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 CreateModel (out IntPtr AModel);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_release", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 Release (IntPtr AInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_acquire", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 Acquire (IntPtr AInstance);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_setjournal", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 SetJournal (byte[] AJournalPath);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_getlasterror", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetLastError (IntPtr AInstance, UInt32 sizeLastErrorString, out UInt32 neededLastErrorString, IntPtr dataLastErrorString, out Byte AHasLastError);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_getsymbollookupmethod", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetSymbolLookupMethod (out UInt64 ASymbolLookupMethod);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_retrieveprogressmessage", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 RetrieveProgressMessage (Int32 ATheProgressIdentifier, UInt32 sizeProgressMessage, out UInt32 neededProgressMessage, IntPtr dataProgressMessage);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_rgbatocolor", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 RGBAToColor (Byte ARed, Byte AGreen, Byte ABlue, Byte AAlpha, out InternalColor ATheColor);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_floatrgbatocolor", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 FloatRGBAToColor (Single ARed, Single AGreen, Single ABlue, Single AAlpha, out InternalColor ATheColor);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_colortorgba", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 ColorToRGBA (InternalColor ATheColor, out Byte ARed, out Byte AGreen, out Byte ABlue, out Byte AAlpha);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_colortofloatrgba", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 ColorToFloatRGBA (InternalColor ATheColor, out Single ARed, out Single AGreen, out Single ABlue, out Single AAlpha);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_getidentitytransform", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetIdentityTransform (out InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_getuniformscaletransform", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetUniformScaleTransform (Single AFactor, out InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_getscaletransform", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetScaleTransform (Single AFactorX, Single AFactorY, Single AFactorZ, out InternalTransform ATransform);

			[DllImport("lib3mf.dll", EntryPoint = "lib3mf_gettranslationtransform", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.Cdecl)]
			public extern static Int32 GetTranslationTransform (Single AVectorX, Single AVectorY, Single AVectorZ, out InternalTransform ATransform);

			public unsafe static sTriangle convertInternalToStruct_Triangle (InternalTriangle intTriangle)
			{
				sTriangle Triangle;
				Triangle.Indices = new UInt32[3];
				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					Triangle.Indices[rowIndex] = intTriangle.Indices[rowIndex];
				}

				return Triangle;
			}

			public unsafe static InternalTriangle convertStructToInternal_Triangle (sTriangle Triangle)
			{
				InternalTriangle intTriangle;
				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					intTriangle.Indices[rowIndex] = Triangle.Indices[rowIndex];
				}

				return intTriangle;
			}

			public unsafe static sTriangleProperties convertInternalToStruct_TriangleProperties (InternalTriangleProperties intTriangleProperties)
			{
				sTriangleProperties TriangleProperties;
				TriangleProperties.ResourceID = intTriangleProperties.ResourceID;
				TriangleProperties.PropertyIDs = new UInt32[3];
				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					TriangleProperties.PropertyIDs[rowIndex] = intTriangleProperties.PropertyIDs[rowIndex];
				}

				return TriangleProperties;
			}

			public unsafe static InternalTriangleProperties convertStructToInternal_TriangleProperties (sTriangleProperties TriangleProperties)
			{
				InternalTriangleProperties intTriangleProperties;
				intTriangleProperties.ResourceID = TriangleProperties.ResourceID;
				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					intTriangleProperties.PropertyIDs[rowIndex] = TriangleProperties.PropertyIDs[rowIndex];
				}

				return intTriangleProperties;
			}

			public unsafe static sPosition convertInternalToStruct_Position (InternalPosition intPosition)
			{
				sPosition Position;
				Position.Coordinates = new Single[3];
				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					Position.Coordinates[rowIndex] = intPosition.Coordinates[rowIndex];
				}

				return Position;
			}

			public unsafe static InternalPosition convertStructToInternal_Position (sPosition Position)
			{
				InternalPosition intPosition;
				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					intPosition.Coordinates[rowIndex] = Position.Coordinates[rowIndex];
				}

				return intPosition;
			}

			public unsafe static sPosition2D convertInternalToStruct_Position2D (InternalPosition2D intPosition2D)
			{
				sPosition2D Position2D;
				Position2D.Coordinates = new Single[2];
				for (int rowIndex = 0; rowIndex < 2; rowIndex++) {
					Position2D.Coordinates[rowIndex] = intPosition2D.Coordinates[rowIndex];
				}

				return Position2D;
			}

			public unsafe static InternalPosition2D convertStructToInternal_Position2D (sPosition2D Position2D)
			{
				InternalPosition2D intPosition2D;
				for (int rowIndex = 0; rowIndex < 2; rowIndex++) {
					intPosition2D.Coordinates[rowIndex] = Position2D.Coordinates[rowIndex];
				}

				return intPosition2D;
			}

			public unsafe static sCompositeConstituent convertInternalToStruct_CompositeConstituent (InternalCompositeConstituent intCompositeConstituent)
			{
				sCompositeConstituent CompositeConstituent;
				CompositeConstituent.PropertyID = intCompositeConstituent.PropertyID;
				CompositeConstituent.MixingRatio = intCompositeConstituent.MixingRatio;
				return CompositeConstituent;
			}

			public unsafe static InternalCompositeConstituent convertStructToInternal_CompositeConstituent (sCompositeConstituent CompositeConstituent)
			{
				InternalCompositeConstituent intCompositeConstituent;
				intCompositeConstituent.PropertyID = CompositeConstituent.PropertyID;
				intCompositeConstituent.MixingRatio = CompositeConstituent.MixingRatio;
				return intCompositeConstituent;
			}

			public unsafe static sMultiPropertyLayer convertInternalToStruct_MultiPropertyLayer (InternalMultiPropertyLayer intMultiPropertyLayer)
			{
				sMultiPropertyLayer MultiPropertyLayer;
				MultiPropertyLayer.ResourceID = intMultiPropertyLayer.ResourceID;
				MultiPropertyLayer.TheBlendMethod = (eBlendMethod) intMultiPropertyLayer.TheBlendMethod;
				return MultiPropertyLayer;
			}

			public unsafe static InternalMultiPropertyLayer convertStructToInternal_MultiPropertyLayer (sMultiPropertyLayer MultiPropertyLayer)
			{
				InternalMultiPropertyLayer intMultiPropertyLayer;
				intMultiPropertyLayer.ResourceID = MultiPropertyLayer.ResourceID;
				intMultiPropertyLayer.TheBlendMethod = (Int32) MultiPropertyLayer.TheBlendMethod;
				return intMultiPropertyLayer;
			}

			public unsafe static sTex2Coord convertInternalToStruct_Tex2Coord (InternalTex2Coord intTex2Coord)
			{
				sTex2Coord Tex2Coord;
				Tex2Coord.U = intTex2Coord.U;
				Tex2Coord.V = intTex2Coord.V;
				return Tex2Coord;
			}

			public unsafe static InternalTex2Coord convertStructToInternal_Tex2Coord (sTex2Coord Tex2Coord)
			{
				InternalTex2Coord intTex2Coord;
				intTex2Coord.U = Tex2Coord.U;
				intTex2Coord.V = Tex2Coord.V;
				return intTex2Coord;
			}

			public unsafe static sTransform convertInternalToStruct_Transform (InternalTransform intTransform)
			{
				sTransform Transform;
				Transform.Fields = new Single[4][];
				for (int colIndex = 0; colIndex < 4; colIndex++) {
					Transform.Fields[colIndex] = new Single[3];
					for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
						Transform.Fields[colIndex][rowIndex] = intTransform.Fields[colIndex * 3 + rowIndex];
					}
				}

				return Transform;
			}

			public unsafe static InternalTransform convertStructToInternal_Transform (sTransform Transform)
			{
				InternalTransform intTransform;
				for (int colIndex = 0; colIndex < 4; colIndex++) {
					for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
						intTransform.Fields[colIndex * 3 + rowIndex] = Transform.Fields[colIndex][rowIndex];
					}
				}

				return intTransform;
			}

			public unsafe static sBox convertInternalToStruct_Box (InternalBox intBox)
			{
				sBox Box;
				Box.MinCoordinate = new Single[3];
				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					Box.MinCoordinate[rowIndex] = intBox.MinCoordinate[rowIndex];
				}

				Box.MaxCoordinate = new Single[3];
				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					Box.MaxCoordinate[rowIndex] = intBox.MaxCoordinate[rowIndex];
				}

				return Box;
			}

			public unsafe static InternalBox convertStructToInternal_Box (sBox Box)
			{
				InternalBox intBox;
				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					intBox.MinCoordinate[rowIndex] = Box.MinCoordinate[rowIndex];
				}

				for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
					intBox.MaxCoordinate[rowIndex] = Box.MaxCoordinate[rowIndex];
				}

				return intBox;
			}

			public unsafe static sColor convertInternalToStruct_Color (InternalColor intColor)
			{
				sColor Color;
				Color.Red = intColor.Red;
				Color.Green = intColor.Green;
				Color.Blue = intColor.Blue;
				Color.Alpha = intColor.Alpha;
				return Color;
			}

			public unsafe static InternalColor convertStructToInternal_Color (sColor Color)
			{
				InternalColor intColor;
				intColor.Red = Color.Red;
				intColor.Green = Color.Green;
				intColor.Blue = Color.Blue;
				intColor.Alpha = Color.Alpha;
				return intColor;
			}

			public unsafe static sBeam convertInternalToStruct_Beam (InternalBeam intBeam)
			{
				sBeam Beam;
				Beam.Indices = new UInt32[2];
				for (int rowIndex = 0; rowIndex < 2; rowIndex++) {
					Beam.Indices[rowIndex] = intBeam.Indices[rowIndex];
				}

				Beam.Radii = new Double[2];
				for (int rowIndex = 0; rowIndex < 2; rowIndex++) {
					Beam.Radii[rowIndex] = intBeam.Radii[rowIndex];
				}

				Beam.CapModes = new eBeamLatticeCapMode[2];
				for (int rowIndex = 0; rowIndex < 2; rowIndex++) {
					Beam.CapModes[rowIndex] = (eBeamLatticeCapMode) intBeam.CapModes[rowIndex];
				}

				return Beam;
			}

			public unsafe static InternalBeam convertStructToInternal_Beam (sBeam Beam)
			{
				InternalBeam intBeam;
				for (int rowIndex = 0; rowIndex < 2; rowIndex++) {
					intBeam.Indices[rowIndex] = Beam.Indices[rowIndex];
				}

				for (int rowIndex = 0; rowIndex < 2; rowIndex++) {
					intBeam.Radii[rowIndex] = Beam.Radii[rowIndex];
				}

				for (int rowIndex = 0; rowIndex < 2; rowIndex++) {
					intBeam.CapModes[rowIndex] = (Int32) Beam.CapModes[rowIndex];
				}

				return intBeam;
			}

			public unsafe static sBall convertInternalToStruct_Ball (InternalBall intBall)
			{
				sBall Ball;
				Ball.Index = intBall.Index;
				Ball.Radius = intBall.Radius;
				return Ball;
			}

			public unsafe static InternalBall convertStructToInternal_Ball (sBall Ball)
			{
				InternalBall intBall;
				intBall.Index = Ball.Index;
				intBall.Radius = Ball.Radius;
				return intBall;
			}

			public static void ThrowError(IntPtr Handle, Int32 errorCode)
			{
				String sMessage = "Lib3MF Error";
				if (Handle != IntPtr.Zero) {
					UInt32 sizeMessage = 0;
					UInt32 neededMessage = 0;
					Byte hasLastError = 0;
					Int32 resultCode1 = GetLastError (Handle, sizeMessage, out neededMessage, IntPtr.Zero, out hasLastError);
					if ((resultCode1 == 0) && (hasLastError != 0)) {
						sizeMessage = neededMessage;
						byte[] bytesMessage = new byte[sizeMessage];

						GCHandle dataMessage = GCHandle.Alloc(bytesMessage, GCHandleType.Pinned);
						Int32 resultCode2 = GetLastError(Handle, sizeMessage, out neededMessage, dataMessage.AddrOfPinnedObject(), out hasLastError);
						dataMessage.Free();

						if ((resultCode2 == 0) && (hasLastError != 0)) {
							sMessage = sMessage + ": " + Encoding.UTF8.GetString(bytesMessage).TrimEnd(char.MinValue);
						}
					}
				}

				throw new Exception(sMessage + "(# " + errorCode + ")");
			}

		}
	}


	class CBase 
	{
		protected IntPtr Handle;

		public CBase (IntPtr NewHandle)
		{
			Handle = NewHandle;
		}

		~CBase ()
		{
			if (Handle != IntPtr.Zero) {
				Internal.Lib3MFWrapper.Release (Handle);
				Handle = IntPtr.Zero;
			}
		}

		protected void CheckError (Int32 errorCode)
		{
			if (errorCode != 0) {
				Internal.Lib3MFWrapper.ThrowError (Handle, errorCode);
			}
		}

		public IntPtr GetHandle ()
		{
			return Handle;
		}

	}

	class CWriter : CBase
	{
		public CWriter (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public void WriteToFile (String AFilename)
		{
			byte[] byteFilename = Encoding.UTF8.GetBytes(AFilename + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Writer_WriteToFile (Handle, byteFilename));
		}

		public UInt64 GetStreamSize ()
		{
			UInt64 resultStreamSize = 0;

			CheckError(Internal.Lib3MFWrapper.Writer_GetStreamSize (Handle, out resultStreamSize));
			return resultStreamSize;
		}

		public void WriteToBuffer (out Byte[] ABuffer)
		{
			UInt64 sizeBuffer = 0;
			UInt64 neededBuffer = 0;
			CheckError(Internal.Lib3MFWrapper.Writer_WriteToBuffer (Handle, sizeBuffer, out neededBuffer, IntPtr.Zero));
			sizeBuffer = neededBuffer;
			ABuffer = new Byte[sizeBuffer];
			GCHandle dataBuffer = GCHandle.Alloc(ABuffer, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Writer_WriteToBuffer (Handle, sizeBuffer, out neededBuffer, dataBuffer.AddrOfPinnedObject()));
			dataBuffer.Free();
		}

		public void WriteToCallback (IntPtr ATheWriteCallback, IntPtr ATheSeekCallback, UInt64 AUserData)
		{

			CheckError(Internal.Lib3MFWrapper.Writer_WriteToCallback (Handle, IntPtr.Zero, IntPtr.Zero, AUserData));
		}

		public void SetProgressCallback (IntPtr AProgressCallback, UInt64 AUserData)
		{

			CheckError(Internal.Lib3MFWrapper.Writer_SetProgressCallback (Handle, IntPtr.Zero, AUserData));
		}

		public UInt32 GetDecimalPrecision ()
		{
			UInt32 resultDecimalPrecision = 0;

			CheckError(Internal.Lib3MFWrapper.Writer_GetDecimalPrecision (Handle, out resultDecimalPrecision));
			return resultDecimalPrecision;
		}

		public void SetDecimalPrecision (UInt32 ADecimalPrecision)
		{

			CheckError(Internal.Lib3MFWrapper.Writer_SetDecimalPrecision (Handle, ADecimalPrecision));
		}

		public void SetStrictModeActive (bool AStrictModeActive)
		{

			CheckError(Internal.Lib3MFWrapper.Writer_SetStrictModeActive (Handle, (Byte)( AStrictModeActive ? 1 : 0 )));
		}

		public bool GetStrictModeActive ()
		{
			Byte resultStrictModeActive = 0;

			CheckError(Internal.Lib3MFWrapper.Writer_GetStrictModeActive (Handle, out resultStrictModeActive));
			return (resultStrictModeActive != 0);
		}

		public String GetWarning (UInt32 AIndex, out UInt32 AErrorCode)
		{
			UInt32 sizeWarning = 0;
			UInt32 neededWarning = 0;
			CheckError(Internal.Lib3MFWrapper.Writer_GetWarning (Handle, AIndex, out AErrorCode, sizeWarning, out neededWarning, IntPtr.Zero));
			sizeWarning = neededWarning;
			byte[] bytesWarning = new byte[sizeWarning];
			GCHandle dataWarning = GCHandle.Alloc(bytesWarning, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Writer_GetWarning (Handle, AIndex, out AErrorCode, sizeWarning, out neededWarning, dataWarning.AddrOfPinnedObject()));
			dataWarning.Free();
			return Encoding.UTF8.GetString(bytesWarning).TrimEnd(char.MinValue);
		}

		public UInt32 GetWarningCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.Writer_GetWarningCount (Handle, out resultCount));
			return resultCount;
		}

		public void AddKeyWrappingCallback (String AConsumerID, IntPtr ATheCallback, UInt64 AUserData)
		{
			byte[] byteConsumerID = Encoding.UTF8.GetBytes(AConsumerID + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Writer_AddKeyWrappingCallback (Handle, byteConsumerID, IntPtr.Zero, AUserData));
		}

		public void SetContentEncryptionCallback (IntPtr ATheCallback, UInt64 AUserData)
		{

			CheckError(Internal.Lib3MFWrapper.Writer_SetContentEncryptionCallback (Handle, IntPtr.Zero, AUserData));
		}

	}

	class CReader : CBase
	{
		public CReader (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public void ReadFromFile (String AFilename)
		{
			byte[] byteFilename = Encoding.UTF8.GetBytes(AFilename + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Reader_ReadFromFile (Handle, byteFilename));
		}

		public void ReadFromBuffer (Byte[] ABuffer)
		{
			GCHandle dataBuffer = GCHandle.Alloc(ABuffer, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Reader_ReadFromBuffer (Handle, (UInt64) ABuffer.Length, dataBuffer.AddrOfPinnedObject()));
			dataBuffer.Free ();
		}

		public void ReadFromCallback (IntPtr ATheReadCallback, UInt64 AStreamSize, IntPtr ATheSeekCallback, UInt64 AUserData)
		{

			CheckError(Internal.Lib3MFWrapper.Reader_ReadFromCallback (Handle, IntPtr.Zero, AStreamSize, IntPtr.Zero, AUserData));
		}

		public void SetProgressCallback (IntPtr AProgressCallback, UInt64 AUserData)
		{

			CheckError(Internal.Lib3MFWrapper.Reader_SetProgressCallback (Handle, IntPtr.Zero, AUserData));
		}

		public void AddRelationToRead (String ARelationShipType)
		{
			byte[] byteRelationShipType = Encoding.UTF8.GetBytes(ARelationShipType + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Reader_AddRelationToRead (Handle, byteRelationShipType));
		}

		public void RemoveRelationToRead (String ARelationShipType)
		{
			byte[] byteRelationShipType = Encoding.UTF8.GetBytes(ARelationShipType + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Reader_RemoveRelationToRead (Handle, byteRelationShipType));
		}

		public void SetStrictModeActive (bool AStrictModeActive)
		{

			CheckError(Internal.Lib3MFWrapper.Reader_SetStrictModeActive (Handle, (Byte)( AStrictModeActive ? 1 : 0 )));
		}

		public bool GetStrictModeActive ()
		{
			Byte resultStrictModeActive = 0;

			CheckError(Internal.Lib3MFWrapper.Reader_GetStrictModeActive (Handle, out resultStrictModeActive));
			return (resultStrictModeActive != 0);
		}

		public String GetWarning (UInt32 AIndex, out UInt32 AErrorCode)
		{
			UInt32 sizeWarning = 0;
			UInt32 neededWarning = 0;
			CheckError(Internal.Lib3MFWrapper.Reader_GetWarning (Handle, AIndex, out AErrorCode, sizeWarning, out neededWarning, IntPtr.Zero));
			sizeWarning = neededWarning;
			byte[] bytesWarning = new byte[sizeWarning];
			GCHandle dataWarning = GCHandle.Alloc(bytesWarning, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Reader_GetWarning (Handle, AIndex, out AErrorCode, sizeWarning, out neededWarning, dataWarning.AddrOfPinnedObject()));
			dataWarning.Free();
			return Encoding.UTF8.GetString(bytesWarning).TrimEnd(char.MinValue);
		}

		public UInt32 GetWarningCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.Reader_GetWarningCount (Handle, out resultCount));
			return resultCount;
		}

		public void AddKeyWrappingCallback (String AConsumerID, IntPtr ATheCallback, UInt64 AUserData)
		{
			byte[] byteConsumerID = Encoding.UTF8.GetBytes(AConsumerID + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Reader_AddKeyWrappingCallback (Handle, byteConsumerID, IntPtr.Zero, AUserData));
		}

		public void SetContentEncryptionCallback (IntPtr ATheCallback, UInt64 AUserData)
		{

			CheckError(Internal.Lib3MFWrapper.Reader_SetContentEncryptionCallback (Handle, IntPtr.Zero, AUserData));
		}

	}

	class CPackagePart : CBase
	{
		public CPackagePart (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public String GetPath ()
		{
			UInt32 sizePath = 0;
			UInt32 neededPath = 0;
			CheckError(Internal.Lib3MFWrapper.PackagePart_GetPath (Handle, sizePath, out neededPath, IntPtr.Zero));
			sizePath = neededPath;
			byte[] bytesPath = new byte[sizePath];
			GCHandle dataPath = GCHandle.Alloc(bytesPath, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.PackagePart_GetPath (Handle, sizePath, out neededPath, dataPath.AddrOfPinnedObject()));
			dataPath.Free();
			return Encoding.UTF8.GetString(bytesPath).TrimEnd(char.MinValue);
		}

		public void SetPath (String APath)
		{
			byte[] bytePath = Encoding.UTF8.GetBytes(APath + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.PackagePart_SetPath (Handle, bytePath));
		}

	}

	class CResource : CBase
	{
		public CResource (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public UInt32 GetResourceID ()
		{
			UInt32 resultUniqueResourceID = 0;

			CheckError(Internal.Lib3MFWrapper.Resource_GetResourceID (Handle, out resultUniqueResourceID));
			return resultUniqueResourceID;
		}

		public UInt32 GetUniqueResourceID ()
		{
			UInt32 resultUniqueResourceID = 0;

			CheckError(Internal.Lib3MFWrapper.Resource_GetUniqueResourceID (Handle, out resultUniqueResourceID));
			return resultUniqueResourceID;
		}

		public CPackagePart PackagePart ()
		{
			IntPtr newPackagePart = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Resource_PackagePart (Handle, out newPackagePart));
			return new CPackagePart (newPackagePart );
		}

		public void SetPackagePart (CPackagePart APackagePart)
		{

			CheckError(Internal.Lib3MFWrapper.Resource_SetPackagePart (Handle, APackagePart.GetHandle()));
		}

		public UInt32 GetModelResourceID ()
		{
			UInt32 resultModelResourceId = 0;

			CheckError(Internal.Lib3MFWrapper.Resource_GetModelResourceID (Handle, out resultModelResourceId));
			return resultModelResourceId;
		}

	}

	class CResourceIterator : CBase
	{
		public CResourceIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public bool MoveNext ()
		{
			Byte resultHasNext = 0;

			CheckError(Internal.Lib3MFWrapper.ResourceIterator_MoveNext (Handle, out resultHasNext));
			return (resultHasNext != 0);
		}

		public bool MovePrevious ()
		{
			Byte resultHasPrevious = 0;

			CheckError(Internal.Lib3MFWrapper.ResourceIterator_MovePrevious (Handle, out resultHasPrevious));
			return (resultHasPrevious != 0);
		}

		public CResource GetCurrent ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ResourceIterator_GetCurrent (Handle, out newResource));
			return new CResource (newResource );
		}

		public CResourceIterator Clone ()
		{
			IntPtr newOutResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ResourceIterator_Clone (Handle, out newOutResourceIterator));
			return new CResourceIterator (newOutResourceIterator );
		}

		public UInt64 Count ()
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.ResourceIterator_Count (Handle, out resultCount));
			return resultCount;
		}

	}

	class CSliceStackIterator : CResourceIterator
	{
		public CSliceStackIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CSliceStack GetCurrentSliceStack ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.SliceStackIterator_GetCurrentSliceStack (Handle, out newResource));
			return new CSliceStack (newResource );
		}

	}

	class CObjectIterator : CResourceIterator
	{
		public CObjectIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CObject GetCurrentObject ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ObjectIterator_GetCurrentObject (Handle, out newResource));
			return new CObject (newResource );
		}

	}

	class CMeshObjectIterator : CResourceIterator
	{
		public CMeshObjectIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CMeshObject GetCurrentMeshObject ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.MeshObjectIterator_GetCurrentMeshObject (Handle, out newResource));
			return new CMeshObject (newResource );
		}

	}

	class CComponentsObjectIterator : CResourceIterator
	{
		public CComponentsObjectIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CComponentsObject GetCurrentComponentsObject ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ComponentsObjectIterator_GetCurrentComponentsObject (Handle, out newResource));
			return new CComponentsObject (newResource );
		}

	}

	class CTexture2DIterator : CResourceIterator
	{
		public CTexture2DIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CTexture2D GetCurrentTexture2D ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Texture2DIterator_GetCurrentTexture2D (Handle, out newResource));
			return new CTexture2D (newResource );
		}

	}

	class CBaseMaterialGroupIterator : CResourceIterator
	{
		public CBaseMaterialGroupIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CBaseMaterialGroup GetCurrentBaseMaterialGroup ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroupIterator_GetCurrentBaseMaterialGroup (Handle, out newResource));
			return new CBaseMaterialGroup (newResource );
		}

	}

	class CColorGroupIterator : CResourceIterator
	{
		public CColorGroupIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CColorGroup GetCurrentColorGroup ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ColorGroupIterator_GetCurrentColorGroup (Handle, out newResource));
			return new CColorGroup (newResource );
		}

	}

	class CTexture2DGroupIterator : CResourceIterator
	{
		public CTexture2DGroupIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CTexture2DGroup GetCurrentTexture2DGroup ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Texture2DGroupIterator_GetCurrentTexture2DGroup (Handle, out newResource));
			return new CTexture2DGroup (newResource );
		}

	}

	class CCompositeMaterialsIterator : CResourceIterator
	{
		public CCompositeMaterialsIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CCompositeMaterials GetCurrentCompositeMaterials ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.CompositeMaterialsIterator_GetCurrentCompositeMaterials (Handle, out newResource));
			return new CCompositeMaterials (newResource );
		}

	}

	class CMultiPropertyGroupIterator : CResourceIterator
	{
		public CMultiPropertyGroupIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CMultiPropertyGroup GetCurrentMultiPropertyGroup ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroupIterator_GetCurrentMultiPropertyGroup (Handle, out newResource));
			return new CMultiPropertyGroup (newResource );
		}

	}

	class CImage3DIterator : CResourceIterator
	{
		public CImage3DIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CImage3D GetCurrentImage3D ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Image3DIterator_GetCurrentImage3D (Handle, out newResource));
			return new CImage3D (newResource );
		}

	}

	class CVolumetricStackIterator : CResourceIterator
	{
		public CVolumetricStackIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CVolumetricStack GetCurrentVolumetricStack ()
		{
			IntPtr newResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumetricStackIterator_GetCurrentVolumetricStack (Handle, out newResource));
			return new CVolumetricStack (newResource );
		}

	}

	class CMetaData : CBase
	{
		public CMetaData (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public String GetNameSpace ()
		{
			UInt32 sizeNameSpace = 0;
			UInt32 neededNameSpace = 0;
			CheckError(Internal.Lib3MFWrapper.MetaData_GetNameSpace (Handle, sizeNameSpace, out neededNameSpace, IntPtr.Zero));
			sizeNameSpace = neededNameSpace;
			byte[] bytesNameSpace = new byte[sizeNameSpace];
			GCHandle dataNameSpace = GCHandle.Alloc(bytesNameSpace, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MetaData_GetNameSpace (Handle, sizeNameSpace, out neededNameSpace, dataNameSpace.AddrOfPinnedObject()));
			dataNameSpace.Free();
			return Encoding.UTF8.GetString(bytesNameSpace).TrimEnd(char.MinValue);
		}

		public void SetNameSpace (String ANameSpace)
		{
			byte[] byteNameSpace = Encoding.UTF8.GetBytes(ANameSpace + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.MetaData_SetNameSpace (Handle, byteNameSpace));
		}

		public String GetName ()
		{
			UInt32 sizeName = 0;
			UInt32 neededName = 0;
			CheckError(Internal.Lib3MFWrapper.MetaData_GetName (Handle, sizeName, out neededName, IntPtr.Zero));
			sizeName = neededName;
			byte[] bytesName = new byte[sizeName];
			GCHandle dataName = GCHandle.Alloc(bytesName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MetaData_GetName (Handle, sizeName, out neededName, dataName.AddrOfPinnedObject()));
			dataName.Free();
			return Encoding.UTF8.GetString(bytesName).TrimEnd(char.MinValue);
		}

		public void SetName (String AName)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.MetaData_SetName (Handle, byteName));
		}

		public String GetKey ()
		{
			UInt32 sizeKey = 0;
			UInt32 neededKey = 0;
			CheckError(Internal.Lib3MFWrapper.MetaData_GetKey (Handle, sizeKey, out neededKey, IntPtr.Zero));
			sizeKey = neededKey;
			byte[] bytesKey = new byte[sizeKey];
			GCHandle dataKey = GCHandle.Alloc(bytesKey, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MetaData_GetKey (Handle, sizeKey, out neededKey, dataKey.AddrOfPinnedObject()));
			dataKey.Free();
			return Encoding.UTF8.GetString(bytesKey).TrimEnd(char.MinValue);
		}

		public bool GetMustPreserve ()
		{
			Byte resultMustPreserve = 0;

			CheckError(Internal.Lib3MFWrapper.MetaData_GetMustPreserve (Handle, out resultMustPreserve));
			return (resultMustPreserve != 0);
		}

		public void SetMustPreserve (bool AMustPreserve)
		{

			CheckError(Internal.Lib3MFWrapper.MetaData_SetMustPreserve (Handle, (Byte)( AMustPreserve ? 1 : 0 )));
		}

		public String GetType ()
		{
			UInt32 sizeType = 0;
			UInt32 neededType = 0;
			CheckError(Internal.Lib3MFWrapper.MetaData_GetType (Handle, sizeType, out neededType, IntPtr.Zero));
			sizeType = neededType;
			byte[] bytesType = new byte[sizeType];
			GCHandle dataType = GCHandle.Alloc(bytesType, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MetaData_GetType (Handle, sizeType, out neededType, dataType.AddrOfPinnedObject()));
			dataType.Free();
			return Encoding.UTF8.GetString(bytesType).TrimEnd(char.MinValue);
		}

		public void SetType (String AType)
		{
			byte[] byteType = Encoding.UTF8.GetBytes(AType + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.MetaData_SetType (Handle, byteType));
		}

		public String GetValue ()
		{
			UInt32 sizeValue = 0;
			UInt32 neededValue = 0;
			CheckError(Internal.Lib3MFWrapper.MetaData_GetValue (Handle, sizeValue, out neededValue, IntPtr.Zero));
			sizeValue = neededValue;
			byte[] bytesValue = new byte[sizeValue];
			GCHandle dataValue = GCHandle.Alloc(bytesValue, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MetaData_GetValue (Handle, sizeValue, out neededValue, dataValue.AddrOfPinnedObject()));
			dataValue.Free();
			return Encoding.UTF8.GetString(bytesValue).TrimEnd(char.MinValue);
		}

		public void SetValue (String AValue)
		{
			byte[] byteValue = Encoding.UTF8.GetBytes(AValue + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.MetaData_SetValue (Handle, byteValue));
		}

	}

	class CMetaDataGroup : CBase
	{
		public CMetaDataGroup (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public UInt32 GetMetaDataCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.MetaDataGroup_GetMetaDataCount (Handle, out resultCount));
			return resultCount;
		}

		public CMetaData GetMetaData (UInt32 AIndex)
		{
			IntPtr newMetaData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.MetaDataGroup_GetMetaData (Handle, AIndex, out newMetaData));
			return new CMetaData (newMetaData );
		}

		public CMetaData GetMetaDataByKey (String ANameSpace, String AName)
		{
			byte[] byteNameSpace = Encoding.UTF8.GetBytes(ANameSpace + char.MinValue);
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);
			IntPtr newMetaData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.MetaDataGroup_GetMetaDataByKey (Handle, byteNameSpace, byteName, out newMetaData));
			return new CMetaData (newMetaData );
		}

		public void RemoveMetaDataByIndex (UInt32 AIndex)
		{

			CheckError(Internal.Lib3MFWrapper.MetaDataGroup_RemoveMetaDataByIndex (Handle, AIndex));
		}

		public void RemoveMetaData (CMetaData ATheMetaData)
		{

			CheckError(Internal.Lib3MFWrapper.MetaDataGroup_RemoveMetaData (Handle, ATheMetaData.GetHandle()));
		}

		public CMetaData AddMetaData (String ANameSpace, String AName, String AValue, String AType, bool AMustPreserve)
		{
			byte[] byteNameSpace = Encoding.UTF8.GetBytes(ANameSpace + char.MinValue);
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);
			byte[] byteValue = Encoding.UTF8.GetBytes(AValue + char.MinValue);
			byte[] byteType = Encoding.UTF8.GetBytes(AType + char.MinValue);
			IntPtr newMetaData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.MetaDataGroup_AddMetaData (Handle, byteNameSpace, byteName, byteValue, byteType, (Byte)( AMustPreserve ? 1 : 0 ), out newMetaData));
			return new CMetaData (newMetaData );
		}

	}

	class CObject : CResource
	{
		public CObject (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public eObjectType GetType ()
		{
			Int32 resultObjectType = 0;

			CheckError(Internal.Lib3MFWrapper.Object_GetType (Handle, out resultObjectType));
			return (eObjectType) (resultObjectType);
		}

		public void SetType (eObjectType AObjectType)
		{
			Int32 enumObjectType = (Int32) AObjectType;

			CheckError(Internal.Lib3MFWrapper.Object_SetType (Handle, enumObjectType));
		}

		public String GetName ()
		{
			UInt32 sizeName = 0;
			UInt32 neededName = 0;
			CheckError(Internal.Lib3MFWrapper.Object_GetName (Handle, sizeName, out neededName, IntPtr.Zero));
			sizeName = neededName;
			byte[] bytesName = new byte[sizeName];
			GCHandle dataName = GCHandle.Alloc(bytesName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Object_GetName (Handle, sizeName, out neededName, dataName.AddrOfPinnedObject()));
			dataName.Free();
			return Encoding.UTF8.GetString(bytesName).TrimEnd(char.MinValue);
		}

		public void SetName (String AName)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Object_SetName (Handle, byteName));
		}

		public String GetPartNumber ()
		{
			UInt32 sizePartNumber = 0;
			UInt32 neededPartNumber = 0;
			CheckError(Internal.Lib3MFWrapper.Object_GetPartNumber (Handle, sizePartNumber, out neededPartNumber, IntPtr.Zero));
			sizePartNumber = neededPartNumber;
			byte[] bytesPartNumber = new byte[sizePartNumber];
			GCHandle dataPartNumber = GCHandle.Alloc(bytesPartNumber, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Object_GetPartNumber (Handle, sizePartNumber, out neededPartNumber, dataPartNumber.AddrOfPinnedObject()));
			dataPartNumber.Free();
			return Encoding.UTF8.GetString(bytesPartNumber).TrimEnd(char.MinValue);
		}

		public void SetPartNumber (String APartNumber)
		{
			byte[] bytePartNumber = Encoding.UTF8.GetBytes(APartNumber + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Object_SetPartNumber (Handle, bytePartNumber));
		}

		public bool IsMeshObject ()
		{
			Byte resultIsMeshObject = 0;

			CheckError(Internal.Lib3MFWrapper.Object_IsMeshObject (Handle, out resultIsMeshObject));
			return (resultIsMeshObject != 0);
		}

		public bool IsComponentsObject ()
		{
			Byte resultIsComponentsObject = 0;

			CheckError(Internal.Lib3MFWrapper.Object_IsComponentsObject (Handle, out resultIsComponentsObject));
			return (resultIsComponentsObject != 0);
		}

		public bool IsValid ()
		{
			Byte resultIsValid = 0;

			CheckError(Internal.Lib3MFWrapper.Object_IsValid (Handle, out resultIsValid));
			return (resultIsValid != 0);
		}

		public void SetAttachmentAsThumbnail (CAttachment AAttachment)
		{

			CheckError(Internal.Lib3MFWrapper.Object_SetAttachmentAsThumbnail (Handle, AAttachment.GetHandle()));
		}

		public CAttachment GetThumbnailAttachment ()
		{
			IntPtr newAttachment = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Object_GetThumbnailAttachment (Handle, out newAttachment));
			return new CAttachment (newAttachment );
		}

		public void ClearThumbnailAttachment ()
		{

			CheckError(Internal.Lib3MFWrapper.Object_ClearThumbnailAttachment (Handle));
		}

		public sBox GetOutbox ()
		{
			Internal.InternalBox intresultOutbox;

			CheckError(Internal.Lib3MFWrapper.Object_GetOutbox (Handle, out intresultOutbox));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Box (intresultOutbox);
		}

		public String GetUUID (out bool AHasUUID)
		{
			Byte resultHasUUID = 0;
			UInt32 sizeUUID = 0;
			UInt32 neededUUID = 0;
			CheckError(Internal.Lib3MFWrapper.Object_GetUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, IntPtr.Zero));
			sizeUUID = neededUUID;
			byte[] bytesUUID = new byte[sizeUUID];
			GCHandle dataUUID = GCHandle.Alloc(bytesUUID, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Object_GetUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, dataUUID.AddrOfPinnedObject()));
			AHasUUID = (resultHasUUID != 0);
			dataUUID.Free();
			return Encoding.UTF8.GetString(bytesUUID).TrimEnd(char.MinValue);
		}

		public void SetUUID (String AUUID)
		{
			byte[] byteUUID = Encoding.UTF8.GetBytes(AUUID + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Object_SetUUID (Handle, byteUUID));
		}

		public CMetaDataGroup GetMetaDataGroup ()
		{
			IntPtr newMetaDataGroup = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Object_GetMetaDataGroup (Handle, out newMetaDataGroup));
			return new CMetaDataGroup (newMetaDataGroup );
		}

		public void SetSlicesMeshResolution (eSlicesMeshResolution AMeshResolution)
		{
			Int32 enumMeshResolution = (Int32) AMeshResolution;

			CheckError(Internal.Lib3MFWrapper.Object_SetSlicesMeshResolution (Handle, enumMeshResolution));
		}

		public eSlicesMeshResolution GetSlicesMeshResolution ()
		{
			Int32 resultMeshResolution = 0;

			CheckError(Internal.Lib3MFWrapper.Object_GetSlicesMeshResolution (Handle, out resultMeshResolution));
			return (eSlicesMeshResolution) (resultMeshResolution);
		}

		public bool HasSlices (bool ARecursive)
		{
			Byte resultHasSlices = 0;

			CheckError(Internal.Lib3MFWrapper.Object_HasSlices (Handle, (Byte)( ARecursive ? 1 : 0 ), out resultHasSlices));
			return (resultHasSlices != 0);
		}

		public void ClearSliceStack ()
		{

			CheckError(Internal.Lib3MFWrapper.Object_ClearSliceStack (Handle));
		}

		public CSliceStack GetSliceStack ()
		{
			IntPtr newSliceStackInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Object_GetSliceStack (Handle, out newSliceStackInstance));
			return new CSliceStack (newSliceStackInstance );
		}

		public void AssignSliceStack (CSliceStack ASliceStackInstance)
		{

			CheckError(Internal.Lib3MFWrapper.Object_AssignSliceStack (Handle, ASliceStackInstance.GetHandle()));
		}

	}

	class CMeshObject : CObject
	{
		public CMeshObject (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public UInt32 GetVertexCount ()
		{
			UInt32 resultVertexCount = 0;

			CheckError(Internal.Lib3MFWrapper.MeshObject_GetVertexCount (Handle, out resultVertexCount));
			return resultVertexCount;
		}

		public UInt32 GetTriangleCount ()
		{
			UInt32 resultVertexCount = 0;

			CheckError(Internal.Lib3MFWrapper.MeshObject_GetTriangleCount (Handle, out resultVertexCount));
			return resultVertexCount;
		}

		public sPosition GetVertex (UInt32 AIndex)
		{
			Internal.InternalPosition intresultCoordinates;

			CheckError(Internal.Lib3MFWrapper.MeshObject_GetVertex (Handle, AIndex, out intresultCoordinates));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Position (intresultCoordinates);
		}

		public void SetVertex (UInt32 AIndex, sPosition ACoordinates)
		{
			Internal.InternalPosition intCoordinates = Internal.Lib3MFWrapper.convertStructToInternal_Position (ACoordinates);

			CheckError(Internal.Lib3MFWrapper.MeshObject_SetVertex (Handle, AIndex, intCoordinates));
		}

		public UInt32 AddVertex (sPosition ACoordinates)
		{
			Internal.InternalPosition intCoordinates = Internal.Lib3MFWrapper.convertStructToInternal_Position (ACoordinates);
			UInt32 resultNewIndex = 0;

			CheckError(Internal.Lib3MFWrapper.MeshObject_AddVertex (Handle, intCoordinates, out resultNewIndex));
			return resultNewIndex;
		}

		public void GetVertices (out sPosition[] AVertices)
		{
			UInt64 sizeVertices = 0;
			UInt64 neededVertices = 0;
			CheckError(Internal.Lib3MFWrapper.MeshObject_GetVertices (Handle, sizeVertices, out neededVertices, IntPtr.Zero));
			sizeVertices = neededVertices;
			var arrayVertices = new Internal.InternalPosition[sizeVertices];
			GCHandle dataVertices = GCHandle.Alloc(arrayVertices, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MeshObject_GetVertices (Handle, sizeVertices, out neededVertices, dataVertices.AddrOfPinnedObject()));
			dataVertices.Free();
			AVertices = new sPosition[sizeVertices];
			for (int index = 0; index < AVertices.Length; index++)
				AVertices[index] = Internal.Lib3MFWrapper.convertInternalToStruct_Position(arrayVertices[index]);
		}

		public sTriangle GetTriangle (UInt32 AIndex)
		{
			Internal.InternalTriangle intresultIndices;

			CheckError(Internal.Lib3MFWrapper.MeshObject_GetTriangle (Handle, AIndex, out intresultIndices));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Triangle (intresultIndices);
		}

		public void SetTriangle (UInt32 AIndex, sTriangle AIndices)
		{
			Internal.InternalTriangle intIndices = Internal.Lib3MFWrapper.convertStructToInternal_Triangle (AIndices);

			CheckError(Internal.Lib3MFWrapper.MeshObject_SetTriangle (Handle, AIndex, intIndices));
		}

		public UInt32 AddTriangle (sTriangle AIndices)
		{
			Internal.InternalTriangle intIndices = Internal.Lib3MFWrapper.convertStructToInternal_Triangle (AIndices);
			UInt32 resultNewIndex = 0;

			CheckError(Internal.Lib3MFWrapper.MeshObject_AddTriangle (Handle, intIndices, out resultNewIndex));
			return resultNewIndex;
		}

		public void GetTriangleIndices (out sTriangle[] AIndices)
		{
			UInt64 sizeIndices = 0;
			UInt64 neededIndices = 0;
			CheckError(Internal.Lib3MFWrapper.MeshObject_GetTriangleIndices (Handle, sizeIndices, out neededIndices, IntPtr.Zero));
			sizeIndices = neededIndices;
			var arrayIndices = new Internal.InternalTriangle[sizeIndices];
			GCHandle dataIndices = GCHandle.Alloc(arrayIndices, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MeshObject_GetTriangleIndices (Handle, sizeIndices, out neededIndices, dataIndices.AddrOfPinnedObject()));
			dataIndices.Free();
			AIndices = new sTriangle[sizeIndices];
			for (int index = 0; index < AIndices.Length; index++)
				AIndices[index] = Internal.Lib3MFWrapper.convertInternalToStruct_Triangle(arrayIndices[index]);
		}

		public void SetObjectLevelProperty (UInt32 AUniqueResourceID, UInt32 APropertyID)
		{

			CheckError(Internal.Lib3MFWrapper.MeshObject_SetObjectLevelProperty (Handle, AUniqueResourceID, APropertyID));
		}

		public bool GetObjectLevelProperty (out UInt32 AUniqueResourceID, out UInt32 APropertyID)
		{
			Byte resultHasObjectLevelProperty = 0;

			CheckError(Internal.Lib3MFWrapper.MeshObject_GetObjectLevelProperty (Handle, out AUniqueResourceID, out APropertyID, out resultHasObjectLevelProperty));
			return (resultHasObjectLevelProperty != 0);
		}

		public void SetTriangleProperties (UInt32 AIndex, sTriangleProperties AProperties)
		{
			Internal.InternalTriangleProperties intProperties = Internal.Lib3MFWrapper.convertStructToInternal_TriangleProperties (AProperties);

			CheckError(Internal.Lib3MFWrapper.MeshObject_SetTriangleProperties (Handle, AIndex, intProperties));
		}

		public void GetTriangleProperties (UInt32 AIndex, out sTriangleProperties AProperty)
		{
			Internal.InternalTriangleProperties intresultProperty;

			CheckError(Internal.Lib3MFWrapper.MeshObject_GetTriangleProperties (Handle, AIndex, out intresultProperty));
			AProperty = Internal.Lib3MFWrapper.convertInternalToStruct_TriangleProperties (intresultProperty);
		}

		public void SetAllTriangleProperties (sTriangleProperties[] APropertiesArray)
		{
			Internal.InternalTriangleProperties[] intdataPropertiesArray = new Internal.InternalTriangleProperties[APropertiesArray.Length];
			for (int index = 0; index < APropertiesArray.Length; index++)
				intdataPropertiesArray[index] = Internal.Lib3MFWrapper.convertStructToInternal_TriangleProperties(APropertiesArray[index]);
			GCHandle dataPropertiesArray = GCHandle.Alloc(intdataPropertiesArray, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MeshObject_SetAllTriangleProperties (Handle, (UInt64) APropertiesArray.Length, dataPropertiesArray.AddrOfPinnedObject()));
			dataPropertiesArray.Free ();
		}

		public void GetAllTriangleProperties (out sTriangleProperties[] APropertiesArray)
		{
			UInt64 sizePropertiesArray = 0;
			UInt64 neededPropertiesArray = 0;
			CheckError(Internal.Lib3MFWrapper.MeshObject_GetAllTriangleProperties (Handle, sizePropertiesArray, out neededPropertiesArray, IntPtr.Zero));
			sizePropertiesArray = neededPropertiesArray;
			var arrayPropertiesArray = new Internal.InternalTriangleProperties[sizePropertiesArray];
			GCHandle dataPropertiesArray = GCHandle.Alloc(arrayPropertiesArray, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MeshObject_GetAllTriangleProperties (Handle, sizePropertiesArray, out neededPropertiesArray, dataPropertiesArray.AddrOfPinnedObject()));
			dataPropertiesArray.Free();
			APropertiesArray = new sTriangleProperties[sizePropertiesArray];
			for (int index = 0; index < APropertiesArray.Length; index++)
				APropertiesArray[index] = Internal.Lib3MFWrapper.convertInternalToStruct_TriangleProperties(arrayPropertiesArray[index]);
		}

		public void ClearAllProperties ()
		{

			CheckError(Internal.Lib3MFWrapper.MeshObject_ClearAllProperties (Handle));
		}

		public void SetGeometry (sPosition[] AVertices, sTriangle[] AIndices)
		{
			Internal.InternalPosition[] intdataVertices = new Internal.InternalPosition[AVertices.Length];
			for (int index = 0; index < AVertices.Length; index++)
				intdataVertices[index] = Internal.Lib3MFWrapper.convertStructToInternal_Position(AVertices[index]);
			GCHandle dataVertices = GCHandle.Alloc(intdataVertices, GCHandleType.Pinned);
			Internal.InternalTriangle[] intdataIndices = new Internal.InternalTriangle[AIndices.Length];
			for (int index = 0; index < AIndices.Length; index++)
				intdataIndices[index] = Internal.Lib3MFWrapper.convertStructToInternal_Triangle(AIndices[index]);
			GCHandle dataIndices = GCHandle.Alloc(intdataIndices, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MeshObject_SetGeometry (Handle, (UInt64) AVertices.Length, dataVertices.AddrOfPinnedObject(), (UInt64) AIndices.Length, dataIndices.AddrOfPinnedObject()));
			dataVertices.Free ();
			dataIndices.Free ();
		}

		public bool IsManifoldAndOriented ()
		{
			Byte resultIsManifoldAndOriented = 0;

			CheckError(Internal.Lib3MFWrapper.MeshObject_IsManifoldAndOriented (Handle, out resultIsManifoldAndOriented));
			return (resultIsManifoldAndOriented != 0);
		}

		public CBeamLattice BeamLattice ()
		{
			IntPtr newTheBeamLattice = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.MeshObject_BeamLattice (Handle, out newTheBeamLattice));
			return new CBeamLattice (newTheBeamLattice );
		}

		public CVolumeData VolumeData ()
		{
			IntPtr newTheVolumeData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.MeshObject_VolumeData (Handle, out newTheVolumeData));
			return new CVolumeData (newTheVolumeData );
		}

	}

	class CBeamLattice : CBase
	{
		public CBeamLattice (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public Double GetMinLength ()
		{
			Double resultMinLength = 0;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetMinLength (Handle, out resultMinLength));
			return resultMinLength;
		}

		public void SetMinLength (Double AMinLength)
		{

			CheckError(Internal.Lib3MFWrapper.BeamLattice_SetMinLength (Handle, AMinLength));
		}

		public void GetClipping (out eBeamLatticeClipMode AClipMode, out UInt32 AUniqueResourceID)
		{
			Int32 resultClipMode = 0;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetClipping (Handle, out resultClipMode, out AUniqueResourceID));
			AClipMode = (eBeamLatticeClipMode) (resultClipMode);
		}

		public void SetClipping (eBeamLatticeClipMode AClipMode, UInt32 AUniqueResourceID)
		{
			Int32 enumClipMode = (Int32) AClipMode;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_SetClipping (Handle, enumClipMode, AUniqueResourceID));
		}

		public bool GetRepresentation (out UInt32 AUniqueResourceID)
		{
			Byte resultHasRepresentation = 0;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetRepresentation (Handle, out resultHasRepresentation, out AUniqueResourceID));
			return (resultHasRepresentation != 0);
		}

		public void SetRepresentation (UInt32 AUniqueResourceID)
		{

			CheckError(Internal.Lib3MFWrapper.BeamLattice_SetRepresentation (Handle, AUniqueResourceID));
		}

		public void GetBallOptions (out eBeamLatticeBallMode ABallMode, out Double ABallRadius)
		{
			Int32 resultBallMode = 0;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBallOptions (Handle, out resultBallMode, out ABallRadius));
			ABallMode = (eBeamLatticeBallMode) (resultBallMode);
		}

		public void SetBallOptions (eBeamLatticeBallMode ABallMode, Double ABallRadius)
		{
			Int32 enumBallMode = (Int32) ABallMode;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_SetBallOptions (Handle, enumBallMode, ABallRadius));
		}

		public UInt32 GetBeamCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBeamCount (Handle, out resultCount));
			return resultCount;
		}

		public sBeam GetBeam (UInt32 AIndex)
		{
			Internal.InternalBeam intresultBeamInfo;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBeam (Handle, AIndex, out intresultBeamInfo));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Beam (intresultBeamInfo);
		}

		public UInt32 AddBeam (sBeam ABeamInfo)
		{
			Internal.InternalBeam intBeamInfo = Internal.Lib3MFWrapper.convertStructToInternal_Beam (ABeamInfo);
			UInt32 resultIndex = 0;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_AddBeam (Handle, intBeamInfo, out resultIndex));
			return resultIndex;
		}

		public void SetBeam (UInt32 AIndex, sBeam ABeamInfo)
		{
			Internal.InternalBeam intBeamInfo = Internal.Lib3MFWrapper.convertStructToInternal_Beam (ABeamInfo);

			CheckError(Internal.Lib3MFWrapper.BeamLattice_SetBeam (Handle, AIndex, intBeamInfo));
		}

		public void SetBeams (sBeam[] ABeamInfo)
		{
			Internal.InternalBeam[] intdataBeamInfo = new Internal.InternalBeam[ABeamInfo.Length];
			for (int index = 0; index < ABeamInfo.Length; index++)
				intdataBeamInfo[index] = Internal.Lib3MFWrapper.convertStructToInternal_Beam(ABeamInfo[index]);
			GCHandle dataBeamInfo = GCHandle.Alloc(intdataBeamInfo, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamLattice_SetBeams (Handle, (UInt64) ABeamInfo.Length, dataBeamInfo.AddrOfPinnedObject()));
			dataBeamInfo.Free ();
		}

		public void GetBeams (out sBeam[] ABeamInfo)
		{
			UInt64 sizeBeamInfo = 0;
			UInt64 neededBeamInfo = 0;
			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBeams (Handle, sizeBeamInfo, out neededBeamInfo, IntPtr.Zero));
			sizeBeamInfo = neededBeamInfo;
			var arrayBeamInfo = new Internal.InternalBeam[sizeBeamInfo];
			GCHandle dataBeamInfo = GCHandle.Alloc(arrayBeamInfo, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBeams (Handle, sizeBeamInfo, out neededBeamInfo, dataBeamInfo.AddrOfPinnedObject()));
			dataBeamInfo.Free();
			ABeamInfo = new sBeam[sizeBeamInfo];
			for (int index = 0; index < ABeamInfo.Length; index++)
				ABeamInfo[index] = Internal.Lib3MFWrapper.convertInternalToStruct_Beam(arrayBeamInfo[index]);
		}

		public UInt32 GetBallCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBallCount (Handle, out resultCount));
			return resultCount;
		}

		public sBall GetBall (UInt32 AIndex)
		{
			Internal.InternalBall intresultBallInfo;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBall (Handle, AIndex, out intresultBallInfo));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Ball (intresultBallInfo);
		}

		public UInt32 AddBall (sBall ABallInfo)
		{
			Internal.InternalBall intBallInfo = Internal.Lib3MFWrapper.convertStructToInternal_Ball (ABallInfo);
			UInt32 resultIndex = 0;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_AddBall (Handle, intBallInfo, out resultIndex));
			return resultIndex;
		}

		public void SetBall (UInt32 AIndex, sBall ABallInfo)
		{
			Internal.InternalBall intBallInfo = Internal.Lib3MFWrapper.convertStructToInternal_Ball (ABallInfo);

			CheckError(Internal.Lib3MFWrapper.BeamLattice_SetBall (Handle, AIndex, intBallInfo));
		}

		public void SetBalls (sBall[] ABallInfo)
		{
			Internal.InternalBall[] intdataBallInfo = new Internal.InternalBall[ABallInfo.Length];
			for (int index = 0; index < ABallInfo.Length; index++)
				intdataBallInfo[index] = Internal.Lib3MFWrapper.convertStructToInternal_Ball(ABallInfo[index]);
			GCHandle dataBallInfo = GCHandle.Alloc(intdataBallInfo, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamLattice_SetBalls (Handle, (UInt64) ABallInfo.Length, dataBallInfo.AddrOfPinnedObject()));
			dataBallInfo.Free ();
		}

		public void GetBalls (out sBall[] ABallInfo)
		{
			UInt64 sizeBallInfo = 0;
			UInt64 neededBallInfo = 0;
			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBalls (Handle, sizeBallInfo, out neededBallInfo, IntPtr.Zero));
			sizeBallInfo = neededBallInfo;
			var arrayBallInfo = new Internal.InternalBall[sizeBallInfo];
			GCHandle dataBallInfo = GCHandle.Alloc(arrayBallInfo, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBalls (Handle, sizeBallInfo, out neededBallInfo, dataBallInfo.AddrOfPinnedObject()));
			dataBallInfo.Free();
			ABallInfo = new sBall[sizeBallInfo];
			for (int index = 0; index < ABallInfo.Length; index++)
				ABallInfo[index] = Internal.Lib3MFWrapper.convertInternalToStruct_Ball(arrayBallInfo[index]);
		}

		public UInt32 GetBeamSetCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBeamSetCount (Handle, out resultCount));
			return resultCount;
		}

		public CBeamSet AddBeamSet ()
		{
			IntPtr newBeamSet = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_AddBeamSet (Handle, out newBeamSet));
			return new CBeamSet (newBeamSet );
		}

		public CBeamSet GetBeamSet (UInt32 AIndex)
		{
			IntPtr newBeamSet = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.BeamLattice_GetBeamSet (Handle, AIndex, out newBeamSet));
			return new CBeamSet (newBeamSet );
		}

	}

	class CVolumeDataItem : CBase
	{
		public CVolumeDataItem (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CVolumetricStack GetVolumetricStack ()
		{
			IntPtr newTheVolumetricStack = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeDataItem_GetVolumetricStack (Handle, out newTheVolumetricStack));
			return new CVolumetricStack (newTheVolumetricStack );
		}

		public void SetVolumetricStack (CVolumetricStack ATheVolumetricStack)
		{

			CheckError(Internal.Lib3MFWrapper.VolumeDataItem_SetVolumetricStack (Handle, ATheVolumetricStack.GetHandle()));
		}

		public sTransform GetTransform ()
		{
			Internal.InternalTransform intresultTransform;

			CheckError(Internal.Lib3MFWrapper.VolumeDataItem_GetTransform (Handle, out intresultTransform));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Transform (intresultTransform);
		}

		public void SetTransform (sTransform ATransform)
		{
			Internal.InternalTransform intTransform = Internal.Lib3MFWrapper.convertStructToInternal_Transform (ATransform);

			CheckError(Internal.Lib3MFWrapper.VolumeDataItem_SetTransform (Handle, intTransform));
		}

	}

	class CVolumeDataLevelset : CVolumeDataItem
	{
		public CVolumeDataLevelset (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public Double GetSolidThreshold ()
		{
			Double resultTheSolidThreshold = 0;

			CheckError(Internal.Lib3MFWrapper.VolumeDataLevelset_GetSolidThreshold (Handle, out resultTheSolidThreshold));
			return resultTheSolidThreshold;
		}

		public void SetSolidThreshold (Double ATheSolidThreshold)
		{

			CheckError(Internal.Lib3MFWrapper.VolumeDataLevelset_SetSolidThreshold (Handle, ATheSolidThreshold));
		}

		public void SetChannel (String AChannelName)
		{
			byte[] byteChannelName = Encoding.UTF8.GetBytes(AChannelName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.VolumeDataLevelset_SetChannel (Handle, byteChannelName));
		}

		public String GetChannel ()
		{
			UInt32 sizeChannelName = 0;
			UInt32 neededChannelName = 0;
			CheckError(Internal.Lib3MFWrapper.VolumeDataLevelset_GetChannel (Handle, sizeChannelName, out neededChannelName, IntPtr.Zero));
			sizeChannelName = neededChannelName;
			byte[] bytesChannelName = new byte[sizeChannelName];
			GCHandle dataChannelName = GCHandle.Alloc(bytesChannelName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.VolumeDataLevelset_GetChannel (Handle, sizeChannelName, out neededChannelName, dataChannelName.AddrOfPinnedObject()));
			dataChannelName.Free();
			return Encoding.UTF8.GetString(bytesChannelName).TrimEnd(char.MinValue);
		}

	}

	class CVolumeDataColor : CVolumeDataItem
	{
		public CVolumeDataColor (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public void SetChannel (eColorChannel ATheColorChannel, String AChannelName)
		{
			Int32 enumTheColorChannel = (Int32) ATheColorChannel;
			byte[] byteChannelName = Encoding.UTF8.GetBytes(AChannelName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.VolumeDataColor_SetChannel (Handle, enumTheColorChannel, byteChannelName));
		}

		public String GetChannel (eColorChannel ATheColorChannel)
		{
			Int32 enumTheColorChannel = (Int32) ATheColorChannel;
			UInt32 sizeChannelName = 0;
			UInt32 neededChannelName = 0;
			CheckError(Internal.Lib3MFWrapper.VolumeDataColor_GetChannel (Handle, enumTheColorChannel, sizeChannelName, out neededChannelName, IntPtr.Zero));
			sizeChannelName = neededChannelName;
			byte[] bytesChannelName = new byte[sizeChannelName];
			GCHandle dataChannelName = GCHandle.Alloc(bytesChannelName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.VolumeDataColor_GetChannel (Handle, enumTheColorChannel, sizeChannelName, out neededChannelName, dataChannelName.AddrOfPinnedObject()));
			dataChannelName.Free();
			return Encoding.UTF8.GetString(bytesChannelName).TrimEnd(char.MinValue);
		}

	}

	class CVolumeDataComposite : CVolumeDataItem
	{
		public CVolumeDataComposite (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CBaseMaterialGroup GetBaseMaterialGroup ()
		{
			IntPtr newBaseMaterialGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeDataComposite_GetBaseMaterialGroup (Handle, out newBaseMaterialGroupInstance));
			return new CBaseMaterialGroup (newBaseMaterialGroupInstance );
		}

		public void SetBaseMaterialGroup (CBaseMaterialGroup ABaseMaterialGroupInstance)
		{

			CheckError(Internal.Lib3MFWrapper.VolumeDataComposite_SetBaseMaterialGroup (Handle, ABaseMaterialGroupInstance.GetHandle()));
		}

		public UInt32 GetMaterialMappingCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.VolumeDataComposite_GetMaterialMappingCount (Handle, out resultCount));
			return resultCount;
		}

		public void GetMaterialMapping (UInt32 AIndex, out UInt32 APropertyID, out String AChannelName)
		{
			UInt32 sizeChannelName = 0;
			UInt32 neededChannelName = 0;
			CheckError(Internal.Lib3MFWrapper.VolumeDataComposite_GetMaterialMapping (Handle, AIndex, out APropertyID, sizeChannelName, out neededChannelName, IntPtr.Zero));
			sizeChannelName = neededChannelName;
			byte[] bytesChannelName = new byte[sizeChannelName];
			GCHandle dataChannelName = GCHandle.Alloc(bytesChannelName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.VolumeDataComposite_GetMaterialMapping (Handle, AIndex, out APropertyID, sizeChannelName, out neededChannelName, dataChannelName.AddrOfPinnedObject()));
			dataChannelName.Free();
			AChannelName = Encoding.UTF8.GetString(bytesChannelName).TrimEnd(char.MinValue);
		}

		public void SetMaterialMapping (UInt32 AIndex, out UInt32 APropertyID, out String AChannelName)
		{
			UInt32 sizeChannelName = 0;
			UInt32 neededChannelName = 0;
			CheckError(Internal.Lib3MFWrapper.VolumeDataComposite_SetMaterialMapping (Handle, AIndex, out APropertyID, sizeChannelName, out neededChannelName, IntPtr.Zero));
			sizeChannelName = neededChannelName;
			byte[] bytesChannelName = new byte[sizeChannelName];
			GCHandle dataChannelName = GCHandle.Alloc(bytesChannelName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.VolumeDataComposite_SetMaterialMapping (Handle, AIndex, out APropertyID, sizeChannelName, out neededChannelName, dataChannelName.AddrOfPinnedObject()));
			dataChannelName.Free();
			AChannelName = Encoding.UTF8.GetString(bytesChannelName).TrimEnd(char.MinValue);
		}

		public void AddMaterialMapping (UInt32 APropertyID, String AChannelName)
		{
			byte[] byteChannelName = Encoding.UTF8.GetBytes(AChannelName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.VolumeDataComposite_AddMaterialMapping (Handle, APropertyID, byteChannelName));
		}

		public void RemoveMaterialMapping (UInt32 AIndex)
		{

			CheckError(Internal.Lib3MFWrapper.VolumeDataComposite_RemoveMaterialMapping (Handle, AIndex));
		}

	}

	class CVolumeDataProperty : CVolumeDataItem
	{
		public CVolumeDataProperty (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public void SetChannel (String AChannelName)
		{
			byte[] byteChannelName = Encoding.UTF8.GetBytes(AChannelName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.VolumeDataProperty_SetChannel (Handle, byteChannelName));
		}

		public String GetChannel ()
		{
			UInt32 sizeChannelName = 0;
			UInt32 neededChannelName = 0;
			CheckError(Internal.Lib3MFWrapper.VolumeDataProperty_GetChannel (Handle, sizeChannelName, out neededChannelName, IntPtr.Zero));
			sizeChannelName = neededChannelName;
			byte[] bytesChannelName = new byte[sizeChannelName];
			GCHandle dataChannelName = GCHandle.Alloc(bytesChannelName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.VolumeDataProperty_GetChannel (Handle, sizeChannelName, out neededChannelName, dataChannelName.AddrOfPinnedObject()));
			dataChannelName.Free();
			return Encoding.UTF8.GetString(bytesChannelName).TrimEnd(char.MinValue);
		}

		public void SetName (String APropertyName)
		{
			byte[] bytePropertyName = Encoding.UTF8.GetBytes(APropertyName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.VolumeDataProperty_SetName (Handle, bytePropertyName));
		}

		public String GetName ()
		{
			UInt32 sizePropertyName = 0;
			UInt32 neededPropertyName = 0;
			CheckError(Internal.Lib3MFWrapper.VolumeDataProperty_GetName (Handle, sizePropertyName, out neededPropertyName, IntPtr.Zero));
			sizePropertyName = neededPropertyName;
			byte[] bytesPropertyName = new byte[sizePropertyName];
			GCHandle dataPropertyName = GCHandle.Alloc(bytesPropertyName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.VolumeDataProperty_GetName (Handle, sizePropertyName, out neededPropertyName, dataPropertyName.AddrOfPinnedObject()));
			dataPropertyName.Free();
			return Encoding.UTF8.GetString(bytesPropertyName).TrimEnd(char.MinValue);
		}

		public void SetIsRequired (bool AIsRequired)
		{

			CheckError(Internal.Lib3MFWrapper.VolumeDataProperty_SetIsRequired (Handle, (Byte)( AIsRequired ? 1 : 0 )));
		}

		public bool IsRequired ()
		{
			Byte resultIsRequired = 0;

			CheckError(Internal.Lib3MFWrapper.VolumeDataProperty_IsRequired (Handle, out resultIsRequired));
			return (resultIsRequired != 0);
		}

	}

	class CVolumeData : CBase
	{
		public CVolumeData (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CVolumeDataLevelset GetLevelset ()
		{
			IntPtr newTheLevelsetData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeData_GetLevelset (Handle, out newTheLevelsetData));
			return new CVolumeDataLevelset (newTheLevelsetData );
		}

		public CVolumeDataLevelset CreateNewLevelset (CVolumetricStack ATheVolumetricStack)
		{
			IntPtr newTheLevelsetData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeData_CreateNewLevelset (Handle, ATheVolumetricStack.GetHandle(), out newTheLevelsetData));
			return new CVolumeDataLevelset (newTheLevelsetData );
		}

		public CVolumeDataComposite GetComposite ()
		{
			IntPtr newTheCompositeData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeData_GetComposite (Handle, out newTheCompositeData));
			return new CVolumeDataComposite (newTheCompositeData );
		}

		public CVolumeDataComposite CreateNewComposite (CVolumetricStack ATheVolumetricStack)
		{
			IntPtr newTheCompositeData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeData_CreateNewComposite (Handle, ATheVolumetricStack.GetHandle(), out newTheCompositeData));
			return new CVolumeDataComposite (newTheCompositeData );
		}

		public CVolumeDataColor GetColor ()
		{
			IntPtr newTheColorData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeData_GetColor (Handle, out newTheColorData));
			return new CVolumeDataColor (newTheColorData );
		}

		public CVolumeDataColor CreateNewColor (CVolumetricStack ATheVolumetricStack)
		{
			IntPtr newTheColorData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeData_CreateNewColor (Handle, ATheVolumetricStack.GetHandle(), out newTheColorData));
			return new CVolumeDataColor (newTheColorData );
		}

		public UInt32 GetPropertyCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.VolumeData_GetPropertyCount (Handle, out resultCount));
			return resultCount;
		}

		public CVolumeDataProperty GetProperty (UInt32 AIndex)
		{
			IntPtr newThePropertyData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeData_GetProperty (Handle, AIndex, out newThePropertyData));
			return new CVolumeDataProperty (newThePropertyData );
		}

		public CVolumeDataProperty FindProperty (String AName)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);
			IntPtr newThePropertyData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeData_FindProperty (Handle, byteName, out newThePropertyData));
			return new CVolumeDataProperty (newThePropertyData );
		}

		public CVolumeDataProperty AddProperty (String AName, CVolumetricStack ATheVolumetricStack)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);
			IntPtr newThePropertyData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumeData_AddProperty (Handle, byteName, ATheVolumetricStack.GetHandle(), out newThePropertyData));
			return new CVolumeDataProperty (newThePropertyData );
		}

		public void RemoveProperty (String AName)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.VolumeData_RemoveProperty (Handle, byteName));
		}

	}

	class CComponent : CBase
	{
		public CComponent (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CObject GetObjectResource ()
		{
			IntPtr newObjectResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Component_GetObjectResource (Handle, out newObjectResource));
			return new CObject (newObjectResource );
		}

		public UInt32 GetObjectResourceID ()
		{
			UInt32 resultUniqueResourceID = 0;

			CheckError(Internal.Lib3MFWrapper.Component_GetObjectResourceID (Handle, out resultUniqueResourceID));
			return resultUniqueResourceID;
		}

		public String GetUUID (out bool AHasUUID)
		{
			Byte resultHasUUID = 0;
			UInt32 sizeUUID = 0;
			UInt32 neededUUID = 0;
			CheckError(Internal.Lib3MFWrapper.Component_GetUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, IntPtr.Zero));
			sizeUUID = neededUUID;
			byte[] bytesUUID = new byte[sizeUUID];
			GCHandle dataUUID = GCHandle.Alloc(bytesUUID, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Component_GetUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, dataUUID.AddrOfPinnedObject()));
			AHasUUID = (resultHasUUID != 0);
			dataUUID.Free();
			return Encoding.UTF8.GetString(bytesUUID).TrimEnd(char.MinValue);
		}

		public void SetUUID (String AUUID)
		{
			byte[] byteUUID = Encoding.UTF8.GetBytes(AUUID + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Component_SetUUID (Handle, byteUUID));
		}

		public bool HasTransform ()
		{
			Byte resultHasTransform = 0;

			CheckError(Internal.Lib3MFWrapper.Component_HasTransform (Handle, out resultHasTransform));
			return (resultHasTransform != 0);
		}

		public sTransform GetTransform ()
		{
			Internal.InternalTransform intresultTransform;

			CheckError(Internal.Lib3MFWrapper.Component_GetTransform (Handle, out intresultTransform));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Transform (intresultTransform);
		}

		public void SetTransform (sTransform ATransform)
		{
			Internal.InternalTransform intTransform = Internal.Lib3MFWrapper.convertStructToInternal_Transform (ATransform);

			CheckError(Internal.Lib3MFWrapper.Component_SetTransform (Handle, intTransform));
		}

	}

	class CComponentsObject : CObject
	{
		public CComponentsObject (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CComponent AddComponent (CObject AObjectResource, sTransform ATransform)
		{
			Internal.InternalTransform intTransform = Internal.Lib3MFWrapper.convertStructToInternal_Transform (ATransform);
			IntPtr newComponentInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ComponentsObject_AddComponent (Handle, AObjectResource.GetHandle(), intTransform, out newComponentInstance));
			return new CComponent (newComponentInstance );
		}

		public CComponent GetComponent (UInt32 AIndex)
		{
			IntPtr newComponentInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ComponentsObject_GetComponent (Handle, AIndex, out newComponentInstance));
			return new CComponent (newComponentInstance );
		}

		public UInt32 GetComponentCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.ComponentsObject_GetComponentCount (Handle, out resultCount));
			return resultCount;
		}

	}

	class CBeamSet : CBase
	{
		public CBeamSet (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public void SetName (String AName)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.BeamSet_SetName (Handle, byteName));
		}

		public String GetName ()
		{
			UInt32 sizeName = 0;
			UInt32 neededName = 0;
			CheckError(Internal.Lib3MFWrapper.BeamSet_GetName (Handle, sizeName, out neededName, IntPtr.Zero));
			sizeName = neededName;
			byte[] bytesName = new byte[sizeName];
			GCHandle dataName = GCHandle.Alloc(bytesName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamSet_GetName (Handle, sizeName, out neededName, dataName.AddrOfPinnedObject()));
			dataName.Free();
			return Encoding.UTF8.GetString(bytesName).TrimEnd(char.MinValue);
		}

		public void SetIdentifier (String AIdentifier)
		{
			byte[] byteIdentifier = Encoding.UTF8.GetBytes(AIdentifier + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.BeamSet_SetIdentifier (Handle, byteIdentifier));
		}

		public String GetIdentifier ()
		{
			UInt32 sizeIdentifier = 0;
			UInt32 neededIdentifier = 0;
			CheckError(Internal.Lib3MFWrapper.BeamSet_GetIdentifier (Handle, sizeIdentifier, out neededIdentifier, IntPtr.Zero));
			sizeIdentifier = neededIdentifier;
			byte[] bytesIdentifier = new byte[sizeIdentifier];
			GCHandle dataIdentifier = GCHandle.Alloc(bytesIdentifier, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamSet_GetIdentifier (Handle, sizeIdentifier, out neededIdentifier, dataIdentifier.AddrOfPinnedObject()));
			dataIdentifier.Free();
			return Encoding.UTF8.GetString(bytesIdentifier).TrimEnd(char.MinValue);
		}

		public UInt32 GetReferenceCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.BeamSet_GetReferenceCount (Handle, out resultCount));
			return resultCount;
		}

		public void SetReferences (UInt32[] AReferences)
		{
			GCHandle dataReferences = GCHandle.Alloc(AReferences, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamSet_SetReferences (Handle, (UInt64) AReferences.Length, dataReferences.AddrOfPinnedObject()));
			dataReferences.Free ();
		}

		public void GetReferences (out UInt32[] AReferences)
		{
			UInt64 sizeReferences = 0;
			UInt64 neededReferences = 0;
			CheckError(Internal.Lib3MFWrapper.BeamSet_GetReferences (Handle, sizeReferences, out neededReferences, IntPtr.Zero));
			sizeReferences = neededReferences;
			AReferences = new UInt32[sizeReferences];
			GCHandle dataReferences = GCHandle.Alloc(AReferences, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamSet_GetReferences (Handle, sizeReferences, out neededReferences, dataReferences.AddrOfPinnedObject()));
			dataReferences.Free();
		}

		public UInt32 GetBallReferenceCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.BeamSet_GetBallReferenceCount (Handle, out resultCount));
			return resultCount;
		}

		public void SetBallReferences (UInt32[] ABallReferences)
		{
			GCHandle dataBallReferences = GCHandle.Alloc(ABallReferences, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamSet_SetBallReferences (Handle, (UInt64) ABallReferences.Length, dataBallReferences.AddrOfPinnedObject()));
			dataBallReferences.Free ();
		}

		public void GetBallReferences (out UInt32[] ABallReferences)
		{
			UInt64 sizeBallReferences = 0;
			UInt64 neededBallReferences = 0;
			CheckError(Internal.Lib3MFWrapper.BeamSet_GetBallReferences (Handle, sizeBallReferences, out neededBallReferences, IntPtr.Zero));
			sizeBallReferences = neededBallReferences;
			ABallReferences = new UInt32[sizeBallReferences];
			GCHandle dataBallReferences = GCHandle.Alloc(ABallReferences, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BeamSet_GetBallReferences (Handle, sizeBallReferences, out neededBallReferences, dataBallReferences.AddrOfPinnedObject()));
			dataBallReferences.Free();
		}

	}

	class CBaseMaterialGroup : CResource
	{
		public CBaseMaterialGroup (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public UInt32 GetCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_GetCount (Handle, out resultCount));
			return resultCount;
		}

		public void GetAllPropertyIDs (out UInt32[] APropertyIDs)
		{
			UInt64 sizePropertyIDs = 0;
			UInt64 neededPropertyIDs = 0;
			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, IntPtr.Zero));
			sizePropertyIDs = neededPropertyIDs;
			APropertyIDs = new UInt32[sizePropertyIDs];
			GCHandle dataPropertyIDs = GCHandle.Alloc(APropertyIDs, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, dataPropertyIDs.AddrOfPinnedObject()));
			dataPropertyIDs.Free();
		}

		public UInt32 AddMaterial (String AName, sColor ADisplayColor)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);
			Internal.InternalColor intDisplayColor = Internal.Lib3MFWrapper.convertStructToInternal_Color (ADisplayColor);
			UInt32 resultPropertyID = 0;

			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_AddMaterial (Handle, byteName, intDisplayColor, out resultPropertyID));
			return resultPropertyID;
		}

		public void RemoveMaterial (UInt32 APropertyID)
		{

			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_RemoveMaterial (Handle, APropertyID));
		}

		public String GetName (UInt32 APropertyID)
		{
			UInt32 sizeName = 0;
			UInt32 neededName = 0;
			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_GetName (Handle, APropertyID, sizeName, out neededName, IntPtr.Zero));
			sizeName = neededName;
			byte[] bytesName = new byte[sizeName];
			GCHandle dataName = GCHandle.Alloc(bytesName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_GetName (Handle, APropertyID, sizeName, out neededName, dataName.AddrOfPinnedObject()));
			dataName.Free();
			return Encoding.UTF8.GetString(bytesName).TrimEnd(char.MinValue);
		}

		public void SetName (UInt32 APropertyID, String AName)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_SetName (Handle, APropertyID, byteName));
		}

		public void SetDisplayColor (UInt32 APropertyID, sColor ATheColor)
		{
			Internal.InternalColor intTheColor = Internal.Lib3MFWrapper.convertStructToInternal_Color (ATheColor);

			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_SetDisplayColor (Handle, APropertyID, intTheColor));
		}

		public sColor GetDisplayColor (UInt32 APropertyID)
		{
			Internal.InternalColor intresultTheColor;

			CheckError(Internal.Lib3MFWrapper.BaseMaterialGroup_GetDisplayColor (Handle, APropertyID, out intresultTheColor));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Color (intresultTheColor);
		}

	}

	class CColorGroup : CResource
	{
		public CColorGroup (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public UInt32 GetCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.ColorGroup_GetCount (Handle, out resultCount));
			return resultCount;
		}

		public void GetAllPropertyIDs (out UInt32[] APropertyIDs)
		{
			UInt64 sizePropertyIDs = 0;
			UInt64 neededPropertyIDs = 0;
			CheckError(Internal.Lib3MFWrapper.ColorGroup_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, IntPtr.Zero));
			sizePropertyIDs = neededPropertyIDs;
			APropertyIDs = new UInt32[sizePropertyIDs];
			GCHandle dataPropertyIDs = GCHandle.Alloc(APropertyIDs, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.ColorGroup_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, dataPropertyIDs.AddrOfPinnedObject()));
			dataPropertyIDs.Free();
		}

		public UInt32 AddColor (sColor ATheColor)
		{
			Internal.InternalColor intTheColor = Internal.Lib3MFWrapper.convertStructToInternal_Color (ATheColor);
			UInt32 resultPropertyID = 0;

			CheckError(Internal.Lib3MFWrapper.ColorGroup_AddColor (Handle, intTheColor, out resultPropertyID));
			return resultPropertyID;
		}

		public void RemoveColor (UInt32 APropertyID)
		{

			CheckError(Internal.Lib3MFWrapper.ColorGroup_RemoveColor (Handle, APropertyID));
		}

		public void SetColor (UInt32 APropertyID, sColor ATheColor)
		{
			Internal.InternalColor intTheColor = Internal.Lib3MFWrapper.convertStructToInternal_Color (ATheColor);

			CheckError(Internal.Lib3MFWrapper.ColorGroup_SetColor (Handle, APropertyID, intTheColor));
		}

		public sColor GetColor (UInt32 APropertyID)
		{
			Internal.InternalColor intresultTheColor;

			CheckError(Internal.Lib3MFWrapper.ColorGroup_GetColor (Handle, APropertyID, out intresultTheColor));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Color (intresultTheColor);
		}

	}

	class CTexture2DGroup : CResource
	{
		public CTexture2DGroup (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public UInt32 GetCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.Texture2DGroup_GetCount (Handle, out resultCount));
			return resultCount;
		}

		public void GetAllPropertyIDs (out UInt32[] APropertyIDs)
		{
			UInt64 sizePropertyIDs = 0;
			UInt64 neededPropertyIDs = 0;
			CheckError(Internal.Lib3MFWrapper.Texture2DGroup_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, IntPtr.Zero));
			sizePropertyIDs = neededPropertyIDs;
			APropertyIDs = new UInt32[sizePropertyIDs];
			GCHandle dataPropertyIDs = GCHandle.Alloc(APropertyIDs, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Texture2DGroup_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, dataPropertyIDs.AddrOfPinnedObject()));
			dataPropertyIDs.Free();
		}

		public UInt32 AddTex2Coord (sTex2Coord AUVCoordinate)
		{
			Internal.InternalTex2Coord intUVCoordinate = Internal.Lib3MFWrapper.convertStructToInternal_Tex2Coord (AUVCoordinate);
			UInt32 resultPropertyID = 0;

			CheckError(Internal.Lib3MFWrapper.Texture2DGroup_AddTex2Coord (Handle, intUVCoordinate, out resultPropertyID));
			return resultPropertyID;
		}

		public sTex2Coord GetTex2Coord (UInt32 APropertyID)
		{
			Internal.InternalTex2Coord intresultUVCoordinate;

			CheckError(Internal.Lib3MFWrapper.Texture2DGroup_GetTex2Coord (Handle, APropertyID, out intresultUVCoordinate));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Tex2Coord (intresultUVCoordinate);
		}

		public void RemoveTex2Coord (UInt32 APropertyID)
		{

			CheckError(Internal.Lib3MFWrapper.Texture2DGroup_RemoveTex2Coord (Handle, APropertyID));
		}

		public CTexture2D GetTexture2D ()
		{
			IntPtr newTexture2DInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Texture2DGroup_GetTexture2D (Handle, out newTexture2DInstance));
			return new CTexture2D (newTexture2DInstance );
		}

	}

	class CCompositeMaterials : CResource
	{
		public CCompositeMaterials (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public UInt32 GetCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.CompositeMaterials_GetCount (Handle, out resultCount));
			return resultCount;
		}

		public void GetAllPropertyIDs (out UInt32[] APropertyIDs)
		{
			UInt64 sizePropertyIDs = 0;
			UInt64 neededPropertyIDs = 0;
			CheckError(Internal.Lib3MFWrapper.CompositeMaterials_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, IntPtr.Zero));
			sizePropertyIDs = neededPropertyIDs;
			APropertyIDs = new UInt32[sizePropertyIDs];
			GCHandle dataPropertyIDs = GCHandle.Alloc(APropertyIDs, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.CompositeMaterials_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, dataPropertyIDs.AddrOfPinnedObject()));
			dataPropertyIDs.Free();
		}

		public CBaseMaterialGroup GetBaseMaterialGroup ()
		{
			IntPtr newBaseMaterialGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.CompositeMaterials_GetBaseMaterialGroup (Handle, out newBaseMaterialGroupInstance));
			return new CBaseMaterialGroup (newBaseMaterialGroupInstance );
		}

		public UInt32 AddComposite (sCompositeConstituent[] AComposite)
		{
			Internal.InternalCompositeConstituent[] intdataComposite = new Internal.InternalCompositeConstituent[AComposite.Length];
			for (int index = 0; index < AComposite.Length; index++)
				intdataComposite[index] = Internal.Lib3MFWrapper.convertStructToInternal_CompositeConstituent(AComposite[index]);
			GCHandle dataComposite = GCHandle.Alloc(intdataComposite, GCHandleType.Pinned);
			UInt32 resultPropertyID = 0;

			CheckError(Internal.Lib3MFWrapper.CompositeMaterials_AddComposite (Handle, (UInt64) AComposite.Length, dataComposite.AddrOfPinnedObject(), out resultPropertyID));
			dataComposite.Free ();
			return resultPropertyID;
		}

		public void RemoveComposite (UInt32 APropertyID)
		{

			CheckError(Internal.Lib3MFWrapper.CompositeMaterials_RemoveComposite (Handle, APropertyID));
		}

		public void GetComposite (UInt32 APropertyID, out sCompositeConstituent[] AComposite)
		{
			UInt64 sizeComposite = 0;
			UInt64 neededComposite = 0;
			CheckError(Internal.Lib3MFWrapper.CompositeMaterials_GetComposite (Handle, APropertyID, sizeComposite, out neededComposite, IntPtr.Zero));
			sizeComposite = neededComposite;
			var arrayComposite = new Internal.InternalCompositeConstituent[sizeComposite];
			GCHandle dataComposite = GCHandle.Alloc(arrayComposite, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.CompositeMaterials_GetComposite (Handle, APropertyID, sizeComposite, out neededComposite, dataComposite.AddrOfPinnedObject()));
			dataComposite.Free();
			AComposite = new sCompositeConstituent[sizeComposite];
			for (int index = 0; index < AComposite.Length; index++)
				AComposite[index] = Internal.Lib3MFWrapper.convertInternalToStruct_CompositeConstituent(arrayComposite[index]);
		}

	}

	class CMultiPropertyGroup : CResource
	{
		public CMultiPropertyGroup (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public UInt32 GetCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_GetCount (Handle, out resultCount));
			return resultCount;
		}

		public void GetAllPropertyIDs (out UInt32[] APropertyIDs)
		{
			UInt64 sizePropertyIDs = 0;
			UInt64 neededPropertyIDs = 0;
			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, IntPtr.Zero));
			sizePropertyIDs = neededPropertyIDs;
			APropertyIDs = new UInt32[sizePropertyIDs];
			GCHandle dataPropertyIDs = GCHandle.Alloc(APropertyIDs, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_GetAllPropertyIDs (Handle, sizePropertyIDs, out neededPropertyIDs, dataPropertyIDs.AddrOfPinnedObject()));
			dataPropertyIDs.Free();
		}

		public UInt32 AddMultiProperty (UInt32[] APropertyIDs)
		{
			GCHandle dataPropertyIDs = GCHandle.Alloc(APropertyIDs, GCHandleType.Pinned);
			UInt32 resultPropertyID = 0;

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_AddMultiProperty (Handle, (UInt64) APropertyIDs.Length, dataPropertyIDs.AddrOfPinnedObject(), out resultPropertyID));
			dataPropertyIDs.Free ();
			return resultPropertyID;
		}

		public void SetMultiProperty (UInt32 APropertyID, UInt32[] APropertyIDs)
		{
			GCHandle dataPropertyIDs = GCHandle.Alloc(APropertyIDs, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_SetMultiProperty (Handle, APropertyID, (UInt64) APropertyIDs.Length, dataPropertyIDs.AddrOfPinnedObject()));
			dataPropertyIDs.Free ();
		}

		public void GetMultiProperty (UInt32 APropertyID, out UInt32[] APropertyIDs)
		{
			UInt64 sizePropertyIDs = 0;
			UInt64 neededPropertyIDs = 0;
			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_GetMultiProperty (Handle, APropertyID, sizePropertyIDs, out neededPropertyIDs, IntPtr.Zero));
			sizePropertyIDs = neededPropertyIDs;
			APropertyIDs = new UInt32[sizePropertyIDs];
			GCHandle dataPropertyIDs = GCHandle.Alloc(APropertyIDs, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_GetMultiProperty (Handle, APropertyID, sizePropertyIDs, out neededPropertyIDs, dataPropertyIDs.AddrOfPinnedObject()));
			dataPropertyIDs.Free();
		}

		public void RemoveMultiProperty (UInt32 APropertyID)
		{

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_RemoveMultiProperty (Handle, APropertyID));
		}

		public UInt32 GetLayerCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_GetLayerCount (Handle, out resultCount));
			return resultCount;
		}

		public UInt32 AddLayer (sMultiPropertyLayer ATheLayer)
		{
			Internal.InternalMultiPropertyLayer intTheLayer = Internal.Lib3MFWrapper.convertStructToInternal_MultiPropertyLayer (ATheLayer);
			UInt32 resultLayerIndex = 0;

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_AddLayer (Handle, intTheLayer, out resultLayerIndex));
			return resultLayerIndex;
		}

		public sMultiPropertyLayer GetLayer (UInt32 ALayerIndex)
		{
			Internal.InternalMultiPropertyLayer intresultTheLayer;

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_GetLayer (Handle, ALayerIndex, out intresultTheLayer));
			return Internal.Lib3MFWrapper.convertInternalToStruct_MultiPropertyLayer (intresultTheLayer);
		}

		public void RemoveLayer (UInt32 ALayerIndex)
		{

			CheckError(Internal.Lib3MFWrapper.MultiPropertyGroup_RemoveLayer (Handle, ALayerIndex));
		}

	}

	class CImage3D : CResource
	{
		public CImage3D (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public UInt32 GetSizeX ()
		{
			UInt32 resultSizeX = 0;

			CheckError(Internal.Lib3MFWrapper.Image3D_GetSizeX (Handle, out resultSizeX));
			return resultSizeX;
		}

		public UInt32 GetSizeY ()
		{
			UInt32 resultSizeY = 0;

			CheckError(Internal.Lib3MFWrapper.Image3D_GetSizeY (Handle, out resultSizeY));
			return resultSizeY;
		}

		public UInt32 GetSheetCount ()
		{
			UInt32 resultSheetCount = 0;

			CheckError(Internal.Lib3MFWrapper.Image3D_GetSheetCount (Handle, out resultSheetCount));
			return resultSheetCount;
		}

		public CAttachment GetSheet (UInt32 AIndex)
		{
			IntPtr newSheet = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Image3D_GetSheet (Handle, AIndex, out newSheet));
			return new CAttachment (newSheet );
		}

		public Double GetSheetMinValue (UInt32 AIndex)
		{
			Double resultMinVal = 0;

			CheckError(Internal.Lib3MFWrapper.Image3D_GetSheetMinValue (Handle, AIndex, out resultMinVal));
			return resultMinVal;
		}

		public Double GetSheetMaxValue (UInt32 AIndex)
		{
			Double resultMaxVal = 0;

			CheckError(Internal.Lib3MFWrapper.Image3D_GetSheetMaxValue (Handle, AIndex, out resultMaxVal));
			return resultMaxVal;
		}

		public CAttachment CreateEmptySheet (UInt32 AIndex, String APath, Double AMin, Double AMax)
		{
			byte[] bytePath = Encoding.UTF8.GetBytes(APath + char.MinValue);
			IntPtr newSheet = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Image3D_CreateEmptySheet (Handle, AIndex, bytePath, AMin, AMax, out newSheet));
			return new CAttachment (newSheet );
		}

		public CAttachment CreateSheetFromBuffer (UInt32 AIndex, String APath, Byte[] AData, Double AMin, Double AMax)
		{
			byte[] bytePath = Encoding.UTF8.GetBytes(APath + char.MinValue);
			GCHandle dataData = GCHandle.Alloc(AData, GCHandleType.Pinned);
			IntPtr newSheet = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Image3D_CreateSheetFromBuffer (Handle, AIndex, bytePath, (UInt64) AData.Length, dataData.AddrOfPinnedObject(), AMin, AMax, out newSheet));
			dataData.Free ();
			return new CAttachment (newSheet );
		}

		public CAttachment CreateSheetFromFile (UInt32 AIndex, String APath, String AFileName, Double AMin, Double AMax)
		{
			byte[] bytePath = Encoding.UTF8.GetBytes(APath + char.MinValue);
			byte[] byteFileName = Encoding.UTF8.GetBytes(AFileName + char.MinValue);
			IntPtr newSheet = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Image3D_CreateSheetFromFile (Handle, AIndex, bytePath, byteFileName, AMin, AMax, out newSheet));
			return new CAttachment (newSheet );
		}

		public void SetSheet (UInt32 AIndex, CAttachment ASheet)
		{

			CheckError(Internal.Lib3MFWrapper.Image3D_SetSheet (Handle, AIndex, ASheet.GetHandle()));
		}

		public void SetSheetMinValue (UInt32 AIndex, Double AMinVal)
		{

			CheckError(Internal.Lib3MFWrapper.Image3D_SetSheetMinValue (Handle, AIndex, AMinVal));
		}

		public void SetSheetMaxValue (UInt32 AIndex, Double AMaxVal)
		{

			CheckError(Internal.Lib3MFWrapper.Image3D_SetSheetMaxValue (Handle, AIndex, AMaxVal));
		}

	}

	class CImage3DChannelSelector : CBase
	{
		public CImage3DChannelSelector (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CImage3D GetImage ()
		{
			IntPtr newImage3D = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_GetImage (Handle, out newImage3D));
			return new CImage3D (newImage3D );
		}

		public void SetImage (CImage3D AImage3D)
		{

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_SetImage (Handle, AImage3D.GetHandle()));
		}

		public void SetSourceChannel (String AChannelName)
		{
			byte[] byteChannelName = Encoding.UTF8.GetBytes(AChannelName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_SetSourceChannel (Handle, byteChannelName));
		}

		public String GetSourceChannel ()
		{
			UInt32 sizeChannelName = 0;
			UInt32 neededChannelName = 0;
			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_GetSourceChannel (Handle, sizeChannelName, out neededChannelName, IntPtr.Zero));
			sizeChannelName = neededChannelName;
			byte[] bytesChannelName = new byte[sizeChannelName];
			GCHandle dataChannelName = GCHandle.Alloc(bytesChannelName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_GetSourceChannel (Handle, sizeChannelName, out neededChannelName, dataChannelName.AddrOfPinnedObject()));
			dataChannelName.Free();
			return Encoding.UTF8.GetString(bytesChannelName).TrimEnd(char.MinValue);
		}

		public void SetDestinationChannel (String AChannelName)
		{
			byte[] byteChannelName = Encoding.UTF8.GetBytes(AChannelName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_SetDestinationChannel (Handle, byteChannelName));
		}

		public String GetDestinationChannel ()
		{
			UInt32 sizeChannelName = 0;
			UInt32 neededChannelName = 0;
			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_GetDestinationChannel (Handle, sizeChannelName, out neededChannelName, IntPtr.Zero));
			sizeChannelName = neededChannelName;
			byte[] bytesChannelName = new byte[sizeChannelName];
			GCHandle dataChannelName = GCHandle.Alloc(bytesChannelName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_GetDestinationChannel (Handle, sizeChannelName, out neededChannelName, dataChannelName.AddrOfPinnedObject()));
			dataChannelName.Free();
			return Encoding.UTF8.GetString(bytesChannelName).TrimEnd(char.MinValue);
		}

		public void SetFilter (eTextureFilter AFilter)
		{
			Int32 enumFilter = (Int32) AFilter;

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_SetFilter (Handle, enumFilter));
		}

		public eTextureFilter GetFilter ()
		{
			Int32 resultFilter = 0;

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_GetFilter (Handle, out resultFilter));
			return (eTextureFilter) (resultFilter);
		}

		public void SetTileStyles (eTextureTileStyle ATileStyleU, eTextureTileStyle ATileStyleV, eTextureTileStyle ATileStyleW)
		{
			Int32 enumTileStyleU = (Int32) ATileStyleU;
			Int32 enumTileStyleV = (Int32) ATileStyleV;
			Int32 enumTileStyleW = (Int32) ATileStyleW;

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_SetTileStyles (Handle, enumTileStyleU, enumTileStyleV, enumTileStyleW));
		}

		public void GetTileStyles (out eTextureTileStyle ATileStyleU, out eTextureTileStyle ATileStyleV, out eTextureTileStyle ATileStyleW)
		{
			Int32 resultTileStyleU = 0;
			Int32 resultTileStyleV = 0;
			Int32 resultTileStyleW = 0;

			CheckError(Internal.Lib3MFWrapper.Image3DChannelSelector_GetTileStyles (Handle, out resultTileStyleU, out resultTileStyleV, out resultTileStyleW));
			ATileStyleU = (eTextureTileStyle) (resultTileStyleU);
			ATileStyleV = (eTextureTileStyle) (resultTileStyleV);
			ATileStyleW = (eTextureTileStyle) (resultTileStyleW);
		}

	}

	class CVolumetricLayer : CBase
	{
		public CVolumetricLayer (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public sTransform GetTransform ()
		{
			Internal.InternalTransform intresultTransform;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_GetTransform (Handle, out intresultTransform));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Transform (intresultTransform);
		}

		public void SetTransform (sTransform ATransform)
		{
			Internal.InternalTransform intTransform = Internal.Lib3MFWrapper.convertStructToInternal_Transform (ATransform);

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_SetTransform (Handle, intTransform));
		}

		public eBlendMethod GetBlendMethod ()
		{
			Int32 resultBlendMethod = 0;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_GetBlendMethod (Handle, out resultBlendMethod));
			return (eBlendMethod) (resultBlendMethod);
		}

		public void SetBlendMethod (eBlendMethod ABlendMethod)
		{
			Int32 enumBlendMethod = (Int32) ABlendMethod;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_SetBlendMethod (Handle, enumBlendMethod));
		}

		public Double GetSourceAlpha ()
		{
			Double resultSourceAlpha = 0;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_GetSourceAlpha (Handle, out resultSourceAlpha));
			return resultSourceAlpha;
		}

		public void SetSourceAlpha (Double ASourceAlpha)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_SetSourceAlpha (Handle, ASourceAlpha));
		}

		public Double GetDestinationAlpha ()
		{
			Double resultDestinationAlpha = 0;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_GetDestinationAlpha (Handle, out resultDestinationAlpha));
			return resultDestinationAlpha;
		}

		public void SetDestinationAlpha (Double ADestinationAlpha)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_SetDestinationAlpha (Handle, ADestinationAlpha));
		}

		public void GetInformation (out sTransform ATransform, out eBlendMethod ABlendMethod, out Double ASourceAlpha, out Double ADestinationAlpha)
		{
			Internal.InternalTransform intresultTransform;
			Int32 resultBlendMethod = 0;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_GetInformation (Handle, out intresultTransform, out resultBlendMethod, out ASourceAlpha, out ADestinationAlpha));
			ATransform = Internal.Lib3MFWrapper.convertInternalToStruct_Transform (intresultTransform);
			ABlendMethod = (eBlendMethod) (resultBlendMethod);
		}

		public void SetInformation (sTransform ATransform, eBlendMethod ABlendMethod, Double ASourceAlpha, Double ADestinationAlpha)
		{
			Internal.InternalTransform intTransform = Internal.Lib3MFWrapper.convertStructToInternal_Transform (ATransform);
			Int32 enumBlendMethod = (Int32) ABlendMethod;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_SetInformation (Handle, intTransform, enumBlendMethod, ASourceAlpha, ADestinationAlpha));
		}

		public CImage3DChannelSelector CreateMaskChannelSelector (CImage3D AImage3D, String ASourceChannel, String ADestinationChannel)
		{
			byte[] byteSourceChannel = Encoding.UTF8.GetBytes(ASourceChannel + char.MinValue);
			byte[] byteDestinationChannel = Encoding.UTF8.GetBytes(ADestinationChannel + char.MinValue);
			IntPtr newChannelSelector = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_CreateMaskChannelSelector (Handle, AImage3D.GetHandle(), byteSourceChannel, byteDestinationChannel, out newChannelSelector));
			return new CImage3DChannelSelector (newChannelSelector );
		}

		public bool HasMaskChannelSelector ()
		{
			Byte resultSelectorExists = 0;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_HasMaskChannelSelector (Handle, out resultSelectorExists));
			return (resultSelectorExists != 0);
		}

		public void ClearMaskChannelSelector ()
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_ClearMaskChannelSelector (Handle));
		}

		public CImage3DChannelSelector GetMaskChannelSelector ()
		{
			IntPtr newChannelSelector = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_GetMaskChannelSelector (Handle, out newChannelSelector));
			return new CImage3DChannelSelector (newChannelSelector );
		}

		public UInt32 GetChannelSelectorCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_GetChannelSelectorCount (Handle, out resultCount));
			return resultCount;
		}

		public CImage3DChannelSelector GetChannelSelector (UInt32 AIndex)
		{
			IntPtr newChannelSelector = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_GetChannelSelector (Handle, AIndex, out newChannelSelector));
			return new CImage3DChannelSelector (newChannelSelector );
		}

		public CImage3DChannelSelector AddChannelSelector (CImage3D AImage3D, String ASourceChannel, String ADestinationChannel)
		{
			byte[] byteSourceChannel = Encoding.UTF8.GetBytes(ASourceChannel + char.MinValue);
			byte[] byteDestinationChannel = Encoding.UTF8.GetBytes(ADestinationChannel + char.MinValue);
			IntPtr newChannelSelector = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_AddChannelSelector (Handle, AImage3D.GetHandle(), byteSourceChannel, byteDestinationChannel, out newChannelSelector));
			return new CImage3DChannelSelector (newChannelSelector );
		}

		public void ClearChannelSelectors ()
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_ClearChannelSelectors (Handle));
		}

		public void ReindexChannelSelector (CImage3DChannelSelector AChannelSelector, UInt32 AIndex)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_ReindexChannelSelector (Handle, AChannelSelector.GetHandle(), AIndex));
		}

		public void RemoveChannelSelector (CImage3DChannelSelector AChannelSelector)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_RemoveChannelSelector (Handle, AChannelSelector.GetHandle()));
		}

		public void RemoveChannelSelectorByIndex (UInt32 AIndex)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricLayer_RemoveChannelSelectorByIndex (Handle, AIndex));
		}

	}

	class CVolumetricStack : CResource
	{
		public CVolumetricStack (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public void Clear ()
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_Clear (Handle));
		}

		public void ClearUnusedDestinationChannels ()
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_ClearUnusedDestinationChannels (Handle));
		}

		public UInt32 GetDestinationChannelCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_GetDestinationChannelCount (Handle, out resultCount));
			return resultCount;
		}

		public void GetDestinationChannel (UInt32 AIndex, out String AName, out Double ABackground)
		{
			UInt32 sizeName = 0;
			UInt32 neededName = 0;
			CheckError(Internal.Lib3MFWrapper.VolumetricStack_GetDestinationChannel (Handle, AIndex, sizeName, out neededName, IntPtr.Zero, out ABackground));
			sizeName = neededName;
			byte[] bytesName = new byte[sizeName];
			GCHandle dataName = GCHandle.Alloc(bytesName, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_GetDestinationChannel (Handle, AIndex, sizeName, out neededName, dataName.AddrOfPinnedObject(), out ABackground));
			dataName.Free();
			AName = Encoding.UTF8.GetString(bytesName).TrimEnd(char.MinValue);
		}

		public UInt32 AddDestinationChannel (String AName, Double ABackground)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);
			UInt32 resultIndex = 0;

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_AddDestinationChannel (Handle, byteName, ABackground, out resultIndex));
			return resultIndex;
		}

		public void UpdateDestinationChannel (UInt32 AIndex, Double ABackground)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_UpdateDestinationChannel (Handle, AIndex, ABackground));
		}

		public void UpdateDestinationChannelByName (String AName, Double ABackground)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_UpdateDestinationChannelByName (Handle, byteName, ABackground));
		}

		public void RemoveDestinationChannel (UInt32 AIndex)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_RemoveDestinationChannel (Handle, AIndex));
		}

		public void RemoveDestinationChannelByName (String AName)
		{
			byte[] byteName = Encoding.UTF8.GetBytes(AName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_RemoveDestinationChannelByName (Handle, byteName));
		}

		public UInt32 GetLayerCount ()
		{
			UInt32 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_GetLayerCount (Handle, out resultCount));
			return resultCount;
		}

		public CVolumetricLayer GetLayer (UInt32 AIndex)
		{
			IntPtr newLayer = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_GetLayer (Handle, AIndex, out newLayer));
			return new CVolumetricLayer (newLayer );
		}

		public CVolumetricLayer AddLayer (sTransform ATransform, eBlendMethod ABlendMethod)
		{
			Internal.InternalTransform intTransform = Internal.Lib3MFWrapper.convertStructToInternal_Transform (ATransform);
			Int32 enumBlendMethod = (Int32) ABlendMethod;
			IntPtr newLayer = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_AddLayer (Handle, intTransform, enumBlendMethod, out newLayer));
			return new CVolumetricLayer (newLayer );
		}

		public void ReindexLayer (CVolumetricLayer ALayer, UInt32 AIndex)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_ReindexLayer (Handle, ALayer.GetHandle(), AIndex));
		}

		public void RemoveLayer (CVolumetricLayer ALayer)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_RemoveLayer (Handle, ALayer.GetHandle()));
		}

		public void RemoveLayerByIndex (UInt32 AIndex)
		{

			CheckError(Internal.Lib3MFWrapper.VolumetricStack_RemoveLayerByIndex (Handle, AIndex));
		}

	}

	class CAttachment : CBase
	{
		public CAttachment (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public String GetPath ()
		{
			UInt32 sizePath = 0;
			UInt32 neededPath = 0;
			CheckError(Internal.Lib3MFWrapper.Attachment_GetPath (Handle, sizePath, out neededPath, IntPtr.Zero));
			sizePath = neededPath;
			byte[] bytesPath = new byte[sizePath];
			GCHandle dataPath = GCHandle.Alloc(bytesPath, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Attachment_GetPath (Handle, sizePath, out neededPath, dataPath.AddrOfPinnedObject()));
			dataPath.Free();
			return Encoding.UTF8.GetString(bytesPath).TrimEnd(char.MinValue);
		}

		public void SetPath (String APath)
		{
			byte[] bytePath = Encoding.UTF8.GetBytes(APath + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Attachment_SetPath (Handle, bytePath));
		}

		public CPackagePart PackagePart ()
		{
			IntPtr newPackagePart = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Attachment_PackagePart (Handle, out newPackagePart));
			return new CPackagePart (newPackagePart );
		}

		public String GetRelationShipType ()
		{
			UInt32 sizePath = 0;
			UInt32 neededPath = 0;
			CheckError(Internal.Lib3MFWrapper.Attachment_GetRelationShipType (Handle, sizePath, out neededPath, IntPtr.Zero));
			sizePath = neededPath;
			byte[] bytesPath = new byte[sizePath];
			GCHandle dataPath = GCHandle.Alloc(bytesPath, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Attachment_GetRelationShipType (Handle, sizePath, out neededPath, dataPath.AddrOfPinnedObject()));
			dataPath.Free();
			return Encoding.UTF8.GetString(bytesPath).TrimEnd(char.MinValue);
		}

		public void SetRelationShipType (String APath)
		{
			byte[] bytePath = Encoding.UTF8.GetBytes(APath + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Attachment_SetRelationShipType (Handle, bytePath));
		}

		public void WriteToFile (String AFileName)
		{
			byte[] byteFileName = Encoding.UTF8.GetBytes(AFileName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Attachment_WriteToFile (Handle, byteFileName));
		}

		public void ReadFromFile (String AFileName)
		{
			byte[] byteFileName = Encoding.UTF8.GetBytes(AFileName + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Attachment_ReadFromFile (Handle, byteFileName));
		}

		public void ReadFromCallback (IntPtr ATheReadCallback, UInt64 AStreamSize, IntPtr ATheSeekCallback, UInt64 AUserData)
		{

			CheckError(Internal.Lib3MFWrapper.Attachment_ReadFromCallback (Handle, IntPtr.Zero, AStreamSize, IntPtr.Zero, AUserData));
		}

		public UInt64 GetStreamSize ()
		{
			UInt64 resultStreamSize = 0;

			CheckError(Internal.Lib3MFWrapper.Attachment_GetStreamSize (Handle, out resultStreamSize));
			return resultStreamSize;
		}

		public void WriteToBuffer (out Byte[] ABuffer)
		{
			UInt64 sizeBuffer = 0;
			UInt64 neededBuffer = 0;
			CheckError(Internal.Lib3MFWrapper.Attachment_WriteToBuffer (Handle, sizeBuffer, out neededBuffer, IntPtr.Zero));
			sizeBuffer = neededBuffer;
			ABuffer = new Byte[sizeBuffer];
			GCHandle dataBuffer = GCHandle.Alloc(ABuffer, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Attachment_WriteToBuffer (Handle, sizeBuffer, out neededBuffer, dataBuffer.AddrOfPinnedObject()));
			dataBuffer.Free();
		}

		public void ReadFromBuffer (Byte[] ABuffer)
		{
			GCHandle dataBuffer = GCHandle.Alloc(ABuffer, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Attachment_ReadFromBuffer (Handle, (UInt64) ABuffer.Length, dataBuffer.AddrOfPinnedObject()));
			dataBuffer.Free ();
		}

	}

	class CTexture2D : CResource
	{
		public CTexture2D (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CAttachment GetAttachment ()
		{
			IntPtr newAttachment = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Texture2D_GetAttachment (Handle, out newAttachment));
			return new CAttachment (newAttachment );
		}

		public void SetAttachment (CAttachment AAttachment)
		{

			CheckError(Internal.Lib3MFWrapper.Texture2D_SetAttachment (Handle, AAttachment.GetHandle()));
		}

		public eTextureType GetContentType ()
		{
			Int32 resultContentType = 0;

			CheckError(Internal.Lib3MFWrapper.Texture2D_GetContentType (Handle, out resultContentType));
			return (eTextureType) (resultContentType);
		}

		public void SetContentType (eTextureType AContentType)
		{
			Int32 enumContentType = (Int32) AContentType;

			CheckError(Internal.Lib3MFWrapper.Texture2D_SetContentType (Handle, enumContentType));
		}

		public void GetTileStyleUV (out eTextureTileStyle ATileStyleU, out eTextureTileStyle ATileStyleV)
		{
			Int32 resultTileStyleU = 0;
			Int32 resultTileStyleV = 0;

			CheckError(Internal.Lib3MFWrapper.Texture2D_GetTileStyleUV (Handle, out resultTileStyleU, out resultTileStyleV));
			ATileStyleU = (eTextureTileStyle) (resultTileStyleU);
			ATileStyleV = (eTextureTileStyle) (resultTileStyleV);
		}

		public void SetTileStyleUV (eTextureTileStyle ATileStyleU, eTextureTileStyle ATileStyleV)
		{
			Int32 enumTileStyleU = (Int32) ATileStyleU;
			Int32 enumTileStyleV = (Int32) ATileStyleV;

			CheckError(Internal.Lib3MFWrapper.Texture2D_SetTileStyleUV (Handle, enumTileStyleU, enumTileStyleV));
		}

		public eTextureFilter GetFilter ()
		{
			Int32 resultFilter = 0;

			CheckError(Internal.Lib3MFWrapper.Texture2D_GetFilter (Handle, out resultFilter));
			return (eTextureFilter) (resultFilter);
		}

		public void SetFilter (eTextureFilter AFilter)
		{
			Int32 enumFilter = (Int32) AFilter;

			CheckError(Internal.Lib3MFWrapper.Texture2D_SetFilter (Handle, enumFilter));
		}

	}

	class CBuildItem : CBase
	{
		public CBuildItem (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CObject GetObjectResource ()
		{
			IntPtr newObjectResource = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.BuildItem_GetObjectResource (Handle, out newObjectResource));
			return new CObject (newObjectResource );
		}

		public String GetUUID (out bool AHasUUID)
		{
			Byte resultHasUUID = 0;
			UInt32 sizeUUID = 0;
			UInt32 neededUUID = 0;
			CheckError(Internal.Lib3MFWrapper.BuildItem_GetUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, IntPtr.Zero));
			sizeUUID = neededUUID;
			byte[] bytesUUID = new byte[sizeUUID];
			GCHandle dataUUID = GCHandle.Alloc(bytesUUID, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BuildItem_GetUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, dataUUID.AddrOfPinnedObject()));
			AHasUUID = (resultHasUUID != 0);
			dataUUID.Free();
			return Encoding.UTF8.GetString(bytesUUID).TrimEnd(char.MinValue);
		}

		public void SetUUID (String AUUID)
		{
			byte[] byteUUID = Encoding.UTF8.GetBytes(AUUID + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.BuildItem_SetUUID (Handle, byteUUID));
		}

		public UInt32 GetObjectResourceID ()
		{
			UInt32 resultUniqueResourceID = 0;

			CheckError(Internal.Lib3MFWrapper.BuildItem_GetObjectResourceID (Handle, out resultUniqueResourceID));
			return resultUniqueResourceID;
		}

		public bool HasObjectTransform ()
		{
			Byte resultHasTransform = 0;

			CheckError(Internal.Lib3MFWrapper.BuildItem_HasObjectTransform (Handle, out resultHasTransform));
			return (resultHasTransform != 0);
		}

		public sTransform GetObjectTransform ()
		{
			Internal.InternalTransform intresultTransform;

			CheckError(Internal.Lib3MFWrapper.BuildItem_GetObjectTransform (Handle, out intresultTransform));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Transform (intresultTransform);
		}

		public void SetObjectTransform (sTransform ATransform)
		{
			Internal.InternalTransform intTransform = Internal.Lib3MFWrapper.convertStructToInternal_Transform (ATransform);

			CheckError(Internal.Lib3MFWrapper.BuildItem_SetObjectTransform (Handle, intTransform));
		}

		public String GetPartNumber ()
		{
			UInt32 sizePartNumber = 0;
			UInt32 neededPartNumber = 0;
			CheckError(Internal.Lib3MFWrapper.BuildItem_GetPartNumber (Handle, sizePartNumber, out neededPartNumber, IntPtr.Zero));
			sizePartNumber = neededPartNumber;
			byte[] bytesPartNumber = new byte[sizePartNumber];
			GCHandle dataPartNumber = GCHandle.Alloc(bytesPartNumber, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.BuildItem_GetPartNumber (Handle, sizePartNumber, out neededPartNumber, dataPartNumber.AddrOfPinnedObject()));
			dataPartNumber.Free();
			return Encoding.UTF8.GetString(bytesPartNumber).TrimEnd(char.MinValue);
		}

		public void SetPartNumber (String ASetPartnumber)
		{
			byte[] byteSetPartnumber = Encoding.UTF8.GetBytes(ASetPartnumber + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.BuildItem_SetPartNumber (Handle, byteSetPartnumber));
		}

		public CMetaDataGroup GetMetaDataGroup ()
		{
			IntPtr newMetaDataGroup = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.BuildItem_GetMetaDataGroup (Handle, out newMetaDataGroup));
			return new CMetaDataGroup (newMetaDataGroup );
		}

		public sBox GetOutbox ()
		{
			Internal.InternalBox intresultOutbox;

			CheckError(Internal.Lib3MFWrapper.BuildItem_GetOutbox (Handle, out intresultOutbox));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Box (intresultOutbox);
		}

	}

	class CBuildItemIterator : CBase
	{
		public CBuildItemIterator (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public bool MoveNext ()
		{
			Byte resultHasNext = 0;

			CheckError(Internal.Lib3MFWrapper.BuildItemIterator_MoveNext (Handle, out resultHasNext));
			return (resultHasNext != 0);
		}

		public bool MovePrevious ()
		{
			Byte resultHasPrevious = 0;

			CheckError(Internal.Lib3MFWrapper.BuildItemIterator_MovePrevious (Handle, out resultHasPrevious));
			return (resultHasPrevious != 0);
		}

		public CBuildItem GetCurrent ()
		{
			IntPtr newBuildItem = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.BuildItemIterator_GetCurrent (Handle, out newBuildItem));
			return new CBuildItem (newBuildItem );
		}

		public CBuildItemIterator Clone ()
		{
			IntPtr newOutBuildItemIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.BuildItemIterator_Clone (Handle, out newOutBuildItemIterator));
			return new CBuildItemIterator (newOutBuildItemIterator );
		}

		public UInt64 Count ()
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.BuildItemIterator_Count (Handle, out resultCount));
			return resultCount;
		}

	}

	class CSlice : CBase
	{
		public CSlice (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public void SetVertices (sPosition2D[] AVertices)
		{
			Internal.InternalPosition2D[] intdataVertices = new Internal.InternalPosition2D[AVertices.Length];
			for (int index = 0; index < AVertices.Length; index++)
				intdataVertices[index] = Internal.Lib3MFWrapper.convertStructToInternal_Position2D(AVertices[index]);
			GCHandle dataVertices = GCHandle.Alloc(intdataVertices, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Slice_SetVertices (Handle, (UInt64) AVertices.Length, dataVertices.AddrOfPinnedObject()));
			dataVertices.Free ();
		}

		public void GetVertices (out sPosition2D[] AVertices)
		{
			UInt64 sizeVertices = 0;
			UInt64 neededVertices = 0;
			CheckError(Internal.Lib3MFWrapper.Slice_GetVertices (Handle, sizeVertices, out neededVertices, IntPtr.Zero));
			sizeVertices = neededVertices;
			var arrayVertices = new Internal.InternalPosition2D[sizeVertices];
			GCHandle dataVertices = GCHandle.Alloc(arrayVertices, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Slice_GetVertices (Handle, sizeVertices, out neededVertices, dataVertices.AddrOfPinnedObject()));
			dataVertices.Free();
			AVertices = new sPosition2D[sizeVertices];
			for (int index = 0; index < AVertices.Length; index++)
				AVertices[index] = Internal.Lib3MFWrapper.convertInternalToStruct_Position2D(arrayVertices[index]);
		}

		public UInt64 GetVertexCount ()
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.Slice_GetVertexCount (Handle, out resultCount));
			return resultCount;
		}

		public UInt64 AddPolygon (UInt32[] AIndices)
		{
			GCHandle dataIndices = GCHandle.Alloc(AIndices, GCHandleType.Pinned);
			UInt64 resultIndex = 0;

			CheckError(Internal.Lib3MFWrapper.Slice_AddPolygon (Handle, (UInt64) AIndices.Length, dataIndices.AddrOfPinnedObject(), out resultIndex));
			dataIndices.Free ();
			return resultIndex;
		}

		public UInt64 GetPolygonCount ()
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.Slice_GetPolygonCount (Handle, out resultCount));
			return resultCount;
		}

		public void SetPolygonIndices (UInt64 AIndex, UInt32[] AIndices)
		{
			GCHandle dataIndices = GCHandle.Alloc(AIndices, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Slice_SetPolygonIndices (Handle, AIndex, (UInt64) AIndices.Length, dataIndices.AddrOfPinnedObject()));
			dataIndices.Free ();
		}

		public void GetPolygonIndices (UInt64 AIndex, out UInt32[] AIndices)
		{
			UInt64 sizeIndices = 0;
			UInt64 neededIndices = 0;
			CheckError(Internal.Lib3MFWrapper.Slice_GetPolygonIndices (Handle, AIndex, sizeIndices, out neededIndices, IntPtr.Zero));
			sizeIndices = neededIndices;
			AIndices = new UInt32[sizeIndices];
			GCHandle dataIndices = GCHandle.Alloc(AIndices, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Slice_GetPolygonIndices (Handle, AIndex, sizeIndices, out neededIndices, dataIndices.AddrOfPinnedObject()));
			dataIndices.Free();
		}

		public UInt64 GetPolygonIndexCount (UInt64 AIndex)
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.Slice_GetPolygonIndexCount (Handle, AIndex, out resultCount));
			return resultCount;
		}

		public Double GetZTop ()
		{
			Double resultZTop = 0;

			CheckError(Internal.Lib3MFWrapper.Slice_GetZTop (Handle, out resultZTop));
			return resultZTop;
		}

	}

	class CSliceStack : CResource
	{
		public CSliceStack (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public Double GetBottomZ ()
		{
			Double resultZBottom = 0;

			CheckError(Internal.Lib3MFWrapper.SliceStack_GetBottomZ (Handle, out resultZBottom));
			return resultZBottom;
		}

		public UInt64 GetSliceCount ()
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.SliceStack_GetSliceCount (Handle, out resultCount));
			return resultCount;
		}

		public CSlice GetSlice (UInt64 ASliceIndex)
		{
			IntPtr newTheSlice = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.SliceStack_GetSlice (Handle, ASliceIndex, out newTheSlice));
			return new CSlice (newTheSlice );
		}

		public CSlice AddSlice (Double AZTop)
		{
			IntPtr newTheSlice = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.SliceStack_AddSlice (Handle, AZTop, out newTheSlice));
			return new CSlice (newTheSlice );
		}

		public UInt64 GetSliceRefCount ()
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.SliceStack_GetSliceRefCount (Handle, out resultCount));
			return resultCount;
		}

		public void AddSliceStackReference (CSliceStack ATheSliceStack)
		{

			CheckError(Internal.Lib3MFWrapper.SliceStack_AddSliceStackReference (Handle, ATheSliceStack.GetHandle()));
		}

		public CSliceStack GetSliceStackReference (UInt64 ASliceRefIndex)
		{
			IntPtr newTheSliceStack = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.SliceStack_GetSliceStackReference (Handle, ASliceRefIndex, out newTheSliceStack));
			return new CSliceStack (newTheSliceStack );
		}

		public void CollapseSliceReferences ()
		{

			CheckError(Internal.Lib3MFWrapper.SliceStack_CollapseSliceReferences (Handle));
		}

		public void SetOwnPath (String APath)
		{
			byte[] bytePath = Encoding.UTF8.GetBytes(APath + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.SliceStack_SetOwnPath (Handle, bytePath));
		}

		public String GetOwnPath ()
		{
			UInt32 sizePath = 0;
			UInt32 neededPath = 0;
			CheckError(Internal.Lib3MFWrapper.SliceStack_GetOwnPath (Handle, sizePath, out neededPath, IntPtr.Zero));
			sizePath = neededPath;
			byte[] bytesPath = new byte[sizePath];
			GCHandle dataPath = GCHandle.Alloc(bytesPath, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.SliceStack_GetOwnPath (Handle, sizePath, out neededPath, dataPath.AddrOfPinnedObject()));
			dataPath.Free();
			return Encoding.UTF8.GetString(bytesPath).TrimEnd(char.MinValue);
		}

	}

	class CConsumer : CBase
	{
		public CConsumer (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public String GetConsumerID ()
		{
			UInt32 sizeConsumerID = 0;
			UInt32 neededConsumerID = 0;
			CheckError(Internal.Lib3MFWrapper.Consumer_GetConsumerID (Handle, sizeConsumerID, out neededConsumerID, IntPtr.Zero));
			sizeConsumerID = neededConsumerID;
			byte[] bytesConsumerID = new byte[sizeConsumerID];
			GCHandle dataConsumerID = GCHandle.Alloc(bytesConsumerID, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Consumer_GetConsumerID (Handle, sizeConsumerID, out neededConsumerID, dataConsumerID.AddrOfPinnedObject()));
			dataConsumerID.Free();
			return Encoding.UTF8.GetString(bytesConsumerID).TrimEnd(char.MinValue);
		}

		public String GetKeyID ()
		{
			UInt32 sizeKeyID = 0;
			UInt32 neededKeyID = 0;
			CheckError(Internal.Lib3MFWrapper.Consumer_GetKeyID (Handle, sizeKeyID, out neededKeyID, IntPtr.Zero));
			sizeKeyID = neededKeyID;
			byte[] bytesKeyID = new byte[sizeKeyID];
			GCHandle dataKeyID = GCHandle.Alloc(bytesKeyID, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Consumer_GetKeyID (Handle, sizeKeyID, out neededKeyID, dataKeyID.AddrOfPinnedObject()));
			dataKeyID.Free();
			return Encoding.UTF8.GetString(bytesKeyID).TrimEnd(char.MinValue);
		}

		public String GetKeyValue ()
		{
			UInt32 sizeKeyValue = 0;
			UInt32 neededKeyValue = 0;
			CheckError(Internal.Lib3MFWrapper.Consumer_GetKeyValue (Handle, sizeKeyValue, out neededKeyValue, IntPtr.Zero));
			sizeKeyValue = neededKeyValue;
			byte[] bytesKeyValue = new byte[sizeKeyValue];
			GCHandle dataKeyValue = GCHandle.Alloc(bytesKeyValue, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Consumer_GetKeyValue (Handle, sizeKeyValue, out neededKeyValue, dataKeyValue.AddrOfPinnedObject()));
			dataKeyValue.Free();
			return Encoding.UTF8.GetString(bytesKeyValue).TrimEnd(char.MinValue);
		}

	}

	class CAccessRight : CBase
	{
		public CAccessRight (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CConsumer GetConsumer ()
		{
			IntPtr newConsumer = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.AccessRight_GetConsumer (Handle, out newConsumer));
			return new CConsumer (newConsumer );
		}

		public eWrappingAlgorithm GetWrappingAlgorithm ()
		{
			Int32 resultAlgorithm = 0;

			CheckError(Internal.Lib3MFWrapper.AccessRight_GetWrappingAlgorithm (Handle, out resultAlgorithm));
			return (eWrappingAlgorithm) (resultAlgorithm);
		}

		public eMgfAlgorithm GetMgfAlgorithm ()
		{
			Int32 resultAlgorithm = 0;

			CheckError(Internal.Lib3MFWrapper.AccessRight_GetMgfAlgorithm (Handle, out resultAlgorithm));
			return (eMgfAlgorithm) (resultAlgorithm);
		}

		public eDigestMethod GetDigestMethod ()
		{
			Int32 resultAlgorithm = 0;

			CheckError(Internal.Lib3MFWrapper.AccessRight_GetDigestMethod (Handle, out resultAlgorithm));
			return (eDigestMethod) (resultAlgorithm);
		}

	}

	class CContentEncryptionParams : CBase
	{
		public CContentEncryptionParams (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public eEncryptionAlgorithm GetEncryptionAlgorithm ()
		{
			Int32 resultAlgorithm = 0;

			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetEncryptionAlgorithm (Handle, out resultAlgorithm));
			return (eEncryptionAlgorithm) (resultAlgorithm);
		}

		public void GetKey (out Byte[] AByteData)
		{
			UInt64 sizeByteData = 0;
			UInt64 neededByteData = 0;
			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetKey (Handle, sizeByteData, out neededByteData, IntPtr.Zero));
			sizeByteData = neededByteData;
			AByteData = new Byte[sizeByteData];
			GCHandle dataByteData = GCHandle.Alloc(AByteData, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetKey (Handle, sizeByteData, out neededByteData, dataByteData.AddrOfPinnedObject()));
			dataByteData.Free();
		}

		public void GetInitializationVector (out Byte[] AByteData)
		{
			UInt64 sizeByteData = 0;
			UInt64 neededByteData = 0;
			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetInitializationVector (Handle, sizeByteData, out neededByteData, IntPtr.Zero));
			sizeByteData = neededByteData;
			AByteData = new Byte[sizeByteData];
			GCHandle dataByteData = GCHandle.Alloc(AByteData, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetInitializationVector (Handle, sizeByteData, out neededByteData, dataByteData.AddrOfPinnedObject()));
			dataByteData.Free();
		}

		public void GetAuthenticationTag (out Byte[] AByteData)
		{
			UInt64 sizeByteData = 0;
			UInt64 neededByteData = 0;
			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetAuthenticationTag (Handle, sizeByteData, out neededByteData, IntPtr.Zero));
			sizeByteData = neededByteData;
			AByteData = new Byte[sizeByteData];
			GCHandle dataByteData = GCHandle.Alloc(AByteData, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetAuthenticationTag (Handle, sizeByteData, out neededByteData, dataByteData.AddrOfPinnedObject()));
			dataByteData.Free();
		}

		public void SetAuthenticationTag (Byte[] AByteData)
		{
			GCHandle dataByteData = GCHandle.Alloc(AByteData, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_SetAuthenticationTag (Handle, (UInt64) AByteData.Length, dataByteData.AddrOfPinnedObject()));
			dataByteData.Free ();
		}

		public void GetAdditionalAuthenticationData (out Byte[] AByteData)
		{
			UInt64 sizeByteData = 0;
			UInt64 neededByteData = 0;
			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetAdditionalAuthenticationData (Handle, sizeByteData, out neededByteData, IntPtr.Zero));
			sizeByteData = neededByteData;
			AByteData = new Byte[sizeByteData];
			GCHandle dataByteData = GCHandle.Alloc(AByteData, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetAdditionalAuthenticationData (Handle, sizeByteData, out neededByteData, dataByteData.AddrOfPinnedObject()));
			dataByteData.Free();
		}

		public UInt64 GetDescriptor ()
		{
			UInt64 resultDescriptor = 0;

			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetDescriptor (Handle, out resultDescriptor));
			return resultDescriptor;
		}

		public String GetKeyUUID ()
		{
			UInt32 sizeUUID = 0;
			UInt32 neededUUID = 0;
			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetKeyUUID (Handle, sizeUUID, out neededUUID, IntPtr.Zero));
			sizeUUID = neededUUID;
			byte[] bytesUUID = new byte[sizeUUID];
			GCHandle dataUUID = GCHandle.Alloc(bytesUUID, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.ContentEncryptionParams_GetKeyUUID (Handle, sizeUUID, out neededUUID, dataUUID.AddrOfPinnedObject()));
			dataUUID.Free();
			return Encoding.UTF8.GetString(bytesUUID).TrimEnd(char.MinValue);
		}

	}

	class CResourceData : CBase
	{
		public CResourceData (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CPackagePart GetPath ()
		{
			IntPtr newPath = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ResourceData_GetPath (Handle, out newPath));
			return new CPackagePart (newPath );
		}

		public eEncryptionAlgorithm GetEncryptionAlgorithm ()
		{
			Int32 resultEncryptionAlgorithm = 0;

			CheckError(Internal.Lib3MFWrapper.ResourceData_GetEncryptionAlgorithm (Handle, out resultEncryptionAlgorithm));
			return (eEncryptionAlgorithm) (resultEncryptionAlgorithm);
		}

		public eCompression GetCompression ()
		{
			Int32 resultCompression = 0;

			CheckError(Internal.Lib3MFWrapper.ResourceData_GetCompression (Handle, out resultCompression));
			return (eCompression) (resultCompression);
		}

		public void GetAdditionalAuthenticationData (out Byte[] AByteData)
		{
			UInt64 sizeByteData = 0;
			UInt64 neededByteData = 0;
			CheckError(Internal.Lib3MFWrapper.ResourceData_GetAdditionalAuthenticationData (Handle, sizeByteData, out neededByteData, IntPtr.Zero));
			sizeByteData = neededByteData;
			AByteData = new Byte[sizeByteData];
			GCHandle dataByteData = GCHandle.Alloc(AByteData, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.ResourceData_GetAdditionalAuthenticationData (Handle, sizeByteData, out neededByteData, dataByteData.AddrOfPinnedObject()));
			dataByteData.Free();
		}

	}

	class CResourceDataGroup : CBase
	{
		public CResourceDataGroup (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public String GetKeyUUID ()
		{
			UInt32 sizeUUID = 0;
			UInt32 neededUUID = 0;
			CheckError(Internal.Lib3MFWrapper.ResourceDataGroup_GetKeyUUID (Handle, sizeUUID, out neededUUID, IntPtr.Zero));
			sizeUUID = neededUUID;
			byte[] bytesUUID = new byte[sizeUUID];
			GCHandle dataUUID = GCHandle.Alloc(bytesUUID, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.ResourceDataGroup_GetKeyUUID (Handle, sizeUUID, out neededUUID, dataUUID.AddrOfPinnedObject()));
			dataUUID.Free();
			return Encoding.UTF8.GetString(bytesUUID).TrimEnd(char.MinValue);
		}

		public CAccessRight AddAccessRight (CConsumer AConsumer, eWrappingAlgorithm AWrappingAlgorithm, eMgfAlgorithm AMgfAlgorithm, eDigestMethod ADigestMethod)
		{
			Int32 enumWrappingAlgorithm = (Int32) AWrappingAlgorithm;
			Int32 enumMgfAlgorithm = (Int32) AMgfAlgorithm;
			Int32 enumDigestMethod = (Int32) ADigestMethod;
			IntPtr newTheAccessRight = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ResourceDataGroup_AddAccessRight (Handle, AConsumer.GetHandle(), enumWrappingAlgorithm, enumMgfAlgorithm, enumDigestMethod, out newTheAccessRight));
			return new CAccessRight (newTheAccessRight );
		}

		public CAccessRight FindAccessRightByConsumer (CConsumer AConsumer)
		{
			IntPtr newTheAccessRight = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.ResourceDataGroup_FindAccessRightByConsumer (Handle, AConsumer.GetHandle(), out newTheAccessRight));
			return new CAccessRight (newTheAccessRight );
		}

		public void RemoveAccessRight (CConsumer AConsumer)
		{

			CheckError(Internal.Lib3MFWrapper.ResourceDataGroup_RemoveAccessRight (Handle, AConsumer.GetHandle()));
		}

	}

	class CKeyStore : CBase
	{
		public CKeyStore (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CConsumer AddConsumer (String AConsumerID, String AKeyID, String AKeyValue)
		{
			byte[] byteConsumerID = Encoding.UTF8.GetBytes(AConsumerID + char.MinValue);
			byte[] byteKeyID = Encoding.UTF8.GetBytes(AKeyID + char.MinValue);
			byte[] byteKeyValue = Encoding.UTF8.GetBytes(AKeyValue + char.MinValue);
			IntPtr newConsumer = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.KeyStore_AddConsumer (Handle, byteConsumerID, byteKeyID, byteKeyValue, out newConsumer));
			return new CConsumer (newConsumer );
		}

		public UInt64 GetConsumerCount ()
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.KeyStore_GetConsumerCount (Handle, out resultCount));
			return resultCount;
		}

		public CConsumer GetConsumer (UInt64 AConsumerIndex)
		{
			IntPtr newConsumer = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.KeyStore_GetConsumer (Handle, AConsumerIndex, out newConsumer));
			return new CConsumer (newConsumer );
		}

		public void RemoveConsumer (CConsumer AConsumer)
		{

			CheckError(Internal.Lib3MFWrapper.KeyStore_RemoveConsumer (Handle, AConsumer.GetHandle()));
		}

		public CConsumer FindConsumer (String AConsumerID)
		{
			byte[] byteConsumerID = Encoding.UTF8.GetBytes(AConsumerID + char.MinValue);
			IntPtr newConsumer = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.KeyStore_FindConsumer (Handle, byteConsumerID, out newConsumer));
			return new CConsumer (newConsumer );
		}

		public UInt64 GetResourceDataGroupCount ()
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.KeyStore_GetResourceDataGroupCount (Handle, out resultCount));
			return resultCount;
		}

		public CResourceDataGroup AddResourceDataGroup ()
		{
			IntPtr newResourceDataGroup = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.KeyStore_AddResourceDataGroup (Handle, out newResourceDataGroup));
			return new CResourceDataGroup (newResourceDataGroup );
		}

		public CResourceDataGroup GetResourceDataGroup (UInt64 AResourceDataIndex)
		{
			IntPtr newResourceDataGroup = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.KeyStore_GetResourceDataGroup (Handle, AResourceDataIndex, out newResourceDataGroup));
			return new CResourceDataGroup (newResourceDataGroup );
		}

		public void RemoveResourceDataGroup (CResourceDataGroup AResourceDataGroup)
		{

			CheckError(Internal.Lib3MFWrapper.KeyStore_RemoveResourceDataGroup (Handle, AResourceDataGroup.GetHandle()));
		}

		public CResourceDataGroup FindResourceDataGroup (CPackagePart APartPath)
		{
			IntPtr newResourceDataGroup = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.KeyStore_FindResourceDataGroup (Handle, APartPath.GetHandle(), out newResourceDataGroup));
			return new CResourceDataGroup (newResourceDataGroup );
		}

		public CResourceData AddResourceData (CResourceDataGroup AResourceDataGroup, CPackagePart APartPath, eEncryptionAlgorithm AAlgorithm, eCompression ACompression, Byte[] AAdditionalAuthenticationData)
		{
			Int32 enumAlgorithm = (Int32) AAlgorithm;
			Int32 enumCompression = (Int32) ACompression;
			GCHandle dataAdditionalAuthenticationData = GCHandle.Alloc(AAdditionalAuthenticationData, GCHandleType.Pinned);
			IntPtr newResourceData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.KeyStore_AddResourceData (Handle, AResourceDataGroup.GetHandle(), APartPath.GetHandle(), enumAlgorithm, enumCompression, (UInt64) AAdditionalAuthenticationData.Length, dataAdditionalAuthenticationData.AddrOfPinnedObject(), out newResourceData));
			dataAdditionalAuthenticationData.Free ();
			return new CResourceData (newResourceData );
		}

		public void RemoveResourceData (CResourceData AResourceData)
		{

			CheckError(Internal.Lib3MFWrapper.KeyStore_RemoveResourceData (Handle, AResourceData.GetHandle()));
		}

		public CResourceData FindResourceData (CPackagePart AResourcePath)
		{
			IntPtr newResourceData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.KeyStore_FindResourceData (Handle, AResourcePath.GetHandle(), out newResourceData));
			return new CResourceData (newResourceData );
		}

		public UInt64 GetResourceDataCount ()
		{
			UInt64 resultCount = 0;

			CheckError(Internal.Lib3MFWrapper.KeyStore_GetResourceDataCount (Handle, out resultCount));
			return resultCount;
		}

		public CResourceData GetResourceData (UInt64 AResourceDataIndex)
		{
			IntPtr newResourceData = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.KeyStore_GetResourceData (Handle, AResourceDataIndex, out newResourceData));
			return new CResourceData (newResourceData );
		}

		public String GetUUID (out bool AHasUUID)
		{
			Byte resultHasUUID = 0;
			UInt32 sizeUUID = 0;
			UInt32 neededUUID = 0;
			CheckError(Internal.Lib3MFWrapper.KeyStore_GetUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, IntPtr.Zero));
			sizeUUID = neededUUID;
			byte[] bytesUUID = new byte[sizeUUID];
			GCHandle dataUUID = GCHandle.Alloc(bytesUUID, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.KeyStore_GetUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, dataUUID.AddrOfPinnedObject()));
			AHasUUID = (resultHasUUID != 0);
			dataUUID.Free();
			return Encoding.UTF8.GetString(bytesUUID).TrimEnd(char.MinValue);
		}

		public void SetUUID (String AUUID)
		{
			byte[] byteUUID = Encoding.UTF8.GetBytes(AUUID + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.KeyStore_SetUUID (Handle, byteUUID));
		}

	}

	class CModel : CBase
	{
		public CModel (IntPtr NewHandle) : base (NewHandle)
		{
		}

		public CPackagePart RootModelPart ()
		{
			IntPtr newRootModelPart = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_RootModelPart (Handle, out newRootModelPart));
			return new CPackagePart (newRootModelPart );
		}

		public CPackagePart FindOrCreatePackagePart (String AAbsolutePath)
		{
			byte[] byteAbsolutePath = Encoding.UTF8.GetBytes(AAbsolutePath + char.MinValue);
			IntPtr newModelPart = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_FindOrCreatePackagePart (Handle, byteAbsolutePath, out newModelPart));
			return new CPackagePart (newModelPart );
		}

		public void SetUnit (eModelUnit AUnit)
		{
			Int32 enumUnit = (Int32) AUnit;

			CheckError(Internal.Lib3MFWrapper.Model_SetUnit (Handle, enumUnit));
		}

		public eModelUnit GetUnit ()
		{
			Int32 resultUnit = 0;

			CheckError(Internal.Lib3MFWrapper.Model_GetUnit (Handle, out resultUnit));
			return (eModelUnit) (resultUnit);
		}

		public String GetLanguage ()
		{
			UInt32 sizeLanguage = 0;
			UInt32 neededLanguage = 0;
			CheckError(Internal.Lib3MFWrapper.Model_GetLanguage (Handle, sizeLanguage, out neededLanguage, IntPtr.Zero));
			sizeLanguage = neededLanguage;
			byte[] bytesLanguage = new byte[sizeLanguage];
			GCHandle dataLanguage = GCHandle.Alloc(bytesLanguage, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Model_GetLanguage (Handle, sizeLanguage, out neededLanguage, dataLanguage.AddrOfPinnedObject()));
			dataLanguage.Free();
			return Encoding.UTF8.GetString(bytesLanguage).TrimEnd(char.MinValue);
		}

		public void SetLanguage (String ALanguage)
		{
			byte[] byteLanguage = Encoding.UTF8.GetBytes(ALanguage + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Model_SetLanguage (Handle, byteLanguage));
		}

		public CWriter QueryWriter (String AWriterClass)
		{
			byte[] byteWriterClass = Encoding.UTF8.GetBytes(AWriterClass + char.MinValue);
			IntPtr newWriterInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_QueryWriter (Handle, byteWriterClass, out newWriterInstance));
			return new CWriter (newWriterInstance );
		}

		public CReader QueryReader (String AReaderClass)
		{
			byte[] byteReaderClass = Encoding.UTF8.GetBytes(AReaderClass + char.MinValue);
			IntPtr newReaderInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_QueryReader (Handle, byteReaderClass, out newReaderInstance));
			return new CReader (newReaderInstance );
		}

		public CTexture2D GetTexture2DByID (UInt32 AUniqueResourceID)
		{
			IntPtr newTextureInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetTexture2DByID (Handle, AUniqueResourceID, out newTextureInstance));
			return new CTexture2D (newTextureInstance );
		}

		public ePropertyType GetPropertyTypeByID (UInt32 AUniqueResourceID)
		{
			Int32 resultThePropertyType = 0;

			CheckError(Internal.Lib3MFWrapper.Model_GetPropertyTypeByID (Handle, AUniqueResourceID, out resultThePropertyType));
			return (ePropertyType) (resultThePropertyType);
		}

		public CBaseMaterialGroup GetBaseMaterialGroupByID (UInt32 AUniqueResourceID)
		{
			IntPtr newBaseMaterialGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetBaseMaterialGroupByID (Handle, AUniqueResourceID, out newBaseMaterialGroupInstance));
			return new CBaseMaterialGroup (newBaseMaterialGroupInstance );
		}

		public CTexture2DGroup GetTexture2DGroupByID (UInt32 AUniqueResourceID)
		{
			IntPtr newTexture2DGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetTexture2DGroupByID (Handle, AUniqueResourceID, out newTexture2DGroupInstance));
			return new CTexture2DGroup (newTexture2DGroupInstance );
		}

		public CCompositeMaterials GetCompositeMaterialsByID (UInt32 AUniqueResourceID)
		{
			IntPtr newCompositeMaterialsInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetCompositeMaterialsByID (Handle, AUniqueResourceID, out newCompositeMaterialsInstance));
			return new CCompositeMaterials (newCompositeMaterialsInstance );
		}

		public CMultiPropertyGroup GetMultiPropertyGroupByID (UInt32 AUniqueResourceID)
		{
			IntPtr newMultiPropertyGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetMultiPropertyGroupByID (Handle, AUniqueResourceID, out newMultiPropertyGroupInstance));
			return new CMultiPropertyGroup (newMultiPropertyGroupInstance );
		}

		public CMeshObject GetMeshObjectByID (UInt32 AUniqueResourceID)
		{
			IntPtr newMeshObjectInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetMeshObjectByID (Handle, AUniqueResourceID, out newMeshObjectInstance));
			return new CMeshObject (newMeshObjectInstance );
		}

		public CComponentsObject GetComponentsObjectByID (UInt32 AUniqueResourceID)
		{
			IntPtr newComponentsObjectInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetComponentsObjectByID (Handle, AUniqueResourceID, out newComponentsObjectInstance));
			return new CComponentsObject (newComponentsObjectInstance );
		}

		public CColorGroup GetColorGroupByID (UInt32 AUniqueResourceID)
		{
			IntPtr newColorGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetColorGroupByID (Handle, AUniqueResourceID, out newColorGroupInstance));
			return new CColorGroup (newColorGroupInstance );
		}

		public CSliceStack GetSliceStackByID (UInt32 AUniqueResourceID)
		{
			IntPtr newSliceStacInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetSliceStackByID (Handle, AUniqueResourceID, out newSliceStacInstance));
			return new CSliceStack (newSliceStacInstance );
		}

		public String GetBuildUUID (out bool AHasUUID)
		{
			Byte resultHasUUID = 0;
			UInt32 sizeUUID = 0;
			UInt32 neededUUID = 0;
			CheckError(Internal.Lib3MFWrapper.Model_GetBuildUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, IntPtr.Zero));
			sizeUUID = neededUUID;
			byte[] bytesUUID = new byte[sizeUUID];
			GCHandle dataUUID = GCHandle.Alloc(bytesUUID, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.Model_GetBuildUUID (Handle, out resultHasUUID, sizeUUID, out neededUUID, dataUUID.AddrOfPinnedObject()));
			AHasUUID = (resultHasUUID != 0);
			dataUUID.Free();
			return Encoding.UTF8.GetString(bytesUUID).TrimEnd(char.MinValue);
		}

		public void SetBuildUUID (String AUUID)
		{
			byte[] byteUUID = Encoding.UTF8.GetBytes(AUUID + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Model_SetBuildUUID (Handle, byteUUID));
		}

		public CBuildItemIterator GetBuildItems ()
		{
			IntPtr newBuildItemIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetBuildItems (Handle, out newBuildItemIterator));
			return new CBuildItemIterator (newBuildItemIterator );
		}

		public sBox GetOutbox ()
		{
			Internal.InternalBox intresultOutbox;

			CheckError(Internal.Lib3MFWrapper.Model_GetOutbox (Handle, out intresultOutbox));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Box (intresultOutbox);
		}

		public CResourceIterator GetResources ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetResources (Handle, out newResourceIterator));
			return new CResourceIterator (newResourceIterator );
		}

		public CObjectIterator GetObjects ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetObjects (Handle, out newResourceIterator));
			return new CObjectIterator (newResourceIterator );
		}

		public CMeshObjectIterator GetMeshObjects ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetMeshObjects (Handle, out newResourceIterator));
			return new CMeshObjectIterator (newResourceIterator );
		}

		public CComponentsObjectIterator GetComponentsObjects ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetComponentsObjects (Handle, out newResourceIterator));
			return new CComponentsObjectIterator (newResourceIterator );
		}

		public CTexture2DIterator GetTexture2Ds ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetTexture2Ds (Handle, out newResourceIterator));
			return new CTexture2DIterator (newResourceIterator );
		}

		public CBaseMaterialGroupIterator GetBaseMaterialGroups ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetBaseMaterialGroups (Handle, out newResourceIterator));
			return new CBaseMaterialGroupIterator (newResourceIterator );
		}

		public CColorGroupIterator GetColorGroups ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetColorGroups (Handle, out newResourceIterator));
			return new CColorGroupIterator (newResourceIterator );
		}

		public CTexture2DGroupIterator GetTexture2DGroups ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetTexture2DGroups (Handle, out newResourceIterator));
			return new CTexture2DGroupIterator (newResourceIterator );
		}

		public CCompositeMaterialsIterator GetCompositeMaterials ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetCompositeMaterials (Handle, out newResourceIterator));
			return new CCompositeMaterialsIterator (newResourceIterator );
		}

		public CMultiPropertyGroupIterator GetMultiPropertyGroups ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetMultiPropertyGroups (Handle, out newResourceIterator));
			return new CMultiPropertyGroupIterator (newResourceIterator );
		}

		public CSliceStackIterator GetSliceStacks ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetSliceStacks (Handle, out newResourceIterator));
			return new CSliceStackIterator (newResourceIterator );
		}

		public CImage3DIterator GetImage3Ds ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetImage3Ds (Handle, out newResourceIterator));
			return new CImage3DIterator (newResourceIterator );
		}

		public CModel MergeToModel ()
		{
			IntPtr newMergedModelInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_MergeToModel (Handle, out newMergedModelInstance));
			return new CModel (newMergedModelInstance );
		}

		public CVolumetricStackIterator GetVolumetricStacks ()
		{
			IntPtr newResourceIterator = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetVolumetricStacks (Handle, out newResourceIterator));
			return new CVolumetricStackIterator (newResourceIterator );
		}

		public CMeshObject AddMeshObject ()
		{
			IntPtr newMeshObjectInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddMeshObject (Handle, out newMeshObjectInstance));
			return new CMeshObject (newMeshObjectInstance );
		}

		public CComponentsObject AddComponentsObject ()
		{
			IntPtr newComponentsObjectInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddComponentsObject (Handle, out newComponentsObjectInstance));
			return new CComponentsObject (newComponentsObjectInstance );
		}

		public CSliceStack AddSliceStack (Double AZBottom)
		{
			IntPtr newSliceStackInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddSliceStack (Handle, AZBottom, out newSliceStackInstance));
			return new CSliceStack (newSliceStackInstance );
		}

		public CTexture2D AddTexture2DFromAttachment (CAttachment ATextureAttachment)
		{
			IntPtr newTexture2DInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddTexture2DFromAttachment (Handle, ATextureAttachment.GetHandle(), out newTexture2DInstance));
			return new CTexture2D (newTexture2DInstance );
		}

		public CBaseMaterialGroup AddBaseMaterialGroup ()
		{
			IntPtr newBaseMaterialGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddBaseMaterialGroup (Handle, out newBaseMaterialGroupInstance));
			return new CBaseMaterialGroup (newBaseMaterialGroupInstance );
		}

		public CColorGroup AddColorGroup ()
		{
			IntPtr newColorGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddColorGroup (Handle, out newColorGroupInstance));
			return new CColorGroup (newColorGroupInstance );
		}

		public CTexture2DGroup AddTexture2DGroup (CTexture2D ATexture2DInstance)
		{
			IntPtr newTexture2DGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddTexture2DGroup (Handle, ATexture2DInstance.GetHandle(), out newTexture2DGroupInstance));
			return new CTexture2DGroup (newTexture2DGroupInstance );
		}

		public CCompositeMaterials AddCompositeMaterials (CBaseMaterialGroup ABaseMaterialGroupInstance)
		{
			IntPtr newCompositeMaterialsInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddCompositeMaterials (Handle, ABaseMaterialGroupInstance.GetHandle(), out newCompositeMaterialsInstance));
			return new CCompositeMaterials (newCompositeMaterialsInstance );
		}

		public CMultiPropertyGroup AddMultiPropertyGroup ()
		{
			IntPtr newMultiPropertyGroupInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddMultiPropertyGroup (Handle, out newMultiPropertyGroupInstance));
			return new CMultiPropertyGroup (newMultiPropertyGroupInstance );
		}

		public CImage3D AddImage3D (UInt32 ASizeX, UInt32 ASizeY, UInt32 ASheetCount)
		{
			IntPtr newInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddImage3D (Handle, ASizeX, ASizeY, ASheetCount, out newInstance));
			return new CImage3D (newInstance );
		}

		public CVolumetricStack AddVolumetricStack ()
		{
			IntPtr newInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddVolumetricStack (Handle, out newInstance));
			return new CVolumetricStack (newInstance );
		}

		public CBuildItem AddBuildItem (CObject AObject, sTransform ATransform)
		{
			Internal.InternalTransform intTransform = Internal.Lib3MFWrapper.convertStructToInternal_Transform (ATransform);
			IntPtr newBuildItemInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddBuildItem (Handle, AObject.GetHandle(), intTransform, out newBuildItemInstance));
			return new CBuildItem (newBuildItemInstance );
		}

		public void RemoveBuildItem (CBuildItem ABuildItemInstance)
		{

			CheckError(Internal.Lib3MFWrapper.Model_RemoveBuildItem (Handle, ABuildItemInstance.GetHandle()));
		}

		public CMetaDataGroup GetMetaDataGroup ()
		{
			IntPtr newTheMetaDataGroup = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetMetaDataGroup (Handle, out newTheMetaDataGroup));
			return new CMetaDataGroup (newTheMetaDataGroup );
		}

		public CAttachment AddAttachment (String AURI, String ARelationShipType)
		{
			byte[] byteURI = Encoding.UTF8.GetBytes(AURI + char.MinValue);
			byte[] byteRelationShipType = Encoding.UTF8.GetBytes(ARelationShipType + char.MinValue);
			IntPtr newAttachmentInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_AddAttachment (Handle, byteURI, byteRelationShipType, out newAttachmentInstance));
			return new CAttachment (newAttachmentInstance );
		}

		public void RemoveAttachment (CAttachment AAttachmentInstance)
		{

			CheckError(Internal.Lib3MFWrapper.Model_RemoveAttachment (Handle, AAttachmentInstance.GetHandle()));
		}

		public CAttachment GetAttachment (UInt32 AIndex)
		{
			IntPtr newAttachmentInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetAttachment (Handle, AIndex, out newAttachmentInstance));
			return new CAttachment (newAttachmentInstance );
		}

		public CAttachment FindAttachment (String AURI)
		{
			byte[] byteURI = Encoding.UTF8.GetBytes(AURI + char.MinValue);
			IntPtr newAttachmentInstance = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_FindAttachment (Handle, byteURI, out newAttachmentInstance));
			return new CAttachment (newAttachmentInstance );
		}

		public UInt32 GetAttachmentCount ()
		{
			UInt32 resultAttachmentCount = 0;

			CheckError(Internal.Lib3MFWrapper.Model_GetAttachmentCount (Handle, out resultAttachmentCount));
			return resultAttachmentCount;
		}

		public bool HasPackageThumbnailAttachment ()
		{
			Byte resultHasThumbnail = 0;

			CheckError(Internal.Lib3MFWrapper.Model_HasPackageThumbnailAttachment (Handle, out resultHasThumbnail));
			return (resultHasThumbnail != 0);
		}

		public CAttachment CreatePackageThumbnailAttachment ()
		{
			IntPtr newAttachment = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_CreatePackageThumbnailAttachment (Handle, out newAttachment));
			return new CAttachment (newAttachment );
		}

		public CAttachment GetPackageThumbnailAttachment ()
		{
			IntPtr newAttachment = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetPackageThumbnailAttachment (Handle, out newAttachment));
			return new CAttachment (newAttachment );
		}

		public void RemovePackageThumbnailAttachment ()
		{

			CheckError(Internal.Lib3MFWrapper.Model_RemovePackageThumbnailAttachment (Handle));
		}

		public void AddCustomContentType (String AExtension, String AContentType)
		{
			byte[] byteExtension = Encoding.UTF8.GetBytes(AExtension + char.MinValue);
			byte[] byteContentType = Encoding.UTF8.GetBytes(AContentType + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Model_AddCustomContentType (Handle, byteExtension, byteContentType));
		}

		public void RemoveCustomContentType (String AExtension)
		{
			byte[] byteExtension = Encoding.UTF8.GetBytes(AExtension + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.Model_RemoveCustomContentType (Handle, byteExtension));
		}

		public void SetRandomNumberCallback (IntPtr ATheCallback, UInt64 AUserData)
		{

			CheckError(Internal.Lib3MFWrapper.Model_SetRandomNumberCallback (Handle, IntPtr.Zero, AUserData));
		}

		public CKeyStore GetKeyStore ()
		{
			IntPtr newKeyStore = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.Model_GetKeyStore (Handle, out newKeyStore));
			return new CKeyStore (newKeyStore );
		}

	}

	class Wrapper
	{
		private static void CheckError (Int32 errorCode)
		{
			if (errorCode != 0) {
				Internal.Lib3MFWrapper.ThrowError (IntPtr.Zero, errorCode);
			}
		}

		public static void GetLibraryVersion (out UInt32 AMajor, out UInt32 AMinor, out UInt32 AMicro)
		{

			CheckError(Internal.Lib3MFWrapper.GetLibraryVersion (out AMajor, out AMinor, out AMicro));
		}

		public static bool GetPrereleaseInformation (out String APrereleaseInfo)
		{
			Byte resultHasPrereleaseInfo = 0;
			UInt32 sizePrereleaseInfo = 0;
			UInt32 neededPrereleaseInfo = 0;
			CheckError(Internal.Lib3MFWrapper.GetPrereleaseInformation (out resultHasPrereleaseInfo, sizePrereleaseInfo, out neededPrereleaseInfo, IntPtr.Zero));
			sizePrereleaseInfo = neededPrereleaseInfo;
			byte[] bytesPrereleaseInfo = new byte[sizePrereleaseInfo];
			GCHandle dataPrereleaseInfo = GCHandle.Alloc(bytesPrereleaseInfo, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.GetPrereleaseInformation (out resultHasPrereleaseInfo, sizePrereleaseInfo, out neededPrereleaseInfo, dataPrereleaseInfo.AddrOfPinnedObject()));
			dataPrereleaseInfo.Free();
			APrereleaseInfo = Encoding.UTF8.GetString(bytesPrereleaseInfo).TrimEnd(char.MinValue);
			return (resultHasPrereleaseInfo != 0);
		}

		public static bool GetBuildInformation (out String ABuildInformation)
		{
			Byte resultHasBuildInfo = 0;
			UInt32 sizeBuildInformation = 0;
			UInt32 neededBuildInformation = 0;
			CheckError(Internal.Lib3MFWrapper.GetBuildInformation (out resultHasBuildInfo, sizeBuildInformation, out neededBuildInformation, IntPtr.Zero));
			sizeBuildInformation = neededBuildInformation;
			byte[] bytesBuildInformation = new byte[sizeBuildInformation];
			GCHandle dataBuildInformation = GCHandle.Alloc(bytesBuildInformation, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.GetBuildInformation (out resultHasBuildInfo, sizeBuildInformation, out neededBuildInformation, dataBuildInformation.AddrOfPinnedObject()));
			dataBuildInformation.Free();
			ABuildInformation = Encoding.UTF8.GetString(bytesBuildInformation).TrimEnd(char.MinValue);
			return (resultHasBuildInfo != 0);
		}

		public static void GetSpecificationVersion (String ASpecificationURL, out bool AIsSupported, out UInt32 AMajor, out UInt32 AMinor, out UInt32 AMicro)
		{
			byte[] byteSpecificationURL = Encoding.UTF8.GetBytes(ASpecificationURL + char.MinValue);
			Byte resultIsSupported = 0;

			CheckError(Internal.Lib3MFWrapper.GetSpecificationVersion (byteSpecificationURL, out resultIsSupported, out AMajor, out AMinor, out AMicro));
			AIsSupported = (resultIsSupported != 0);
		}

		public static CModel CreateModel ()
		{
			IntPtr newModel = IntPtr.Zero;

			CheckError(Internal.Lib3MFWrapper.CreateModel (out newModel));
			return new CModel (newModel );
		}

		public static void Release (CBase AInstance)
		{

			CheckError(Internal.Lib3MFWrapper.Release (AInstance.GetHandle()));
		}

		public static void Acquire (CBase AInstance)
		{

			CheckError(Internal.Lib3MFWrapper.Acquire (AInstance.GetHandle()));
		}

		public static void SetJournal (String AJournalPath)
		{
			byte[] byteJournalPath = Encoding.UTF8.GetBytes(AJournalPath + char.MinValue);

			CheckError(Internal.Lib3MFWrapper.SetJournal (byteJournalPath));
		}

		public static bool GetLastError (CBase AInstance, out String ALastErrorString)
		{
			Byte resultHasLastError = 0;
			UInt32 sizeLastErrorString = 0;
			UInt32 neededLastErrorString = 0;
			CheckError(Internal.Lib3MFWrapper.GetLastError (AInstance.GetHandle(), sizeLastErrorString, out neededLastErrorString, IntPtr.Zero, out resultHasLastError));
			sizeLastErrorString = neededLastErrorString;
			byte[] bytesLastErrorString = new byte[sizeLastErrorString];
			GCHandle dataLastErrorString = GCHandle.Alloc(bytesLastErrorString, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.GetLastError (AInstance.GetHandle(), sizeLastErrorString, out neededLastErrorString, dataLastErrorString.AddrOfPinnedObject(), out resultHasLastError));
			dataLastErrorString.Free();
			ALastErrorString = Encoding.UTF8.GetString(bytesLastErrorString).TrimEnd(char.MinValue);
			return (resultHasLastError != 0);
		}

		public static UInt64 GetSymbolLookupMethod ()
		{
			UInt64 resultSymbolLookupMethod = 0;

			CheckError(Internal.Lib3MFWrapper.GetSymbolLookupMethod (out resultSymbolLookupMethod));
			return resultSymbolLookupMethod;
		}

		public static void RetrieveProgressMessage (eProgressIdentifier ATheProgressIdentifier, out String AProgressMessage)
		{
			Int32 enumTheProgressIdentifier = (Int32) ATheProgressIdentifier;
			UInt32 sizeProgressMessage = 0;
			UInt32 neededProgressMessage = 0;
			CheckError(Internal.Lib3MFWrapper.RetrieveProgressMessage (enumTheProgressIdentifier, sizeProgressMessage, out neededProgressMessage, IntPtr.Zero));
			sizeProgressMessage = neededProgressMessage;
			byte[] bytesProgressMessage = new byte[sizeProgressMessage];
			GCHandle dataProgressMessage = GCHandle.Alloc(bytesProgressMessage, GCHandleType.Pinned);

			CheckError(Internal.Lib3MFWrapper.RetrieveProgressMessage (enumTheProgressIdentifier, sizeProgressMessage, out neededProgressMessage, dataProgressMessage.AddrOfPinnedObject()));
			dataProgressMessage.Free();
			AProgressMessage = Encoding.UTF8.GetString(bytesProgressMessage).TrimEnd(char.MinValue);
		}

		public static sColor RGBAToColor (Byte ARed, Byte AGreen, Byte ABlue, Byte AAlpha)
		{
			Internal.InternalColor intresultTheColor;

			CheckError(Internal.Lib3MFWrapper.RGBAToColor (ARed, AGreen, ABlue, AAlpha, out intresultTheColor));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Color (intresultTheColor);
		}

		public static sColor FloatRGBAToColor (Single ARed, Single AGreen, Single ABlue, Single AAlpha)
		{
			Internal.InternalColor intresultTheColor;

			CheckError(Internal.Lib3MFWrapper.FloatRGBAToColor (ARed, AGreen, ABlue, AAlpha, out intresultTheColor));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Color (intresultTheColor);
		}

		public static void ColorToRGBA (sColor ATheColor, out Byte ARed, out Byte AGreen, out Byte ABlue, out Byte AAlpha)
		{
			Internal.InternalColor intTheColor = Internal.Lib3MFWrapper.convertStructToInternal_Color (ATheColor);

			CheckError(Internal.Lib3MFWrapper.ColorToRGBA (intTheColor, out ARed, out AGreen, out ABlue, out AAlpha));
		}

		public static void ColorToFloatRGBA (sColor ATheColor, out Single ARed, out Single AGreen, out Single ABlue, out Single AAlpha)
		{
			Internal.InternalColor intTheColor = Internal.Lib3MFWrapper.convertStructToInternal_Color (ATheColor);

			CheckError(Internal.Lib3MFWrapper.ColorToFloatRGBA (intTheColor, out ARed, out AGreen, out ABlue, out AAlpha));
		}

		public static sTransform GetIdentityTransform ()
		{
			Internal.InternalTransform intresultTransform;

			CheckError(Internal.Lib3MFWrapper.GetIdentityTransform (out intresultTransform));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Transform (intresultTransform);
		}

		public static sTransform GetUniformScaleTransform (Single AFactor)
		{
			Internal.InternalTransform intresultTransform;

			CheckError(Internal.Lib3MFWrapper.GetUniformScaleTransform (AFactor, out intresultTransform));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Transform (intresultTransform);
		}

		public static sTransform GetScaleTransform (Single AFactorX, Single AFactorY, Single AFactorZ)
		{
			Internal.InternalTransform intresultTransform;

			CheckError(Internal.Lib3MFWrapper.GetScaleTransform (AFactorX, AFactorY, AFactorZ, out intresultTransform));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Transform (intresultTransform);
		}

		public static sTransform GetTranslationTransform (Single AVectorX, Single AVectorY, Single AVectorZ)
		{
			Internal.InternalTransform intresultTransform;

			CheckError(Internal.Lib3MFWrapper.GetTranslationTransform (AVectorX, AVectorY, AVectorZ, out intresultTransform));
			return Internal.Lib3MFWrapper.convertInternalToStruct_Transform (intresultTransform);
		}

	}

}
