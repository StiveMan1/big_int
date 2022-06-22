cmake_minimum_required(VERSION 3.1)
project(big_int C)

set(CMAKE_C_STANDARD 99)
set(PROJECT_VERSION 1.0)
set(DEFAULT_BUILD_TYPE "Release")
set(SOURCE_FILES src/big_int.c src/big_int_ui.c src/big_int_si.c src/big_int_bi.c)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(GNUInstallDirs)
add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
