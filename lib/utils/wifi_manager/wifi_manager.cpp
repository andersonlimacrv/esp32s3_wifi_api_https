#include "wifi_manager.h"

/* 
1. Inicializa a conexão Wi-Fi;
2. Aguarda até que a conexão Wi-Fi seja estabelecida;
3. Obtém e imprime o endereço MAC do dispositivo uma vez;
4. Quando conectado, imprime as informações de IP uma vez;
5. Gerencia eventos de conexão, desconexão e obtenção de IP;
6. Faz login na AP;
7. Faz sincroniza NTP (Simple Network Time Protocol).
*/

WiFiManager::WiFiManager() {
    wifi_event_group = xEventGroupCreate();
    if (wifi_event_group == NULL) {
        ESP_LOGE("WiFiManager", "Failed to create event group");
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
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WiFiManager::wifiEventHandler, this);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &WiFiManager::wifiEventHandler, this);

    wifi_config_t wifi_configuration = {};
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.ssid), ssid, sizeof(wifi_configuration.sta.ssid) - 1);
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.password), pass, sizeof(wifi_configuration.sta.password) - 1);

    esp_wifi_set_config(WIFI_IF_STA, &wifi_configuration);
    esp_wifi_start();

    while (true) {
        xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
        printf("wifi_event_group: %x\n", wifi_event_group);
        printf("WIFI_CONNECTED_BIT: %x\n", WIFI_CONNECTED_BIT);
        if (xEventGroupGetBits(wifi_event_group) & WIFI_CONNECTED_BIT) {
            printf("WiFi CONNECTED SUCCESSFULLY.\n");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            xTaskCreate(client_post_auth_login, "client_post_auth_login", 8192, NULL, 1, NULL);
            xTaskCreate(ntp_time_sync_task, "ntp_time_sync_task", 2048, NULL, 1, NULL);
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            break;
        }
        printf("Retrying WiFi connection...\n");
        vTaskDelay(5000 / portTICK_PERIOD_MS);  
    }
}

void WiFiManager::printIpInfoTask(void *pvParameter) {
    WiFiManager* manager = static_cast<WiFiManager*>(pvParameter);
    if (xEventGroupGetBits(manager->wifi_event_group) & WIFI_CONNECTED_BIT) {
        vTaskDelay(1200 / portTICK_PERIOD_MS);
        esp_netif_ip_info_t ip_info;
        esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info);

        printf("WiFi got IP: " IPSTR "\n", IP2STR(&ip_info.ip));
        printf("Subnet Mask: " IPSTR "\n", IP2STR(&ip_info.netmask));
        printf("Gateway IP: " IPSTR "\n", IP2STR(&ip_info.gw));
    }

    vTaskDelete(NULL); 
}

void WiFiManager::printMacAddressTask(void *pvParameter) {
    WiFiManager* manager = static_cast<WiFiManager*>(pvParameter);
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    vTaskDelete(NULL);
}

void WiFiManager::wifiEventHandler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {   
    WiFiManager* manager = static_cast<WiFiManager*>(event_handler_arg);
    switch (event_id) {
    case WIFI_EVENT_STA_START:
        printf("wifievent_group: %x\n", manager->wifi_event_group);
        printf("WIFI_CONNECTED_BIT: %x\n", WIFI_CONNECTED_BIT);
        xEventGroupClearBits(manager->wifi_event_group, WIFI_CONNECTED_BIT);
        printf("wifievent_group: %x\n", manager->wifi_event_group);
        printf("WIFI_CONNECTED_BIT: %x\n", WIFI_CONNECTED_BIT);
        printf("WiFi CONNECTING.\n");
        esp_wifi_connect();
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi CONNECTED.\n");
        printf("wifievent_group: %x\n", manager->wifi_event_group);
        printf("WIFI_CONNECTED_BIT: %x\n", WIFI_CONNECTED_BIT);

        xEventGroupSetBits(manager->wifi_event_group, WIFI_CONNECTED_BIT);
        printf("wifievent_group: %x\n", manager->wifi_event_group);
        printf("WIFI_CONNECTED_BIT: %x\n", WIFI_CONNECTED_BIT);
        xTaskCreate(WiFiManager::printMacAddressTask, "print_mac_address_task", 2048, manager, 2, NULL);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi DISCONNECTED...\n");
        printf("wifievent_group: %x\n", manager->wifi_event_group);
        printf("WIFI_CONNECTED_BIT: %x\n", WIFI_CONNECTED_BIT);
        xEventGroupClearBits(manager->wifi_event_group, WIFI_CONNECTED_BIT);
         printf("wifievent_group: %x\n", manager->wifi_event_group);
        printf("WIFI_CONNECTED_BIT: %x\n", WIFI_CONNECTED_BIT);
        esp_wifi_connect(); 
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("WiFi got IP...\n");
        xTaskCreate(WiFiManager::printIpInfoTask, "print_ip_info_task", 2048, manager, 1, NULL);
        break;
    default:
        break;
    }
}
