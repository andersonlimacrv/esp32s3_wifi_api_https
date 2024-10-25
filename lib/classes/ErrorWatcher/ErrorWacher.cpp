#include "ErrorWatcher.h"

#define TAG "ErrorWatcher"  

ErrorWatcher::ErrorWatcher() : criticalErrors(0), warningErrors(0), infoErrors(0) {}

void ErrorWatcher::addError(ErrorType errorType) {
    switch (errorType) {
        case CRITICAL:
            criticalErrors++;
            printf("[%s] Critical error added. Total critical errors: %d\n", TAG, criticalErrors);
            break;
        case WARNING:
            warningErrors++;
            printf("[%s] Warning error added. Total warning errors: %d\n", TAG, warningErrors);
            break;
        case INFO:
            infoErrors++;
            printf("[%s] Info error added. Total info errors: %d\n", TAG, infoErrors);
            break;
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

ErrorWatcher errorWatcher;
