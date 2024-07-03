#include "data.h"

// Exemple of how to create a periodic task for Post the condensador data
void post_condensador_mock_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {

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

        char* payload_copy = new char[payload.size() + 1];
            std::strcpy(payload_copy, payload.c_str());

        xTaskCreate(post_condensador_task, "post_condensador_task", 10000, (void*)payload_copy, 1, NULL);
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}


// Exemple of how to create a periodic task for Post the bomba data
void post_bombas_condensador_mock_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {
        for (int bombaId : equipment.bombaIds) {
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


            char* payload_copy = new char[payload.size() + 1];
            std::strcpy(payload_copy, payload.c_str());

            xTaskCreate(post_bomba_task, "post_bomba_task", 8192, (void*)payload_copy, 1, NULL);
        }
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}


// Exemple of how to create a periodic task for Post the ventilador data
void post_ventiladores_condensador_mock_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {
        for (int ventiladorId : equipment.ventiladorIds) {

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
            
            char* payload_copy = new char[payload.size() + 1];
            std::strcpy(payload_copy, payload.c_str());

            xTaskCreate(post_ventilador_task, "post_ventilador_task", 8192, (void*)payload_copy, 1, NULL);
        }
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}


void post_ambientes_mock_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {
        for (int ambienteId : equipment.ambienteIds) {
        char time_buffer[64];
        get_time_now(time_buffer, sizeof(time_buffer));

        std::hash<std::string> hasher;
        size_t seed = hasher(time_buffer);

        std::mt19937 gen(seed);
        std::uniform_real_distribution<float> temperaturaAtual_dist(20.00, 30.00);

        float temperaturaAtual = temperaturaAtual_dist(gen);

        std::string payload = format_payload_ambiente(
            time_buffer,
            temperaturaAtual,
            ambienteId);
        char* payload_copy = new char[payload.size() + 1];
        std::strcpy(payload_copy, payload.c_str());
        xTaskCreate(post_ambiente_task, "post_ambiente_task", 8192, (void*)payload_copy, 1, NULL);
        }
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}

void post_compressores_mock_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {
        for (int compressaoId : equipment.compressorIds) {
        char time_buffer[64];
        get_time_now(time_buffer, sizeof(time_buffer));

        std::hash<std::string> hasher;
        size_t seed = hasher(time_buffer);

        std::mt19937 gen(seed);
        std::uniform_real_distribution<float> pressaoSuccao_dist(0.00, 1.15);
        std::uniform_real_distribution<float> pressaoDescarga_dist(0.00, 1.15);
        std::uniform_real_distribution<float> pressaoOleoReservatorio_dist(0.00, 1.15);
        std::uniform_real_distribution<float> pressaoOleoAposFiltro_dist(0.00, 1.15);
        std::uniform_real_distribution<float> temperaturaSuccao_dist(0.00, 1.15);
        std::uniform_real_distribution<float> temperaturaDescarga_dist(0.00, 1.15);
        std::uniform_real_distribution<float> temperaturaOleo_dist(0.00, 1.15);
        std::uniform_real_distribution<float> corrente_dist(0.00, 1.15);
        std::uniform_int_distribution<int> frequencia_dist(55, 60);
        std::uniform_int_distribution<int> horas_dist(100, 200);
        std::uniform_int_distribution<int> slide_dist(0, 100);

        float pressaoSuccao = pressaoSuccao_dist(gen);
        float pressaoDescarga = pressaoDescarga_dist(gen);
        float pressaoOleoReservatorio = pressaoOleoReservatorio_dist(gen);
        float pressaoOleoAposFiltro = pressaoOleoAposFiltro_dist(gen);
        float temperaturaSuccao = temperaturaSuccao_dist(gen);
        float temperaturaDescarga = temperaturaDescarga_dist(gen);
        float temperaturaOleo = temperaturaOleo_dist(gen);
        float corrente = corrente_dist(gen);
        int frequencia = frequencia_dist(gen);
        int horas = horas_dist(gen);
        int slide = slide_dist(gen);

        bool ligado = true;
        std::string payload = format_payload_compressor(
            ligado,
            time_buffer,
            pressaoSuccao,
            pressaoDescarga,
            pressaoOleoReservatorio,
            pressaoOleoAposFiltro,
            temperaturaSuccao,
            temperaturaDescarga,
            temperaturaOleo,
            corrente,
            horas,
            frequencia,
            slide,
            compressaoId);
        
        char* payload_copy = new char[payload.size() + 1];
        std::strcpy(payload_copy, payload.c_str());

            xTaskCreate(post_compressor_task, "post_compressor_task", 8192, (void*)payload_copy, 1, NULL);
        }
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}


void post_regimes_mock_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {
        for (int regimeId : equipment.regimeIds) {
        char time_buffer[64];
        get_time_now(time_buffer, sizeof(time_buffer));

        std::hash<std::string> hasher;
        size_t seed = hasher(time_buffer);

        std::mt19937 gen(seed);
        std::uniform_real_distribution<float> pressaoAtual_dist(0.00, 1.15);

        float pressaoAtual = pressaoAtual_dist(gen);

        std::string payload = format_payload_regime(
            time_buffer,
            pressaoAtual,
            regimeId);
        char* payload_copy = new char[payload.size() + 1];
        std::strcpy(payload_copy, payload.c_str());
        xTaskCreate(post_regime_task, "post_regime_task", 8192, (void*)payload_copy, 1, NULL);
        }
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}

void post_regime_condensacao_mock_task(void* pvParameters) {
    TickType_t last_wake_time_post = xTaskGetTickCount();
    const TickType_t interval_post = pdMS_TO_TICKS(60000); // Post each 60 seconds

    for (;;) {
        char time_buffer[64];
        get_time_now(time_buffer, sizeof(time_buffer));

        std::hash<std::string> hasher;
        size_t seed = hasher(time_buffer);

        std::mt19937 gen(seed);
        std::uniform_real_distribution<float> pressaoAtual_dist(0.00, 1.15);

        float pressaoAtual = pressaoAtual_dist(gen);

        std::string payload = format_payload_regime(
            time_buffer,
            pressaoAtual,
            equipment.condensacaoId);
        char* payload_copy = new char[payload.size() + 1];
        std::strcpy(payload_copy, payload.c_str());
        xTaskCreate(post_regime_condensacao_task, "post_regime_condensacao_task", 8192, (void*)payload_copy, 1, NULL);
        
        vTaskDelayUntil(&last_wake_time_post, interval_post);
    }
}
        

