#include "wifi_manager.h"

EventGroupHandle_t WiFiManager::wifi_event_group = nullptr;
WiFiManager wifiManager; // Instância global

WiFiManager::WiFiManager() {
    if (wifi_event_group == nullptr) {
        wifi_event_group = xEventGroupCreate();
        if (wifi_event_group == NULL) {
           printf("[WiFiManager] Failed to create event group");
        }
    }
}

void WiFiManager::startConnection(const char* ssid, const char* pass) {
    this->ssid = ssid;
    this->pass = pass;
    initializeWiFi();
    connect();
}

void WiFiManager::initializeWiFi() {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
}

void WiFiManager::connect() {
    // Register event handlers for Wi-Fi events and IP acquisition
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WiFiManager::wifiEventHandler, this);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &WiFiManager::wifiEventHandler, this);

    // Configure Wi-Fi settings
    wifi_config_t wifi_configuration = {};
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.ssid), ssid, sizeof(wifi_configuration.sta.ssid) - 1);
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.password), pass, sizeof(wifi_configuration.sta.password) - 1);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_configuration);
    esp_wifi_start();

    // Wait for Wi-Fi connection to be established
    EventBits_t uxBits = xEventGroupWaitBits(
        wifi_event_group,               // The event group to wait for
        WIFI_CONNECTED_BIT,             // The bit that should be set
        pdFALSE,                        // Do not clear the bit when it exits
        pdTRUE,                         // Wait for all bits specified (only one in this case)
        portMAX_DELAY                   // Wait indefinitely until the bit is set
    );

    // Check if the connection bit was set
    if (uxBits & WIFI_CONNECTED_BIT) {
        // after Wi-Fi connection is established
        xTaskCreate(ntp_time_sync_task, "ntp_time_sync_task", 2048, NULL, 1, NULL);
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Wait for 5 sec 
        xTaskCreate(client_post_auth_login, "client_post_auth_login", 8192, NULL, 1, NULL);
    } else {
        printf("Failed to connect to Wi-Fi.\n");
    }
}


void WiFiManager::printIpInfoTask(void* pvParameter) {
    if (xEventGroupGetBits(wifi_event_group) & WIFI_CONNECTED_BIT) {
        vTaskDelay(1200 / portTICK_PERIOD_MS);
        esp_netif_ip_info_t ip_info;
        esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info);

        printf("WiFi got IP: " IPSTR "\n", IP2STR(&ip_info.ip));
        printf("Subnet Mask: " IPSTR "\n", IP2STR(&ip_info.netmask));
        printf("Gateway IP: " IPSTR "\n", IP2STR(&ip_info.gw));
    }

    vTaskDelete(NULL);
}

void WiFiManager::printMacAddressTask(void* pvParameter) {
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    vTaskDelete(NULL);
}

void WiFiManager::wifiEventHandler(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    switch (event_id) {
    case WIFI_EVENT_STA_START:
        printf("WiFi CONNECTING.\n");
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        esp_wifi_connect();
        break;
    case WIFI_EVENT_STA_CONNECTED:
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        printf("WiFi CONNECTED SUCCESSFULLY.\n");
        xTaskCreate(WiFiManager::printMacAddressTask, "print_mac_address_task", 2048, NULL, 2, NULL);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        printf("WiFi DISCONNECTED...\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        esp_wifi_connect(); // Retry connection
        break;
    case IP_EVENT_STA_GOT_IP:
        xTaskCreate(WiFiManager::printIpInfoTask, "print_ip_info_task", 2048, NULL, 1, NULL);
        break;
    default:
        break;
    }
}

bool WiFiManager::isConnected() const {
    return (xEventGroupGetBits(wifi_event_group) & WIFI_CONNECTED_BIT) != 0;
}
