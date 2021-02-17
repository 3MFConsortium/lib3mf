
if (USE_INCLUDED_LIBZIP)
  file(GLOB LIBZIP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Source/Libraries/libzip/*.c")
  if (UNIX)
	file(GLOB LIBZIP_FILES_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Source/Libraries/libzip/Unix/*.c")
  else()
	file(GLOB LIBZIP_FILES_PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Source/Libraries/libzip/Win/*.c")
  endif()
endif()

if (USE_INCLUDED_ZLIB)
  file(GLOB ZLIB_FILES ./Source/Libraries/zlib/*.c)
endif()

file (GLOB CPPBASE64_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Source/Libraries/cpp-base64/*.cpp")

# sources
set(SRCS_PLATFORM
  ${LIBZIP_FILES}
  ${LIBZIP_FILES_PLATFORM}
  Source/Common/Platform/NMR_Platform_GCC.cpp
  Source/Common/Platform/NMR_ImportStream_GCC_Native.cpp
  Source/Common/Platform/NMR_ImportStream_GCC_Win32.cpp
  Source/Common/Platform/NMR_ExportStream_GCC_Native.cpp
  Source/Common/Platform/NMR_ExportStream_GCC_Win32.cpp
  Source/Common/Platform/NMR_ExportStream_ZIP.cpp
)

set(SRCS_COMMON
${SRCS_PLATFORM}
${ZLIB_FILES}
${CPPBASE64_FILES}
Source/API/lib3mf.cpp
Source/API/lib3mf_attachment.cpp
Source/API/lib3mf_base.cpp
Source/API/lib3mf_basematerialgroup.cpp
Source/API/lib3mf_basematerialgroupiterator.cpp
Source/API/lib3mf_beamlattice.cpp
Source/API/lib3mf_beamset.cpp
Source/API/lib3mf_builditem.cpp
Source/API/lib3mf_builditemiterator.cpp
Source/API/lib3mf_colorgroup.cpp
Source/API/lib3mf_colorgroupiterator.cpp
Source/API/lib3mf_component.cpp
Source/API/lib3mf_compositematerials.cpp
Source/API/lib3mf_compositematerialsiterator.cpp
Source/API/lib3mf_componentsobject.cpp
Source/API/lib3mf_componentsobjectiterator.cpp
Source/API/lib3mf_meshobject.cpp
Source/API/lib3mf_meshobjectiterator.cpp
Source/API/lib3mf_metadata.cpp
Source/API/lib3mf_metadatagroup.cpp
Source/API/lib3mf_multipropertygroup.cpp
Source/API/lib3mf_multipropertygroupiterator.cpp
Source/API/lib3mf_contentencryptionparams.cpp
Source/API/lib3mf_accessright.cpp
Source/API/lib3mf_consumer.cpp
Source/API/lib3mf_resourcedata.cpp
Source/API/lib3mf_resourcedatagroup.cpp
Source/API/lib3mf_keystore.cpp
Source/API/lib3mf_model.cpp
Source/API/lib3mf_object.cpp
Source/API/lib3mf_objectiterator.cpp
Source/API/lib3mf_packagepart.cpp
Source/API/lib3mf_reader.cpp
Source/API/lib3mf_resource.cpp
Source/API/lib3mf_resourceiterator.cpp
Source/API/lib3mf_slice.cpp
Source/API/lib3mf_slicestack.cpp
Source/API/lib3mf_slicestackiterator.cpp
Source/API/lib3mf_texture2d.cpp
Source/API/lib3mf_texture2dgroup.cpp
Source/API/lib3mf_texture2dgroupiterator.cpp
Source/API/lib3mf_texture2diterator.cpp
Source/API/lib3mf_writer.cpp
Source/API/lib3mf_utils.cpp
Source/Common/3MF_ProgressMonitor.cpp
Source/Common/Math/NMR_Matrix.cpp
Source/Common/Math/NMR_PairMatchingTree.cpp
Source/Common/Math/NMR_Vector.cpp
Source/Common/Math/NMR_VectorTree.cpp
Source/Common/MeshExport/NMR_MeshExporter.cpp
Source/Common/MeshExport/NMR_MeshExporter_STL.cpp
Source/Common/MeshImport/NMR_MeshImporter.cpp
Source/Common/MeshImport/NMR_MeshImporter_STL.cpp
Source/Common/MeshInformation/NMR_MeshInformation.cpp
Source/Common/MeshInformation/NMR_MeshInformationContainer.cpp
Source/Common/MeshInformation/NMR_MeshInformationFactory.cpp
Source/Common/MeshInformation/NMR_MeshInformationHandler.cpp
Source/Common/MeshInformation/NMR_MeshInformation_Properties.cpp
Source/Common/Mesh/NMR_Mesh.cpp
Source/Common/Mesh/NMR_BeamLattice.cpp
Source/Common/Mesh/NMR_MeshBuilder.cpp
Source/Common/NMR_Exception.cpp
Source/Common/NMR_Exception_Windows.cpp
Source/Common/NMR_ModelWarnings.cpp
Source/Common/NMR_StringUtils.cpp
Source/Common/NMR_SecureContext.cpp
Source/Common/NMR_UUID.cpp
Source/Common/OPC/NMR_OpcPackagePart.cpp
Source/Common/OPC/NMR_OpcPackageRelationship.cpp
Source/Common/OPC/NMR_OpcPackageReader.cpp
Source/Common/OPC/NMR_OpcPackageContentTypesReader.cpp
Source/Common/OPC/NMR_OpcPackageRelationshipReader.cpp
Source/Common/OPC/NMR_OpcPackageWriter.cpp
Source/Common/Platform/NMR_XmlReader_Native.cpp
Source/Common/Platform/NMR_EncryptionHeader.cpp
Source/Common/Platform/NMR_ExportStream.cpp
Source/Common/Platform/NMR_ExportStream_Callback.cpp
Source/Common/Platform/NMR_ExportStream_Compressed.cpp
Source/Common/Platform/NMR_ExportStream_Encrypted.cpp
Source/Common/Platform/NMR_ExportStream_Memory.cpp
Source/Common/Platform/NMR_ExportStream_Dummy.cpp
Source/Common/Platform/NMR_ExportStream_ZIP.cpp
Source/Common/Platform/NMR_ImportStream_Callback.cpp
Source/Common/Platform/NMR_ImportStream_Compressed.cpp
Source/Common/Platform/NMR_ImportStream_Memory.cpp
Source/Common/Platform/NMR_ImportStream_Shared_Memory.cpp
Source/Common/Platform/NMR_ImportStream_Unique_Memory.cpp
Source/Common/Platform/NMR_ImportStream_ZIP.cpp
Source/Common/Platform/NMR_ImportStream_Encrypted.cpp
Source/Common/Platform/NMR_PortableZIPWriter.cpp
Source/Common/Platform/NMR_PortableZIPWriterEntry.cpp
Source/Common/Platform/NMR_Time.cpp
Source/Common/Platform/NMR_XmlReader.cpp
Source/Common/Platform/NMR_XmlWriter.cpp
Source/Common/Platform/NMR_XmlWriter_Native.cpp
Source/Model/Classes/NMR_PackageResourceID.cpp
Source/Model/Classes/NMR_Model.cpp
Source/Model/Classes/NMR_ModelAttachment.cpp
Source/Model/Classes/NMR_ModelBaseMaterial.cpp
Source/Model/Classes/NMR_ModelBaseMaterials.cpp
Source/Model/Classes/NMR_ModelContext.cpp
Source/Model/Classes/NMR_ModelColorGroup.cpp
Source/Model/Classes/NMR_ModelCompositeMaterials.cpp
Source/Model/Classes/NMR_ModelBuildItem.cpp
Source/Model/Classes/NMR_ModelComponent.cpp
Source/Model/Classes/NMR_ModelComponentsObject.cpp
Source/Model/Classes/NMR_ModelMeshBeamLatticeAttributes.cpp
Source/Model/Classes/NMR_ModelMeshObject.cpp
Source/Model/Classes/NMR_ModelMetaData.cpp
Source/Model/Classes/NMR_ModelMetaDataGroup.cpp
Source/Model/Classes/NMR_ModelMultiPropertyGroup.cpp
Source/Model/Classes/NMR_ModelObject.cpp
Source/Model/Classes/NMR_ModelResource.cpp
Source/Model/Classes/NMR_ModelTexture2D.cpp
Source/Model/Classes/NMR_ModelTexture2DGroup.cpp
Source/Model/Classes/NMR_ModelSlice.cpp
Source/Model/Classes/NMR_ModelSliceStack.cpp
Source/Model/Classes/NMR_KeyStore.cpp
Source/Model/Classes/NMR_KeyStoreConsumer.cpp
Source/Model/Classes/NMR_KeyStoreAccessRight.cpp
Source/Model/Classes/NMR_KeyStoreFactory.cpp
Source/Model/Classes/NMR_KeyStoreResourceData.cpp
Source/Model/Classes/NMR_KeyStoreResourceDataGroup.cpp
Source/Model/Classes/NMR_KeyStoreCEKParams.cpp
Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Ball.cpp
Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BallRef.cpp
Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Balls.cpp
Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Beam.cpp
Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Beams.cpp
Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamLattice.cpp
Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamSet.cpp
Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamSets.cpp
Source/Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Ref.cpp
Source/Model/Reader/NMR_ModelReader.cpp
Source/Model/Reader/NMR_ModelReaderNode.cpp
Source/Model/Reader/NMR_ModelReaderNode_ModelBase.cpp
Source/Model/Reader/NMR_ModelReaderNode_KeyStoreBase.cpp
Source/Model/Reader/NMR_ModelReader_3MF.cpp
Source/Model/Reader/NMR_ModelReader_3MF_Native.cpp
Source/Model/Reader/NMR_ModelReader_ColorMapping.cpp
Source/Model/Reader/NMR_ModelReader_InstructionElement.cpp
Source/Model/Reader/NMR_ModelReader_STL.cpp
Source/Model/Reader/NMR_ModelReader_TexCoordMapping.cpp
Source/Model/Reader/NMR_KeyStoreOpcPackageReader.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_BaseMaterial.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_BaseMaterials.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Build.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_BuildItem.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Color.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Colors.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Component.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Components.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Composite.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_CompositeMaterials.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Mesh.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_MetaData.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_MetaDataGroup.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Multi.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_MultiProperties.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Object.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Resources.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Tex2Coord.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Tex2DGroup.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Texture2D.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Triangle.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Triangles.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Vertex.cpp
Source/Model/Reader/v100/NMR_ModelReaderNode100_Vertices.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Build.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_BuildItem.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Color.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Component.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Components.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Material.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Mesh.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Object.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Resources.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Texture.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_TextureVertex.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_TextureVertices.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Triangle.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Triangles.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Vertex.cpp
Source/Model/Reader/v093/NMR_ModelReaderNode093_Vertices.cpp
Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Polygon.cpp
Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Segment.cpp
Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Slice.cpp
Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRef.cpp
Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRefModel.cpp
Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRefResources.cpp
Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceStack.cpp
Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Vertex.cpp
Source/Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Vertices.cpp
Source/Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStore.cpp
Source/Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreCipherValue.cpp
Source/Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreKEKParams.cpp
Source/Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreCEKParams.cpp
Source/Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreConsumer.cpp
Source/Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreAccessRight.cpp
Source/Model/Reader/NMR_ModelReaderNode_StringValue.cpp
Source/Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreResourceData.cpp
Source/Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreResourceDataGroup.cpp
Source/Model/Writer/NMR_KeyStoreOpcPackageWriter.cpp
Source/Model/Writer/NMR_ModelWriter.cpp
Source/Model/Writer/NMR_ModelWriterNode.cpp
Source/Model/Writer/NMR_ModelWriterNode_ModelBase.cpp
Source/Model/Writer/NMR_ModelWriterNode_KeyStoreBase.cpp
Source/Model/Writer/NMR_ModelWriter_3MF.cpp
Source/Model/Writer/NMR_ModelWriter_3MF_Native.cpp
Source/Model/Writer/NMR_ModelWriter_ColorMapping.cpp
Source/Model/Writer/NMR_ModelWriter_STL.cpp
Source/Model/Writer/NMR_ModelWriter_TexCoordMapping.cpp
Source/Model/Writer/NMR_ModelWriter_TexCoordMappingContainer.cpp
Source/Model/Writer/SecureContent101/NMR_ModelWriterNode_KeyStore.cpp
Source/Model/Writer/v100/NMR_ModelWriterNode100_Mesh.cpp
Source/Model/Writer/v100/NMR_ModelWriterNode100_Model.cpp
)

# FILELISTVARIABLE must be the name of a variable that holds a list of source-file paths relative to ${CMAKE_CURRENT_SOURCE_DIR}
# FOLDERNAME will be the name of this folder in the IDE
# FOLDERHOME paths in the IDE will be relative to this path
function(sourceGroupByFolder FILELISTVARIABLE FOLDERNAME FOLDERHOME)
  SET(SOURCE_GROUP_DELIMITER "/")
  SET(last_dir "")
  SET(files "")
  FOREACH(file ${${FILELISTVARIABLE}})
    file(RELATIVE_PATH relative_file "${FOLDERHOME}" "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
    GET_FILENAME_COMPONENT(dir "${relative_file}" DIRECTORY)
  IF (NOT "${dir}" STREQUAL "${last_dir}")
      IF (files)
        SOURCE_GROUP("${FOLDERNAME}${SOURCE_GROUP_DELIMITER}${last_dir}" FILES ${files})
      ENDIF (files)
      SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
  ENDFOREACH(file)
  IF (files)
    SOURCE_GROUP("${FOLDERNAME}${SOURCE_GROUP_DELIMITER}${last_dir}" FILES ${files})
  ENDIF (files)
endfunction(sourceGroupByFolder)

sourceGroupByFolder(SRCS_COMMON "Source Files" "${CMAKE_CURRENT_SOURCE_DIR}/Source")

file(GLOB_RECURSE HDRS_COMMON RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Include/*.h" "Include/*.hpp")
sourceGroupByFolder(HDRS_COMMON "Header Files" "${CMAKE_CURRENT_SOURCE_DIR}/Include")
