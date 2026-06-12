{
  description = "pmenu dev flake";

  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
      pkgVersionFromFile = builtins.replaceStrings ["\""] [""] (nixpkgs.lib.fileContents ./src/version.txt);
      fs = nixpkgs.lib.fileset;
    in {
      devShells.${system}.default = pkgs.mkShell {
        # Tools for use in dev shell
        buildInputs = with pkgs; [
          gcc
          ncurses
          gdb
          gnumake
        ];
      };

        packages.${system} = {
          pmenu = pkgs.stdenv.mkDerivation {
            pname = "pmenu";
            version = "${pkgVersionFromFile}";
            src = ./.;
            #src = fs.toSource rec {
            #  root = ./.;
            #  fileset.maybeMissing = fs.unions [
            #      ./makefile
            #      ./src
            #  ];
            #};

            # Needed during build phase
            nativeBuildInputs = with pkgs; [
              gcc # Included in stdenv anyways
              gnumake
            ];

            # Needed during runtime
            buildInputs = with pkgs; [
              ncurses
            ];

            buildPhase = ''
              make pmenu
            '';

            installPhase = ''
              mkdir -p $out/bin
              cp bin/pmenu $out/bin
            '';

            # Package metadata
            meta = with pkgs.lib; {
              description = "TUI power menu";
              homepage = "https://github.com/phreshbrread/pmenu";
              platforms = platforms.linux;
            };
          };

          # Make default package when running 'nix run / build'
          default = self.packages.${system}.pmenu;
        };
    };
}

