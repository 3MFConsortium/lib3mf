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
