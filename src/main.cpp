#include "include.h"


void setup() {
    initializeEquipmentIds();
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_DEBUG);
    nvs_flash_init();

    wifi_event_group = xEventGroupCreate();
    start_connection();  // Função que conecta wifi, sincroniza npm e loga na api, caso não 

    // Criar tarefa para postar dados do condensador a cada 60 segundos
    xTaskCreate(post_condensador_mock_task, "post_condensador_mock_task", 8000, NULL, 1, NULL);
    // Criar tarefa para postar dados das bombas periodicamente
    xTaskCreate(post_bombas_condensador_mock_task, "post_bombas_condensador_mock_task", 8000, NULL, 1, NULL);
    // Criar tarefa para postar dados dos ventiladores periodicamente
    xTaskCreate(post_ventiladores_condensador_mock_task, "post_ventiladores_condensador_mock_task", 8000, NULL, 1, NULL);
    // Criar tarefa para postar dados das ambientes periodicamente
    xTaskCreate(post_ambientes_mock_task, "post_ambientes_mock_task", 8000, NULL, 1, NULL);

    // Criar tarefa para postar dados dos compressores a cada 60 segundos
    xTaskCreate(post_compressores_mock_task, "post_compressores_mock_task", 8000, NULL, 1, NULL);

    // Criar tarefa para postar dados dos regimes a cada 60 segundos
    xTaskCreate(post_regimes_mock_task, "post_regimes_mock_task", 8000, NULL, 1, NULL);

    // Criar tarefa para postar dados do regime de condensação a cada 60 segundos
    xTaskCreate(post_regime_condensacao_mock_task, "post_regime_condensacao_mock_task", 8000, NULL, 1, NULL);
}

void loop() {
    // não deverá conter nada no loop;
}
