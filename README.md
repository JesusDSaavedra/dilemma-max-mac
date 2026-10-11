# Dilemma Max V3 — macOS trackpad gestures + Catppuccin

Personal firmware configuration for a [Bastard Keyboards Dilemma Max V3](https://bastardkb.com/product/dilemma-max-v3-hotswap/) with the Procyon trackpad, built on [Bastardkb/bastardkb-qmk](https://github.com/Bastardkb/bastardkb-qmk).

The full user guide (Spanish) is in [`docs/GUIA-CONFIGURACION.md`](docs/GUIA-CONFIGURACION.md).

**Dilemma Dojo**, an interactive trainer (Spanish) for touch typing and for this layout, is published at
<https://jesusdsaavedra.github.io/dilemma-max-mac/>. Its source is [`index.html`](index.html). It includes a layer map, typing lessons, layer and shortcut drills, and a Herdr simulator.

## What is here

| Folder | Contents |
|---|---|
| `firmware/` | Ready-to-flash `.uf2` builds, named `version_date_commit`. Flash the same file to **both** halves. |
| `patches/` | Every change as `git format-patch` commits, to apply on top of `bastardkb-qmk` at `bc50f889`. |
| `keymap/mac/` | A copy of the `mac` keymap, for quick reference. The patches also contain it. |
| `docs/` | The user guide: gestures, keys, tuning, backup and troubleshooting. |

## Features

- **Split serial on `GP16`**, a bodge-wire repair after `GP1` was damaged.
- **Layout based on Gentleman's** [`vendor` keymap](https://github.com/Alan-TheGentleman/qmk_userspace/tree/main/keyboards/bastardkb/dilemma/4x6_4_procyon/keymaps/vendor). It assumes a US macOS input source.
  - **Base layer:** QWERTY with GACS home-row mods. The tap-hold timings are tuned for a learner: Shift 200 ms; Cmd, Alt and Ctrl 260 ms; other tap-hold keys 200 ms. Flow Tap (150 ms) is on.
  - **Layers:** MAGIC, PROG, NUMBERS, NAV (hold Backspace) and POINTER (hold Z or /).
  - **Thumbs:** `Click Space PROG NUMBS | MAGIC Enter NAV HERDR`.
- **Herdr layer:** hold the right outer thumb and every key sends the Herdr prefix `Ctrl+Q` followed by that key.
- **Layer colors:** while a layer is held, only its active keys light up in the layer's color, and the held keys light up white. The colors appear once the layer has been on for 250 ms.
- **Mac lock key:** the top-left key sends `Ctrl+Cmd+Q` (Lock Screen).
- **Idle RGB off:** the LEDs turn off after 15 minutes without input and come back on the next input; both halves stay in sync.
- **Protected firmware keys:** `QK_BOOT` and `EE_CLR` only fire after being held for 2 s, and the key turns red while held.
- **macOS trackpad gestures** in mouse-fallback mode:
  - pinch-to-zoom;
  - two-finger flick and left-edge swipe for back/forward, sent as mouse buttons 4/5 so they work regardless of keyboard layout;
  - 3-finger swipes for desktops, Mission Control and App Exposé;
  - adjustable cursor speed.
- **Five palette RGB themes**, all with a dimming key-press ripple:
  - Catppuccin Mocha (default)
  - Gruvbox Dark
  - Alegra (Teal, with Indigo edges)
  - Desert (sand, orange, terracotta and canyon red)
  - Horizon

  Each one is power-normalized to avoid supply brownouts with both halves connected, and brightness is capped at 128.
- **Theme layer:** hold NUMBS + MAGIC (the two innermost thumbs), then use the right half:

  | Keys | Action |
  |---|---|
  | `Y` `U` `I` `O` `P` | Select a theme |
  | `K` | RGB on/off |
  | `H` / `J` | Brightness down / up |
  | `N` / `M` | Previous / next effect |
- **maxtouch recovery:** the trackpad is reconfigured automatically if its chip resets.
- **VIA** with 8 layers and custom keys:

  | Key | Code |
  |---|---|
  | Cursor speed down | `0x7E40` |
  | Cursor speed up | `0x7E41` |
  | Pinch modifier | `0x7E42` |
  | Catppuccin theme | `0x7E43` |
  | Gruvbox theme | `0x7E44` |
  | Alegra theme | `0x7E45` |
  | Desert theme | `0x7E46` |
  | Horizon theme | `0x7E47` |

## Rebuild from source

```bash
git clone --recurse-submodules https://github.com/Bastardkb/bastardkb-qmk.git
cd bastardkb-qmk
git checkout -b feat/mac-trackpad-gestures bc50f889
git am /path/to/dilemma-max-mac/patches/*.patch
qmk compile -kb bastardkb/dilemma/4x6_4_procyon -km mac
```

On macOS, install the QMK CLI with `uv tool install qmk`, and the toolchain with `brew install osx-cross/arm/arm-none-eabi-gcc@8 osx-cross/arm/arm-none-eabi-binutils make`. Then add `/opt/homebrew/opt/arm-none-eabi-gcc@8/bin` to `PATH`.

## Back up your VIA layout

Changes made in VIA live in the keyboard's EEPROM, not in this repo. Export them from VIA → **Save + Load** → **Save**, and keep the `.json` next to the firmware.
