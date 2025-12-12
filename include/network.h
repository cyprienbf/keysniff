#ifndef NETWORK_H
#define NETWORK_H

#include "config.h"

bool network_init();

bool network_post(const char *data);

bool network_clean();

// NETWORK_H
#endif
