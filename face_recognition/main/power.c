#include "power.h"

static const char* TAG = "POWER";


static int64_t last_activity_us;


void power_manager_task(void *args){
    const int64_t IDLE_TIMEOUT = 60LL * 1000000LL;

    for(;;){
        int64_t now = esp_timer_get_time();
        EventBits_t bits = xEventGroupGetBits(s_wifi_event_group);

        bool active = 
            bits & STREAM_READY_BIT ||
            bits & MQTT_READY_BIT;
            
        
        if(!active && (now - last_activity_us) > IDLE_TIMEOUT){
            ESP_LOGI(TAG, "Idle timeout reached, entering light sleep");
            esp_light_sleep_start();

            ESP_LOGI(TAG, "Waking up from light sleep");
            mark_activity();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void power_init(void){
    ESP_LOGI(TAG, "Power init running");
    esp_sleep_enable_timer_wakeup(60LL * 1000000LL);

    xTaskCreatePinnedToCore(
        power_manager_task,
        "power_manager_task",
        4096,
        NULL,
        5,
        NULL,
        1
        
    );

}

void mark_activity(void){
    last_activity_us = esp_timer_get_time();
}
int64_t get_last_activity(void){
    return last_activity_us;
}
