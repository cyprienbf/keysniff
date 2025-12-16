#include "../include/network.h"
#include "../include/config.h"
#include "../include/utils.h"

#include <assert.h>
#include <curl/curl.h>
#include <string.h>

CURL *curl = nullptr;

[[nodiscard]]
static bool network_init()
{
    assert(curl == nullptr);

    LOG("Initializing global curl environment...");

    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    {
        LOG("Critical: curl_global_init failed");
        return false;
    }

    curl = curl_easy_init();

    if (curl)
    {
        LOG("Network initialized successfully");
        return true;
    }

    LOG("Critical: curl_easy_init failed");
    curl_global_cleanup();
    return false;
}

[[nodiscard]]
static bool network_post(const char *data)
{
    assert(curl != nullptr);
    assert(data != nullptr);

    if (!curl || !data)
    {
        LOG("Error: Invalid state (curl or data is nullptr)");
        return false;
    }

    LOG("Preparing POST request to: %s", SERVER_URL);
    LOG("Payload size: %zu bytes", strlen(data));

    curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

#ifdef DEBUG
    LOG("Warning: Disabling SSL verification for debugging");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    const CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        LOG("curl_easy_perform failed: %s", curl_easy_strerror(res));
    }
    else
    {
        LOG("POST request sent successfully");
    }

    return (res == CURLE_OK);
}

static bool network_clean()
{
    LOG("Starting network cleanup...");

    if (curl)
    {
        curl_easy_cleanup(curl);
        curl = nullptr;
        curl_global_cleanup();

        LOG("Network resources released");
        return true;
    }
    else
    {
        LOG("Warning: network_clean called but curl was already nullptr");
        return false;
    }
}

bool send_data(char *data)
{
    assert(data != nullptr);

    if (!data)
    {
        LOG("Error: send_data received nullptr");
        return false;
    }

    if (!network_init())
    {
        LOG("Error: Initialization failed");
        return false;
    }

    const bool success = network_post(data);

    if (!success)
    {
        LOG("Error: POST operation failed");
    }

    network_clean();

    data[0] = '\0';

    LOG("Sequence complete. Result: %s", success ? "TRUE" : "FALSE");
    return success;
}