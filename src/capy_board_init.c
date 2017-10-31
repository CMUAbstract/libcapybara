#include <msp430.h>
#include <stdio.h>
#include <libmspware/driverlib.h>

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
    //LOG2("i2c init\r\n");
   /* i2c_setup();
    LOG2("fxl init\r\n");
    fxl_init();
    LOG2("RADIO_SW\r\n");

    fxl_out(BIT_PHOTO_SW);
    fxl_out(BIT_RADIO_SW);
    fxl_out(BIT_RADIO_RST);
    fxl_out(BIT_APDS_SW);
    fxl_pull_up(BIT_CCS_WAKE);
		*/
		// SENSE_SW is present but is not electrically correct: do not use.
#else // BOARD_{MAJOR,MINOR}
#error Unsupported board: do not know what pins to configure (see BOARD var)
#endif // BOARD_{MAJOR,MINOR}
}

// TODO put this in a library of useful things
void i2c_setup(void) {
  /*
  * Select Port 1
  * Set Pin 6, 7 to input Secondary Module Function:
  *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL)
  */
    GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_P1,
    GPIO_PIN6 + GPIO_PIN7,
    GPIO_SECONDARY_MODULE_FUNCTION
  );
	//LOG2("In i2c init\r\n");

	EUSCI_B_I2C_initMasterParam param = {0};
  param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
  param.i2cClk = CS_getSMCLK();
  param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
  param.byteCounterThreshold = 0;
  param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
  //LOG2("In i2c init\r\n");

  EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);
  LOG2("In i2c init\r\n");
}

