#include <msp430.h>
#include <stdio.h>
#include <libmspware/driverlib.h>
#include <libmspware/i2c_setup.h>

#include <libmsp/watchdog.h>
#include <libmsp/clock.h>
#include <libmsp/gpio.h>
#include <libmsp/periph.h>
#include <libmsp/sleep.h>
#include <libmsp/mem.h>
#include <libmsp/uart.h>
//#include <libmspuartlink/uartlink.h>

#if BOARD_MAJOR == 1 && BOARD_MINOR == 1
#include <libfxl/fxl6408.h>
#endif// BOARD_{MAJOR,MINOR}

//Other stuff
#include <libio/console.h>
#include "reconfig.h"
#include "power.h"
#include "capybara.h"
#include "capy_board_init.h"
#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x

/** @brief Handler for capybara power-on sequence
    TODO add this to libcapybara...
*/
void capy_board_init(void) {
    msp_watchdog_disable();
    msp_gpio_unlock();
		__enable_interrupt();
// Don't wait if we're on continuous power
#ifndef LIBCAPYBARA_CONT_POWER
#pragma message ("continuous power not defined!")
capybara_wait_for_supply();
#if BOARD_MAJOR == 1 && BOARD_MINOR == 1
    capybara_wait_for_vcap();
#endif // BOARD_{MAJOR,MINOR}
#endif // LIBCAPYBARA_CONT_POWER
    capybara_config_pins();
    msp_clock_setup();
// Set up deep_discharge stop
#ifndef LIBCAPYBARA_CONT_POWER
#if BOARD_MAJOR == 1 && BOARD_MINOR == 0
    capybara_shutdown_on_deep_discharge();
#elif BOARD_MAJOR == 1 && BOARD_MINOR == 1
		capybara_wait_for_supply();
    if (capybara_shutdown_on_deep_discharge() == CB_ERROR_ALREADY_DEEPLY_DISCHARGED) {
        capybara_shutdown();
    }
#endif //BOARD.{MAJOR,MINOR}
#endif //LIBCAPYBARA_CONT_POWER

#if BOARD_MAJOR == 1 && BOARD_MINOR == 0
    GPIO(PORT_SENSE_SW, OUT) &= ~BIT(PIN_SENSE_SW);
    GPIO(PORT_SENSE_SW, DIR) |= BIT(PIN_SENSE_SW);

    GPIO(PORT_RADIO_SW, OUT) &= ~BIT(PIN_RADIO_SW);
    GPIO(PORT_RADIO_SW, DIR) |= BIT(PIN_RADIO_SW);

    P3OUT &= ~BIT5;
    P3DIR |= BIT5;

    P3OUT &= ~BIT0;
    P3DIR |= BIT0;
    GPIO(PORT_DEBUG, OUT) &= ~BIT(PIN_DEBUG);
    GPIO(PORT_DEBUG, DIR) |= BIT(PIN_DEBUG);
#elif BOARD_MAJOR == 1 && BOARD_MINOR == 1

    INIT_CONSOLE();
    __enable_interrupt();
    msp_gpio_unlock();
    LOG2("Setting up i2c\r\n");
    EUSCI_B_I2C_setup();
    LOG2("fxl init\r\n");
    fxl_init();
    LOG2("RADIO_SW\r\n");

    fxl_out(BIT_PHOTO_SW);
    fxl_out(BIT_RADIO_SW);
    fxl_out(BIT_RADIO_RST);
    fxl_out(BIT_APDS_SW);
    fxl_pull_up(BIT_CCS_WAKE);
    LOG2("Done fxl!\r\n");
		// SENSE_SW is present but is not electrically correct: do not use.
#else // BOARD_{MAJOR,MINOR}
#error Unsupported board: do not know what pins to configure (see BOARD var)
#endif // BOARD_{MAJOR,MINOR}
    __enable_interrupt();
}

