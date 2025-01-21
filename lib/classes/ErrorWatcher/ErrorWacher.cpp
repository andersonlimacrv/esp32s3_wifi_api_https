#include "ErrorWatcher.h"

#define TAG "ErrorWatcher"  

ErrorWatcher::ErrorWatcher() : criticalErrors(0), warningErrors(0), infoErrors(0), sucessHit(0) {}

void ErrorWatcher::addEvent(EventType eventType) {
    switch (eventType) {
        case ERR_CRITICAL:
            criticalErrors++;
            printf("[%s] Critical error added. Total critical errors: %d\n", TAG, criticalErrors);
            break;
        case ERR_WARNING:
            warningErrors++;
            printf("[%s] Warning error added. Total warning errors: %d\n", TAG, warningErrors);
            break;
        case ERR_INFO:
            infoErrors++;
            printf("[%s] Info error added. Total info errors: %d\n", TAG, infoErrors);
            break;
        case SUCESS:
            sucessHit++;
            printf("[%s] Sucess event added. Total sucess events: %d\n", TAG, sucessHit);
    }

    checkRestartCondition();  
}

void ErrorWatcher::checkRestartCondition() {
    if (criticalErrors >= CRITICAL_ERROR_LIMIT) {
        printf("[%s] Critical error limit reached, restarting ESP...\n", TAG);
        esp_restart();
    } else if (warningErrors >= WARNING_ERROR_LIMIT) {
        printf("[%s] Warning error limit reached, restarting ESP...\n", TAG);
        esp_restart();
    } else if (infoErrors >= INFO_ERROR_LIMIT) {
        printf("[%s] Info error limit reached, restarting ESP...\n", TAG);
        esp_restart();
    } else if(sucessHit >= SUCESS_HIT_LIMIT) {
      printf("[%s] Sucess limit reached, clear all errors...\n", TAG);
      clearAllErros();
    }
}

int ErrorWatcher::getCriticalErrors() const {
    return criticalErrors;
}

int ErrorWatcher::getWarningErrors() const {
    return warningErrors;
}

int ErrorWatcher::getInfoErrors() const {
    return infoErrors;
}

int ErrorWatcher::getSucessHit() const { 
  return sucessHit;
}


int ErrorWatcher::clearAllErros() {
  this->criticalErrors = 0;
  this->warningErrors = 0;
  this->infoErrors = 0;
  this->sucessHit = 0;

  printf("[%s] Current Errors: Error CRITICAL (%d), Error WARNING: (%d), Error INFO (%d), SUCCESS:(%d) \n", TAG, criticalErrors, warningErrors, infoErrors, sucessHit);
  return 0;
}

ErrorWatcher errorWatcher;
