#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void sleep_ms(const uint16_t ms);
void get_username(char *buffer, size_t size);
void get_time(char *buffer, size_t size);

#ifdef DEBUG
    #define LOG(fmt, ...)                                                      \
        fprintf(stderr, "[DEBUG] %s: " fmt "\n",                               \
                __func__ __VA_OPT__(, ) __VA_ARGS__)
#else
    #define LOG(...)                                                           \
        do                                                                     \
        {                                                                      \
        } while (0)
#endif
