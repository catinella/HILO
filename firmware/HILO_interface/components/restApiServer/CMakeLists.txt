idf_component_register(
	SRCS  "restApiServer.c"
	INCLUDE_DIRS  
		"include" 
		"../crossPlatform_code"
	REQUIRES
		esp_netif
		esp_event
		freertos
		log
		esp_http_server
		json
		esp_timer
)
