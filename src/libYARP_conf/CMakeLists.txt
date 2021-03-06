# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

project(YARP_conf)

#########################################################################
# Generate compiler.h header

# Read the file containing all the documentation for compiler.h
# Change offset here in case include/yarp/conf/compiler.dox.in comment changes
file(READ
     "${CMAKE_CURRENT_SOURCE_DIR}/include/yarp/conf/compiler.dox.in"
     _compiler_dox
     OFFSET 97)


include(WriteCompilerDetectionHeader)

get_property(_cxx_known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)

write_compiler_detection_header(
  FILE "${PROJECT_BINARY_DIR}/include/yarp/conf/compiler.h"
  PREFIX YARP
  COMPILERS
    GNU
    Clang
    AppleClang
    MSVC
  FEATURES ${_cxx_known_features}
  VERSION 3.1.0
  PROLOG ${_compiler_dox})


#########################################################################
# Generate the other headers

foreach(conf version system options api numeric)
  set(outconf "${PROJECT_BINARY_DIR}/include/yarp/conf/${conf}.h")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/yarp/conf/${conf}.h.in"
                 "${outconf}")
endforeach(conf)


#########################################################################
# Create the YARP_conf interface library

add_library(YARP_conf INTERFACE)
add_library(YARP::YARP_conf ALIAS YARP_conf)

target_include_directories(YARP_conf INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
                                               $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


#########################################################################
# Install all the headers

install(TARGETS YARP_conf
        EXPORT YARP_conf
        COMPONENT YARP_conf)

install(DIRECTORY "${PROJECT_BINARY_DIR}/include/yarp/conf"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/yarp
        COMPONENT YARP_conf
        FILES_MATCHING PATTERN "*.h")


#########################################################################
# Install cmake files

include(YarpInstallBasicPackageFiles)
yarp_install_basic_package_files(YARP_conf
                                 DEPENDENCIES ${YARP_conf_PUBLIC_DEPS}
                                 PRIVATE_DEPENDENCIES ${YARP_conf_PRIVATE_DEPS})
