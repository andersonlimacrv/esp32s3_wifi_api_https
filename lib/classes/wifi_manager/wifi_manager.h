#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "../../../src/Credentials.h"
#include "../utils/http_client/http_client.h"
#include "../utils/ntp_time_sync/ntp_time_sync.h"
#include <string.h>

class WiFiManager {
public:
    WiFiManager();
    void startConnection(const char* ssid, const char* pass, bool useStaticIP);
    bool isConnected() const;

private:
    void setTxPower();
    void initializeWiFi();
    void connect(bool useStaticIP); 
    static void printIpInfoTask(void *pvParameter);
    static void printMacAddressTask(void *pvParameter);
    static void wifiEventHandler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

    static EventGroupHandle_t wifi_event_group;
    static const int WIFI_CONNECTED_BIT = BIT0; 
    const char* ssid;
    const char* pass;
    
    // Variáveis para IP estático
    static const char* STATIC_IP;
    static const char* STATIC_NETMASK;
    static const char* STATIC_GATEWAY;
    static const char* STATIC_DNS;
};


extern WiFiManager wifiManager;

#endif // WIFI_MANAGER_H
