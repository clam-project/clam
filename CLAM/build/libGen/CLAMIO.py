#!/usr/bin/python

import libGen, sys

def makelib( ) :
    libCLAMIO = libGen.LibGenerator( "IO" )

    print "Generating libCLAM%s..."%libCLAMIO.libName
    libCLAMIO.activate( 'XML' )
    libCLAMIO.activate( 'PTHREADS' )

    libCLAMIO.addFolder( "Tools/AudioFileIO" )
    libCLAMIO.addFolder( "Processing/AudioFileIO" )


    if sys.platform == "linux2" :
        libCLAMIO.blackBall( "RtAudio" )
        libCLAMIO.blackBall( "RtAAudioDevice" )
        libCLAMIO.blackBall( "foo" )
        libCLAMIO.blackBall( "SndPcm" )
        libCLAMIO.addFolder( "Tools/AudioIO/Linux" )
        libCLAMIO.activate( 'ALSA' )
    else :
        libCLAMIO.blackBall( "ALSAAudioDevice" )
        libCLAMIO.blackBall( "foo" )
        libCLAMIO.blackBall( "SndPcm" )
        libCLAMIO.addFolder( "Tools/AudioIO/RtAudio" )
        libCLAMIO.activate( 'RTAUDIO' )

    libCLAMIO.addFolder( "Tools/AudioIO" )
    libCLAMIO.addFolder( "Processing/AudioIO" )

    libCLAMIO.addFolder( "Tools/MIDIIO" )
    libCLAMIO.addFolder( "Tools/MIDIIO/File" )
    libCLAMIO.addFolder( "Tools/MIDIIO/TextFile" )
    if sys.platform == "win32" :
        libCLAMIO.addFolder( "Tools/MIDIIO/Portmidi" )
    else :
        libCLAMIO.addFolder( "Tools/MIDIIO/Linux" )
    libCLAMIO.addFolder( "Processing/MIDIIO" )

    libCLAMIO.addFolder( "Tools/SDIF" )
    libCLAMIO.addFolder( "Processing/SDIFIO" )
    
    libCLAMIO.dependsOn( "Core" )

    libCLAMIO.generateFiles( )

    print "Files are being generated on build/Libs/%s..."%libCLAMIO.libName
    
if __name__ == "__main__" :
    makelib()


    
