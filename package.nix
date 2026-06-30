{
  lib,
  stdenv,
  ncurses,
  gnumake,
  pkgVersionFromFile,
}:

stdenv.mkDerivation {
  pname = "pmenu";
  version = pkgVersionFromFile;

  src = lib.fileset.toSource {
    root = ./.;
    fileset = lib.fileset.unions [
      ./makefile
      ./src
    ];
  };

  # Needed during build phase
  nativeBuildInputs = [
    gnumake
  ];

  # Needed during runtime
  buildInputs = [
    ncurses
  ];

  buildPhase = ''
    runHook preBuild

    make pmenu

    runHook postBuild
  '';

  installPhase = ''
    runHook preInstall

    mkdir -p $out/bin
    cp bin/pmenu $out/bin/

    runHook postInstall
  '';

  # Package metadata
  meta = with lib; {
    description = "TUI power menu for Linux";
    homepage = "https://github.com/phreshbrread/pmenu";
    platforms = platforms.linux;
    mainProgram = "pmenu";
  };
}
