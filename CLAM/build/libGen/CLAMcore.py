#!/usr/bin/python

import libGen, sys

def makelib() :
    libCLAMCore = libGen.LibGenerator("Core")

    print "Generating libCLAM%s..."%libCLAMCore.libName

    # These files are blackballed since they depend on
    # some concrete ProcessingData
    libCLAMCore.blackBall( "AudioInPort" )
    libCLAMCore.blackBall( "AudioInPortPublisher" )
    libCLAMCore.blackBall( "AudioOutPort" )
    libCLAMCore.blackBall( "AudioOutPortPublisher" )
    libCLAMCore.blackBall( "ScaleCnv" )

    libCLAMCore.addFolder( "Base" )

    libCLAMCore.addFolder( "Data/Base" )

    libCLAMCore.addFolder( "Defines" )
    libCLAMCore.addFolder( "Defines/Windows" )    
    libCLAMCore.addFolder( "Defines/Unix" )

    libCLAMCore.addFolder( "Errors" )

    libCLAMCore.addFolder( "Flow/Ports" )
    libCLAMCore.addFolder( "Flow/Networks" )
    libCLAMCore.addFolder( "Flow/Controls" )

    libCLAMCore.addFolder( "Processing/Base" )
    
    libCLAMCore.addFolder( "Standard" )
    libCLAMCore.addFolder( "Storage/Base" )
    libCLAMCore.addFolder( "Storage/XML" )
    libCLAMCore.addFolder( "System" )
    libCLAMCore.addFolder( "System/Threads" )
    libCLAMCore.addFolder( "CbLib", "externals" )

    libCLAMCore.activate('XML')
    libCLAMCore.activate('PTHREADS')

    libCLAMCore.generateFiles()
    print "Files are being generated on build/Libs/%s..."%libCLAMCore.libName

if __name__ == "__main__" :
    makelib()
