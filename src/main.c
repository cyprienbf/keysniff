#include <stdio.h>
#include <stdint.h>

#include "../include/config.h"
#include "../include/network.h"

int main(int argc, char *argv[])
{
    char data[MAX_BUFFER_SIZE];
    snprintf(data, sizeof(data), "test: %s", argc > 1 ? argv[1] : "default");

#ifdef DEBUG
    printf("data: %s\n", data);
#endif

    if (network_init())
    {
        network_post(data);
        network_clean();
    }
    else
    {
        return 1;
    }

    return 0;
}
