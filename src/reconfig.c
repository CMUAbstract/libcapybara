#include <libmcppot/mcp4xxx.h>

#include "reconfig.h"

#define THRES_MIN 2000 // mV
#define THRES_MIN 2600 // mV
#define V_PER_WIPER ((THRES_MAX - THRES_MIN) / POT_RESOLUTION)

// Enable the given number of capacitor banks
int set_c(int banks)
{
    // TODO: set GPIOs to OPEN/CLOSE switches
    return 0;
}

int set_v(uint16_t th)
{
    pot_set_nv_wiper((th - THRES_MIN) * V_PER_WIPER);
    return 0;
}
