project(RISC-E LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.0)

add_subdirectory(src)

add_executable(MIPSEmulator src/Driver.cpp)

target_link_libraries(MIPSEmulator mips_cpu)

set(CMAKE_INSTALL_PREFIX ./RISC-E)

install(TARGETS MIPSEmulator RUNTIME DESTINATION .)
install(DIRECTORY examples DESTINATION .)
