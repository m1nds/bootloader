set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_CXX_COMPILER "aarch64-unknown-linux-gnu-g++")
set(CMAKE_ASM_COMPILER "aarch64-unknown-linux-gnu-gcc")

add_executable(bootloader "")

set_target_properties(bootloader PROPERTIES LINK_DEPENDS ../bootloader.ld)

target_sources(bootloader PUBLIC
    entry.S
    main.cpp
    serial/serial.cpp
    boot/boot.cpp
    utils/string.cpp
)

target_include_directories(bootloader PRIVATE
    utils/includes
    serial/includes
    boot/includes
)

target_link_options(bootloader PRIVATE
    -nostdlib
    -T ../bootloader.ld
)

target_compile_options(bootloader PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall; -Wextra; -Werror; -ffreestanding; -fno-stack-protector; -fno-zero-initialized-in-bss; -O1; -g>
    $<$<COMPILE_LANGUAGE:ASM>:>
)
