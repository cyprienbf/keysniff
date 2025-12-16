#pragma once

#define DEBUG

#define SERVER_URL "https://post.cyprien.ovh/"

#ifdef DEBUG
    #define MAX_BUFFER_SIZE 1024 * 2
#else
    #define MAX_BUFFER_SIZE (1024 * 100) - 512
#endif
