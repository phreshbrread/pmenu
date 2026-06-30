{ lib,
  stdenv,
  fetchFromGitHub,
  ncurses,
  gnumake
}:

stdenv.mkDerivation (finalAttrs: {
  pname = "pmenu";
  version = "1.4.2";

  # Get source from GitHub repo
  src = fetchFromGitHub {
    owner = "phreshbrread";
    repo = "pmenu";
    rev = "bb5c4e94ccc866472c02c64855eac8c5f37d24a7";
    hash = "sha256-57w2rfeRlHbH1xRvKkdpYQBM6mfbkTN1HTooTLWJs8c=";
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
