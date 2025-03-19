cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

enable_testing()

add_executable(t2mp_test tests/t2mp_test.cpp)
add_test(NAME t2mp_test COMMAND t2mp_test --force-colors -d)


