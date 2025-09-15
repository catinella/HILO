idf_component_register(
	SRCS         "wifiNetwork.c"
	INCLUDE_DIRS  "include" "../crossPlatform_code"
	REQUIRES
		esp_event
		esp_wifi
		esp_netif
		nvs_flash
		esp_system
		freertos
		log
)
