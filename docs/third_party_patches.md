# Third-Party Local Patches

This file tracks local edits made to vendored third-party code in this repository.
Goal: keep these patches minimal, auditable, and removable once upstreamed.

## tinybvh

### Why patched

`lib3mf` currently vendors tinybvh in:

- `Libraries/tinybvh/Include/tiny_bvh.h`

Two issues required local fixes for CI/toolchain stability:

1. WASM/Emscripten build failure:
   tinybvh selected `_aligned_malloc` in an Emscripten path where it is unavailable.
2. MinGW stability concern in heavy CSG tests:
   thread-spawning behavior in tinybvh build path is a likely crash source on some toolchains.

### Local changes

1. **WASM allocator fix** (vendor header patch):
   file: `Libraries/tinybvh/Include/tiny_bvh.h`
   change: treat `__EMSCRIPTEN__` like Linux for aligned allocation macro selection.

2. **Threaded tinybvh build control** (CMake option):
   file: `CMakeLists.txt`
   option: `LIB3MF_TINYBVH_THREADED_BUILDS`
   behavior:
   - defaults `OFF` on `MINGW` and `EMSCRIPTEN`
   - defaults `ON` elsewhere
   - when `OFF`, `NO_THREADED_BUILDS=1` is passed as a compile definition.

### Notes

- These changes are intentionally minimal.
- This is a temporary compatibility patch set; preferred long-term state is to consume upstream tinybvh fixes.

### Removal criteria

Remove local patches once all are true:

1. Upstream tinybvh includes a proper Emscripten allocator path.
2. MinGW CI is stable for boolean CSG tests without `NO_THREADED_BUILDS`.
3. `lib3mf` CI passes on Linux/Windows/MinGW/WASM after removing the local edits.
