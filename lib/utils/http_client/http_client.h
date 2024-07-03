#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include "esp_http_client.h"

typedef struct {
    const char * const post_login;
    const char * const post_ambiente;
    const char * const post_condensador;
    const char * const post_bomba_condensador;
    const char * const post_ventilador_condensador;
    const char * const post_compressor;
    const char * const post_regime;
    const char * const post_regime_condensacao;
} EndpointPaths;

const EndpointPaths paths = {
    .post_login = "/api/auth/login",
    .post_ambiente = "/api/leitura-ambiente",
    .post_condensador = "/api/leitura-condensador",
    .post_bomba_condensador = "/api/leitura-bomba",
    .post_ventilador_condensador = "/api/leitura-ventilador",
    .post_compressor = "/api/leitura-compressor",
    .post_regime = "/api/leitura-regime",
    .post_regime_condensacao = "/api/leitura-condensacao"
};

extern const EndpointPaths paths;

esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt);
void client_post_auth_login(void *param);
void client_post_function(const char* payload, const char* post_path);
void retry_post_task(void *param);

void post_bomba_task(void* pvParameters);
void post_ventilador_task(void* pvParameters);
void post_condensador_task(void* pvParameters);
void post_ambiente_task(void* pvParameters);
void post_compressor_task(void* pvParameters);
void post_regime_task(void* pvParameters);
void post_regime_condensacao_task(void* pvParameters);

#endif 
