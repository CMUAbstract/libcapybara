#include <msp430.h>

#include <libmsp/periph.h>

#include "capybara.h"

static void capybara_config_pins()
{
    GPIO(LIBCAPYBARA_PORT_BOOST_SW, OUT) &= ~BIT(LIBCAPYBARA_PIN_BOOST_SW);
    GPIO(LIBCAPYBARA_PORT_BOOST_SW, DIR) |= BIT(LIBCAPYBARA_PIN_BOOST_SW);
}

void capybara_init()
{
#ifndef LIBCAPYBARA_CONT_POWER // Don't wait if we're on continuous power
    capybara_wait_for_supply();
#if (BOARD_MAJOR == 1 && BOARD_MINOR == 1) || BOARD_MAJOR == 2
    capybara_wait_for_vcap();
#endif // BOARD_{MAJOR,MINOR}
#endif // LIBCAPYBARA_CONT_POWER

    capybara_config_pins();

   // Set up deep_discharge stop
#ifndef LIBCAPYBARA_CONT_POWER
    if (capybara_shutdown_on_deep_discharge() == CB_ERROR_ALREADY_DEEPLY_DISCHARGED) {
        capybara_shutdown();
    }
#endif //LIBCAPYBARA_CONT_POWER
}
