cmake_minimum_required (VERSION 2.8)

IF (NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 3.1)
  cmake_policy(SET CMP0054 OLD)
ENDIF ()

project (lib3MF)

# Define Version
set(LIB3MF_VERSION_MAJOR 1)				# increase on every backward-compatibility breaking change of the API
set(LIB3MF_VERSION_MINOR 0)				# increase on every backward compatible change of the API
set(LIB3MF_VERSION_MICRO 4)				# increase on on every change that does not alter the API

set(CMAKE_INSTALL_BINDIR bin CACHE PATH "directory for installing binary files")
set(CMAKE_INSTALL_LIBDIR lib CACHE PATH "directory for installing library files")
set(CMAKE_INSTALL_INCLUDEDIR include/lib3mf CACHE PATH "directory for installing header files")

set(NMR_COM_NATIVE FALSE) # by default, do not actually implement a COM interface
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  add_definitions(-DBUILD_DLL)
  add_compile_options(-Wall)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # using GCC
  add_definitions(-DBUILD_DLL)
  add_compile_options(-Wall) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
  set(CMAKE_MACOSX_RPATH ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  add_definitions(-DBUILD_DLL -DLIB3MF_EXPORTS /W3)
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} xmllite.lib")
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
  if(NMR_COM_NATIVE)
	add_definitions(-DNMR_COM_NATIVE)
  endif()
  
  string(REGEX REPLACE "/bin/[^/]*$" "" VCINSTALLDIR "${CMAKE_C_COMPILER}")
  message(STATUS "Guessed MSVC directory: ${VCINSTALLDIR}")
  link_directories ("${VCINSTALLDIR}/../../../UnitTest/lib")
endif()

file(GLOB LIBZIP_FILES ./Source/Libraries/libzip/*.c)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Include/Model/COM/NMR_COMVersion.h.in" "${CMAKE_CURRENT_BINARY_DIR}/Include/Model/COM/NMR_COMVersion.h")

# sources for native COM implementation or COM emulation
set(SRCS_COM)

if(NMR_COM_NATIVE)
	set(SRCS_COM
	./Source/Common/Platform/NMR_ExportStream_COM.cpp
	./Source/Common/Platform/NMR_ImportStream_COM.cpp
	./Source/Common/Platform/NMR_Platform_COM.cpp
	./Source/Common/Platform/NMR_XmlReader_COM.cpp
	./Source/Common/Platform/NMR_XmlWriter_COM.cpp
	./Source/Model/Reader/NMR_ModelReader_3MF_OPC.cpp
	./Source/Model/Writer/NMR_ModelWriter_3MF_OPC.cpp
	)
else()
	set(SRCS_COM
	${LIBZIP_FILES}
	./Source/Model/COM/NMR_COMInterface_GCC.cpp
	./Source/Common/Platform/NMR_Platform_GCC.cpp
	./Source/Common/OPC/NMR_OpcPackageReader.cpp
	./Source/Common/OPC/NMR_OpcPackageRelationshipReader.cpp
	./Source/Common/OPC/NMR_OpcPackageContentTypesReader.cpp
	./Source/Common/Platform/NMR_ImportStream_GCC_Native.cpp
	./Source/Common/Platform/NMR_ImportStream_GCC_Win32.cpp
	./Source/Common/Platform/NMR_ImportStream_ZIP.cpp
	./Source/Common/Platform/NMR_ExportStream_GCC_Native.cpp
	./Source/Common/Platform/NMR_ExportStream_GCC_Win32.cpp
	./Source/Common/Platform/NMR_ExportStream_ZIP.cpp
	./Source/Common/Platform/NMR_XmlReader_Native.cpp
	./Source/Model/Reader/NMR_ModelReader_3MF_Native.cpp
	./Source/Model/Reader/NMR_ModelReader_3MF_GCC.cpp
	)
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

set(SRCS_UNITTEST
./Source/UnitTests/UnitTest_AllTests.cpp
./Source/UnitTests/UnitTest_Attachments.cpp
./Source/UnitTests/UnitTest_BeamLattice.cpp
./Source/UnitTests/UnitTest_Production.cpp
./Source/UnitTests/UnitTest_ReaderStrictMode.cpp
./Source/UnitTests/UnitTest_Extensions.cpp
./Source/UnitTests/UnitTest_OPC_Issues.cpp
./Source/UnitTests/UnitTest_ReadWrite.cpp
./Source/UnitTests/UnitTest_Slice.cpp
./Source/UnitTests/UnitTest_Textures.cpp
./Source/UnitTests/UnitTest_Thumbnails.cpp
)

set(SRCS_BATCHTEST
./Source/UnitTests/UnitTest_BatchTests.cpp
)

set(SRCS_UNIX_MAIN
./Source/Main/main.cpp
)

set(SRCS_COMMON
${SRCS_COM}
./Source/Model/Reader/NMR_ModelReader_InstructionElement.cpp
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
./Source/Common/Mesh/NMR_BeamLattice.cpp
./Source/Common/Mesh/NMR_MeshBuilder.cpp
./Source/Common/NMR_Exception.cpp
./Source/Common/NMR_Exception_Windows.cpp
./Source/Common/NMR_StringUtils.cpp
./Source/Common/NMR_UUID.cpp
./Source/Common/OPC/NMR_OpcPackagePart.cpp
./Source/Common/OPC/NMR_OpcPackageRelationship.cpp
./Source/Common/OPC/NMR_OpcPackageWriter.cpp
./Source/Common/Platform/NMR_ExportStream.cpp
./Source/Common/Platform/NMR_ExportStream_Callback.cpp
./Source/Common/Platform/NMR_ExportStream_GCC.cpp
./Source/Common/Platform/NMR_ExportStream_Memory.cpp
./Source/Common/Platform/NMR_ExportStream_Dummy.cpp
./Source/Common/Platform/NMR_ExportStream_ZIP.cpp
./Source/Common/Platform/NMR_ImportStream_GCC.cpp
./Source/Common/Platform/NMR_ImportStream_Callback.cpp
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
./Source/Model/Classes/NMR_PackageResourceID.cpp
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
./Source/Model/Classes/NMR_ModelMeshBeamLatticeAttributes.cpp
./Source/Model/Classes/NMR_ModelMeshObject.cpp
./Source/Model/Classes/NMR_ModelMetaData.cpp
./Source/Model/Classes/NMR_ModelObject.cpp
./Source/Model/Classes/NMR_ModelResource.cpp
./Source/Model/Classes/NMR_ModelTexture2D.cpp
./Source/Model/Classes/NMR_ModelSliceStackResource.cpp
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
./Source/Model/COM/NMR_COMInterface_ModelMeshBeamSet.cpp
./Source/Model/COM/NMR_COMInterface_ModelPropertyHandler.cpp
./Source/Model/COM/NMR_COMInterface_ModelReader.cpp
./Source/Model/COM/NMR_COMInterface_ModelResourceFactory.cpp
./Source/Model/COM/NMR_COMInterface_ModelResourceIterator.cpp
./Source/Model/COM/NMR_COMInterface_ModelTexture2D.cpp
./Source/Model/COM/NMR_COMInterface_ModelWriter.cpp
./Source/Model/COM/NMR_COMInterface_Slice.cpp
./Source/Model/COM/NMR_DLLInterfaces.cpp
./Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Beam.cpp
./Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Beams.cpp
./Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamLattice.cpp
./Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamSet.cpp
./Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamSets.cpp
./Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Ref.cpp
./Source/Model/Reader/NMR_ModelReader.cpp
./Source/Model/Reader/NMR_ModelReaderNode.cpp
./Source/Model/Reader/NMR_ModelReaderNode_Model.cpp
./Source/Model/Reader/NMR_ModelReaderWarnings.cpp
./Source/Model/Reader/NMR_ModelReader_3MF.cpp
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
./Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Polygon.cpp
./Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Segment.cpp
./Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Slice.cpp
./Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRef.cpp
./Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRefModel.cpp
./Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRefResources.cpp
./Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceStack.cpp
./Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Vertex.cpp
./Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Vertices.cpp
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
	)

	SET(SRCS_MANAGED_TEST
	"${SRCS_COMMON}"
	"${SRCS_WIN_MANAGEDTEST}"
	)
	SET(SRCS_DLL_TEST
	 ./Source/UnitTests/UnitTest_DLLInterfaces.cpp
	)
else()
# Linux and MAC
	SET(SRCS
	"${SRCS_UNIX_MAIN}"
	"${SRCS_COMMON}"
	)
endif()


if(NOT DEFINED LIB3MF_TESTS)
	set(LIB3MF_TESTS TRUE)
endif()
message("LIB3MF_TESTS ... " ${LIB3MF_TESTS})


#########################################################
# Set version info

if(DEFINED ENV{BUILD_NUMBER})
	set(BUILD_NUMBER $ENV{BUILD_NUMBER})
else()
	set(BUILD_NUMBER 0)
endif()

if (WIN32)
	message("BUILD_NUMBER ... " ${BUILD_NUMBER})
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/Release/VersionInfo)
	
	include(generate_product_version)
	generate_product_version(
	   VERSION_FILES_OUTPUTLOCATION
	   NAME "lib3MF"
	   FILE_DESCRIPTION "Lib3MF is the reference-implementation of the 3D Manufacturing Format file standard"
	   BUNDLE "3MF Consortium"
	   VERSION_MAJOR ${LIB3MF_VERSION_MAJOR}
	   VERSION_MINOR ${LIB3MF_VERSION_MINOR}
	   VERSION_PATCH ${LIB3MF_VERSION_MICRO}
	   VERSION_REVISION ${BUILD_NUMBER}
	   PRODUCT_VERSION_MAJOR ${LIB3MF_VERSION_MAJOR}
	   PRODUCT_VERSION_MINOR ${LIB3MF_VERSION_MINOR}
	   PRODUCT_VERSION_PATCH ${LIB3MF_VERSION_MICRO}
	   COMPANY_NAME "3MF Consortium"
	)
	message("VERSION_FILES_OUTPUTLOCATION ... " ${VERSION_FILES_OUTPUTLOCATION})
else()
	set(VERSION_FILES_OUTPUTLOCATION "")
endif()


#########################################################
# shared library
add_library(${PROJECT_NAME} SHARED ${SRCS} ${VERSION_FILES_OUTPUTLOCATION})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Include)
install(TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(DIRECTORY Include/Model DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY Include/Common DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "")
if (WIN32)
	#########################################################
	# platform specific tests
	if(LIB3MF_TESTS)
		SET(UNITTEST_NAME "${PROJECT_NAME}unittests")
		add_library(${UNITTEST_NAME} SHARED ${SRCS_MANAGED_TEST})
		target_include_directories(${UNITTEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
		target_include_directories(${UNITTEST_NAME} PUBLIC "${VCINSTALLDIR}/../../../UnitTest/include")
		target_include_directories(${UNITTEST_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Include)
	
		target_compile_options(${UNITTEST_NAME} PUBLIC "$<$<CONFIG:DEBUG>:/Od;/Ob0;/Gm;/sdl;/W3;/WX;/FC;-DNMR_USEASSERTIONS;-DNMR_UNITTESTS;/wd4996>")
		target_compile_options(${UNITTEST_NAME} PUBLIC "$<$<CONFIG:RELEASE>:/O2;/sdl;/WX;/Oi;/Gy;/FC;/wd4996>")
		# wd4996 masks the deprecated-warning
		file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/TestOutput")
	endif()
elseif(UNIX)
	if (NOT APPLE)
		SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS -s)
	endif()
	# Uncomment the following to but the version info into the .so-file.
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION "${LIB3MF_VERSION_MAJOR}")
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SOVERSION "${LIB3MF_VERSION_MAJOR}.${LIB3MF_VERSION_MINOR}.${LIB3MF_VERSION_MICRO}.${BUILD_NUMBER}")
endif()


#########################################################
# platform independent tests for shared library interface
if(LIB3MF_TESTS)
	ADD_SUBDIRECTORY (googletest EXCLUDE_FROM_ALL)
	enable_testing()
	SET(CTEST_SHARED "${PROJECT_NAME}_CTest_shared")
	SET(gtest_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest")
	if(NMR_COM_NATIVE)
		add_definitions ( -DNMR_COM_NATIVE)
	endif()
	add_definitions ( -DTESTFILESPATH="${CMAKE_CURRENT_SOURCE_DIR}/TestFiles")
	include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
	add_executable(${CTEST_SHARED} ${SRCS_UNITTEST})
	target_include_directories(${CTEST_SHARED} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	add_dependencies(${CTEST_SHARED} ${PROJECT_NAME})
	target_link_libraries(${CTEST_SHARED} gtest gtest_main ${PROJECT_NAME})
	add_test(${CTEST_SHARED} ${CTEST_SHARED})
endif()

#########################################################
# testing the production + slice extension
if(FALSE)
	SET(CTEST_BATCH "${PROJECT_NAME}_BatchTests")
	add_executable(${CTEST_BATCH} ${SRCS_BATCHTEST})
	target_include_directories(${CTEST_BATCH} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
	add_dependencies(${CTEST_BATCH} ${PROJECT_NAME})
	target_link_libraries(${CTEST_BATCH} ${PROJECT_NAME})
endif()
