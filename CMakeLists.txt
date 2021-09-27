cmake_minimum_required (VERSION 3.10)
project(clox)

add_library(libclox src/chunk.c src/memory.c src/debug.c src/value.c src/vm.c)

add_executable(clox src/main.c)
target_link_libraries(clox PRIVATE libclox)

enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
add_executable(testrunner src/test/test_chunk.cpp src/test/test_value_array.cpp)
target_link_libraries(testrunner libclox gtest_main)
gtest_discover_tests(testrunner)
