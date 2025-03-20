cmake_minimum_required(VERSION 3.20)
project(GRPS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

include(FetchContent)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)

FetchContent_MakeAvailable(sfml)

add_executable(${PROJECT_NAME} main.cpp src/arrows.cpp src/braces.cpp src/blocks.cpp src/lines.cpp src/memory_vizual.cpp)

enable_testing()

add_executable(grps_test tests/tmp_test.cpp)
add_test(NAME grps_test COMMAND grps_test --force-colors -d)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()
