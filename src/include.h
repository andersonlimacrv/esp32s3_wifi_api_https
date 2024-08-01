#include <SoftwareSerial.h>
#include <ModbusMaster.h>

#include "../lib/utils/ntp_time_sync/ntp_time_sync.h"
#include "../lib/utils/http_client/http_client.h"
#include "../lib/utils/http_client/mock/data.h"
#include "../lib/utils/payload_formatter/payload_formatter.h"
#include "../lib/classes/wifi_manager/wifi_manager.h"
#include "Credentials.h"
#include "config.h"

