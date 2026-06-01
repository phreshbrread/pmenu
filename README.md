# pmenu
TUI-based power menu written in C for Linux and BSD systems.
Handles shutting down, rebooting and suspending.

## Usage
Simply run from a terminal.
I recommend setting a keybind to open and execute at the same time.
Example: "kitty -T pmenu -o font_size=16 pmenu"

Run "pmenu --help" to view valid cmd arguments

## Install
Currently there are no pre-built binaries. Manual installation is required on normal distros.
On Nix, this repo can be added as a custom package though.
To do so, include the following in your flake.nix:
```
    inputs = {
        pmenu.url = "github:phreshbrread/pmenu";
    };
```

Make sure the nixosConfiguration for your system receives this input.
Then, in your configuration.nix (or wherever else your package lists are):
```
    environment.systemPackages = with pkgs; [
        inputs.pmenu.packages.${pkgs.system}.default
    ];
```

## Building
### Dependencies
- ncurses

### With nix
Simply run 'nix build' or 'nix run' to build according to the included flake.nix

### Manual build
Run the following to build using cmake:
```
    mkdir -p build && cmake -B build && cmake --build build
```

This will output a binary at ./build/pmenu
