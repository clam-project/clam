#!/usr/bin/python

import libGen, sys

def makelib() :
    libCLAMProcessing = libGen.LibGenerator( "Processing" )

    print "Generating libCLAM%s..."%libCLAMProcessing.libName
    # Depends on CUIDADO code
    libCLAMProcessing.blackBall( "OnsetDetector" )
    libCLAMProcessing.blackBall( "SMSAnalysis" )
    libCLAMProcessing.blackBall( "SMSSynthesis" )

    libCLAMProcessing.addFile( "ScaleCnv", "Standard" )

    libCLAMProcessing.addFolder( "Data/BasicProcessing" )
    libCLAMProcessing.addFolder( "Data/Descriptors" )
    libCLAMProcessing.addFolder( "Data/Descriptors/Melody" )

    libCLAMProcessing.addFolder( "Processing/Analysis" )
    libCLAMProcessing.addFolder( "Processing/ArithOps" )
    libCLAMProcessing.addFolder( "Processing/Controls" )
    libCLAMProcessing.addFolder( "Processing/Generators" )
    libCLAMProcessing.addFolder( "Processing/Plugins" )
    libCLAMProcessing.addFolder( "Processing/Synthesis" )
    libCLAMProcessing.addFolder( "Processing/Transformations" )
    
    libCLAMProcessing.addFolder( "Flow/Ports")
    libCLAMProcessing.addFolder( "numrec", "externals")

    libCLAMProcessing.activate( 'XML' )
    libCLAMProcessing.activate( 'PTHREADS' )
    libCLAMProcessing.activate( 'FFTW' )

    libCLAMProcessing.dependsOn( "Core" )

    libCLAMProcessing.generateFiles()
    print "Files are being generated on build/Libs/%s..."%libCLAMProcessing.libName

if __name__ == "__main__" :
    makelib()
