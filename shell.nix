with import <nixpkgs> {};

pkgs.mkShell {
  name = "bootloader-shell";
  buildInputs = [
    pkgs.ninja
    pkgs.cmake
    pkgs.qemu
    pkgs.pkgsCross.aarch64-multiplatform.buildPackages.binutils
    pkgs.pkgsCross.aarch64-multiplatform.buildPackages.gdb
    pkgs.pkgsCross.aarch64-multiplatform.buildPackages.gcc
  ];

  shellHook = ''
    export CMAKE_GENERATOR=Ninja
    export CMAKE_COLOR_DIAGNOSTICS=ON
  '';

}
