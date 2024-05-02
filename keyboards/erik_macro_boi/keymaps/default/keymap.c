// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    ERIK_MACRO = SAFE_RANGE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_4x4(
            ERIK_MACRO
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case ERIK_MACRO:
            if (record->event.pressed) {
                SEND_STRING("Happy birthday!!");
            }
            else {
                SEND_STRING(" This keyboard is broken");
            }
            break;
    }

    return true;
}
