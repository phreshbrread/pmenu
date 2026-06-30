{
  description = "TUI power menu for Linux";

  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      packages.${system} = rec {
        pmenu = pkgs.callPackage ./package.nix {};
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
