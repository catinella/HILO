include(${CMAKE_CURRENT_LIST_DIR}/../configUtils.cmake)

idf_component_register(
	SRCS "prod.c"
	INCLUDE_DIRS "." "../components/crossPlatform_code"
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



