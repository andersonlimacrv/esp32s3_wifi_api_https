#ifndef NTP_TIME_SYNC_H
#define NTP_TIME_SYNC_H

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <esp_sntp.h>

void get_time_now(char *time_buffer, size_t buffer_size);
void handler_time_sync_cb(struct timeval *tv);
void ntp_time_sync_task(void *pvParameter);


#endif 
