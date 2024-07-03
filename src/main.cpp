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
    initializeEquipmentIds();
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_DEBUG);
    nvs_flash_init();

    wifi_event_group = xEventGroupCreate();
    start_connection();  // Função que conecta wifi, sincroniza npm e loga na api, caso não 

    // Criar tarefa para postar dados do condensador a cada 60 segundos
    xTaskCreate(post_condensador_task, "post_condensador_task", 4096, NULL, 1, NULL);
    // Criar tarefa para postar dados das bombas periodicamente
    xTaskCreate(post_bombas_task, "post_bombas_task", 4096, NULL, 1, NULL);
    // Criar tarefa para postar dados dos ventiladores periodicamente
    xTaskCreate(post_ventiladores_task, "post_ventiladores_task", 4096, NULL, 1, NULL);

    

}

void loop() {
    // não deverá conter nada no loop;
}
