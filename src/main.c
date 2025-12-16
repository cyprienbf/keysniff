#include "../include/config.h"
#include "../include/input.h"
#include "../include/network.h"
#include "../include/utils.h"

int main()
{
    LOG("Application started (Debug Mode)");
    LOG("Buffer size initialized to: %d bytes", MAX_BUFFER_SIZE);

    char data[MAX_BUFFER_SIZE] = {0};

    while (true)
    {
        LOG("Waiting for input data...");

        get_data(data, sizeof(data));

        LOG("Data Captured:\n%s", data);

        LOG("Initiating network transfer...");

        if (!send_data(data))
        {
            LOG("Critical Error: send_data returned false. Terminating.");
            return 1;
        }

        LOG("Transfer successful. Resetting loop.");
    }

    return 0;
}