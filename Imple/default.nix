{ nixpkgs ? import <nixpkgs> {} }:
let
  inherit (nixpkgs) pkgs;
  packages = with pkgs; [ gnumake gcc flex bison boost ];
in pkgs.stdenv.mkDerivation {
  name = "ispp";
  buildInputs =  packages;
}

