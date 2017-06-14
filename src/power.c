#include <msp430.h>

#include <libmsp/periph.h>
#include <libmsp/sleep.h>

#include "power.h"

void capybara_wait_for_supply()
{
    // wait for BOOST_OK: supply voltage stablized
    GPIO(LIBCAPYBARA_PORT_VBOOST_OK, IES) &= ~BIT(LIBCAPYBARA_PIN_VBOOST_OK);
    GPIO(LIBCAPYBARA_PORT_VBOOST_OK, IFG) &= ~BIT(LIBCAPYBARA_PIN_VBOOST_OK);
    GPIO(LIBCAPYBARA_PORT_VBOOST_OK, IE) |= BIT(LIBCAPYBARA_PIN_VBOOST_OK);

    __disable_interrupt(); // classic lock-check-sleep pattern
    while ((GPIO(LIBCAPYBARA_PORT_VBOOST_OK, IN) & BIT(LIBCAPYBARA_PIN_VBOOST_OK)) !=
                BIT(LIBCAPYBARA_PIN_VBOOST_OK)) {
        __bis_SR_register(LPM4_bits + GIE);
        __disable_interrupt();
    }
    __enable_interrupt();

    GPIO(LIBCAPYBARA_PORT_VBOOST_OK, IE) &= ~BIT(LIBCAPYBARA_PIN_VBOOST_OK);
    GPIO(LIBCAPYBARA_PORT_VBOOST_OK, IFG) &= ~BIT(LIBCAPYBARA_PIN_VBOOST_OK);
}

void capybara_wait_for_vcap()
{
    // Wait for Vcap to recover
    // NOTE: this is the crudest implementation: sleep for a fixed interval
    // Alternative implemenation: use Vcap supervisor, comparator, or ADC
    msp_sleep(PERIOD_LIBCAPYBARA_VCAP_RECOVERY_TIME);
}

void capybara_shutdown()
{
    // Disable booster
    GPIO(LIBCAPYBARA_PORT_BOOST_SW, OUT) |= BIT(LIBCAPYBARA_PIN_BOOST_SW);

    // Sleep, while we wait for supply voltage to drop
    __disable_interrupt();
    while (1) {
        __bis_SR_register(LPM4_bits);
    }
}
