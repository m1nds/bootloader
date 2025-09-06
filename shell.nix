with import <nixpkgs> {};

clangStdenv.mkDerivation {
  name = "clang-nix-shell";
  buildInputs = [
    pkgs.openssl
    pkgs.pkg-config
    pkgs.libselinux
    pkgs.flex
    pkgs.bc
    pkgs.bison
    pkgs.clang_18
    pkgs.llvm_18
    pkgs.lld_18
    pkgs.ninja
    pkgs.cmake
    pkgs.gnumake
    pkgs.qemu
    pkgs.binutils
    pkgs.pkgsCross.aarch64-multiplatform.buildPackages.gcc
    pkgs.pkgsCross.aarch64-multiplatform.buildPackages.glibc
    pkgs.pkgsCross.aarch64-multiplatform.buildPackages.binutils
  ];

  shellHook = ''
    export CMAKE_GENERATOR=Ninja
    export CMAKE_COLOR_DIAGNOSTICS=ON
  '';

}
