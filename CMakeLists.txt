cmake_minimum_required(VERSION 3.16)

project(rudecgi
    VERSION 5.1.0
    DESCRIPTION "C++ library for parsing CGI form data, cookies, and file uploads"
    HOMEPAGE_URL "https://github.com/mflood/rudecgi"
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CTest)

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

configure_file(rudecgi.pc.in "${CMAKE_CURRENT_BINARY_DIR}/rudecgi.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/rudecgi.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

# ---- Tests & examples ----

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(RUDECGI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
