#!/bin/bash

# TODO: Not removing new versions

GenerateDoxygen()
{
	Name=$1
	Branch=$2
	TargetDir=$3
	Date=$4
	cvs co $Branch -d CLAM-for-doxygen CLAM
	pushd CLAM-for-doxygen
		Version=`grep PROJECT_NUMBER .doxygen.cfg | (read var eq value; echo $value )`
		echo Generating Doxygen, output will be on the DoxyLog file...
		(
			cat .doxygen.cfg
			echo "PROJECT_NAME = $Name"
			echo "PROJECT_NUMBER = $Version $Date"
		) | doxygen - >DoxyLog 2>&1 

		Tarball=$TargetDir.tar.bz

		mv doxygen/html $TargetDir
		tar cfvj $Tarball $TargetDir

		ssh clamadm@www.iua.upf.es rm -rf $TargetDir
		scp -r $Tarball "clamadm@www.iua.upf.es:"
		ssh clamadm@www.iua.upf.es tar xfvj $Tarball
		ssh clamadm@www.iua.upf.es rm $Tarball
		scp -r DoxyLog "clamadm@www.iua.upf.es:$TargetDir"
	popd
	rm -rf CLAM-for-doxygen
}


if [ "$1" == "main" ]
then
	GenerateDoxygen "CLAM" "" "CLAM-doxygen" ""
elif [ "$1" == "devel" ]
then
	Date=`date -u +"%Y-%m-%d"`
	GenerateDoxygen "CLAM-development" "-r development-branch" "CLAM-devel-doxygen" "__$Date"
else
	echo "For main branch:  $0 main "  >&2
	echo "For devel branch:  $0 devel "  >&2
	exit -1
fi




