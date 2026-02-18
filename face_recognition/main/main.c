#include <esp_log.h>
#include "sdkconfig.h"
#include "event_groups.h"
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_camera.h"
#include "esp_wifi.h"
#include "esp_timer.h"
#include "esp_http_server.h"
#include "wifi.h"
#include "power.h"
#include "mqtt.h"
#include "capture.h"
#include "server.h"
#include "system.h"


static const char* TAG = "AI_THINKER";


void app_main(void)
{
    
    esp_err_t ret = nvs_flash_init();
    if (ret==ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret=nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);


    system_init();

    init_camera();
    vTaskDelay(pdMS_TO_TICKS(1500));

    power_init();

    esp_err_t wifi_status = init_wifi();
    if(wifi_status==ESP_OK){
        ESP_LOGI(TAG, "CONNECTED TO %s", CONFIG_WIFI_SSID);
    }else{
        ESP_LOGE(TAG, "FAILED TO CONNECT TO %s", CONFIG_WIFI_SSID);
    }
    vTaskDelay(pdMS_TO_TICKS(100));

    mqtt_start();
    
    xTaskCreatePinnedToCore(
        handle_webServer,
        "handle_webServer",
        4096,
        NULL,
        2,
        &xWebServerHandle,
        0
    ); 
    
    
    xTaskCreatePinnedToCore(
        handle_capture, //Task Function
        "handle_capture", //Task Name
        4096,  //Stack Size
        NULL, //parameters
        2, // Priority
        &xCaptureTaskHandle, //Task Handle
        1 // core
    );   
}
