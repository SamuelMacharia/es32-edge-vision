#pragma once


#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "event_groups.h"
#include "esp_err.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "esp_event.h"
#include "wifi.h"
#include "mqtt.h"

void power_init(void);
void mark_activity(void);

