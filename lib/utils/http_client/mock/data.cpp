#include "../../payload_formatter/payload_formatter.h"
#include "../../ntp_time_sync/ntp_time_sync.h"
#include "../http_client.h"
#include <random>
#include <cstring>
#include "../../../../src/Credentials.h" 

// Exemple of a POST request for the condensador
void post_condensador_mock_task(void* pvParameters) {
    char time_buffer[64];
    get_time_now(time_buffer, sizeof(time_buffer));
    bool ligado = true;

    std::hash<std::string> hasher;
    size_t seed = hasher(time_buffer);

    std::mt19937 gen(seed); 

    std::uniform_real_distribution<float> float_tempIn_dist(15.0, 20.0); 
    std::uniform_real_distribution<float> float_tempOut_dist(21.0, 30.0);
    std::uniform_real_distribution<float> float_umidIn_dist(50, 60);
    std::uniform_real_distribution<float> float_umidOut_dist(80, 90);
    std::uniform_real_distribution<float> velocidadeArEntrada_dist(5.6, 11.15);
    std::uniform_real_distribution<float> corrente_dist(0.00, 1.15); 
    std::uniform_int_distribution<int> frequencia_dist(55, 60); 

    float temperaturaEntrada = float_tempIn_dist(gen);
    float temperaturaSaida = float_tempOut_dist(gen);
    float umidadeRelativaEntrada = float_umidIn_dist(gen);
    float umidadeRelativaSaida = float_umidOut_dist(gen);
    float velocidadeArEntrada = velocidadeArEntrada_dist(gen);
    float corrente = corrente_dist(gen);
    int frequencia = frequencia_dist(gen);

    std::string payload = format_payload_condensador(
        ligado,
        time_buffer,
        temperaturaEntrada, 
        umidadeRelativaEntrada, 
        temperaturaSaida, 
        umidadeRelativaSaida, 
        velocidadeArEntrada, 
        corrente, 
        frequencia, 
        equipment.condensadorId);

    client_post_function(payload.c_str(), paths.post_condensador);
    vTaskDelete(NULL);
}

// Exemple of how to create a periodic task for Post the condensador data
void post_condensador_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {
        xTaskCreate(post_condensador_mock_task, "post_condensador_mock_task", 13000, NULL, 1, NULL);
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}

// Exemple of a POST request for the bomba
void post_bomba_condensador_mock_task(void* pvParameters) {
    int bombaId = *(int*)pvParameters; // Cast the pvParameters to int* and dereference to get the bombaId

    char time_buffer[64];
    get_time_now(time_buffer, sizeof(time_buffer));
    bool ligado = true;

    std::hash<std::string> hasher;
    size_t seed = hasher(time_buffer);

    std::mt19937 gen(seed);

    std::uniform_real_distribution<float> corrente_dist(0.00, 1.15);
    std::uniform_int_distribution<int> frequencia_dist(55, 60);

    float corrente = corrente_dist(gen);
    int frequencia = frequencia_dist(gen);

    std::string payload = format_payload_bomba_condensador(
        ligado,
        time_buffer,
        corrente,
        frequencia,
        bombaId); 
    client_post_function(payload.c_str(), paths.post_bomba_condensador);
     vTaskDelete(NULL);
}

// Exemple of how to create a periodic task for Post the bomba data
void post_bombas_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {
        for (int bombaId : equipment.bombaIds) {
            int* bombaIdPtr = new int(bombaId); // Allocate memory to store bombaId
            xTaskCreate(post_bomba_condensador_mock_task, "post_bomba_condensador_mock_task", 13000, (void*)bombaIdPtr, 1, NULL);
        }
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}

// Exemple of a POST request for the ventilador
void post_ventilador_condensador_mock_task(void* pvParameters) {
   int ventiladorId = *(int*)pvParameters; // Cast the pvParameters to int* and dereference to get the ventiladorId

    char time_buffer[64];
    get_time_now(time_buffer, sizeof(time_buffer));
    bool ligado = true;

    std::hash<std::string> hasher;
    size_t seed = hasher(time_buffer);

    std::mt19937 gen(seed);

    std::uniform_real_distribution<float> corrente_dist(0.00, 1.15);
    std::uniform_int_distribution<int> frequencia_dist(55, 60);

    float corrente = corrente_dist(gen);
    int frequencia = frequencia_dist(gen);

    std::string payload = format_payload_ventilador_condensador(
        ligado,
        time_buffer,
        corrente,
        frequencia,
        ventiladorId); 

    client_post_function(payload.c_str(), paths.post_ventilador_condensador);
    vTaskDelete(NULL);
}


// Exemple of how to create a periodic task for Post the ventilador data
void post_ventiladores_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {
        for (int ventiladorId : equipment.ventiladorIds) {
            int* ventiladorIdPtr = new int(ventiladorId); // Allocate memory to store ventiladorId
            xTaskCreate(post_ventilador_condensador_mock_task, "post_ventilador_condensador_mock_task", 13000, (void*)ventiladorIdPtr, 1, NULL);
        }
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}

