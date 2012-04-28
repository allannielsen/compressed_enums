IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None Debug
        Release RelWithDebInfo MinSizeRel."
        FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CXX11_FLAGS "-std=c++0x -stdlib=libc++")
IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    IF(NOT CMAKE_CXX_FLAGS)
        SET(CMAKE_CXX_FLAGS "-Werror" CACHE STRING "Flags used by the compiler during all build types." FORCE)
        SET(CMAKE_C_FLAGS   "-Werror" CACHE STRING "Flags used by the compiler during all build types." FORCE)
    ENDIF()

else()
    IF(NOT CMAKE_CXX_FLAGS)
        SET(CMAKE_CXX_FLAGS "-Wall" CACHE STRING "Flags used by the compiler during all build types." FORCE)
        SET(CMAKE_C_FLAGS   "-Wall" CACHE STRING "Flags used by the compiler during all build types." FORCE)
    ENDIF()

endif()

execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/version.rb" "-n" "${PROJECT_NAME}" "-h" "${CMAKE_CURRENT_BINARY_DIR}/version.h" "-s" "${CMAKE_CURRENT_BINARY_DIR}/version.c" "-C" "${CMAKE_CURRENT_BINARY_DIR}/version.cmake" 
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)

include(${CMAKE_CURRENT_BINARY_DIR}/version.cmake)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(reruntarget ALL)
add_custom_command(OUTPUTS "file-which-does-not-exists"
    COMMAND "/usr/bin/touch" "${CMAKE_CURRENT_BINARY_DIR}/version.cmake"
                   TARGET  reruntarget
)

add_library(version STATIC ${CMAKE_CURRENT_BINARY_DIR}/version.c)


option(GMB_TARGET "Compiled for target" OFF)
IF(GMB_TARGET)
    ADD_DEFINITIONS(-DGMB_TARGET)
ELSE(GMB_TARGET)
    ADD_DEFINITIONS(-UGMB_TARGET)
ENDIF(GMB_TARGET)

set(PROJECT_VERSION ${${PROJECT_NAME}_VERSION1})
set(PROJECT_VERSION_FULL ${${PROJECT_NAME}_VERSION})
set(PROJECT_VNAME ${PROJECT_NAME}${${PROJECT_NAME}_VERSION1})

message(STATUS "Project name =   ${PROJECT_NAME}")
message(STATUS "  Version =      ${${PROJECT_NAME}_VERSION}")
message(STATUS "  Hash =         ${${PROJECT_NAME}_HASH}")
message(STATUS "  Type =         ${CMAKE_BUILD_TYPE}")
message(STATUS "  cxx_flags =    ${CMAKE_CXX_FLAGS}")
message(STATUS "  Target opt =   ${GMB_TARGET}")

option(GMB_API_VERSION_FILE "File to include to get GMB API VERSION" "")

IF( GMB_API_VERSION_FILE )
    message(STATUS "  Include file = ${GMB_API_VERSION_FILE}")
    include(${GMB_API_VERSION_FILE})
    message(STATUS "  API Version =  ${GMB_API_VERSION}")
ELSE( GMB_API_VERSION_FILE )
    SET(GMB_API_VERSION "NO_API_VERSION")
    SET(GMB_OS_VERSION "NO_OS_VERSION")
    message(STATUS "  API Version = ${GMB_API_VERSION}")
ENDIF( GMB_API_VERSION_FILE )

set(CPACK_GMB_NAME "pirateisland")
set(CPACK_GMB_VERSION ${PROJECT_VERSION_FULL})
set(CPACK_GMB_PACKAGE_TYPE "GAMELOGIC")
set(CPACK_GMB_API_VERSION ${GMB_API_VERSION})
set(CPACK_GMB_OS_VERSION ${GMB_OS_VERSION})
set(CPACK_GMB_URL_PREFIX "http://download.gamblify.com/gamblify-slotmachine-packages/games/pirateisland/engine/")

add_definitions(-DGMB_API_VERSION=\"${GMB_API_VERSION}\")

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR     "lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR     "bin" CACHE PATH "Installation directory for executables")
set(INSTALL_QML_DIR     "qml" CACHE PATH "Installation directory for QML files")
set(INSTALL_INCLUDE_DIR "include/${PROJECT_VNAME}" CACHE PATH "Installation directory for header files")
set(INSTALL_DATA_DIR    "share/${PROJECT_VNAME}" CACHE PATH "Installation directory for data files")
set(INSTALL_CMAKE_DIR   "${INSTALL_DATA_DIR}/CMake" CACHE PATH "Installation directory for cmake files")

# cpack settings

SET(CPACK_PACKAGE_VENDOR "Gamblify")
SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_GENERATOR "GMB")
SET(CPACK_STRIP_FILES TRUE)
SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}@${GMB_OS_VERSION}@${GMB_API_VERSION}@${${PROJECT_NAME}_VERSION4}")
INCLUDE(CPack)

message(STATUS "  Package name = ${CPACK_PACKAGE_FILE_NAME}")
