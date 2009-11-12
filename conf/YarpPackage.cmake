#
# Packing stuff
#
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Yet Another Robot Platform")
SET(CPACK_PACKAGE_NAME "yarp")
SET(CPACK_PACKAGE_VENDOR "YARP")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
INCLUDE(YarpVersion)
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME
  "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")


#  if you are making debian packages, this is useful, but not otherwise
#CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/conf/template/control.in
#               ${CMAKE_BINARY_DIR}/DEBIAN/control @ONLY IMMEDIATE)
#INSTALL_FILES(/doc/DEBIAN FILES ${CMAKE_BINARY_DIR}/DEBIAN/control)

IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")

  SET(CPACK_PACKAGE_EXECUTABLES "yarp" "yarp")
  INCLUDE(CPack)

ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
