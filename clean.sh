#!/bin/sh
set -e

echo "Cleaning repository..."
rm -rf ./result ./Image ./initramfs.cpio.gz initramfs
