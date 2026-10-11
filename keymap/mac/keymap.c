/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "digitizer_mouse_fallback.h"
#ifdef DYNAMIC_KEYMAP_ENABLE
#    include "dynamic_keymap.h"
#endif

enum dilemma_keymap_layers {
    LAYER_BASE = 0,
    LAYER_MAGIC,   // Hold MAGIC: RGB, media, QK_BOOT / EE_CLR.
    LAYER_PROG,    // Hold PROG: programming symbols.
    LAYER_NUMBERS, // Hold NUMBS: number pad and F keys.
    LAYER_NAV,     // Hold Backspace: arrows and paging.
    LAYER_POINTER, // Hold Z or /: mouse buttons, cursor speed, pinch modifier.
    LAYER_HERDR,   // Hold HERDR: every key sends HERDR_PREFIX first (see process_record_user).
    LAYER_THEME,   // NUMBS + MAGIC: lighting and theme controls.
};

// Layout from Alan-TheGentleman/qmk_userspace (the `vendor` keymap), on a US macOS input source.
#define MAGIC MO(LAYER_MAGIC)
#define PROG MO(LAYER_PROG)
#define NUMBS MO(LAYER_NUMBERS)
#define NAV LT(LAYER_NAV, KC_BSPC)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)
#define HERDR MO(LAYER_HERDR)
// Ctrl+Cmd+Q: macOS "Lock Screen". Letters only, so it works with any input source.
#define LOCK_MAC C(G(KC_Q))

// Custom keycodes start at SAFE_RANGE (QK_USER_0). Type the hex values into
// VIA's "Any" key to place them (see readme.md).
enum mac_keycodes {
    CURSOR_SPEED_DOWN = SAFE_RANGE, // 0x7E40
    CURSOR_SPEED_UP,                // 0x7E41
    PINCH_MOD_CYCLE,                // 0x7E42
    RGB_THEME_RESTORE,              // 0x7E43
    RGB_THEME_GRUVBOX,              // 0x7E44
    RGB_THEME_ALEGRA,               // 0x7E45
    RGB_THEME_DESERT,               // 0x7E46
    RGB_THEME_HORIZON,              // 0x7E47
};

#define CSPD_DN CURSOR_SPEED_DOWN
#define CSPD_UP CURSOR_SPEED_UP
#define PINCH_M PINCH_MOD_CYCLE
#define THM_CAT RGB_THEME_RESTORE
#define THM_GRV RGB_THEME_GRUVBOX
#define THM_ALG RGB_THEME_ALEGRA
#define THM_DES RGB_THEME_DESERT
#define THM_HOR RGB_THEME_HORIZON

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
     LOCK_MAC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_ESC, LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F), KC_G,       KC_H, RSFT_T(KC_J), RCTL_T(KC_K), LALT_T(KC_L), RGUI_T(KC_SCLN), KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_GRV,    PT_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, PT_SLSH, KC_LALT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                      MS_BTN1,  KC_SPC,    PROG,   NUMBS,       MAGIC,  KC_ENT,     NAV,    HERDR
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  // NUMBS (left) and MAGIC (right) inner thumbs are _______ so holding both reaches LAYER_THEME.
  [LAYER_MAGIC] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      QK_BOOT, RM_SPDU, RM_SATU, RM_HUEU, RM_VALU, RM_TOGG,    RM_TOGG, RM_VALU, RM_HUEU, RM_SATU, RM_SPDU, QK_BOOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       EE_CLR, RM_SPDD, RM_SATD, RM_HUED, RM_VALD, RM_NEXT,    RM_NEXT, RM_VALD, RM_HUED, RM_SATD, RM_SPDD,  EE_CLR,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_VOLU, KC_MUTE, KC_VOLD, KC_MPRV, KC_MNXT, XXXXXXX,    XXXXXXX, KC_MNXT, KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                       XXXXXXX, KC_MPLY, KC_MSTP, _______,    _______, KC_MSTP, KC_MPLY, XXXXXXX
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  [LAYER_PROG] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX, XXXXXXX,  KC_GRV, XXXXXXX,  KC_DLR, XXXXXXX,    KC_CIRC, KC_LPRN, KC_RPRN, KC_MINS, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX, KC_LSFT,   KC_AT, KC_HASH, KC_PERC, XXXXXXX,    KC_AMPR, KC_LCBR, KC_RCBR,  KC_EQL, KC_QUOT, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX,  KC_GRV, XXXXXXX, XXXXXXX, KC_QUES, XXXXXXX,    KC_ASTR, KC_LBRC, KC_RBRC, KC_EXLM, KC_BSLS, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                       _______, _______, _______, _______,    XXXXXXX,  KC_ENT,   KC_LT,   KC_GT
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  [LAYER_NUMBERS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      _______, _______, _______,   KC_F7,   KC_F8,   KC_F9,    _______,    KC_7,    KC_8,    KC_9, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      _______, KC_LSFT, KC_LALT,   KC_F4,   KC_F5,   KC_F6,    _______,    KC_4,    KC_5,    KC_6,    KC_0, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      _______, KC_LGUI, KC_LCTL,   KC_F1,   KC_F2,   KC_F3,    _______,    KC_1,    KC_2,    KC_3, _______, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                       _______, _______, _______, _______,    _______, _______, _______, _______
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  [LAYER_NAV] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_CAPS, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_INS, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX,  KC_ENT, _______, XXXXXXX
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  // Gentleman's DPI / sniping / drag-scroll keys do not exist here: cursor speed
  // replaces the DPI keys, the pinch modifier replaces one sniping-DPI key.
  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      XXXXXXX, XXXXXXX, CSPD_DN, CSPD_UP, XXXXXXX, PINCH_M,    XXXXXXX, XXXXXXX, CSPD_UP, CSPD_DN, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                       MS_BTN2, MS_BTN1, MS_BTN3, XXXXXXX,    XXXXXXX, MS_BTN3, MS_BTN1, MS_BTN2
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  // Hold HERDR (right outer thumb): each key sends HERDR_PREFIX, then itself.
  // Hold Shift / Alt on the left thumbs for the prefix+shift / prefix+alt bindings.
  [LAYER_HERDR] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      XXXXXXX,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_LBRC,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      XXXXXXX,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_QUES,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                       XXXXXXX, KC_LSFT, KC_LALT, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, _______
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  // Hold NUMBS + MAGIC. Theme and lighting controls on the right half.
  [LAYER_THEME] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, _______, _______, _______, _______, _______,    XXXXXXX, XXXXXXX, XXXXXXX, RM_HUED, RM_HUEU, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, _______, _______, _______, _______, _______,    THM_CAT, THM_GRV, THM_ALG, THM_DES, THM_HOR, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, _______, _______, _______, _______, _______,    RM_VALD, RM_VALU, RM_TOGG, RM_SATD, RM_SATU, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, _______, _______, _______, _______, _______,    RM_PREV, RM_NEXT, RM_SPDD, RM_SPDU, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         _______, _______, _______, _______,    _______, _______, _______, _______
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),
};
// clang-format on

// NUMBS + MAGIC together turn on the theme layer.
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, LAYER_NUMBERS, LAYER_MAGIC, LAYER_THEME);
}

// Firmware keys (QK_BOOT, EE_CLR) only fire after being held for
// DANGER_KEY_HOLD_MS, so an accidental tap does nothing. Releasing the key
// earlier cancels it. Handled on the half connected over USB.
static uint16_t danger_keycode = KC_NO;
static uint16_t danger_since;
static keypos_t danger_pos;

void housekeeping_task_user(void) {
    if (danger_keycode == KC_NO || timer_elapsed(danger_since) < DANGER_KEY_HOLD_MS) {
        return;
    }
    const uint16_t keycode = danger_keycode;
    danger_keycode         = KC_NO;
    if (keycode == QK_BOOT) {
        reset_keyboard();
    } else {
        // Same as QMK's own EE_CLR handling.
        eeconfig_disable();
        soft_reset_keyboard();
    }
}

#ifdef RGB_MATRIX_ENABLE
// Layer indicators: while a layer is held, only the keys that do something on it
// light up in the layer's color, the keys held to reach it light up white, and
// everything else turns off. Colors are tuned for the WS2812 LEDs, whose green
// is much brighter than their red, and scaled by the current brightness.
static const rgb_t layer_colors[] = {
    [LAYER_MAGIC]   = {.r = 0xFF, .g = 0x70, .b = 0x00}, // Yellow
    [LAYER_PROG]    = {.r = 0x90, .g = 0x00, .b = 0xFF}, // Purple
    [LAYER_NUMBERS] = {.r = 0xFF, .g = 0x20, .b = 0x00}, // Orange
    [LAYER_NAV]     = {.r = 0x00, .g = 0xC0, .b = 0x20}, // Green
    [LAYER_POINTER] = {.r = 0x00, .g = 0x60, .b = 0xFF}, // Blue
    [LAYER_HERDR]   = {.r = 0x00, .g = 0xB0, .b = 0x90}, // Teal
    [LAYER_THEME]   = {.r = 0xFF, .g = 0x00, .b = 0x80}, // Pink
};

// Whether the base-layer keycode is one of the keys held to reach the layer.
static bool is_layer_trigger(uint16_t keycode, uint8_t layer) {
    uint8_t target = 0xFF;
    if (IS_QK_MOMENTARY(keycode)) {
        target = QK_MOMENTARY_GET_LAYER(keycode);
    } else if (IS_QK_LAYER_TAP(keycode)) {
        target = QK_LAYER_TAP_GET_LAYER(keycode);
    }
    if (layer == LAYER_THEME) {
        return target == LAYER_NUMBERS || target == LAYER_MAGIC;
    }
    return target == layer;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Tracked here rather than in layer_state_set_user, because the other half
    // gets the layer state over the split link without calling that hook.
    static uint8_t  last_layer = LAYER_BASE;
    static uint16_t layer_since;
    const uint8_t   layer = get_highest_layer(layer_state);
    if (layer != last_layer) {
        last_layer  = layer;
        layer_since = timer_read();
    }
    if (layer == LAYER_BASE || layer >= ARRAY_SIZE(layer_colors) || timer_elapsed(layer_since) < LAYER_INDICATOR_DELAY) {
        return false;
    }
    const uint8_t val   = rgb_matrix_get_val();
    const rgb_t   color = layer_colors[layer];

    // Turn everything off first, including LEDs that are not under a key.
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            const uint8_t led = g_led_config.matrix_co[row][col];
            if (led == NO_LED || led < led_min || led >= led_max) {
                continue;
            }
            const keypos_t pos     = {.row = row, .col = col};
            const uint16_t keycode = keymap_key_to_keycode(layer, pos);
            if (is_layer_trigger(keymap_key_to_keycode(LAYER_BASE, pos), layer)) {
                rgb_matrix_set_color(led, val, val, val);
            } else if (keycode != KC_NO && keycode != KC_TRNS) {
                rgb_matrix_set_color(led, color.r * val / 255, color.g * val / 255, color.b * val / 255);
            }
        }
    }
    // A firmware key being held to confirm shows full red.
    if (danger_keycode != KC_NO) {
        const uint8_t led = g_led_config.matrix_co[danger_pos.row][danger_pos.col];
        if (led != NO_LED && led >= led_min && led < led_max) {
            rgb_matrix_set_color(led, 0xFF, 0, 0);
        }
    }
    return false;
}
#endif

// Home-row mod tapping terms, from Gentleman's keymap.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LGUI_T(KC_A):
        case RGUI_T(KC_SCLN):
            return 260;
        case LSFT_T(KC_F):
        case RSFT_T(KC_J):
            return 200;
        case LALT_T(KC_S):
        case LCTL_T(KC_D):
        case RCTL_T(KC_K):
        case LALT_T(KC_L):
            return 260;
        default:
            return TAPPING_TERM;
    }
}

// Modifier held while pinching.
enum pinch_mode {
    PINCH_MODE_CMD = 0, // Figma.
    PINCH_MODE_CTRL,
    PINCH_MODE_NONE, // Plain scroll, for AutoCAD.
    PINCH_MODE_COUNT,
};

// User settings persisted in the 32-bit user EEPROM block.
typedef union {
    uint32_t raw;
    struct {
        uint8_t  magic;
        uint8_t  pinch_mode;
        uint16_t cursor_cpi;
    };
} user_config_t;

_Static_assert(sizeof(user_config_t) == sizeof(uint32_t), "user_config_t must fit in 32 bits");

// Bump this whenever the defaults or the keymap change, so devices that were
// flashed before reset to the new defaults on the next boot.
#define USER_CONFIG_MAGIC 0xDD

static user_config_t user_config;

// Cursor speeds, slowest first. Each step changes the speed by 33-50%, so it
// is easy to feel. 400 is the 1:1 speed.
static const uint16_t cursor_cpi_steps[] = {50, 75, 100, 150, 200, 300, 400, 600, 800};

static void user_config_apply(void) {
    pointing_device_set_cpi(user_config.cursor_cpi);
}

static void user_config_set_defaults(void) {
    user_config.raw        = 0;
    user_config.magic      = USER_CONFIG_MAGIC;
    user_config.pinch_mode = PINCH_MODE_CMD;
    user_config.cursor_cpi = MAC_CURSOR_CPI_DEFAULT;
    eeconfig_update_user(user_config.raw);
}

static bool user_config_is_valid(void) {
    return user_config.magic == USER_CONFIG_MAGIC && user_config.pinch_mode < PINCH_MODE_COUNT && user_config.cursor_cpi >= MAC_CURSOR_CPI_MIN && user_config.cursor_cpi <= MAC_CURSOR_CPI_MAX;
}

void eeconfig_init_user(void) {
    user_config_set_defaults();
    user_config_apply();
}

// Switches to the default effect and brightness. This is saved, so VIA shows
// it and later changes made from VIA or keys are kept.
static void rgb_theme_restore(void) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_enable();
    rgb_matrix_mode(RGB_MATRIX_DEFAULT_MODE);
    rgb_matrix_sethsv(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_DEFAULT_VAL);
#endif
}

// Turns the RGB on and switches the effect, keeping the current brightness.
static void rgb_theme_select(uint8_t mode) {
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_enable();
    rgb_matrix_mode(mode);
#else
    (void)mode;
#endif
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    if (!user_config_is_valid()) {
        user_config_set_defaults();
#ifdef DYNAMIC_KEYMAP_ENABLE
        // VIA only reloads its keymap copy when the build date changes, so a
        // firmware flashed on the same day as an older one would keep the old
        // layout and the custom keys would be missing. Reload it from flash.
        dynamic_keymap_reset();
        // The macro area moves when the layer count changes, so clear it too.
        dynamic_keymap_macro_reset();
#endif
        rgb_theme_restore();
    }
    user_config_apply();
}

static void cursor_speed_step(bool faster) {
    const uint16_t cpi  = user_config.cursor_cpi;
    uint16_t       next = cpi;
    if (faster) {
        for (uint8_t i = 0; i < ARRAY_SIZE(cursor_cpi_steps); i++) {
            if (cursor_cpi_steps[i] > cpi) {
                next = cursor_cpi_steps[i];
                break;
            }
        }
    } else {
        for (uint8_t i = ARRAY_SIZE(cursor_cpi_steps); i > 0; i--) {
            if (cursor_cpi_steps[i - 1] < cpi) {
                next = cursor_cpi_steps[i - 1];
                break;
            }
        }
    }
    next = next < MAC_CURSOR_CPI_MIN ? MAC_CURSOR_CPI_MIN : next > MAC_CURSOR_CPI_MAX ? MAC_CURSOR_CPI_MAX : next;
    if (next != cpi) {
        user_config.cursor_cpi = next;
        eeconfig_update_user(user_config.raw);
        user_config_apply();
    }
}

// The fallback reads the modifier when a pinch starts, so changing it mid-pinch is safe.
uint8_t digitizer_pinch_mods_user(void) {
    switch (user_config.pinch_mode) {
        case PINCH_MODE_CTRL:
            return MOD_BIT(KC_LEFT_CTRL);
        case PINCH_MODE_NONE:
            return 0;
        default:
            return MOD_BIT(KC_LEFT_GUI);
    }
}

// True for keys that get the Herdr prefix: plain keys (letters, digits,
// punctuation, Tab, ...) and shifted ones like KC_QUES. Modifiers, layer keys,
// mod-taps, mouse and media keys, KC_NO and the custom keycodes are left alone.
static bool herdr_prefixed(uint16_t keycode) {
    if (IS_QK_MODS(keycode)) {
        keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
    }
    return IS_BASIC_KEYCODE(keycode);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == QK_BOOT || keycode == EE_CLR) {
        if (record->event.pressed) {
            danger_keycode = keycode;
            danger_since   = timer_read();
            danger_pos     = record->event.key;
        } else if (danger_keycode == keycode) {
            danger_keycode = KC_NO; // Released before the hold time: cancel.
        }
        return false;
    }
    if (!record->event.pressed) {
        return true;
    }
    if (IS_LAYER_ON(LAYER_HERDR) && herdr_prefixed(keycode)) {
        // Send the prefix alone, then let the key go out with the held Shift / Alt.
        const uint8_t mods = get_mods();
        clear_mods();
        clear_weak_mods();
        tap_code16(HERDR_PREFIX);
        set_mods(mods);
        return true;
    }
    switch (keycode) {
        case CURSOR_SPEED_DOWN:
            cursor_speed_step(false);
            return false;
        case CURSOR_SPEED_UP:
            cursor_speed_step(true);
            return false;
        case PINCH_MOD_CYCLE:
            user_config.pinch_mode = (user_config.pinch_mode + 1) % PINCH_MODE_COUNT;
            eeconfig_update_user(user_config.raw);
            return false;
        case RGB_THEME_RESTORE:
            rgb_theme_restore();
            return false;
        case RGB_THEME_GRUVBOX:
            rgb_theme_select(RGB_MATRIX_CUSTOM_gruvbox_dark);
            return false;
        case RGB_THEME_ALEGRA:
            rgb_theme_select(RGB_MATRIX_CUSTOM_alegra);
            return false;
        case RGB_THEME_DESERT:
            rgb_theme_select(RGB_MATRIX_CUSTOM_desert);
            return false;
        case RGB_THEME_HORIZON:
            rgb_theme_select(RGB_MATRIX_CUSTOM_horizon);
            return false;
    }
    return true;
}
