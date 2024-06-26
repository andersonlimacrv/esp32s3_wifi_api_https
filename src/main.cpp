#include "include.h"

const EndpointPaths paths = {
    .post_login = "/api/auth/login",
    .post_ambiente = "/api/leitura-ambiente",
    .post_condensador = "/api/leitura-condensador",
    .post_bomba_condensador = "/api/leitura-bomba",
    .post_ventilador_condensador = "/api/leitura-ventilador",
};

struct send_payload {
    String payload;
    int pathCode;
};


void setup() {
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_DEBUG);

    nvs_flash_init();

    wifi_event_group = xEventGroupCreate();
    wifi_connection();  // Função que executa uma task que não é liberada até a conexão com a rede estiver estabelecida
    xTaskCreate(&ntp_time_sync_task, "ntp_time_sync_task", 2048, NULL, 2, NULL);
    xTaskCreate(&print_ip_info_task, "print_ip_info_task", 2048, NULL, 5, NULL);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI("SETUP ->", "Start client:");
    xTaskCreate(client_post_auth_login, "client_post_auth_login", 8192, NULL, 1, NULL);
    vTaskDelay(10000 / portTICK_PERIOD_MS);

    // create a task to send mock data
    xTaskCreate(post_condensador_mock_task, "post_condensador_mock_task", 8192, NULL, 1, NULL);
}

void loop() {
    // não deverá conter nada no loop;
}
