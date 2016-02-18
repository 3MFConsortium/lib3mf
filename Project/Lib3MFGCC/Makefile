#
# Installation target folder can be given using the INSTALL_ROOT variable.
# It defaults to /usr/local if not set on command line.
#
# Example:
#   make install INSTALL_ROOT=/opt
#
# This will install into /opt/include/lib3mf and /opt/lib
#
# Supported variables:
#   INSTALL_ROOT        Define the base folder for installation
#   INSTALL_BUILD_TYPE  Define which build to install Release or Debug
#                       (default is Release)
#   INSTALL_LIB_DIR     Define the name of the library folder, e.g for
#                       installation into 'lib64' (default is 'lib')
#

INSTALL_ROOT = /usr/local
INSTALL_FILE = install -m 644 -p
INSTALL_DIR  = cp -f -R

INSTALL_LIB_DIR = lib
INSTALL_BUILD_TYPE = Release

SYSTEM_NAME := $(shell uname -s)
SYSTEM_MAKEFILE := Makefile.$(SYSTEM_NAME)

ifneq ("$(wildcard $(SYSTEM_MAKEFILE))","")
  include $(SYSTEM_MAKEFILE)
else
  $(error Unsupported system: $(SYSTEM_NAME) - Could not find $(SYSTEM_MAKEFILE))
endif

.PHONY: install_headers
install_headers:
	@test -d $(INSTALL_ROOT)/include/lib3mf/ || mkdir -p $(INSTALL_ROOT)/include/lib3mf/
	-$(INSTALL_DIR) ../../Include/* $(INSTALL_ROOT)/include/lib3mf/

.PHONY: install_library
install_library:
	@test -d $(INSTALL_ROOT)/$(INSTALL_LIB_DIR)/ || mkdir -p $(INSTALL_ROOT)/$(INSTALL_LIB_DIR)/
	-$(INSTALL_FILE) bin/$(INSTALL_BUILD_TYPE)/lib3* $(INSTALL_ROOT)/$(INSTALL_LIB_DIR)/

.PHONY: install
install: install_headers install_library
