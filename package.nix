{ lib,
  stdenv,
  fetchFromGitHub,
  ncurses,
  gnumake
}:

stdenv.mkDerivation (finalAttrs: {
  pname   = "pmenu-tui";
  version = "1.4.1";

  # Get source from GitHub repo
  src = fetchFromGitHub {
    owner = "phreshbrread";
    repo  = "pmenu";
    rev   = "v${finalAttrs.version}";
    hash  = "sha256-57w2rfeRlHbH1xRvKkdpYQBM6mfbkTN1HTooTLWJs8c=";
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
