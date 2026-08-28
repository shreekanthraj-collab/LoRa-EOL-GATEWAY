#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Frozen EOL operator GPIO map.
 *
 * These GPIOs belong to the EOL fixture/controller only.
 * They do NOT modify the frozen Main Gateway radio GPIO map.
 */
#define EOL_UI_START_GPIO       1U
#define EOL_UI_NEXT_GPIO        2U
#define EOL_UI_FAIL_STOP_GPIO   3U
#define EOL_UI_RESET_GPIO       4U
#define EOL_UI_BUZZER_GPIO      15U

typedef enum
{
    EOL_UI_BUTTON_START = 0,
    EOL_UI_BUTTON_NEXT,
    EOL_UI_BUTTON_FAIL_STOP,
    EOL_UI_BUTTON_RESET
} EolUiButton_t;

/**
 * Initialize EOL operator GPIOs.
 */
bool eol_ui_init(void);

/**
 * Read an operator button.
 *
 * Returns true when pressed.
 * Buttons are active LOW.
 */
bool eol_ui_button_pressed(EolUiButton_t button);

/**
 * PASS indication:
 * one long beep.
 */
void eol_ui_signal_pass(void);

/**
 * FAIL indication:
 * three beeps.
 */
void eol_ui_signal_fail(void);

#ifdef __cplusplus
}
#endif
