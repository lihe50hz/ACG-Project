cmake_minimum_required(VERSION 3.22)

project(Sparkium)

set(CMAKE_CXX_STANDARD 17)

set(SPARKIUM_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets/)

add_subdirectory(external/LongMarch)

find_package(tinyobjloader CONFIG REQUIRED)
set(TINYOBJLOADER_LIB_NAME tinyobjloader::tinyobjloader)
list(APPEND SPARKIUM_LIB_LIST ${TINYOBJLOADER_LIB_NAME})

find_package(tinyfiledialogs CONFIG REQUIRED)
set(TINYFILEDIALOGS_LIB_NAME tinyfiledialogs::tinyfiledialogs)
list(APPEND SPARKIUM_LIB_LIST ${TINYFILEDIALOGS_LIB_NAME})

find_package(Stb REQUIRED)
set(STB_INC_DIR ${Stb_INCLUDE_DIR})

find_package(mikktspace CONFIG REQUIRED)
set(MIKKTSPACE_LIB_NAME mikktspace::mikktspace)
list(APPEND SPARKIUM_LIB_LIST ${MIKKTSPACE_LIB_NAME})

find_package(imguizmo CONFIG REQUIRED)
set(IMGUIZMO_LIB_NAME PRIVATE imguizmo::imguizmo)
list(APPEND SPARKIUM_LIB_LIST ${IMGUIZMO_LIB_NAME})

add_subdirectory(code)
