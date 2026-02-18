#pragma once

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "event_groups.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_err.h"
#include "pins.h"
#include "wifi.h"
#include "power.h"
#include "system.h"


esp_err_t init_camera(void);
void handle_capture(void* args);