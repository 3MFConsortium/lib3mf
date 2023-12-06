#!/bin/bash

base_path=$(dirname $(realpath -s $0))"/"
cd "$base_path"

if [ ! -d "../Libraries/" ]; then
  mkdir "../Libraries"
fi

# Updating the cpp-base64 library
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

echo "$tag" > "$lib_path_dest${lib_name}_$tag.txt"


# Updating the zlib library
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

echo "$tag" > "$lib_path_dest${lib_name}_$tag.txt"


# Updating the libzip library
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
echo "$tag" > "$lib_path_dest${lib_name}_$tag.txt"

# updating the googletest library
lib_name="googletest"
lib_path_src="$base_path$lib_name/googletest/"

cd "$lib_name"
tag=$(git describe --contains HEAD)
cd "$base_path"

# Delete the destination library folder when updating
lib_path_dest="../Libraries/$lib_name/"
rm -rf "$lib_path_dest"
mkdir -p "$lib_path_dest"

mkdir "$lib_path_dest/Include"
mkdir "$lib_path_dest/Source"
mkdir "$lib_path_dest/Include/src"

cp -r "${lib_path_src}include/gtest/" "${lib_path_dest}Include/" 

find "${lib_path_src}src/" -name "*.h" \
  -exec cp {} "${lib_path_dest}Include/src" \;

find "${lib_path_src}src/" -name "*.cc" \
  ! -name "*all*" \
  ! -name "*main*" \
  -exec cp {} "${lib_path_dest}Source" \;

echo "$tag" > "$lib_path_dest${lib_name}_$tag.txt"