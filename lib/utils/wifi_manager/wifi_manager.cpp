#include "wifi_manager.h"
/* 
1. Inicializa a conexão Wi-Fi.
2. Aguarda até que a conexão Wi-Fi seja estabelecida.
3. Obtém e imprime o endereço MAC do dispositivo uma vez.
4. Quando conectado, imprime as informações de IP uma vez.
5. Gerencia eventos de conexão, desconexão e obtenção de IP. 
*/

EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

void print_mac_address_task(void *pvParameter)
{
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    vTaskDelete(NULL);
}

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("WiFi CONNECTING.\n");
        esp_wifi_connect();
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi CONNECTED.\n");
         xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi DISCONNECTED...\n");
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        esp_wifi_connect();  // Attempt to reconnect
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("WiFi got IP...\n");
        xTaskCreate(&print_ip_info_task, "print_ip_info_task", 2048, NULL, 5, NULL);
        break;
    default:
        break;
    }
}

void wifi_connection()
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    wifi_config_t wifi_configuration = {};
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.ssid), CREDENTIALS_SSID, sizeof(wifi_configuration.sta.ssid) - 1);
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.password), CREDENTIALS_PASSWORD, sizeof(wifi_configuration.sta.password) - 1);

    esp_wifi_set_config(WIFI_IF_STA, &wifi_configuration);
    esp_wifi_start();

    while (true)
    {
        xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
        if (xEventGroupGetBits(wifi_event_group) & WIFI_CONNECTED_BIT)
        {
            printf("WiFi CONNECTED SUCCESSFULLY.\n");
            break;
        }
        printf("Retrying WiFi connection...\n");
        vTaskDelay(5000 / portTICK_PERIOD_MS);  
    }
}

void print_ip_info_task(void *pvParameter)
{
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


