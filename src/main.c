#include "../include/config.h"
#include "../include/network.h"
#include "../include/input.h"

#include <stdio.h>

int main()
{
    char data[MAX_BUFFER_SIZE];
    size_t current_len = 0;

    while (true)
    {
        listener(data, &current_len);
        if (!send_data(data)) { return 1; };
    }
    return 0;
}
