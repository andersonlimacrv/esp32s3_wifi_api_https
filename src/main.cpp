#include <stdio.h>
#include <string.h>
#include <cstring>

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_client.h"

#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#include "Credentials.h"

static const char *TAG_WIFI = "WiFi CONNECTION";
static const char *TAG_HTTP = "HTTP CLIENT";
static const char *SETUP = "SETUP RUNNING";


// Define CLIENT certificate  by command: openssl s_client -showcerts -connect actions-fastapi.onrender.com:443 
// Error on render : E (4072) esp-tls: couldn't get hostname for :actions-fastapi.onrender.com: getaddrinfo() returns 202, addrinfo=0x0

extern const uint8_t ClientCert_pem_start[] asm("_binary_src_certs_ClientCert_pem_start");
extern const uint8_t ClientCert_pem_end[] asm("_binary_src_certs_ClientCert_pem_end"); 

// openssl req -newkey rsa:2048 -nodes -keyout PrivateKey.pem -x509 -days 3650 -out Certificate.pem -subj "/CN=ESP32S3-CESS"
extern const uint8_t Certificate_pem_start[] asm("_binary_src_certs_Certificate_pem_start");
extern const uint8_t Certificate_pem_end[] asm("_binary_src_certs_Certificate_pem_end");
extern const uint8_t PrivateKey_pem_start[] asm("_binary_src_certs_PrivateKey_pem_start");
extern const uint8_t PrivateKey_pem_end[] asm("_binary_src_certs_PrivateKey_pem_end");

typedef struct {
    const char * const users;
    const char * const products;
} EndpointPaths;

// Constantes para os paths
static const EndpointPaths paths = {
    .users = "/users",
    .products = "/products",
};

// WiFi
static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        ESP_LOGI(TAG_WIFI, "WiFi connecting ...");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG_WIFI,"WiFi connected ...");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG_WIFI,"WiFi lost connection ...");
        break;
    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG_WIFI,"WiFi got IP ...");
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
    esp_wifi_init(&wifi_initiation); // 					                   

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    wifi_config_t wifi_configuration = {};
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.ssid), CREDENTIALS_SSID, sizeof(wifi_configuration.sta.ssid) - 1);
    strncpy(reinterpret_cast<char*>(wifi_configuration.sta.password), CREDENTIALS_PASSWORD, sizeof(wifi_configuration.sta.password) - 1);

    esp_wifi_set_config(WIFI_IF_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_connect();

}

// CLIENT
esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG_HTTP, "HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
        break;

    default:
        break;
    }
    return ESP_OK;
}

static void client_post_rest_function(const char *path)
{   
    char url[strlen(BACKEND_URL) + strlen(path) + 1]; 
    strcpy(url, BACKEND_URL); 
    strcat(url, path);
    ESP_LOGI(SETUP, "url: %s", url);
    esp_http_client_config_t config_post = {};
    config_post.url = url;
    config_post.method = HTTP_METHOD_POST;
    config_post.cert_pem =  (const char *)Certificate_pem_start;
    config_post.event_handler = client_event_post_handler;
        
    esp_http_client_handle_t client = esp_http_client_init(&config_post);

    const char *post_data = "test ...";
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

void setup() {
    nvs_flash_init();
    wifi_connection();

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    ESP_LOGI(SETUP,"WIFI was initiated ...........");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    ESP_LOGI(SETUP, "Start client:");
    client_post_rest_function(paths.users);
}

void loop() {}
