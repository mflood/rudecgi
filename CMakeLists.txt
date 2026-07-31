cmake_minimum_required(VERSION 3.16)

project(rudecgi
    VERSION 5.4.0
    DESCRIPTION "C++ library for parsing CGI form data, cookies, and file uploads"
    HOMEPAGE_URL "https://github.com/mflood/rudecgi"
    LANGUAGES CXX)

# PROJECT_IS_TOP_LEVEL is CMake 3.21+; this project still supports 3.16, and
# an undefined variable would read as false and silently drop the tests.
if(NOT DEFINED PROJECT_IS_TOP_LEVEL)
    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        set(PROJECT_IS_TOP_LEVEL TRUE)
    else()
        set(PROJECT_IS_TOP_LEVEL FALSE)
    endif()
endif()

include(GNUInstallDirs)
# Only offer tests and examples when this is the top-level project.
# Without the guard, a FetchContent consumer inherits our demo and
# test_* targets into their own build.
if(PROJECT_IS_TOP_LEVEL)
    include(CTest)
endif()

option(RUDECGI_BUILD_EXAMPLES "Build example programs" ON)

add_library(rudecgi
    src/AbstractFactory.cpp
    src/AbstractImplementation.cpp
    src/AbstractParser.cpp
    src/AbstractParserFactory.cpp
    src/CaseMatch.cpp
    src/CaselessMatch.cpp
    src/ClientData.cpp
    src/CommandLineExtractor.cpp
    src/CookieData.cpp
    src/CookieParser.cpp
    src/DataObject.cpp
    src/DataRepos.cpp
    src/Factory.cpp
    src/FileData.cpp
    src/FormdataExtractor.cpp
    src/GETExtractor.cpp
    src/Implementation.cpp
    src/Interactive.cpp
    src/NewMultipartParser.cpp
    src/POSTExtractor.cpp
    src/ParserFactory.cpp
    src/PathData.cpp
    src/PathMap.cpp
    src/PathMapParser.cpp
    src/PathParser.cpp
    src/SimpleData.cpp
    src/SimpleXMLParser.cpp
    src/URLEncodeParser.cpp
    src/UnknownExtractor.cpp
    src/UnknownParser.cpp
    src/WordMatch.cpp
    src/cgi.cpp)

add_library(rudecgi::rudecgi ALIAS rudecgi)

target_compile_features(rudecgi PUBLIC cxx_std_17)

# Single source of truth for the version reported by CGI::version().
# Generated rather than hardcoded so it cannot drift from the project version
# the way the old "3.0" string did.
configure_file(src/rudecgi_version.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/internal/rudecgi_version.h" @ONLY)
target_include_directories(rudecgi PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/internal")

# Public header is consumed as <rude/cgi.h>; stage it under that path
# so in-tree consumers see the same layout as installed ones.
configure_file(src/cgi.h "${CMAKE_CURRENT_BINARY_DIR}/include/rude/cgi.h" COPYONLY)
target_include_directories(rudecgi PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# Shared-library version follows the old libtool -version-info 5:0:0 lineage.
set_target_properties(rudecgi PROPERTIES
    VERSION 5.0.1
    SOVERSION 5
    POSITION_INDEPENDENT_CODE ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(MSVC)
    target_compile_options(rudecgi PRIVATE /W4)
else()
    target_compile_options(rudecgi PRIVATE -Wall -Wextra)
endif()

# ---- Install ----

install(TARGETS rudecgi
    EXPORT rudecgi-targets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(FILES src/cgi.h
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/rude")

install(FILES man3/rudecgi.3
    DESTINATION "${CMAKE_INSTALL_MANDIR}/man3")

install(EXPORT rudecgi-targets
    FILE rudecgi-targets.cmake
    NAMESPACE rudecgi::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/rudecgi")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/rudecgi-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/rudecgi-config.cmake"
    "include(\"\${CMAKE_CURRENT_LIST_DIR}/rudecgi-targets.cmake\")\n")
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/rudecgi-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/rudecgi-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/rudecgi")

# The .pc file must not bake in the configure-time CMAKE_INSTALL_PREFIX: that
# breaks "cmake --install --prefix <dir>", which is what the README teaches.
# Express prefix relative to ${pcfiledir} instead, so the file stays correct
# wherever it lands.  Walk back up one "../" per component of
# <libdir>/pkgconfig -- computed by hand rather than with file(RELATIVE_PATH),
# which requires absolute arguments and rejects "/" as a root on Windows.
# Falls back to the absolute prefix if the caller set an absolute
# CMAKE_INSTALL_LIBDIR, where a relative path is not meaningful.
if(IS_ABSOLUTE "${CMAKE_INSTALL_LIBDIR}")
    set(RUDECGI_PC_PREFIX "${CMAKE_INSTALL_PREFIX}")
else()
    file(TO_CMAKE_PATH "${CMAKE_INSTALL_LIBDIR}/pkgconfig" RUDECGI_PC_SUBDIR)
    string(REGEX REPLACE "/+" "/" RUDECGI_PC_SUBDIR "${RUDECGI_PC_SUBDIR}")
    string(REGEX REPLACE "^/+|/+$" "" RUDECGI_PC_SUBDIR "${RUDECGI_PC_SUBDIR}")
    string(REPLACE "/" ";" RUDECGI_PC_PARTS "${RUDECGI_PC_SUBDIR}")
    set(RUDECGI_PC_REL_PREFIX "")
    foreach(part IN LISTS RUDECGI_PC_PARTS)
        if(NOT part STREQUAL "" AND NOT part STREQUAL ".")
            string(APPEND RUDECGI_PC_REL_PREFIX "../")
        endif()
    endforeach()
    if(RUDECGI_PC_REL_PREFIX STREQUAL "")
        set(RUDECGI_PC_REL_PREFIX "./")
    endif()
    set(RUDECGI_PC_PREFIX "\${pcfiledir}/${RUDECGI_PC_REL_PREFIX}")
endif()

configure_file(rudecgi.pc.in "${CMAKE_CURRENT_BINARY_DIR}/rudecgi.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/rudecgi.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

# ---- Tests & examples ----

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(PROJECT_IS_TOP_LEVEL AND RUDECGI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
