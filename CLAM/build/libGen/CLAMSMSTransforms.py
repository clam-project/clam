#!/usr/bin/python

import libGen, sys

def main() :
    libCLAMSMS = libGen.LibGenerator( "SMS" )

    libCLAMSMS.add( "Processing/Transformations/SMS" )

    libCLAMSMS.activate( 'XML' )
    libCLAMSMS.activate( 'PTHREADS' )

    libCLAMSMS.dependsOn( "Core" )
    libCLAMSMS.dependsOn( "Processing" )
    libCLAMSMS.dependsOn( "IO" )

    libCLAMSMS.generateFiles( )

if __name__ == "__main__" :
    main()
else :
    print "This is an executable script"
    sys.exit(2)
