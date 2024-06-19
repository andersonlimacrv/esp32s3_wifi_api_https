#include <stdio.h>
#include <string.h>
#include <cstring>
#include <stdlib.h> 
#include <time.h>

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_sntp.h"
#include "esp_http_client.h"
#include <esp_event.h>
#include <esp_system.h>

#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"

#include "Credentials.h"