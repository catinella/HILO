#-------------------------------------------------------------------------------------------------------------------------------
#                                                    _   _ ___ _     ___  
#                                                   | | | |_ _| |   / _ \
#                                                   | |_| || || |  | | | |
#                                                   |  _  || || |__| |_| |
#                                                   |_| |_|___|_____\___/ 
#                                                    Hardware in the loop
#                                              (https://github.com/catinella/HILO)
#
#
#
#
# Filename: utils.pri
#
# Author:   Silvano Catinella <catinella@yahoo.com>
#
# Description:
#	This is a general purpose library for qmake6 tool-chain building process 
# 
#-------------------------------------------------------------------------------------------------------------------------------

defineTest(checkPreTargetDepsExist) {
	#
	# Description:
	#	This function checks for all dependences listed in the global var PRE_TARGETDEPS
	#
	out = true
	isEmpty(PRE_TARGETDEPS) {
		message("PRE_TARGETDEPS is empty: nothing to check")
	
	} else {
		for(dep, PRE_TARGETDEPS) {
			abs = $$absolute_path($$dep, $$PWD)
			exists($$abs) {
				message("$$dep ..OK!")
			} else {
				error("PRE_TARGETDEPS missing file: $$abs")
				out = false
			}
		}
	}	

	return($$out)
}

