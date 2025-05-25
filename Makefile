PROJ_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Configuration of extension
EXT_NAME=pdb
EXT_CONFIG=${PROJ_DIR}extension_config.cmake

ifneq (${VCPKG_ROOT}, "")
	VCPKG_TOOLCHAIN_PATH=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
endif

# Include the Makefile from extension-ci-tools
include extension-ci-tools/makefiles/duckdb_extension.Makefile
