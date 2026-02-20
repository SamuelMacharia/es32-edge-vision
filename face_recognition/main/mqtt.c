#include "mqtt.h"


static const char* TAG = "MQTT_EVENT_HANDLER";

#define MQTT_TOPIC_STATUS_HEART "esp32_cam/%s/status/heartbeat"
#define MQTT_TOPIC_CMD_START "esp32_cam/%s/cmd/start_stream"

static char device_id[32];


void mqtt_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
    void* event_data){
    esp_mqtt_event_handle_t event = event_data;

    switch(event_id){
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED, msg_id=%d", event->msg_id);
            char topic [128];

            xEventGroupSetBits(s_wifi_event_group, MQTT_READY_BIT);
            mark_activity();
            break;
        case MQTT_EVENT_DISCONNECTED:
            xEventGroupClearBits(s_wifi_event_group, MQTT_READY_BIT);
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            //mqtt_handle_data(event);
            break;
        default:
            break;
    }
    
}

esp_err_t mqtt_start(void){
    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = CONFIG_MQTT_BROKER_URI,
    };
    esp_mqtt_client_handle_t mqtt_client = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_register_event(
        mqtt_client,
        ESP_EVENT_ANY_ID,
        mqtt_event_handler,
        NULL
    );

    return esp_mqtt_client_start(mqtt_client);

}