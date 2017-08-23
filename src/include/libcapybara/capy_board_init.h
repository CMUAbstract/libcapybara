#ifndef CAPY_BOARD_INIT_H
#define CAPY_BOARD_INIT_H

void i2c_setup();
void capy_board_init();

#if BOARD_MAJOR == 1 && BOARD_MINOR == 0
#define PORT_SENSE_SW 3
#define PIN_SENSE_SW  7


#define PORT_RADIO_SW 3
#define PIN_RADIO_SW  2

#elif BOARD_MAJOR == 1 && BOARD_MINOR == 1

#define PORT_PHOTO_SENSE 2
#define PIN_PHOTO_SENSE 3 // GPIO extender pins
#define BIT_CCS_WAKE  (1 << 2)
#define BIT_SENSE_SW  (1 << 3)
#define BIT_PHOTO_SW  (1 << 4)
#define BIT_APDS_SW   (1 << 5)
#define BIT_RADIO_RST (1 << 6)
#define BIT_RADIO_SW  (1 << 7)

#endif // BOARD.{MAJOR,MINOR}

#endif//CAPY_BOARD_INIT_H
