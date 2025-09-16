include(${CMAKE_CURRENT_LIST_DIR}/../configUtils.cmake)

idf_component_register(
	SRCS         "firmware-esp32.c"
	INCLUDE_DIRS 
		"." 
		"../components/crossPlatform_code" 
		"../components/wifiNetwork/include"
		"../components/restApiServer/include"
	REQUIRES
		esp_timer
		log
		esp_wifi
		nvs_flash
		esp_http_server
		json
)


# Configuration file reading...
parse_config("${CMAKE_SOURCE_DIR}/CMakeLists.conf" "main")


#
# Checking for the firmware/iTest option
#
if(DEFINED FIRMWARETEST)
	message(WARNING "[!] TEST mode has been enabled")
	set(myexec ${FIRMWARETEST})
else()
	message(STATUS "Normal mode")
	set(myexec prod.c)
endif()


#
# Chjecking for the selected file
#
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${myexec}")
	if(NOT "${myexec}" STREQUAL "prod.c")
		message(STATUS "Testing firmware: ${CMAKE_CURRENT_SOURCE_DIR}/${myexec}")
	endif()

	execute_process(
		COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_CURRENT_SOURCE_DIR}" ./clink.sh ${myexec}
	)
else()
	message(FATAL_ERROR "Firmware file '[ERROR!] ${CMAKE_CURRENT_SOURCE_DIR}/${myexec}' does not exist.")
endif()


if(DEFINED HILO_MYSSIDNAME)
	message(STATUS "AP SSID: ${HILO_MYSSIDNAME}")
else()
	message(FATAL_ERROR "You have to define HILO_MYSSIDNAME symbol")
endif()


if(DEFINED HILO_MYPASSWORD)
	message(STATUS "AP Password: ${HILO_MYPASSWORD}")
else()
	message(FATAL_ERROR "You have to define HILO_MYPASSWORD symbol")
endif()


if(DEFINED HILO_SSIDNAME)
	message(STATUS "Ext. network SSID: ${HILO_SSIDNAME}")
else()
	message(FATAL_ERROR "You have to define HILO_MYSSIDNAME symbol")
endif()


if(DEFINED HILO_PASSWORD)
	message(STATUS "Ext. network Password: ${HILO_PASSWORD}")
else()
	message(FATAL_ERROR "You have to define HILO_MYPASSWORD symbol")
endif()


target_compile_definitions(
	${COMPONENT_LIB}
	PRIVATE
	HILO_MYSSIDNAME="\"${HILO_MYSSIDNAME}\""
	HILO_MYPASSWORD="\"${HILO_MYPASSWORD}\""
	HILO_SSIDNAME="\"${HILO_SSIDNAME}\""
	HILO_PASSWORD="\"${HILO_PASSWORD}\""
)

