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
          cmake
        ];
      };

        packages.${system} = {
          pmenu = pkgs.stdenv.mkDerivation {
            pname = "pmenu";
            version = "${pkgVersionFromFile}";
            src = fs.toSource rec {
              root = ./.;
              fileset = fs.unions [
                  ./CMakeLists.txt
                  ./src/version.txt
                  ./src/main.c
              ];
            };

            # Needed during build phase
            nativeBuildInputs = with pkgs; [
              gcc # Included in stdenv anyways
              cmake
            ];

            # Needed during runtime
            buildInputs = with pkgs; [
              ncurses
            ];
/*
            # Build package
            buildPhase = ''
              #gcc -Wall -Wextra -Wpedantic --std=c99 -lmenu -lncurses -ltinfo ./src/main.c -o pmenu
              mkdir -p build
              cmake -B build
              cmake --build build
            '';

            # Install package
            installPhase = ''
              mkdir -p $out/bin
              cp pmenu $out/bin/
            '';
            */

            # Package metadata
            meta = with pkgs.lib; {
              description = "TUI power menu";
              homepage = "https://github.com/phreshbrread/pmenu";
              platforms = platforms.linux;
            };
          };

          # Make default package when running 'nix build'
          default = self.packages.${system}.pmenu;
        };
    };
}

