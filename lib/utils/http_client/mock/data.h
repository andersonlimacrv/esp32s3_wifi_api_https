#ifndef DATA_H
#define DATA_H

#include "../../payload_formatter/payload_formatter.h"
#include "../../ntp_time_sync/ntp_time_sync.h"
#include "../../../classes/wifi_manager/wifi_manager.h"
#include "../http_client.h"
#include <random>
#include <cstring>
#include "../../../../src/Credentials.h" 

extern WiFiManager wifiManager;

void post_condensador_mock_task(void* pvParameters);
void post_bombas_condensador_mock_task(void* pvParameters);
void post_ventiladores_condensador_mock_task(void* pvParameters);
void post_ambientes_mock_task(void* pvParameters);
void post_compressores_mock_task(void* pvParameters);
void post_regimes_mock_task(void* pvParameters);
void post_regime_condensacao_mock_task(void* pvParameters);
void post_energia_mock_task(void* pvParameters);


#endif // DATA_H
