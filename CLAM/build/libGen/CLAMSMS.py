#!/usr/bin/python

import SettingsGen, sys, os, AutoconfTools

def makeAutoconf( outPath ) :
    outPath = "../Libs/%s"%"SMS"

    if not os.path.exists( outPath ) :
        os.makedirs( outPath )

    script = AutoconfTools.AutoconfScript()

    xml = AutoconfTools.Feature( "xml" )
    script.addFeature( xml )

    double = AutoconfTools.Feature( "double", False )
    script.addFeature( double )

    checks = AutoconfTools.Feature( "checks" )
    script.addFeature( checks )

    releaseAsserts = AutoconfTools.Feature( "release_asserts", False )
    script.addFeature( releaseAsserts )
  
    script.commitToFile( outPath )
   
    AutoconfTools.copySupportFiles( outPath )


def makeSettings( outPath) :
    if not os.path.exists( outPath ) :
        os.makedirs( outPath )

    libCLAMSMS = SettingsGen.LibGenerator( "SMS" )

    print "Generating libCLAM%s..."%libCLAMSMS.libName

    libCLAMSMS.addFile( "SMSAnalysisCore", "Processing/Analysis" )
    libCLAMSMS.addFile( "SMSAnalysisConfig","Processing/Analysis")
    libCLAMSMS.addFile( "SMSAnalysis", "Processing/Analysis" )
    libCLAMSMS.addFile( "SMSSynthesisConfig", "Processing/Synthesis" )
    libCLAMSMS.addFile( "SMSSynthesis", "Processing/Synthesis" )
    libCLAMSMS.addFolder( "Processing/Transformations/SMS" )

    libCLAMSMS.dependsOn( "Core" )
    libCLAMSMS.dependsOn( "Processing" )
    libCLAMSMS.dependsOn( "IO" )

    libCLAMSMS.generateFiles( outPath )

    print "Files are being generated on %s/%s..."%(outPath,libCLAMSMS.libName)

if __name__ == "__main__" :
    makeSettings("../Libs")
