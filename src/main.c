#include "../include/config.h"
#include "../include/network.h"
#include "../include/input.h"

int main()
{
    char data[MAX_BUFFER_SIZE];
    while (true)
    {
        get_data(data, sizeof(data));
        if (!send_data(data)) { return 1; };
    }
    return 0;
}
