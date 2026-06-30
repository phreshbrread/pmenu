{
  description = "TUI power menu for Linux";

  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};

      # Get version from file in src/
      pkgVersionFromFile = builtins.replaceStrings ["\""] [""] (nixpkgs.lib.fileContents ./src/version.txt);
    in {
      packages.${system} = rec {
        pmenu = pkgs.callPackage ./package.nix {
          inherit pkgVersionFromFile;
        };
        default = pmenu;
      };

      devShells.${system}.default = pkgs.mkShell {
        # Grab dependencies from package.nix
        inputsFrom = [ self.packages.${system}.pmenu ];

        # Extra tools just for dev environment
        nativeBuildInputs = with pkgs; [
          gdb
        ];
      };
    };
}
