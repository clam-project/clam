#!/usr/bin/python

import libGen, sys

def main( ) :
    libCLAMIO = libGen.LibGenerator( "IO" )

    libCLAMIO.activate( 'XML' )
    libCLAMIO.activate( 'PTHREADS' )

    libCLAMIO.add( "Tools/AudioFileIO" )
    libCLAMIO.add( "Processing/AudioFileIO" )


    if sys.platform == "linux2" :
        libCLAMIO.blackBall( "RtAudio" )
        libCLAMIO.blackBall( "RtAAudioDevice" )
        libCLAMIO.blackBall( "foo" )
        libCLAMIO.blackBall( "SndPcm" )
        libCLAMIO.add( "Tools/AudioIO/Linux" )
        libCLAMIO.activate( 'ALSA' )
    else :
        libCLAMIO.blackBall( "ALSAAudioDevice" )
        libCLAMIO.blackBall( "foo" )
        libCLAMIO.blackBall( "SndPcm" )
        libCLAMIO.add( "Tools/AudioIO/RtAudio" )
        libCLAMIO.activate( 'RTAUDIO' )

    libCLAMIO.add( "Tools/AudioIO" )
    libCLAMIO.add( "Processing/AudioIO" )

    libCLAMIO.add( "Tools/MIDIIO" )
    libCLAMIO.add( "Tools/MIDIIO/File" )
    libCLAMIO.add( "Tools/MIDIIO/TextFile" )
    if sys.platform == "win32" :
        libCLAMIO.add( "Tools/MIDIIO/Portmidi" )
    else :
        libCLAMIO.add( "Tools/MIDIIO/Linux" )
    libCLAMIO.add( "Processing/MIDIIO" )

    libCLAMIO.add( "Tools/SDIF" )
    libCLAMIO.add( "Processing/SDIFIO" )
    
    libCLAMIO.dependsOn( "Core" )

    libCLAMIO.generateFiles( )
    
if __name__ == "__main__" :
    main()
else :
    print "This is an executable script"
    sys.exit(2)


    
