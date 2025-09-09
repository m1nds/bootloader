#!/bin/sh
set -e

cmake -S . -B build
cmake --build build

aarch64-unknown-linux-gnu-objcopy -O binary build/src/bootloader bootloader.img

dd if=/dev/zero of=pflash.bin bs=1M count=512
dd if=bootloader.img of=pflash.bin conv=notrunc bs=1M count=20
dd if=Image of=pflash.bin conv=notrunc bs=1M seek=50
