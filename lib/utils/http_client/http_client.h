#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cJSON.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_http_client.h"
#include "freertos/event_groups.h"
#include "../payload_formatter/payload_formatter.h"
#include "../ntp_time_sync/ntp_time_sync.h"
#include "../../../src/Credentials.h"
#include "freertos/event_groups.h"

typedef struct {
    const char * const post_login;
    const char * const post_ambiente;
    const char * const post_condensador;
    const char * const post_bomba_condensador;
    const char * const post_ventilador_condensador;
    const char * const post_compressor;
    const char * const post_regime;
    const char * const post_regime_condensacao;
    const char * const post_energia;
} EndpointPaths;

const EndpointPaths paths = {
    .post_login = "/api/auth/login",
    .post_ambiente = "/api/leitura-ambiente",
    .post_condensador = "/api/leitura-condensador",
    .post_bomba_condensador = "/api/leitura-bomba",
    .post_ventilador_condensador = "/api/leitura-ventilador",
    .post_compressor = "/api/leitura-compressor",
    .post_regime = "/api/leitura-regime",
    .post_regime_condensacao = "/api/leitura-condensacao",
    .post_energia = "/api/leitura-energia"
};

extern const EndpointPaths paths;

esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt);
void client_post_auth_login(void *param);
void client_post_function(const char* payload, const char* post_path);
void authenticated_post_task(const char* payload, const char* post_path);
void retry_post_task(void *param);

void post_bomba_task(void* pvParameters);
void post_ventilador_task(void* pvParameters);
void post_condensador_task(void* pvParameters);
void post_ambiente_task(void* pvParameters);
void post_compressor_task(void* pvParameters);
void post_regime_task(void* pvParameters);
void post_regime_condensacao_task(void* pvParameters);
void post_energia_task(void* pvParameters);

void initialize_auth_event_group();
void initialize_post_mutex();

#endif
