# Archie: Fast & Easy Package Management for Arch Linux

## 📋 Project Status

**This project is no longer actively maintained** due to disagreements among the original development team. 

## Recommended Alternatives

This project is no longer maintained. The tools below carry the torch.

Both are **independently maintained**, actively developed, and keep the original Archie idea intact: short commands, fast startup, less typing. They just take very different approaches.

### 🌟 **[Archium](https://github.com/keircn/archium)** by [Keiran](https://github.com/keircn)

A faithful continuation of classic Archie. Same foundation, steadily improved and expanded.

* Native C binary, tiny and extremely fast
* Works with `yay`, `paru`, or plain `pacman`
* Readline editing and proper bash/fish/zsh completions
* Built-in config, logs, and cache handling
* Plugin system for custom commands and extensions
* Lots of features, lots of knobs, very customizable
* Mature, stable, and regularly updated

If you want **“Archie, but bigger and more capable”**, this is the natural upgrade.

Think: the original, refined and reinforced.

### 🦀 **[Archie-ng](https://github.com/Gur0v/archie-ng)** by [Gurov](https://github.com/Gur0v)

A full rewrite from scratch in Rust. Not a fork with fixes. A clean break.

* Memory-safe by design: no leaks, no overflows, no shell injection
* No dependency on `yay` or `paru`: ships its own pacman+AUR backend
* Instant startup, non-blocking tab completion with a warm cache
* Fully declarative TOML config: every command, prompt, and keybinding is user-defined
* Per-command confirmation, prompts, and completion source control
* Custom commands via `shell:` prefix for anything outside the builtins
* Minimal runtime dependencies
* Very active development

Same single-key workflow, but the internals are completely different. Less legacy, fewer sharp edges.

If you want something **lean, modern, and built right from the start**, this is the forward-looking option.

Think: Archie rebuilt from the ground up, not patched.

## Features

- **System Updates**: Quickly update your entire system
- **Package Management**: 
  - Install packages
  - Remove packages
  - Purge packages
- **System Maintenance**:
  - Clean package cache
  - Remove orphaned packages
- **Package Search**: Easily find packages in repositories

## Installation

### Method 1: Manual Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/TuxForge/archie.git
   cd archie
   ```

2. Compile the program:
   ```sh
   make
   ```

3. Install the binary (requires root permissions):
   ```sh
   sudo make install
   ```

### Method 2: AUR Installation

Archie is available on the Arch User Repository (AUR). Install using your preferred AUR helper:

```sh
yay -S archie
# Or
paru -S archie
```

## Usage

### Interactive Mode

When you run `archie`, you'll see an interactive prompt:

```
Welcome to Archie, type "h" for help
$
```

#### Available Commands

| Command | Action |
|---------|--------|
| `u` | Update the system |
| `i` | Install a package |
| `r` | Remove a package |
| `p` | Purge a package |
| `c` | Clean cache |
| `o` | Clean orphaned packages |
| `s` | Search for a package |
| `h` | Display help |
| `q` | Quit the application |

### Command-Line Arguments

#### Execute Specific Commands

Run Archie with a specific command using `--exec`:

```sh
archie --exec <command>
```

Supported commands: `u`, `i`, `r`, `p`, `c`, `o`, `s`, `h`

Example:
```sh
archie --exec u  # Updates the system directly
```

#### Version Information

Display version details:
```sh
archie --version
```

## Dependencies

- `gcc`: GNU Compiler Collection
- `yay` or `paru`: AUR helpers
- `git`: Version control system
- `readline`: Command-line input library
- `ncurses`: Text-based user interface library

## Important Notes

### AUR Helper Selection

- Archie uses `yay` by default
- If only `paru` is installed, it will use `paru`
- To force `paru` when both are available, create a file:
  ```sh
  touch $HOME/.archie-use-paru
  ```

### Upgrade Troubleshooting

If you encounter issues upgrading Archie, try:
```sh
yay -Scc --noconfirm && yay -S archie --noconfirm
```

## License

Archie is licensed under the GNU General Public License. See the [LICENSE](LICENSE) file for complete details.

## Credits

- Created & maintained by Gurov
- Contributors: scklss, Keiran

## Support

For issues, feature requests, or contributions, please visit the [GitHub repository](https://github.com/TuxForge/archie).
