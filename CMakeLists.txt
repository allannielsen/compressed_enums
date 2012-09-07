CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(CompressedEnums)

include(./.cmake/version.cmake)


include_directories(include)

add_subdirectory(test)

INSTALL(FILES include/compressed_enums.hxx include/consoleColor.h
    DESTINATION include/compressed_enums)
