#ifndef CREDENTIALS_EMPTY_H
#define CREDENTIALS_EMPTY_H

#include "../lib/classes/IdsList/ID_List.h"

#define USE_EXTERNAL_BACKEND 0 /* 0 = LOCAL, 1 = EXTERNAL CLOUD SGEM API */
#define DEBUG_TERMINAL 0 /* 0 = NOT PRINT ON TERMINAL, 1 = PRINT ON TERMINAL */

static const char * const CREDENTIALS_SSID = ""; /* WIFI SSID */
static const char * const CREDENTIALS_PASSWORD = ""; /* WIFI PASSWORD */
static const char * const CREDENTIALS_LOGIN_USERNAME = ""; /* USERNAME FROM SGEM USER TO POST */
static const char * const CREDENTIALS_LOGIN_PASSWORD = ""; /* PASSWORD FROM SGEM USER TO POST*/

/* VARIABLES WHEN IP IS STATIC */
static const char * const CREDENTIALS_WIFI_IP = ""; 
static const char * const CREDENTIALS_NETMASK = "";
static const char * const CREDENTIALS_GATEWAY = ""; 
static const char * const CREDENTIALS_DNS = "";
static const char * const CREDENTIALS_DNS_SECOND = "";
static const char * const CREDENTIALS_NTP_SERVER = "pool.ntp.org";

// Function to initialize the equipment IDs inline, example with IDs
inline void initializeEquipmentIds() {
    IdsList& eq = Singleton<IdsList>::instance();
    eq.unidadeId = 0;
    eq.condensadorId = 0;
    eq.condensacaoId = 0;
    eq.ambienteIds = {0};
    eq.chillerIds = {0};
    eq.compressorIds = {1,2,3}; 
    eq.bombaIds = {1, 2, 0, 0, 0, 0, 0, 0, 0, 0};       // Aways 10 positions - if not active = 0
    eq.ventiladorIds = {1, 2, 0, 0, 0, 0, 0, 0, 0, 0};  // Aways 10 positions - if not active = 0
    eq.regimeIds = {1, 0, 0, 4, 5, 0, 0, 0, 0, 0};      // Aways 10 positions - if not active = 0
}

// Definition of the backend URL
#if USE_EXTERNAL_BACKEND
    #define CREDENTIALS_BACKEND_URL "" // Backend URL from SGEM
#else
    //#define CREDENTIALS_BACKEND_URL "http://192.168.18.18:7022"
    #define CREDENTIALS_BACKEND_URL "http://192.168.2.155:7022" // Backend LOCAL from development environment
#endif

#endif // CREDENTIALS_H
