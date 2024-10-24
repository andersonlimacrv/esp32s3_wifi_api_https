#include "include.h"

QueueHandle_t postQueueAmbientes;
QueueHandle_t postQueueVentiladores;
QueueHandle_t postQueueBombas;
QueueHandle_t postQueueCompressores;

void createQueue(QueueHandle_t *queue, const char *name, UBaseType_t length, UBaseType_t itemSize) {
    *queue = xQueueCreate(length, itemSize);
    if (*queue == NULL) {
        Serial.printf("[MAIN] Failed to create queue. - %s\n", name);
    } else {
        Serial.printf("[MAIN] Successfully created queue. - %s\n", name);
    }
}

void setup() {
    initializeEquipmentIds();
    initialize_auth_event_group();
    initialize_post_mutex();
    esp_log_level_set("*", ESP_LOG_DEBUG);
    
    Serial.begin(115200);
    
    wifiManager.startConnection(CREDENTIALS_SSID,CREDENTIALS_PASSWORD, false);

    if (wifiManager.isConnected()) {
        createQueue(&postQueueAmbientes, "postQueueAmbientes", 50, sizeof(char*));
        //createQueue(&postQueueVentiladores, "postQueueVentiladores", 12, sizeof(char*));
        //createQueue(&postQueueBombas, "postQueueBombas", 12, sizeof(char*));
        //createQueue(&postQueueCompressores, "postQueueCompressores", 12, sizeof(char*));
        // Cria tarefa para processar a fila e enviar os dados se necessário (ambientes)
        //xTaskCreate(process_queue_ambientes_task, "process_queue_ambientes_task", 1096, NULL, 1, NULL);
        // Cria tarefa para processar a fila e enviar os dados se há (ventiladores)
        //xTaskCreate(process_queue_ventiladores_task, "process_queue_ventiladores_task", 1096, NULL, 1, NULL);
        // Cria tarefa para processar a fila e enviar os dados se há (bombas)
        //xTaskCreate(process_queue_bombas_task, "process_queue_bombas_task", 1096, NULL, 1, NULL);

        // Cria tarefa para processar a fila e enviar os dados se há (compressores)
        xTaskCreate(process_queue_compressores_task, "process_queue_compressores_task", 1096, NULL, 1, NULL);

        // Cria tarefa para postar dados dos compressores
        xTaskCreate(post_compressores_mock_task, "post_compressores_mock_task", 6096, NULL, 1, NULL);

        // Cria tarefa para postar dados do condensador
        //xTaskCreate(post_condensador_mock_task, "post_condensador_mock_task", 6096, NULL, 1, NULL);
        // Cria tarefa para postar dados das bombas
        //xTaskCreate(post_bombas_condensador_mock_task, "post_bombas_condensador_mock_task", 6096, NULL, 1, NULL);
        // Cria tarefa para postar dados dos ventiladores
        //xTaskCreate(post_ventiladores_condensador_mock_task, "post_ventiladores_condensador_mock_task", 6096, NULL, 1, NULL);
        // Cria tarefa para postar dados das ambientes periodicamente
        //xTaskCreate(post_ambientes_mock_task, "post_ambientes_mock_task", 6096, NULL, 1, NULL);
        // Cria tarefa para postar dados dos regimes
        //xTaskCreate(post_regimes_mock_task, "post_regimes_mock_task", 6096, NULL, 1, NULL);
        // Cria tarefa para postar dados dos regimes de condensação
        // xTaskCreate(post_regime_condensacao_mock_task, "post_regime_condensacao_mock_task", 6096, NULL, 1, NULL);
        // Cria tarefa para postar dados das energias
        // xTaskCreate(post_energia_mock_task, "post_energia_mock_task", 6096, NULL, 1, NULL);
    } else {
        Serial.println("[MAIN] Client Wi-Fi Not Connected.");
    }
}

void loop() {
    // não deverá conter nada no loop;
}
