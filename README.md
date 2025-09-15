# Bootloader

Simple `aarch64` bootloader for the `cortex-a72` CPU and `QEMU virt` machine that implements the `Linux Boot Protocol`. The project is part of the 2025 LSE recruitement. I implemented some part of the project as a hobby project to deepen my knowledge. Here is what I achieved so far :
- Booting from RAM using Linux Boot Protocol
- Simple CLI using PL011 UART Serial I/O

I will try to implement these features in the near future :
- CRC32 Checksum
- memdump and memtest commands
- tftpboot using PXE

## Environment and dependencies
I provided a `shell.nix` file containing all the dependencies to easily compile the bootloader. It contains `CMake`, `QEMU`,`Ninja` and a `cross-compiled` toolchain for the `aarch64-multiplatform` target containing `binutils`, `gdb` and `gcc`.

## Building Linux and Busybox

I implemented `Nix` scripts to build `Linux` and `Busybox`. This will be helpful to create a Linux image and a initramfs containing a functionning Init script that uses `Busybox`. You can build each of the projects using the following commands.
```sh
nix-build busybox.nix
nix-build linux.nix
```

I also provided a `build-tools.sh` script to easily create and setup a working environment. If you are not using `NixOS`, you will have to manually cross-compile the projects to the correct target. I personnally used `aarch64-unknown-linux-musl`.

## Building the bootloader

This can be done using `CMake`. However, I provided a `build-bt.sh` script that automatically compiles the bootloader and create
the `pflash.bin` used in `QEMU` for testing.

## Running the bootloader

This can easily be achieved using `QEMU`. Here is the command I used.
```sh
qemu-system-aarch64 -nographic -machine virt -cpu cortex-a72 -kernel pflash.bin -initrd initramfs.cpio.gz -serial mon:stdio -m 2G -smp 4
```
