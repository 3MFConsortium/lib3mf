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
