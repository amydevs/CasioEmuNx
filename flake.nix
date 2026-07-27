{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    devkitNix.url = "github:amydevs/devkitNix";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      devkitNix,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ devkitNix.overlays.default ];
        };
      in
      {
        devShells.default = pkgs.mkShell.override { stdenv = pkgs.devkitNix.stdenvA64; } {};
        packages.default = pkgs.devkitNix.stdenvA64.mkDerivation {
          name = "casio-emu-nx";
          src = ./emulator;

          nativeBuildInputs = [
            pkgs.cmake
            pkgs.ninja # Optional, but recommended for speed
          ];

          cmakeFlags = [ "-DCMAKE_BUILD_TYPE=Release" ];

          installPhase = ''
            mkdir $out
            cp example.nro $out
          '';
        };
      }
    );
}
