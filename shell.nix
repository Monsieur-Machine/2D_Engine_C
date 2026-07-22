with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "env";
  nativeBuildInputs = [
    gnumake
    gcc
    gdb
    pkg-config
    valgrind
  ];
  buildInputs = [
    SDL2
    libpng
  ];
  shellHook = ''
    export LD_LIBRARY_PATH=${pkgs.SDL2}/lib:${pkgs.sdl3}/lib:${pkgs.libpng}/lib:$LD_LIBRARY_PATH
    export PKG_CONFIG_PATH=${pkgs.SDL2}/lib/pkgconfig:${pkgs.libpng}/lib/pkgconfig:$PKG_CONFIG_PATH
  '';
}
