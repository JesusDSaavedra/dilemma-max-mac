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
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_FREE_1, // Layers 4 to 7 in VIA: empty (transparent), free to fill from VIA.
    LAYER_FREE_2,
    LAYER_FREE_3,
    LAYER_FREE_4,
    LAYER_THEME, // Layer 8 in VIA: lighting and theme controls, on LOWER + RAISE.
};

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)

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
#define RGB_RST RGB_THEME_RESTORE
#define THM_CAT RGB_THEME_RESTORE
#define THM_GRV RGB_THEME_GRUVBOX
#define THM_ALG RGB_THEME_ALEGRA
#define THM_DES RGB_THEME_DESERT
#define THM_HOR RGB_THEME_HORIZON

// clang-format off
#define LAYOUT_TRANSPARENT LAYOUT( \
    _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______, \
                      _______, _______, _______, _______,    _______, _______, _______, _______)
// clang-format on

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_LALT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         KC_LALT, KC_BSPC,  KC_SPC,   LOWER,      RAISE,  KC_ENT,  KC_DEL, KC_LGUI
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  [LAYER_LOWER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_NEXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_RST,    KC_LBRC,   KC_P7,   KC_P8,   KC_P9, KC_RBRC, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_TOGG, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_PPLS,   KC_P4,   KC_P5,   KC_P6, KC_PMNS, KC_PEQL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_PREV, XXXXXXX, XXXXXXX, XXXXXXX,  EE_CLR, QK_BOOT,    KC_PAST,   KC_P1,   KC_P2,   KC_P3, KC_PSLS, KC_PDOT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         XXXXXXX, XXXXXXX, XXXXXXX, _______,    _______, _______, _______, _______
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  [LAYER_RAISE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_F12,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    CSPD_DN, CSPD_UP, PINCH_M, XXXXXXX, XXXXXXX, KC_VOLU,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPLY, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, KC_MUTE,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPRV, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, XXXXXXX,    QK_BOOT,  EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         _______, _______, _______, _______,    _______, XXXXXXX, XXXXXXX, XXXXXXX
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  // Explicitly transparent, so VIA's reset fills them with _______ instead of KC_NO.
  [LAYER_FREE_1] = LAYOUT_TRANSPARENT,
  [LAYER_FREE_2] = LAYOUT_TRANSPARENT,
  [LAYER_FREE_3] = LAYOUT_TRANSPARENT,
  [LAYER_FREE_4] = LAYOUT_TRANSPARENT,

  // Hold LOWER + RAISE. Theme and lighting controls on the right half.
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

// LOWER + RAISE together turn on the theme layer.
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, LAYER_LOWER, LAYER_RAISE, LAYER_THEME);
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
#define USER_CONFIG_MAGIC 0xDB

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
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
