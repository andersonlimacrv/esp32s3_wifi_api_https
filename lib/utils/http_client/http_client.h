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
} EndpointPaths;

extern const EndpointPaths paths;

esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt);
void client_post_auth_login(void *param);
void client_post_generic_function(const char* payload, const char* post_path);

#endif 
