#ifndef ERROR_WATCHER_H
#define ERROR_WATCHER_H

#include "esp_system.h"
#include "../../../src/Credentials.h"
#include <stdio.h>
class ErrorWatcher {
public:
    
    enum EventType {
        ERR_CRITICAL,
        ERR_WARNING,
        ERR_INFO,
        SUCESS
    };

    ErrorWatcher();
    
    void addEvent(EventType eventType);
    
    int getCriticalErrors() const;
    int getWarningErrors() const;
    int getInfoErrors() const;
    int getSucessHit() const;
    int clearAllErros();

private:
    int criticalErrors;
    int warningErrors;
    int infoErrors;
    int sucessHit;

    const int CRITICAL_ERROR_LIMIT = CREDENTIALS_CRITICAL_ERROR_LIMIT;  
    const int WARNING_ERROR_LIMIT = CREDENTIALS_WARNING_ERROR_LIMIT;  
    const int INFO_ERROR_LIMIT = CREDENTIALS_INFO_ERROR_LIMIT;
    const int SUCESS_HIT_LIMIT = CREDENTIALS_SUCESS_HIT;
      

    void checkRestartCondition();
};

extern ErrorWatcher errorWatcher;

#endif // ERROR_WATCHER_H
