import os, re, shelve

class LibGenerator :

    libName = None
    settingsTmpl = ""
    sourceFilenames = []
    headers = dict()
    libTopPath = "./"
    libGenTop = "../.."
    extraIncludes = []
    extraLibPaths = []
    extraLibs = []

    isCxxSourceRE = re.compile( r"[^\.#]+\.cxx\Z" )
    isCSourceRE = re.compile(r"[^\.#]+\.c\Z" )
    isCHeaderRE = re.compile( r"[^\.#].+\.hxx\Z" )
    isCxxHeaderRE = re.compile( r"[^\.#].+\.h\Z" )

    setlibName = re.compile( r"@libname@" )
    setXMLRE = re.compile( r"@XML@" )
    setALSARE = re.compile( r"@ALSA@" )
    setFFTWRE = re.compile( r"@FFTW@" )
    setFLTKRE = re.compile( r"@FLTK@" )
    setPthreadsRE = re.compile( r"@PTHREADS@" )
    setRtAudioRE = re.compile( r"@RTAUDIO@" )
    setSourcesListRE = re.compile( r"@sourceslist@" )
    setExtraIncsRE = re.compile( r"@extraincludes@" )
    setExtraLibNames = re.compile( r"@extralibs@" )
    setExtraLibPaths = re.compile( r"@extralibpaths@" )
    removeAllCmds = re.compile( r"@.+@" )

    variables = dict()

    hdrNormRE = re.compile( r"#include \"(?P<hdr>.+)\"" )

    def __init__( self, libName ) :
        self.libName = libName
        self.libfilename = "CLAM"+libName

        tmplFile = file( "settings.tpl.cfg", "r" )
        self.settingsTmpl = tmplFile.read()
        tmplFile.close()

        [self.libTopPath] = re.search( r"TOP = (.+)", self.settingsTmpl ).groups()
        print "TOP=%s"%self.libTopPath

        self.variables={
            'ALSA':"0",
            'FFTW':"0",
            'FLTK':"0",
            'RTAUDIO':"0",
            'PTHREADS':"0",
            'XML':"0"
            }

    def dependsOn( self, CLAMlibname ) :
        self.extraLibs.append( "CLAM%s"%CLAMlibname )
        self.extraLibPaths.append( "../%s"%CLAMlibname )

    def add( self, folder, rootFolder="src" ) :
        if rootFolder != "src" :
            self.extraIncludes.append( "$(TOP)/%s"%(rootFolder,) )
        basePath = "%s/%s/%s"%(self.libGenTop, rootFolder, folder)
        if not os.path.exists( basePath ) or not os.path.isdir( basePath ) :
            raise TypeError, "%s is not a CLAM src folder !!"%(basePath,)

        folderContents = os.listdir( basePath )

        headerDB = shelve.open( "CLAM_Headers", "c" )

        for item in folderContents :
            
            fullPath = basePath + "/" + item
            
            if os.path.isdir( fullPath ) : continue

            if ( self.isCSourceRE.search( item ) != None or
                 self.isCxxSourceRE.search( item ) != None ) :
                self.sourceFilenames.append( "$(TOP)/src/"+folder+"/"+item)
                continue

            if ( self.isCxxHeaderRE.search( item ) != None or
                 self.isCHeaderRE.search( item ) != None ) :
                if not basePath in self.headers.keys() :
                    self.headers[ basePath ] = [item]
                else :
                    self.headers[ basePath ].append( item )
                headerDB[ item ] = self.libName

        headerDB.sync()
        headerDB.close()
        
    def activate( self, key ) :
        self.variables[key] = "1"


    def deactivate( self, key ) :
        self.variables[key] = "0"

    def generateFiles( self ) :
        libFilesPath = self.libGenTop+"/build/Libs/"+self.libName
        if not os.path.exists( libFilesPath  ) :
            os.mkdir( libFilesPath )

        libSettings = self.settingsTmpl

        libSettings = self.setlibName.sub( self.libfilename, libSettings )
        libSettings = self.setXMLRE.sub( self.variables['XML'], libSettings )
        libSettings = self.setALSARE.sub( self.variables['ALSA'], libSettings )
        libSettings = self.setFFTWRE.sub( self.variables['FFTW'], libSettings )
        libSettings = self.setFLTKRE.sub( self.variables['FLTK'], libSettings )
        libSettings = self.setRtAudioRE.sub( self.variables['RTAUDIO'], libSettings )
        libSettings = self.setPthreadsRE.sub( self.variables['PTHREADS'], libSettings )

        if len( self.extraLibPaths ) > 0 :
            extraLibPathsStr = " ".join( self.extraLibPaths )
            libSettings = self.setExtraLibPaths.sub( extraLibPathsStr, libSettings )

        if len( self.extraLibs ) > 0 :
            extraLibsStr = " ".join( self.extraLibs )
            libSettings = self.setExtraLibNames.sub( extraLibsStr, libSettings )

        sourcesStr = "\\\n\t".join( self.sourceFilenames )

        libSettings = self.setSourcesListRE.sub( sourcesStr, libSettings )

        if len( self.extraIncludes ) > 0 :
            incDirsStr = "\\\n\t".join( self.extraIncludes )
            libSettings = self.setExtraIncsRE.sub( incDirsStr, libSettings )

        libSettings = self.removeAllCmds.sub( "", libSettings )

        libSettingsFile = file( "%s/%s"%(libFilesPath,"settings.cfg"), "w" )

        libSettingsFile.write( libSettings )

        libSettingsFile.close()

        # And now the includes
        libIncPath = "%s/include/CLAM/%s"%( libFilesPath, self.libName )
        if os.path.exists(  libIncPath ) :
            for filename in os.listdir( libIncPath ) :
                os.remove( "%s/%s"%(libIncPath,filename) )
            os.removedirs( libIncPath )

        os.makedirs( libIncPath )

        # and now we standardize the include directives

        for basePath in self.headers.keys() :
            for header in self.headers[basePath] :
                pureHeader = self.purifyHeader( basePath, header )
                newHeader = file( libIncPath +"/"+header, "w" )
                newHeader.write( pureHeader )
                newHeader.close()

    def purifyHeader( self, basePath, header ) :
        pureHeader = []

        headerDB = shelve.open( "CLAM_Headers", "r" )

        hdrFilename = "%s/%s"%(basePath,header)
        hdrFile = file( hdrFilename, "r" )

        for line in hdrFile :
            match = self.hdrNormRE.search( line )
            if match != None :
                try:
                    module = headerDB[match.groupdict()['hdr']]
                except KeyError :
                    raise TypeError, "Error processing %s : the header %s is not on the DB"%(hdrFilename,match.groupdict()['hdr'] )
                pureHeader.append( self.hdrNormRE.sub( "#include <CLAM/%s/\g<hdr>>"%module, line ) )
            else :
                pureHeader.append( line )
        
        hdrFile.close()

        headerDB.close()

        return "".join(pureHeader)
        

        


        

        
