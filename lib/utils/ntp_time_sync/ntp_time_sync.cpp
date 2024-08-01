#include "ntp_time_sync.h"

void get_time_now(char *time_buffer, size_t buffer_size) {
    time_t now;
    time(&now);
    struct tm *timeinfo = localtime(&now); 
    strftime(time_buffer, buffer_size, "%Y-%m-%dT%H:%M:%S.000Z", timeinfo);
}

void handler_time_sync_cb(struct timeval *tv) { 
    printf("Initialized SNTP.\n");
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
