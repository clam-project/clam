#!/usr/bin/python

import CLAMcore
import CLAMProcessing
import CLAMIO
import CLAMSMS
import CLAMVM
import libGen

def main() :

    libGen.resetHeaderDB()
    CLAMcore.makelib()
    CLAMProcessing.makelib()
    CLAMIO.makelib()
    CLAMSMS.makelib()
    CLAMVM.makelib()

if __name__ == "__main__" :
    main()
else :
    print "This is an executable script"
    sys.exit(2)
