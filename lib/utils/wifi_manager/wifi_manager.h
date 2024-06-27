// wifi_manager.h

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "../../../src/Credentials.h"
#include "../http_client/http_client.h"
#include "../ntp_time_sync/ntp_time_sync.h"
#include <string.h>

extern EventGroupHandle_t wifi_event_group;
extern const int WIFI_CONNECTED_BIT;

void start_connection();
void print_ip_info_task(void *pvParameter);
void print_mac_address_task(void *pvParameter);
void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#endif 
