# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

yarp_prepare_plugin(SDLJoypad
                    CATEGORY device
                    TYPE yarp::dev::SDLJoypad
                    INCLUDE SDLJoypad.h
                    EXTRA_CONFIG WRAPPER=JoypadControlServer
                    DEPENDS "YARP_USE_SDL")

if(ENABLE_SDLJoypad)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  yarp_add_plugin(yarp_SDLJoypad SDLJoypad.cpp
                                 SDLJoypad.h)

  target_link_libraries(yarp_SDLJoypad PRIVATE YARP::YARP_OS
                                               YARP::YARP_sig
                                               YARP::YARP_dev)
  list(APPEND YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS YARP_OS
                                                      YARP_sig
                                                      YARP_dev)

  target_include_directories(yarp_SDLJoypad SYSTEM PRIVATE ${SDL_INCLUDE_DIR})
  target_link_libraries(yarp_SDLJoypad PRIVATE ${SDL_LIBRARY})
#   list(APPEND YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS SDL) (not using targets)

  yarp_install(TARGETS yarp_SDLJoypad
               EXPORT YARP_${YARP_PLUGIN_MASTER}
               COMPONENT ${YARP_PLUGIN_MASTER}
               LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
               ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})

  yarp_install(FILES SDLJoypad.ini
               COMPONENT ${YARP_PLUGIN_MASTER}
               DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

  set(YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS ${YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS} PARENT_SCOPE)

  set_property(TARGET yarp_SDLJoypad PROPERTY FOLDER "Plugins/Device")
endif()
