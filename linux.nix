let
  pkgs = import <nixpkgs> {
    crossSystem = {
      config = "aarch64-unknown-linux-musl";
      useLLVM = false;
    };
  };
  linux = pkgs.linuxPackages_latest.kernel;
in
pkgs.stdenv.mkDerivation {
  name = "aarch64-unknown-linux-musl-kernel";
  buildCommand = ''
    mkdir -p $out
    cp ${linux}/Image $out
  '';
}

