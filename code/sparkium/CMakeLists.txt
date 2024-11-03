file(GLOB SOURCES "*.cpp" "*.h")

add_executable(sparkium ${SOURCES})

target_include_directories(sparkium PUBLIC ${SPARKIUM_INCLUDE_DIR})

file(GLOB subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

foreach (subdir ${subdirs})
    if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
        set(SPARKIUM_SUBLIB_NAME sparkium_${subdir})
        add_subdirectory(${subdir})
    endif ()
endforeach ()

message(STATUS "SPARKIUM_LIBS: ${SPARKIUM_LIBS}")

target_link_libraries(sparkium ${SPARKIUM_LIBS})

target_link_libraries(sparkium LongMarch)
