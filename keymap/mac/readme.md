# Dilemma Max (Procyon) – macOS keymap

The `default` keymap with VIA enabled and trackpad gestures tuned for macOS.

macOS does not support USB trackpads, so the trackpad runs in mouse fallback mode and the
keyboard detects gestures itself. Gestures that send keys (3 finger swipes, pinch) only work
when the trackpad half is the one plugged into the computer. Back and forward
are mouse buttons 4 and 5, sent in the trackpad's own mouse report.

## Gestures

| Gesture                          | Result                                                     |
| -------------------------------- | ---------------------------------------------------------- |
| 1 finger move                    | Move the cursor                                            |
| 1 / 2 / 3 finger tap             | Left / right / middle click                                |
| Tap, then touch and move         | Drag                                                       |
| 2 finger move                    | Scroll                                                     |
| 2 finger pinch / spread          | Zoom out / in: scroll while the pinch modifier is held     |
| 2 finger flick right, anywhere   | Mouse button 4: back                                       |
| 2 finger flick left, anywhere    | Mouse button 5: forward                                    |
| 2 finger swipe from left edge    | Mouse button 4: back                                       |
| 3 finger swipe left / right      | `Ctrl+→` / `Ctrl+←`: next / previous desktop               |
| 3 finger swipe up / down         | `Ctrl+↑` / `Ctrl+↓`: Mission Control / App Exposé          |

Two fingers decide between scroll and pinch after one of them moves about 1.2 mm: fingers
moving in opposite directions (apart or together) pinch, fingers moving the same way scroll.
Resting one finger and moving the other scrolls, so pinch with both fingers. The choice holds
until you lift your fingers, and nothing is lost while it is being made. Scroll sends one wheel
step per 1.5 mm of vertical travel and per 1 mm of horizontal travel, and zoom one step per
2.5 mm of change in the distance between the fingers.

Back and forward use mouse buttons 4 and 5 instead of `Cmd+[` / `Cmd+]`, because those
shortcuts depend on the keyboard layout (a Latin American layout turns them into `Cmd+´` and
`Cmd++`). Chromium browsers (Brave, Chrome) treat mouse buttons 4 and 5 as back and forward on
any layout. Each one is a single click: pressed in one mouse report and released in the next.

For an edge swipe, put two fingers along the left edge (one above the other works best, their
midpoint must start within about 10.5 mm, a quarter of the width, of the edge) and swipe about
9 mm inwards to go back. It fires once, then the gesture is ignored until you lift your fingers.
Nothing scrolls while the swipe is undecided; moving up, down or outwards, or pinching, from the
edge scrolls or pinches as usual. The right edge has no edge swipe
(`DIGITIZER_EDGE_SWIPE_FORWARD_KC` is `KC_NO`), so flicks, scroll and pinch work there like
anywhere else.

A flick is a quick two finger swipe sideways anywhere outside the left edge zone: about 6 mm in
100 ms, mostly horizontal (less than 2.5 mm up or down, and at most half as much up or down as
sideways). Fingers right go back, fingers left go forward, like swiping between pages on a Mac
trackpad. It fires once, then the gesture is ignored until you lift your fingers. To tell a flick
from a scroll, horizontal scroll waits up to 100 ms after the fingers start moving and then
catches up, so nothing is lost; vertical scroll and pinch are not delayed. If flicks fire while
scrolling, raise `DIGITIZER_FLICK_DISTANCE` or lower `DIGITIZER_FLICK_TIMEOUT`. If scroll or zoom
is too slow or too fast, lower or raise `DIGITIZER_SCROLL_DIVISOR` (vertical),
`DIGITIZER_SCROLL_DIVISOR_H` (horizontal) or `DIGITIZER_PINCH_DIVISOR`. If edge swipes fire while
scrolling sideways from near the left edge, shrink `DIGITIZER_EDGE_SWIPE_ZONE`.

The tap, scroll, swipe and pinch thresholds are in `config.h`:

| Define                                        | Default | Meaning                                                           |
| --------------------------------------------- | ------- | ----------------------------------------------------------------- |
| `DIGITIZER_TWO_FINGER_DECISION_DISTANCE`      | `120`   | Travel (1.2 mm) before scroll or pinch is chosen                  |
| `DIGITIZER_PINCH_OPPOSITE_COS_PERCENT`        | `50`    | Pinch only if the fingers move more than 120 degrees apart        |
| `DIGITIZER_PINCH_MIN_DISTANCE_CHANGE_PERCENT` | `50`    | ...and their distance changes by at least half of their travel    |
| `DIGITIZER_TWO_FINGER_STATIONARY_PERCENT`     | `25`    | A finger moving less than 25% of the other one is resting: scroll |
| `DIGITIZER_SCROLL_DIVISOR`                    | `150`   | Higher scrolls slower: one vertical wheel step per 1.5 mm         |
| `DIGITIZER_SCROLL_DIVISOR_H`                  | `100`   | Higher scrolls slower: one horizontal wheel step per 1 mm         |
| `DIGITIZER_PINCH_DIVISOR`                     | `250`   | Higher zooms slower: one zoom step per 2.5 mm of spread           |
| `DIGITIZER_EDGE_SWIPE_ZONE`                   | `1050`  | Edge zone (10.5 mm, 25% of the width) for the fingers' midpoint   |
| `DIGITIZER_EDGE_SWIPE_DISTANCE`               | `900`   | Inward travel (9 mm) of the midpoint for an edge swipe            |
| `DIGITIZER_EDGE_SWIPE_THRESHOLD`              | `400`   | Vertical travel (4 mm) that cancels an edge swipe                 |
| `DIGITIZER_EDGE_SWIPE_BACK_KC`                | `QK_MOUSE_BUTTON_4` | Mouse button 4: back, from the left edge              |
| `DIGITIZER_EDGE_SWIPE_FORWARD_KC`             | `KC_NO` | No right edge zone: gestures there flick, scroll or pinch         |
| `DIGITIZER_FLICK_DISTANCE`                    | `600`   | Horizontal travel (6 mm) of the midpoint for a flick              |
| `DIGITIZER_FLICK_TIMEOUT`                     | `100`   | ms from the start of the motion to cover that distance            |
| `DIGITIZER_FLICK_THRESHOLD`                   | `250`   | Vertical travel (2.5 mm) that makes a flick a scroll              |
| `DIGITIZER_FLICK_REST_DISTANCE`               | `50`    | Fingers moved under 0.5 mm are resting: the timeout waits         |
| `DIGITIZER_FLICK_RIGHT_KC`                    | `QK_MOUSE_BUTTON_4` | Mouse button 4: back, fingers moving right            |
| `DIGITIZER_FLICK_LEFT_KC`                     | `QK_MOUSE_BUTTON_5` | Mouse button 5: forward, fingers moving left          |

## Keys

The speed and pinch keys are on the `RAISE` layer, top letter row of the right half (the `Y`,
`U` and `I` positions); both settings are saved and survive a power cycle. The theme key is on
the `LOWER` layer at the `T` position.

| Key                 | Hex      | Action                                                                   |
| ------------------- | -------- | ------------------------------------------------------------------------ |
| `CURSOR_SPEED_DOWN` | `0x7E40` | Slower cursor, one step per press (down to 12.5%)                        |
| `CURSOR_SPEED_UP`   | `0x7E41` | Faster cursor, one step per press (up to 200%)                           |
| `PINCH_MOD_CYCLE`   | `0x7E42` | Pinch modifier: `Cmd` (Figma, default) → `Ctrl` → none (AutoCAD) → `Cmd` |
| `RGB_THEME_RESTORE` | `0x7E43` | Turns the RGB on and restores the Catppuccin effect and default brightness, without touching the layout or other settings |

VIA has 8 layers (`DYNAMIC_KEYMAP_LAYER_COUNT`); the ones past `RAISE` start transparent. Use
`MO(3)`…`MO(7)` (or `TG`, `TO`, …) in VIA to reach them, and VIA's **Save + Load** tab to back up
your layout as a `.json` file.

The cursor starts at 25% of the trackpad's 1:1 speed. The steps are 12.5%, 19%, 25%, 37.5%,
50%, 75%, 100%, 150% and 200%, so each press is easy to feel. Cursor speed only changes the
pointer; scroll and swipes feel the same at every speed.

To place a key anywhere with [VIA](https://usevia.app), pick the "Any" key and type the hex
value. VIA keeps its own copy of the layout in EEPROM and only reloads it when the build date
changes. To make sure the keys above show up, this keymap stores a settings version: when a
new firmware changes it, the first boot resets the cursor speed, the pinch modifier and the
VIA layout to the defaults (your VIA key changes are lost once). `EE_CLR` does the same.

## Lighting

The default effect is a [Catppuccin](https://catppuccin.com) Mocha theme (`rgb_matrix_user.inc`):

- Every key shows a color from the Mocha accent palette (Rosewater, Flamingo, Pink, Mauve, Red,
  Maroon, Peach, Yellow, Green, Teal, Sky, Sapphire, Blue, Lavender) as a smooth gradient from
  the outer edge to the center. Both halves mirror each other.
- Each key press sends a ripple that dims the keys it passes (up to about 82% darker at the crest)
  as it travels from the pressed key across both halves in about 1.3 s, fading as it goes
  (`CATPPUCCIN_RIPPLE_*` in `rgb_matrix_user.inc`). It needs
  `RGB_MATRIX_KEYPRESSES`, which also shifts the custom effect's index, so the settings version
  changes whenever that define is toggled. An earlier report blamed the ripple for a dead split
  trackpad; the real cause was the power brownout described below.
- Power: with both halves connected, bright pastels can sag the supply until the LEDs drift to
  red and the trackpad chip resets. Pastels light all three channels of each LED (about 79% on
  average) while saturated effects light one or two (about 50%), so the effect scales every
  color by `CATPPUCCIN_POWER_SCALE` (`160` of 255, about 50/79). It then draws about as much as a
  saturated effect at the same brightness. All channels are scaled equally, so the hues stay the
  same; set it to `255` for the exact palette values.
- The default brightness is 64 (of the keyboard's maximum 176). `RM_NEXT` / `RM_PREV` /
  `RM_TOGG` on `LOWER` and VIA still change the effect and brightness. Keep the brightness low
  if the colors drift to red or the trackpad stops; the firmware now reconfigures the trackpad
  when its chip resets, but a sagging supply is still best avoided.

After flashing a firmware with a new settings version (see above), the first boot selects this
effect and the default brightness again.

## Build

    qmk compile -kb bastardkb/dilemma/4x6_4_procyon -km mac
