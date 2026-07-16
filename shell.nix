with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "env";
  nativeBuildInputs = [
    gnumake
    gcc
    pkg-config
  ];
  buildInputs = [
    SDL2
    SDL2_image
  ];
  shellHook = ''
    export LD_LIBRARY_PATH=${pkgs.SDL2}/lib:${pkgs.SDL2_image}/lib:$LD_LIBRARY_PATH
    export PKG_CONFIG_PATH=${pkgs.SDL2}/lib/pkgconfig:${pkgs.SDL2_image}/lib/pkgconfig:$PKG_CONFIG_PATH
  '';
}
