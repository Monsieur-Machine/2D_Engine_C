with import <nixpkgs> {};
pkgs.mkShell {
  buildInputs = [
    # Tools
    gnumake
    gcc
    gdb
    pkg-config
    valgrind

    # Libs
    libpng
    json_c
    sdl3
    lua
    ncurses
  ];

  shellHook = ''
    export LD_LIBRARY_PATH=${pkgs.libpng}/lib:${pkgs.sdl3}/lib:${pkgs.ncurses}/lib:$LD_LIBRARY_PATH
    export PKG_CONFIG_PATH=${pkgs.libpng}/lib/pkgconfig:${pkgs.sdl3}/lib/pkgconfig:${pkgs.ncurses}/lib/pkgconfig:$PKG_CONFIG_PATH
    unset TEMP TMP TEMPDIR TMPDIR
  '';
}
