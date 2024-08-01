#ifndef NTP_TIME_SYNC_H
#define NTP_TIME_SYNC_H

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <esp_sntp.h>

/**
 * @brief Obtém o tempo atual em formato de string.
 * 
 * @param time_buffer Buffer para armazenar o tempo formatado.
 * @param buffer_size Tamanho do buffer.
 */
void get_time_now(char *time_buffer, size_t buffer_size);

/**
 * @brief Manipulador de callback para notificações de sincronização de tempo NTP.
 * 
 * @param tv Estrutura timeval com o tempo sincronizado.
 */
void handler_time_sync_cb(struct timeval *tv);

/**
 * @brief Tarefa para inicializar a sincronização de tempo NTP.
 * 
 * @param pvParameter Parâmetros da tarefa.
 */
void ntp_time_sync_task(void *pvParameter);

#endif // NTP_TIME_SYNC_H
