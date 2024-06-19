#include "includes.h"

#define USE_LOCAL_BACKEND
char time_buffer[30];
char header[512] = "bearer ";
static const char *SETUP = "SETUP RUNNING";
extern const uint8_t ClientCert_pem_start[] asm("_binary_src_certs_ClientCert_pem_start");
extern const uint8_t ClientCert_pem_end[] asm("_binary_src_certs_ClientCert_pem_end"); 
static const char *bearerToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJJZCI6ImI3NjBiYmUyLTgzYmUtNGNkNS1iYmFkLTRiM2JmNDBhYWUzMCIsInVuaXF1ZV9uYW1lIjoiSEFSRFdBUkUiLCJmYW1pbHlfbmFtZSI6IjEiLCJzdWIiOiJIQVJEV0FSRSIsInJvbGUiOiJBRE1JTiIsImp0aSI6IjFiMGZlM2QwLTFmODAtNDkyMS05YWE3LWE3MmM4MTBmMDRmYiIsImlhdCI6MTcxODc4NDgzMywiR3J1cG9JZCI6IjEiLCJFbXByZXNhSWQiOiIiLCJVbmlkYWRlSWQiOiIiLCJuYmYiOjE3MTg3ODQ4MzMsImV4cCI6MTcxODg3MTIzMn0.eInYv-xriVNvAKd_-XaP2wTVRC-mdtc1h1v5VWcQlFM";
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
        xTaskCreate(&print_mac_address_task, "print_mac_address_task", 2048, NULL, 5, NULL);  
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi LOST CONNECTION [!] \n");
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);  
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("WiFi getting IP...\n");
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

void print_ip_info_task(void *pvParameter)
{
    while (1)
    {
        xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
        esp_netif_ip_info_t ip_info;
        esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info);

        printf("WiFi got IP: " IPSTR "\n", IP2STR(&ip_info.ip));
        printf("Subnet Mask: " IPSTR "\n", IP2STR(&ip_info.netmask));
        printf("Gateway IP: " IPSTR "\n", IP2STR(&ip_info.gw));
        
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void get_time_now(char *time_buffer, size_t buffer_size) {
    time_t now;
    time(&now);
    struct tm *timeinfo = localtime(&now); 
    strftime(time_buffer, buffer_size, "%Y-%m-%dT%H:%M:%S.000Z", timeinfo);
}

// Função chamada quando a hora é sincronizada
void handler_time_sync_cb(struct timeval *tv)
{ 
    printf("Initializing handler for time sync notification\n");
    time_t now;
    time(&now);
    struct tm *timeinfo = localtime(&now); 
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%H:%M:%S.000Z", timeinfo);
    printf("Time Synced: %s\n", time_buffer);
}

// Tarefa que inicializa o SNTP e aguarda a sincronização do tempo
void ntp_time_sync_task(void *pvParameter)
{
    printf("Initializing SNTP\n");
    setenv("TZ", "UTC", 1);
    tzset();

    esp_sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_set_time_sync_notification_cb(&handler_time_sync_cb);
    esp_sntp_init();

    vTaskDelete(NULL);
}

esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt) // Handdler que recebe e imprime a resposta do servidor
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

static void client_post_rest_function(const char *path) // Função que envia o POST para o servidor
{
    char url[strlen(BACKEND_URL) + strlen(path) + 1]; 
    strcpy(url, BACKEND_URL); 
    strcat(url, path);
    printf("SET ENDPOINT: %s \n", url);
    vTaskDelay(5000 / portTICK_PERIOD_MS);

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

    char payload[512];
    int temperaturaAtual = 0; // Valor inicial da temperatura
    srand(time(NULL));        // Inicializa a semente do gerador de números randômicos

    while (1) {
        
        memset(header, 0, sizeof(header)); // Limpa o buffer header antes de adicionar um novo token
        size_t headerSize = sizeof(header);
        strlcat(header, "bearer ", headerSize); 
        strlcat(header, bearerToken, headerSize);
        printf("Authorization -> %s \n", header);

        get_time_now(time_buffer, sizeof(time_buffer)); 
        temperaturaAtual = rand() % 21 + 10; // Gerando um valor randômico para temperatura entre 10 e 30 graus

        snprintf(payload, sizeof(payload), "{\"dataHora\": \"%s\",\"temperaturaAtual\": %d,\"ambienteId\": 1}", time_buffer, temperaturaAtual);
        printf("PAYLOAD:\n %s \n", payload);

        esp_http_client_set_post_field(client, payload, strlen(payload));
        esp_http_client_set_header(client, "Authorization", header);
        esp_http_client_set_header(client, "Content-Type", "application/json");

        esp_http_client_perform(client);

        // Espera 30 segundos antes de enviar o próximo POST
        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
    esp_http_client_cleanup(client);
}


void setup() {
    nvs_flash_init();
    wifi_event_group = xEventGroupCreate();
    wifi_connection();

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    ESP_LOGI(SETUP,"WIFI was initiated ...........");
    xTaskCreate(&ntp_time_sync_task, "ntp_time_sync_task", 2048, NULL, 5, NULL);
    xTaskCreate(&print_ip_info_task, "print_ip_info_task", 2048, NULL, 5, NULL);
    
    ESP_LOGI(SETUP, "Start client:");
    client_post_rest_function(paths.post_temperatura);
}

void loop() {
    // não deverá conter nada no loop;
}
