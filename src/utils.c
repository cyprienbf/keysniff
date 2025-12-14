#include "../include/utils.h"

#include <threads.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #include <lmcons.h>
#else
    #include <unistd.h>
#endif

void sleep_ms(const uint16_t ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    thrd_sleep(&ts, NULL);
}

void get_username(char *buffer, size_t size)
{
#ifdef _WIN32
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;

    if (!GetUserNameA(username, &username_len))
    {
       snprintf(username, username_len, "unknown_username");
    }
    // GetUserName(usr, &len)
#else
    const char *username = getlogin();
#endif
    snprintf(buffer, size, "%s", username ? username : "unknown_username");
}

void get_time(char *buffer, size_t size)
{
    if (!buffer || !size) { return; }

    const time_t now = time(nullptr);
    const struct tm *t = localtime(&now);
    
    if (!t)
    {
        snprintf(buffer, size, "[unknown_date]");
    }
    else
    {
        size_t res = strftime(buffer, size, "[%Y-%m-%d %H:%M:%S]", t);
        
        if (res == 0)
        {
            snprintf(buffer, size, "[unknown_date]");
        }
    }
}

