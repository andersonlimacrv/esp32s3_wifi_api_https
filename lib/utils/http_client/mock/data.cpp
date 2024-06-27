#include "../../payload_formatter/payload_formatter.h"
#include "../../ntp_time_sync/ntp_time_sync.h"
#include "../http_client.h"
#include <random>
#include <cstring> 

/* Exemplo de post no endpoint do condensador */
void post_condensador_mock_task(void* pvParameters) {
    char time_buffer[64];
    get_time_now(time_buffer, sizeof(time_buffer));
    int condensadorId = 1;
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
    std::uniform_int_distribution<int> frequencia_dist(55, 64); 

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
        condensadorId);

    client_post_function(payload.c_str(), paths.post_condensador);
    vTaskDelete(NULL);
}
