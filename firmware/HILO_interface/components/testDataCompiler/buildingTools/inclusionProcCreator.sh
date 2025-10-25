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
# File:   inclusionProcCreator.sh
#
# Author: Silvano Catinella <catinella@yahoo.com>
#
# Language: BASH
#
# Description:
#	This script is used to make a simple C-procedure that includes all selected header files
#
#	Command's arguments:
#		--targetName=<file-name>   The name of the file that contains all include directives
#		[--prefix=<string>]        The file names prefix of the eader files to include
#		[--path=<incfiles-path>]   The path of the header files
#		[--pathErasing]"           Option to erase the path in the include directive. Useful with -I gcc option
#
#-------------------------------------------------------------------------------------------------------------------------------


function usage() {
	echo "use $0 --targetName=<file-name> [--prefix=<string>] [--path=<incfiles-path>] [--pathErasing]" >&2
}

function usageError() {
	echo "ERROR! \"$1\" is unknown argument" >&2
	usage
	exit 127
}

tgtFile=""
incFilesPrfx=""
incFilesPath="./"
declare -i pathErasing=0

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
				incFilesPrfx="$val"
			;;
			"--path")
				incFilesPath="$val"
			;;
			*)
				# ERROR!
				usageError "$arg"
			;;
		esac
	
	elif expr "$arg" : '--[^-=]\+' >/dev/null ; then
		case "$arg" in
			"--help")
				usage
				exit 0
			;;
			"--pathErasing")
				pathErasing=1
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
# Check for the header files
#
IncFilesList=$(ls ${incFilesPath}/${incFilesPrfx}*.h) || {
	echo "ERROR! no files found" >&2
	exit 129
}

echo "// Self generated file by $0" > $tgtFile

for file in $IncFilesList
do
	if [ $pathErasing -eq 1 ]; then
		incFile=${file##*/}
	else
		incFile=$file
	fi

	echo "#include <$incFile>" >> $tgtFile
done
