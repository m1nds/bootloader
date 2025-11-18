cmake_minimum_required(VERSION 3.29)
project(bootloader LANGUAGES CXX ASM)

add_compile_definitions(
    IMAGE_SIZE=0x3effa00
    IMAGE_CRC32=0x359153a5
)

add_subdirectory(src)
