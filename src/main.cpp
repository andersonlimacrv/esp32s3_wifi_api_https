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
    start_connection();  // Função que conecta wifi, sincroniza npm e loga na api, caso não 

    TickType_t interval_post = 2 * 60 * 1000 / portTICK_PERIOD_MS;  //  2 minutos 
    TickType_t last_wake_time_post = xTaskGetTickCount(); // Calcula o tempo atual

    for (;;) {
        xTaskCreate(post_condensador_mock_task, "post_condensador_mock_task", 12948, NULL, 2, NULL);
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}

void loop() {
    // não deverá conter nada no loop;
}
