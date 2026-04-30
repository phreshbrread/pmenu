{
  description = "pmenu dev flake";

  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      devShells.${system}.default = pkgs.mkShell {
        # Tools for use in dev shell
        buildInputs = with pkgs; [
          gcc
          ncurses
          gdb
        ];
      };

        packages.${system} = {
          pmenu = pkgs.stdenv.mkDerivation {
            pname = "pmenu";
            version = "1.0.1";
            src = ./.;

            # Needed during build phase
            nativeBuildInputs = with pkgs; [
              gcc # Included in stdenv anyways
            ];

            # Needed during runtime
            buildInputs = with pkgs; [
              ncurses
            ];

            # Build package
            buildPhase = ''
              gcc -Wall -Wextra -Wpedantic -g --std=c99 -lmenu -lncurses -ltinfo ./src/main.c -o pmenu
            '';

            # Install package
            installPhase = ''
              mkdir -p $out/bin
              cp pmenu $out/bin/
            '';

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

