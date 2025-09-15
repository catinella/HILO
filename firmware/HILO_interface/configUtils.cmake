#------------------------------------------------------------------------------------------------------------------------------
#                                                    _   _ ___ _     ___  
#                                                   | | | |_ _| |   / _ \
#                                                   | |_| || || |  | | | |
#                                                   |  _  || || |__| |_| |
#                                                   |_| |_|___|_____\___/ 
#                                                    Hardware in the loop
#                                              (https://github.com/catinella/HILO)
#
#
# File name: configUtils.cmake
#
# Author: Silvano Catinella <catinella@yahoo.com>
#
# Description:
#	C-Make library for the configuration file parsing. The files want to use this library have to incude the following
#	statements:
#
#	include(${CMAKE_SOURCE_DIR}/config_utils.cmake)  # Including the library file
#	parse_config("<configuration file>" "<label>")   # It calls the parser and creates the variables
#	                                                 # Project's config file: "${CMAKE_SOURCE_DIR}/CMakeLists.conf"
#
#	[!] Because the config-file is defined as a dependence one, too. If this function will not found the argument defined one,
#	    then a new empty one will be created
#
#-----------------------------------------------------------------------------------------------------------------------------------

function(parse_config CONFIG_FILE TAG)
	message(STATUS "\n")
	if(EXISTS "${CONFIG_FILE}")
		set(filecontent "")
		message(STATUS "------------------------------------------------------------------------------")
		message(STATUS " ${TAG} module building configuration")
		message(STATUS "------------------------------------------------------------------------------")
		file(STRINGS "${CONFIG_FILE}" filecontent REGEX "^[ \t]*([A-Za-z_][^=]+)=([^\n]+)")
		string(STRIP filecontent "${filecontent}")
		string(CONCAT filecontent "${filecontent}" ";")
		#message(WARNING "===>${filecontent}")
	
		foreach(row ${filecontent})
			if(NOT ${row} MATCHES "^[ \t]*#")
				string(REGEX REPLACE "\n" "" match  "${row}")
				string(REGEX REPLACE "^[ \t]*([^=]+)[\t ]*=[\t ]*(.+)$" "\\1" varname  "${row}")
				string(REGEX REPLACE "^[ \t]*([^=]+)[\t ]*=[\t ]*(.+)$" "\\2" varvalue "${row}")
		
				# Global vars creation....
				set(${varname} "${varvalue}" PARENT_SCOPE)
			
				message(STATUS "Configuration data: ${varname}=${varvalue}")
			endif()
		endforeach()
	else()
		message(WARNING "Configuration file not found I will create an empty one")
		file(TOUCH "${CONFIG_FILE}")
	endif()
	message(STATUS "\n")
endfunction()


