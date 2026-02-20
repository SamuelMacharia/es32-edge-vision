#pragma once

#include <stdio.h>
#include <inttypes.h>

#include "sdkconfig.h"
#include "esp_err.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "event_groups.h"
#include "wifi.h"
#include "power.h"


esp_err_t mqtt_start(void);
void mqtt_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
    void* event_data);