file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_library(${SPARKIUM_SUBLIB_NAME} ${SOURCES})

list(APPEND SPARKIUM_LIBS ${SPARKIUM_SUBLIB_NAME})
set(SPARKIUM_LIBS ${SPARKIUM_SUBLIB_NAME} PARENT_SCOPE)

target_include_directories(${SPARKIUM_SUBLIB_NAME} PUBLIC ${SPARKIUM_INCLUDE_DIR})

target_link_libraries(${SPARKIUM_SUBLIB_NAME} PUBLIC LongMarch sparkium_utils sparkium_app sparkium_assets sparkium_scene)
