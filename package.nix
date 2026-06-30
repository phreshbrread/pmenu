{ lib,
  stdenv,
  fetchFromGitHub,
  ncurses,
  gnumake
}:

stdenv.mkDerivation (finalAttrs: {
  pname = "pmenu";
  version = "1.4.2";

  # Get source from GH repo
  src = fetchFromGitHub {
    owner = "phreshbrread";
    repo = "pmenu";
    rev = "v${finalAttrs.version}";
    hash = lib.fakeHash;
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
    maintainers = with maintainers; [ phreshbrread ]; # Add yourself to pkgs/maintainers/maintainer-list.nix first!
    platforms = platforms.linux;
    mainProgram = "pmenu";
  };
})
