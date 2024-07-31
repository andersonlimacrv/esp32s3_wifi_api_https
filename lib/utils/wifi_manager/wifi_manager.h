#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "../../../src/Credentials.h"
#include "../http_client/http_client.h"
#include "../ntp_time_sync/ntp_time_sync.h"
#include <string.h>

class WiFiManager {
public:
    WiFiManager();
    void startConnection(const char* ssid, const char* pass);
    static void printIpInfoTask(void *pvParameter);
    static void printMacAddressTask(void *pvParameter);
    static void wifiEventHandler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

private:
    const char* ssid;
    const char* pass;
    EventGroupHandle_t wifi_event_group;
    static const int WIFI_CONNECTED_BIT = BIT0; 
    void initializeWiFi();
    void connect();
};

#endif // WIFI_MANAGER_H
