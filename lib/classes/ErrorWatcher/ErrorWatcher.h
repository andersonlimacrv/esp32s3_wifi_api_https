#ifndef ERROR_WATCHER_H
#define ERROR_WATCHER_H

#include "esp_system.h"
#include "../../../src/Credentials.h"
#include <stdio.h>
class ErrorWatcher {
public:
    
    enum ErrorType {
        CRITICAL,
        WARNING,
        INFO
    };

   
    ErrorWatcher();

    
    void addError(ErrorType errorType);

    
    int getCriticalErrors() const;
    int getWarningErrors() const;
    int getInfoErrors() const;

private:
    int criticalErrors;
    int warningErrors;
    int infoErrors;

    const int CRITICAL_ERROR_LIMIT = CREDENTIALS_CRITICAL_ERROR_LIMIT;  
    const int WARNING_ERROR_LIMIT = CREDENTIALS_WARNING_ERROR_LIMIT;  
    const int INFO_ERROR_LIMIT = CREDENTIALS_INFO_ERROR_LIMIT;      

    void checkRestartCondition();
};

extern ErrorWatcher errorWatcher;

#endif // ERROR_WATCHER_H
