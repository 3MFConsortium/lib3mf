#include <stdio.h>
#include "lib3mf_dynamic.h"

#ifndef _WIN32
#define printf_s printf
#endif

#ifndef LIB3MF_LIBRARY_LOCATION
#error "LIB3MF_LIBRARY_LOCATION is expected from lib3mf CDynamic package config"
#endif

static int fail(const char *message, Lib3MFResult code) {
  printf_s("%s (%d)\n", message, (int) code);
  return (int) code;
}

int main() {
  sLib3MFDynamicWrapperTable wrapperTable;
  Lib3MFResult result;
  Lib3MF_uint32 major, minor, micro;

  result = InitLib3MFWrapperTable(&wrapperTable);
  if (result != LIB3MF_SUCCESS) {
    return fail("Failed to initialize wrapper table", result);
  }

  result = LoadLib3MFWrapperTable(&wrapperTable, LIB3MF_LIBRARY_LOCATION);
  if (result != LIB3MF_SUCCESS) {
    return fail("Failed to load lib3mf library", result);
  }

  result = wrapperTable.m_GetLibraryVersion(&major, &minor, &micro);
  if (result != LIB3MF_SUCCESS) {
    ReleaseLib3MFWrapperTable(&wrapperTable);
    return fail("Failed to query version", result);
  }

  printf_s("Lib3MF.Version = %u.%u.%u\n", major, minor, micro);
  ReleaseLib3MFWrapperTable(&wrapperTable);
  return 0;
}
