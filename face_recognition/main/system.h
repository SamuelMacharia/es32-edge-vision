#pragma once

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "event_groups.h"
#include "esp_camera.h"



extern QueueHandle_t xFrameQueue;
extern EventGroupHandle_t s_wifi_event_group;


extern TaskHandle_t xCaptureTaskHandle;
extern TaskHandle_t xStreamingTaskHandle;
extern TaskHandle_t xWebServerHandle;
extern TaskHandle_t xMqttTaskHandle;
extern TaskHandle_t xPSTaskHandle;
extern TaskHandle_t handle_detect_face;
extern TaskHandle_t handle_face_recognition;

void system_init(void);
