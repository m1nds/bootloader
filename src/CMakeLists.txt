set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_LINKER "ld.lld")
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld -nostdlib -T ${CMAKE_SOURCE_DIR}/bootloader.ld -no-pie")

add_executable(bootloader "")

set_target_properties(bootloader PROPERTIES LINK_DEPENDS ../bootloader.ld)

target_sources(bootloader PUBLIC
    entry.S
    main.cpp
)

#target_include_directories(bootloader PRIVATE)

target_link_options(bootloader PRIVATE
    -nostdlib
    -T ${CMAKE_SOURCE_DIR}/bootloader.ld
    -no-pie
)

target_compile_options(bootloader PRIVATE
    $<$<COMPILE_LANGUAGE:CXX,ASM>:--target=aarch64-linux-gnu;-mcpu=cortex-a72;-nostdlibinc;-fno-stack-protector;-fno-threadsafe-statics;-fno-zero-initialized-in-bss;-ggdb3;-O0>

    $<$<COMPILE_LANGUAGE:CXX>:-ffreestanding;-fno-exceptions;-fno-rtti;-fno-builtin;-fno-pie;-fno-pic>
)
