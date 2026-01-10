#-------------------------------------------------------------------------------------------------------------------------------
#
# Description:
#	This code modifies CONFIG global var based on GDB value
#	[!] Because the change must be available in the main scope, this code CANNOT be moved inside a function, as usual.
#
#-------------------------------------------------------------------------------------------------------------------------------
CONFIG -= debug
CONFIG -= release
contains(GDB, 1|true|yes|on) {
	message("WARNING! You are building $$TARGET module in debug mode")
	CONFIG  += debug
} else {
	message("[i] You are building $$TARGET in release mode")
	CONFIG  += release
}
