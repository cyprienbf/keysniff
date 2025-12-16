#include "../include/utils.h"

#include <assert.h>
#include <stdint.h>
#include <time.h>

#ifdef _WIN32
    #include <lmcons.h>
    #include <windows.h>
#else
    #include <threads.h>
    #include <unistd.h>
#endif

void sleep_ms(const uint16_t ms)
{
    LOG("Sleeping for %u ms", ms);

#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    thrd_sleep(&ts, nullptr);
#endif
}

void get_username(char *buffer, size_t size)
{
    assert(buffer != nullptr);
    assert(size > 0);

    if (!buffer || size < 1)
    {
        LOG("Error: Invalid buffer passed to get_username");
        return;
    }

    const char *final_username = nullptr;

#ifdef _WIN32
    char username_win[UNLEN + 1];
    DWORD username_len = UNLEN + 1;

    LOG("Querying Windows API for username...");
    if (GetUserNameA(username_win, &username_len))
    {
        final_username = username_win;
    }
    else
    {
        LOG("Warning: GetUserNameA failed with error code %lu", GetLastError());
    }
#else
    LOG("Querying system for login name...");
    final_username = getlogin();

    if (!final_username)
    {
        LOG("Warning: getlogin() returned nullptr");
    }
#endif

    if (!final_username)
    {
        final_username = "unknown_user";
    }

    const int32_t res = snprintf(buffer, size, "%s", final_username);

    if (res < 0 || (size_t)res >= size)
    {
        LOG("Warning: Username buffer was truncated (size: %zu)", size);
    }
    else
    {
        LOG("Username retrieved: %s", buffer);
    }
}

void get_time(char *buffer, size_t size)
{
    assert(buffer != nullptr);
    assert(size > 0);

    if (!buffer || !size)
    {
        LOG("Error: Invalid buffer passed to get_time");
        return;
    }

    const time_t now = time(nullptr);
    if (now == (time_t)-1)
    {
        LOG("Error: time() call failed");
        snprintf(buffer, size, "[time_error]");
        return;
    }

    const struct tm *t = localtime(&now);

    if (!t)
    {
        LOG("Error: localtime() returned nullptr");
        snprintf(buffer, size, "[unknown_date]");
    }
    else
    {
        const size_t res = strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);

        if (res == 0)
        {
            LOG("Warning: Time buffer too small for format string");
            snprintf(buffer, size, "[time_truncated]");
        }
        else
        {
            LOG("Current time formatted: %s", buffer);
        }
    }
}