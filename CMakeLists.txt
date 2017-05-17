cmake_minimum_required (VERSION 2.8)
project (lib3MF)

IF (NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 3.1)
  cmake_policy(SET CMP0054 OLD)
ENDIF ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  add_definitions(-DBUILD_DLL -DNMR_COM_EMULATION)
  add_compile_options(-Wall)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_definitions(-DBUILD_DLL -DNMR_COM_EMULATION)
  add_compile_options(-Wall) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
  set(CMAKE_MACOSX_RPATH ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  add_definitions(-DBUILD_DLL -DLIB3MF_EXPORTS)
  add_definitions(/W3)
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} xmllite.lib")

  string(REGEX REPLACE "/bin/[^/]*$" "" VCINSTALLDIR "${CMAKE_C_COMPILER}")
  message(STATUS "Guessed MSVC directory: ${VCINSTALLDIR}")
  link_directories ("${VCINSTALLDIR}/../../../UnitTest/lib")
endif()


set(SRCS_WIN_MAIN
./Source/Main/dllmain.cpp
)

set(SRCS_WIN_MANAGEDTEST
./Source/UnitTests/UnitTest_Geometry.cpp
./Source/UnitTests/UnitTest_Mesh.cpp
./Source/UnitTests/UnitTest_MeshFormat_STL.cpp
./Source/UnitTests/UnitTest_Model.cpp
./Source/UnitTests/UnitTest_ModelFactory.cpp
)

set(SRCS_WINONLY
./Source/Common/Platform/NMR_ExportStream_COM.cpp
./Source/Common/Platform/NMR_ImportStream_COM.cpp
./Source/Common/Platform/NMR_Platform_COM.cpp
./Source/Common/Platform/NMR_XmlReader_COM.cpp
./Source/Common/Platform/NMR_XmlWriter_COM.cpp
./Source/Model/Reader/NMR_ModelReader_3MF_OPC.cpp
)

set(SRCS_UNIX_MAIN
./Source/Main/main.cpp
)
file(GLOB LIBZIP_FILES ./Source/Libraries/libzip/*.c)
set(SRCS_UNIXONLY
./Source/Model/COM/NMR_COMInterface_GCC.cpp
./Source/Common/Platform/NMR_ExportStream_GCC_Native.cpp
./Source/Common/Platform/NMR_ExportStream_GCC_Win32.cpp
./Source/Common/Platform/NMR_ExportStream_ZIP.cpp
./Source/Common/Platform/NMR_ImportStream_GCC_Native.cpp
./Source/Common/Platform/NMR_ImportStream_GCC_Win32.cpp
./Source/Common/Platform/NMR_ImportStream_ZIP.cpp
./Source/Model/Reader/NMR_ModelReader_3MF_Native.cpp
./Source/Common/OPC/NMR_OpcPackageReader.cpp
./Source/Common/OPC/NMR_OpcPackageRelationshipReader.cpp
./Source/Common/Platform/NMR_Platform_GCC.cpp
./Source/Common/Platform/NMR_XmlReader_Native.cpp
${LIBZIP_FILES}
)

set(SRCS_COMMON
./Source/Common/Math/NMR_Matrix.cpp
./Source/Common/Math/NMR_PairMatchingTree.cpp
./Source/Common/Math/NMR_Vector.cpp
./Source/Common/Math/NMR_VectorTree.cpp
./Source/Common/MeshExport/NMR_MeshExporter.cpp
./Source/Common/MeshExport/NMR_MeshExporter_STL.cpp
./Source/Common/MeshImport/NMR_MeshImporter.cpp
./Source/Common/MeshImport/NMR_MeshImporter_STL.cpp
./Source/Common/MeshInformation/NMR_MeshInformation.cpp
./Source/Common/MeshInformation/NMR_MeshInformationContainer.cpp
./Source/Common/MeshInformation/NMR_MeshInformationFactory.cpp
./Source/Common/MeshInformation/NMR_MeshInformationHandler.cpp
./Source/Common/MeshInformation/NMR_MeshInformation_BaseMaterials.cpp
./Source/Common/MeshInformation/NMR_MeshInformation_NodeColors.cpp
./Source/Common/MeshInformation/NMR_MeshInformation_TexCoords.cpp
./Source/Common/Mesh/NMR_Mesh.cpp
./Source/Common/Mesh/NMR_MeshBuilder.cpp
./Source/Common/NMR_Exception.cpp
./Source/Common/NMR_Exception_Windows.cpp
./Source/Common/NMR_StringUtils.cpp
./Source/Common/OPC/NMR_OpcPackagePart.cpp
./Source/Common/OPC/NMR_OpcPackageRelationship.cpp
./Source/Common/OPC/NMR_OpcPackageWriter.cpp
./Source/Common/Platform/NMR_ExportStream.cpp
./Source/Common/Platform/NMR_ExportStream_Callback.cpp
./Source/Common/Platform/NMR_ExportStream_GCC.cpp
./Source/Common/Platform/NMR_ExportStream_ZIP.cpp
./Source/Common/Platform/NMR_ImportStream_GCC.cpp
./Source/Common/Platform/NMR_ImportStream_Memory.cpp
./Source/Common/Platform/NMR_PortableZIPWriter.cpp
./Source/Common/Platform/NMR_PortableZIPWriterEntry.cpp
./Source/Common/Platform/NMR_Time.cpp
./Source/Common/Platform/NMR_XmlReader.cpp
./Source/Common/Platform/NMR_XmlReader_GCC.cpp
./Source/Common/Platform/NMR_XmlWriter.cpp
./Source/Common/Platform/NMR_XmlWriter_GCC.cpp
./Source/Common/Platform/NMR_XmlWriter_Native.cpp
./Source/Libraries/zlib/adler32.c
./Source/Libraries/zlib/compress.c
./Source/Libraries/zlib/crc32.c
./Source/Libraries/zlib/deflate.c
./Source/Libraries/zlib/infback.c
./Source/Libraries/zlib/inffast.c
./Source/Libraries/zlib/inflate.c
./Source/Libraries/zlib/inftrees.c
./Source/Libraries/zlib/trees.c
./Source/Libraries/zlib/uncompr.c
./Source/Libraries/zlib/zutil.c
./Source/Model/Classes/NMR_Model.cpp
./Source/Model/Classes/NMR_ModelAttachment.cpp
./Source/Model/Classes/NMR_ModelBaseMaterial.cpp
./Source/Model/Classes/NMR_ModelBaseMaterials.cpp
./Source/Model/Classes/NMR_ModelBuildItem.cpp
./Source/Model/Classes/NMR_ModelComponent.cpp
./Source/Model/Classes/NMR_ModelComponentsObject.cpp
./Source/Model/Classes/NMR_ModelDefaultProperty.cpp
./Source/Model/Classes/NMR_ModelDefaultProperty_BaseMaterial.cpp
./Source/Model/Classes/NMR_ModelDefaultProperty_Color.cpp
./Source/Model/Classes/NMR_ModelDefaultProperty_TexCoord2D.cpp
./Source/Model/Classes/NMR_ModelMeshObject.cpp
./Source/Model/Classes/NMR_ModelMetaData.cpp
./Source/Model/Classes/NMR_ModelObject.cpp
./Source/Model/Classes/NMR_ModelResource.cpp
./Source/Model/Classes/NMR_ModelTexture2D.cpp
./Source/Model/Classes/NMR_ModelThumbnail.cpp
./Source/Model/COM/NMR_COMInterface_Model.cpp
./Source/Model/COM/NMR_COMInterface_ModelAttachment.cpp
./Source/Model/COM/NMR_COMInterface_ModelBaseMaterial.cpp
./Source/Model/COM/NMR_COMInterface_ModelBuildItem.cpp
./Source/Model/COM/NMR_COMInterface_ModelBuildItemIterator.cpp
./Source/Model/COM/NMR_COMInterface_ModelComponent.cpp
./Source/Model/COM/NMR_COMInterface_ModelComponentsObject.cpp
./Source/Model/COM/NMR_COMInterface_ModelDefaultPropertyHandler.cpp
./Source/Model/COM/NMR_COMInterface_ModelFactory.cpp
./Source/Model/COM/NMR_COMInterface_ModelMeshObject.cpp
./Source/Model/COM/NMR_COMInterface_ModelPropertyHandler.cpp
./Source/Model/COM/NMR_COMInterface_ModelReader.cpp
./Source/Model/COM/NMR_COMInterface_ModelResourceFactory.cpp
./Source/Model/COM/NMR_COMInterface_ModelResourceIterator.cpp
./Source/Model/COM/NMR_COMInterface_ModelTexture2D.cpp
./Source/Model/COM/NMR_COMInterface_ModelWriter.cpp
./Source/Model/COM/NMR_DLLInterfaces.cpp
./Source/Model/Reader/NMR_ModelReader.cpp
./Source/Model/Reader/NMR_ModelReaderNode.cpp
./Source/Model/Reader/NMR_ModelReaderNode_Model.cpp
./Source/Model/Reader/NMR_ModelReaderWarnings.cpp
./Source/Model/Reader/NMR_ModelReader_3MF.cpp
./Source/Model/Reader/NMR_ModelReader_3MF_GCC.cpp
./Source/Model/Reader/NMR_ModelReader_ColorMapping.cpp
./Source/Model/Reader/NMR_ModelReader_STL.cpp
./Source/Model/Reader/NMR_ModelReader_TexCoordMapping.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Build.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_BuildItem.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Color.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Component.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Components.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Material.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Mesh.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Object.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Resources.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Texture.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_TextureVertex.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_TextureVertices.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Triangle.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Triangles.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Vertex.cpp
./Source/Model/Reader/v093/NMR_ModelReaderNode093_Vertices.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_BaseMaterial.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_BaseMaterials.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Build.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_BuildItem.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Color.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Colors.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Component.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Components.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Mesh.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_MetaData.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Object.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Resources.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Tex2Coord.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Tex2DGroup.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Texture2D.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Triangle.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Triangles.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Vertex.cpp
./Source/Model/Reader/v100/NMR_ModelReaderNode100_Vertices.cpp
./Source/Model/Writer/NMR_ModelWriter.cpp
./Source/Model/Writer/NMR_ModelWriterNode.cpp
./Source/Model/Writer/NMR_ModelWriter_3MF.cpp
./Source/Model/Writer/NMR_ModelWriter_3MF_GCC.cpp
./Source/Model/Writer/NMR_ModelWriter_3MF_Native.cpp
./Source/Model/Writer/NMR_ModelWriter_ColorMapping.cpp
./Source/Model/Writer/NMR_ModelWriter_STL.cpp
./Source/Model/Writer/NMR_ModelWriter_TexCoordMapping.cpp
./Source/Model/Writer/NMR_ModelWriter_TexCoordMappingContainer.cpp
./Source/Model/Writer/v100/NMR_ModelWriterNode100_Mesh.cpp
./Source/Model/Writer/v100/NMR_ModelWriterNode100_Model.cpp
)

if (WIN32)
	SET(SRCS
	"${SRCS_WIN_MAIN}"
	"${SRCS_COMMON}"
	"${SRCS_WINONLY}"
	)

	SET(SRCS_MANAGED_TEST
	"${SRCS_COMMON}"
	"${SRCS_WINONLY}"
	"${SRCS_WIN_MANAGEDTEST}"
	)
else()
# Linux and MAC
	SET(SRCS
	"${SRCS_UNIX_MAIN}"
	"${SRCS_COMMON}"
	"${SRCS_UNIXONLY}"
	)
endif()


if(NOT DEFINED LIB3MF_TESTS)
	set(LIB3MF_TESTS TRUE)
endif()
message("LIB3MF_TESTS ... " ${LIB3MF_TESTS})


add_library(${PROJECT_NAME} SHARED ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

if (WIN32)
	if(LIB3MF_TESTS)
		SET(UNITTEST_NAME "${PROJECT_NAME}unittests")
		add_library(${UNITTEST_NAME} SHARED ${SRCS_MANAGED_TEST})
		target_include_directories(${UNITTEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
		target_include_directories (${UNITTEST_NAME} PUBLIC "${VCINSTALLDIR}/../../../UnitTest/include")
	
		target_compile_options(${UNITTEST_NAME} PUBLIC "$<$<CONFIG:DEBUG>:/Od;/Ob0;/Gm;/sdl;/W3;/WX;/FC;-DNMR_USEASSERTIONS;-DNMR_UNITTESTS>")
		target_compile_options(${UNITTEST_NAME} PUBLIC "$<$<CONFIG:RELEASE>:/O2;/sdl;/WX;/Oi;/Gy;/FC>")

		file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/TestOutput")
	endif()
elseif(UNIX AND NOT APPLE)
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS -s)
endif()




