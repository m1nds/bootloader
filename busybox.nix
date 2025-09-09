let
  pkgs = import <nixpkgs> {
    crossSystem = {
      config = "aarch64-unknown-linux-musl";
      useLLVM = true;
    };
  };
  busybox-static = pkgs.busybox.override { enableStatic = true; };
in
pkgs.stdenv.mkDerivation {
  name = "aarch64-unknown-linux-musl-busybox";

  buildCommand = ''
    mkdir -p $out
    cp ${busybox-static}/bin/busybox $out/busybox
  '';
}

