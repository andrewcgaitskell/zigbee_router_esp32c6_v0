#include <stdio.h>
#include "esp_log.h"
#include "esp_zigbee_core.h"
#include "esp_zigbee_zcl_basic.h"
#include "esp_zigbee_zcl_identify.h"

#define ROUTER_ENDPOINT 1
#define MANUFACTURER "ESPRESSIF"
#define MODEL "ESP-ZB-ROUTER"

static const char *TAG = "ZB_ROUTER";

static void zb_app_task(void *pvParameters)
{
    esp_zb_cfg_t zb_nwk_cfg = ESP_ZB_ZR_CONFIG();
    esp_zb_init(&zb_nwk_cfg);

    esp_zb_ep_list_t *ep_list = esp_zb_ep_list_create();

    esp_zb_endpoint_config_t endpoint_config = {
        .endpoint = ROUTER_ENDPOINT,
        .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID, // Technically not critical for router
        .app_device_id = ESP_ZB_HA_ON_OFF_LIGHT_DEVICE_ID, // Placeholder; router only, type is not important here
        .app_device_version = 0,
    };

    esp_zb_cluster_list_t *cluster_list = esp_zb_zcl_cluster_list_create();
    esp_zb_cluster_list_add_basic_cluster(cluster_list, esp_zb_basic_cluster_create(NULL), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    esp_zb_cluster_list_add_identify_cluster(cluster_list, esp_zb_identify_cluster_create(NULL), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);

    zcl_basic_manufacturer_info_t info = {
        .manufacturer_name = MANUFACTURER,
        .model_identifier = MODEL,
    };

    esp_zb_ep_list_add_ep(ep_list, cluster_list, endpoint_config);
    esp_zcl_utility_add_ep_basic_manufacturer_info(ep_list, ROUTER_ENDPOINT, &info);
    esp_zb_device_register(ep_list);

    esp_zb_set_primary_network_channel_set(ESP_ZB_PRIMARY_CHANNEL_MASK);
    ESP_ERROR_CHECK(esp_zb_start(false));
    esp_zb_stack_main_loop();
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting Zigbee Router...");
    xTaskCreate(zb_app_task, "zb_task", 4096, NULL, 5, NULL);
}
