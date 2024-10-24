#include "wifi_manager.h"
/* 
1. Inicializa a conexão Wi-Fi;
2. Aguarda até que a conexão Wi-Fi seja estabelecida;
3. Obtém e imprime o endereço MAC do dispositivo uma vez;
4. Quando conectado, imprime as informações de IP uma vez;
5. Gerencia eventos de conexão, desconexão e obtenção de IP;
6. Faz login na API;
7. Faz sincroniza NTP (Simple Network Time Protocol).
*/

// Definição dos bits para o EventGroup
#define WIFI_CONNECTED_BIT    (1 << 0)
#define WIFI_DISCONNECTED_BIT (1 << 1)
#define IP_GOT_BIT            (1 << 2)

EventGroupHandle_t WiFiManager::wifi_event_group = nullptr;
WiFiManager wifiManager; 

static const char* TAG = "WifiManager";
const char* WiFiManager::STATIC_IP = CREDENTIALS_WIFI_IP;
const char* WiFiManager::STATIC_NETMASK = CREDENTIALS_NETMASK;
const char* WiFiManager::STATIC_GATEWAY = CREDENTIALS_GATEWAY;
const char* WiFiManager::STATIC_DNS = CREDENTIALS_DNS;


WiFiManager::WiFiManager() {
    if (wifi_event_group == nullptr) {  
        wifi_event_group = xEventGroupCreate();
        if (wifi_event_group == nullptr) {  
            printf("[%s] WiFi event group creation failed (nullptr)\n", TAG);
        } else {
            printf("[%s] WiFi event group created successfully. \n", TAG, (void*)wifi_event_group);
        }
    } else {
        printf("[%s] WiFi event group already created: %p\n", TAG, (void*)wifi_event_group);
    }
}


void WiFiManager::startConnection(const char* ssid, const char* pass, bool useStaticIP) {
    printf("[%s] Starting Wi-Fi connection with SSID: %s\n",TAG, ssid);
    this->ssid = ssid;
    this->pass = pass;
    initializeWiFi();
    connect(useStaticIP);
}

void WiFiManager::initializeWiFi() {
    // Initialize NVS (Non-Volatile Storage)
    esp_err_t ret = nvs_flash_init();
    // Check if NVS was successfully initialized
    if (ret == ESP_OK) {
        printf("[%s] NVS Flash initialized successfully.\n", TAG);
    } 
    // If there are no free pages or a new version of NVS is found, erase and reinitialize
    else if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("[%s] NVS has no free pages or new version found, erasing and reinitializing...\n", TAG);
        nvs_flash_erase();
        ret = nvs_flash_init();  // Reinitialize after erasing

        if (ret == ESP_OK) {
            printf("[%s] NVS reinitialized successfully after erasing.\n", TAG);
        } else {
            printf("[%s] Failed to reinitialize NVS: %s\n", TAG, esp_err_to_name(ret));
        }
    } 
    else {
        printf("[%s] Failed to initialize NVS: %s\n", TAG, esp_err_to_name(ret));
    }

    esp_err_t ret_network = esp_netif_init();
    // Initialize networking stack and WiFi drivers
    if (ret_network == ESP_OK) {
        printf("[%s] Networking stack initialized successfully.\n", TAG);
    } else {
        printf("[%s] Failed to initialize networking stack: %s\n", TAG, esp_err_to_name(ret_network));
    }
    esp_err_t net_loop_create = esp_event_loop_create_default();
    if (net_loop_create == ESP_OK) {
        printf("[%s] Event loop created successfully.\n", TAG);
    } else {
        printf("[%s] Failed to create event loop: %s\n", TAG, esp_err_to_name(net_loop_create));
    }
    
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
}

void WiFiManager::setTxPower() {
    int8_t power = 74; 
    
    esp_err_t ret = esp_wifi_set_max_tx_power(power);
    if (ret == ESP_OK) {
        printf("[%s] POWER OF TRANSMISSION SET TO %d .\n", TAG, power);
    } else {
        printf("[%s] ERROR SETTING POWER: %s\n", esp_err_to_name(ret));  
    }
}
void WiFiManager::connect(bool useStaticIP) {
    // Register event handlers for Wi-Fi events and IP acquisition
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WiFiManager::wifiEventHandler, this);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &WiFiManager::wifiEventHandler, this);
    // Set Wi-Fi mode to STA
    esp_err_t mode_err = esp_wifi_set_mode(WIFI_MODE_STA); 
    if (mode_err != ESP_OK) {
        printf("[%s] Failed to set Wi-Fi mode to STA: %s\n", TAG, esp_err_to_name(mode_err));
        return;
    }
    // Configure Wi-Fi settings
    wifi_config_t wifi_configuration = {};
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.ssid), ssid, sizeof(wifi_configuration.sta.ssid) - 1);
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.password), pass, sizeof(wifi_configuration.sta.password) - 1);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_configuration);

    if (useStaticIP) {
        esp_netif_ip_info_t ip_info;
        
        ip4addr_aton(STATIC_IP, (ip4_addr_t*)&ip_info.ip);
        ip4addr_aton(STATIC_NETMASK, (ip4_addr_t*)&ip_info.netmask);
        ip4addr_aton(STATIC_GATEWAY, (ip4_addr_t*)&ip_info.gw);

        esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
        esp_netif_dhcpc_stop(netif);
        esp_err_t err = esp_netif_set_ip_info(netif, &ip_info);
        if (err == ESP_OK) {
            printf("[%s] CONFIGURED STATIC IP: " IPSTR "\n", TAG, IP2STR(&ip_info.ip));
        } else {
            printf("[%s] Failed to configure static IP: %s\n", TAG, esp_err_to_name(err));
        }
          
        esp_netif_dns_info_t dns_info;
        ip4addr_aton(STATIC_DNS, (ip4_addr_t*)&dns_info.ip.u_addr.ip4); 
        esp_netif_set_dns_info(netif, ESP_NETIF_DNS_MAIN, &dns_info);
        printf("[%s] DNS CONFIGURED: " IPSTR "\n", TAG, IP2STR(&dns_info.ip.u_addr.ip4));
            
        
    } else {
        esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
        esp_netif_dhcpc_start(netif);
        printf("[%s] CONFIGURING DHCP\n", TAG);
    }

    esp_err_t wifi_err = esp_wifi_start();

    if (wifi_err != ESP_OK) {
        printf("[%s] Failed to start Wi-Fi: %s\n", TAG, esp_err_to_name(wifi_err));
        return;
    }

    printf("[%s] Starting Wi-Fi...\n", TAG);
    setTxPower();
    // Wait for Wi-Fi connection to be established
    printf("[%s] Waiting for WIFI_CONNECTED_BIT: %x\n", TAG, WIFI_CONNECTED_BIT);
    EventBits_t uxBits = xEventGroupWaitBits(
        wifi_event_group,               // The event group to wait for
        WIFI_CONNECTED_BIT,             // The bit that should be set
        pdFALSE,                        // Do not clear the bit when it exits
        pdTRUE,                         // Wait for all bits specified (only one in this case)
        portMAX_DELAY                   // Wait indefinitely until the bit is set
    );
    printf("[%s] WIFI_CONNECTED_BIT: %x\n", TAG, uxBits);
    if (uxBits & WIFI_CONNECTED_BIT) {
        // after Wi-Fi connection is established
        printf("[%s] Connected to Wi-Fi. WIFI_CONNECTED_BIT: %x\n", TAG, uxBits);
        xTaskCreate(ntp_time_sync_task, "ntp_time_sync_task", 2048, NULL, 1, NULL);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        xTaskCreate(client_post_auth_login, "client_post_auth_login", 8192, NULL, 1, NULL);
    } else {
        printf("[%s] Failed to connect to Wi-Fi.\n", TAG);
    }

}

void WiFiManager::wifiEventHandler(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    WiFiManager* instance = static_cast<WiFiManager*>(event_handler_arg);
    wifi_event_sta_disconnected_t* disc_event = nullptr;
    switch (event_id) {
    case WIFI_EVENT_STA_START:
        printf("[%s] WiFi CONNECTING. EVENT: [%d]\n", TAG , event_id);
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        esp_wifi_connect();
        break;
    case WIFI_EVENT_STA_CONNECTED:
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        printf("[%s] WiFi CONNECTED SUCCESSFULLY. EVENT: [%d]\n", TAG, event_id);
        xTaskCreate(WiFiManager::printMacAddressTask, "print_mac_address_task", 2048, NULL, 2, NULL);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        disc_event = (wifi_event_sta_disconnected_t*)event_data;
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        printf("[%s] WiFi DISCONNECTED... EVENT: [%d]\n", TAG, event_id);

        if (disc_event) {
            if (disc_event->reason == 15) { 
                printf("[%s] Credentials are wrong! Check your SSID and PASSWORD.\n", TAG);
            } else {
                printf("[%s] Disconnected for reason: %d\n", TAG, disc_event->reason);
            }
        } else {
            printf("[%s] Disconnected with unknown reason.\n", TAG);
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
        esp_wifi_connect(); 
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("[%s] Got IP!\n", TAG);
        xTaskCreate(WiFiManager::printIpInfoTask, "print_ip_info_task", 2048, NULL, 2, NULL);
        break;
    case WIFI_EVENT_AP_PROBEREQRECVED:
        printf("[%s] PROBE REQUEST RECEIVED. EVENT: [%d]\n", TAG, event_id);
        break;
    case WIFI_EVENT_STA_AUTHMODE_CHANGE: 
        printf("[%s] AUTH MODE CHANGED. EVENT: [%d] \n", TAG, event_id);
        break;
    case WIFI_EVENT_STA_WPS_ER_SUCCESS:
        printf("[%s] WPS ER SUCCESS. EVENT: [%d] \n", TAG, event_id);
        break;
    case WIFI_EVENT_STA_WPS_ER_FAILED:
        printf("[%s] WPS ER FAILED. EVENT: [%d] \n", TAG, event_id);
        break;
    case WIFI_EVENT_STA_WPS_ER_TIMEOUT:
        printf("[%s] WPS ER TIMEOUT. EVENT: [%d] \n", TAG, event_id);
        break;
    case WIFI_EVENT_STA_WPS_ER_PIN:
        printf("[%s] WPS ER PIN. EVENT: [%d] \n", TAG, event_id);
        break;
    case WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP:
        printf("[%s] WPS ER PBC OVERLAP. EVENT: [%d] \n", TAG, event_id);
        break;
    case WIFI_EVENT_AP_START:
        printf("[%s] AP STARTED. EVENT: [%d] - NEED CONFIGURE MODE STA. RECONNECTING ... \n", TAG, event_id);
        instance->connect(true);  
        break;
    default:
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        printf("[%s] UNKNOWN EVENT: %d\n", TAG, event_id);
        instance->connect(true);  
        break;
    }
}


void WiFiManager::printIpInfoTask(void *pvParameter) {
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info);
    printf("[%s] IP Address: " IPSTR "\n", TAG, IP2STR(&ip_info.ip));
    printf("[%s] Subnet Mask: " IPSTR "\n", TAG, IP2STR(&ip_info.netmask));
    printf("[%s] Gateway: " IPSTR "\n", TAG, IP2STR(&ip_info.gw));
    vTaskDelete(NULL);
}

void WiFiManager::printMacAddressTask(void *pvParameter) {
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    printf("[%s] MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", TAG, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    vTaskDelete(NULL);
}

bool WiFiManager::isConnected() const {
    EventBits_t bits = xEventGroupGetBits(wifi_event_group);
    return (bits & WIFI_CONNECTED_BIT) != 0;
}
