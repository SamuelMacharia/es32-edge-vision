#include "system.h"

#define CAMERA_MODEL_AI_THINKER
#define FRAME_QUEUE_LEN 3



TaskHandle_t xCaptureTaskHandle;
TaskHandle_t xStreamingTaskHandle;
TaskHandle_t xWebServerHandle;
TaskHandle_t xMqttTaskHandle;
TaskHandle_t xPSTaskHandle;
TaskHandle_t handle_detect_face;
TaskHandle_t handle_face_recognition;

QueueHandle_t xFrameQueue = NULL;
EventGroupHandle_t s_wifi_event_group = NULL;

void system_init(void){
    s_wifi_event_group = xEventGroupCreate();
    xFrameQueue = xQueueCreate(FRAME_QUEUE_LEN, sizeof(camera_fb_t *));

}