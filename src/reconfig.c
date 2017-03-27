#include <libmcppot/mcp4xxx.h>

#include "reconfig.h"

int capybara_config_banks(capybara_bankmask_t banks)
{
    // TODO: set GPIOs to OPEN/CLOSE switches
    return 0;
}

int capybara_config_threshold(uint16_t wiper)
{
    pot_set_nv_wiper(wiper);
    return 0;
}
