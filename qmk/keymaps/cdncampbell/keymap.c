// Rob's KB16-01 keymap: the four workflow layers from cdncampbell-KB16_01.layout.json,
// plus a lighting hook that paints the whole pad in the active layer's colour.
// VIA stays enabled, so VIA's saved keymap (EEPROM) wins over the defaults below.

#include QMK_KEYBOARD_H

#include "lib/layer_status/layer_status.h"

enum layer_names {
    _BASE,
    _CURSOR,
    _TERM,
    _OBSIDIAN
};

// Matrix columns 0-3 are the 4x4 keys; column 4 is the press on each dial
// (small top-left, small right, big). The fourth row has no dial, so it has four keys.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Browser, windows, screenshots
    [_BASE] = LAYOUT(
        C(KC_C),    C(KC_V),    C(KC_S),       C(KC_F),       C(KC_0),
        C(KC_T),    C(KC_W),    C(S(KC_T)),    C(KC_L),       TO(_CURSOR),
        A(KC_LEFT), A(KC_RGHT), KC_PSCR,       S(G(KC_LEFT)), TO(_BASE),
        G(KC_LEFT), G(KC_UP),   G(KC_RGHT),    S(G(KC_RGHT))
    ),
    // Cursor: AI, editing, panels
    [_CURSOR] = LAYOUT(
        C(KC_C),    C(KC_V),    C(KC_S),       C(KC_F),       C(KC_0),
        C(KC_L),    C(KC_K),    C(KC_ENT),     C(KC_BSPC),    TO(_TERM),
        C(KC_SLSH), C(S(KC_I)), KC_F2,         C(KC_D),       TO(_BASE),
        C(KC_GRV),  C(KC_B),    C(KC_BSLS),    C(S(KC_F))
    ),
    // Terminal and git. Ctrl+C is SIGINT and Ctrl+S is XOFF in Ptyxis, so copy, paste
    // and find use Shift and save is off. The git commands are VIA macros M0-M9.
    [_TERM] = LAYOUT(
        C(S(KC_C)), C(S(KC_V)), KC_NO,         C(S(KC_F)),    C(KC_0),
        QK_MACRO_0, QK_MACRO_1, QK_MACRO_2,    QK_MACRO_3,    TO(_OBSIDIAN),
        QK_MACRO_4, QK_MACRO_5, QK_MACRO_6,    QK_MACRO_7,    TO(_BASE),
        QK_MACRO_8, QK_MACRO_9, C(KC_C),       C(KC_R)
    ),
    // Obsidian: notes, editing, view. The MEH combos need binding in Obsidian's hotkeys.
    [_OBSIDIAN] = LAYOUT(
        C(KC_C),    C(KC_V),    C(KC_S),       C(KC_F),       C(KC_0),
        C(KC_N),    MEH(KC_D),  MEH(KC_I),     C(KC_E),       TO(_BASE),
        C(KC_L),    C(KC_B),    C(KC_I),       C(KC_K),       TO(_BASE),
        C(KC_G),    MEH(KC_LBRC), MEH(KC_RBRC), MEH(KC_BSLS)
    ),
};

#ifdef ENCODER_MAP_ENABLE
// Dials: small top-left zooms, small right switches tabs, big one undoes. On the
// terminal layer the big dial scrolls instead, because Ctrl+Z suspends the running job.
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE]     = { ENCODER_CCW_CW(C(KC_MINS), C(KC_EQL)),    ENCODER_CCW_CW(C(KC_PGUP), C(KC_PGDN)),   ENCODER_CCW_CW(C(KC_Z), C(S(KC_Z))) },
    [_CURSOR]   = { ENCODER_CCW_CW(C(KC_MINS), C(KC_EQL)),    ENCODER_CCW_CW(C(KC_PGUP), C(KC_PGDN)),   ENCODER_CCW_CW(C(KC_Z), C(S(KC_Z))) },
    [_TERM]     = { ENCODER_CCW_CW(C(KC_MINS), C(S(KC_EQL))), ENCODER_CCW_CW(C(KC_PGUP), C(KC_PGDN)),   ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
    [_OBSIDIAN] = { ENCODER_CCW_CW(C(KC_MINS), C(KC_EQL)),    ENCODER_CCW_CW(C(S(KC_TAB)), C(KC_TAB)),  ENCODER_CCW_CW(C(KC_Z), C(S(KC_Z))) },
};
#endif

#ifdef RGB_MATRIX_ENABLE
// Hue per layer; must match the colours on kb16-cheat-sheet-colours.html.
static const uint8_t layer_hue[] = {
    [_BASE]     = 170, // blue
    [_CURSOR]   = 21,  // orange
    [_TERM]     = 85,  // green
    [_OBSIDIAN] = 191, // purple
};

// Runs on every lighting frame and overwrites whatever effect is selected, so the pad
// always shows the layer colour. Brightness still follows the board's brightness setting.
// Keys that do nothing on this layer (KC_NO) stay dark. keymap_key_to_keycode reads
// VIA's live keymap, so this keeps working after keys are remapped in VIA.
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer >= ARRAY_SIZE(layer_hue)) {
        return false;
    }
    rgb_t on = hsv_to_rgb((hsv_t){layer_hue[layer], 255, rgb_matrix_get_val()});

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led = g_led_config.matrix_co[row][col];
            if (led == NO_LED || led < led_min || led >= led_max) {
                continue; // dial presses have no LED
            }
            if (keymap_key_to_keycode(layer, (keypos_t){.row = row, .col = col}) == KC_NO) {
                rgb_matrix_set_color(led, 0, 0, 0);
            } else {
                rgb_matrix_set_color(led, on.r, on.g, on.b);
            }
        }
    }
    return false;
}
#endif

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    render_layer_status();
    return true;
}
#endif
