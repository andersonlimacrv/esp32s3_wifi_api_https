#include "ntp_time_sync.h"
/* Sincronização de tempo usando o SNTP (Simple Network Time Protocol)

1. Obtém o tempo atual em um formato específico.
2. Define um tratador para notificações de sincronização de tempo.
3. Inicializa o SNTP para sincronização de tempo com um servidor NTP específico.

*/

void get_time_now(char *time_buffer, size_t buffer_size) {
    time_t now;
    time(&now);
    struct tm *timeinfo = localtime(&now); 
    strftime(time_buffer, buffer_size, "%Y-%m-%dT%H:%M:%S.000Z", timeinfo);
}

void handler_time_sync_cb(struct timeval *tv) { 
    printf("Initializing handler for time sync notification\n");
    char time_buffer[64];
    get_time_now(time_buffer, sizeof(time_buffer));
    printf("Time Synced: %s\n", time_buffer);
}

void ntp_time_sync_task(void *pvParameter) {
    printf("Initializing SNTP\n");
    setenv("TZ", "UTC", 1);
    tzset();

    esp_sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_set_time_sync_notification_cb(&handler_time_sync_cb);
    esp_sntp_init();

    vTaskDelete(NULL);
}
