# pmenu
TUI-based power menu written in C for Linux and BSD systems.

<p align="center">
  <img src="/img/example.png" alt="pmenu running in a terminal">
</p>

## Usage
Simply run from a terminal.
I recommend setting a keybind to open and execute at the same time.<br>
Example: ``kitty -T pmenu -o font_size=16 pmenu``

Run ``pmenu --help`` to view valid cmd arguments

## Install
### Nix
This repo can be added as a custom package.<br>
To do so, include the following in your ``flake.nix``:
```
    inputs = {
        pmenu.url = "github:phreshbrread/pmenu";
    };
```

Make sure the ``nixosConfiguration`` for your system receives this input.<br>
Then, in your ``configuration.nix`` (or wherever else your package lists are):
```
    environment.systemPackages = with pkgs; [
        inputs.pmenu.packages.${pkgs.system}.default
    ];
```

### Everything else
There are currently no prebuilt binaries.<br>
To install, simply run ``cmake-install.sh`` in ``scripts/`` to build and install.<br>
Alternatively, manually run the following:
```
    mkdir -p build && cmake -B build && cmake --build build && cmake --install build
```

## Building
### Dependencies
- cmake
- ncurses

### With nix
Simply run ``nix build`` to build according to the included ``flake.nix``

### Manual build
Run ``cmake-build.sh`` in the ``script/`` directory, or manually run the following:
```
    mkdir -p build && cmake -B build && cmake --build build
```

This will output a binary at ``./build/pmenu``
