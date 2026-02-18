#pragma once

#include <stdio.h>
#include <inttypes.h>

#include "event_groups.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "sdkconfig.h"
#include "esp_event.h"
#include "system.h"


void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
    void* event_data);
esp_err_t init_wifi(void);