#include "esp_wifi.h"
#include <string.h>

// this WiFi access is specific to ESP-based WiFi boards
// example docs at https://github.com/espressif/esp-idf/tree/master/examples/wifi

static bool scan_done = false;

#define DEFAULT_SCAN_LIST_SIZE 30

static wifi_ap_record_t getWifiNetworks(){
    // based on https://github.com/espressif/esp-idf/blob/master/examples/wifi/scan/main/scan.c
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_scan_start(NULL, true);
    esp_wifi_scan_get_ap_records(&number, ap_info);
    esp_wifi_scan_get_ap_num(&ap_count);

    for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) {
        //const char* id = ap_info[i].ssid;
        //uint8_t rssi = ap_info[i].rssi;
        //wifi_auth_mode_t authMode = ap_info[i].authmode;
    }

    esp_wifi_stop;
    return *ap_info;
}

/*

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        scan_done = true;
    }
}

static void init_wifi(void)
{
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    esp_wifi_scan_start(NULL, true);

    while (1) {
        if (scan_done == true) {
            //display_scan_result();
            scan_done = false;
        } else { 
            vTaskDelay(100 / portTICK_RATE_MS);
            continue;
        }
    }
}
*/