#!/usr/bin/python

import libGen, sys

def main() :
    libCLAMProcessing = libGen.LibGenerator( "Processing" )

    libCLAMProcessing.blackBall( "OnsetDetector" )

    libCLAMProcessing.add( "Data/BasicProcessing" )
    libCLAMProcessing.add( "Data/Descriptors" )
    libCLAMProcessing.add( "Data/Descriptors/Melody" )

    libCLAMProcessing.add( "Processing/Analysis" )
    libCLAMProcessing.add( "Processing/ArithOps" )
    libCLAMProcessing.add( "Processing/Controls" )
    libCLAMProcessing.add( "Processing/Generators" )
    libCLAMProcessing.add( "Processing/Plugins" )
    libCLAMProcessing.add( "Processing/Synthesis" )
    libCLAMProcessing.add( "Processing/Transformations" )

    # These are already on Core
    libCLAMProcessing.blackBall( "InPort" )
    libCLAMProcessing.blackBall( "InPortPublisher" )
    libCLAMProcessing.blackBall( "OutPort" )
    libCLAMProcessing.blackBall( "OutPortPublisher" )
    libCLAMProcessing.blackBall( "PhantomBuffer" )
    libCLAMProcessing.blackBall( "ReadingRegion" )
    libCLAMProcessing.blackBall( "Region" )
    libCLAMProcessing.blackBall( "StreamImpl" )
    libCLAMProcessing.blackBall( "WritingRegion" )
    libCLAMProcessing.add( "Flow/Ports")
    libCLAMProcessing.add( "numrec", "externals")

    libCLAMProcessing.activate( 'XML' )
    libCLAMProcessing.activate( 'PTHREADS' )
    libCLAMProcessing.activate( 'FFTW' )

    libCLAMProcessing.dependsOn( "Core" )

    libCLAMProcessing.generateFiles()
    print "Files are being generated on build/Libs..."

if __name__ == "__main__" :
    main()
else :
    print "This is an executable script"
    sys.exit(2)
