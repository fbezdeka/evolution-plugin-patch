#
# Evolution patch module
#
# Copyright (c) Florian Bezdeka, 2025
#
# SPDX-License-Identifier: LGPL-3.0-only
#

cmake_minimum_required(VERSION 3.25.0)
cmake_policy(VERSION 3.25.0)

project(evolution-module-patch C)

set(REQUIRE_EVOLUTION_VERSION 3.40.0)

#
# Base bits
#
set(GETTEXT_PACKAGE "evolution-module-patch")
set(RELEASE_NAME "Evolution module to format messages with subject starting with '[PATCH' as text/x-patch.")
set(VERSION "0.0.1")
set(VERSION_INFO "Release")
set(LANGUAGE_SUPPORT_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/locale)

add_definitions(-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\")

# Filenames for tarball
set(ARCHIVE_BASE_NAME ${CMAKE_PROJECT_NAME}-${VERSION})
set(ARCHIVE_FULL_NAME ${ARCHIVE_BASE_NAME}.tar.xz)

find_package(PkgConfig REQUIRED)

function(pkg_check_variable _output_name _pkg _name)
	execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=${_name} ${_pkg}
			OUTPUT_VARIABLE _pkg_result
			OUTPUT_STRIP_TRAILING_WHITESPACE)

	set("${_output_name}" "${_pkg_result}" CACHE STRING "pkg-config variable ${_name} of ${_pkg}")
endfunction()

pkg_check_modules(EVOLUTION_SHELL REQUIRED evolution-shell-3.0>=${REQUIRE_EVOLUTION_VERSION})
pkg_check_variable(EVOLUTION_MODULE_DIR evolution-shell-3.0 moduledir)

option(FORCE_INSTALL_PREFIX "Whether to force install evolution-data-server and evolution files into the install prefix" OFF)
if(FORCE_INSTALL_PREFIX)
	pkg_check_variable(evo_prefix evolution-shell-3.0 prefix)
	string(REGEX REPLACE "^${evo_prefix}" "${CMAKE_INSTALL_PREFIX}" EVOLUTION_MODULE_DIR "${EVOLUTION_MODULE_DIR}")
endif(FORCE_INSTALL_PREFIX)

include(CheckCCompilerFlag)

# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# Dist
# This generates the dist tarball from a git clone
add_custom_target(
	dist
	COMMAND git archive --prefix=${ARCHIVE_BASE_NAME}/ HEAD | xz -z > ${CMAKE_BINARY_DIR}/${ARCHIVE_FULL_NAME}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_subdirectory(src)
