// SPDX-License-Identifier: GPL-2.0-or-later

// Catppuccin Mocha: the accent palette as a gradient, mirrored on both halves,
// with a dimming ripple that spreads from each key press across the keyboard.
RGB_MATRIX_EFFECT(catppuccin_mocha)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// How long a ripple lasts, in ms. It travels CATPPUCCIN_RIPPLE_RADIUS in this time.
#    ifndef CATPPUCCIN_RIPPLE_DURATION
#        define CATPPUCCIN_RIPPLE_DURATION 1300
#    endif

// How far a ripple travels, in LED position units (a key is about 18 units wide and the
// whole keyboard is 224 wide), so 224 crosses both halves from any key.
#    ifndef CATPPUCCIN_RIPPLE_RADIUS
#        define CATPPUCCIN_RIPPLE_RADIUS 224
#    endif

// The width of the ripple ring, in LED position units.
#    ifndef CATPPUCCIN_RIPPLE_WIDTH
#        define CATPPUCCIN_RIPPLE_WIDTH 28
#    endif

// How much the ring darkens the keys it passes, out of 255 (210 is about 82% darker at the
// crest). The ring also fades out as it travels.
#    ifndef CATPPUCCIN_RIPPLE_DIM
#        define CATPPUCCIN_RIPPLE_DIM 210
#    endif

// Scales every color, out of 255, so the effect draws about as much power as a saturated one at
// the same brightness. The pastels load each LED at about 79% on average and a saturated effect
// at about 50%, so 160 (about 50/79) matches them. All channels are scaled equally, so the hues
// stay the same. 255 shows the exact palette values.
#    ifndef CATPPUCCIN_POWER_SCALE
#        define CATPPUCCIN_POWER_SCALE 160
#    endif

#    define CATPPUCCIN_HEX(hex) \
        { .r = ((hex) >> 16) & 0xFF, .g = ((hex) >> 8) & 0xFF, .b = (hex) & 0xFF }

// Catppuccin Mocha accents, from the outer edge of each half to the center.
static const rgb_t catppuccin_mocha_palette[] = {
    CATPPUCCIN_HEX(0xF5E0DC), // Rosewater
    CATPPUCCIN_HEX(0xF2CDCD), // Flamingo
    CATPPUCCIN_HEX(0xF5C2E7), // Pink
    CATPPUCCIN_HEX(0xCBA6F7), // Mauve
    CATPPUCCIN_HEX(0xF38BA8), // Red
    CATPPUCCIN_HEX(0xEBA0AC), // Maroon
    CATPPUCCIN_HEX(0xFAB387), // Peach
    CATPPUCCIN_HEX(0xF9E2AF), // Yellow
    CATPPUCCIN_HEX(0xA6E3A1), // Green
    CATPPUCCIN_HEX(0x94E2D5), // Teal
    CATPPUCCIN_HEX(0x89DCEB), // Sky
    CATPPUCCIN_HEX(0x74C7EC), // Sapphire
    CATPPUCCIN_HEX(0x89B4FA), // Blue
    CATPPUCCIN_HEX(0xB4BEFE), // Lavender
};

#    define CATPPUCCIN_PALETTE_STEPS (ARRAY_SIZE(catppuccin_mocha_palette) - 1)

static rgb_t catppuccin_blend(rgb_t a, rgb_t b, uint8_t amount_of_b) {
    return (rgb_t){blend8(a.r, b.r, amount_of_b), blend8(a.g, b.g, amount_of_b), blend8(a.b, b.b, amount_of_b)};
}

// The gradient color for an LED, from its horizontal distance to the center.
static rgb_t catppuccin_gradient(uint8_t x) {
    const uint8_t  center = k_rgb_matrix_center.x;
    const uint8_t  dist   = x > center ? x - center : center - x;
    const uint8_t  span   = center > 0 ? center : 1;
    const uint16_t pos    = MIN((uint16_t)(span - MIN(dist, span)) * CATPPUCCIN_PALETTE_STEPS * 256 / span, CATPPUCCIN_PALETTE_STEPS * 256 - 1);
    const uint8_t  index  = pos >> 8;
    return catppuccin_blend(catppuccin_mocha_palette[index], catppuccin_mocha_palette[index + 1], pos & 0xFF);
}

// How much the ripple darkens an LED (0 to CATPPUCCIN_RIPPLE_DIM): the strongest ring that reaches it.
static uint8_t catppuccin_ripple(uint8_t led) {
    uint8_t strength = 0;
#    ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    for (uint8_t j = 0; j < g_last_hit_tracker.count; j++) {
        const uint16_t tick = g_last_hit_tracker.tick[j];
        if (tick >= CATPPUCCIN_RIPPLE_DURATION) {
            continue;
        }
        const int16_t  dx     = g_led_config.point[led].x - g_last_hit_tracker.x[j];
        const int16_t  dy     = g_led_config.point[led].y - g_last_hit_tracker.y[j];
        const uint8_t  dist   = sqrt16(dx * dx + dy * dy);
        const uint16_t radius = (uint32_t)tick * CATPPUCCIN_RIPPLE_RADIUS / CATPPUCCIN_RIPPLE_DURATION;
        const uint16_t offset = dist > radius ? dist - radius : radius - dist;
        if (offset >= CATPPUCCIN_RIPPLE_WIDTH) {
            continue;
        }
        const uint8_t ring = (CATPPUCCIN_RIPPLE_WIDTH - offset) * 255 / CATPPUCCIN_RIPPLE_WIDTH;
        const uint8_t fade = (uint32_t)(CATPPUCCIN_RIPPLE_DURATION - tick) * 255 / CATPPUCCIN_RIPPLE_DURATION;
        strength           = MAX(strength, scale8(scale8(ring, fade), CATPPUCCIN_RIPPLE_DIM));
    }
#    else
    (void)led;
#    endif
    return strength;
}

static bool catppuccin_mocha(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // The power scale folds into the brightness, so each channel is scaled once.
    const uint8_t val = scale8(rgb_matrix_get_val(), CATPPUCCIN_POWER_SCALE);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        const rgb_t   rgb     = catppuccin_gradient(g_led_config.point[i].x);
        const uint8_t led_val = scale8(val, 255 - catppuccin_ripple(i));
        rgb_matrix_set_color(i, scale8(rgb.r, led_val), scale8(rgb.g, led_val), scale8(rgb.b, led_val));
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
