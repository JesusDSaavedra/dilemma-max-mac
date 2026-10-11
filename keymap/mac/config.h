// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * macOS has no USB trackpad (PTP) support, so the trackpad runs in mouse
 * fallback mode and gestures are detected on the keyboard. Distances below are
 * in trackpad units: the Procyon reports 100 units per millimetre.
 */

/* 3-finger swipes, mapped to the native macOS trackpad shortcuts. */
#define DIGITIZER_SWIPE_LEFT_KC C(KC_RIGHT) // Fingers left: next desktop on the right.
#define DIGITIZER_SWIPE_RIGHT_KC C(KC_LEFT) // Fingers right: previous desktop on the left.
#define DIGITIZER_SWIPE_UP_KC C(KC_UP)      // Mission Control.
#define DIGITIZER_SWIPE_DOWN_KC C(KC_DOWN)  // App Exposé.

/* Gesture tuning. */
#define DIGITIZER_SCROLL_DIVISOR 150               // Higher scrolls slower: one vertical wheel step per 1.5 mm.
#define DIGITIZER_SCROLL_DIVISOR_H 100             // Horizontal scroll is faster: one step per 1 mm.
#define DIGITIZER_MOUSE_TAP_DETECTION_TIMEOUT 200  // ms a touch may last and still count as a tap.
#define DIGITIZER_MOUSE_TAP_DISTANCE 50            // Max movement (0.5 mm) for a touch to count as a tap.
#define DIGITIZER_MOUSE_SWIPE_DISTANCE 500         // Min travel (5 mm) for a 3-finger swipe.
#define DIGITIZER_MOUSE_SWIPE_THRESHOLD 300        // Max travel (3 mm) on the other axis during a swipe.
#define DIGITIZER_MOUSE_SWIPE_TIMEOUT 1000         // ms to finish a swipe before it becomes a scroll.

/*
 * Cursor speed. CURSOR_SPEED_UP/DOWN step through the speeds listed in
 * keymap.c; the choice is stored in EEPROM. Only pointer motion is scaled, so
 * scroll and swipes feel the same at every speed. DIGITIZER_MOUSE_DEFAULT_CPI
 * is the 1:1 reference: 400 CPI moves the cursor 1:1 with the trackpad.
 */
#define DIGITIZER_MOUSE_CPI_POINTER_ONLY
#define DIGITIZER_MOUSE_DEFAULT_CPI 400
#define MAC_CURSOR_CPI_DEFAULT 100 // 25% of the 1:1 speed.
#define MAC_CURSOR_CPI_MIN 50      // 12.5% of the 1:1 speed.
#define MAC_CURSOR_CPI_MAX 800     // 200% of the 1:1 speed.

/*
 * Pinch to zoom. Two fingers moving apart or together send scroll while a
 * modifier is held (Cmd by default, for Figma). PINCH_MOD_CYCLE switches the
 * modifier between Cmd, Ctrl and none (plain scroll, for AutoCAD).
 *
 * Nothing is sent until one finger moved the decision distance. Then fingers
 * moving in opposite directions pinch and fingers moving the same way (or one
 * finger resting) scroll, until the fingers lift.
 */
#define DIGITIZER_MOUSE_PINCH_ENABLE
#define DIGITIZER_TWO_FINGER_DECISION_DISTANCE 120     // Travel (1.2 mm) before pinch or scroll is chosen.
#define DIGITIZER_PINCH_OPPOSITE_COS_PERCENT 50        // Pinch if the fingers move more than 120 degrees apart.
#define DIGITIZER_PINCH_MIN_DISTANCE_CHANGE_PERCENT 50 // ...and their distance changed by half their travel.
#define DIGITIZER_TWO_FINGER_STATIONARY_PERCENT 25     // A finger moving under 25% of the other one rests: scroll.
#define DIGITIZER_PINCH_DIVISOR 250                    // Higher zooms slower: one zoom step per 2.5 mm of spread.

/*
 * Back and forward are mouse buttons 4 and 5, not Cmd+[ / Cmd+]: those depend
 * on the keyboard layout (a Latin American layout turns them into Cmd+´ and
 * Cmd++), while Chromium browsers (Brave, Chrome) and most Mac apps treat
 * buttons 4 and 5 as back and forward on any layout. Mouse buttons are sent in
 * the trackpad's own mouse report: pressed in one report, released in the next.
 */

/*
 * Edge swipe: put two fingers along the left edge and swipe right to go back
 * (mouse button 4). It fires once, then the gesture is ignored until the
 * fingers lift. Other motion from the edge (up, down, outwards, pinch) scrolls
 * or pinches as usual. Nothing is sent while the swipe is undecided. The right
 * edge has no zone (FORWARD_KC is KC_NO), so flicks, scroll and pinch work
 * there as anywhere else.
 */
#define DIGITIZER_MOUSE_EDGE_SWIPE_ENABLE
#define DIGITIZER_EDGE_SWIPE_ZONE (DIGITIZER_RESOLUTION_X * 25 / 100) // Midpoint of the fingers within 25% (10.5 mm) of an edge.
#define DIGITIZER_EDGE_SWIPE_DISTANCE 900                              // Inward travel (9 mm) of the midpoint to fire.
#define DIGITIZER_EDGE_SWIPE_THRESHOLD 400                             // Vertical travel (4 mm) that cancels the swipe.
#define DIGITIZER_EDGE_SWIPE_BACK_KC QK_MOUSE_BUTTON_4                 // Mouse button 4: back.
#define DIGITIZER_EDGE_SWIPE_FORWARD_KC KC_NO                          // No right edge zone.

/*
 * Two finger flick, like swiping between pages on a Mac trackpad: anywhere
 * outside the left edge zone, flick two fingers right to go back (mouse button 4)
 * or left to go forward (mouse button 5). It fires once, then the gesture is ignored until the
 * fingers lift. Horizontal scroll is held back until the flick timeout passes,
 * then sent without loss; vertical scroll and pinch are not delayed. If flicks
 * fire while scrolling, raise DISTANCE or lower TIMEOUT.
 */
#define DIGITIZER_MOUSE_FLICK_ENABLE
#define DIGITIZER_FLICK_DISTANCE 600        // Horizontal travel (6 mm) of the midpoint to fire.
#define DIGITIZER_FLICK_TIMEOUT 100         // ms from the start of the motion to cover that distance.
#define DIGITIZER_FLICK_THRESHOLD 250       // Vertical travel (2.5 mm) that makes it a scroll.
#define DIGITIZER_FLICK_REST_DISTANCE 50    // Fingers moved under 0.5 mm are resting: the timeout waits.
#define DIGITIZER_FLICK_RIGHT_KC QK_MOUSE_BUTTON_4 // Fingers right: mouse button 4, back.
#define DIGITIZER_FLICK_LEFT_KC QK_MOUSE_BUTTON_5  // Fingers left: mouse button 5, forward.

/*
 * RGB: the Catppuccin Mocha effect from rgb_matrix_user.inc.
 *
 * Power: with both halves connected, bright pastels sag the supply until the
 * LEDs drift to red and the trackpad chip resets (a brownout). The pastels
 * load each LED at about 79% on average, against about 50% for a saturated
 * effect, so the effect scales its colors by CATPPUCCIN_POWER_SCALE to match
 * a saturated effect at the same brightness, and the default brightness is low.
 *
 * RGB_MATRIX_KEYPRESSES enables the dimming ripple on key presses
 * (SPLIT_TRANSPORT_MIRROR from keyboard.json sends presses to both halves).
 * It also adds the built-in reactive effects, which shifts the custom effect's
 * index, so USER_CONFIG_MAGIC must change whenever this define is toggled.
 */
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_catppuccin_mocha
#undef RGB_MATRIX_DEFAULT_VAL
#define RGB_MATRIX_DEFAULT_VAL 64 // Of 255.
// Brightness cap (the keyboard allows 176). Brightness keys and VIA cannot go
// past it, which keeps both halves well clear of the brownout.
#undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 128
#define CATPPUCCIN_POWER_SCALE 160 // Of 255: about 50/79, the load of a saturated effect.

/*
 * VIA: 8 layers instead of the default 4, all of them defined in keymap.c.
 * Changing this moves the VIA data in EEPROM, so USER_CONFIG_MAGIC must change
 * with it.
 */
#define DYNAMIC_KEYMAP_LAYER_COUNT 8

/*
 * Home-row mods, from Gentleman's `vendor` keymap: 120 ms by default, with
 * per-key terms in get_tapping_term() (GUI 180, Shift 120, Alt and Ctrl 200).
 */
// Longer than Gentleman's 120 so slower presses while learning still count as
// taps. Home-row mods get their own terms in get_tapping_term (keymap.c).
#define TAPPING_TERM 200
// Flow Tap: a tap-hold key pressed within 150 ms of the previous key while
// typing always taps, so fast typing never triggers a modifier or a layer.
#define FLOW_TAP_TERM 150
// Layer colors show only after the layer has been on this long, so a quick
// tap of a layer key does not flash the lights.
#define LAYER_INDICATOR_DELAY 250
// QK_BOOT and EE_CLR fire only after being held this long (2 s).
#define DANGER_KEY_HOLD_MS 2000
#define TAPPING_TERM_PER_KEY

/* Sync the layer and LED state to both halves. */
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE

/*
 * Herdr layer: every key sends this prefix first, then itself. It matches
 * `[keys] prefix = "ctrl+q"` in ~/.config/herdr/config.toml.
 */
#ifndef HERDR_PREFIX
#    define HERDR_PREFIX C(KC_Q)
#endif
