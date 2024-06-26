#include "http_client.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../payload_formatter/payload_formatter.h"
#include "../ntp_time_sync/ntp_time_sync.h"
#include "../../../src/Credentials.h"
#include "cJSON.h"


static const char *backend_url = CREDENTIALS_BACKEND_URL;
extern const uint8_t ClientCert_pem_start[] asm("_binary_lib_utils_http_client_certs_ClientCert_pem_start");
extern const uint8_t ClientCert_pem_end[] asm("_binary_lib_utils_http_client_certs_ClientCert_pem_end"); 
char header[512] = "bearer ";
char bearerToken[512] = "";
char time_buffer[30];


void get_response_token(const char *response) {
    cJSON *root = cJSON_Parse(response);
    if (root == NULL) {
        printf("Response was not valid JSON.\n");
        return;
    }
    cJSON *token_json = cJSON_GetObjectItem(root, "token");
    if (token_json == NULL || !cJSON_IsString(token_json)) {
        printf("Response does not contain token.\n");
        cJSON_Delete(root);
        return;
    }
    strncpy(bearerToken, token_json->valuestring, sizeof(bearerToken) - 1);
    bearerToken[sizeof(bearerToken) - 1] = '\0'; 
    printf("Token: %s\n", bearerToken);
    cJSON_Delete(root);
}

esp_err_t client_event_auth_handler(esp_http_client_event_handle_t evt) {
    char response_buffer[4096]; 
    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            snprintf(response_buffer, sizeof(response_buffer), "%.*s", evt->data_len, (char *)evt->data);
            get_response_token(response_buffer);
            break;

        default:
            break;
    }
    return ESP_OK;
}

esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt) {
    char response_buffer[2048]; 
    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            snprintf(response_buffer, sizeof(response_buffer), "%.*s", evt->data_len, (char *)evt->data);
            printf("RESPONSE: %s\n", response_buffer);
            break;

        default:
            break;
    }
    return ESP_OK;
}


void client_post_auth_login(void *param) {
    while (1) {
        const char *path = paths.post_login;
        char url[strlen(backend_url) + strlen(path) + 1]; 
        strcpy(url, backend_url); 
        strcat(url, path);
        printf("SET ENDPOINT: %s \n", url);
        esp_http_client_config_t config_post = {};
        config_post.url = url;
        config_post.method = HTTP_METHOD_POST;
        config_post.timeout_ms = 9000;
        config_post.buffer_size_tx = 4096;
        config_post.buffer_size = 4096;


        if (strncmp(url, "https", 5) == 0) {
            printf("Using SSL\n");  
            config_post.cert_pem = (const char *)ClientCert_pem_start;
        } else {
            printf("Not using SSL\n");
            config_post.cert_pem = NULL;
        }
        config_post.event_handler = client_event_auth_handler;
        esp_http_client_handle_t client = esp_http_client_init(&config_post);

        std::string payload = format_payload_login(CREDENTIALS_LOGIN_USERNAME, CREDENTIALS_LOGIN_PASSWORD);
        esp_http_client_set_post_field(client, payload.c_str(), payload.length());
        esp_http_client_set_header(client, "Content-Type", "application/json");
        esp_http_client_perform(client);
        esp_http_client_cleanup(client);
        
        vTaskDelay(86400000 / portTICK_PERIOD_MS); // 24h em milisegundos
    }    
}

void client_post_generic_function(const char* payload, const char* post_path) {
    char url[strlen(backend_url) + strlen(post_path) + 1];
    strcpy(url, backend_url);
    strcat(url, post_path);
    printf("SENDING DATA TO ENDPOINT: %s \n", url);

    // Configuração do cliente HTTP
    esp_http_client_config_t config_post = {};
    config_post.url = url;
    config_post.method = HTTP_METHOD_POST;
    config_post.timeout_ms = 9000;
    config_post.buffer_size_tx = 2048;
    config_post.buffer_size = 4096;

    if (strncmp(url, "https", 5) == 0) {
        printf("Using SSL\n");  
        config_post.cert_pem = (const char *)ClientCert_pem_start;
    } else {
        printf("Not using SSL\n");
        config_post.cert_pem = NULL;
    }
    config_post.event_handler = client_event_post_handler;
    esp_http_client_handle_t client = esp_http_client_init(&config_post);

    char header[512];
    memset(header, 0, sizeof(header));
    size_t headerSize = sizeof(header);
    strlcat(header, "bearer ", headerSize);
    strlcat(header, bearerToken, headerSize);
    printf("Authorization -> %s \n", header);

    printf("PAYLOAD TO SEND:\n %s \n", payload);

    esp_http_client_set_post_field(client, payload, strlen(payload));
    esp_http_client_set_header(client, "Authorization", header);
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}
