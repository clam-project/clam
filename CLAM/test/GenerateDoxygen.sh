#!/bin/bash

# TODO: Not removing new versions

GenerateDoxygen()
{
	Name=$1
	Branch=$2
	TargetDir=$3
	cvs co $Branch -d CLAM-for-doxygen CLAM
	pushd CLAM-for-doxygen
	# VERSION=`grep PROJECT_NUMBER .doxygen.cfg | (read var eq value; echo $value )`
	echo Generating Doxygen, output will be on the DoxyLog file...
	(
		cat .doxygen.cfg
		echo "PROJECT_NAME = $Name"
	) | doxygen - >DoxyLog 2>&1 

	ssh clamadm@www.iua.upf.es mv $TargetDir /tmp
	scp -r doxygen/html "clamadm@www.iua.upf.es:$TargetDir"
	scp -r DoxyLog "clamadm@www.iua.upf.es:$TargetDir"
	popd
	rm -rf CLAM-for-doxygen
}


if [ "$1" == "main" ]
then
	GenerateDoxygen "CLAM" "" "CLAM-doxygen"
elif [ "$1" == "devel" ]
then
	GenerateDoxygen "CLAM-development" "-r development-branch" "CLAM-devel-doxygen"
else
	echo "For main branch:  $0 main "  >&2
	echo "For devel branch:  $0 devel "  >&2
	exit -1
fi




