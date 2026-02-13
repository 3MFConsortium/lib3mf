#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="${root_dir}/build/install-smoke"
install_dir="${root_dir}/build/install-root"
rm -rf "${build_dir}" "${install_dir}"

generator="Unix Makefiles"
if command -v ninja >/dev/null 2>&1; then
  generator="Ninja"
fi

cmake -S "${root_dir}" -B "${build_dir}" -G "${generator}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="${install_dir}" \
  -DUSE_INCLUDED_ZLIB=OFF \
  -DUSE_INCLUDED_LIBZIP=OFF \
  -DUSE_INCLUDED_SSL=OFF \
  -DLIB3MF_TESTS=OFF

cmake --build "${build_dir}" --target lib3mf
cmake --install "${build_dir}"

if ! find "${install_dir}" -type f \( -name "lib3mf*.so" -o -name "lib3mf*.so.*" -o -name "lib3mf*.dylib" \) | grep -q .; then
  echo "Missing installed shared library (lib3mf*.so|dylib)." >&2
  exit 1
fi

pc_file=""
for candidate in \
  "${install_dir}/lib/pkgconfig/lib3mf.pc" \
  "${install_dir}/lib64/pkgconfig/lib3mf.pc"
do
  if [[ -f "${candidate}" ]]; then
    pc_file="${candidate}"
    break
  fi
done

if [[ -z "${pc_file}" ]]; then
  echo "Missing installed lib3mf.pc." >&2
  exit 1
fi

if [[ -f "${install_dir}/include/lib3mf/Bindings/C/lib3mf.h" && -f "${install_dir}/include/lib3mf/Bindings/C/lib3mf_types.h" ]]; then
  header_root="${install_dir}/include/lib3mf/Bindings/C"
elif [[ -f "${install_dir}/include/Bindings/C/lib3mf.h" && -f "${install_dir}/include/Bindings/C/lib3mf_types.h" ]]; then
  header_root="${install_dir}/include/Bindings/C"
else
  echo "Missing installed C headers (lib3mf.h, lib3mf_types.h)." >&2
  exit 1
fi

export PKG_CONFIG_PATH="${install_dir}/lib/pkgconfig:${install_dir}/lib64/pkgconfig:${PKG_CONFIG_PATH:-}"
pkg-config --cflags --libs lib3mf >/dev/null

cat > "${build_dir}/test.c" <<'EOF'
#include <lib3mf.h>

int main(void) {
  Lib3MF_uint32 major = 0;
  Lib3MF_uint32 minor = 0;
  Lib3MF_uint32 micro = 0;
  return (int) lib3mf_getlibraryversion(&major, &minor, &micro);
}
EOF

cc "${build_dir}/test.c" -o "${build_dir}/test" $(pkg-config --cflags --libs lib3mf)

echo "Install smoke test passed. Headers at: ${header_root}; pkg-config file: ${pc_file}"
