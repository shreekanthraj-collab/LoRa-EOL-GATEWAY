#include "eol_ui.h"

#include "nfw_delay.h"
#include "nfw_gpio.h"

#define EOL_UI_BEEP_LONG_MS       1000U
#define EOL_UI_BEEP_SHORT_MS       200U
#define EOL_UI_BEEP_GAP_MS         200U

static bool s_initialized = false;

static uint32_t button_gpio(EolUiButton_t button)
{
    switch (button)
    {
        case EOL_UI_BUTTON_START:
            return EOL_UI_START_GPIO;

        case EOL_UI_BUTTON_NEXT:
            return EOL_UI_NEXT_GPIO;

        case EOL_UI_BUTTON_FAIL_STOP:
            return EOL_UI_FAIL_STOP_GPIO;

        case EOL_UI_BUTTON_RESET:
            return EOL_UI_RESET_GPIO;

        default:
            return 0U;
    }
}

bool eol_ui_init(void)
{
    const NfwGpioConfig_t button_config =
    {
        .direction = NFW_GPIO_DIRECTION_INPUT,
        .pull = NFW_GPIO_PULL_UP,
        .initialLevel = true
    };

    NfwGpioConfig_t config;

    config = button_config;
    config.pin = EOL_UI_START_GPIO;

    if (nfwGpioInit(&config) != NFW_STATUS_OK)
    {
        return false;
    }

    config.pin = EOL_UI_NEXT_GPIO;

    if (nfwGpioInit(&config) != NFW_STATUS_OK)
    {
        return false;
    }

    config.pin = EOL_UI_FAIL_STOP_GPIO;

    if (nfwGpioInit(&config) != NFW_STATUS_OK)
    {
        return false;
    }

    config.pin = EOL_UI_RESET_GPIO;

    if (nfwGpioInit(&config) != NFW_STATUS_OK)
    {
        return false;
    }

    const NfwGpioConfig_t buzzer_config =
    {
        .pin = EOL_UI_BUZZER_GPIO,
        .direction = NFW_GPIO_DIRECTION_OUTPUT,
        .pull = NFW_GPIO_PULL_NONE,
        .initialLevel = false
    };

    if (nfwGpioInit(&buzzer_config) != NFW_STATUS_OK)
    {
        return false;
    }

    s_initialized = true;

    return true;
}

bool eol_ui_button_pressed(EolUiButton_t button)
{
    bool level = true;

    if (!s_initialized)
    {
        return false;
    }

    if (button_gpio(button) == 0U)
    {
        return false;
    }

    if (nfwGpioRead(button_gpio(button), &level) != NFW_STATUS_OK)
    {
        return false;
    }

    /*
     * Buttons use pull-ups and connect to GND when pressed.
     */
    return !level;
}

static void buzzer_on(void)
{
    (void)nfwGpioWrite(EOL_UI_BUZZER_GPIO, true);
}

static void buzzer_off(void)
{
    (void)nfwGpioWrite(EOL_UI_BUZZER_GPIO, false);
}

void eol_ui_signal_pass(void)
{
    if (!s_initialized)
    {
        return;
    }

    buzzer_on();
    (void)nfwDelayMs(EOL_UI_BEEP_LONG_MS);
    buzzer_off();
}

void eol_ui_signal_fail(void)
{
    if (!s_initialized)
    {
        return;
    }

    for (unsigned int i = 0U; i < 3U; ++i)
    {
        buzzer_on();
        (void)nfwDelayMs(EOL_UI_BEEP_SHORT_MS);

        buzzer_off();

        if (i < 2U)
        {
            (void)nfwDelayMs(EOL_UI_BEEP_GAP_MS);
        }
    }
}
