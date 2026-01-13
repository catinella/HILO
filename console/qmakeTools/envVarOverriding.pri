#-------------------------------------------------------------------------------------------------------------------------------
#
# Description:
#	This code overrides the variables belong to the ENVVARSLIST list, with the valuse defined by the environment variables
#	with the same names
#
#	[!] Because the change must be available in the main scope, this code CANNOT be moved inside a function, as usual.
#
#-------------------------------------------------------------------------------------------------------------------------------
message("Setting overriding by environment variables")
for(varName, ENVVARSLIST) {
	varContent = $$getenv($$varName)
	!isEmpty(varContent) {
		eval($${varName} = $$quote($$varContent))
		message("   $$varName")
	}
}   
