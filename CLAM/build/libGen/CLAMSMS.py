#!/usr/bin/python

import libGen, sys

def makelib() :
    libCLAMSMS = libGen.LibGenerator( "SMS" )

    print "Generating libCLAM%s..."%libCLAMSMS.libName
    
    libCLAMSMS.addFile( "SMSAnalysis", "Processing/Analysis" )
    libCLAMSMS.addFile( "SMSSynthesis", "Processing/Synthesis" )
    libCLAMSMS.addFolder( "Processing/Transformations/SMS" )

    libCLAMSMS.activate( 'XML' )
    libCLAMSMS.activate( 'PTHREADS' )

    libCLAMSMS.dependsOn( "Core" )
    libCLAMSMS.dependsOn( "Processing" )
    libCLAMSMS.dependsOn( "IO" )

    libCLAMSMS.generateFiles( )

    print "Files are being generated on build/Libs/%s..."%libCLAMSMS.libName

if __name__ == "__main__" :
    makelib()
