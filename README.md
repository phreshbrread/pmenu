# pmenu
TUI-based power menu written in C for Linux and BSD systems.

<p align="center">
  <img src="/img/example.png" alt="pmenu running in a terminal">
</p>

# Usage
Simply run from a terminal.<br>
Menu options can be selected using the arrow keys, hjkl, or mouse clicks.<br>
Run ``pmenu --help`` to view valid cmd arguments.

To have this act like a standard power prompt, I recommend setting a keybind to open and run in a new terminal.<br>
Example: ``kitty -T pmenu -o font_size=16 pmenu``

# Install
## Nix
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

## Everything else
### Building
#### Dependencies:
- gcc
- gnumake
- ncurses

To build, simply run ``make`` to output a binary at ``bin/pmenu``.

### Installing
To install, simply run ``make install`` (may require superuser permissions).<br>
Alternatively, if you don't have superuser perms, running ``make install-local`` will install pmenu to ``~/.local/bin/pmenu``
