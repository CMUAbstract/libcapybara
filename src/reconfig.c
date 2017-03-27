#include <msp430.h>

#include <libmcppot/mcp4xxx.h>
#include <libmsp/gpio.h>
#include <libmsp/periph.h>

#include "reconfig.h"

#if defined(LIBCAPYBARA_SWITCH_DESIGN__NC)

#if defined(LIBCAPYBARA_SWITCH_CONTROL__TWO_PIN)

#define BANK_PORT_INNER(i, op) LIBCAPYBARA_BANK_PORT_ ## i ## _ ## op ## _PORT
#define BANK_PORT(i, op) BANK_PORT_INNER(i, op)

#define BANK_PIN_INNER(i, op) LIBCAPYBARA_BANK_PORT_ ## i ## _ ## op ## _PIN
#define BANK_PIN(i, op) BANK_PIN_INNER(i, op)

#define BANK_CONNECT(i) do { \
        GPIO(BANK_PORT(i, CLOSE), OUT) |= BIT(BANK_PIN(i, CLOSE)); \
        GPIO(BANK_PORT(i, CLOSE), DIR) |= BIT(BANK_PIN(i, CLOSE)); \
        GPIO(BANK_PORT(i, CLOSE), DIR) &= ~BIT(BANK_PIN(i, CLOSE)); \
    } while (0);

#define BANK_DISCONNECT(i) do { \
        GPIO(BANK_PORT(i, OPEN), OUT) &= ~BIT(BANK_PIN(i, OPEN)); \
        GPIO(BANK_PORT(i, OPEN), DIR) |= BIT(BANK_PIN(i, OPEN)); \
        GPIO(BANK_PORT(i, OPEN), DIR) &= ~BIT(BANK_PIN(i, OPEN)); \
    } while (0);

#elif defined(LIBCAPYBARA_SWITCH_CONTROL__ONE_PIN)

#define BANK_PORT_INNER(i) LIBCAPYBARA_BANK_PORT_ ## i ## _ ## _PORT
#define BANK_PORT(i) BANK_PORT_INNER(i, op)

#define BANK_PIN_INNER(i) LIBCAPYBARA_BANK_PORT_ ## i ## _ ## _PIN
#define BANK_PIN(i) BANK_PIN_INNER(i)

#define BANK_CONNECT(i) do { \
        GPIO(BANK_PORT(i), OUT) &= ~BIT(BANK_PIN(i)); \
        GPIO(BANK_PORT(i), DIR) |= BIT(BANK_PIN(i)); \
        GPIO(BANK_PORT(i), DIR) &= ~BIT(BANK_PIN(i)); \
    } while (0);

#else // LIBCAPYBARA_SWITCH_CONTROL
#error Invalid value of config option: LIBCAPYBARA_SWITCH_CONTROL
#endif // LIBCAPYBARA_SWITCH_CONTROL

#elif defined(LIBCAPYBARA_SWITCH_DESIGN__NO)

#error Not implemented: switch design NO
#else // LIBCAPYBARA_SWITCH_DESIGN
#error Invalid value of config option: LIBCAPYBARA_SWITCH_DESIGN
#endif // LIBCAPYBARA_SWITCH_DESIGN

int capybara_config_banks(capybara_bankmask_t banks)
{
    // If the switches would be all on one port, we'd do this in one
    // assignment. Since on our board, they are hooked up to two
    // ports, we either have optimal code that does two assignments
    // but is not generic, or we have generic code that does four
    // assignments. We do the latter here.

    // NOTE: This is not a loop, because the pins and ports are
    // resolved at compile time. We don't want a runtime map.

#define CONFIG_BANK(i) \
    if (banks & (1 << i)) { BANK_CONNECT(i); } else { BANK_DISCONNECT(i); }

    CONFIG_BANK(0);
    CONFIG_BANK(1);
    CONFIG_BANK(2);
    CONFIG_BANK(3);

    return 0;
}

int capybara_config_threshold(uint16_t wiper)
{
    // TODO: read and avoid writing if value is the same to reduce wear on the EEPROM
    pot_set_nv_wiper(wiper);
    return 0;
}

int capybara_config(capybara_bankmask_t banks, uint16_t wiper)
{
    int rc;

    rc = capybara_config_banks(banks);
    if (rc) return rc;

    rc = capybara_config_threshold(wiper);
    if (rc) return rc;

    return 0;
}
