#include "capture.h"

static const char* TAG = "AI_THINKER";


camera_config_t config;


esp_err_t init_camera(void){
    
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 2;

    esp_err_t err = esp_camera_init(&config);
    if(err != ESP_OK){
        ESP_LOGE(TAG, "Camera Initialization failed");
        return err;
    }

    //flip the camera to the right orientation
    sensor_t *s = esp_camera_sensor_get();
    s->set_vflip(s,1);
    s->set_hmirror(s,0);

    ESP_LOGI(TAG, "Camera Initialization successful");
    return ESP_OK;
}


void handle_capture(void* args){
    camera_fb_t* fb = NULL;
    
    for(;;){
        xEventGroupWaitBits(
            s_wifi_event_group,
            STREAM_READY_BIT,
            pdFALSE,
            pdTRUE,
            portMAX_DELAY
        );
       fb = esp_camera_fb_get();
       if(!fb){
        ESP_LOGE(TAG, "Camera Capture Failed");
        
        break;
       }else{
        if(xQueueSend(xFrameQueue, &fb, portMAX_DELAY) != pdPASS){
            esp_camera_fb_return(fb);
        }
       }
       UBaseType_t high_water_mark = uxTaskGetStackHighWaterMark(NULL);
       ESP_LOGI(TAG, "Remaining Stack size %u words", (unsigned int)high_water_mark);
            
       vTaskDelay(pdMS_TO_TICKS(10));
    }
}