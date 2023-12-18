#!/bin/bash

base_path=$(dirname $(realpath -s $0))"/"
cd "$base_path"

if [ ! -d "../Libraries/" ]; then
  mkdir "../Libraries"
fi

update_all_libs=false

if [ -z "$1" ]; then
 update_all_libs=true
fi

# Updating the fast_float library
if [[ "$1" == "fast_float" || "$update_all_libs" == true ]]; then
  lib_name="fast_float"
  lib_path_src="$base_path$lib_name/"

  cd "$lib_name"
  tag=$(git describe --contains HEAD)
  cd "$base_path"

  # Delete the destination library folder when updating
  lib_path_dest="../Libraries/$lib_name/"
  rm -rf "$lib_path_dest"
  mkdir -p "$lib_path_dest"

  mkdir "$lib_path_dest/Include"
  cp ${lib_path_src}include/$lib_name/*.h "${lib_path_dest}Include"

  echo "\"$tag\"" > "$lib_path_dest${lib_name}_$tag.txt"
fi


# Updating the zlib library
if [[ "$1" == "zlib" || "$update_all_libs" == true ]]; then
  lib_name="zlib"
  lib_path_src="$base_path$lib_name/"

  cd "$lib_name"
  tag=$(git describe --contains HEAD)
  cd "$base_path"

  # Delete the destination library folder when updating
  lib_path_dest="../Libraries/$lib_name/"
  rm -rf "$lib_path_dest"
  mkdir -p "$lib_path_dest"

  mkdir "$lib_path_dest/Include"
  mkdir "$lib_path_dest/Source"
  cp ${lib_path_src}*.h "${lib_path_dest}Include"
  cp ${lib_path_src}*.c "${lib_path_dest}Source"

  echo "\"$tag\"" > "$lib_path_dest${lib_name}_$tag.txt"
fi


# Updating the cpp-base64 library
if [[ "$1" == "cpp-base64" || "$update_all_libs" == true  ]]; then
  lib_name="cpp-base64"
  lib_path_src="$base_path$lib_name/"

  cd "$lib_name"
  tag=$(git describe --contains HEAD)
  cd "$base_path"

  # Delete the destination library folder when updating
  lib_path_dest="../Libraries/$lib_name/"
  rm -rf "$lib_path_dest"
  mkdir -p "$lib_path_dest"

  mkdir "$lib_path_dest/Include"
  mkdir "$lib_path_dest/Source"
  cp ${lib_path_src}base*.h "${lib_path_dest}Include"
  cp ${lib_path_src}base*.cpp "${lib_path_dest}Source"

  echo "\"$tag\"" > "$lib_path_dest${lib_name}_$tag.txt"
fi

# Updating the libzip library
if [[ "$1" == "libzip" || "$update_all_libs" == true ]]; then
  lib_name="libzip"
  lib_path_src="$base_path$lib_name/"

  cd "$lib_name"
  tag=$(git describe --contains HEAD)
  cd "$base_path"

  # Delete the destination library folder when updating
  lib_path_dest="../Libraries/$lib_name/"
  rm -rf "$lib_path_dest"
  mkdir -p "$lib_path_dest"

  mkdir "$lib_path_dest/Include"
  mkdir "$lib_path_dest/Source"
  find "${lib_path_src}lib" -name "*.h" \
    ! -name "*crypto*" \
    -exec cp {} "${lib_path_dest}Include" \;

  find "${lib_path_src}lib" -name "*.c" \
    ! -name "*crypto*" \
    ! -name "*aes*" \
    ! -name "*bzip2*" \
    ! -name "*xz*" \
    ! -name "*zstd*" \
    ! -name "*win32*" \
    ! -name "*unix*" \
    ! -name "*stdio_named*" \
    ! -name "*uwp*" \
    -exec cp {} "${lib_path_dest}Source" \;

  # copy windows specific files to win and unix specific files to unix folders
  mkdir ${lib_path_dest}Source/win
  mkdir ${lib_path_dest}Source/unix

  find "${lib_path_src}lib" -name "*win32*.c" \
    -exec cp {} "${lib_path_dest}Source/win" \;

  find "${lib_path_src}lib" \( -name "*unix*.c" -o -name "*stdio_named*.c" \) \
    -exec cp {} "${lib_path_dest}Source/unix" \;
 
  cd "${lib_path_dest}Source"
  cmake -DPROJECT_SOURCE_DIR=${lib_path_src} -P ${lib_path_src}cmake/GenerateZipErrorStrings.cmake
  cd "$base_path"
  echo "\"$tag\"" > "$lib_path_dest${lib_name}_$tag.txt"
fi