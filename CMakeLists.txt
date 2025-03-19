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

#add_executable(test_ERP main_tests.cpp tests/tests.cpp src/arrows.cpp src/braces.cpp src/blocks.cpp src/lines.cpp src/memory_vizual)


enable_testing()

add_executable(t2mp_test tests/t2mp_test.cpp)
add_test(NAME t2mp_test COMMAND t2mp_test --force-colors -d)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

#target_link_libraries(test_ERP
    #PRIVATE
    #sfml-graphics
    #sfml-window
    #sfml-system
#)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()
