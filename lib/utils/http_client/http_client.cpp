#include "http_client.h"


// Definindo o grupo de eventos e o bit de autenticação
static EventGroupHandle_t auth_event_group;
// Definindo o mutex (semaforo) - para fazer 1 post por vez
static SemaphoreHandle_t xMutex;
#define AUTHENTICATED_BIT BIT1

static const char *backend_url = CREDENTIALS_BACKEND_URL;
extern const uint8_t ClientCert_pem_start[] asm("_binary_lib_utils_http_client_certs_ClientCert_pem_start");
extern const uint8_t ClientCert_pem_end[] asm("_binary_lib_utils_http_client_certs_ClientCert_pem_end"); 
char header[512] = "bearer ";
char bearerToken[512] = "";
char time_buffer[30];
char last_payload[4096];
char last_post_path[256];
bool retry_post = false;

void initialize_auth_event_group() {
    auth_event_group = xEventGroupCreate();
    if (auth_event_group == NULL) {
        printf("[HTTP_CLIENT] Failed to create event group.\n");
    }
}

void initialize_post_mutex() {
    xMutex = xSemaphoreCreateMutex();
    if (xMutex == NULL) {
        printf("Mutex creation failed!\n");
        // Handle error here
    }
}

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
    xEventGroupSetBits(auth_event_group, AUTHENTICATED_BIT);
    printf("NEW TOKEN: %s\n", bearerToken);
    cJSON_Delete(root);

    if (retry_post) {
        retry_post = false;
        printf("Retrying last post with new token...\n");
        xTaskCreate(retry_post_task, "retry_post_task", 8192, NULL, 1, NULL);
    }
}

void retry_post_task(void *param) {
    client_post_function(last_payload, last_post_path);
    vTaskDelete(NULL);
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
    int http_status = 0;

    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            snprintf(response_buffer, sizeof(response_buffer), "%.*s", evt->data_len, (char *)evt->data);
            printf("RESPONSE: %s\n", response_buffer);
            break;

        case HTTP_EVENT_DISCONNECTED:
            http_status = esp_http_client_get_status_code(evt->client);
            if (http_status == 401) {
                printf("HTTP STATUS CODE: [%d] - UNAUTHORIZED\n", http_status);
                retry_post = true;
                xTaskCreate(client_post_auth_login, "client_post_auth_login", 8192, NULL, 1, NULL);
            } else if (http_status == 200 || http_status == 201) {
                printf("HTTP STATUS CODE: [%d] - OK\n", http_status);
            } else {
                printf("HTTP STATUS CODE: [%d] - FAILED\n", http_status);
            }
            break;

        default:
            break;
    }

    return ESP_OK;
}

void client_post_auth_login(void *param) {
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
        config_post.transport_type = HTTP_TRANSPORT_OVER_SSL;
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

    vTaskDelete(NULL);
}

void client_post_function(const char* payload, const char* post_path) {
    strncpy(last_payload, payload, sizeof(last_payload) - 1);
    strncpy(last_post_path, post_path, sizeof(last_post_path) - 1);

    char url[strlen(backend_url) + strlen(post_path) + 1];
    strcpy(url, backend_url);
    strcat(url, post_path);
    printf("SENDING DATA TO ENDPOINT: %s \n", url);

    // Configuração do cliente HTTP
    esp_http_client_config_t config_post = {};
    config_post.url = url;
    config_post.method = HTTP_METHOD_POST;
    config_post.timeout_ms = 9000;
    config_post.buffer_size_tx = 4096;
    config_post.buffer_size = 4096;

    if (strncmp(url, "https", 5) == 0) {
        printf("Using SSL\n");
        config_post.cert_pem = (const char *)ClientCert_pem_start;
        config_post.transport_type = HTTP_TRANSPORT_OVER_SSL;
    } else {
        printf("Not using SSL\n");
        config_post.cert_pem = NULL;
    }
    config_post.event_handler = client_event_post_handler;
    esp_http_client_handle_t client = esp_http_client_init(&config_post);
    if (client == NULL) {
        printf("Failed to initialize HTTP client\n");
        return;
    }

    char auth_header[512];
    memset(auth_header, 0, sizeof(auth_header));
    size_t headerSize = sizeof(auth_header);
    strlcat(auth_header, "bearer ", headerSize);
    strlcat(auth_header, bearerToken, headerSize);
    printf("Authorization -> %s \n", auth_header);

    printf("PAYLOAD TO SEND:\n %s \n", payload);

    esp_http_client_set_post_field(client, payload, strlen(payload));
    esp_http_client_set_header(client, "Authorization", auth_header);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

void authenticated_post_task(const char* payload, const char* postPath) {
    // Take the mutex before proceeding
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
        printf("WAITING FOR AUTHENTICATION...\n");

        const TickType_t xDelay = pdMS_TO_TICKS(10000); // Wait for 10 seconds
        EventBits_t uxBits = xEventGroupWaitBits(auth_event_group, AUTHENTICATED_BIT, pdFALSE, pdTRUE, xDelay);

        if (uxBits & AUTHENTICATED_BIT) {
            printf("AUTHENTICATION SUCCESSFUL...\n");
            client_post_function(payload, postPath);
        } else {
            printf("AUTHENTICATION FAILED, RETRYING...\n");
            xTaskCreate(client_post_auth_login, "client_post_auth_login", 8192, NULL, 1, NULL);

            const TickType_t retryDelay = pdMS_TO_TICKS(5000); // Retry after 5 seconds
            uxBits = xEventGroupWaitBits(auth_event_group, AUTHENTICATED_BIT, pdFALSE, pdTRUE, retryDelay);

            if (uxBits & AUTHENTICATED_BIT) {
                printf("RETRY AUTHENTICATION SUCCESSFUL, SENDING DATA...\n");
                client_post_function(payload, postPath);
            } else {
                printf("RETRY AUTHENTICATION FAILED, ABORTING...\n");
            }
        }

        // Release the mutex after the task is done
        xSemaphoreGive(xMutex);
    }

    vTaskDelete(NULL);
}

void post_condensador_task(void* pvParameters) {
    printf("POST CONDENSADOR TASK CREATED.\n");
    authenticated_post_task((const char*)pvParameters, paths.post_condensador);
}

void post_bomba_task(void* pvParameters) {
    printf("POST BOMBA TASK CREATED.\n");
    authenticated_post_task((const char*)pvParameters, paths.post_bomba_condensador);
}

void post_ventilador_task(void* pvParameters) {
    printf("POST VENTILADOR TASK CREATED.\n");
    authenticated_post_task((const char*)pvParameters, paths.post_ventilador_condensador);
}

void post_ambiente_task(void* pvParameters) {
    printf("POST AMBIENTE TASK CREATED.\n");
    authenticated_post_task((const char*)pvParameters, paths.post_ambiente);
}

void post_compressor_task(void* pvParameters) {
    printf("POST COMPRESSOR TASK CREATED.\n");
    authenticated_post_task((const char*)pvParameters, paths.post_compressor);
}

void post_regime_task(void* pvParameters) {
    printf("POST REGIME TASK CREATED.\n");
    authenticated_post_task((const char*)pvParameters, paths.post_regime);
}

void post_regime_condensacao_task(void* pvParameters) {
    printf("POST REGIME CONDENSACAO TASK CREATED.\n");
    authenticated_post_task((const char*)pvParameters, paths.post_regime_condensacao);
}

void post_energia_task(void* pvParameters) {
    printf("POST ENERGIA TASK CREATED.\n");
    authenticated_post_task((const char*)pvParameters, paths.post_energia);
}