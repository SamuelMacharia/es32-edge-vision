#include "server.h"

static const char* TAG = "Web Server";


httpd_uri_t stream_uri={
        .uri ="/stream",
        .method = HTTP_GET,
        .handler= start_stream_handler,
        .user_ctx = NULL
    };
httpd_handle_t server = NULL;


esp_err_t startWebServer(){
    ESP_LOGI(TAG, "Starting http server");

    if(server != NULL){
        ESP_LOGW(TAG, "Web Server already running");
        return ESP_OK;
    }
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    config.recv_wait_timeout = 10;
    config.send_wait_timeout = 10;

    esp_err_t err = httpd_start(&server, &config);
    if(err != ESP_OK){
        ESP_LOGE(TAG, "Failed to start server %s", esp_err_to_name(err));
        server= NULL;
        return err;
    }
    httpd_register_uri_handler(server, &stream_uri);
    ESP_LOGI(TAG, "Web Server Started Successfully");
    return ESP_OK;
}

void handle_webServer(void *args){
    xEventGroupWaitBits(
        s_wifi_event_group,
        WIFI_CONNECTED_BIT,
        pdFALSE,
        pdTRUE,
        portMAX_DELAY
    );
    startWebServer();
    vTaskDelete(NULL);
}

void handle_stream(void* args){
    httpd_req_t* req = (httpd_req_t*)args;
    camera_fb_t* recieved_fb = NULL;
    esp_err_t res = ESP_OK;

    static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=frame";
    static const char* _STREAM_BOUNDARY = "\r\n--frame\r\n";
    static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
    
    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    httpd_resp_set_hdr(req, "Cache-control", "no-cache");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin","*");
    while(httpd_req_to_sockfd(req)>=0){
        if(xQueueReceive(xFrameQueue, &recieved_fb, portMAX_DELAY)==pdTRUE){          
            if(res != ESP_OK){
                ESP_LOGE(TAG, "Something happened %s", res);
                break;
            }

            char header[64];
            size_t hlen = snprintf(header, sizeof(header), _STREAM_PART, recieved_fb->len);
            res= httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));

            if(res==ESP_OK){
                httpd_resp_send_chunk(req, header, hlen);
            }
            if(res==ESP_OK){
                httpd_resp_send_chunk(req, (const char*)recieved_fb->buf, recieved_fb->len);
            }
            esp_camera_fb_return(recieved_fb);
            recieved_fb=NULL;
        }
    }
    httpd_req_async_handler_complete(req);
    xEventGroupClearBits(s_wifi_event_group, STREAM_READY_BIT);
    vTaskDelete(NULL);
}

esp_err_t start_stream_handler(httpd_req_t* req){
    httpd_req_t *copy = NULL;

    //create a persistent copy safe for other tasks
    esp_err_t res = httpd_req_async_handler_begin(req, &copy);

    if(res==ESP_OK){
        xTaskCreatePinnedToCore(handle_stream, "handle_stream", 4096, (void*)copy, 3, &xStreamingTaskHandle, 0);
        xEventGroupSetBits(s_wifi_event_group, STREAM_READY_BIT);
    }else{
        xEventGroupClearBits(s_wifi_event_group, STREAM_READY_BIT);
    }
    return res;
}
