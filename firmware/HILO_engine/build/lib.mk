#-------------------------------------------------------------------------------------------------------------------------------
#                                                     _   _ ___ _     ___  
#                                                    | | | |_ _| |   / _ \ 
#                                                    | |_| || || |  | | | |
#                                                    |  _  || || |__| |_| |
#                                                    |_| |_|___|_____\___/ 
#                                                     Hardware in the loop
# File name: lib.mk
#
# Author: Silvano Catinella <catinella@yahoo.com>
#
# Description:
#	Functions library used by Makefile
#
#-------------------------------------------------------------------------------------------------------------------------------

# Lowercase string converter
define _lc
$(shell echo $(1) |tr '[A-Z]' '[a-z]')
endef

# Uppercase string converter
define _uc
$(shell echo $(1) |tr '[a-z]' '[A-Z]')
endef

#
# Description:
#	It accepts a STM MCU part-name and returns the ARM architecture type ({M0|M3|M4...})
#
define _getArch
$(if $(findstring STM32F0,$(call _uc,$(1))),F0,\
$(if $(findstring STM32F3,$(call _uc,$(1))),F3,\
$(if $(findstring STM32F4,$(call _uc,$(1))),F4,)))
endef

