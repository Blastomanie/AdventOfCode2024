cmake_minimum_required(VERSION 3.5.0)
project(AdventOfCode2023 VERSION 0.1.0)

add_subdirectory(lib)

file(GLOB_RECURSE SOURCES "days/*/*.cpp")

find_package(SDL2 REQUIRED)

set(IMGUI_PATH "lib/imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp ${IMGUI_PATH}/backends/imgui_impl_sdlrenderer2.cpp)
add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})

foreach(SOURCE_FILE ${SOURCES})

    get_filename_component(FILE_NAME ${SOURCE_FILE} NAME_WE)

    get_filename_component(DAY_DIR ${SOURCE_FILE} DIRECTORY)
    get_filename_component(DAY_NAME ${DAY_DIR} NAME)

    add_executable(${FILE_NAME} ${SOURCE_FILE})
    target_link_libraries(${FILE_NAME} PRIVATE utils ${SDL2_LIBRARIES} ImGui)

    file(GLOB_RECURSE TXT_FILES "days/${DAY_NAME}/*.txt")

    foreach(TXT_FILE ${TXT_FILES})
        get_filename_component(TXT_FILE_NAME ${TXT_FILE} NAME)
        add_custom_command(TARGET ${FILE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TXT_FILE} ${CMAKE_CURRENT_BINARY_DIR}/days/${DAY_NAME}/${TXT_FILE_NAME}
            DEPENDS ${TXT_FILE}
    )
    endforeach()

    set_target_properties(${FILE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/days/${DAY_NAME})
endforeach()