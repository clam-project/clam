#!/usr/bin/python

import libGen, sys

def main() :
    libCLAMCore = libGen.LibGenerator("Core")

    libCLAMCore.add( "Base" )
    libCLAMCore.add( "Data/Base" )
    libCLAMCore.add( "Defines" )

    #libCLAMCore.add( "Defines/Windows" )
    
    libCLAMCore.add( "Defines/Unix" )
    libCLAMCore.add( "Errors" )

    # These files are blackballed since they depend on
    # the Audio concrete ProcessingData
    libCLAMCore.blackBall( "AudioInPort" )
    libCLAMCore.blackBall( "AudioInPortPublisher" )
    libCLAMCore.blackBall( "AudioOutPort" )
    libCLAMCore.blackBall( "AudioOutPortPublisher" ) 
    libCLAMCore.add( "Flow/Ports" )

    libCLAMCore.add( "Flow/Networks" )
    libCLAMCore.add( "Flow/Controls" )
    libCLAMCore.add( "Processing/Base" )
    libCLAMCore.add( "Data/Base" )
    
    libCLAMCore.add( "Standard" )
    libCLAMCore.add( "Storage/Base" )
    libCLAMCore.add( "Storage/XML" )
    libCLAMCore.add( "System/Threads" )
    libCLAMCore.add( "CbLib", "externals" )

    libCLAMCore.activate('XML')
    libCLAMCore.activate('PTHREADS')

    libCLAMCore.generateFiles()
    print "Files are being generated on build/Libs..."

if __name__ == "__main__" :
    main()
else :
    print "This is an executable script"
    sys.exit(2)
