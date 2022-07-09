cmake_minimum_required(VERSION 3.14)

project(chesscpp)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_compile_options(
-g
-O3
)
enable_testing()

add_executable(
    chesscpp 
    test_transforms.cpp
    test_main.cpp
    test_transposition.cpp
    test_pstables.cpp
    test_expressions.cpp
    test_utils.cpp
    test_collections.cpp
    test_extractions.cpp
)
target_link_libraries(
  chesscpp
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(chesscpp)

