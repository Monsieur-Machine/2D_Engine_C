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
    libpng
    sdl3
    ncurses
  ];
  shellHook = ''
    export LD_LIBRARY_PATH=${pkgs.libpng}/lib:${pkgs.sdl3}/lib:${pkgs.ncurses}/lib:$LD_LIBRARY_PATH
    export PKG_CONFIG_PATH=${pkgs.libpng}/lib/pkgconfig:${pkgs.sdl3}/lib/pkgconfig:${pkgs.ncurses}/lib/pkgconfig:$PKG_CONFIG_PATH
    unset TEMP TMP TEMPDIR TMPDIR
  '';
}
