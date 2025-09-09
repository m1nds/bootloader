#!/bin/sh
set -e

# Clean to remove old artefacts
./clean.sh

# Building linux
echo "Building Linux image..."
nix-build linux.nix
cp result/Image .
echo "Successfully build Linux image."

# Creating initramfs filesystem
echo "Preparing initramfs..."
mkdir -pv initramfs/{bin,sbin,etc,proc,sys,dev,usr/bin,usr/sbin,tmp,lib,lib64}

# Copying init script into initramfs
cp cfg/busybox_init initramfs/init
chmod +x initramfs/init

# Building and copying busybox into initramfs
echo "Building busybox binary..."
nix-build busybox.nix
cp result/busybox initramfs/bin/
echo "Successfully build busybox binary."

# Creating sh symlink
cd initramfs
ln -sn busybox bin/sh

# Creating initramfs.cpio.gz file
find . -print0 | cpio --null -ov --format=newc | gzip -9 > "../initramfs.cpio.gz"
echo "Successfully created initramfs.cpio.gz"

