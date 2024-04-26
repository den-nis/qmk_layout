/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H

bool process_record_user(uint16_t keycode, keyrecord_t *record);
void move_workspace_linux(int workspace);
void move_workspace_windows(int workspace);

enum custom_keycodes {
    TOGL = SAFE_RANGE, //Toggle between windows or linux mode
    MODF, //Modifier, my custom modifier key, currently capslock
    MTO1, //Move to workspace 1
    MTO2,
    MTO3,
    MTO4,
    MTO5,
    MTO6,
};

const uint8_t WORKSPACE_RGB_INDEX[] = { 6, 12, 18, 23, 28, 34, 39, 44, 50, 56, 61, 66, 69 };
const uint8_t WORKSPACE_RGB_INDEX_LENGTH = sizeof(WORKSPACE_RGB_INDEX) / sizeof(uint8_t);
const uint16_t LINUX_WORKSPACE_KEYS[] = { KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19 };

bool windows_mode = false;
int mod_layer = 3;
int current_workspace = 0;

int sign(int x) {
    return (x > 0) - (x < 0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (keycode >= MTO1 && keycode <= MTO6 && record->event.pressed)
    {
        int desired_workspace = keycode - MTO1;

        if (windows_mode) {
            move_workspace_windows(desired_workspace);
        } else {
            move_workspace_linux(desired_workspace);
        }
    }

    switch (keycode) {
        case TOGL:
            if (record->event.pressed) {
                windows_mode = !windows_mode;
            }
            break;
    }

    return true;
};

void move_workspace_linux(int desired_workspace) {

    tap_code(LINUX_WORKSPACE_KEYS[desired_workspace]);
    current_workspace = desired_workspace;
}

void move_workspace_windows(int desired_workspace) {

    if (current_workspace == desired_workspace) {
        return;
    }

    register_code(KC_LGUI);
    register_code(KC_LCTL);

    while (current_workspace != desired_workspace) {

        int direction = sign(desired_workspace - current_workspace);

        if (direction == 1) {
            tap_code(KC_RGHT);
        }

        if (direction == -1) {
            tap_code(KC_LEFT);
        }

        current_workspace += direction;
        wait_ms(100);
    }

    unregister_code(KC_LGUI);
    unregister_code(KC_LCTL);

    return;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = led_min; i < led_max; i++) {

        if (windows_mode) {
            rgb_matrix_set_color(i, RGB_BLUE);
        } else {
            rgb_matrix_set_color(i, RGB_ORANGE);
        }

        for (uint8_t j = 0; j < WORKSPACE_RGB_INDEX_LENGTH; j++) {
            if (WORKSPACE_RGB_INDEX[j] == i) {
                if (j == current_workspace) {
                    rgb_matrix_set_color(i, RGB_WHITE);
                }
            }
        }
    }

    return false;
}


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the QK_BOOT key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR,          KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
        MO(2) , KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGDN,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   TOGL,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        _______, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,          _______,
        _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, RGB_VAI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,            _______,
        KC_CAPS, _______, RGB_VAD, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, RGB_HUI, _______, _______, _______, NK_TOGG, _______, _______, _______, _______,          _______, RGB_MOD, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    ),

    [2] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, MTO2   , MTO3   , _______, KC_LCBR, KC_RCBR, KC_PIPE, KC_EQL , _______, _______, _______,          _______,
        _______, _______, _______, _______, MTO1   , MTO4   , _______, KC_LPRN, KC_RPRN, KC_AMPR, _______, KC_DQT ,          _______,          _______,
        _______,          _______, _______, _______, MTO5   , MTO6   , KC_LBRC, KC_RBRC, KC_DQUO, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) }
};
#endif





