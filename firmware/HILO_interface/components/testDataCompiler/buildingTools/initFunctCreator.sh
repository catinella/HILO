#!/bin/bash
#-------------------------------------------------------------------------------------------------------------------------------
#                                                    _   _ ___ _     ___  
#                                                   | | | |_ _| |   / _ \
#                                                   | |_| || || |  | | | |
#                                                   |  _  || || |__| |_| |
#                                                   |_| |_|___|_____\___/ 
#                                                    Hardware in the loop
#
#
# File:   initFunctCreator.sh
#
# Author: Silvano Catinella <catinella@yahoo.com>
#
# Language: BASH
#
# Description:
#	This script creates a procedure to register all sub-modules
#
#	Command's arguments:
#		--targetName=<file-name>   The name of the file that contains all include directives
#		--prefix=<string>          The file names prefix of the sub-modules
#		--path=<headerFiles-path>
#
#-------------------------------------------------------------------------------------------------------------------------------


function usage() {
	echo "use $0 --targetName=<file-name> --prefix=<string> [--path=<header-files path>]" >&2
}

function usageError() {
	echo "ERROR! \"$1\" is unknown argument" >&2
	usage
	exit 127
}

tgtFile=""
smPrfx=""
incFilesPath="./"

for arg in $*
do
	if expr "$arg" : '--[^-=]\+=' >/dev/null ; then
		key=${arg%%=*}
		val=${arg#*=}
		
		case "$key" in
			"--targetName")
				tgtFile="$val"
			;;
			"--prefix")
				smPrfx="$val"
			;;
			"--path")
				incFilesPath="$val"
			;;
			*)
				# ERROR!
				usageError "$arg"
			;;
		esac
	fi
done


#
# Check for the target file name
#
[ -z "$tgtFile" ] && {
	echo "ERROR! undefined target file" >&2
	usage
	exit 131
}


#
# Check for the sub-modules prefix
#
[ -z "$tgtFile" ] && {
	echo "ERROR! undefined target file" >&2
	usage
	exit 131
}


#
# Check for the header files
#
subModulesList=$(ls ${incFilesPath}/${incFilesPrfx}*.h |sed -e 's/^.*\///' -e 's/\.h$//') || {
	echo "ERROR! no files found" >&2
	exit 129
}

echo "// ----------- Self generated procedure by $0 -----------" > $tgtFile
echo "if (" >> $tgtFile

for sm in $subModulesList
do
	echo -e "\t${sm}_init() == WERROR_SUCCESS &&" >> $tgtFile
done
echo -e "\ttrue\n)\n\t err = WERROR_SUCCESS;" >> $tgtFile
echo -e "// ---------------------- END ----------------------\n" >> $tgtFile
