#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"

#ifdef LINUX

struct input_event {
    // struct timeval time;
    uint16_t type;
    uint16_t code;
    int32_t value;
};

static constexpr char KEYBOARD_PATH[] = "/dev/input/event0";

bool is_key_pressed(int key);
void listener();

#endif

// INPUT_H
#endif
