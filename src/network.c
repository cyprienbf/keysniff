#include "../include/network.h"
#include "../include/config.h"

#include <curl/curl.h>

CURL *curl = nullptr;

static bool network_init()
{
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    return curl ? true : false; 
}

static bool network_post(const char *data)
{
    if (!curl || !data) { return false; }

    curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

#ifdef DEBUG
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    const CURLcode res = curl_easy_perform(curl);

#ifdef DEBUG
    printf("curl_error: %d\n", (int)res);
#endif

    curl_easy_cleanup(curl);

    return res == CURLE_OK ? true : false;
}

static bool network_clean()
{
    if (curl)
    {
        curl_global_cleanup();
        return true;
    }
    return false;
}

bool send_data(char *data)
{
    if (!network_init()) { return false; }

    if (!network_post(data)) { return false; }

    if (!network_clean())
    {
#ifdef DEBUG
        perror("failed to clean curl");
#endif
    }

    data[0] = '\0'; // -> memset(data, 0, MAX_BUFFER_SIZE);
    return true;
}
