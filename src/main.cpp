#include "includes.h"


#define USE_LOCAL_BACKEND

static const char *SETUP = "SETUP RUNNING";

extern const uint8_t ClientCert_pem_start[] asm("_binary_src_certs_ClientCert_pem_start");
extern const uint8_t ClientCert_pem_end[] asm("_binary_src_certs_ClientCert_pem_end"); 


static const char * bearerToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJJZCI6ImMxNmI4NzgzLTcyZDItNDNhYy1hODM3LTRlYmQxOGM2NGJmMCIsInVuaXF1ZV9uYW1lIjoiU3VwZXIiLCJmYW1pbHlfbmFtZSI6IlVzZXIiLCJzdWIiOiJTR0VNX0lOSVRJQUxfU1VQRVJfVVNFUiIsInJvbGUiOiJTVVBFUl9VU0VSIiwianRpIjoiNmI1YTZmYjAtZGE0OC00YTk4LWI2NzAtOTJmMTQ4YTZlM2U5IiwiaWF0IjoxNzE4NjMzNzcxLCJHcnVwb0lkIjoiIiwiRW1wcmVzYUlkIjoiIiwiVW5pZGFkZUlkIjoiIiwibmJmIjoxNzE4NjMzNzcxLCJleHAiOjE3MTg3MjAxNzB9.INUOlFT01MyizmeFh893J-Fh4_RomxEdTzbZ20cYGcs";
char header[512] = "bearer ";

typedef struct {
    const char * const login;
    const char * const post_temperatura;
} EndpointPaths;

static const EndpointPaths paths = {
    .login = "/api/auth/login",
    .post_temperatura = "/api/leitura-ambiente",
};

static EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

void print_ip_info_task(void *pvParameter)
{
    while (1)
    {
        xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY); // Espera até que o bit de conexão WiFi esteja setado
        esp_netif_ip_info_t ip_info;
        esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info);

        printf("WiFi got IP: " IPSTR "\n", IP2STR(&ip_info.ip));
        printf("Subnet Mask: " IPSTR "\n", IP2STR(&ip_info.netmask));
        printf("Gateway IP: " IPSTR "\n", IP2STR(&ip_info.gw));
        
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);// Remove o bit de conexão WiFi para não imprimir novamente
    }
}

void print_mac_address_task(void *pvParameter)
{
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    vTaskDelete(NULL);
}

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("WiFi CONECTING ... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi CONNECTED. \n");
        // Cria a task para imprimir o endereço MAC
        xTaskCreate(&print_mac_address_task, "print_mac_address_task", 2048, NULL, 5, NULL);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi LOST CONNECTION [!] \n");
        // Remove o bit de conexão WiFi para que a task de IP não imprima enquanto desconectado
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("WiFi getting IP...\n");
        // Seta o bit de conexão WiFi
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
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

esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        printf("RESPONSE: %.*s\n", evt->data_len, (char *)evt->data);
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
    printf("SET ENDPOINT: %s \n", url);

    esp_http_client_config_t config_post = {};
    config_post.url = url;
    config_post.method = HTTP_METHOD_POST;
    config_post.timeout_ms = 9000;
    config_post.buffer_size_tx = 2048;

    if (strncmp(url, "https", 5) == 0) {
        printf("Using SSL\n");  
        config_post.cert_pem = (const char *)ClientCert_pem_start;
    } else {
        printf("Not using SSL\n");
        config_post.cert_pem = NULL;
    }
    config_post.event_handler = client_event_post_handler;
    esp_http_client_handle_t client = esp_http_client_init(&config_post);

    const char *post_data = "{\n\"temperaturaAtual\":12,\n\"ambienteId\":1\n}";
    printf("PAYLOAD:\n %s \n", post_data);


    // Authentication: Bearer    
    size_t headerSize = sizeof(header);
    strlcat(header, bearerToken, headerSize);
    printf("Authorization -> %s \n", header);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Authorization", header);
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}



void setup() {
    nvs_flash_init();
    wifi_event_group = xEventGroupCreate();
    wifi_connection();

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    ESP_LOGI(SETUP,"WIFI was initiated ...........");

    xTaskCreate(&print_ip_info_task, "print_ip_info_task", 2048, NULL, 5, NULL);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    ESP_LOGI(SETUP, "Start client:");
    client_post_rest_function(paths.post_temperatura);
    
}

void loop() {}
