#!/usr/bin/python

import libGen, sys

def makelib() :
    libCLAMVM = libGen.LibGenerator( "VM_fl" )

    print "Generating libCLAM%s..."%libCLAMVM.libName

    libCLAMVM.activate( 'FLTK' )
    libCLAMVM.activate( 'PTHREADS' )

    # Files that do not compile
    libCLAMVM.blackBall( "TmplSlice" )
    libCLAMVM.blackBall( "SupportedToolkits" )
    libCLAMVM.blackBall( "Fl_Browsable_Playable_Audio" )
    libCLAMVM.blackBall( "Fl_Range" )
    # Deprecated stuff
    libCLAMVM.blackBall( "GLLinearSpRenderer" )
    libCLAMVM.blackBall( "GLLogPowSpRenderer" )
    libCLAMVM.blackBall( "GLLogSpRenderer" )
    libCLAMVM.blackBall( "GLPhaseSpRenderer" )
    libCLAMVM.blackBall( "GLSpectralBlock" )
    libCLAMVM.blackBall( "GLSprite" )
    libCLAMVM.blackBall( "GLTF2DRenderer" )
    libCLAMVM.blackBall( "GLWaveRenderer" )
    

    libCLAMVM.addFolder( "Visualization/Base" )
    libCLAMVM.addFolder( "Visualization/GL" )
    libCLAMVM.addFolder( "Visualization/Presentation" )
    libCLAMVM.addFolder( "Visualization/Plot" )
    libCLAMVM.addFolder( "Visualization/Presentation/Base" )
    libCLAMVM.addFolder( "Visualization/Presentation/FLTK" )
    libCLAMVM.addFolder( "Visualization/View" )
    libCLAMVM.addFolder( "Visualization/Util" )
    libCLAMVM.addFolder( "Visualization/Widget/FLTK" )
    libCLAMVM.addFile( "FLTKWrapper", "Visualization/Widget" )

    libCLAMVM.dependsOn( "Core" )
    libCLAMVM.dependsOn( "Processing" )
    libCLAMVM.dependsOn( "IO" )

    libCLAMVM.generateFiles()

    print "Files are being generated on build/Libs/%s..."%libCLAMVM.libName
