# Install script for directory: /Users/aiden/Desktop/EVT-repos/WSS/libs/EVT-core/samples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/opt/homebrew/bin/arm-none-eabi-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/blink/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/button/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/button_debounce/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/can/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/canopen/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/echo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/eeprom/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/gpio_interrupt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/iwdg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/lcd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/log/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/millis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/queue/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/read_write/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/rtc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/rtc_timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/thermistor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/aiden/Desktop/EVT-repos/WSS/cmake-build-debug/libs/EVT-core/samples/spi/cmake_install.cmake")
endif()

