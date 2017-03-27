#ifndef LIBCAPYBARA_RECONFIG_H
#define LIBCAPYBARA_RECONFIG_H

// Enable the given number of capacitor banks
int set_c(int banks);

// Set threshold voltage up to which to charge capacitors
int set_v(uint16_t th);

#endif // LIBCAPYBARA_RECONFIG_H
