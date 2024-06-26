#include "../../payload_formatter/payload_formatter.h"
#include "../../ntp_time_sync/ntp_time_sync.h"
#include "../http_client.h"

/* Exemple post on condensador Endpoint */

void post_condensador_mock_task(void* pvParameters) {
    char time_buffer[64];
    get_time_now(time_buffer, sizeof(time_buffer));
    int condensadorId = 1;
    bool ligado = true;
    float temperaturaEntrada = 25.0;
    float umidadeRelativaEntrada = 70.0;
    float temperaturaSaida = 30.0;
    float umidadeRelativaSaida = 89.0;
    float velocidadeArEntrada = 22.0;
    float corrente = 0.098712398;
    int frequencia = 60;

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

    client_post_generic_function(payload.c_str(), paths.post_condensador);
    vTaskDelete(NULL);
}