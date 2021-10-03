cmake_minimum_required (VERSION 3.10)
project(clox C)


add_library(libclox 
  src/chunk.c 
  src/memory.c 
  src/debug.c 
  src/value.c 
  src/scanner.c
  src/compiler.c
  src/object.c
  src/vm.c)

add_executable(clox src/main.c)
target_link_libraries(clox PRIVATE libclox)