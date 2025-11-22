# Links

https://github.com/espressif/esp-zigbee-sdk/tree/main/examples/esp_zigbee_HA_sample/HA_color_dimmable_light

https://github.com/espressif/esp-zigbee-sdk/issues/503



If so, I think you can refer to the below code to change the dimmable light example to get the range extender device;

        static void esp_zb_task(void *pvParameters)
        {
            /* initialize Zigbee stack */
            esp_zb_cfg_t zb_nwk_cfg = ESP_ZB_ZR_CONFIG();
            esp_zb_init(&zb_nwk_cfg);
            esp_zb_ep_list_t *ep_list = esp_zb_ep_list_create();
            esp_zb_endpoint_config_t endpoint_config = {
                .endpoint = HA_COLOR_DIMMABLE_LIGHT_ENDPOINT,
                .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID,
                .app_device_id = ESP_ZB_HA_RANGE_EXTENDER_DEVICE_ID,
                .app_device_version = 0,
            };
            esp_zb_cluster_list_t *cluster_list = esp_zb_zcl_cluster_list_create();
            esp_zb_cluster_list_add_basic_cluster(cluster_list, esp_zb_basic_cluster_create(NULL),
                                                  ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
            esp_zb_cluster_list_add_identify_cluster(cluster_list, esp_zb_identify_cluster_create(NULL),
                                                     ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
            zcl_basic_manufacturer_info_t info = {
                .manufacturer_name = ESP_MANUFACTURER_NAME,
                .model_identifier = ESP_MODEL_IDENTIFIER,
            };
        
            esp_zb_ep_list_add_ep(ep_list, cluster_list, endpoint_config);
            esp_zcl_utility_add_ep_basic_manufacturer_info(ep_list, HA_COLOR_DIMMABLE_LIGHT_ENDPOINT, &info);
            esp_zb_device_register(ep_list);
            esp_zb_core_action_handler_register(zb_action_handler);
            esp_zb_set_primary_network_channel_set(ESP_ZB_PRIMARY_CHANNEL_MASK);
            ESP_ERROR_CHECK(esp_zb_start(false));
            esp_zb_stack_main_loop();
        }

