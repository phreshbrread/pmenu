{ lib,
  stdenv,
  fetchFromGitHub,
  ncurses,
  gnumake
}:

stdenv.mkDerivation (finalAttrs: {
  pname   = "pmenu-tui";
  version = "1.4.3";

  # Get source from GitHub repo
  src = fetchFromGitHub {
    owner = "phreshbrread";
    repo  = "pmenu";
    rev   = "v${finalAttrs.version}";
    hash  = "sha256-TSFGJ7xO8M+D8DOfqedOr72dVeT1cvFi/61E456xLjo=";
  };

  nativeBuildInputs = [
    gnumake
  ];

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

  meta = with lib; {
    description = "TUI power menu for Linux";
    homepage = "https://github.com/phreshbrread/pmenu";
    license = licenses.mit;
    maintainers = with maintainers; [ phreshbrread ];
    platforms = platforms.linux;
    mainProgram = "pmenu";
  };
})
