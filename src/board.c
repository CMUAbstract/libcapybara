#include <msp430.h>

#include <libmspware/driverlib.h>
#include <libmsp/periph.h>
#include <libio/console.h>

#if (BOARD_MAJOR == 1 && BOARD_MINOR == 1) || BOARD_MAJOR == 2
#include <libfxl/fxl6408.h>
#endif// BOARD_{MAJOR,MINOR}

#include "reconfig.h"
#include "power.h"
#include "capybara.h"
#include "board.h"

static void i2c_setup(void) {
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
	//PRINTF("set periph move\r\n");

	EUSCI_B_I2C_initMasterParam param = {0};
  param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
  param.i2cClk = CS_getSMCLK();
  param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
  param.byteCounterThreshold = 0;
  param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
  //PRINTF("Done param init\r\n");

  //EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);
  // Dumpting the contents here because clang and gcc must have different
  // calling conventions, or at least different inlining conventions!

  uint16_t preScalarValue;

  //Disable the USCI module and clears the other bits of control register
  HWREG16(EUSCI_B0_BASE + OFS_UCBxCTLW0) = UCSWRST;

  //Configure Automatic STOP condition generation
  HWREG16(EUSCI_B0_BASE + OFS_UCBxCTLW1) &= ~UCASTP_3;
  HWREG16(EUSCI_B0_BASE + OFS_UCBxCTLW1) |= param.autoSTOPGeneration;

  //Byte Count Threshold
  HWREG16(EUSCI_B0_BASE + OFS_UCBxTBCNT) = param.byteCounterThreshold;
  /*
   * Configure as I2C master mode.
   * UCMST = Master mode
   * UCMODE_3 = I2C mode
   * UCSYNC = Synchronous mode
   */
  HWREG16(EUSCI_B0_BASE + OFS_UCBxCTLW0) |= UCMST + UCMODE_3 + UCSYNC;

  //Configure I2C clock source
  HWREG16(EUSCI_B0_BASE +
          OFS_UCBxCTLW0) |= (param.selectClockSource + UCSWRST);

  /*
   * Compute the clock divider that achieves the fastest speed less than or
   * equal to the desired speed.  The numerator is biased to favor a larger
   * clock divider so that the resulting clock is always less than or equal
   * to the desired clock, never greater.
   */
  // For now, this is hardcoded to 8000000 / 40000 precalculated since clang
  // doesn't have a __udivsi function **sigh**
  // TODO make this rely on macros at least!
  //preScalarValue = (uint16_t)(param.i2cClk / param.dataRate);
  preScalarValue = (uint16_t)(200);
  HWREG16(EUSCI_B0_BASE + OFS_UCBxBRW) = preScalarValue;
  // Done i2c initialization
}

/** @brief Handler for capybara power-on sequence
    TODO add this to libcapybara...
*/
void capybara_board_init(void) {

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
#elif (BOARD_MAJOR == 1 && BOARD_MINOR == 1) || BOARD_MAJOR == 2

    LOG2("Setting up i2c\r\n");
    i2c_setup();
    LOG2("fxl init\r\n");
    fxl_init();

#if (BOARD_MAJOR == 1 && BOARD_MINOR == 1)
    fxl_out(BIT_PHOTO_SW);
    fxl_out(BIT_RADIO_SW);
    fxl_out(BIT_RADIO_RST);
    fxl_out(BIT_APDS_SW);
    fxl_pull_up(BIT_CCS_WAKE);

    // SENSE_SW is present but is not electrically correct: do not use.

#elif BOARD_MAJOR == 2
    LOG2("SENSE_SW\r\n");
    fxl_out(BIT_SENSE_SW);
#endif // BOARD VERSION 1.1 and 2

    LOG2("Done fxl!\r\n");

#else // BOARD_{MAJOR,MINOR}
#error Unsupported board: do not know what pins to configure (see BOARD var)
#endif // BOARD_{MAJOR,MINOR}
}
